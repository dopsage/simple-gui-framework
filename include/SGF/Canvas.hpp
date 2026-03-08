
/********** SIMPLE GUI FRAMEWORK CANVAS MODULE **********/

#ifndef _CANVAS_HPP
#define _CANVAS_HPP

#include <chrono>
#include <unordered_map>
#include <string>
#include <vector>

#include "SFML/Graphics.hpp"

#include "SGF/Context.hpp"
#include "SGF/InputParser.hpp"
#include "SGF/Rectangle.hpp"
#include "SGF/Types.hpp"

namespace sgf
{

/* Core of SGF, it facilitates rendering of a window and its contents in form of
 * simple rectangular areas (Rectangle instances) by wrapping SFML features in a
 * way that makes the framework intuitive and primitive (but simple). It also defines
 * a Context instance which manages drawing over it, I advise looking at its module.*/
class Canvas final
{
private:
    milliseconds                        constructionTime;
    Context                             context;
    int                                 drawingRate;
    static const std::string            RES_FONT_FILE;
    milliseconds                        getEpochTime() const;
    InputParser                         inputParser;
    bool                                active;
    Vector2D                            position;
    std::vector<Rectangle*>             rectanglePtrs;
    std::unordered_map<int, sf::Text>   rectangleTexts;
    sf::Font                            sfmlFont;
    sf::RenderWindow                    sfmlWindow;
    Vector2D                            size;
    int                                 tickCount;
    std::string                         title;

public:
    Canvas();
    void                add(Rectangle* rectanglePtr);
    Context*            getContextPtr();
    int                 getDrawingRate()                    const;
    milliseconds        getElapsedTime()                    const;
    float 		        getHeight()                         const;
    InputParser*        getInputParserPtr();
    Vector2D            getPosition()                       const;
    Rectangle*          getRectanglePtr(int rectangleId);
    Vector2D            getSize()                           const;
    int                 getTickCount()                      const;
    std::string         getTitle()                          const;
    float 		        getWidth()                          const;
    float 		        getX()                              const;
    float 		        getY()                              const;
    bool                isActive()                          const;
    void                stop();
    void                remove(int rectangleId);
    void                remove(Rectangle* rectanglePtr);
    void                setDrawingRate(int amount);
    void                setPosition(Vector2D position);
    void                setSize(Vector2D size);
    void                setTitle(std::string title);
    bool                tick();
};

}

#endif	// _CANVAS_HPP
