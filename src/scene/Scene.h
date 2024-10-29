#ifndef _SCENE
#define _SCENE

#include "ComponentSystem.h"
#include <string>

class Scene {
public:
    Scene(std::string& filename);
    Scene(const Scene& scene) = default;
    Scene& operator=(const Scene& scene) = default;
    ~Scene() = default;

    void update(float deltaTime);

private:
    ComponentSystem m_componentSystem;

    void importScene(const std::string& filename);
    void importBinary(const std::string& filename);
    void importText(const std::string& filename);

};

#endif
