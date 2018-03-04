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

#ifndef QNANOQUICKITEM_H
#define QNANOQUICKITEM_H

#include <QtQuick/QQuickFramebufferObject>

class QNanoQuickItemPainter;

#ifdef QNANO_USE_RENDERNODE
class QNanoQuickItem : public QQuickItem
#else
class QNanoQuickItem : public QQuickFramebufferObject
#endif
{
    Q_OBJECT
    Q_PROPERTY(QString contextName READ contextName NOTIFY contextNameChanged)
    Q_PROPERTY(QString backendName READ backendName NOTIFY backendNameChanged)
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
    Q_PROPERTY(PixelAlign pixelAlign READ pixelAlign WRITE setPixelAlign NOTIFY pixelAlignChanged)
    Q_PROPERTY(PixelAlign pixelAlignText READ pixelAlignText WRITE setPixelAlignText NOTIFY pixelAlignTextChanged)
    Q_PROPERTY(bool highQualityRendering READ highQualityRendering WRITE setHighQualityRendering NOTIFY highQualityRenderingChanged)
    Q_PROPERTY(bool mouseEventsEnabled READ mouseEventsEnabled WRITE setMouseEventsEnabled NOTIFY mouseEventsEnabledChanged)
    Q_PROPERTY(bool mouseHoverEventsEnabled READ mouseHoverEventsEnabled WRITE setMouseHoverEventsEnabled NOTIFY mouseHoverEventsEnabledChanged)
    Q_PROPERTY(Qt::MouseButtons acceptedButtons READ acceptedButtons WRITE setAcceptedButtons NOTIFY acceptedButtonsChanged)
    Q_PROPERTY(int textureWidth READ textureWidth WRITE setTextureWidth NOTIFY textureWidthChanged)
    Q_PROPERTY(int textureHeight READ textureHeight WRITE setTextureHeight NOTIFY textureHeightChanged)
    Q_ENUMS(PixelAlign)

public:

    enum PixelAlign {
        PixelAlignNone = 0,
        PixelAlignHalf = 1,
        PixelAlignFull = 2
    };

    QNanoQuickItem(QQuickItem *parent = 0);
    ~QNanoQuickItem();

    QString contextName() const;

    QString backendName() const;

    QColor fillColor() const;
    void setFillColor(const QColor &color);

    QNanoQuickItem::PixelAlign pixelAlign() const;
    void setPixelAlign(QNanoQuickItem::PixelAlign align);

    PixelAlign pixelAlignText() const;
    void setPixelAlignText(QNanoQuickItem::PixelAlign align);

    bool highQualityRendering() const;
    void setHighQualityRendering(bool value);

    Qt::MouseButtons acceptedButtons() const;
    void setAcceptedButtons(Qt::MouseButtons buttons);

    bool mouseEventsEnabled() const;
    void setMouseEventsEnabled(bool enabled);

    bool mouseHoverEventsEnabled() const;
    void setMouseHoverEventsEnabled(bool enabled);

    int textureWidth() const;
    void setTextureWidth(int width);

    int textureHeight() const;
    void setTextureHeight(int height);

protected:

    virtual QNanoQuickItemPainter* createItemPainter() const = 0;

#ifndef QNANO_USE_RENDERNODE
    // Reimplement from QQuickFramebufferObject
    QQuickFramebufferObject::Renderer *createRenderer() const Q_DECL_OVERRIDE Q_DECL_FINAL;
#endif
#if (QT_VERSION < QT_VERSION_CHECK(5, 6, 0)) || defined(QNANO_USE_RENDERNODE)
    // Reimplement from QQuickItem
    QSGNode *updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *nodeData) Q_DECL_OVERRIDE Q_DECL_FINAL;
#endif
#ifdef QNANO_USE_RENDERNODE
    void itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData &value) Q_DECL_OVERRIDE Q_DECL_FINAL;
#endif

Q_SIGNALS:
    void contextNameChanged();
    void backendNameChanged();
    void fillColorChanged();
    void pixelAlignChanged();
    void pixelAlignTextChanged();
    void highQualityRenderingChanged();
    void acceptedButtonsChanged();
    void mouseEventsEnabledChanged();
    void mouseHoverEventsEnabledChanged();
    void textureWidthChanged();
    void textureHeightChanged();

private:

    friend class QNanoQuickItemPainter;

    void setContextName(const QString &name);
    void setBackendName(const QString &name);

    QString m_contextName;
    QString m_backendName;
    QColor m_fillColor;
    QNanoQuickItem::PixelAlign m_pixelAlign;
    QNanoQuickItem::PixelAlign m_pixelAlignText;
    bool m_highQualityRendering;
    Qt::MouseButtons m_acceptedMouseButtons;
    bool m_mouseEnabled;
    int m_textureWidth;
    int m_textureHeight;
};

#define QNANO_PROPERTY(type, variable, getter, setter) \
private: \
    Q_PROPERTY(type getter READ getter WRITE setter NOTIFY getter##Changed) \
Q_SIGNALS: \
    void getter##Changed(); \
public: \
    type const& getter() const { return variable; } \
public Q_SLOTS: \
    void setter(type const &v) { if(v == variable) return; variable = v; emit getter##Changed(); update(); } \
private: \
    type variable;

#endif // QNANOQUICKITEM_H
