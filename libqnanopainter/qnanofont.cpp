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

#include "qnanofont.h"
#include "nanovg/nanovg.h"
#include <QFile>
#include <QDebug>
#include "qnanopainter.h"

/*!
    \class QNanoFont
    \brief QNanoFont is a class for setting a font for QNanoPainter text painting.
    \inmodule QNanoPainter

    QNanoFont caches all loaded fonts so calling any constructor, \a setFilename(),
    \a setFontId() etc. in QPainter paint() method is a cheap operation.

    TODO: Write more documentation here.
*/

/*!
    \enum QNanoFont::FontId

    FontId is used to load a default fonts. These fonts cover the normal use needs
    so that applications do not necessarily need to include own TrueType font files.
    Fonts are differentiated by their weight (thickness) and normal or italic style.
    \value DEFAULT_FONT_THIN Thin font.
    \value DEFAULT_FONT_THIN_ITALIC Thin italic font.
    \value DEFAULT_FONT_LIGHT Light font.
    \value DEFAULT_FONT_LIGHT_ITALIC Light italic font.
    \value DEFAULT_FONT_NORMAL Normal font.
    \value DEFAULT_FONT_NORMAL_ITALIC Normal italic font.
    \value DEFAULT_FONT_BOLD Bold font.
    \value DEFAULT_FONT_BOLD_ITALIC Bold italic font.

    \sa setFontId()
*/

/*!
    \fn QNanoFont::QNanoFont()

    Constructs a default font object.
    Default style of a font is FontId:DEFAULT_FONT_NORMAL at size 16px.
*/

QNanoFont::QNanoFont()
{
}

/*!
    \fn QNanoFont::QNanoFont(const QString &filename)

    Constructs a font by loading TrueType font with a \a filename.

    \sa setFilename()
*/

QNanoFont::QNanoFont(const QString &filename)
    : m_filename(filename)
{
}

/*!
    \fn QNanoFont::QNanoFont(FontId fontId)

    Constructs a font by loading font with a \a fontId.

    \sa setFontId()
*/

QNanoFont::QNanoFont(FontId fontId)
{
    setFontId(fontId);
}

/*!
    Destroys the QNanoFont.
*/

QNanoFont::~QNanoFont()
{
}

/*!
    \fn void QNanoFont::setFilename(const QString &filename)

    Sets to load a TrueType font with a \a filename.
*/

void QNanoFont::setFilename(const QString &filename)
{
    if (m_filename == filename)
        return;
    m_filename = filename;
    m_fontFilenameChanged = true;
}

/*!
    \fn void QNanoFont::setFontId(FontId fontId)

    Sets to load a TrueType font with a \a fontId.
*/

void QNanoFont::setFontId(FontId fontId)
{
    switch (fontId)
    {
    case QNanoFont::DEFAULT_FONT_THIN:
        setFilename(":/qnanopainter/data/Roboto-Thin.ttf");
        break;
    case QNanoFont::DEFAULT_FONT_THIN_ITALIC:
        setFilename(":/qnanopainter/data/Roboto-ThinItalic.ttf");
        break;
    case QNanoFont::DEFAULT_FONT_LIGHT:
        setFilename(":/qnanopainter/data/Roboto-Light.ttf");
        break;
    case QNanoFont::DEFAULT_FONT_LIGHT_ITALIC:
        setFilename(":/qnanopainter/data/Roboto-LightItalic.ttf");
        break;
    case QNanoFont::DEFAULT_FONT_NORMAL:
        setFilename(":/qnanopainter/data/Roboto-Regular.ttf");
        break;
    case QNanoFont::DEFAULT_FONT_NORMAL_ITALIC:
        setFilename(":/qnanopainter/data/Roboto-Italic.ttf");
        break;
    case QNanoFont::DEFAULT_FONT_BOLD:
        setFilename(":/qnanopainter/data/Roboto-Bold.ttf");
        break;
    case QNanoFont::DEFAULT_FONT_BOLD_ITALIC:
        setFilename(":/qnanopainter/data/Roboto-BoldItalic.ttf");
        break;
    }
}

/*!
    \fn void QNanoFont::setPixelSize(float size)

    Sets the height of the font to \a size in pixels.
*/

void QNanoFont::setPixelSize(float size)
{
    m_size = size;
    m_fontPropertiesChanged = true;
}

/*!
    \fn void QNanoFont::setPointSize(float size)

    Sets the height of the font to \a size in points.
*/

void QNanoFont::setPointSize(float size)
{
    m_size = QNanoPainter::ptToPx(size);
    m_fontPropertiesChanged = true;
}

/*!
    \fn void QNanoFont::setBlur (float blur)

    Sets the blur amount of the font to \a blur.
    This is most useful for shadow/glowing effect by painting text
    first with a blurred font and the over it with a non-blurred font.
*/

void QNanoFont::setBlur (float blur)
{
    m_blur = blur;
    m_fontPropertiesChanged = true;
}

/*!
    \fn void QNanoFont::setLetterSpacing(float spacing)

    Sets the font letter spacing in pixels to \a spacing. This means the
    amount of (extra) space in between letters. The default value is 0.
    Letter spacing can be also negative to tighten the appearance of text.
*/

void QNanoFont::setLetterSpacing(float spacing)
{
    m_spacing = spacing;
    m_fontPropertiesChanged = true;
}

/*!
    \fn void QNanoFont::setLineHeight(float lineHeight)

    Sets the line height of the text to \a lineHeight. This means the
    amount of space in between text lines when text is wrapped to multiple
    lines. The default value is 1.0.
*/

void QNanoFont::setLineHeight(float lineHeight)
{
    m_lineHeight = lineHeight;
    m_fontPropertiesChanged = true;
}

// ***** Private *****

int QNanoFont::getID(NVGcontext* nvg)
{
    if (m_fontPropertiesChanged) {
        nvgFontSize(nvg, m_size);
        nvgFontBlur(nvg, m_blur);
        nvgTextLetterSpacing(nvg, m_spacing);
        nvgTextLineHeight(nvg, m_lineHeight);
        m_fontPropertiesChanged = false;
    }

    if (m_filename.isEmpty()) {
        // No font file set, so loading the default font
        setFontId(QNanoFont::DEFAULT_FONT_NORMAL);
    }

    if (m_fontFilenameChanged) {
        m_id = nvgFindFont(nvg,m_filename.toUtf8().constData());
        m_fontFilenameChanged = false;
    }

    if (m_id == -1) {
        // Font is not yet in cache, so load and add it
        QFile file(m_filename);
        if (!file.open(QFile::ReadOnly))
        {
            qWarning() << "Could not open font file: " << m_filename;
        } else {
            size_t length = static_cast<size_t>(file.bytesAvailable());
            char * data = static_cast<char*>(malloc(length));
            file.read(data, static_cast<qint64>(length));
            m_id = nvgCreateFontMem(nvg, m_filename.toUtf8().constData(),
                                    reinterpret_cast<unsigned char*>(data),
                                    static_cast<int>(length), 1);
            file.close();
        }
    }
    return m_id;
}

void QNanoFont::setParentPainter(QNanoPainter *parentPainter)
{
    m_parentPainter = parentPainter;
}
