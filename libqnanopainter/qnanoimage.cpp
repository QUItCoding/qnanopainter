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

#include "qnanoimage.h"
#include "qnanopainter.h"
#include <QDir>

QNanoImage::QNanoImage()
    : m_parentPainter(nullptr)
    , m_flags(0)
    , m_id(-1)
    , m_width(0)
    , m_height(0)
{
}

QNanoImage::QNanoImage(const QString &filename, ImageFlags flags)
    : m_parentPainter(nullptr)
    , m_filename(filename)
    , m_flags(flags)
    , m_id(-1)
    , m_width(0)
    , m_height(0)
{
}

QNanoImage::~QNanoImage()
{
}

void QNanoImage::setFilename(const QString &filename)
{
    if (m_filename == filename)
        return;
    m_filename = filename;
}

void QNanoImage::setFlags(ImageFlags flags)
{
    m_flags = flags;
}

int QNanoImage::getID(NVGcontext* nvg)
{
    // Image cache should always exist at this point
    auto *dataCache = &m_parentPainter->m_dataCache;
    Q_ASSERT(dataCache);
    QString uniqueKey(m_filename);
    uniqueKey.append(QString::number(m_flags));
    if (dataCache->contains(uniqueKey)) {
        // Image is in cache
        const QNanoDataElement *f = dataCache->value(uniqueKey);
        m_width = f->width;
        m_height = f->height;
        m_id = f->id;
    } else {
        // Image is not yet in cache, so load and add it
        QNanoDataElement *f = new QNanoDataElement();
        QFile file(m_filename);
        if (!file.open(QFile::ReadOnly))
        {
            qWarning() << "Could not open image file: " << m_filename;
        } else {
            QByteArray array = file.readAll();
            int length = array.size();
            unsigned char* data = (unsigned char*)&array.constData()[0];
            m_id = nvgCreateImageMem(nvg, m_flags, data, length);
            f->id = m_id;
            nvgImageSize(nvg, m_id, &m_width, &m_height);
            f->width = m_width;
            f->height = m_height;
            dataCache->insert(uniqueKey, f);
        }
    }
    return m_id;
}

int QNanoImage::width() const
{
    return m_width;
}

int QNanoImage::height() const
{
    return m_height;
}

// ***** Private *****

/*!
   \internal
*/

void QNanoImage::setParentPainter(QNanoPainter *parentPainter)
{
    m_parentPainter = parentPainter;
}
