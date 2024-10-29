#ifndef _IMP_SCENELOADER
#define _IMP_SCENELOADER

#include "ComponentSystem.h"
#include "Mesh.h"
#include <assimp/scene.h>
#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>

struct MeshFindData {
    Mesh mesh;
    unsigned int index;
    MeshCode code;
};

class SceneLoader {
public:
    SceneLoader() : m_loaded(false) {}

    void load(const std::string filename);
    void saveBin(const std::string filename);
    void saveText(const std::string filename);

    bool successfullyLoaded() { return m_loaded; }

private:
    ComponentSystem m_componentSystem;
    std::vector<MeshFindData> m_meshes;
    std::vector<Entity> m_entities;
    bool m_loaded;

    void loadAllMeshes(const aiScene* scene);
    void loadMesh(const aiMesh* mesh, unsigned int index);
    void loadVertex(const aiVector3D& pos, const aiVector3D& texCoords, const aiVector3D& normal, const aiVector3D& tangent, const aiVector3D& bitangent, Mesh& mesh);
    void loadEntities(const aiNode* node, const aiScene* scene, Entity parent);
    void loadEntity(unsigned int meshIndex, Entity parent, Entity entity);
    MeshCode getMeshCode(unsigned int meshIndex);
    Entity randomEntity();
    void saveHeavyData(const std::string& filename);
    void saveMesh(MeshFindData& mesh, const std::string& folder);
    void saveSceneYaml(const std::string& filename);
    void emitEntity(Entity entity, YAML::Emitter& emitter);
};

#endif
