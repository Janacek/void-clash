/********************************************************************
**
**              Transformable.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef OGL_TRANSFORMABLE
#define OGL_TRANSFORMABLE

#include <glm/gtx/transform.hpp>
#include <vector>

namespace ogl
{
	enum Axis
	{
		Xaxis = 0,
		Yaxis = 1,
		Zaxis = 2
	};

	class Transformable
	{
	public:
		Transformable(void);
		virtual ~Transformable(void);

		// Ask explicitly to copy every component
		virtual void copyComponentTransform(const Transformable &other)
		{
			_transform = other._transform;
			_MVP = other._MVP;
		}

		void resetTransform(void);

		void translate(const glm::vec3 &moves);
		void translate(float x, float y, float z = 0.0f);

		void scale(const glm::vec3 &scales);
		void scale(float x, float y, float z = 1.0f);

		void rotate(const float &angles, Axis axis = ogl::Zaxis);
		void rotate(const float &angles, const glm::vec3 &axis);

		const glm::mat4& getTransform(void);
		const glm::mat4& getMVP(const glm::mat4& camera);

		void ortho(float left, float top, float width, float height);

	protected:
		glm::mat4 _transform;
		glm::mat4 _MVP;
	};
}

#endif
