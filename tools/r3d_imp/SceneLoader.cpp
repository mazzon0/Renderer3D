#include "pch.h"
#include "SceneLoader.h"

void SceneLoader::load(const std::string filename) {
    m_loaded = true;
    m_folder = filename.substr(0, filename.find_last_of("/\\"));
    Assimp::Importer importer;

    /* TODO: check flags documentation */
    const aiScene* scene = importer.ReadFile(filename,
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);
    
    if(scene==nullptr) {
        std::cout << "ERROR: failed to read the file: " << filename << std::endl;
        m_loaded = false;
    }

    // TODO: Open scene data and save to entity 0

    loadAllMeshes(scene);
    loadAllMaterials(scene);

    loadEntities(scene->mRootNode, scene, 0);
}

void SceneLoader::saveBin(const std::string filename) {
    /*
    saveHeavyData(filename);
    saveSceneBin(filename); not implemented
    */
}

void SceneLoader::saveText(const std::string filename) {
    m_saved = true;
    saveHeavyData(filename);
    saveSceneYaml(filename);
}

void SceneLoader::loadAllMeshes(const aiScene* scene) {
    std::cout << "Loading " << scene->mNumMeshes << " meshes" << std::endl;
    m_meshes.reserve(scene->mNumMeshes);
    for(size_t i=0; i<scene->mNumMeshes; ++i) {
        loadMesh(scene->mMeshes[i], i);
    }
}

void SceneLoader::loadMesh(const aiMesh* mesh, unsigned int index) {
    MeshFindData mfd;

    // vertices
    mfd.mesh.m_vertices.reserve(mesh->mNumVertices);
    for(size_t i=0; i < mesh->mNumVertices; ++i) {
        loadVertex(mesh->mVertices[i], mesh->mTextureCoords[0][i], mesh->mNormals[i], mesh->mTangents[i], mesh->mBitangents[i], mfd.mesh);
    }

    // indices
    aiFace* face;
    size_t prevSize, nextSize;
    mfd.mesh.m_indices.reserve(mesh->mNumFaces*3);
    for(size_t i=0; i<mesh->mNumFaces; ++i) {
        face = &mesh->mFaces[i];
        prevSize = mfd.mesh.m_indices.size();
        nextSize = prevSize + face->mNumIndices; /* TODO: remove unutilized variables */

        for(size_t j=0; j<face->mNumIndices; ++j) {
            mfd.mesh.m_indices.push_back(face->mIndices[j]);
        }
    }

    // other data
    mfd.index = index;
    mfd.code = randomEntity();
    mfd.materialIndex = mesh->mMaterialIndex;

    m_meshes.emplace_back(mfd);
}

void SceneLoader::loadVertex(const aiVector3D& pos, const aiVector3D& texCoords, const aiVector3D& normal, const aiVector3D& tangent, const aiVector3D& bitangent, Mesh& mesh) {
    Vertex v(
        glm::vec3(pos.x, pos.y, pos.z),
        glm::vec2(texCoords.x, texCoords.y),
        glm::vec3(normal.x, normal.y, normal.z),
        glm::vec3(tangent.x, tangent.y, tangent.z),
        glm::vec3(bitangent.x, bitangent.y, bitangent.z));
    
    mesh.m_vertices.emplace_back(v);
}

void SceneLoader::loadAllMaterials(const aiScene* scene) {
    std::cout << "Loading " << scene->mNumMaterials << " materials" << std::endl;
    m_textures.reserve(scene->mNumMaterials);
    for(size_t i=0; i<scene->mNumMaterials; ++i) {
        loadMaterial(scene->mMaterials[i], i);
    }
}

