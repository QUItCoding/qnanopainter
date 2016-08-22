/**********************************************************
** Copyright (c) 2015 QUIt Coding <info@quitcoding.com>
**
** This software is provided 'as-is', without any express or implied
** warranty.  In no event will the authors be held liable for any damages
** arising from the use of this software.
**
** Permission is granted to anyone to use this software for any purpose,
** including commercial applications, and to alter it and redistribute it
** freely, subject to the following restrictions:
**
** 1. The origin of this software must not be misrepresented; you must not
**    claim that you wrote the original software. If you use this software
**    in a product, an acknowledgment in the product documentation would be
**    appreciated but is not required.
** 2. Altered source versions must be plainly marked as such, and must not be
**    misrepresented as being the original software.
** 3. This notice may not be removed or altered from any source distribution.
**
**********************************************************/

#include "qnanopainter.h"

#if defined(QNANO_QT_GL_INCLUDE)
// Let the Qt include OpenGL headers

#define GL_GLEXT_PROTOTYPES
#include <QtGui/qopengl.h>

#else
// Manually include OpenGL headers

#if defined(Q_OS_IOS)
#include <OpenGLES/ES2/gl.h>
#elif defined(Q_OS_ANDROID)
#include <GLES2/gl2.h>
#elif defined(Q_OS_OSX)
#include <OpenGL/gl.h>
#elif defined(Q_OS_LINUX)
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#else
#include <GLES2/gl2.h>
#endif

#endif

#ifdef QT_OPENGL_ES_2
#define NANOVG_GLES2_IMPLEMENTATION
#else
#define NANOVG_GL2_IMPLEMENTATION
#endif

#include "nanovg/nanovg.h"
#include "nanovg/nanovg_gl.h"
#include <QScreen>
#include <QGuiApplication>

/*!
    \class QNanoPainter
    \brief The QNanoPainter class provides an API for painting into QNanoQuickItem.
    \inmodule QNanoPainter

    TODO: Write more documentation here.
*/

/*!
    \enum QNanoPainter::PathWinding

    PathWinding is used to specify the direction of path drawing.
    This direction is used to determine if a subpath is solid or hole in the path.

    \value WINDING_CCW (default) Clockwise winding for solid shapes.

    \value WINDING_CW Counter-clockwise winding for holes.

    \sa setPathWinding()
*/

/*!
    \enum QNanoPainter::LineCap

    LineCap is used to defining how the end of the line (cap) is drawn.

    \value CAP_BUTT (default) Square line ending that does not cover the end point of the line.

    \value CAP_ROUND Round line ending.

    \value CAP_SQUARE Square line ending that covers the end point and extends beyond it by half the line width.

    \sa setLineCap()
*/

/*!
    \enum QNanoPainter::LineJoin

    LineJoin is used to defining how the joins between two connected lines are drawn.

    \value JOIN_ROUND Circular arc between the two lines is filled.

    \value JOIN_BEVEL The triangular notch between the two lines is filled.

    \value JOIN_MITER (default) The outer edges of the lines are extended to meet at an angle, and this area is filled.

    \sa setLineJoin(), setMiterLimit()
*/

/*!
    \enum QNanoPainter::TextAlign

    TextAlign is used to defining how the text is aligned horizontally.

    \value ALIGN_LEFT (default) Align the left side of the text horizontally to the specified position.

    \value ALIGN_CENTER Align the center of the text horizontally to the specified position.

    \value ALIGN_RIGHT Align the right side of the text horizontally to the specified position.

    \sa setTextAlign(), fillText()
*/

/*!
    \enum QNanoPainter::TextBaseline

    TextBaseline is used to defining how the text is aligned (baselined) vertically.

    \value BASELINE_TOP Align the top of the text vertically to the specified position.

    \value BASELINE_MIDDLE Align the middle of the text vertically to the specified position.

    \value BASELINE_BOTTOM Align the bottom of the text vertically to the specified position.

    \value BASELINE_ALPHABETIC (default) Align the baseline of the text vertically to the specified position.

    \sa setTextBaseline(), fillText()
*/

/*!
    \enum QNanoPainter::PixelAlign

    PixelAlign is used to defining if painting or text should be aligned to pixels.

    \value PIXEL_ALIGN_NONE (default) Don't do any alignment.

    \value PIXEL_ALIGN_HALF Align to half pixels. This will make painting appear sharp when line width is odd.

    \value PIXEL_ALIGN_FULL Align to full pixels. This will make painting appear sharp when line width is even.

    \sa setPixelAlign(), setPixelAlignText()
*/


/*!
    Constructs a painter.
*/

QNanoPainter::QNanoPainter()
    : m_nvgContext(nullptr)
    , m_textAlign(QNanoPainter::ALIGN_LEFT)
    , m_textBaseline(QNanoPainter::BASELINE_ALPHABETIC)
    , m_devicePixelRatio(1.0)
    , m_fontSet(false)
    , m_pixelAlign(QNanoPainter::PIXEL_ALIGN_NONE)
    , m_pixelAlignText(QNanoPainter::PIXEL_ALIGN_NONE)
{
    // Initialize NanoVG for correct GL version
    // TODO: Allow to enable/disable NVG_DEBUG, possibly some own general debug define
#ifdef QT_OPENGL_ES_2
    m_nvgContext = nvgCreateGLES2(NVG_ANTIALIAS | NVG_DEBUG);
#else
    m_nvgContext = nvgCreateGL2(NVG_ANTIALIAS | NVG_DEBUG);
#endif

    Q_ASSERT_X(m_nvgContext, "QNanoPainter::QNanoPainter", "Could not init nanovg!");


}

