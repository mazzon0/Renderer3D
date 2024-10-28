#ifndef _EVENTS
#define _EVENTS

#include <vector>
#include <string>

enum class EventType {
    KEY,
    CHAR,
    DROP,
    SCROLL,
    CURSOR_ENTER,
    CURSOR_POS,
    MOUSE_BUTTON,
    WINDOW_CLOSE,
    WINDOW_RESIZE,
    WINDOW_FOCUS,
    WINDOW_ICONIFY
};

class Event {
public:
    bool m_handled;
    Event() {m_handled = false;}
    virtual EventType getEventType() const = 0;
};

// children of Event
class KeyEvent final : public Event {
public:
    int m_key, m_scancode, m_action, m_mods;
    KeyEvent(int key, int scancode, int action, int mods) : m_key(key), m_scancode(scancode), m_action(action), m_mods(mods) {}
    EventType getEventType() const override {return EventType::KEY;}
};

class CharEvent final : public Event {
public:
    uint32_t m_codepoint;
    int m_mods;
    CharEvent(uint32_t codepoint, int mods) : m_codepoint(codepoint), m_mods(mods) {}
    EventType getEventType() const override { return EventType::CHAR; }
};

class DropEvent final : public Event {
public:
    std::vector<std::string> m_paths;
    DropEvent(int count, const char* paths[]) {
        m_paths.reserve(count);
        for (int i = 0; i < count; ++i) {
            m_paths.emplace_back(paths[i]);
        }
    }
    EventType getEventType() const override { return EventType::DROP; }
};

class ScrollEvent final : public Event {
public:
    double m_dx, m_dy;
    ScrollEvent(double dx, double dy) : m_dx(dx), m_dy(dy) {}
    EventType getEventType() const override { return EventType::SCROLL; }
};

class CursorEnterEvent final : public Event {
public:
    int m_entered;
    CursorEnterEvent(int entered) : m_entered(entered) {}
    EventType getEventType() const override { return EventType::CURSOR_ENTER; }
};

class CursorPosEvent final : public Event {
public:
    double m_x, m_y;
    CursorPosEvent(double x, double y) : m_x(x), m_y(y) {}
    EventType getEventType() const override { return EventType::CURSOR_POS; }
};

class MouseButtonEvent final : public Event {
public:
    int m_button, m_action, m_mods;
    MouseButtonEvent(int button, int action, int mods) : m_button(button), m_action(action), m_mods(mods) {}
    EventType getEventType() const override { return EventType::MOUSE_BUTTON; }
};

class WindowCloseEvent final : public Event {
public:
    WindowCloseEvent() = default;
    EventType getEventType() const override { return EventType::WINDOW_CLOSE; }
};

class WindowResizeEvent final : public Event {
public:
    int m_width, m_height;
    WindowResizeEvent(int width, int height) : m_width(width), m_height(height) {}
    EventType getEventType() const override { return EventType::WINDOW_RESIZE; }
};

class WindowFocusEvent final : public Event {
public:
    int m_focused;
    WindowFocusEvent(int focused) : m_focused(focused) {}
    EventType getEventType() const override { return EventType::WINDOW_FOCUS; }
};

class WindowIconifyEvent final : public Event {
public:
    int m_iconified;
    WindowIconifyEvent(int iconified) : m_iconified(iconified) {}
    EventType getEventType() const override { return EventType::WINDOW_ICONIFY; }
};

#endif
