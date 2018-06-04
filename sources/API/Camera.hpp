/********************************************************************
**
**              Camera.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef OGL_CAMERA
#define OGL_CAMERA

#include "VoidClash_OpenGL.h"
#include "Transformable.hpp"

namespace ogl
{
	class Camera : public Transformable
	{
	public:
		Camera(void);
		~Camera(void);

		void setupOrthographic(float sizeX, float sizeY, bool reverse = true);

		void setupPerspective(const GLfloat &fovY, const GLfloat &aspect,
				      const GLfloat &zNear, const GLfloat &zFar);
		void lookAt(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up);
		void update(void);

		const glm::mat4& getProjectionMatrix(void) const;
		const glm::mat4& getView(void) const;
		const glm::mat4& getFinal(void) const;
		const glm::vec3& getEye(void) const;
		bool isOrtho(void) {
			return _isOrtho;
		}

	private:
		glm::mat4 _projection;
		glm::mat4 _view;
		glm::mat4 _final;
		glm::vec3 _eye;
		bool _isOrtho;
	};
}

#endif
