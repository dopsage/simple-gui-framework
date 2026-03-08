
/********** SIMPLE GUI FRAMEWORK TYPES HEADER **********/

#ifndef _TYPES_HPP
#define _TYPES_HPP

#include <cstdint>

namespace sgf
{
    typedef uint8_t     byte;
    typedef uint32_t    milliseconds;
    struct  Color3D     { byte r, g, b; };
    struct  Vector2D    { float x, y; };
}

#endif	// _TYPES_HPP
