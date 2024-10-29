#ifndef _IMP_COMPONENTSYSTEM
#define _IMP_COMPONENTSYSTEM

#include "Components.h"
#include <unordered_map>
#include <tuple>
#include <iostream>

class ComponentSystem {
public:
    template<typename T>
    void add(Entity entity, T& component);

    template<typename T>
    T& get(Entity entity);

    template<typename T>
    bool hasComponent(Entity entity);   // TODO: just 'has'?

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

template<typename T>
T& ComponentSystem::get(Entity entity) {
    return getMap<T>()[entity];
}

template<typename T>
bool ComponentSystem::hasComponent(Entity entity) {
    auto& map = getMap<T>();
    return (map.find(entity) != map.end());
}

#endif
