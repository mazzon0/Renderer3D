#ifndef _IMP_MESH
#define _IMP_MESH

#include "Components.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

struct Vertex {
    Vertex(glm::vec3 p, glm::vec2 tc, glm::vec3 n, glm::vec3 t, glm::vec3 b)
        : pos(p), texCoords(tc), norm(n), tang(t), bitang(b) {}

    glm::vec3 pos;
    glm::vec2 texCoords;
    glm::vec3 norm;
    glm::vec3 tang;
    glm::vec3 bitang;
};

struct Mesh {
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
};

struct MeshFindData {
    Mesh mesh;
    MeshCode code;
    unsigned int index;
    unsigned int materialIndex;
};

enum TextureFormat : uint8_t {
    RGBA8888,
    ARGB8888,
    RGBA5650,
    RGBA0010
};

struct MaterialFindData {
    unsigned int index;
    std::string albedoPath;
    std::string emissionPath;
    std::string normalPath;
    std::string metalnessPath;
    std::string roughnessPath;
};

#endif
