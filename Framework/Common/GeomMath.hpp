#pragma once

#include <functional>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#ifndef TWO_PI
#define TWO_PI 3.14159265358979323846f * 2.0f
#endif

#ifndef RAD
#define RAD 0.0174532925f
#endif

namespace Aurora
{
	template<int row,int col,class T,glm::qualifier Q>
	inline void abs(glm::mat<row,col,T,Q>& mat)
	{
		for (int i = 0; i < row; ++i)
		{
			for (int j = 0; j < col; ++j)
			{
				mat[i][j] = std::abs(mat[i][j]);
			}
		}
	}

	inline void TransformAabb(const glm::vec3& halfExtents, float margin, const glm::mat4& trans,
		glm::vec3& aabbMinOut, glm::vec3& aabbMaxOut)
	{
		glm::vec3 halfExtentsWithMargin = halfExtents + glm::vec3(margin, margin, margin);
		glm::vec3  center;
		glm::vec3  extent;
		center = trans[3]; //get origin;

		glm::mat3 basis = trans;
		abs(basis);

		extent[0] = glm::dot(trans[0], trans[0]);
		extent[1] = glm::dot(trans[1], trans[1]);
		extent[2] = glm::dot(trans[2], trans[2]);

		aabbMinOut = center - extent;
		aabbMaxOut = center + extent;
	}

	typedef std::function<double(double)> nr_f;
	typedef std::function<double(double)> nr_fprime;

	inline double newton_raphson(double x0, nr_f f, nr_fprime fprime)
	{
		double x, x1 = x0;

		do {
			x = x1;
			double fx = f(x);
			double fx1 = fprime(x);
			x1 = x - (fx / fx1);
		} while (fabs(x1 - x) >= std::numeric_limits<double>::epsilon());

		return x1;
	}

	
}
