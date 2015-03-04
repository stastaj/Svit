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

      explicit Image (Vector2i& _size);
      
      Image(const Image& _img);
      
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
      write_png (std::string filename);
      
      int 
      write_hdr(std::string filename);
	};
}

#endif

