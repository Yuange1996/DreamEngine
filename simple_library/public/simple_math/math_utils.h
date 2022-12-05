#pragma once
#include <vector>

//����
#include "transformation/vector/vector_2d.h"
#include "transformation/vector/vector_3d.h"
#include "transformation/vector/vector_4d.h"

//����
#include "transformation/matrix/matrix_2x2.h"
#include "transformation/matrix/matrix_3x3.h"
#include "transformation/matrix/matrix_4x4.h"

using namespace std;
namespace math_utils
{
	//����˷�
	fvector_3d mul(const fvector_2d &in_3d, const fmatrix_3x3& in_matrix_3x3);
	fvector_3d mul(const fvector_3d &in_3d, const fmatrix_3x3& in_matrix_3x3);
	fvector_4d mul(const fvector_4d &in_3d, const fmatrix_4x4& in_matrix_3x3);

	//������ת
	void rot_radian(float in_radian, fmatrix_3x3& in_world_matrix_3x3);
	void rot_angle(float in_angle, fmatrix_3x3& in_world_matrix_3x3);

	//��������
	void set_scale(const fvector_2d& in_scale, fmatrix_3x3& in_world_matrix_3x3);
	fmatrix_3x3 set_scale(const fvector_2d& in_scale);

	//�ж�bool�ǲ�����һ����Χ��
	bool scalar_neare_qual_float(float f1, float f2, float epsilon);
	fmatrix_4x4 matrix_perspective(float in_fov_radian, float aspect_ratio, float near_z, float far_z);

	//����Ŀ��
	fmatrix_4x4 matrix_look_at_target(const fvector_4d &in_view_pos, const fvector_4d& in_target_pos, const fvector_4d& in_view_up);

	fmatrix_4x4 build_view_matrix(const fvector_4d& in_view_pos,const fmatrix_4x4 &in_view_matrix);
}