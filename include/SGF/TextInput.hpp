
/********** SIMPLE GUI FRAMEWORK TEXT INPUT MODULE **********/

#ifndef _TEXTINPUT_HPP
#define _TEXTINPUT_HPP

#include <vector>

#include "SGF/Canvas.hpp"
#include "SGF/Rectangle.hpp"
#include "SGF/TextProperties.hpp"
#include "SGF/Types.hpp"

namespace sgf
{
    
class Canvas;

typedef void (*TextInputListener)(std::wstring content, Rectangle* instancePtr, Canvas* canvasPtr);

/* NOTICE:  On Windows wchar_t stores half of unicode character (2 bytes)!
 *          this requires some exclusive support to include in the future ... */
typedef const std::vector<std::pair<wchar_t, wchar_t>> UnicodeRangeVector;

// All of unicode
const UnicodeRangeVector IF_ALL =
{
    {0x00000000, 0x0010FFFF}
};

// Visible ASCII
const UnicodeRangeVector IF_ASCII =
{
    {0x00000020, 0x0000007E}
};

// Math expression
const UnicodeRangeVector IF_MATH =
{
    {0x00000028, 0x0000002B},   // ( ) * +
    {0x00000030, 0x00000039},   // 0-9
    {0x0000002D, 0x0000002F},   // - . /
    {0x0000003D, 0x0000003D},   // =
    {0x00000041, 0x0000005A},   // A-Z
    {0x0000005E, 0x0000005E},   // ^
    {0x00000061, 0x0000007A}    // a-z
};

/* Provides a text input functionality with really simplistic controls. It is possible
 * to specify certain padding value of the input text, which automatically transforms
 * according to the current data. */
class TextInput final : public Rectangle
{
private:
    milliseconds        blinkDuration;
    Rectangle           cursor;
    int 			    cursorIndex;
    Rectangle           field;
    UnicodeRangeVector* filterPtr;
    bool                focused;
    bool                isFieldMouseDown;
    static const int    KEY_BACKSPACE;
    static const int    KEY_LEFT_ARROW;
    static const int    KEY_RETURN;
    static const int    KEY_RIGHT_ARROW;
    milliseconds        lastBlinkTime;
    int                 leftPadding;
    TextInputListener   listener;
    static void         onFieldKeyboardEvent(int keycode, wchar_t unicode, Rectangle* instancePtr, Canvas* canvasPtr);
    static void         onFieldMouseEvent(MouseEvent event, Vector2D position, Rectangle* instancePtr, Canvas* canvasPtr);
    void                updateCursorPosition();
    void                sanitizeContent();
    int                 verticalPadding;
    
public:
	TextInput();
    void                copy(Rectangle* other)                      override;
    milliseconds        getBlinkDuration()                          const;
    Rectangle*          getCursorPtr();
    Rectangle*          getFieldPtr();
    UnicodeRangeVector* getFilterPtr();
    int                 getLeftPadding()                            const;
    TextInputListener   getListener()                               const;
    int                 getVerticalPadding()                        const;
    bool                isAllowedByFilter(wchar_t unicode)          const;
    bool                isFocused()                                 const;
    void                onAdd() override;
    void                onRemove() override;
    void                onTick(int tickCount) override;
    void                setBlinkDuration(milliseconds duration);
    void                setCursorWidth(float width);
    void                setFieldText(TextProperties* textPtr);
    void                setFilterPtr(UnicodeRangeVector* filterPtr);
    void                setLeftPadding(float padding);
    void                setListener(TextInputListener callback);
    void                setPosition(Vector2D position)              override;
	void                setPriority(int priority)                   override;
	void                setSize(Vector2D size)                      override;
    void                setVerticalPadding(float padding);
    void                setVisible(bool visible)                    override;
};

}

#endif	// _TEXTINPUT_HPP
