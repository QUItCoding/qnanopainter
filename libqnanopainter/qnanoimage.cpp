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
    , m_imageData(nullptr)
    , m_flags(0)
{
}

QNanoImage::QNanoImage(const QString &filename, ImageFlags flags)
    : m_parentPainter(nullptr)
    , m_imageData(nullptr)
    , m_filename(filename)
    , m_flags(flags)
{
    updateUniqueKey();
}

QNanoImage::~QNanoImage()
{
}

void QNanoImage::setFilename(const QString &filename)
{
    if (m_filename == filename)
        return;
    m_filename = filename;
    updateUniqueKey();
}

void QNanoImage::setFlags(ImageFlags flags)
{
    if (m_flags == flags)
        return;
    m_flags = flags;
    updateUniqueKey();
}

int QNanoImage::getID(NVGcontext* nvg)
{
    // Image cache should always exist at this point
    auto *dataCache = &m_parentPainter->m_dataCache;
    Q_ASSERT(dataCache);
    if (dataCache->contains(m_uniqueKey)) {
        // Image is in cache
        m_imageData = dataCache->value(m_uniqueKey);
    } else {
        // Image is not yet in cache, so load and add it
        QFile file(m_filename);
        if (!file.open(QFile::ReadOnly))
        {
            qWarning() << "Could not open image file: " << m_filename;
        } else {
            m_imageData = new QNanoDataElement();
            QByteArray array = file.readAll();
            int length = array.size();
            unsigned char* data = (unsigned char*)&array.constData()[0];
            m_imageData->id = nvgCreateImageMem(nvg, m_flags, data, length);
            nvgImageSize(nvg, m_imageData->id, &m_imageData->width, &m_imageData->height);
            dataCache->insert(m_uniqueKey, m_imageData);
        }
    }
    return m_imageData->id;
}

int QNanoImage::width() const
{
    if (!m_imageData) return 0;
    return m_imageData->width;
}

int QNanoImage::height() const
{
    if (!m_imageData) return 0;
    return m_imageData->height;
}

// ***** Private *****

/*!
   \internal
*/

void QNanoImage::setParentPainter(QNanoPainter *parentPainter)
{
    m_parentPainter = parentPainter;
}

void QNanoImage::updateUniqueKey()
{
    m_uniqueKey = m_filename;
    m_uniqueKey.append(QString::number(m_flags));
}
