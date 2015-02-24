#include "image/image.h"

#include <assert.h>
#include <iostream>
#include <png.h>

namespace Svit
{
  Image::Image ():iterations(0),size(Vector2i(0,0))
	{
		// TODO use this::resize
		data.resize(0);    
	}

	Image::Image (Vector2i& _size)
		: size(_size),iterations(0)
	{
		data.resize(size.x * size.y);

		for (std::vector<Vector3>::iterator it=data.begin();it!=data.end();++it)
      *it = Vector3();
	}
  
	const Vector3& 
  Image::operator() (int x, int y)
	{
		assert(x >= 0 && y >= 0);
		return data[y * size.x + x];
	}

	void
	Image::resize (Vector2i& _size)
	{
		data.resize(_size.x * _size.y);
		size = _size;
	}

  void
  Image::add_image(Image &_img){
    std::vector<Vector3>::iterator it2=_img.data.begin();
    for(std::vector<Vector3>::iterator it=data.begin();
        it!=data.end();++it,++it2)
        *it += *it2;
  }

  void
  Image::scale(float _scale){
    for(std::vector<Vector3>::iterator it=data.begin();it!=data.end();++it)
        *it *= _scale;
  }

	int 
  Image::write (std::string filename)
	{
		FILE *file = fopen(filename.c_str(), "wb");
		png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, 
				NULL, NULL);
		png_infop info = png_create_info_struct(png);

		png_init_io(png, file);

		png_set_IHDR(png, info, size.x, size.y, 8, PNG_COLOR_TYPE_RGB,
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, 
				PNG_FILTER_TYPE_BASE);

		png_text text;
		char key[] = "Created with";
    char value[] = "PathTracer Svit";
		text.compression = PNG_TEXT_COMPRESSION_NONE;
		text.key = key;
		text.text = value;
		png_set_text(png, info, &text, 1);

		png_write_info(png, info);

		size_t row_size = size.x * 3;
		png_bytep row = (png_bytep)malloc(row_size * sizeof(png_byte));

		for (unsigned y = 0; y < size.y; y++)
		{
			for (unsigned x = 0; x < size.x; x++)
			{
        const Vector3& rgb = (*this)(x, y);
				row[x*3 + 0] = (png_byte)((rgb.x > 1.0 ? 1.0 : rgb.x) * 255.0); 
				row[x*3 + 1] = (png_byte)((rgb.y > 1.0 ? 1.0 : rgb.y) * 255.0); 
				row[x*3 + 2] = (png_byte)((rgb.z > 1.0 ? 1.0 : rgb.z) * 255.0); 
			}

			png_write_row(png, row);
		}

		png_write_end(png, NULL);
		fclose(file);
		png_free_data(png, info, PNG_FREE_ALL, -1);
		png_destroy_write_struct(&png, (png_infopp)NULL);
		free(row);

		return 0;
	}
}