/*!
    Destroys the painter.
*/

QNanoPainter::~QNanoPainter()
{
    if (m_nvgContext) {
#ifdef QT_OPENGL_ES_2
    nvgDeleteGLES2(m_nvgContext);
#else
    nvgDeleteGL2(m_nvgContext);
#endif
    }

    qDeleteAll(m_dataCache);
}

// *** State Handling ***

// QNanoPainter state controls how the painting paths will be rendered.
// The state contains transform, fill and stroke styles, text and font styles,
// and scissor clipping.

/*!
    \fn void QNanoPainter::save()

    Pushes and saves the current render state into a state stack.
    A matching restore() must be used to restore the state.

    \sa restore()
*/

void QNanoPainter::save()
{
    nvgSave(nvgCtx());
}

/*!
    \fn void QNanoPainter::restore()

    Pops and restores current render state.
    So previously saved state will be restored.

    \sa save()
*/

void QNanoPainter::restore()
{
    nvgRestore(nvgCtx());
}

/*!
    \fn void QNanoPainter::reset()

    Resets the current painter state to default values.
    Does not affect the render state stack.

    \sa save(), restore()
*/

void QNanoPainter::reset()
{
    m_textAlign = QNanoPainter::ALIGN_LEFT;
    m_textBaseline = QNanoPainter::BASELINE_ALPHABETIC;
    m_fontSet = false;
    m_pixelAlign = QNanoPainter::PIXEL_ALIGN_NONE;
    m_pixelAlignText = QNanoPainter::PIXEL_ALIGN_NONE;
    nvgReset(nvgCtx());
}

// *** Render styles ***

// Fill and stroke render style can be either a solid color or a brush which is a gradient or a pattern.
// Solid color is QNanoColor, brush can be QNanoLinearGradient etc. or QNanoImagePattern.
//
// Current render style can be saved and restored using save() and restore().

/*!
    \fn void QNanoPainter::setStrokeStyle(const QNanoColor &color)

    Sets the stroke style to a solid \a color.
*/

void QNanoPainter::setStrokeStyle(const QNanoColor &color)
{
    nvgStrokeColor(nvgCtx(), color.nvgColor());
}

/*!
    \fn void QNanoPainter::setStrokeStyle(const QNanoBrush &brush)
    \overload

    Sets the stroke style to \a brush. Brush can be some gradient or QNanoImagePattern.
*/

void QNanoPainter::setStrokeStyle(const QNanoBrush &brush)
{
    // If brush is QNanoImagePattern set its painter
    QNanoImagePattern *ip = dynamic_cast<QNanoImagePattern*>(const_cast<QNanoBrush*>(&brush));
    if (ip && ip->m_image) {
        ip->m_image->setParentPainter(this);
    }
    QNanoBrush* b = const_cast<QNanoBrush*>(&brush);
    if (b) {
        b->setParentPainter(this);
    }
    nvgStrokePaint(nvgCtx(), brush.nvgPaint(nvgCtx()));
}

/*!
    \fn void QNanoPainter::setFillStyle(const QNanoColor &color)

    Sets the fill style to a solid \a color.
*/

void QNanoPainter::setFillStyle(const QNanoColor &color)
{
    nvgFillColor(nvgCtx(), color.nvgColor());
}

/*!
    \fn void QNanoPainter::setFillStyle(const QNanoBrush &brush)
    \overload

    Sets the fill style to \a brush. Brush can be some gradient or QNanoImagePattern.
*/

void QNanoPainter::setFillStyle(const QNanoBrush &brush)
{
    // If brush is QNanoImagePattern set its painter
    QNanoImagePattern *ip = dynamic_cast<QNanoImagePattern*>(const_cast<QNanoBrush*>(&brush));
    if (ip && ip->m_image) {
        ip->m_image->setParentPainter(this);
    }
    QNanoBrush* b = const_cast<QNanoBrush*>(&brush);
    if (b) {
        b->setParentPainter(this);
    }
    nvgFillPaint(nvgCtx(), brush.nvgPaint(nvgCtx()));
}

/*!
    \fn void QNanoPainter::setMiterLimit(float limit)

    Sets the miter limit to \a limit. Miter limit controls when a sharp corner
    is beveled. When corner degree between lines in a path is less than this limit,
    LineJoin will be applied between the lines.
    The default limit is 10.0.

    \sa setLineJoin()
*/

void QNanoPainter::setMiterLimit(float limit)
{
    nvgMiterLimit(nvgCtx(), limit);
}

/*!
    \fn void QNanoPainter::setLineWidth(float width)

    Sets the line width of stoke to \a width in pixels.
    The default line width is 1.0.

    \sa stroke()
*/

void QNanoPainter::setLineWidth(float width)
{
    nvgStrokeWidth(nvgCtx(), width);
}

/*!
    \fn void QNanoPainter::setLineCap(LineCap cap)

    Sets the end of the line of stoke to \a cap.
    The default line cap is CAP_BUTT.
*/

void QNanoPainter::setLineCap(LineCap cap)
{
    nvgLineCap(nvgCtx(), cap);
}

/*!
    \fn void QNanoPainter::setLineJoin(LineJoin join)

    Sets the line join of stoke to \a join. This join is used when drawing
    sharper corners than miter limit.
    The default line join is JOIN_MITER.

    \sa setMiterLimit()

*/

void QNanoPainter::setLineJoin(LineJoin join)
{
    nvgLineJoin(nvgCtx(), join);
}

