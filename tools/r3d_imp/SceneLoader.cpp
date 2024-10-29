#include "SceneLoader.h"
#include "utils.pch"

void SceneLoader::load(const std::string filename) {
    m_loaded = true;
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

    // Load all meshes
    loadAllMeshes(scene);

    // Bind meshes with entities
    loadEntities(scene->mRootNode, scene, 0);
}

void SceneLoader::saveBin(const std::string filename) {
    /*
    saveHeavyData(filename);
    saveSceneBin(filename); not implemented
    */
}

void SceneLoader::saveText(const std::string filename) {
    //saveHeavyData(filename);
    saveSceneYaml(filename);
}

void SceneLoader::loadAllMeshes(const aiScene* scene) {
    std::cout << "Loading " << scene->mNumMeshes << " meshes" << std::endl;
    for(size_t i=0; i<scene->mNumMeshes; ++i) {
        loadMesh(scene->mMeshes[i], i);
    }
}

void SceneLoader::loadMesh(const aiMesh* mesh, unsigned int index) {
    MeshFindData mfd;

    // vertices
    mfd.mesh.m_vertices.reserve(mesh->mNumVertices);
    for(size_t i=0; i < mesh->mNumVertices; ++i) {
        loadVertex(mesh->mVertices[i], mfd.mesh);
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
            //std::cout << "Index " << j << std::endl;
        }
    }

    // other data
    mfd.index = index;
    mfd.code = randomEntity();

    m_meshes.emplace_back(mfd);
}

void SceneLoader::loadVertex(const aiVector3D& vertex, Mesh& m) {
    /* TODO: implement */

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
    meshComp.code = getMeshCode(meshIndex);
    m_componentSystem.add<MeshComponent>(entity, meshComp);
}

MeshCode SceneLoader::getMeshCode(unsigned int meshIndex) {
    for(size_t i=0; i<m_meshes.size(); ++i) {
        if(m_meshes[i].index == meshIndex) {
            return m_meshes[i].code;
        }
    }

    std::cout << "ERROR: mesh index " << meshIndex << " not found" << std::endl;
    m_loaded = false;
    return 0;
}

Entity SceneLoader::randomEntity() {
    /* TODO: make those variables static? */
    std::random_device rd;
    std::mt19937_64 e2(rd());
    std::uniform_int_distribution<uint64_t> dist;
    return dist(e2);
}

void SceneLoader::saveHeavyData(const std::string& filename) {
    /* TODO: custom folder division character */
    std::string folder = filename.substr(0, filename.find_last_of('/'));

    for(MeshFindData& mesh : m_meshes) {
        saveMesh(mesh, filename);
    }

    /* TODO: textures */
}

void SceneLoader::saveMesh(const MeshFindData& mesh, const std::string& folder) {

}

void SceneLoader::saveSceneYaml(const std::string& filename) {
    YAML::Emitter emitter;
    emitter << YAML::BeginSeq;

    for(Entity e : m_entities) {
        emitEntity(e, emitter);
    }

    emitter << YAML::EndSeq;

    if(!emitter.good()) {
        std::cout << "ERROR: failed saving the scene " << filename << " with the sequent error: " << emitter.GetLastError() << std::endl;
        return;
    }

    std::ofstream file(filename);
    file << emitter.c_str();
    file.close();

    std::cout << "Scene successfully saved: full_filename" << std::endl;
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

    emitter << YAML::EndMap;
}