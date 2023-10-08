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
#include <QFile>

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
{
}

/*!
    \fn QNanoImage::QNanoImage(const QString &filename, ImageFlags flags)

    Constructs a image by loading it with a \a filename and optional \a flags.

    \sa setFilename(), setFlags()
*/

QNanoImage::QNanoImage(const QString &filename, ImageFlags flags)
    : m_filename(filename)
    , m_flags(flags)
{
    updateUniqueKey();
}

QNanoImage::QNanoImage(const QImage &image, const QString &filename, ImageFlags flags)
    : m_image(std::make_unique<QImage>(image))
    , m_filename(filename)
    , m_flags(flags)
{
    updateUniqueKey();
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
    m_textureId = 0;
    updateUniqueKey();
}

/*!
    \fn void QNanoImage::setFrameBuffer(const QOpenGLFramebufferObject *fbo)

    Sets to grab an image from a \a fbo. Also automatically sets flags
    to QNanoImage::FLIPY to have image in correct orientation.
*/

void QNanoImage::setFrameBuffer(const QOpenGLFramebufferObject *fbo)
{
    Q_ASSERT(fbo);
    m_imageData.reset(new QNanoDataElement());
    m_imageData->width = fbo->width();
    m_imageData->height = fbo->height();
    m_textureId = fbo->texture();
    m_flags |= QNanoImage::FLIPY;
    m_filename.clear();
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
    if (!m_imageData)
        return 0;
    return m_imageData->width;
}

/*!
    \fn int QNanoImage::height() const

    Returns the height of image in pixels.
*/

int QNanoImage::height() const
{
    if (!m_imageData)
        return 0;
    return m_imageData->height;
}

/*!
    \fn QNanoImage QNanoImage::fromFrameBuffer(const QOpenGLFramebufferObject *fbo)

    Static convenience function that returns a image constructed
    from QOpenGLFramebufferObject \a fbo. Optional \a flags paramater
    can be used to set specific flags, by default it is set to QNanoImage::FLIP
    so that image will be in correct orientation. Image size will match to
    fbo size.

    See https://doc.qt.io/qt-5/qopenglframebufferobject.html
*/

QNanoImage QNanoImage::fromFrameBuffer(const QOpenGLFramebufferObject *fbo, ImageFlags flags)
{
    Q_ASSERT(fbo);
    QNanoImage image;
    image.m_imageData.reset(new QNanoDataElement());
    image.m_imageData->width = fbo->width();
    image.m_imageData->height = fbo->height();
    image.m_textureId = fbo->texture();
    image.m_flags = flags;
    image.updateUniqueKey();
    return image;
}

QNanoImage QNanoImage::fromCache(QNanoPainter *painter, const QString &filename, ImageFlags flags)
{
    Q_ASSERT(painter);
    QNanoImage image;
    image.m_imageData.reset(new QNanoDataElement());
    image.m_filename = filename;
    image.m_flags = flags;
    image.updateUniqueKey();
    image.m_parentPainter = painter;
    image.getID(painter->nvgCtx());
    return image;
}

// ***** Private *****

/*!
   \internal
*/

int QNanoImage::getID(NVGcontext* nvg)
{
    // Image cache should always exist at this point
    Q_ASSERT(m_parentPainter);
    if (!m_parentPainter)
        return -1;
#ifdef QNANO_USE_RHI
    auto *dataCache = &m_localDataCache;
#else
    auto *dataCache = &m_parentPainter->m_dataCache;
#endif
    if (dataCache->contains(m_uniqueKey)) {
        // Image is in cache
        m_imageData = dataCache->value(m_uniqueKey);
    } else if (m_imageData && m_textureId > 0) {
        m_imageData->id = m_parentPainter->m_backend->nvglCreateImageFromHandle(nvg, m_textureId,
                                                                                m_imageData->width, m_imageData->height,
                                                                                m_flags);
        dataCache->insert(m_uniqueKey, m_imageData);
    } else if (m_image) {
        if (m_image->size().isEmpty()) {
            qWarning() << "Empty image: " << m_filename;
        } else {
            if (m_image->format() != QImage::Format_RGBA8888 && m_image->format() != QImage::Format_RGBA8888_Premultiplied) {
                qWarning() << "Converting image" << m_filename << "from" << m_image->format() << "to" << QImage::Format_RGBA8888_Premultiplied;
                m_image->convertTo(QImage::Format_RGBA8888_Premultiplied);
            }
            m_imageData.reset(new QNanoDataElement());
            QNanoImage::ImageFlags flags = m_flags;
            if (m_image->format() == QImage::Format_RGBA8888_Premultiplied)
                flags |= QNanoImage::PREMULTIPLIED;
            else
                flags &= ~QNanoImage::PREMULTIPLIED;
            m_imageData->id = nvgCreateImageRGBA(nvg, m_image->width(), m_image->height(), flags, m_image->bits());
            nvgImageSize(nvg, m_imageData->id, &m_imageData->width, &m_imageData->height);
            dataCache->insert(m_uniqueKey, m_imageData);
        }
    } else {
        // Image is not yet in cache, so load and add it
        QFile file(m_filename);
        if (file.open(QFile::ReadOnly)) {
            m_imageData.reset(new QNanoDataElement());
            QByteArray array = file.readAll();
            int length = array.size();
            unsigned char* data = reinterpret_cast<unsigned char*>(&array.data()[0]);
            m_imageData->id = nvgCreateImageMem(nvg, m_flags, data, length);
            nvgImageSize(nvg, m_imageData->id, &m_imageData->width, &m_imageData->height);
            dataCache->insert(m_uniqueKey, m_imageData);
        }
    }
    return m_imageData ? m_imageData->id : -1;
}

void QNanoImage::setParentPainter(QNanoPainter *parentPainter)
{
    m_parentPainter = parentPainter;
}

void QNanoImage::updateUniqueKey()
{
    if (m_textureId > 0)
        m_uniqueKey = QString("%1_").arg(QString::number(m_textureId));
    else
        m_uniqueKey = m_filename;

    m_uniqueKey.append(QString::number(m_flags));
}
