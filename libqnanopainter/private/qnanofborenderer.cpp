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

#include "qnanofborenderer.h"
#include <QSGSimpleTextureNode>

QNanoFBORenderer::QNanoFBORenderer()
    : m_itemPainter(nullptr)
    , m_window(nullptr)
{
}

QNanoFBORenderer::~QNanoFBORenderer()
{
    // This will handle cleaning up QNanoQuickItemPainter
    // Which is created by user in QNanoQuickItem::createItemPainter()
    if (m_itemPainter) {
        delete m_itemPainter;
        m_itemPainter = nullptr;
    }
}

void QNanoFBORenderer::setItemPainter(QNanoQuickItemPainter *itemPainter)
{
    m_itemPainter = itemPainter;
    if (m_itemPainter) {
        m_itemPainter->initialize();
    }
}

void QNanoFBORenderer::setWindow(QQuickWindow *window)
{
    if (m_window == window)
        return;
    m_window = window;
}

void QNanoFBORenderer::synchronize(QQuickFramebufferObject * item)
{
    if (m_itemPainter) {
        // If size has changed, update
        if ((int)m_itemPainter->width() != (int)item->width() || (int)m_itemPainter->height() != (int)item->height()) {
            m_itemPainter->setSize(item->width(), item->height());
            m_itemPainter->sizeChanged(item->width(), item->height());
            // Note: invalidated automatically when size changes if textureFollowsItemSize is true
            //invalidateFramebufferObject();
        }
        m_itemPainter->presynchronize(item);
    }
}

void QNanoFBORenderer::render()
{
    if (m_itemPainter) {
        m_itemPainter->render();
    }
    // Reset the GL state for Qt side
    if (m_window) {
        m_window->resetOpenGLState();
    }
}

QOpenGLFramebufferObject *QNanoFBORenderer::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    return new QOpenGLFramebufferObject(size, format);
}

