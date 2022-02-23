#pragma once
#include "vectors.h"

class MarchingInfo {
public:
	MarchingInfo() : cur_t(INFINITY), t_next_x(INFINITY), t_next_y(INFINITY), t_next_z(INFINITY) {}

	void nextCell();


	float cur_t;
	int grid_i, grid_j, grid_k;
	float t_next_x, t_next_y, t_next_z;
	float d_tx, d_ty, d_tz;
	int sign_x, sign_y, sign_z;
	Vec3f normal;
};