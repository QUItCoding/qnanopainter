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

#ifndef QNANOBRUSH_H
#define QNANOBRUSH_H

#include "nanovg/nanovg.h"
class QNanoPainter;

// Base class of all Brushes: Gradients & ImagePattern
class QNanoBrush
{

public:
    QNanoBrush()
    {
        m_parentPainter = nullptr;
    }

    virtual ~QNanoBrush() {}

protected:

    // Reimplement this in Brush sub-classes
    // Implementations should update m_nvgPaint if needed and then return it
    virtual NVGpaint nvgPaint(NVGcontext* nvg) const = 0;

    mutable NVGpaint m_nvgPaint;

    QNanoPainter* parentPainter() const {
        return m_parentPainter;
    }

    void setParentPainter(QNanoPainter *parentPainter)
    {
        m_parentPainter = parentPainter;
    }

private:

    // Grant QNanoPainter access to private & protected members
    friend class QNanoPainter;

    QNanoPainter *m_parentPainter;

};

#endif // QNANOBRUSH_H
