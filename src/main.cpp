#include "Application.h"

int main(int argc, char* argv[]) {
    Application::init(argv[0]);
    Application::run();
    Application::close();
}