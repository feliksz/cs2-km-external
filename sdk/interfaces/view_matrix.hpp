#pragma once
#include <common.hpp>

namespace cstrike {
	struct ViewMatrix {
		f32 m[4][4];

		bool world_to_screen(const V3& in, V2i& out) {
			float View = 0.f;
			float SightX = 1920;
			float SightY = 1080 / 2;

			View = m[3][0] * in.x + m[3][1] * in.y + m[3][2] * in.z + m[3][3];
			if (View <= 0.01)
				return false;

			float ox = m[0][0] * in.x + m[0][1] * in.y + m[0][2] * in.z + m[0][3];
			float oy = m[1][0] * in.x + m[1][1] * in.y + m[1][2] * in.z + m[1][3];
			ox *= 1.f / View;
			oy *= 1.f / View;

			float x = 1920 * .5f, y = 1080 * .5f;
			x += 0.5f * ox * (f32)1920 + 0.5f;
			y -= 0.5f * oy * (f32)1080 + 0.5f;
			out.x = (i32)x;
			out.y = (i32)y;

			return true;
		}
	};
}