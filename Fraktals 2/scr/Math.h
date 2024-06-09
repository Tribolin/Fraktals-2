#pragma once
#include "cmath"
#define Degree_to_Radian 0.0174533

namespace Math
{
	class Vec2
	{
	public:
		Vec2(double x, double y);
		~Vec2();
		double x;
		double y;
		Vec2 operator+(const Vec2& v2);
		Vec2 operator-(Vec2 v2);
		Vec2 operator/(double v2);

		double magnitude();
		void normalize();
	private:

	};


	class Vec3
	{
	public:
		Vec3(double x, double y, double z);
		~Vec3();
		double x;
		double y;
		double z;
		Vec3 operator + (Vec3 v2);
		Vec3 operator - (Vec3 v2);
		double magnitude();
		void normalize();

	private:

	};
}