#pragma once


constexpr long double MPI = 3.14159265358979323851;

float toRadian(float degrees) {
	return degrees * MPI / 180.0;
}

struct Vec4 {
	float x{};
	float y{};
	float z{};
	float w{};
};

struct Vec3 {
	float x{};
	float y{};
	float z{};
};
struct Vec2 {
	float x{};
	float y{};
};


	struct Matrix4f
	{
		float data[4][4];


		Matrix4f operator*(const Matrix4f& other) const
		{
			Matrix4f result;
			for (int i = 0; i < 4; ++i) 
			{
				for (int j = 0; j < 4; ++j) 
				{
					result.data[i][j] = 0.0f;
					for (int k = 0; k < 4; ++k) 
					{
						result.data[i][j] += data[i][k] * other.data[k][j];
					}
				}
			}
			return result;
		}
	};

