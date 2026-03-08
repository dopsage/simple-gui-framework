
/********** SIMPLE GUI FRAMEWORK VERTICAL LIST MODULE **********/

#ifndef _VLIST_HPP
#define _VLIST_HPP

#include <stdexcept>
#include <vector>

#include "SGF/Canvas.hpp"
#include "SGF/Rectangle.hpp"
#include "SGF/Types.hpp"

namespace sgf
{

/* Features a vertical list that can organize inserted rectangle instances starting
 * at top left corner of rectangle defining the list, in bottom direction.
 * Notice that list neither does add rectangle instances to a canvas automatically
 * nor removes them, it just organizes them nicely. */
class VList final : public Rectangle
{
private:
    std::vector<Rectangle*> memberPtrs;

public:
    VList();
    void        copy(Rectangle* other)                      override;
    int         getCount()                                  const;
    Rectangle*  getMemberPtr(int index);
    void        insert(int index, Rectangle* rectanglePtr);
    void        push(Rectangle* rectanglePtr);
    void        remove(int index);
    void        remove(Rectangle* rectanglePtr);
    void        setPosition(Vector2D position)              override;
    void        setPriority(int priority)                   override;
    void        setVisible(bool visible)                    override;
};

}

#endif	// _VLIST_HPP
