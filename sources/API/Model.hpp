/********************************************************************
**
**              Model.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef OGL_MODEL
#define OGL_MODEL

#include <map>
#include <vector>
#include <string>
#include "Transformable.hpp"
#include "Instanced.hpp"
#include "Texture.hpp"
#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"

#define INVALID_MATERIAL 0xFFFFFFFF

#define INDEX_BUFFER 0
#define POS_VB       1
#define NORMAL_VB    2
#define TEXCOORD_VB  3
#define WVP_MAT_VB   4
#define WORLD_MAT_VB 5

namespace ogl
{
	struct Vertex
	{
		glm::vec3 m_pos;
		glm::vec2 m_tex;
		glm::vec3 m_normal;

		Vertex() {}

		Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal)
		{
			m_pos = pos;
			m_tex = tex;
			m_normal = normal;
		}
	};

	struct ModelEntry
	{
		ModelEntry()
		{
			NumIndices = 0;
			BaseVertex = 0;
			BaseIndex = 0;
			MaterialIndex = INVALID_MATERIAL;
		}

		unsigned int NumIndices;
		unsigned int BaseVertex;
		unsigned int BaseIndex;
		unsigned int MaterialIndex;
	};

	class Model : public Transformable, public Instanced
	{
	public:
		Model(void);
		~Model(void);

		void setPath(const std::string &path);
		void setInstanced(bool instanced);
		void setBufferIndexes(int vertex, int textureCoord, int normals);
		bool create(const std::string& filename = "");
		void draw(void);
		void destroy(void);

		GLuint getId(void) { return _vao; }

	private:
		bool InitFromScene(const aiScene* pScene, const std::string& filename);
		void InitModel(const aiMesh* paiMesh,
			std::vector<glm::vec3>& Positions,
			std::vector<glm::vec3>& Normals,
			std::vector<glm::vec2>& TexCoords,
			std::vector<unsigned int>& Indices);

		bool InitMaterials(const aiScene* pScene, const std::string& filename);

		GLuint _vao;
		GLuint m_Buffers[6];

		int _vertexBinding;
		int _textureCoordBinding;
		int _normalsBinding;
		bool _instanced;
		bool _created;

		std::vector<ModelEntry> m_Entries;
		std::vector<ogl::Texture*> m_Textures;
		std::string _path;
	};
}

#endif /* end of include guard: OGL_MODEL */
