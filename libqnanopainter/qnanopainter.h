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

#ifndef QNANOPAINTER_H
#define QNANOPAINTER_H

#include <QRectF>
#include <QCache>
#include <QTransform>
#include <QSharedPointer>
#include <QSurfaceFormat>
#include "qnanocolor.h"
#include "private/qnanobrush.h"
#include "private/qnanodataelement.h"
#include "private/qnanobackend.h"
#include "qnanolineargradient.h"
#include "qnanoradialgradient.h"
#include "qnanoimage.h"
#include "qnanoimagepattern.h"
#include "qnanofont.h"

struct NVGcontext;

class QNanoPainter
{

public:

    QNanoPainter();
    ~QNanoPainter();

    enum PathWinding {
        WINDING_CCW = 1,  // Winding for solid shapes, counter-clockwise
        WINDING_CW = 2    // Winding for holes, clockwise
    };
    enum LineCap {
        CAP_BUTT = 0,
        CAP_ROUND = 1,
        CAP_SQUARE = 2
    };
    enum LineJoin {
        JOIN_ROUND = 1,
        JOIN_BEVEL = 3,
        JOIN_MITER = 4
    };
    enum TextAlign {
        ALIGN_LEFT = 1,
        ALIGN_CENTER = 2,
        ALIGN_RIGHT = 4
    };
    enum TextBaseline {
        BASELINE_TOP = 8,
        BASELINE_MIDDLE = 16,
        BASELINE_BOTTOM = 32,
        BASELINE_ALPHABETIC = 64
    };
    enum PixelAlign {
        PIXEL_ALIGN_NONE = 0,
        PIXEL_ALIGN_HALF = 1,
        PIXEL_ALIGN_FULL = 2
    };

    // *** State Handling ***

    void save();
    void restore();
    void reset();


    // *** Render styles ***

    void setStrokeStyle(const QNanoColor &color);
    void setStrokeStyle(const QNanoBrush &brush);
    void setFillStyle(const QNanoColor &color);
    void setFillStyle(const QNanoBrush &brush);
    void setMiterLimit(float limit);
    void setLineWidth(float width);
    void setLineCap(LineCap cap);
    void setLineJoin(LineJoin join);
    void setGlobalAlpha(float alpha);

    // *** Transforms ***

    void resetTransform();
    void transform(const QTransform &transform);
    void translate(float x, float y);
    void translate(const QPointF &point);
    void rotate(float angle);
    void skewX(float angle);
    void skewY(float angle);
    void scale(float scale);
    void scale(float scaleX, float scaleY);
    const QTransform currentTransform() const;

    // *** Scissoring ***
    //
    // Scissoring allows you to clip the rendering into a rectangle. This is useful for varius
    // user interface cases like rendering a text edit or a timeline.

    void setClipRect(float x, float y, float w, float h);
    void setClipRect(const QRectF &rect);
    void resetClipping();

    //  *** Paths ***

