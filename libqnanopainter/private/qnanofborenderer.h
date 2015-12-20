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

#ifndef QNANOFBORENDERER_H
#define QNANOFBORENDERER_H

#include "qnanoquickitem.h"
#include "qnanoquickitempainter.h"
#include <QtGui/QOpenGLFramebufferObject>
#include <QtQuick/QQuickWindow>

class QNanoFBORenderer : public QQuickFramebufferObject::Renderer
{

public:
    explicit QNanoFBORenderer();
    ~QNanoFBORenderer();

    // Set the item painter class which will handle rendering into FBO
    void setItemPainter(QNanoQuickItemPainter *itemPainter);
    // Set to point QQuickWindow
    void setWindow(QQuickWindow *window);

protected:
    // Re-implemented from QQuickFramebufferObject::Renderer

    // Creates initial FBO.
    // Gets also called whenever item size changes if textureFollowsItemSize = true
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) Q_DECL_OVERRIDE Q_DECL_FINAL;

    // Gets called when FBO should be rendered into.
    void render() Q_DECL_OVERRIDE Q_DECL_FINAL;

    // Gets called as a result of QQuickFramebufferObject::update().
    void synchronize(QQuickFramebufferObject * item) Q_DECL_OVERRIDE Q_DECL_FINAL;

private:
    QNanoQuickItemPainter *m_itemPainter;
    QQuickWindow *m_window;
};

#endif // QNANOFBORENDERER_H
