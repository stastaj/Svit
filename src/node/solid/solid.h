#ifndef SVIT_SOLID
#define SVIT_SOLID

#include "node/node.h"
#include "geom/ray.h"
#include "geom/intersection.h"
#include "material/material.h"

#include <list>
#include <memory>

namespace Svit
{
  /**
   * @brief The Solid class represents a single intersectable primitive with 
   * material ID and light ID. 
   */
	class Solid : public Node
	{
		public:
		  const int material;
      const int light;
      unsigned int ID;
      
      Solid(int _mat,int _light):material(_mat),light(_light)
      {}
      
			virtual void
			dump (const char *_name, unsigned int _level = 0) = 0;
	};
}

#endif

