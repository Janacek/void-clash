/********************************************************************
**
**              Model.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "Model.hpp"
#include "CheckError.hpp"
#include "Log.hpp"

using namespace std;
using namespace ogl;

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

Model::Model(void)
{
	std::memset(m_Buffers, 0, sizeof(m_Buffers));

	m_Entries.clear();
	m_Textures.clear();

	_vertexBinding = POS_VB;
	_textureCoordBinding = TEXCOORD_VB;
	_normalsBinding = NORMAL_VB;

	_created = false;
	_instanced = false;
}


Model::~Model()
{

}

/////////////////////////////////////////////////////////////////////
/////	Are we going to do instanced rendering ?
/////////////////////////////////////////////////////////////////////

void Model::setInstanced(bool instanced)
{
    _instanced = instanced;
}

/////////////////////////////////////////////////////////////////////
/////	Create
/////////////////////////////////////////////////////////////////////

bool Model::create(const string& filename)
{
	std::string fullPath(filename);
	if (fullPath.empty() == true)
	{
		fullPath = _path;
	}

	VC_INFO("Model::Loading mesh " + fullPath);

	// Create the buffers for the vertices attributes
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glGenBuffers(4, m_Buffers);

	bool Ret = false;
	Assimp::Importer Importer;

	const aiScene* pScene =
		Importer.ReadFile(fullPath.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);

	if (pScene)
	{
		Ret = InitFromScene(pScene, fullPath);
	}
	else
	{
		VC_ERROR_CRITICAL("Model::" + std::string(Importer.GetErrorString()));
	}

	// Instanced buffers
    if (_instanced == true)
        ogl::Instanced::create();

	VC_INFO("Model::" + fullPath + " loaded");

	glBindVertexArray(0);

	checkError();

	// Reset the max instances
	_maxInstances = INVALID_INSTANCES_NUMBER;

	return Ret;
}

/////////////////////////////////////////////////////////////////////
/////	Destroy
/////////////////////////////////////////////////////////////////////

void Model::destroy()
{
	glDeleteVertexArrays(1, &_vao);

	for (unsigned int i = 0; i < m_Textures.size(); i++) {
		delete(m_Textures[i]);
	}

	if (m_Buffers[0] != 0) {
		glDeleteBuffers(4, m_Buffers);
	}

	// Instanced buffers
    if (_instanced == true)
        ogl::Instanced::destroy();

	checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Set buffer binding index
/////////////////////////////////////////////////////////////////////

void Model::setBufferIndexes(int vertex, int textureCoord, int normals)
{
	_vertexBinding = vertex;
	_textureCoordBinding = textureCoord;
	_normalsBinding = normals;
}

/////////////////////////////////////////////////////////////////////
/////	Assimp API
/////////////////////////////////////////////////////////////////////

bool Model::InitFromScene(const aiScene* pScene, const string& filename)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	vector<glm::vec3> Positions;
	vector<glm::vec3> Normals;
	vector<glm::vec2> TexCoords;
	vector<unsigned int> Indices;

	unsigned int NumVertices = 0;
	unsigned int NumIndices = 0;

	// Count the number of vertices and indices
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
		m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
		m_Entries[i].BaseVertex = NumVertices;
		m_Entries[i].BaseIndex = NumIndices;

		NumVertices += pScene->mMeshes[i]->mNumVertices;
		NumIndices += m_Entries[i].NumIndices;
	}

	// Reserve space in the vectors for the vertex attributes and indices
	Positions.reserve(NumVertices);
	Normals.reserve(NumVertices);
	TexCoords.reserve(NumVertices);
	Indices.reserve(NumIndices);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitModel(paiMesh, Positions, Normals, TexCoords, Indices);
	}

	if (!InitMaterials(pScene, filename)) {
		return false;
	}

	// Generate and populate the buffers with vertex attributes and the indices
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(_vertexBinding);
	glVertexAttribPointer(_vertexBinding, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(_textureCoordBinding);
	glVertexAttribPointer(_textureCoordBinding, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(_normalsBinding);
	glVertexAttribPointer(_normalsBinding, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	checkError();

	return true;
}

void Model::InitModel(const aiMesh* paiMesh,
	vector<glm::vec3>& Positions,
	vector<glm::vec3>& Normals,
	vector<glm::vec2>& TexCoords,
	vector<unsigned int>& Indices)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	// Populate the vertex attribute vectors
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
		Normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		TexCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
	}

	// Populate the index buffer
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		if (Face.mNumIndices != 3)
			VC_WARNING("Model::Face.mNumIndices != 3 (Triangulation error)");
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}
}

bool Model::InitMaterials(const aiScene* pScene, const string& filename)
{
	// Extract the directory part from the file name
	string::size_type SlashIndex = filename.find_last_of("/");
	string Dir;

	if (SlashIndex == string::npos) {
		Dir = ".";
	}
	else if (SlashIndex == 0) {
		Dir = "/";
	}
	else {
		Dir = filename.substr(0, SlashIndex);
	}

	bool Ret = true;

	// Initialize the materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_Textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				string p(Path.data);

				if (p.substr(0, 2) == ".\\") {
					p = p.substr(2, p.size() - 2);
				}

				string FullPath = Dir + "/" + p;
				std::replace(FullPath.begin(), FullPath.end(), '\\', '/');

				m_Textures[i] = new ogl::Texture;

				if (!m_Textures[i]->create(FullPath.c_str())) {
					VC_WARNING_CRITICAL("Model::Error loading texture ");
					delete m_Textures[i];
					m_Textures[i] = NULL;
					Ret = false;
				}
				else {
					VC_INFO("Model::Loaded texture " + FullPath);
				}
			}
		}
	}

	checkError();

	return Ret;
}

/////////////////////////////////////////////////////////////////////
/////	Draw the model
/////////////////////////////////////////////////////////////////////

void Model::draw(void)
{
	glBindVertexArray(_vao);

	// Draw all model parts
	for (unsigned int i = 0; i < m_Entries.size(); i++)
	{
		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		// Bind textures
		if (MaterialIndex >= m_Textures.size())
			VC_ERROR_CRITICAL("Model::MaterialIndex >= m_Textures.size()");

		if (m_Textures[MaterialIndex]) {
			m_Textures[MaterialIndex]->bind(i);
		}

		// Only one draw
		if (_instanced == false)
		{
			glDrawElementsBaseVertex(GL_TRIANGLES,
				m_Entries[i].NumIndices,
				GL_UNSIGNED_INT,
				(void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
				m_Entries[i].BaseVertex);
		}

		// Instanced draw
		else if (_maxInstances != INVALID_INSTANCES_NUMBER)
		{
			glDrawElementsInstancedBaseVertex(GL_TRIANGLES,
				m_Entries[i].NumIndices,
				GL_UNSIGNED_INT,
				(void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
				_maxInstances,
				m_Entries[i].BaseVertex);

			// Reset the max instances
		    _maxInstances = INVALID_INSTANCES_NUMBER;
		}
	}

	// Unbind textures
	for (unsigned int i = 0; i < m_Entries.size(); i++)
	{
		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		if (MaterialIndex >= m_Textures.size())
			VC_ERROR_CRITICAL("Model::MaterialIndex >= m_Textures.size()");

		if (m_Textures[MaterialIndex]) {
			m_Textures[MaterialIndex]->bind(i);
		}
	}

	glBindVertexArray(0);

	checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Set the loading path
/////////////////////////////////////////////////////////////////////

void Model::setPath(const std::string &path)
{
	_path = path;
}
