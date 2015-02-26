#ifndef SVIT_IMAGE
#define SVIT_IMAGE

#include "geom/vector.h"
#include "geom/point.h"

#include <vector>
#include <string>
#include <string.h>

namespace Svit
{
	class Image
	{
		private:

			std::vector<Vector3> data;

		public:
      int iterations;
			Vector2i size;

      explicit Image (Vector2i& _size):size(_size),iterations(0)
      {
        data.resize(size.x*size.y);
        memset(&data[0],0,sizeof(Vector3)*data.size());
      }      
      
			const Vector3& 
      operator() (int x, int y);
      
      inline void
      add_to_pixel(const int x,const int y, const Vector3& v){
        data[y * size.x + x]+=v;
      }
			
      void
      add_image(Image& _img);

      void
      scale(float _scale);

			int 
      write (std::string filename);
	};
}

#endif

