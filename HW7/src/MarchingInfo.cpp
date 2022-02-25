#include "MarchingInfo.h"


void MarchingInfo::nextCell()
{
	int flag = 0;
	if (t_next_x < t_next_y && t_next_x < t_next_z) flag = 1;
	if (t_next_y < t_next_x && t_next_y < t_next_z) flag = 2;
	if (t_next_z < t_next_x && t_next_z < t_next_y) flag = 3;
	switch (flag)
	{
	case 1:
		grid_i += sign_x;
		cur_t = t_next_x;
		t_next_x += d_tx;
		normal = Vec3f(-1, 0, 0) * sign_x;
		break;
	case 2:
		grid_j += sign_y;
		cur_t = t_next_y;
		t_next_y += d_ty;
		normal = Vec3f(0, -1, 0) * sign_y;
		break;
	case 3:
		grid_k += sign_z;
		cur_t = t_next_z;
		t_next_z += d_tz;
		normal = Vec3f(0, 0, -1) * sign_z;
		break;
	}
}
