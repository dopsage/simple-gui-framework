
/********** SIMPLE GUI FRAMEWORK FUNCTION GRAPHER MODULE **********/

#ifndef _FUNCTIONGRAPHER_HPP
#define _FUNCTIONGRAPHER_HPP

#include <cmath>
#include <stdexcept>
#include <vector>

#include "SGF/Context.hpp"
#include "SGF/FunctionParser.hpp"
#include "SGF/FunctionProperties.hpp"
#include "SGF/Rectangle.hpp"
#include "SGF/Types.hpp"

namespace sgf
{

class Context;

struct ViewProperties
{
    // Notice:  these properties are updated by the setView call, writing them is silly
    //          there is no dedicated module for it because it is read-only and does not
    //          need any better versatility than primitive struct.
    
    float   pixelsPerUnit;  // [pixels] Amount of screen pixels per one graph plane unit
    float   hS;             // [units]  Horizontal start
    float   hE;             // [units]  Horizontal end
    int     heT;            // [units]  Horizontal end tick
    int     hsT;            // [units]  Horizontal start tick
    float   vS;             // [units]  Vertical start
    float   vE;             // [units]  Vertical end
    int     veT;            // [units]  Vectical end tick
    int     vsT;            // [units]  Vectical start tick
};

/* Uses canvas context graphics in order to render a 2-dimensional cartesian
 * plane in bounds of rectangle which it identifies as. The plane contains
 * horizontal and vertical lines that indicate whole numbers, additionally
 * origin axes (y=0 and x=0) are marked red. The grapher defines its own
 * plane (different than the canvas uses, which are pure screen coordinates)
 * so the units of its graphing plane remain in tact.
 * Personally, I noticed it can be used as some kind of a game engine specifically
 * for the tile-based ones (: */
class FunctionGrapher final : public Rectangle
{
private:
    bool                                cacheDefinition(FunctionProperties* function);
    std::vector<FunctionProperties*>    functionPtrs;
    bool                                isViewSet;
    static void                         onFunctionGrapherContextUse(Context* contextPtr, Rectangle* instancePtr, Canvas* canvasPtr);
    static const int                    PIXELS_PER_STEP;
    float                               step;
    ViewProperties                      view;

public:
    FunctionGrapher();
    void            add(FunctionProperties* function);
    void            copy(Rectangle* other)                  override;
    ViewProperties* getViewPtr();
    void            remove(FunctionProperties* function);
    Vector2D        toCanvasPlane(Vector2D grapherCoord)    const;
    Vector2D        toGrapherPlane(Vector2D canvasCoord)    const;
    void            setView(float hS, float hE, float vS);
};

}

#endif	// _FUNCTIONGRAPHER_HPP
