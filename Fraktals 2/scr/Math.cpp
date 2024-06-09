#include "Math.h"
using namespace Math;

Vec2::Vec2(double x, double y)
	:x(x),y(y)
{

}

Vec2::~Vec2()
{
}



Vec2 Math::Vec2::operator+(const Vec2& v2)
{
	return Vec2(x + v2.x, y + v2.y);
}

Vec2 Vec2::operator -(Vec2 v2)
{
	return Vec2(x - v2.x, y - v2.y);
}

Vec2 Math::Vec2::operator/(double s1)
{
	return Vec2(x/s1,y/s1);
}

double Vec2::magnitude()
{
	return sqrt(x*x+y*y);
}

void Math::Vec2::normalize()
{
	float length = magnitude();
	x /= length;
	y /= length;

}

Vec3::Vec3(double x, double y,double z)
	:x(x), y(y), z(z) 
{

}

Vec3::~Vec3()
{
}

Vec3 Vec3::operator +(Vec3 v2)
{
	return Vec3(x + v2.x, y + v2.y,z+v2.z);
}

Vec3 Vec3::operator -(Vec3 v2)
{
	return Vec3(x - v2.x, y - v2.y, z + v2.z);
}

double Vec3::magnitude()
{
	return sqrt(x * x + y * y+ z*z);
}

void Math::Vec3::normalize()
{
	float length = magnitude();
	x /= length;
	y /= length;
	z /= length;
}

