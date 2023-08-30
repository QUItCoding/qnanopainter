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

#include "qnanoquickitem.h"
#include "qnanoquickitempainter.h"
#if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
#include <QSGSimpleTextureNode>
#endif

/*!
    \enum QNanoQuickItem::PixelAlign

    PixelAlign is used to define if painting or text should be aligned to pixels.

    \value PixelAlignNone (default) Do not do any alignment.

    \value PixelAlignHalf Align to half pixels. This will make painting appear sharp when line width is odd.

    \value PixelAlignFull Align to full pixels. This will make painting appear sharp when line width is even.

    \sa setPixelAlign(), setPixelAlignText()
*/

/*!
    \class QNanoQuickItem
    \brief The QNanoQuickItem creates QtQuick item using QNanoPainter.
    \inmodule QNanoPainter

    TODO: Write more documentation here.
*/

/*!
    \property QNanoQuickItem::contextName
    \brief Get name and version of used OpenGL context.
*/

/*!
    \property QNanoQuickItem::backendName
    \brief Get name of used QNanoPainter backend.
*/

/*!
    \property QNanoQuickItem::fillColor
    \brief The color to use for filling the item ie. the item background.

    The default color is transparent.
*/

/*!
    \property QNanoQuickItem::pixelAlign
    \brief Set the pixelAlign mode.

    The default value is false.
*/

/*!
    \property QNanoQuickItem::pixelAlignText
    \brief Set the pixelAlign mode of text.

    The default value is false.
*/

/*!
    \property QNanoQuickItem::highQualityRendering
    \brief Set the highQualityRendering mode on.

    The default value is false.
*/

/*!
    \property QNanoQuickItem::mouseEventsEnabled
    \brief This property defines if item should accept mouse events.
    The default value is \c false.
*/

/*!
    \property QNanoQuickItem::mouseHoverEventsEnabled
    \brief This property defines if item should accept mouse hover events.
    The default value is \c false.

    This is basically QML property for accessing QQuickItem::acceptHoverEvents()
*/

/*!
    \property QNanoQuickItem::acceptedButtons
    \brief This property holds the mouse buttons that the item reacts to.
    The default value is \c Qt.LeftButton.

    To accept all buttons use \c Qt.AllButtons.
*/

/*!
    \property QNanoQuickItem::textureWidth
    \brief This property controls the width of attached QNanoQuickItemPainter texture.
    By default width is -1, meaning it follows QNanoQuickItem width.

    Changing the texture size will recreate the FBO which can affect
    the performance. So if item size is e.g. animated it may be useful
    to set the texture size manually. Note: When setting texture size
    manually, usually you should also set textureFollowsItemSize to false.
*/

/*!
    \property QNanoQuickItem::textureHeight
    \brief This property controls the height of attached QNanoQuickItemPainter texture.
    By default height is -1, meaning it follows QNanoQuickItem height.

    Changing the texture size will recreate the FBO which can affect
    the performance. So if item size is e.g. animated it may be useful
    to set the texture size manually. Note: When setting texture size
    manually, usually you should also set textureFollowsItemSize to false.
*/

/*!
    \macro void QNANO_PROPERTY(type, variable, getter, setter)
    \relates QNanoQuickItem

    This helper macro can be used to simplify implementing properties.
    Creates a Q_PROPERTY, local variable, getter, setter and signal all in one.
    Setter will also call update().
    Parameters are property variable \a type, name of private \a variable,
    name of the \a getter method and  the \a setter method.

    Usage examples:

    QNANO_PROPERTY(float, m_animationValue, animValue, setAnimValue)

    QNANO_PROPERTY(bool, m_showBorder, showBorder, setShowBorder)
*/

/*!
    Constructs a QNanoQuickItem with the given \a parent item.
*/

QNanoQuickItem::QNanoQuickItem(QQuickItem *parent)
#ifdef QNANO_USE_RHI
  : QQuickRhiItem(parent)
#elif QNANO_USE_RENDERNODE
  : QQuickItem(parent)
#else
  : QQuickFramebufferObject(parent)
#endif
{

#if defined(QNANO_USE_RENDERNODE) || defined(QNANO_USE_RHI)
    setFlag(ItemHasContents, true);
    connect(this, SIGNAL(scaleChanged()), this, SLOT(update()));
#endif
    // Default to antialiased
    setAntialiasing(true);

#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
#if !defined(QNANO_USE_RENDERNODE) && !defined(QNANO_USE_RHI)
    // New API in Qt 5.6 to mirror FBO
    // Note: Not needed for RHI
    setMirrorVertically(true);
#endif
#endif

#ifdef QNANO_DEBUG_COLLECT
    const int DEBUG_UPDATE_FREQUENCY_MS = 1000;
    m_debugUpdateTimer.setInterval(DEBUG_UPDATE_FREQUENCY_MS);
    connect(&m_debugUpdateTimer, &QTimer::timeout, this, &QNanoQuickItem::updateDebug);
    m_debugUpdateTimer.start();
#endif

#ifdef QNANO_USE_RHI
    // By default the fillColor is transparent.
    setAlphaBlending(true);
#endif
}

