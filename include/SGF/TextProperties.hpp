
/********** SIMPLE GUI FRAMEWORK TEXT PROPERTIES MODULE **********/

#ifndef _TEXTPROPERTIES_HPP
#define _TEXTPROPERTIES_HPP

#include <memory>
#include <string>

#include "SGF/Types.hpp"

namespace sgf
{

struct TextProperties
{
    // Constant
    Color3D         color;      // Uniform color
    std::wstring    content;    // Contained text data in unicode (UTF-16 for Windows, UTF-32 for Linux/Mac)
    int             size;       // Character size in pixels
    
    // Dynamic (they may be externally changed)
    std::unique_ptr<int[]>  characterWidths;    // Widths of all content unicode characters in pixels
    int                     length;             // Amount of unicode characters used to form the content
    bool                    refreshFlag;        // If set, canvas updates the SFML text(s) with the properties and resets the flag
    
    TextProperties();
    TextProperties(Color3D color, std::wstring content, int size);
    TextProperties(const TextProperties& other);
    TextProperties operator=(const TextProperties& other);
};

}
    
#endif	// _TEXTPROPERTIES_HPP
