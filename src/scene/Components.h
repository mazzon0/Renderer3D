#ifndef _COMPONENT
#define _COMPONENT

#include <stdint.h>

using Entity = uint64_t;

struct ParentComponent {
    Entity parent;
};

struct TransformComponent {

};

struct MeshComponent {

};

struct MaterialComponent {

};

struct LightComponent {

};

#endif
