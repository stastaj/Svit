#ifndef SVIT_IMAGE
#define SVIT_IMAGE

#include "geom/vector.h"
#include "geom/point.h"

#include <vector>
#include <string>

namespace Svit
{
	class Image
	{
		private:

			std::vector<Vector3> data;

		public:
      int iterations;
			Vector2i size;

			const Vector3& 
      operator() (int x, int y);
      
      inline void
      set_pixel(const int x,const int y, const Vector3& v){
        data[y * size.x + x]=v;
      }
      
			void
			resize (Vector2i& _size);

			Image ();

			Image (Vector2i& _size);

      void
      add_image(Image& _img);

      void
      scale(float _scale);

			int 
      write (std::string filename);
	};
}

#endif

