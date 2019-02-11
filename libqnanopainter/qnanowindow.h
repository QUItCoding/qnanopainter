/**********************************************************
** Copyright (c) 2018 QUIt Coding <info@quitcoding.com>
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

#ifndef QNANOWINDOW_H
#define QNANOWINDOW_H

#include <QOpenGLWindow>
#include <QColor>
#include "qnanopainter.h"
#ifdef QNANO_DEBUG
#include "private/qnanodebug.h"
#endif

class QNanoWindow : public QOpenGLWindow
{
    Q_OBJECT
public:
    QNanoWindow(QOpenGLWindow::UpdateBehavior updateBehavior = NoPartialUpdate,
                QWindow *parent = nullptr);

    QColor fillColor() const;
    void setFillColor(const QColor &color);

Q_SIGNALS:
    void touchSignal(QTouchEvent *event);
    void paintSignal(QNanoPainter *painter);

protected:
    virtual void paint(QNanoPainter *painter);

    // Reimplemented from QOpenGLWindow
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
#ifdef QNANO_ENABLE_TOUCH_SIGNALS
    void touchEvent(QTouchEvent *event) Q_DECL_OVERRIDE;
#endif

private:
    // These are internal
    void prepaint();
    void postpaint();

    QColor m_fillColor;
    QNanoPainter *m_painter;
    bool m_setupDone;
#ifdef QNANO_DEBUG
    QNanoDebug m_debug;
#endif
};

#endif // QNANOWINDOW_H