/*!
    \fn void QNanoPainter::setGlobalAlpha(float alpha)

    Sets the global alpha (transparency) value to \a alpha. This alpha value is
    applied to all rendered shapes. Already transparent paths will get
    proportionally more transparent as well.
    Alpha should be between 0.0 (fully transparent) and 1.0 (fully opaque).
    By default alpha is 1.0.
*/

void QNanoPainter::setGlobalAlpha(float alpha)
{
    nvgGlobalAlpha(nvgCtx(), alpha);
}


// *** Transforms ***

// The paths, gradients, patterns and scissor region are transformed by an transformation
// matrix at the time when they are passed to the API.
// The current transformation matrix is a affine matrix:
//   [sx kx tx]
//   [ky sy ty]
//   [ 0  0  1]
// Where: sx,sy define scaling, kx,ky skewing, and tx,ty translation.
// The last row is assumed to be 0,0,1 and is not stored.
//
// Current coordinate system (transformation) can be saved and restored using save() and restore().

/*!
    \fn void QNanoPainter::resetTransform()

    Resets current transform to a identity matrix.
*/

void QNanoPainter::resetTransform()
{
    nvgResetTransform(nvgCtx());
}

/*!
    \fn void QNanoPainter::transform(const QTransform &transform)

    Premultiplies current coordinate system by specified \a transform.
*/

void QNanoPainter::transform(const QTransform &transform)
{
    nvgTransform(nvgCtx()
                 , transform.m11(), transform.m12()
                 , transform.m21(), transform.m22()
                 , transform.m31(), transform.m32());
}

/*!
    \fn void QNanoPainter::translate(float x, float y)

    Translates current coordinate system by \a x and \a y.
*/

void QNanoPainter::translate(float x, float y)
{
    nvgTranslate(nvgCtx(), x, y);
}

/*!
    \fn void QNanoPainter::translate(const QPointF &point)

    Translates current coordinate system by \a point.
*/

void QNanoPainter::translate(const QPointF &point)
{
    translate(point.x(), point.y());
}

/*!
    \fn void QNanoPainter::rotate(float angle)

    Rotates current coordinate system by \a angle. Angle is specified in radians.
*/

void QNanoPainter::rotate(float angle)
{
    nvgRotate(nvgCtx(), angle);
}

/*!
    \fn void QNanoPainter::skewX(float angle)

    Skews the current coordinate system along X axis by \a angle. Angle is specifid in radians.
*/

void QNanoPainter::skewX(float angle)
{
    nvgSkewX(nvgCtx(), angle);
}

/*!
    \fn void QNanoPainter::skewY(float angle)

    Skews the current coordinate system along Y axis by \a angle. Angle is specifid in radians.
*/

void QNanoPainter::skewY(float angle)
{
    nvgSkewY(nvgCtx(), angle);
}

/*!
    \fn void QNanoPainter::scale(float scale)

    Scales the current coordinat system by \a scale. Both x and y coordinates
    are scaled evenly.
*/

void QNanoPainter::scale(float scale)
{
    nvgScale(nvgCtx(), scale, scale);
}

/*!
    \fn void QNanoPainter::scale(float scaleX, float scaleY)

    Scales the current coordinat system by \a scaleX and \a scaleY.
*/

void QNanoPainter::scale(float scaleX, float scaleY)
{
    nvgScale(nvgCtx(), scaleX, scaleY);
}

/*!
    \fn const QTransform QNanoPainter::currentTransform() const

    Returns the current transform.
*/

const QTransform QNanoPainter::currentTransform() const
{
    float *xform = new float[6];
    nvgCurrentTransform(nvgCtx(), xform);
    QTransform t(xform[0], xform[1], xform[2], xform[3], xform[4], xform[5]);
    delete [] xform;
    return t;
}

// *** Scissoring ***

// Scissoring allows you to clip the rendering into a rectangle. This is useful for varius
// user interface cases like rendering a text edit or a timeline.

/*!
    \fn void QNanoPainter::setClipRect(float x, float y, float width, float height)

    Sets the current scissor rectangle to (\a x, \a y, \a width, \a height).
    The scissor rectangle is transformed by the current transform.
*/

void QNanoPainter::setClipRect(float x, float y, float width, float height)
{
    _checkAlignPixelsAdjust(&x, &y);
    _checkAlignPixels(&width, &height);
    nvgScissor(nvgCtx(), x, y, width, height);
}

/*!
    \fn void QNanoPainter::setClipRect(const QRectF &rect)

    Sets the current scissor rectangle to \a rect.
    The scissor rectangle is transformed by the current transform.
    \sa resetClipping()
*/

void QNanoPainter::setClipRect(const QRectF &rect)
{
    setClipRect(rect.x(), rect.y(), rect.width(), rect.height());
}

/*!
    \fn void QNanoPainter::resetClipping()

    Resets and disables clipping.
    \sa setClipRect()
*/

void QNanoPainter::resetClipping()
{
    nvgResetScissor(nvgCtx());
}

//  *** Paths ***

// All shape drawing in QNanoPainter belong to paths. Drawing a path starts with beginPath()
// which clears the currently defined path. Then draw paths using shapes such as lines, rects,
// circles etc. And finally stroke and/or fill the path using current styles.
//
// QNanoPainter uses even-odd fill rule to draw shapes. To draw holes into a shape you can
// set pathWinding correspondingly.
//
// Current transtormation affects how paths are drawn.

/*!
    \fn void QNanoPainter::beginPath()

    Begins drawing a new path while clearing the current path.
*/

void QNanoPainter::beginPath()
{
    nvgBeginPath(nvgCtx());
}