void SceneLoader::loadMaterial(const aiMaterial* material, unsigned int index) {
    MaterialFindData mfd;
    mfd.index = index;

    int nAlbedo = material->GetTextureCount(aiTextureType_BASE_COLOR);
    int nNormal = material->GetTextureCount(aiTextureType_NORMAL_CAMERA);
    int nEmissive = material->GetTextureCount(aiTextureType_EMISSION_COLOR);
    int nMetalness = material->GetTextureCount(aiTextureType_METALNESS);
    int nRoughness = material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS);
    //int nAmbientOcclusion = material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION);
    /* TODO: add ambient occlusion for static scenes? */
    /* TODO: add other texture types, such as sheen, cloarcoat and transmission? */
    
    /* TODO: fix normal importing and metalness-roughness channels */
    /* TODO: what about the textures over index 0? */
    /* TODO: add white texture when not present */
    aiString path;
    if(nAlbedo > 0) {
        material->GetTexture(aiTextureType_BASE_COLOR, 0, &path);
        mfd.albedoPath = path.C_Str();
        if(!isTextureLoaded(mfd.albedoPath)) {
            m_textures.emplace_back(mfd.albedoPath);
        }
    }

    if(nNormal > 0) {
        material->GetTexture(aiTextureType_NORMAL_CAMERA, 0, &path);
        mfd.normalPath = path.C_Str();
        if(!isTextureLoaded(mfd.normalPath)) {
            m_textures.emplace_back(mfd.normalPath);
        }
    }

    if(nEmissive > 0) {
        material->GetTexture(aiTextureType_EMISSION_COLOR, 0, &path);
        mfd.emissionPath = path.C_Str();
        if(!isTextureLoaded(mfd.emissionPath)) {
            m_textures.emplace_back(mfd.emissionPath);
        }
    }

    if(nMetalness > 0) {
        material->GetTexture(aiTextureType_METALNESS, 0, &path);
        mfd.metalnessPath = path.C_Str();
        if(!isTextureLoaded(mfd.metalnessPath)) {
            m_textures.emplace_back(mfd.metalnessPath);
        }
    }

    if(nRoughness > 0) {
        material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &path);
        mfd.roughnessPath = path.C_Str();
        if(!isTextureLoaded(mfd.roughnessPath)) {
            m_textures.emplace_back(mfd.roughnessPath);
        }
    }

    m_materials.emplace_back(mfd);
}

bool SceneLoader::isTextureLoaded(const std::string& path) {
    for(size_t i=0; i<m_textures.size(); ++i) {
        if(m_textures[i].compare(path) == 0) {
            return true;
        }
    }

    return false;
}

void SceneLoader::loadEntities(const aiNode* node, const aiScene* scene, Entity parent) {
    Entity entity;
    std::cout << "Loading " << node->mNumMeshes << " entities" << std::endl;
    for(size_t i=0; i<node->mNumMeshes; ++i) {
        entity = randomEntity();
        loadEntity(node->mMeshes[i], parent, entity);
        
    }

    for(size_t i=0; i<node->mNumChildren; ++i) {
        loadEntities(node->mChildren[i], scene, entity);    // TODO: choose the parent. now the parent is a node of the previous (the last one just for this implementation)
    }
}

void SceneLoader::loadEntity(unsigned int meshIndex, Entity parent, Entity entity) {
    m_entities.push_back(entity);

    // Parent component
    if(parent != 0) {
        ParentComponent parentComp {parent};
        m_componentSystem.add<ParentComponent>(entity, parentComp);
    }
    
    // Mesh component
    MeshComponent meshComp;
    const MeshFindData& mfd = getMesh(meshIndex);
    meshComp.code = mfd.code;
    m_componentSystem.add<MeshComponent>(entity, meshComp);

    // Material component
    MaterialComponent materialComp;
    const MaterialFindData& mat = getMaterial(mfd.materialIndex);
    materialComp.albedoPath = mat.albedoPath;
    materialComp.normalPath = mat.normalPath;
    materialComp.emissionPath = mat.emissionPath;
    materialComp.metalnessPath = mat.metalnessPath;
    materialComp.roughnessPath = mat.roughnessPath;
    m_componentSystem.add<MaterialComponent>(entity, materialComp);
}

const MeshFindData& SceneLoader::getMesh(unsigned int meshIndex) {
    for(size_t i=0; i<m_meshes.size(); ++i) {
        if(m_meshes[i].index == meshIndex) {
            return m_meshes[i];
        }
    }

    std::cout << "ERROR: mesh index " << meshIndex << " not found in meshes" << std::endl;
    m_loaded = false;
    return m_meshes[0];
}

const MaterialFindData& SceneLoader::getMaterial(unsigned int materialIndex) {
    for(size_t i=0; i<m_materials.size(); ++i) {
        if(m_materials[i].index == materialIndex) {
            return m_materials[i];
        }
    }

    std::cout << "ERROR: material index " << materialIndex << " not found in materials" << std::endl;
    m_loaded = false;
    return m_materials[0];
}

Entity SceneLoader::randomEntity() {
    /* TODO: make those variables static? */
    /* TODO: avoid repetitions and 0? */
    std::random_device rd;
    std::mt19937_64 e2(rd());
    std::uniform_int_distribution<uint64_t> dist;
    return dist(e2);
}

