
/********** SIMPLE GUI FRAMEWORK CONTEXT MODULE **********/

#ifndef _CONTEXT_HPP
#define _CONTEXT_HPP

#include <cmath>

#include "SFML/Graphics.hpp"

#include "SGF/Types.hpp"

namespace sgf
{

class Canvas;

/* Allows to draw over canvas, that is it. Every rectangle is passed a reference
 * pointer to instance of Context stored in canvas so they can utilize it and
 * implement custom styles. */
class Context final
{
    // Allow canvas pass the SFML window reference in order to draw on it
    friend class Canvas;

private:
    sf::RenderWindow* sfmlWindowPtr;
    
public:
    Context();
    
    void line(Vector2D start, Vector2D end, Color3D color, int thickness);
};

}

#endif	// _CONTEXT_HPP
