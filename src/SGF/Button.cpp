
#include "SGF/Button.hpp"

using namespace sgf;

void Button::onButtonMouseEvent(MouseEvent event, Vector2D position, Rectangle* instancePtr, Canvas* canvasPtr)
{
	Button* b = (Button*)instancePtr;
    
    /* Button feeds the listener only when both down and up mouse events occurred
     * in its rectangular bounds. */
	if(event == MouseEvent::DOWN)
	{
		b->isMouseDown = true;
	}
	else if(event == MouseEvent::UP && b->isMouseDown)
	{
		b->isMouseDown = false;
        if(b->listener != nullptr && b->contains(position))
            b->listener(instancePtr, canvasPtr);
	}
}

Button::Button() :
        Rectangle::Rectangle(),
        isMouseDown(false)
{
    setListener(nullptr);
    
	setMouseListener(Button::onButtonMouseEvent);
}

void Button::copy(Rectangle* other)
{
    Rectangle::copy(other);
    
    // Button copying resets its mouse listener, return it
    setMouseListener(Button::onButtonMouseEvent);
    
    // Assuming copying target is a button (it must be)
    Button* ob = (Button*)other;
    
    setListener(ob->getListener());     // listener
}

ButtonListener Button::getListener() const
{
    return listener;
}

void Button::setListener(ButtonListener callback)
{
    listener = callback;
}
