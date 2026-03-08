
/********** SIMPLE GUI FRAMEWORK RECTANGLE MODULE **********/

#ifndef _RECTANGLE_HPP
#define _RECTANGLE_HPP

#include <stdexcept>

#include "SFML/Graphics.hpp"

#include "SGF/TextProperties.hpp"
#include "SGF/Types.hpp"

namespace sgf
{
    
class Canvas;
class Context;
class Rectangle;

enum class MouseEvent { DOWN, MOVE, UP };

typedef void (*ContextListener) (Context* contextPtr, Rectangle* instancePtr, Canvas* canvasPtr);
typedef void (*KeyboardListener)(int keycode, wchar_t unicode, Rectangle* instancePtr, Canvas* canvasPtr);
typedef void (*MouseListener)   (MouseEvent event, Vector2D position, Rectangle* instancePtr, Canvas* canvasPtr);

/* Literal SFML drawable rectangle (sf::RectangleShape) wrapper, that introduces features
 * of a primitive interactive rectangular area. Rectangle may declare use of text,
 * in such case canvas associates SFML drawable text (sf::Text) with the rectangle.
 * The text is controlled by playing with text properties instance that every rectangle
 * instance contains and allows to alter (see Types module). */
class Rectangle
{
    // Allow canvas to access `sfmlRect` in drawing purposes, this is just a simple trust
    friend class Canvas;

private:
    bool                added;
    Canvas*             canvasPtr;
	Color3D	            color;
    ContextListener     contextListener;
    int                 id;
    KeyboardListener    keyboardListener;
    MouseListener       mouseListener;
    void*               metaPtr;
    Vector2D 		    position;
	int 			    priority;
	sf::RectangleShape  sfmlRect;
	Vector2D 		    size;
	TextProperties*     textPtr;
    bool                visible;

public:
	Rectangle();
	virtual ~Rectangle();
    bool                    contains(Vector2D position) const;
    virtual void            copy(Rectangle* other);
    Canvas*                 getCanvasPtr();
	Color3D	                getColor()                  const;
    ContextListener         getContextListener()        const;
    float                   getHeight()                 const;
    int                     getId()                     const;
    KeyboardListener        getKeyboardListener()       const;
    void*                   getMetaPtr();
    MouseListener           getMouseListener()          const;
    Vector2D                getPosition()               const;
	int 	 		        getPriority()               const;
	Vector2D                getSize()                   const;
    TextProperties*         getText();
    float                   getWidth()                  const;
    float                   getX()                      const;
    float                   getY()                      const;
    bool                    isAdded()                   const;
    bool                    isUsingText()               const;
    bool                    isVisible()                 const;
    
    /* These methods invoke set listeners, and can be used for their emulation.
     * Just saying: canvas calls onContextUse every frame in appropriate moment,
     *              so its emulation is of no use anyways. */
    void                    onContextUse(Context* contextPtr);
    void                    onKeyboardInput(int keycode, wchar_t unicode);
    void                    onMouseInput(MouseEvent event, Vector2D position);
    
    virtual void            onAdd();
    virtual void            onRemove();
    virtual void            onTick(int tickCount);
    virtual void            setColor(Color3D color);
    
    // Rectangle can store only one callback method per device, did not see reason for not doing so
    void                    setContextListener(ContextListener callback);
    void                    setKeyboardListener(KeyboardListener callback);
    void                    setMouseListener(MouseListener callback);
    
    void                    setMetaPtr(void* ptr);
    virtual void            setPosition(Vector2D position);
	virtual void            setPriority(int priority);
	virtual void            setSize(Vector2D size);
    virtual void            setText(TextProperties* text);
    virtual void            setVisible(bool visible);
};

}

#endif	// _RECTANGLE_HPP