/*!
    \fn void QNanoPainter::closePath()

    Closes the current sub-path with a line segment.
    This is equivalent to lineTo([starting point]) as the last path element.
*/

void QNanoPainter::closePath()
{
    nvgClosePath(nvgCtx());
}

/*!
    \fn void QNanoPainter::moveTo(float x, float y)

    Starts new sub-path with ( \a x, \a y) as first point.
*/

void QNanoPainter::moveTo(float x, float y)
{
    _checkAlignPixelsAdjust(&x, &y);
    nvgMoveTo(nvgCtx(), x, y);
}

/*!
    \fn void QNanoPainter::moveTo(const QPointF &point)
    \overload

    Starts new sub-path with \a point as first point.
*/

void QNanoPainter::moveTo(const QPointF &point)
{
    moveTo(point.x(), point.y());
}

/*!
    \fn void QNanoPainter::lineTo(float x, float y)

    Adds line segment from the last point in the path to the ( \a x, \a y) point.
*/

void QNanoPainter::lineTo(float x, float y)
{
    _checkAlignPixelsAdjust(&x, &y);
    nvgLineTo(nvgCtx(), x, y);
}

/*!
    \fn void QNanoPainter::lineTo(const QPointF &point)
    \overload

    Adds line segment from the last point in the path to the \a point.
*/

void QNanoPainter::lineTo(const QPointF &point)
{
    lineTo(point.x(), point.y());
}

/*!
    \fn void QNanoPainter::bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y)

    Adds cubic bezier segment from last point in the path via two
    control points (\a c1x, \a c1y and \a c2x, \a c2y) to the specified point (\a x, \a y).
*/

void QNanoPainter::bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y)
{
    _checkAlignPixelsAdjust(&x, &y);
    _checkAlignPixelsAdjust(&c1x, &c1y, &c2x, &c2y);
    nvgBezierTo(nvgCtx(), c1x, c1y, c2x, c2y, x, y);
}

/*!
    \fn void QNanoPainter::bezierTo(const QPointF &controlPoint1, const QPointF &controlPoint2, const QPointF &endPoint)
    \overload

    Adds cubic bezier segment from last point in the path via two
    control points (\a controlPoint1 and \a controlPoint2) to the specified point \a endPoint.
*/

void QNanoPainter::bezierTo(const QPointF &controlPoint1, const QPointF &controlPoint2, const QPointF &endPoint)
{
    bezierTo(controlPoint1.x(), controlPoint1.y(), controlPoint2.x(), controlPoint2.y(), endPoint.x(), endPoint.y());
}

/*!
    \fn void QNanoPainter::quadTo(float cx, float cy, float x, float y)

    Adds quadratic bezier segment from last point in the path via
    a control point (\a cx, \a cy) to the specified point (\a x, \a y).
*/

void QNanoPainter::quadTo(float cx, float cy, float x, float y)
{
    _checkAlignPixelsAdjust(&cx, &cy, &x, &y);
    nvgQuadTo(nvgCtx(), cx, cy, x, y);
}

/*!
    \fn void QNanoPainter::quadTo(const QPointF &controlPoint, const QPointF &endPoint)
    \overload

    Adds quadratic bezier segment from last point in the path via
    a \a controlPoint to the specified \a endPoint.
*/

void QNanoPainter::quadTo(const QPointF &controlPoint, const QPointF &endPoint)
{
    quadTo(controlPoint.x(), controlPoint.y(), endPoint.x(), endPoint.y());
}

/*!
    \fn void QNanoPainter::arcTo(float c1x, float c1y, float c2x, float c2y, float radius)

    Adds an arc segment at the corner defined by the last path point,
    and two specified points (\a c1x, \a c1y and \a c2x, \a c2y) with \a radius.
*/

void QNanoPainter::arcTo(float c1x, float c1y, float c2x, float c2y, float radius)
{
    _checkAlignPixelsAdjust(&c1x, &c1y, &c2x, &c2y);
    nvgArcTo(nvgCtx(), c1x, c1y, c2x, c2y, radius);
}

/*!
    \fn void QNanoPainter::arcTo(const QPointF &controlPoint1, const QPointF &controlPoint2, float radius)
    \overload

    Adds an arc segment at the corner defined by the last path point,
    and two specified points (\a controlPoint1 and \a controlPoint2) with \a radius.
*/

void QNanoPainter::arcTo(const QPointF &controlPoint1, const QPointF &controlPoint2, float radius)
{
    arcTo(controlPoint1.x(), controlPoint1.y(), controlPoint2.x(), controlPoint2.y(), radius);
}

/*!
    \fn void QNanoPainter::arc(float centerX, float centerY, float radius, float a0, float a1, PathWinding direction)

    Creates new circle arc shaped sub-path. The arc center is at \a centerX, \a centerY,
    with \a radius, and the arc is drawn from angle \a a0 to \a a1,
    and swept in \a direction (WINDING_CW or WINDING_CCW).
    Angles are specified in radians.
*/

void QNanoPainter::arc(float centerX, float centerY, float radius, float a0, float a1, PathWinding direction)
{
    _checkAlignPixelsAdjust(&centerX, &centerY);
    nvgArc(nvgCtx(), centerX, centerY, radius, a0, a1, direction);
}

/*!
    \fn void QNanoPainter::arc(const QPointF &centerPoint, float radius, float a0, float a1, PathWinding direction)
    \overload

    Creates new circle arc shaped sub-path. The arc center is at \a centerPoint,
    with \a radius, and the arc is drawn from angle \a a0 to \a a1,
    and swept in \a direction (WINDING_CW or WINDING_CCW).
    Angles are specified in radians.
*/

