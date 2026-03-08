
#include "SGF/VList.hpp"

using namespace sgf;

VList:: VList() :
        Rectangle::Rectangle(),
        memberPtrs()
{

}

void VList::copy(Rectangle* other)
{
    // Not implemented yet, there was no need ...
    throw std::logic_error("VList::copy is not implemented");
}

int VList::getCount() const
{
    return memberPtrs.size();
}

Rectangle* VList::getMemberPtr(int index)
{
    return (index > -1 && index < getCount()) ? memberPtrs[index] : nullptr;
}

void VList::insert(int index, Rectangle* rectanglePtr)
{
    if(index < 0 || index > getCount() || rectanglePtr == nullptr) return;
    
    // Sum of heights of members with indices below `index` is the new member vertical offset
    float   offset  = 0.0f;
    int     mi      = 0;
    for(; mi < index; mi++)
        offset += getMemberPtr(mi)->getHeight();
    
    // New rectangle instance is inserted to the list both visually and logically
    rectanglePtr->setPosition   ({ this->getX(), this->getY() + offset });
    rectanglePtr->setPriority   (this->getPriority() + 1);
    rectanglePtr->setVisible    (isVisible());
    memberPtrs.insert(memberPtrs.begin() + mi, rectanglePtr);
    
    // Members with indices above `index` need to be moved by the new member height down
    for(++mi; mi < getCount(); mi++)
        getMemberPtr(mi)->setPosition({
            this->getX(),
            getMemberPtr(mi)->getY() + rectanglePtr->getHeight()
        });
    
    // At the end, size of the list must also be updated
    setSize({
        std::max(rectanglePtr->getWidth(), this->getWidth()),   // Widest member tells the width
        this->getHeight() + rectanglePtr->getHeight()           // Sum of member heights tells the height
    });
}

void VList::push(Rectangle* rectanglePtr)
{
    VList::insert(getCount(), rectanglePtr);
}

void VList::remove(int index)
{
    if(index < 0 || index >= getCount()) return;
    
    /* Members with indices above `index` need to be moved up by height of the target.
     * Compute the maximum member (excluding the target) width simultaneously. */
    Rectangle*  targetPtr       = getMemberPtr(index);
    float       targetHeight    = targetPtr->getHeight();
    float       maxWidth        = 0.0f;
    for(int mi = 0; mi < getCount(); mi++)
    {
        Rectangle* memberPtr = getMemberPtr(mi);
        
        if(mi == index)
            continue;
        else if(mi > index)
            memberPtr->setPosition({
                this->getX(),
                memberPtr->getY() - targetHeight
            });
    
        if(memberPtr->getWidth() > maxWidth)
            maxWidth = memberPtr->getWidth();
    }
    
    // Target rectangle is removed from the list
    memberPtrs.erase(memberPtrs.begin() + index);
    
    // Finally size of the list must be updated
    setSize({ maxWidth, this->getHeight() - targetHeight });
}

void VList::remove(Rectangle* rectanglePtr)
{
    for(int mi = 0; mi < getCount(); mi++)
        if(getMemberPtr(mi) == rectanglePtr)
            VList::remove(mi);
}

void VList::setPosition(Vector2D position)
{
    // Move all members together with the area
    for(int mi = 0; mi < getCount(); mi++)
        getMemberPtr(mi)->setPosition({
            position.x,
            getMemberPtr(mi)->getY() - this->getY() + position.y
        });
    
	Rectangle::setPosition(position);
}

void VList::setPriority(int priority)
{
    Rectangle::setPriority(priority);

    // List members should be drawn above the list, which is still a rectangle
    for(int mi = 0; mi < getCount(); mi++)
        getMemberPtr(mi)->setPriority(priority + 1);
}

void VList::setVisible(bool visible)
{
    Rectangle::setVisible(visible);
    
    for(int mi = 0; mi < getCount(); mi++)
        getMemberPtr(mi)->setVisible(visible);
}
