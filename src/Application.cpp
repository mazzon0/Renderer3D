#include "Application.h"
#include "platform/Platform.h"
#include <utility>

Application::Application() : window(RESIZABLE) {}

void Application::iInit(const char* exePath) {
    m_executablePath = exePath;
    m_running = true;

    window.show("Renderer 3D", 700, 400);
}

void Application::iRun() {
    while(m_running) {
        Platform::sendEvents();
        
        window.forEachEvent(std::function<void(Event&)>([this](Event& event) {
            this->processEvent(event);
        }));
    }
}

void Application::iExit() {
}

void Application::processEvent(Event& event) {
    switch(event.getEventType()) {
        case EventType::WINDOW_CLOSE:
            Application::close();
            break;
    }
}