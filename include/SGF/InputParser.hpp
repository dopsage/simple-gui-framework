
/********** SIMPLE GUI FRAMEWORK INPUT PARSER MODULE **********/

#ifndef _INPUTPARSER_HPP
#define _INPUTPARSER_HPP

#include <vector>

#include "SFML/Graphics.hpp"

#include "SGF/Rectangle.hpp"
#include "SGF/Types.hpp"

namespace sgf
{

/* Defines how rectangles contained in a specified source vector are receiving SFML
 * keyboard and mouse events. The class also introduces lots of custom events, that
 * are pretty useful. */
class InputParser final
{
private:
    Rectangle*                  getHoveredRectanglePtr(Vector2D position);
    Rectangle*                  keyboardReceiverPtr;
    Rectangle*                  mouseReceiverPtr;
    std::vector<Rectangle*>*    rectanglesSourcePtr;

public:
    InputParser();
    bool    isKeyboardFree() const;
    void    parseSfmlEvent(const sf::Event& event);
    void    setKeyboardReceiverPtr(Rectangle* receiverPtr);
    void    setRectanglesSourcePtr(std::vector<Rectangle*>* sourcePtr);
};

}

#endif	// _INPUTPARSER_HPP
