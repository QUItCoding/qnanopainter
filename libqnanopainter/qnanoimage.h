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

#ifndef QNANOIMAGE_H
#define QNANOIMAGE_H

#include "nanovg/nanovg.h"
#include "private/qnanodataelement.h"
#include <QString>
#include <QDebug>

class QNanoPainter;

class QNanoImage
{
public:

    enum ImageFlag {
        GENERATE_MIPMAPS   = 1<<0, // Generate mipmaps during creation of the image.
        REPEATX            = 1<<1, // Repeat image in X direction.
        REPEATY            = 1<<2, // Repeat image in Y direction.
        FLIPY              = 1<<3, // Flips (inverses) image in Y direction when rendered.
        PREMULTIPLIED      = 1<<4, // Image data has premultiplied alpha.
        NEAREST            = 1<<5, // Image interpolation is Nearest instead Linear
    };

    Q_DECLARE_FLAGS(ImageFlags, ImageFlag)

    // Default constructor, creates uninitialized image
    QNanoImage();

    // Constructs an image with the filename and flags
    QNanoImage(const QString &filename, ImageFlags flags = nullptr);

    ~QNanoImage();

    // Set the filename of the image
    void setFilename(const QString &filename);

    // Set the flags for image
    void setFlags(ImageFlags flags);

    int width() const;
    int height() const;

private:
    friend class QNanoPainter;
    friend class QNanoImagePattern;

    // Load image (if needed) and return its id.
    // So currently image is loaded when drawing it for the first time
    int getID(NVGcontext* nvg);

    void setParentPainter(QNanoPainter *parentPainter);

    void updateUniqueKey();

    QNanoPainter *m_parentPainter;
    QNanoDataElement *m_imageData;
    QString m_filename;
    QNanoImage::ImageFlags m_flags;
    QString m_uniqueKey;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(QNanoImage::ImageFlags)

#endif // QNANOIMAGE_H