    void beginPath();
    void closePath();
    void moveTo(float x, float y);
    void moveTo(const QPointF &point);
    void lineTo(float x, float y);
    void lineTo(const QPointF &point);
    void bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y);
    void bezierTo(const QPointF &controlPoint1, const QPointF &controlPoint2, const QPointF &endPoint);
    void quadTo(float cx, float cy, float x, float y);
    void quadTo(const QPointF &controlPoint, const QPointF &endPoint);
    void arcTo(float c1x, float c1y, float c2x, float c2y, float radius);
    void arcTo(const QPointF &controlPoint1, const QPointF &controlPoint2, float radius);

    void arc(float cx, float cy, float radius, float a0, float a1, PathWinding direction=QNanoPainter::WINDING_CW);
    void arc(const QPointF &centerPoint, float radius, float a0, float a1, PathWinding direction=QNanoPainter::WINDING_CW);
    void rect(float x, float y, float width, float height);
    void rect(const QRectF &rect);
    void roundedRect(float x, float y, float width, float height, float radius);
    void roundedRect(const QRectF &rect, float radius);
    void roundedRect(float x, float y, float width, float height, float radiusTopLeft, float radiusTopRight, float radiusBottomRight, float radiusBottomLeft);
    void roundedRect(const QRectF &rect, float radiusTopLeft, float radiusTopRight, float radiusBottomRight, float radiusBottomLeft);
    void ellipse(float centerX, float centerY, float radiusX, float radiusY);
    void ellipse(const QRectF &rect);
    void circle(float centerX, float centerY, float radius);
    void circle(const QPointF &centerPoint, float radius);

    void setPathWinding(PathWinding winding);
    void beginSolidSubPath();
    void beginHoleSubPath();

    void fill();
    void stroke();

    // *** Direct drawing ***

    void fillRect(float x, float y, float width, float height);
    void fillRect(const QRectF &rect);
    void strokeRect(float x, float y, float width, float height);
    void strokeRect(const QRectF &rect);

    // *** Images ***

    void drawImage(QNanoImage &image, float x, float y);
    void drawImage(QNanoImage &image, float x, float y, float width, float height);
    void drawImage(QNanoImage &image, const QRectF destinationRect);
    void drawImage(QNanoImage &image, const QRectF sourceRect, const QRectF destinationRect);

    // *** Text ***

    void setFont(QNanoFont &font);
    void setTextAlign(TextAlign align);
    void setTextBaseline(TextBaseline baseline);

    void fillText(const QString &text, float x, float y, float maxWidth = -1);
    void fillText(const QString &text, const QPointF &point, float maxWidth = -1);
    void fillText(const QString &text, const QRectF &rect);

    const QRectF textBoundingBox(const QString &text, float x, float y, float maxWidth = -1);

    // *** Other ***

    void setAntialias(float antialias);
    void setPixelAlign(PixelAlign align);
    void setPixelAlignText(PixelAlign align);
    float devicePixelRatio() const;

    // ***** Static methods *****

    static float mmToPx(float mm);
    static float ptToPx(float pt);


/* TODO: Update these for better API

    // Calculates the glyph x positions of the specified text. If end is specified only the sub-string will be used.
    // Measured values are returned in local coordinate space.
    int textGlyphPositions(float x, float y, const char* string, const char* end, NVGglyphPosition* positions, int maxPositions);

    // Returns the vertical metrics based on the current text style.
    // Measured values are returned in local coordinate space.
    void textMetrics(float* ascender, float* descender, float* lineh);

    // Breaks the specified text into lines. If end is specified only the sub-string will be used.
    // White space is stripped at the beginning of the rows, the text is split at word boundaries or when new-line characters are encountered.
    // Words longer than the max width are slit at nearest character (i.e. no hyphenation).
    int textBreakLines(const char* string, const char* end, float breakRowWidth, NVGtextRow* rows, int maxRows);
*/

private:

    friend class QNanoDebug;
    friend class QNanoWindow;
    friend class QNanoWidget;
    friend class QNanoQuickItemPainter;
    friend class QNanoFont;
    friend class QNanoImage;
    friend class QNanoBoxGradient;
    friend class QNanoImagePattern;
    friend class QNanoLinearGradient;
    friend class QNanoRadialGradient;

    static QNanoPainter *getInstance();

    inline NVGcontext* nvgCtx() const {
        return m_nvgContext;
    }

    void enableAntialiasing(bool enable);
    void enableHighQualityRendering(bool enable);

    void _checkFont();
    void _checkAlignPixelsAdjust(float *a, float *b = nullptr, float *c = nullptr, float *d = nullptr);
    void _checkAlignPixels(float *a, float *b = nullptr, float *c = nullptr, float *d = nullptr);
    void _checkAlignPixelsText(float *a, float *b = nullptr);
    void _checkAlignPixelsAdjustOne(float *a);

    // TODO: Consider implementing QNanoDataCache class with methods instead of this
    QHash<QString, QNanoDataElement*> m_dataCache;

    NVGcontext* m_nvgContext;
    QScopedPointer<QNanoBackend> m_backend;
    QSurfaceFormat m_surfaceFormat;

    QNanoPainter::TextAlign m_textAlign;
    QNanoPainter::TextBaseline m_textBaseline;
    float m_devicePixelRatio;
    bool m_fontSet;
    QNanoPainter::PixelAlign m_pixelAlign;
    QNanoPainter::PixelAlign m_pixelAlignText;
    QSharedPointer<QNanoFont> m_defaultFont;
    QString m_openglContextName;

};

#endif // QNANOPAINTER_H
