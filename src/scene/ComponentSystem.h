#ifndef _COMPONENTSYSTEM
#define _COMPONENTSYSTEM

#include "Components.h"
#include <unordered_map>
#include <tuple>
#include <iostream>

class ComponentSystem {
public:
    template<typename T>
    void add(Entity entity, T& component);

private:
    std::tuple<
        std::unordered_map<Entity, ParentComponent>,
        std::unordered_map<Entity, TransformComponent>,
        std::unordered_map<Entity, MeshComponent>,
        std::unordered_map<Entity, MaterialComponent>,
        std::unordered_map<Entity, LightComponent>
        > m_components;

    template <typename T>
    std::unordered_map<Entity, T>& getMap();
};

template<typename T>
std::unordered_map<Entity, T>& ComponentSystem::getMap() {
    return std::get<std::unordered_map<Entity, T>>(m_components);
}

template<typename T>
void ComponentSystem::add(Entity entity, T& component) {
    getMap<T>().emplace(entity, component);
}

#endif
