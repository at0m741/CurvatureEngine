#include <Geodesics.h>

bool invert_3x3(const float m[3][3], float inv[3][3]) {
	float det =
		m[0][0]*(m[1][1]*m[2][2]-m[1][2]*m[2][1])
		- m[0][1]*(m[1][0]*m[2][2]-m[1][2]*m[2][0])
		+ m[0][2]*(m[1][0]*m[2][1]-m[1][1]*m[2][0]);

	if (std::fabs(det) < 1e-14) return false;
	float idet = 1.0/det;

	inv[0][0] =  (m[1][1]*m[2][2]-m[2][1]*m[1][2]) * idet;
	inv[0][1] = -(m[0][1]*m[2][2]-m[2][1]*m[0][2]) * idet;
	inv[0][2] =  (m[0][1]*m[1][2]-m[1][1]*m[0][2]) * idet;

	inv[1][0] = -(m[1][0]*m[2][2]-m[2][0]*m[1][2]) * idet;
	inv[1][1] =  (m[0][0]*m[2][2]-m[2][0]*m[0][2]) * idet;
	inv[1][2] = -(m[0][0]*m[1][2]-m[1][0]*m[0][2]) * idet;

	inv[2][0] =  (m[1][0]*m[2][1]-m[2][0]*m[1][1]) * idet;
	inv[2][1] = -(m[0][0]*m[2][1]-m[2][0]*m[0][1]) * idet;
	inv[2][2] =  (m[0][0]*m[1][1]-m[1][0]*m[0][1]) * idet;

	return true;
}
