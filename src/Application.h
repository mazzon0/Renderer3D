#ifndef _APP
#define _APP

#include "platform/Window.h"

class Application {
public: /* TODO: keep or remove inlines? */
    static inline void init(int nArgs, char* args[]) { getInstance().iInit(nArgs, args); }
    static inline void run() { getInstance().iRun();}
    static inline void exit() { getInstance().iExit();}

    static inline void close() { getInstance().iClose(); }

private:
    void iInit(int nArgs, char* args[]);
    void iRun();
    void iExit();
    void iClose() { m_running = false; }

    void processEvent(Event& event);

    Window window;
    const char* m_executablePath;
    bool m_running;

private:
    Application();
    ~Application() = default;
    
    static inline Application& getInstance() {
        static Application instance;
        return instance;
    }

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
};

#endif
