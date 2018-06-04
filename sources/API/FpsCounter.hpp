/********************************************************************
**
**              FpsCounter.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef OGL_FPS_COUNTER
#define OGL_FPS_COUNTER

#include "Text.hpp"

namespace ogl
{
	class FpsCounter
	{
	public:
		FpsCounter(void);
		~FpsCounter(void);

		void start(void);
		void update(float deltaTime);
		void destroy(void);

	private:
		void display(void);

	private:
		float _totalTime;
		ogl::Text *_text;
		ogl::Font *_font;

		int _framePerSeconds;
		bool _drawable;
	};
}

#endif
