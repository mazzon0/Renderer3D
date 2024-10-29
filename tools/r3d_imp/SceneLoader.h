#ifndef _IMP_SCENELOADER
#define _IMP_SCENELOADER

#include "ComponentSystem.h"
#include "Data.h"
#include <assimp/scene.h>
#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>

class SceneLoader {
public:
    SceneLoader() : m_loaded(false), m_saved(false) {}

    void load(const std::string filename);
    void saveBin(const std::string filename);
    void saveText(const std::string filename);

    bool successfullyLoaded() { return m_loaded; }
    bool successfullySaved() { return m_saved; }

private:
    ComponentSystem m_componentSystem;
    std::vector<MeshFindData> m_meshes;
    std::vector<MaterialFindData> m_materials;
    std::vector<std::string> m_textures;
    std::vector<Entity> m_entities;
    std::string m_folder;
    bool m_loaded;
    bool m_saved;

    void loadAllMeshes(const aiScene* scene);
    void loadMesh(const aiMesh* mesh, unsigned int index);
    void loadVertex(const aiVector3D& pos, const aiVector3D& texCoords, const aiVector3D& normal, const aiVector3D& tangent, const aiVector3D& bitangent, Mesh& mesh);

    void loadAllMaterials(const aiScene* scene);
    void loadMaterial(const aiMaterial* material, unsigned int index);
    bool isTextureLoaded(const std::string& path);

    void loadEntities(const aiNode* node, const aiScene* scene, Entity parent);
    void loadEntity(unsigned int meshIndex, Entity parent, Entity entity);
    const MeshFindData& getMesh(unsigned int meshIndex);
    const MaterialFindData& getMaterial(unsigned int materialIndex);
    Entity randomEntity();

    void saveHeavyData(const std::string& filename);
    void saveMesh(MeshFindData& mesh, const std::string& folder);
    void copyTextureFile(const std::string& texturePath, const std::string& dstFolder);

    void saveSceneYaml(const std::string& filename);
    void emitEntity(Entity entity, YAML::Emitter& emitter);
};

#endif
