
#include "SGF/VSlider.hpp"

using namespace sgf;

void VSlider::onHandleMouseEvent(MouseEvent event, Vector2D position, Rectangle* instancePtr, Canvas* canvasPtr)
{
	/* It is known, that the listening rectangle is actually the slider handle,
     * therefore pointer cast is a safe way of accessing the instance. */
	Rectangle*  h = (Rectangle*)instancePtr;
    
    /* After all, it is also guaranteed that metadata of the handle instance obtained 
     * above is the controlling slider instance pointer, thus we can access it. */
	VSlider*    s = (VSlider*)h->getMetaPtr();
    
	if(event == MouseEvent::DOWN)
	{
		s->isHandleMouseDown    = true;
		s->handleOffsetY        = h->getY() - position.y;
	}
	else if(s->isHandleMouseDown && event == MouseEvent::MOVE)
	{
		// Clamp the slider vertical position to the slider background area
		float maxHandleY    = s->getY()     + s->getHeight() - h->getHeight();
		float newHandleY    = position.y    + s->handleOffsetY;
		newHandleY = newHandleY < s->getY() ? s->getY() : (newHandleY > maxHandleY ? maxHandleY : newHandleY);
		
		// Use clamped position to update the slider handle
		h->setPosition({ h->getX(), newHandleY });

		// Update slider value
		s->value = (h->getY() - s->getY()) / (maxHandleY - s->getY());
        
        /* Invoke the slider listener due to the slider value change.
         * It is possible to access private members since we are still in a member
         * (this static function) of a class to which these private fields belong. */
        if(s->listener != nullptr)
            s->listener(s->value, s, canvasPtr);
	}
	else if(event == MouseEvent::UP)
	{
		s->isHandleMouseDown = false;
	}
}

VSlider::   VSlider() :
            Rectangle::Rectangle(),
            handle(),
            handleOffsetY(0.0f),
            isHandleMouseDown(false),
            value(0.0f)
{
	setListener(nullptr);
    setHandleHeight(50.0f);
    
    /* Pass the slider instance pointer for handle mouse event listener in order
     * to allow the handle to access the instance. */
    handle.setMetaPtr(this);
    
    handle.setMouseListener(VSlider::onHandleMouseEvent);
}

void VSlider::copy(Rectangle* other)
{
    Rectangle::copy(other);
    
    /* Assuming copying target is a vertical slider (it must be).
     * Copying handle resets important data, which must be restored afterwards.
     * The instance fields are copied lastly. */
    VSlider* os = (VSlider*)other;
    
    handle.copy(os->getHandlePtr());   // handle
    handle.setMetaPtr(this);
    handle.setMouseListener(VSlider::onHandleMouseEvent);
    
    setListener(os->getListener());    // listener
}

VSliderListener VSlider::getListener() const
{
    return listener;
}

Rectangle* VSlider::getHandlePtr()
{
    return &handle;
}

float VSlider::getValue() const
{
	return value;
}

void VSlider::setHandleHeight(float height)
{
    // Guess setter is enough convienience, no direct getter needed
	handle.setSize({
        handle.getWidth(),
        height < 0.0f ? 0.0f : (height > this->getHeight() ? this->getHeight() : height)
    });
}

void VSlider::setListener(VSliderListener callback)
{
    listener = callback;
}

void VSlider::onAdd()
{
    Rectangle::onAdd();
    
    getCanvasPtr()->add(&handle);
}

void VSlider::onRemove()
{
    Rectangle::onRemove();
    
    getCanvasPtr()->remove(handle.getId());
}

void VSlider::setPosition(Vector2D position)
{
    // First move the handle according to the new background's position
	handle.setPosition({
		handle.getX() - this->getX() + position.x,
		handle.getY() - this->getY() + position.y,
	});
    
	Rectangle::setPosition(position);
}

void VSlider::setPriority(int priority)
{
	sgf::Rectangle:: setPriority(priority);
	handle          .setPriority(priority + 1);  // Draw the handle above background
}

void VSlider::setSize(Vector2D size)
{
	Rectangle::setSize(size);
    
    // Handle width is dependant on the background's, but its height is variable
	handle.setSize({ this->getWidth(), handle.getHeight() });
}

void VSlider::setVisible(bool visible)
{
    Rectangle::  setVisible(visible);
    handle      .setVisible(visible);
}