/*!
    Destroys the QNanoQuickItem.
*/

QNanoQuickItem::~QNanoQuickItem()
{
}

/*!
    \fn QNanoQuickItemPainter* QNanoQuickItem::createItemPainter() const

    Implement this method to (re)create painter for this item. The
    painter class should be inherited from QNanoQuickItemPainter.
    QNanoQuickItem takes the ownership of the created object and
    deletes it when needed.

    Example code:

    QNanoQuickItemPainter* MyItem::createItemPainter() const
    {
        return new MyItemPainter();
    }

*/

/*!
    \fn QString QNanoQuickItem::contextName() const

    Returns used Qt OpenGL context type and version as a string.
    This can be for example "OpenGL ES 2.0", "OpenGL 4.3" etc.
*/
QString QNanoQuickItem::contextName() const
{
    return m_contextName;
}

/*!
    \fn QString QNanoQuickItem::backendName() const

    Returns used rendering backend name. Currently available backends are:
    - "OpenGL 2": Used for desktop OpenGL context 2.0 - 3.1
    - "OpenGL 3": Used for desktop OpenGL context >= 3.2
    - "OpenGL ES 2": Used for OpenGL ES context 2.0
    - "OpenGL ES 3": Used for OpenGL ES context >= 3.0

    To affect which backend is used, configure the OpenGL context
    version of the Qt application e.g. with QSurfaceFormat.
*/
QString QNanoQuickItem::backendName() const
{
    return m_backendName;
}

/*!
    \fn bool QNanoQuickItem::pixelAlign() const

    Returns current pixel align used for painting.
    By default there is no pixel align, so PixelAlignNone.

    \sa setPixelAlign(), pixelAlignText()
*/

QNanoQuickItem::PixelAlign QNanoQuickItem::pixelAlign() const
{
    return m_pixelAlign;
}

/*!
    \fn void QNanoQuickItem::setPixelAlign(QNanoQuickItem::PixelAlign align)

    Set the pixelAlign to \a align. Pixel alignment means that
    painting is aligned to full or half pixels. When painting with odd
    line widths (e.g. 1), aligning to PixelAlignHalf makes to drawing sharp.
    Similarly when painting with even widths (e.g. 2) and PixelAlignFull.
    Animations when pixelAlign is enabled are obviously less smooth as
    everyting move in full pixels.
    By default there is no pixel align, so PixelAlignNone.

    \sa pixelAlign()
*/

void QNanoQuickItem::setPixelAlign(QNanoQuickItem::PixelAlign align)
{
    if (m_pixelAlign == align)
        return;
    m_pixelAlign = align;
    Q_EMIT pixelAlignChanged();
    update();
}

/*!
    \fn bool QNanoQuickItem::pixelAlignText() const

    Returns true when pixel align for texts is on.

    \sa setPixelAlignText(), pixelAlign()
*/

QNanoQuickItem::PixelAlign QNanoQuickItem::pixelAlignText() const
{
    return m_pixelAlignText;
}

/*!
    \fn void QNanoQuickItem::setPixelAlignText(QNanoQuickItem::PixelAlign align)

    Set the pixelAlign of text to \a align. Pixel alignment means that
    painting of texts is aligned to full pixels. This is especially useful
    when drawing smaller font sizes and preference is to have them sharp
    instead of slightly blurry. Animations when pixelAlign is
    enabled are obviously less smooth as everyting move in full pixels.
    Note: Currently texts support only one type of alignment so PixelAlignHalf
    and PixelAlignFull both result to same sharp alignment.
    By default there is no pixel align, so PixelAlignNone.

    \sa pixelAlignText(), setPixelAlign()
*/

void QNanoQuickItem::setPixelAlignText(QNanoQuickItem::PixelAlign align)
{
    if (m_pixelAlignText == align)
        return;
    m_pixelAlignText = align;
    Q_EMIT pixelAlignTextChanged();
    update();
}

/*!
    \fn bool QNanoQuickItem::highQualityRendering() const

    Returns true when highQualityRendering is on.

    \sa setHighQualityRendering()
*/

bool QNanoQuickItem::highQualityRendering() const
{
    return m_highQualityRendering;
}

