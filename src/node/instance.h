#ifndef SVIT_INSTANCE
#define SVIT_INSTANCE

#include "node/node.h"
#include "geom/matrix.h"

namespace Svit
{
	class Instance : public Node
	{
		private:
			Node *node;
			Matrix44 matrix;
			Matrix44 inverse;

			void
			combine (Matrix44& _transformation);

			void
			recompute_inverse ();

		public:
			Instance (Node *_node);

			boost::optional<Intersection>
			intersect (Ray& _ray, float _best);

			void
			set_material (std::unique_ptr<Material> _material);

			void
			reset ();

			void
			translate (Vector3 const& _translation);

			void
			rotate (Vector3 const& _axis, float _angle);

			void
			scale (Vector3 const& _scale);

			void
			scale (float _scale);

			void 
			set_matrix (Matrix44& _matrix);
	};
}

#endif

