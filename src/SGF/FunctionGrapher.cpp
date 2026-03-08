
//#include <iostream>
#include "SGF/FunctionGrapher.hpp"

using namespace sgf;

bool FunctionGrapher::cacheDefinition(FunctionProperties* function)
{
    // Parse the function using dedicated module
    FunctionParser parser(function->definition);
    if(parser.getError() != FPError::NONE)
        return false;
    
    // Fill mapping cache
    function->mappingCache.clear();
    int     sC  = this->getWidth() / PIXELS_PER_STEP;   // [-]      step count
    float   sL  = (view.hE - view.hS) / sC;             // [units]  step length
    for(int sI = 0; sI <= sC; sI++)
    {
        float fA = view.hS + sI * sL;   // Function argument
        float fV = parser.getValue(fA); // Function value
        function->mappingCache[fA] = fV;
    }
    return true;
}

void FunctionGrapher::onFunctionGrapherContextUse(Context* contextPtr, Rectangle* instancePtr, Canvas* canvasPtr)
{
    FunctionGrapher* fg = (FunctionGrapher*)instancePtr;
    
    if(!fg->isViewSet) return;

    /*** Draw graphing plane unit ticks for both axes ***/
    
    for(int hgC = fg->view.hsT; hgC <= fg->view.heT; hgC++)
    {
        float hcC = fg->toCanvasPlane({ (float)hgC, 0.0f }).x;
        contextPtr->line(
            { hcC, fg->getY() },
            { hcC, fg->getY() + fg->getHeight() },
            { sgf::byte(!hgC ? 255 : 0), 0, 0 },
            2
        );
    }
    for(int vgC = fg->view.vsT; vgC <= fg->view.veT; vgC++)
    {
        float vcC = fg->toCanvasPlane({ 0.0f, (float)vgC }).y;
        contextPtr->line(
            { fg->getX(), vcC },
            { fg->getX() + fg->getWidth(), vcC },
            { sgf::byte(!vgC ? 255 : 0), 0, 0 },
            2
        );
    }
    
    /*** Draw registered function graphs using their mapping cache ***/
    
    for(FunctionProperties* function : fg->functionPtrs)
    {
        // Refresh mapping cache of the registered function if it has the refresh flag set
        if(function->refreshFlag)
        {
            if(!fg->cacheDefinition(function))
                // Function definition parsing error, cannot generate cache
                continue;
                
            function->refreshFlag = false;
        }
        
        /* With ensured freshness of mapping cache, draw the graph by drawing lines
         * that approximate the perfect graph curve. Remember about coorinate plane
         * transformations! */
        auto left   = function->mappingCache.begin();
        auto right  = function->mappingCache.begin();
        for(++right; right != function->mappingCache.end(); left++, right++)
        {
            contextPtr->line(
                { fg->toCanvasPlane({ left->first,  left->second  }) },
                { fg->toCanvasPlane({ right->first, right->second }) },
                { 0, 0, 255 },
                3
            );
        }
    }
}

const int FunctionGrapher::PIXELS_PER_STEP = 4;

FunctionGrapher::   FunctionGrapher() :
                    Rectangle::Rectangle(),
                    functionPtrs(),
                    isViewSet(false),
                    step(0.0f)
{
    setContextListener(FunctionGrapher::onFunctionGrapherContextUse);
}

void FunctionGrapher::add(FunctionProperties* function)
{
    if(std::find(functionPtrs.begin(), functionPtrs.end(), function) == functionPtrs.end())
    {
        // Trigger immediate update before registration
        function->refreshFlag = true;
        functionPtrs.push_back(function);
    }
}

void FunctionGrapher::copy(Rectangle* other)
{
    // Not implemented yet, there was no need ...
    throw std::logic_error("FunctionGraph::copy is not implemented");
}

ViewProperties* FunctionGrapher::getViewPtr()
{
    return &view;
}

void FunctionGrapher::remove(FunctionProperties* function)
{
    std::vector<FunctionProperties*>::iterator it =
    std::find(functionPtrs.begin(), functionPtrs.end(), function);
    
    if(it != functionPtrs.end())
        functionPtrs.erase(it);
}

Vector2D FunctionGrapher::toCanvasPlane(Vector2D grapherCoord) const
{
    return
    {
        // Actually i got this by solving for canvasCoord from the equations in toGrapherPlane
        (grapherCoord.x - view.hS) * view.pixelsPerUnit + this->getX(),
        this->getY() - (grapherCoord.y - view.vE) * view.pixelsPerUnit
    };
}

Vector2D FunctionGrapher::toGrapherPlane(Vector2D canvasCoord) const
{
    return
    {
        // Simple math, remember about screen coordinate space (Y grows downwards!)
        view.hS + (canvasCoord.x - this->getX()) / view.pixelsPerUnit,
        view.vE - (canvasCoord.y - this->getY()) / view.pixelsPerUnit
    };
}

void FunctionGrapher::setView(float hS, float hE, float vS)
{
    if(this->getWidth() < 1 || this->getHeight() < 1 || hE <= hS) return;

    view.pixelsPerUnit  = this->getWidth() / (hE - hS);

    // Initialize horizontal axis
    view.hE    = hE;
    view.hS    = hS;
    view.heT   = std::floor(hE);
    view.hsT   = std::ceil(hS);
    
    // Initialize vertical axis
    view.vE    = vS + this->getHeight() / view.pixelsPerUnit;
    view.vS    = vS;
    view.veT   = std::floor(view.vE);
    view.vsT   = std::ceil(vS);
    
    isViewSet = true;
    
    // Schedule all registered functions for refreshal so they fit the new view
    for(FunctionProperties* function : functionPtrs)
        function->refreshFlag = true;
}
