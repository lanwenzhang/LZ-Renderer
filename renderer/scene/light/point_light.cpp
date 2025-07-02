#include "point_light.h"
#include "../shadow/point_light_shadow.h"

namespace lzgl::renderer {

	PointLight::PointLight() {
		mShadow = new PointLightShadow();
	}
	PointLight::~PointLight() {}
}

