
/********** SIMPLE GUI FRAMEWORK VERTICAL SLIDER MODULE **********/

#ifndef _VSLIDER_HPP
#define _VSLIDER_HPP

#include "SGF/Canvas.hpp"
#include "SGF/Rectangle.hpp"
#include "SGF/Types.hpp"

namespace sgf
{

class Canvas;

typedef void (*VSliderListener)(float value, Rectangle* instancePtr, Canvas* canvasPtr);

/* Provides a vertical slider functionality by means of two rectangles. The class
 * derives from `Rectangle` to act as the slider background, but it also stores
 * a rectangle instance that acts as the slider handle. */
class VSlider final : public Rectangle
{
private:
	Rectangle       handle;
	float 		    handleOffsetY;
	bool 		    isHandleMouseDown;
    
	static void     onHandleMouseEvent(MouseEvent event, Vector2D position, Rectangle* instancePtr, Canvas* canvasPtr);
    
    // The callback is invoked when the slider value changes
    VSliderListener listener;
    
	float  	        value;

public:
	VSlider();
    void            copy(Rectangle* other)                  override;
    VSliderListener getListener()                           const;
    Rectangle*      getHandlePtr();
    
    /* Returns a number in closed range <0,1> which indicates a normalized position
     * of the handle relative to the background it slides along (0 indicates top). */
	float           getValue()                              const;
    
    void            onAdd()                                 override;
    void            onRemove()                              override;
	void            setHandleHeight(float height);
    void            setListener(VSliderListener callback);
	void            setPosition(Vector2D position)          override;
	void            setPriority(int priority)               override;
	void            setSize(Vector2D size)                  override;
    void            setVisible(bool visible)                override;
};

}

#endif	// _VSLIDER_HP
