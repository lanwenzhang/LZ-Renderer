#pragma once

#include "../object.h"
#include "geometry/geometry.h"
#include "material/material.h"

namespace lzgl::renderer {

	class Mesh : public Object {
	public:
		Mesh(Geometry* geometry, Material* material);
		~Mesh();


	public:
		Geometry* mGeometry{ nullptr };
		Material* mMaterial{ nullptr };

	};

}