void QNanoPainter::arc(const QPointF &centerPoint, float radius, float a0, float a1, PathWinding direction)
{
    arc(centerPoint.x(), centerPoint.y(), radius, a0, a1, direction);
}

/*!
    \fn void QNanoPainter::rect(float x, float y, float width, float height)

    Creates new rectangle shaped sub-path in position \a x, \a y with
    size \a width, \a height.
*/

void QNanoPainter::rect(float x, float y, float width, float height)
{
    _checkAlignPixelsAdjust(&x, &y);
    _checkAlignPixels(&width, &height);
    nvgRect(nvgCtx(), x, y, width, height);
}

/*!
    \fn void QNanoPainter::rect(const QRectF &rect)
    \overload

    Creates new rectangle shaped sub-path at \a rect.
    This is an overloaded method using QRectF.
*/

void QNanoPainter::rect(const QRectF &rect)
{
    this->rect(rect.x(), rect.y(), rect.width(), rect.height());
}

/*!
    \fn void QNanoPainter::roundedRect(float x, float y, float width, float height, float radius)

    Creates new rounded rectangle shaped sub-path in position \a x, \a y with
    size \a width, \a height. Corners rounding will be \a radius.
*/

void QNanoPainter::roundedRect(float x, float y, float width, float height, float radius)
{
    _checkAlignPixelsAdjust(&x, &y);
    _checkAlignPixels(&width, &height);
    nvgRoundedRect(nvgCtx(), x, y, width, height, radius);
}

/*!
    \fn void QNanoPainter::roundedRect(const QRectF &rect, float radius)
    \overload

    Creates new rounded rectangle shaped sub-path at \a rect with \a radius corners.
    This is an overloaded method using QRectF.
*/

void QNanoPainter::roundedRect(const QRectF &rect, float radius)
{
    roundedRect(rect.x(), rect.y(), rect.width(), rect.height(), radius);
}

/*!
    \fn void QNanoPainter::ellipse(float centerX, float centerY, float radiusX, float radiusY)

    Creates new ellipse shaped sub-path into ( \a centerX, \a centerY) with \a radiusX and \a radiusY.
*/

void QNanoPainter::ellipse(float centerX, float centerY, float radiusX, float radiusY)
{
    _checkAlignPixelsAdjust(&centerX, &centerX);
    _checkAlignPixels(&radiusX, &radiusY);
    nvgEllipse(nvgCtx(), centerX, centerY, radiusX, radiusY);
}

/*!
    \fn void QNanoPainter::ellipse(const QRectF &rect)
    \overload

    Creates new ellipse shaped sub-path into \a rect.
*/

void QNanoPainter::ellipse(const QRectF &rect)
{
    ellipse(rect.x()+rect.width()/2, rect.y()+rect.height()/2, rect.width()/2, rect.height()/2);
}

/*!
    \fn void QNanoPainter::circle(float centerX, float centerY, float radius)

    Creates new circle shaped sub-path into ( \a centerX, \a centerY) with \a radius.
*/

void QNanoPainter::circle(float centerX, float centerY, float radius)
{
    _checkAlignPixelsAdjust(&centerX, &centerX);
    nvgCircle(nvgCtx(), centerX, centerY, radius);
}

/*!
    \fn void QNanoPainter::circle(const QPointF &centerPoint, float radius)

    Creates new circle shaped sub-path into \a centerPoint with \a radius.
*/

void QNanoPainter::circle(const QPointF &centerPoint, float radius)
{
    circle(centerPoint.x(), centerPoint.y(), radius);
}

/*!
    \fn void QNanoPainter::setPathWinding(PathWinding winding)

    Sets the current sub-path \a winding to either WINDING_CCW (default) or WINDING_CW.
    WINDING_CCW draws solid subpaths while WINDING_CW draws holes.
*/

void QNanoPainter::setPathWinding(PathWinding winding)
{
    nvgPathWinding(nvgCtx(), winding);
}

/*!
    \fn void QNanoPainter::beginSolidSubPath()

    Start a solid subpath. This is equivalent to setPathWinding(QNanoPainter::WINDING_CCW))
*/

void QNanoPainter::beginSolidSubPath()
{
    nvgPathWinding(nvgCtx(), QNanoPainter::WINDING_CCW);
}

/*!
    \fn void QNanoPainter::beginHoleSubPath()

    Start a hole subpath. This is equivalent to setPathWinding(QNanoPainter::WINDING_CW)
*/

void QNanoPainter::beginHoleSubPath()
{
    nvgPathWinding(nvgCtx(), QNanoPainter::WINDING_CW);
}

/*!
    \fn void QNanoPainter::fill()

    Fills the current path with current fill style.
    \sa setFillStyle()
*/

void QNanoPainter::fill()
{
    nvgFill(nvgCtx());
}

/*!
    \fn void QNanoPainter::stroke()

    Strokes the current path with current stroke style.
    \sa setStrokeStyle()
*/

void QNanoPainter::stroke()
{
    nvgStroke(nvgCtx());
}

// *** Direct drawing ***

// In addition to paths, these methods allow drawing rects directly.
// NOTE: For performance it's better to use paths and draw multiple elements
// with one beginPath -> fill/stroke. So only use these for a single rectangle needs.

/*!
    \fn void QNanoPainter::fillRect(float x, float y, float width, float height)

    Draws a filled rectangle into specified position ( \a x, \a y) at size \a width, \a height.
*/

void QNanoPainter::fillRect(float x, float y, float width, float height)
{
    _checkAlignPixelsAdjust(&x, &y);
    _checkAlignPixels(&width, &height);
    nvgBeginPath(nvgCtx());
    nvgRect(nvgCtx(), x, y, width, height);
    nvgFill(nvgCtx());
}