/*!
    \fn void QNanoQuickItem::setHighQualityRendering(bool value)

    Set the highQualityRendering to \a value. HighQualityRendering gives
    a more correct rendering in some corner-cases but isn't normally needed.
    Enabling it results into higher rendering cost.
    The default value is false.

    \sa highQualityRendering()
*/

void QNanoQuickItem::setHighQualityRendering(bool value)
{
    if (m_highQualityRendering == value)
        return;
    m_highQualityRendering = value;
    Q_EMIT highQualityRenderingChanged();
    update();
}

/*!
    \fn QColor QNanoQuickItem::fillColor() const

    Returns the current fill color.

    \sa setFillColor()
*/

QColor QNanoQuickItem::fillColor() const
{
    return m_fillColor;
}

/*!
    \fn void QNanoQuickItem::setFillColor (const QColor &color)

    Set the fill color to \a color. This color will be used to draw
    the background of the item. The default color is transparent.

    \sa fillColor()
*/

void QNanoQuickItem::setFillColor(const QColor &color)
{
    if (m_fillColor == color)
        return;
    m_fillColor = color;

#ifdef QNANO_USE_RHI
    const bool blend = (m_fillColor.alpha() < 255);
    setAlphaBlending(blend);
#endif
    Q_EMIT fillColorChanged();
    update();
}

/*!
    \fn bool QNanoQuickItem::mouseEventsEnabled() const

    Returns true when mouse events for the item are enabled.

    \sa setMouseEventsEnabled()
*/

bool QNanoQuickItem::mouseEventsEnabled() const
{
    return m_mouseEnabled;
}

/*!
    \fn void QNanoQuickItem::setMouseEventsEnabled(bool enabled)

    Enable or disabled receiving mouse events depending on \a enabled.
    The default value is false, so to receive mouse event they need to be enabled first.
    Once enabled, QNanoQuickItemPainter::mouseEvent() gets called for all events.

    \sa mouseEventsEnabled()
*/

void QNanoQuickItem::setMouseEventsEnabled(bool enabled)
{
    if (m_mouseEnabled == enabled)
        return;
    m_mouseEnabled = enabled;
    if (m_mouseEnabled) {
        setAcceptedMouseButtons(m_acceptedMouseButtons);
    } else {
        setAcceptedMouseButtons(Qt::NoButton);
    }
    Q_EMIT mouseEventsEnabledChanged();
}

/*!
    \fn bool QNanoQuickItem::mouseHoverEventsEnabled() const

    Returns true when mouse hover events for the item are enabled.

    \sa setMouseHoverEventsEnabled()
*/

bool QNanoQuickItem::mouseHoverEventsEnabled() const
{
    return acceptHoverEvents();
}

/*!
    \fn void QNanoQuickItem::setMouseHoverEventsEnabled(bool enabled)

    Enable or disabled receiving mouse hover events depending on \a enabled.
    The default value is false, so to receive mouse hover event they need to be enabled first.
    Once enabled, QQuickItem hover events (hoverEnterEvent(), hoverMoveEvent() and hoverLeaveEvent())
    event handles will get called.

    \sa mouseHoverEventsEnabled()
*/

void QNanoQuickItem::setMouseHoverEventsEnabled(bool enabled)
{
    if (acceptHoverEvents() == enabled)
        return;
    setAcceptHoverEvents(enabled);
    Q_EMIT mouseHoverEventsEnabledChanged();
}

/*!
    \fn Qt::MouseButtons QNanoQuickItem::acceptedButtons() const

    Returns currently accepted mouse buttons.

    \sa setAcceptedButtons()
*/

Qt::MouseButtons QNanoQuickItem::acceptedButtons() const
{
    return m_acceptedMouseButtons;
}

/*!
    \fn void QNanoQuickItem::setAcceptedButtons(Qt::MouseButtons buttons)

    Set which mouse \a buttons are accepted (meaning listened).
    The default value is \c Qt::LeftButton.

    \sa acceptedButtons()
*/

void QNanoQuickItem::setAcceptedButtons(Qt::MouseButtons buttons)
{
    if (m_acceptedMouseButtons == buttons)
        return;
    m_acceptedMouseButtons = buttons;
    if (m_mouseEnabled) {
        setAcceptedMouseButtons(m_acceptedMouseButtons);
    }
    Q_EMIT acceptedButtonsChanged();
}

/*!
    \fn int QNanoQuickItem::textureWidth() const

    Returns width of attached QNanoQuickItemPainter texture.
    By default width is -1, meaning it follows QNanoQuickItem width.
*/

int QNanoQuickItem::textureWidth() const
{
    return m_textureWidth;
}

