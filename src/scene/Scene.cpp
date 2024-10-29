#include "Scene.h"
#include <iostream>

Scene::Scene(std::string& filename) {
    importScene(filename);
}

void Scene::update(float deltaTime) {

}

void Scene::importScene(const std::string& filename) {
    // file postfix
    size_t dotPos = filename.find_last_of('.');
    std::string postfix = filename.substr(dotPos+1, filename.size()-dotPos-1);
    if(postfix.compare(".sb3d")) {
        importBinary(filename);
    }
    else if(postfix.compare(".st3d")) {
        importText(filename);
    }
    else {
        std::cout << "ERROR: cannot open this file: " << filename << std::endl;
    }

}

void Scene::importBinary(const std::string& filename) {
    std::cout << "ERROR: binary scene importing has not been implemented: " << filename << std::endl;
}

void Scene::importText(const std::string& filename) {
    std::cout << "ERROR: text scene importing has not been implemented: " << filename << std::endl;
}