/*!
    \fn void QNanoPainter::fillRect(const QRectF &rect)
    \overload

    Draws a filled rectangle into \a rect.
    This is an overloaded method using QRectF.
*/

void QNanoPainter::fillRect(const QRectF &rect)
{
    fillRect(rect.x(), rect.y(), rect.width(), rect.height());
}

/*!
    \fn void QNanoPainter::strokeRect(float x, float y, float width, float height)

    Draws a stoked rectangle into specified position ( \a x, \a y) at size \a width, \a height.
*/

void QNanoPainter::strokeRect(float x, float y, float width, float height)
{
    _checkAlignPixelsAdjust(&x, &y);
    _checkAlignPixels(&width, &height);
    nvgBeginPath(nvgCtx());
    nvgRect(nvgCtx(), x, y, width, height);
    nvgStroke(nvgCtx());
}

/*!
    \fn void QNanoPainter::strokeRect(const QRectF &rect)
    \overload

    Draws a stoked rectangle into \a rect.
    This is an overloaded method using QRectF.
*/

void QNanoPainter::strokeRect(const QRectF &rect)
{
    strokeRect(rect.x(), rect.y(), rect.width(), rect.height());
}

// *** Images ***
// These methods are used to draw images.

/*!
    \fn void QNanoPainter::drawImage(QNanoImage &image, float x, float y)

    Draw \a image into \a x, \a y, at its default size.
*/

void QNanoPainter::drawImage(QNanoImage &image, float x, float y)
{
    _checkAlignPixelsAdjust(&x, &y);
    image.setParentPainter(this);
    // Note: Required here just to get width&height of image updated
    image.getID(nvgCtx());
    drawImage(image, x, y, image.width(), image.height());
}

/*!
    \fn void QNanoPainter::drawImage(QNanoImage &image, float x, float y, float width, float height)
    \overload

    Draw \a image into \a x, \a y, at given \a width and \a height.
*/

void QNanoPainter::drawImage(QNanoImage &image, float x, float y, float width, float height)
{
    _checkAlignPixelsAdjust(&x, &y);
    _checkAlignPixels(&width, &height);
    image.setParentPainter(this);
    NVGpaint ip = nvgImagePattern(nvgCtx(), x, y, width, height, 0.0f, image.getID(nvgCtx()), 1.0f);
    nvgSave(nvgCtx());
    nvgBeginPath(nvgCtx());
    nvgRect(nvgCtx(), x, y, width, height);
    nvgFillPaint(nvgCtx(), ip);
    nvgFill(nvgCtx());
    nvgRestore(nvgCtx());
}

/*!
    \fn void QNanoPainter::drawImage(QNanoImage &image, const QRectF destinationRect)
    \overload

    Draw \a image into position and size of \a destinationRect.
*/

void QNanoPainter::drawImage(QNanoImage &image, const QRectF destinationRect)
{
    drawImage(image, destinationRect.x(), destinationRect.y(), destinationRect.width(), destinationRect.height());
}

/*!
    \fn void QNanoPainter::drawImage(QNanoImage &image, const QRectF sourceRect, const QRectF destinationRect)
    \overload

    Draw \a image into position and size of \a destinationRect, from \a sourceRect area of image.
*/

void QNanoPainter::drawImage(QNanoImage &image, const QRectF sourceRect, const QRectF destinationRect)
{
    image.setParentPainter(this);
    float sx = sourceRect.x();
    float sy = sourceRect.y();
    float sw = sourceRect.width();
    float sh = sourceRect.height();
    float dx = destinationRect.x();
    float dy = destinationRect.y();
    float dw = destinationRect.width();
    float dh = destinationRect.height();
    _checkAlignPixelsAdjust(&dx, &dy);
    _checkAlignPixels(&dw, &dh);
    float startX = dx - sx * (dw/sw);
    float startY = dy - sy * (dh/sh);
    float endX = dw * image.width() / sw;
    float endY = dh * image.height() / sh;
    NVGpaint ip = nvgImagePattern(nvgCtx(), startX, startY, endX, endY, 0.0f, image.getID(nvgCtx()), 1.0f);
    nvgSave(nvgCtx());
    nvgBeginPath(nvgCtx());
    rect(dx, dy, dw, dh);
    nvgFillPaint(nvgCtx(), ip);
    nvgFill(nvgCtx());
    nvgRestore(nvgCtx());
}

// *** Text ***

// QNanoPainter supports rendering truetype fonts using QNanoFont.
// The appearance is set on QNanoFont, font, size, spacing etc.
// Text color comes from current fillStyle.
//
// Font measure functions return values in local space, the calculations are
// carried in the same resolution as the final rendering. This is done because
// the text glyph positions are snapped to the nearest pixels sharp rendering.
//
// The local space means that values are not rotated or scale as per the current
// transformation. For example if you set font size to 12, which would mean that
// line height is 16, then regardless of the current scaling and rotation, the
// returned line height is always 16. Some measures may vary because of the scaling
// since aforementioned pixel snapping.
//
// Note: currently only solid color fill is supported for text.

/*!
    \fn void QNanoPainter::setFont(QNanoFont &font)

    Sets the \a font as currently active font.
*/

void QNanoPainter::setFont(QNanoFont &font)
{
    font.setParentPainter(this);
    nvgFontFaceId(nvgCtx(), font.getID(nvgCtx()));
    m_fontSet = true;
}

