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

#ifndef QNANOWIDGET_H
#define QNANOWIDGET_H

#include <QOpenGLWidget>
#include <QTouchEvent>
#include <QColor>
#include "qnanopainter.h"
#ifdef QNANO_DEBUG_RENDER
#include "private/qnanodebug.h"
#endif

class QNanoWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit QNanoWidget(QWidget *parent = nullptr);
    virtual ~QNanoWidget() Q_DECL_OVERRIDE {}

    QColor fillColor() const;
    void setFillColor(const QColor &color);

Q_SIGNALS:
    void touchSignal(QTouchEvent *event);
    void paintSignal(QNanoPainter *painter);

protected:
    virtual void paint(QNanoPainter *painter);

    // Reimplemented from QOpenGLWidget
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

#ifdef QNANO_ENABLE_TOUCH_SIGNALS
    bool event(QEvent *event) Q_DECL_OVERRIDE;
#endif

private:
    // These are internal
    void prepaint();
    void postpaint();

    QColor m_fillColor = {0, 0, 0, 0};
    QNanoPainter *m_painter = nullptr;
    bool m_setupDone = false;
#ifdef QNANO_DEBUG_RENDER
    QNanoDebug m_debug;
#endif

};

#endif // QNANOWIDGET_H
