#ifndef EVENT_HPP
#define EVENT_HPP

#include <cstdint>
#include <queue>

namespace wm
{
    class Window;

    enum class EventType {
        NO_EVENT,
        KEY_DOWN,
        KEY_UP,
        BUTTON_DOWN,
        BUTTON_UP,
        TEXT_INPUT,
        EXPOSE,
        MOTION,
        MOUSE_ENTER,
        MOUSE_LEAVE,
        RESIZE,
        CLOSE,
        FOCUS_GOT,
        FOCUS_LOST,
        SHOW,
        HIDE
    };

    typedef uint32_t KeyCode;
    typedef uint32_t KeyMod;

    typedef uint32_t Button;
    typedef uint32_t ButtonMask;

    struct AnyEvent
    {
        EventType type;
        Window *window;
        uint32_t x, y, a, b, c;
    };
    
    struct KeyEvent
    {
        EventType type;
        Window *window;
        KeyCode keycode; // native
        KeyMod keymod;
        bool repeat;
    };

    struct ButtonEvent
    {
        EventType type;
        Window *window;
        int x, y;
        Button button;
        ButtonMask buttons;
        KeyMod keymod;
    };

    struct CharacterEvent
    {
        EventType type;
        Window *window;
        uint32_t unicode;
    };

    struct ExposeEvent
    {
        EventType type;
        Window *window;
        int x, y;
        int width, height;
    };

    struct MotionEvent
    {
        EventType type;
        Window *window;
        int x, y;
        ButtonMask buttons;
        KeyMod keymod;
    };

    struct MouseOverEvent
    {
        EventType type;
        Window *window;
        int x, y;
    };

    struct ResizeEvent
    {
        EventType type;
        Window *window;
        int width, height;
    };

    typedef AnyEvent CloseEvent;
    typedef AnyEvent FocusEvent;
    typedef AnyEvent ShowEvent;

    union Event
    {
        EventType type;
        AnyEvent any;
        KeyEvent key;
        ButtonEvent button;
        CharacterEvent character;
        CloseEvent close;
        ExposeEvent expose;
        FocusEvent focus;
        MotionEvent motion;
        MouseOverEvent mouseover;
        ResizeEvent resize;
        ShowEvent show;
    };

    class EventQueue
    {
    protected:
        virtual bool push(const Event& event) = 0;
        // put with timeout?

    public:
        virtual ~EventQueue() {}

        friend class EventReader;
    };

    class SimpleQueue : public EventQueue
    {
    public:
        // single threaded queue
        bool empty() const { return queue.empty(); }
        const Event& peek() const { return queue.front(); }
        Event pop() // rvalue ref here?
        {
            Event event = queue.front();
            queue.pop();
            return event;
        }
        
    protected:
        virtual bool push(const Event& event)
        {
            queue.push(event);
        }

    private:
        std::queue<Event> queue;
    };

#if 0
    class ProducerConsumerEventQueue : public EventQueue
    {
    public:
        Event wait();
        bool poll(Event& event);

        // bool wait_for(Event& event, timeout);
        // bool wait_until(Event& event, timeout);

    }; 
#endif
}

#endif