/*!
    \fn void QNanoPainter::setTextAlign(TextAlign align)

    Sets the horizontal alignment of text to \a align.
    The default alignment is ALIGN_LEFT.
*/

void QNanoPainter::setTextAlign(TextAlign align)
{
    m_textAlign = align;
    nvgTextAlign(nvgCtx(), m_textAlign | m_textBaseline);
}

/*!
    \fn void QNanoPainter::setTextBaseline(TextBaseline baseline)

    Sets the vertical alignment (baseline) of text to \a baseline.
    The default alignment is BASELINE_ALPHABETIC.
*/

void QNanoPainter::setTextBaseline(TextBaseline baseline)
{
    m_textBaseline = baseline;
    nvgTextAlign(nvgCtx(), m_textAlign | m_textBaseline);
}

/*!
    \fn void QNanoPainter::fillText(const QString &text, float x, float y, float maxWidth)

    Draws \a text string at specified location ( \a x, \a y), with current textAlign and textBaseline.
    To make the text wrap into multiple lines, set optional \a maxWidth parameter to preferred
    row width in pixels. White space is stripped at the beginning of the rows,
    the text is split at word boundaries or when new-line characters are encountered.
    Words longer than the max width are split at nearest character (i.e. no hyphenation).
*/

void QNanoPainter::fillText(const QString &text, float x, float y, float maxWidth)
{
    _checkAlignPixelsText(&x, &y);
    _checkFont();
    if (maxWidth < 0) {
        nvgText(nvgCtx(), x, y, text.toUtf8().constData(), nullptr);
    } else {
        nvgTextBox(nvgCtx(), x, y, maxWidth, text.toUtf8().constData(), nullptr);
    }
}

/*!
    \fn void QNanoPainter::fillText(const QString &text, const QPointF &point, float maxWidth)
    \overload

    Draws \a text string at specified \a point, with current textAlign and textBaseline.
    To make the text wrap into multiple lines, set optional \a maxWidth parameter to preferred
    row width in pixels. White space is stripped at the beginning of the rows,
    the text is split at word boundaries or when new-line characters are encountered.
    Words longer than the max width are split at nearest character (i.e. no hyphenation).

    This is an overloaded method using QPointF.
*/

void QNanoPainter::fillText(const QString &text, const QPointF &point, float maxWidth)
{
    fillText(text, point.x(), point.y(), maxWidth);
}

/*!
    \fn void QNanoPainter::fillText(const QString &text, const QRectF &rect)
    \overload

    Draws \a text string inside \a rect, with current textAlign and textBaseline.
    Width of the rect parameter is used as maxWidth.

    This is an overloaded method using QRectF.
*/

void QNanoPainter::fillText(const QString &text, const QRectF &rect)
{
    float x = rect.x();
    float y = rect.y();
    _checkAlignPixelsText(&x, &y);
    _checkFont();
    nvgTextBox(nvgCtx(), x, y, rect.width(), text.toUtf8().constData(), nullptr);
}

/*!
    \fn QRectF QNanoPainter::textBoundingBox(const QString &text, float x, float y, float maxWidth)

    Measures bounding box of a \a text string at (\a x, \a y).
    To measure multi-line text, set optional \a maxWidth parameter to preferred row width in pixels.
    Returns QRectF with values [xmin, ymin, width, height].
    Measured values are returned in local coordinate space.
*/

const QRectF QNanoPainter::textBoundingBox(const QString &text, float x, float y, float maxWidth)
{
    _checkAlignPixelsText(&x, &y);
    _checkFont();
    float bounds[4];
    if (maxWidth < 0) {
        nvgTextBounds(nvgCtx(), x, y, text.toUtf8().constData(), nullptr, bounds);
    } else {
        nvgTextBoxBounds(nvgCtx(), x, y, maxWidth, text.toUtf8().constData(), nullptr, bounds);
    }
    return QRectF(bounds[0], bounds[1], bounds[2]-bounds[0], bounds[3]-bounds[1]);
}

/*!
    \fn void QNanoPainter::setAntialias(float antialias)

    Set the current antialiasing amount to \a antialias in pixels.
    More antialias means smoother painting. This only affects fill and stroke painting,
    not images or texts. To smoothen text, see QNanoFont setBlur().
    The default value is 1.0.

    Antialiasing can be modified per-path so it can be set before each stroke/fill.
    To disable antialiasing from the whole QNanoQuickItem, use antialiasing property.
*/

void QNanoPainter::setAntialias(float antialias)
{
    nvgSetAntialias(nvgCtx(), antialias);
}

/*!
    \fn void QNanoPainter::setPixelAlign(PixelAlign align)

    Set the pixelAlign to \a align. Pixel alignment means that
    painting is aligned to half or full pixels. This is especially useful
    when drawing thin lines without antialias. This will automate half
    pixel adjustment in positions so e.g. 1px and 2px non-antialiased
    lines are painted with exact width. Animations when pixelAlign is
    enabled are obviously less smooth as everyting move in full pixels.
    The default value is PIXEL_ALIGN_NONE.
*/

void QNanoPainter::setPixelAlign(PixelAlign align)
{
    m_pixelAlign = align;
}

/*!
    \fn void QNanoPainter::setPixelAlignText(PixelAlign align)

    Set the pixelAlign of text to \a align. Pixel alignment means that
    painting of texts is aligned to half or full pixels. This is especially useful
    when drawing smaller font sizes and preference is to have them sharp
    instead of slightly blurry. Animations when pixelAlign is
    enabled are obviously less smooth as everyting move in full pixels.
    The default value is PIXEL_ALIGN_NONE.
*/

