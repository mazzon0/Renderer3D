#ifndef _INPUT
#define _INPUT

#include "Window.h"

namespace Input {
    bool isKeyDown(const Window& window, int key);
    void getCursorPos(const Window& window, double* x, double* y);
    bool isButtonPressed(const Window& window, int button);
}

#endif
