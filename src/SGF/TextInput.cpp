
#include "SGF/TextInput.hpp"

using namespace sgf;

const int TextInput::KEY_BACKSPACE      = (int)sf::Keyboard::Key::Backspace;
const int TextInput::KEY_LEFT_ARROW     = (int)sf::Keyboard::Key::Left;
const int TextInput::KEY_RETURN         = (int)sf::Keyboard::Key::Return;
const int TextInput::KEY_RIGHT_ARROW    = (int)sf::Keyboard::Key::Right;

void TextInput::onFieldKeyboardEvent(int keycode, wchar_t unicode, Rectangle* instancePtr, Canvas* canvasPtr)
{
	Rectangle*      f    = instancePtr;
	TextInput*      ti   = (TextInput*)f->getMetaPtr();
    TextProperties* text = f->getText();
    
	switch(keycode)
	{
		case TextInput::KEY_BACKSPACE:
		{
			// Erase a character at the cursor position and move the cursor to left
			if(ti->cursorIndex > -1)
			{
				text->content.erase(ti->cursorIndex, 1);
                text->length--;
                text->refreshFlag = true;
                
                ti->cursorIndex--;
			}
			break;
		}
		case TextInput::KEY_LEFT_ARROW:
		{
			// Move the cursor to left (limit at -1)		
			ti->cursorIndex = (ti->cursorIndex > -1) ? (ti->cursorIndex - 1) : (-1);
            ti->updateCursorPosition();
			break;
		}
		case TextInput::KEY_RETURN:
		{
			// Text input can be defocused
			canvasPtr->getInputParserPtr()->setKeyboardReceiverPtr(nullptr);
            ti->cursor.setVisible(false);
            ti->focused = false;
            
            // Notify the listener about finished inputting
            if(ti->listener != nullptr)
                ti->listener(text->content, ti, canvasPtr);
            
			break;
		}
		case sgf::TextInput::KEY_RIGHT_ARROW:
		{
			// Move the cursor to right (limit to length-1)
			ti->cursorIndex =   (ti->cursorIndex < text->length - 1) ?
                                (ti->cursorIndex + 1) :
                                (text->length - 1);
            ti->updateCursorPosition();
			break;
		}
        
        default:
        {
            if(unicode && unicode != 0x00000008 && ti->isAllowedByFilter(unicode))
            {
                /* There is a unicode character inputted (not backspace), update
                 * content at the cursor with it. */
                text->content.insert(ti->cursorIndex + 1, 1, unicode);
                text->length++;
                text->refreshFlag = true;
                
                ti->cursorIndex++;
            }
    
            break;
        }
	}
}

void TextInput::onFieldMouseEvent(MouseEvent event, Vector2D position, Rectangle* instancePtr, Canvas* canvasPtr)
{
    Rectangle* f    = instancePtr;
    TextInput* ti   = (TextInput*)f->getMetaPtr();
 
    // No associated text, no further processing!
    if(!f->isUsingText()) return;
    
    if(event == MouseEvent::DOWN)
    {
        ti->isFieldMouseDown = true;
    }
    else if( event == sgf::MouseEvent::UP                       &&
             ti->isFieldMouseDown                               &&
            !ti->isFocused()                                    &&
             canvasPtr->getInputParserPtr()->isKeyboardFree()   &&
             f->contains(position))
    {
        /* Text input can be focused. Before allowing user to input data, current
         * text content needs to be sanitized, and cursor needs to be set up. */
        ti->sanitizeContent();
        ti->updateCursorPosition();
        ti->cursor.setVisible(true);
        ti->cursorIndex         = f->getText()->length - 1;
        ti->isFieldMouseDown    = false;
        ti->focused             = true;
        
        canvasPtr->getInputParserPtr()->setKeyboardReceiverPtr(&ti->field);
    }
}

void TextInput::updateCursorPosition()
{
    /* If text content is scheduled to be refreshed, it may change character widths
     * array which this method depends on. For safety reasons in such case method
     * is canceled. Detection of text refreshal is done in onTick method (see it). */
    if(!field.isUsingText() || field.getText()->refreshFlag) return;
    
    // Add character widths until the cursor position to get the horizontal shift
    float shift = 0.0f;
    for(int ci = 0; ci < cursorIndex + 1; ci++)
        shift += field.getText()->characterWidths[ci];
    
    cursor.setPosition({
        field.getX() + shift - cursor.getWidth(),
        field.getY()
    });
}

void TextInput::sanitizeContent()
{
    if(!field.isUsingText()) return;
    
    int             ci      = 0;
    TextProperties* text    = field.getText();
    while(ci < text->length)
    {
        if(!isAllowedByFilter(text->content[ci]))
        {
            /* This is an invalid character (as of the current filter), it needs
             * to be erased, thus the loop needs to be delayed due to length change. */
            text->content.erase(ci, 1);
            text->length--;
            text->refreshFlag = true;
        }
        else ci++;
    }
    
    text->refreshFlag = true;
}


TextInput:: TextInput() :
            Rectangle::Rectangle(),
            cursor(),
            cursorIndex(-1),
            field(),
            focused(false),
            isFieldMouseDown(false),
            lastBlinkTime(0)
{
    setBlinkDuration(250);      // blinkDuration
    setCursorWidth(2);
    setFieldText(nullptr);
    setFilterPtr(&IF_ALL);      // filterPtr
    setLeftPadding(0.0f);       // leftPadding
    setListener(nullptr);       // listener
    setVerticalPadding(0.0f);   // verticalPadding
    
    cursor.setVisible(false);
    field.setKeyboardListener(TextInput::onFieldKeyboardEvent);
    field.setMetaPtr(this);
    field.setMouseListener(TextInput::onFieldMouseEvent);
}

