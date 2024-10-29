#include "Application.h"

int main(int argc, char* argv[]) {
    Application::init(argc, argv);
    Application::run();
    Application::close();
}