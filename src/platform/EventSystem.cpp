#include "EventSystem.h"


EventSystem::EventSystem() {
    events.reserve(STD_CAPACITY);
}

void EventSystem::reset() {
    events.clear();
}

void EventSystem::forEach(const std::function<void(Event&)>& action) {
    for (std::unique_ptr<Event>& event : events) {
        if (event) {
            action(*event);
        }
    }
}
