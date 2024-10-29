#ifndef _IMP_COMPONENT
#define _IMP_COMPONENT

#include <glm/glm.hpp>
#include <stdint.h>

using Entity = uint64_t;

using MeshCode = uint64_t;

struct ParentComponent {
    Entity parent;
};

struct TransformComponent {
    glm::vec3 translation;
    glm::vec3 scale;
    glm::vec3 rotation;
};

struct MeshComponent {
    MeshCode code;
};

struct MaterialComponent {

};

struct LightComponent {

};

#endif
