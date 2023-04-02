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

#ifndef QNANOFONT_H
#define QNANOFONT_H

#include <QString>

class QNanoPainter;
struct NVGcontext;

class QNanoFont
{
public:

    enum FontId {
        DEFAULT_FONT_THIN,
        DEFAULT_FONT_THIN_ITALIC,
        DEFAULT_FONT_LIGHT,
        DEFAULT_FONT_LIGHT_ITALIC,
        DEFAULT_FONT_NORMAL,
        DEFAULT_FONT_NORMAL_ITALIC,
        DEFAULT_FONT_BOLD,
        DEFAULT_FONT_BOLD_ITALIC
    };

    QNanoFont();
    QNanoFont(FontId fontId);
    QNanoFont(const QString &filename);
    ~QNanoFont();

    void setFilename(const QString &filename);
    void setFontId(FontId fontId);
    void setPixelSize(float size);
    void setPointSize(float size);
    void setBlur (float blur);
    void setLetterSpacing(float spacing);
    void setLineHeight(float lineHeight);

private:
    friend class QNanoPainter;

    int getID(NVGcontext* nvg);
    void setParentPainter(QNanoPainter *parentPainter);

    QNanoPainter *m_parentPainter = nullptr;
    QString m_filename;
    int m_id = 0;
    // Note: These should be matching to nanovg nvgReset state
    float m_size = 16.0f;
    float m_blur = 0.0f;
    float m_spacing = 0.0f;
    float m_lineHeight = 1.0f;
    bool m_fontPropertiesChanged = false;
    bool m_fontFilenameChanged = true;
};

#endif // QNANOFONT_H
