#pragma once

#include "camera_control.h"

namespace lzgl::renderer {

	class TrackBallCameraControl :public CameraControl {
	public:
		TrackBallCameraControl();
		~TrackBallCameraControl();

		void onCursor(double xpos, double ypos)override;

		void onScroll(float offset)override;

	private:
		void pitch(float angle);
		void yaw(float angle);

		float mMoveSpeed = 0.01f;
	};

}