void QNanoPainter::setPixelAlignText(PixelAlign align)
{
    m_pixelAlignText = align;
    // TODO: Support half pixel alignment?
    if (m_pixelAlignText == PIXEL_ALIGN_NONE) {
        nvgSetPixelAlignText(nvgCtx(), false);
    } else {
        nvgSetPixelAlignText(nvgCtx(), true);
    }
}

/*!
    \fn double QNanoPainter::devicePixelRatio() const

    Returns the ratio between physical pixels and device-independent pixels.
    This is only used on Apple #retina" displays, where it's 2.0 or 3.0
    depending on screen multiplier.
    The default value is 1.0.
*/

double QNanoPainter::devicePixelRatio() const
{
    return m_devicePixelRatio;
}

// ***** Static methods *****

/*!
    \fn float QNanoPainter::mmToPx(float mm)

    Static helper method to convert millimeters \a mm into pixels.
    This allows doing resolution independent drawing, for example to set
    the line width to 2mm use:

    painter->setLineWidth(QNanoPainter::mmToPx(2));
*/

float QNanoPainter::mmToPx(float mm)
{
    qreal ldp = 72.0;
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        ldp = screen->physicalDotsPerInch();
    } else {
        qWarning() << "QScreen required for pxToMm";
    }
    return ldp * mm / 25.4;
}

/*!
    \fn float QNanoPainter::ptToPx(float pt)

    Static helper method to convert points \a pt into pixels.
*/

float QNanoPainter::ptToPx(float pt)
{
    qreal ldp = 72.0;
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        ldp = screen->physicalDotsPerInch();
    } else {
        qWarning() << "QScreen required for ptToPx";
    }
    return pt * (ldp/72.0);
}

/*
int QNanoPainter::textGlyphPositions(float x, float y, const char* string, const char* end, NVGglyphPosition* positions, int maxPositions)
{
    return nvgTextGlyphPositions(nvgCtx(), x, y, string, end, positions, maxPositions);
}

void QNanoPainter::textMetrics(float* ascender, float* descender, float* lineh)
{
    nvgTextMetrics(nvgCtx(), ascender, descender, lineh);
}

int QNanoPainter::textBreakLines(const char* string, const char* end, float breakRowWidth, NVGtextRow* rows, int maxRows)
{
    return nvgTextBreakLines(nvgCtx(), string, end, breakRowWidth, rows, maxRows);
}
*/

// ***** Private *****

/*!
   \internal
*/

void QNanoPainter::enableAntialiasing(bool enable)
{
    NVGparams *params = nvgInternalParams(nvgCtx());
    if (params && params->edgeAntiAlias != enable) {
        params->edgeAntiAlias = enable;
        GLNVGcontext *gl = static_cast<GLNVGcontext*>(params->userPtr);
        if (gl) {
            if (enable) {
                gl->flags |= NVG_ANTIALIAS;
            } else {
                gl->flags &= ~NVG_ANTIALIAS;
            }
            // Re-create shader
            glnvg__renderCreate(gl);
        }
    }
}

void QNanoPainter::enableHighQualityRendering(bool enable)
{

    GLNVGcontext *gl = static_cast<GLNVGcontext*>(nvgInternalParams(nvgCtx())->userPtr);
    if (gl) {
        if (enable) {
            gl->flags |= NVG_STENCIL_STROKES;
        } else {
            gl->flags &= ~NVG_STENCIL_STROKES;
        }
        // Re-create shader
        glnvg__renderCreate(gl);
    }
}

void QNanoPainter::_checkFont() {
    // If user hasn't set a font, load the default one
    if (!m_fontSet) {
        if (m_defaultFont.isNull()) {
            qDebug() << "No font set, using the default font";
            m_defaultFont = QSharedPointer<QNanoFont>::create(QNanoFont::DEFAULT_FONT_NORMAL);
        }
        setFont(*m_defaultFont);
    }
}

// Align to full or half pixel
void QNanoPainter::_checkAlignPixelsAdjust(float *a, float *b, float *c, float *d) {
    if (m_pixelAlign) {
        _checkAlignPixelsAdjustOne(a);
        _checkAlignPixelsAdjustOne(b);
        _checkAlignPixelsAdjustOne(c);
        _checkAlignPixelsAdjustOne(d);
    }
}

// Align to full pixel
void QNanoPainter::_checkAlignPixels(float *a, float *b, float *c, float *d) {
    if (m_pixelAlign) {
        if (a) *a = ((int)(*a * m_devicePixelRatio + 0.5)) / m_devicePixelRatio;
        if (b) *b = ((int)(*b * m_devicePixelRatio + 0.5)) / m_devicePixelRatio;
        if (c) *c = ((int)(*c * m_devicePixelRatio + 0.5)) / m_devicePixelRatio;
        if (d) *d = ((int)(*d * m_devicePixelRatio + 0.5)) / m_devicePixelRatio;
    }
}

// Align text to full pixel
void QNanoPainter::_checkAlignPixelsText(float *a, float *b) {
    if (m_pixelAlignText) {
        if (a) *a = ((int)(*a * m_devicePixelRatio + 0.5)) / m_devicePixelRatio;
        if (b) *b = ((int)(*b * m_devicePixelRatio + 0.5)) / m_devicePixelRatio;
    }
}

void QNanoPainter::_checkAlignPixelsAdjustOne(float *a) {
    if (a) {
        float adjustment = 0.5/m_devicePixelRatio;
        *a = ((int)(*a * m_devicePixelRatio)) / m_devicePixelRatio;
        if (m_pixelAlign == QNanoPainter::PIXEL_ALIGN_HALF) *a += adjustment;
    }
}