void SceneLoader::saveHeavyData(const std::string& filename) {
    std::string folder = filename.substr(0, filename.find_last_of("/\\"));
    std::filesystem::create_directory(folder + "/meshes/");
    std::filesystem::create_directory(folder + "/textures/");

    for(MeshFindData& mesh : m_meshes) {
        saveMesh(mesh, folder);
    }

    for(std::string& texture : m_textures) {
        copyTextureFile(texture, folder);
    }
}

void SceneLoader::saveMesh(MeshFindData& mesh, const std::string& folder) {
    // File opening
    /* TODO: save with names, not code */
    std::ostringstream o;
    o << mesh.code;
    std::string code = o.str();
    std::ofstream file;
    file.open(folder + "/meshes/" + code + ".mb3d", std::ios::binary);
    if(!file.good()) {
        std::cout << "ERROR: failed to open the mesh file: " << folder + "/meshes/" + code + ".mb3d" << std::endl;
        m_saved = false;
        return;
    }

    // File writing
    std::vector<uint32_t> header(3);
    header[0] = 0x3d3d3d3d;
    header[1] = mesh.mesh.m_vertices.size();
    header[2] = mesh.mesh.m_indices.size();
    file.write(reinterpret_cast<char*>(header.data()), sizeof(uint32_t) * header.size());
    file.write(reinterpret_cast<char*>(mesh.mesh.m_vertices.data()), sizeof(Vertex) * mesh.mesh.m_vertices.size());
    file.write(reinterpret_cast<char*>(mesh.mesh.m_indices.data()), sizeof(uint32_t) * mesh.mesh.m_indices.size());

    file.close();
}

void SceneLoader::copyTextureFile(const std::string& texturePath, const std::string& dstFolder) {
    size_t charIndex = texturePath.find_last_of("/\\") + 1;
    std::string filename = texturePath.substr(charIndex, texturePath.size() - charIndex);
    std::ofstream dst(dstFolder + "/textures/" + filename, std::ios::binary);
    std::ifstream src(m_folder + "/" + texturePath, std::ios::binary);

    if(!dst.is_open()) {
        std::cout << "ERROR: failed to open the destination file: " << dstFolder + "/textures/" + filename << std::endl;
        m_saved = false;
        return;
    }
    if(!src.is_open()) {
        std::cout << "ERROR: failed to open the source file: " << m_folder + "/" + texturePath << std::endl;
        m_saved = false;
        return;
    }

    dst << src.rdbuf();

    dst.close();
    src.close();
}

void SceneLoader::saveSceneYaml(const std::string& filename) {
    YAML::Emitter emitter;
    emitter << YAML::BeginSeq;

    for(Entity e : m_entities) {
        emitEntity(e, emitter);
    }

    emitter << YAML::EndSeq;

    if(!emitter.good()) {
        std::cout << "ERROR: failed scene \'" << filename << "\' text encoding with the sequent error: " << emitter.GetLastError() << std::endl;
        m_saved = false;
        return;
    }

    std::ofstream file(filename);
    if(!file.good()) {
        std::cout << "ERROR: failed to save the scene in: " << filename << std::endl;
        m_saved = false;
        return;
    }

    /* TODO: add custom header? */
    file << emitter.c_str();
    file.close();

    std::cout << "Scene successfully saved: " << filename << std::endl;
}

void SceneLoader::emitEntity(Entity entity, YAML::Emitter& emitter) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "entity" << YAML::Value << entity;
    
    if(m_componentSystem.hasComponent<ParentComponent>(entity)) {
        emitter << YAML::Key << "parent";
        emitter << YAML::Value << m_componentSystem.get<ParentComponent>(entity).parent;
    }

    if(m_componentSystem.hasComponent<MeshComponent>(entity)) {
        emitter << YAML::Key << "mesh";
        emitter << YAML::Value << YAML::BeginMap;
        emitter << YAML::Key << "code";
        emitter << YAML::Value << m_componentSystem.get<MeshComponent>(entity).code;
        emitter << YAML::EndMap;
    }

    if(m_componentSystem.hasComponent<MaterialComponent>(entity)) {
        MaterialComponent material = m_componentSystem.get<MaterialComponent>(entity);
        emitter << YAML::Key << "material";
        emitter << YAML::Value << YAML::BeginMap;
        emitter << YAML::Key << "albedo" << YAML::Value << material.albedoPath;
        emitter << YAML::Key << "normal" << YAML::Value << material.normalPath;
        emitter << YAML::Key << "emission" << YAML::Value << material.emissionPath;
        emitter << YAML::Key << "metalness" << YAML::Value << material.metalnessPath;
        emitter << YAML::Key << "roughness" << YAML::Value << material.roughnessPath;
        emitter << YAML::EndMap;
    }

    emitter << YAML::EndMap;
}