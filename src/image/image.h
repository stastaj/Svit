#ifndef SVIT_IMAGE
#define SVIT_IMAGE

#include "geom/vector.h"
#include "geom/point.h"

#include <vector>
#include <string>
#include <string.h>

namespace Svit
{
  /**
   * @brief The Image class represents 2D image in RGB. 
   */
	class Image
	{
		private:
      /**
       * @brief data stores accumulated radiance information, in row-major order,
       * from multiple iterations. After light transport simulation every pixel
       * needs to be divided by the number of the iterations.
       */
			std::vector<Vector3> data;

		public:
      /**
       * @brief Number of iterations from which the radiance was accumulated. 
       */
      int iterations;
      /**
       * @brief size of the image. 
       */
			Vector2i size;

      explicit Image (Vector2i& _size);
      
      Image(const Image& _img);
      
      ~Image(){}
      
			const Vector3& 
      operator() (int x, int y);
      
      inline void
      add_to_pixel(const int x,const int y, const Vector3& v){
        data[y * size.x + x]+=v;
      }
			
      void
      add_image(Image& _img);

      /**
       * @brief scales the whole image by the parameter scale - every pixel 
       * value is multiplied by the _scale parameter.
       * @param _scale Every pixel is multiplied by this parameter value.
       */
      void
      scale(float _scale);
      
      /**
       * @brief write_png Saves png image to the disc with given filename.
       * @param filename
       * @return 
       */
			/*int 
      write_png (std::string filename);
      
      /**
       * @brief write_hdr Saves hdr image to the disc with given filename.
       * @param filename
       * @return 
       */
      int 
      write_hdr(std::string filename);
	};
}

#endif

