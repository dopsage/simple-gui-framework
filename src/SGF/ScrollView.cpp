
#include "SGF/ScrollView.hpp"

using namespace sgf;

void ScrollView::onSliderEvent(float value, Rectangle* instancePtr, Canvas* canvasPtr)
{
    VSlider*    s   = (VSlider*)instancePtr;
    ScrollView* v   = (ScrollView*)s->getMetaPtr();

    /* Update the list position according to the slider value in such a way
     * that makes the list aligned with bottom of the scroll view (background)
     * when slider value is 1.0 and with the top if the value is 0.0 */
    float maxOffset = std::min(0.0f, v->getHeight() - v->list.getHeight());
    v->list.setPosition({ v->list.getX(), v->getY() + value * maxOffset });
}

ScrollView::ScrollView() :
            Rectangle::Rectangle(),
            list(),
            slider()
{
    setSliderWidth(25.F);
    
    slider.setListener(ScrollView::onSliderEvent);
    slider.setMetaPtr(this);
}

void ScrollView::copy(Rectangle* other)
{
    // Not implemented yet, there was no need ...
    throw std::logic_error("ScrollView::copy is not implemented");
}

VList* ScrollView::getListPtr()
{
    return &list;
}

VSlider* ScrollView::getSliderPtr()
{
    return &slider;
}

void ScrollView::onAdd()
{
    getCanvasPtr()->add(&list);
    getCanvasPtr()->add(&slider);
}

void ScrollView::onRemove()
{
    getCanvasPtr()->remove(&list);
    getCanvasPtr()->remove(&slider);
}

void ScrollView::setPosition(Vector2D position)
{
    // First move the list and the slider according to the new view position
    list.setPosition({
        position.x + slider.getWidth(),
        position.y + list.getY() - this->getY()
    });
    slider.setPosition(position);
    
	Rectangle::setPosition(position);
}

void ScrollView::setPriority(int priority)
{
    Rectangle::  setPriority(priority);
    list        .setPriority(priority + 1);
	slider      .setPriority(priority + 1);
}

void ScrollView::setSize(Vector2D size)
{
    Rectangle::  setSize(size);
    list        .setSize({ this->getWidth() - slider.getWidth(), list.getHeight() });
    slider      .setSize({ slider.getWidth(), this->getHeight() });
}

void ScrollView::setSliderWidth(float width)
{
    // List needs to be offset by slider width horizontally axis
    list    .setPosition({ this->getX() + width, this->getY() });
    slider  .setSize({ width < 0.0f ? 0.0f : width, this->getHeight() });
}

void sgf::ScrollView::setVisible(bool visible)
{
    Rectangle::  setVisible(visible);
    list        .setVisible(visible);
    slider      .setVisible(visible);
}
