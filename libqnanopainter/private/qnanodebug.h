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

#ifndef QNANODEBUG_H
#define QNANODEBUG_H

#include "qnanopainter.h"
#include <QElapsedTimer>
#include <QString>

class QNanoDebug
{
public:
    QNanoDebug();
    void start();
    void paintDrawDebug(QNanoPainter *painter, float width, float height);

private:
    NVGdrawDebug m_drawDebug;
    QElapsedTimer m_debugTimer;
    QElapsedTimer m_debugUpdateTimer;
    qint64 m_debugNsElapsed;
    qint64 m_debugCounter;
    QString m_debugMsElapsed;
};

#endif // QNANODEBUG_H
