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

Vec3 Math::Vec3::operator*(double s)
{
	return Vec3(x*s,y*s,z*s);
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

Quaternion::Quaternion(double rr, double ii, double jj, double kk)
	:r(rr), i(ii), j(jj), k(kk)
{
}

Math::Quaternion::Quaternion(double x, Vec3 vec)
	:r(x),i(vec.x),j(vec.y),k(vec.z)
{
}

Quaternion::~Quaternion()
{
}
 Quaternion Quaternion::operator*(Quaternion b)
{
	return Quaternion(
		((r * b.r) - (i * b.i) - (j * b.j) - (k * b.k)),
		((r * b.i) + (i * b.r) + (j * b.k) - (k * b.j)),
		((r * b.j) - (i * b.k) + (j * b.r) + (k * b.i)),
		((r * b.k) + (i * b.j) - (j * b.i) + (k * b.r)));
}
