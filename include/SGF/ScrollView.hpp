
/********** SIMPLE GUI FRAMEWORK SCROLL VIEW MODULE **********/

#ifndef _SCROLLVIEW_HPP
#define _SCROLLVIEW_HPP

#include <stdexcept>

#include "SGF/Canvas.hpp"
#include "SGF/Rectangle.hpp"
#include "SGF/Types.hpp"
#include "SGF/VList.hpp"
#include "SGF/VSlider.hpp"

namespace sgf
{

class Canvas;

// NOTICE:  It would be nice to have VList listener so scroll view knows when to
//          update its slider height accordingly, when vlist member list changes ...

/* Combines VList and VSlider modules to compose vertically-organized rectangle list
 * that vertical position is controlled by the slider and aligned to the background
 * rectangle (scroll view instance itself) top and bottom depending on the slider value. */
class ScrollView : public Rectangle
{
private:
    VList       list;
    static void onSliderEvent(float value, Rectangle* instancePtr, Canvas* canvas);
    VSlider     slider;

public:
    ScrollView();
    void        copy(Rectangle* other)          override;
    VList*      getListPtr();
    VSlider*    getSliderPtr();
    void        onAdd()                         override;
    void        onRemove()                      override;
    void        setPosition(Vector2D position)  override;
    void        setPriority(int priority)       override;
    void        setSize(Vector2D size)          override;
    void        setSliderWidth(float width);
    void        setVisible(bool visible)        override;
};

}

#endif	// _SCROLLVIEW_HPP