/*!
    \fn void QNanoQuickItem::setTextureWidth(int width)

    Set \a width (in pixels) of attached QNanoQuickItemPainter texture.
    To make it follow QNanoQuickItem width, set to -1 (default).

    Changing the texture size will recreate the FBO which can affect
    the performance. So if item size is e.g. animated it may be useful
    to set the texture size manually. Note: When setting texture size
    manually, usually you should also set textureFollowsItemSize to false.
*/

void QNanoQuickItem::setTextureWidth(int width)
{
    if (m_textureWidth == width)
        return;
    m_textureWidth = width;
    Q_EMIT textureWidthChanged();
    update();
}

/*!
    \fn int QNanoQuickItem::textureHeight() const

    Returns height of attached QNanoQuickItemPainter texture.
    By default height is -1, meaning it follows QNanoQuickItem height.
*/

int QNanoQuickItem::textureHeight() const
{
    return m_textureHeight;
}

/*!
    \fn void QNanoQuickItem::setTextureHeight(int height)

    Set \a height (in pixels) of attached QNanoQuickItemPainter texture.
    To make it follow QNanoQuickItem height, set to -1 (default).

    Changing the texture size will recreate the FBO which can affect
    the performance. So if item size is e.g. animated it may be useful
    to set the texture size manually. Note: When setting texture size
    manually, usually you should also set textureFollowsItemSize to false.
*/

void QNanoQuickItem::setTextureHeight(int height)
{
    if (m_textureHeight == height)
        return;
    m_textureHeight = height;
    Q_EMIT textureHeightChanged();
    update();
}

QVariantMap QNanoQuickItem::debug()
{
    if (m_debug.isEmpty()) {
        // Initialize debug data
        m_debug.insert("drawCallCount", 0);
        m_debug.insert("strokeTriCount", 0);
        m_debug.insert("fillTriCount", 0);
        m_debug.insert("textTriCount", 0);
    }
    return m_debug;
}

#ifdef QNANO_DEBUG_COLLECT
/*!
   \internal
*/

void QNanoQuickItem::updateDebug()
{
    if (m_debugDataChanged) {
        Q_EMIT debugChanged();
        m_debugDataChanged = false;
        update();
    }
}

/*!
   \internal
*/

void QNanoQuickItem::updateDebugData(NVGdrawDebug drawDebug)
{
    m_debug.insert("drawCallCount", drawDebug.drawCallCount);
    m_debug.insert("strokeTriCount", drawDebug.strokeTriCount);
    m_debug.insert("fillTriCount", drawDebug.fillTriCount);
    m_debug.insert("textTriCount", drawDebug.textTriCount);
    m_debugDataChanged = true;
}
#endif

/*!
   \internal
*/

#ifndef QNANO_USE_RENDERNODE
#ifdef QNANO_USE_RHI
QQuickRhiItemRenderer *QNanoQuickItem::createRenderer()
#else
QQuickFramebufferObject::Renderer *QNanoQuickItem::createRenderer() const
#endif
{
    return createItemPainter();
}
#endif

/*!
   \internal
*/

void QNanoQuickItem::setContextName(const QString &name)
{
    if (m_contextName != name) {
        m_contextName = name;
        Q_EMIT contextNameChanged();
    }
}

/*!
   \internal
*/

void QNanoQuickItem::setBackendName(const QString &name)
{
    if (m_backendName != name) {
        m_backendName = name;
        Q_EMIT backendNameChanged();
    }
}

/*!
   \internal
*/

#ifdef QNANO_USE_RENDERNODE
QSGNode *QNanoQuickItem::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *nodeData)
{
    Q_UNUSED(nodeData)
    QNanoQuickItemPainter *n = static_cast<QNanoQuickItemPainter *>(node);
    if (!n) {
        n = createItemPainter();
    }
    n->synchronizePainter(this);
    n->markDirty(QSGNode::DirtyMaterial);
    return n;
}

void QNanoQuickItem::itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData &value)
{
    // When item opacity, rotation etc. change call update to synchronize properties with painter
    update();
    QQuickItem::itemChange(change, value);
}

// Flip FBO, see QTBUG-41073
// With Qt 5.6 this is replaced with setMirrorVertically
#elif QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
QSGNode *QNanoQuickItem::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *nodeData)
{
    if (!node) {
        node = QQuickFramebufferObject::updatePaintNode(node, nodeData);
        QSGSimpleTextureNode *n = static_cast<QSGSimpleTextureNode *>(node);
        if (n)
            n->setTextureCoordinatesTransform(QSGSimpleTextureNode::MirrorVertically);
        return node;
    }
    return QQuickFramebufferObject::updatePaintNode(node, nodeData);
}
#endif

#ifdef QNANO_ENABLE_TOUCH_SIGNALS
void QNanoQuickItem::touchEvent(QTouchEvent *event)
{
    event->setAccepted(false);
    Q_EMIT touchSignal(event);
}
#endif
