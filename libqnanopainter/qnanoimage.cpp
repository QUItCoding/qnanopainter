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

/*!
    \class QNanoImage
    \brief QNanoImage is a class for loading and drawing images with QNanoPainter.
    \inmodule QNanoPainter

    QNanoImage can be rendered with QNanoPainter::drawImage() methods or used
    inside QNanoImagePattern.

    Here's how to render single image into specific position:
    \code
        ...
        QNanoImage image(":/images/icon.png", QNanoImage::GENERATE_MIPMAPS);
        painter()->drawImage(image, 10, 20);
        ...
    \endcode

    Here's how to fill item with QNanoImagePattern:
    \code
        ...
        QNanoImage patternImage(":/images/pattern1.png");
        QNanoImagePattern p1 = QNanoImagePattern(patternImage);
        p1.setImageSize(128, 128);
        painter()->setFillStyle(p1);
        painter()->fillRect(0, 0, width(), height());
        ...
    \endcode
*/

/*!
    \enum QNanoImage::ImageFlag

    ImageFlag is used to define properties of loaded image.
    \value GENERATE_MIPMAPS Generate mipmaps during creation of the image. Enabling this often improves rendering quality of scaled images, while consuming a bit more memory.
    \value REPEATX Repeat image in X direction. Enable this when image is used as tiled in QNanoImagePattern.
    \value REPEATY Repeat image in Y direction. Enable this when image is used as tiled in QNanoImagePattern.
    \value FLIPY Flips (inverses) image in Y direction when rendered.
    \value PREMULTIPLIED Image data has premultiplied alpha.
    \value NEAREST Image interpolation is Nearest instead Linear. By default Linear interpolation is used which gives smoother results when scaling the image, but if sharp pixelated outcome is preferred enable this flag.

    \sa setFlags()
*/

/*!
    \fn QNanoImage::QNanoImage()

    Constructs a default image object.
*/

QNanoImage::QNanoImage()
    : m_parentPainter(nullptr)
    , m_imageData(nullptr)
    , m_flags(nullptr)
{
}

/*!
    \fn QNanoImage::QNanoImage(const QString &filename, ImageFlags flags)

    Constructs a image by loading it with a \a filename and optional \a flags.

    \sa setFilename(), setFlags()
*/

QNanoImage::QNanoImage(const QString &filename, ImageFlags flags)
    : m_parentPainter(nullptr)
    , m_imageData(nullptr)
    , m_filename(filename)
    , m_flags(flags)
{
    updateUniqueKey();
}

/*!
    Destroys the QNanoImage.
*/

QNanoImage::~QNanoImage()
{
}

/*!
    \fn void QNanoImage::setFilename(const QString &filename)

    Sets to load an image with a \a filename.
*/

void QNanoImage::setFilename(const QString &filename)
{
    if (m_filename == filename)
        return;
    m_filename = filename;
    updateUniqueKey();
}

/*!
    \fn void QNanoImage::setFlags(ImageFlags flags)

    Sets image to use \a flags.
*/

void QNanoImage::setFlags(ImageFlags flags)
{
    if (m_flags == flags)
        return;
    m_flags = flags;
    updateUniqueKey();
}

/*!
    \fn int QNanoImage::width() const

    Returns the width of image in pixels.
*/

int QNanoImage::width() const
{
    if (!m_imageData) return 0;
    return m_imageData->width;
}

/*!
    \fn int QNanoImage::height() const

    Returns the height of image in pixels.
*/

int QNanoImage::height() const
{
    if (!m_imageData) return 0;
    return m_imageData->height;
}

// ***** Private *****

/*!
   \internal
*/

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
            unsigned char* data = reinterpret_cast<unsigned char*>(&array.data()[0]);
            m_imageData->id = nvgCreateImageMem(nvg, m_flags, data, length);
            nvgImageSize(nvg, m_imageData->id, &m_imageData->width, &m_imageData->height);
            dataCache->insert(m_uniqueKey, m_imageData);
        }
    }
    return m_imageData->id;
}

void QNanoImage::setParentPainter(QNanoPainter *parentPainter)
{
    m_parentPainter = parentPainter;
}

void QNanoImage::updateUniqueKey()
{
    m_uniqueKey = m_filename;
    m_uniqueKey.append(QString::number(m_flags));
}