void TextInput::copy(Rectangle* other)
{
    Rectangle::copy(other);
    
    /* Assuming copying target is a text input (it must be).
     * Copying cursor and field resets important data, which must be restored afterwards.
     * The instance fields are copied lastly. */
    TextInput* oti = (TextInput*)other;
    
    cursor  .copy(oti->getCursorPtr());                             // cursor
    cursor  .setVisible(false);
    
    field   .copy(oti->getFieldPtr());                              // field
    field   .setKeyboardListener(TextInput::onFieldKeyboardEvent);
    field   .setMetaPtr(this);
    field   .setMouseListener(TextInput::onFieldMouseEvent);
    
    setBlinkDuration    (oti->getBlinkDuration());                  // blinkDuration
    setCursorWidth      (oti->getCursorPtr()->getWidth());
    setFieldText        (oti->getFieldPtr()->getText());
    setFilterPtr        (oti->getFilterPtr());                      // filterPtr
    setLeftPadding      (oti->getLeftPadding());                    // leftPadding
    setListener         (oti->getListener());                       // listener
    setVerticalPadding  (oti->getVerticalPadding());                // verticalPadding
}

milliseconds TextInput::getBlinkDuration() const
{
    return blinkDuration;
}

Rectangle* TextInput::getCursorPtr()
{
    return &cursor;
}

Rectangle* TextInput::getFieldPtr()
{
    return &field;
}

UnicodeRangeVector* TextInput::getFilterPtr()
{
    return filterPtr;
}

int TextInput::getLeftPadding() const
{
    return leftPadding;
}

TextInputListener TextInput::getListener() const
{
    return listener;
}

int TextInput::getVerticalPadding() const
{
    return verticalPadding;
}

bool TextInput::isAllowedByFilter(wchar_t unicode) const
{
    // If unicode falls into allowed range, stop and declare allowance
    for(std::pair<wchar_t, wchar_t> range : *filterPtr)
        if(unicode >= range.first && unicode <= range.second)
            return true;

    return false;
}

bool TextInput::isFocused() const
{
    return focused;
}

void TextInput::onAdd()
{
    Rectangle::onAdd();
    
    getCanvasPtr()->add(&cursor);
    getCanvasPtr()->add(&field);
}

void TextInput::onRemove()
{
    Rectangle::onRemove();
    
    // Before removal, field should be defocused from keyboard to free it
    if(focused) getCanvasPtr()->getInputParserPtr()->setKeyboardReceiverPtr(nullptr);
    
    getCanvasPtr()->remove(&cursor);
    getCanvasPtr()->remove(&field);
}

void TextInput::onTick(int tickCount)
{
    // There is no point in blinking logic if text input is hidden or not focused
    if(!field.isUsingText() || !isVisible() || !focused) return;
    
    // SR latch using metadata storage in order to update cursor after text refreshes 
         if( field.getText()->refreshFlag && cursor.getMetaPtr() == nullptr)
    {
        cursor.setMetaPtr((void*)0x14052025);
    }
    else if(!field.getText()->refreshFlag && cursor.getMetaPtr() != nullptr)
    {
        updateCursorPosition();
        cursor.setMetaPtr(nullptr);
    }
    
    // Do the cursor blinking
    if(blinkDuration > 0)
    {
        milliseconds currentTime = getCanvasPtr()->getElapsedTime();
        if(currentTime - lastBlinkTime >= blinkDuration)
        {
            cursor.setVisible(!cursor.isVisible());
            lastBlinkTime = currentTime;
        }
    }
    else if(isVisible() && !cursor.isVisible())
        cursor.setVisible(true);
}

void TextInput::setBlinkDuration(milliseconds duration)
{
    blinkDuration = duration;
}

void TextInput::setCursorWidth(float width)
{
    cursor.setSize({ width < 0.0f ? 0.0f : width, cursor.getHeight() });
}

void TextInput::setFieldText(TextProperties* textPtr)
{
    field.setText(textPtr);
}

void TextInput::setFilterPtr(UnicodeRangeVector* filterPtr)
{
    this->filterPtr = filterPtr;
}

void TextInput::setLeftPadding(float padding)
{
    leftPadding = padding;
    
    // Reseting position and size to the same value triggers recalculation of cursor and field
    setPosition (getPosition());
    setSize     (getSize());
}

void TextInput::setListener(TextInputListener callback)
{
    listener = callback;
}

void TextInput::setPosition(Vector2D position)
{
    Rectangle::setPosition(position);
    
    field.setPosition({
        this->getX() + leftPadding,
        this->getY() + verticalPadding
    });
    updateCursorPosition();
}

void TextInput::setPriority(int priority)
{
    sgf::Rectangle:: setPriority(priority);
    cursor          .setPriority(priority + 2);
    field           .setPriority(priority + 1);
}

void TextInput::setSize(Vector2D size)
{
    Rectangle::setSize(size);
    
    float fieldHeight = this->getHeight() - 2 * verticalPadding;
    cursor  .setSize({ cursor.getWidth(), fieldHeight });
    field   .setSize({ this->getWidth() - leftPadding, fieldHeight });
}

void TextInput::setVerticalPadding(float padding)
{
    verticalPadding = padding;
    
    // Again like in setting the left padding, reset transformations to trigger recalculations
    setPosition(getPosition());
    setSize(getSize());
}

void TextInput::setVisible(bool visible)
{
    Rectangle::setVisible(visible);
    
    // You cannot show cursor together with the text input, but you can hide it
    if(!visible) cursor.setVisible(false);
    
    field.setVisible(visible);
}
