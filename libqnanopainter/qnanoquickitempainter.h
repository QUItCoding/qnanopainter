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

#ifndef QNANOQUICKITEMPAINTER_H
#define QNANOQUICKITEMPAINTER_H

#include <QtGui/QOpenGLFunctions>
#include <QtQuick/QQuickFramebufferObject>
#include <QColor>
#include <QElapsedTimer>
#include <QSharedPointer>
#include "qnanopainter.h"
#include "qnanoquickitem.h"

class QQuickWindow;

class QNanoQuickItemPainter : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions
{

public:
    explicit QNanoQuickItemPainter();
    virtual ~QNanoQuickItemPainter();

    virtual void paint(QNanoPainter *painter) = 0;
    virtual void synchronize(QNanoQuickItem *item);
    virtual void sizeChanged(float width, float height);

    QColor fillColor() const;
    inline QNanoPainter *painter() const
    {
        return m_painter.data();
    }
    inline float width() const
    {
        return m_itemWidth;
    }
    inline float height() const
    {
        return m_itemHeight;
    }

protected:
    // Re-implemented from QQuickFramebufferObject::Renderer

    // Creates initial FBO.
    // Gets also called whenever item size changes if textureFollowsItemSize = true
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) Q_DECL_OVERRIDE;

    // Gets called when FBO should be rendered into.
    void render() Q_DECL_OVERRIDE;

    // Gets called as a result of QQuickFramebufferObject::update().
    void synchronize(QQuickFramebufferObject * item) Q_DECL_OVERRIDE;


private:

    // These are internal
    void initialize();
    void prepaint();
    void postpaint();
    void setSize(float width, float height);

    QQuickWindow *m_window;

    QSharedPointer<QNanoPainter> m_painter;
    QColor m_fillColor;
    float m_itemWidth, m_itemHeight;
    double m_devicePixelRatio;
    bool m_antialiasing;
    bool m_highQualityRendering;
    QNanoQuickItem::PixelAlign m_pixelAlign;
    QNanoQuickItem::PixelAlign m_pixelAlignText;
    bool m_setupDone;
#ifdef QNANO_DEBUG
    void paintDrawDebug();
    NVGdrawDebug m_drawDebug;
    QElapsedTimer m_debugTimer;
    QElapsedTimer m_debugUpdateTimer;
    qint64 m_debugNsElapsed;
    qint64 m_debugCounter;
    QString m_debugMsElapsed;
#endif
};

#endif // QNANOQUICKITEMPAINTER_H
