/*
=====================================================================================
MIT-0 Licence:
-------------------------------------------------------------------------------------

Copyright 2025 Thibault Fillion

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the “Software”), to deal in the 
Software without restriction, including without limitation the rights to use, copy, 
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

=====================================================================================

Very simple header only include for some basic geometry classes and functions.
classes:
Vec2: two dimensions vector class
Vec3: three dimensions vector class
functions:
rotate_around_axis: rotate a point around an axis by some angle
get_distance: returns the distance between two points
get_angle: returns the angle formed between two vectors
*/

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <cmath>

class Vec2{
	public:
	double x, y;
	
	Vec2(double x=0, double y=0): x(x), y(y) {}	
	Vec2 & operator += (const Vec2 & v){x += v.x; y += v.y; return *this;}
	Vec2 & operator -= (const Vec2 & v){x -= v.x; y -= v.y; return *this;}
	Vec2 & operator *= (double s){x *= s; y *= s; return *this;}
	Vec2 & operator /= (double s){x /= s; y /= s; return *this;}
	bool operator == (const Vec2 & v){return (x==v.x and y==v.y);}
	double norm() const {return sqrt(x*x + y*y);}
	double dot(const Vec2 & v) const {return v.x*x + v.y*y;}
	Vec2 unit() const {double n = norm(); return Vec2(x/n, y/n);}
	Vec2 & normalize(){double n = norm(); x/=n; y/=n; return *this;}
	};

Vec2 operator + (const Vec2 & v){return v;}
Vec2 operator - (const Vec2 & v){return Vec2(-v.x, -v.y);}
Vec2 operator + (const Vec2 & a, const Vec2 & b){return Vec2(a.x+b.x, a.y+b.y);}
Vec2 operator - (const Vec2 & a, const Vec2 & b){return Vec2(a.x-b.x, a.y-b.y);}
Vec2 operator * (const Vec2 & v, double s){return Vec2(v.x*s, v.y*s);}
Vec2 operator * (double s, const Vec2 & v){return Vec2(v.x*s, v.y*s);}
Vec2 operator / (const Vec2 & v, double s){return Vec2(v.x/s, v.y/s);}

class Vec3{
	public:
	double x, y, z;
	
	Vec3(double x=0, double y=0, double z=0): x(x), y(y), z(z) {}	
	Vec3 & operator += (const Vec3 & v){x += v.x; y += v.y; z += v.z; return *this;}
	Vec3 & operator -= (const Vec3 & v){x -= v.x; y -= v.y; z -= v.z; return *this;}
	Vec3 & operator *= (double s){x *= s; y *= s; z *= s; return *this;}
	Vec3 & operator /= (double s){x /= s; y /= s; z /= s; return *this;}
	bool operator == (const Vec3 & v){return (x==v.x and y==v.y and z==v.z);} 
	double norm() const {return sqrt(x*x + y*y + z*z);}
	double dot(const Vec3 & v) const {return v.x*x + v.y*y + v.z*z;}
	Vec3 cross(const Vec3 & v) const {
		// adapted from reference:
		// Cross product. (accessed online on the 10th of October 2025). 
		// Wikipedia. https://en.wikipedia.org/wiki/Axis%E2%80%93angle_representation
		return Vec3(
			y*v.z - z*v.y,
			z*v.x - x*v.z,
			x*v.y - y*v.x
			);
		}
	Vec3 unit() const {double n = norm(); return Vec3(x/n, y/n, z/n);}
	Vec3 & normalize(){double n = norm(); x/=n; y/=n; z/=n; return *this;}
	};

Vec3 operator + (const Vec3 & v){return v;}
Vec3 operator - (const Vec3 & v){return Vec3(-v.x, -v.y, -v.z);}
Vec3 operator + (const Vec3 & a, const Vec3 & b){return Vec3(a.x+b.x, a.y+b.y, a.z+b.z);}
Vec3 operator - (const Vec3 & a, const Vec3 & b){return Vec3(a.x-b.x, a.y-b.y, a.z-b.z);}
Vec3 operator * (const Vec3 & v, double s){return Vec3(v.x*s, v.y*s, v.z*s);}
Vec3 operator * (double s, const Vec3 & v){return Vec3(v.x*s, v.y*s, v.z*s);}
Vec3 operator / (const Vec3 & v, double s){return Vec3(v.x/s, v.y/s, v.z/s);}

double get_angle(const Vec3 & a, const Vec3 & b){
	return acos(a.dot(b)/(a.norm() * b.norm()));
	}

double get_angle(const Vec2 & a, const Vec2 & b){
	return acos(a.dot(b)/(a.norm() * b.norm()));
	}

double get_distance(const Vec2 & a, const Vec2 & b){
	return (a-b).norm();
	}

double get_distance(const Vec3 & a, const Vec3 & b){
	return (a-b).norm();
	}
	
Vec3 rotate_around_axis(Vec3 & v, Vec3 & o, Vec3 & u, double angle){
	// Rotates the vector v around the axis defined by the 
	// origin o and the unit vector by the specified angle.
	// Adapted from the Wikipedia article: 
	// Axis-angle representation. (accessed online on the 10th of October 2025). 
	// Wikipedia. https://en.wikipedia.org/wiki/Axis%E2%80%93angle_representation
	Vec3 v2 = o + (v-o) + sin(angle) * (u.cross(v-o)) + (1. -cos(angle))*(u.cross(u.cross(v-o)));
	return v2;
	}

#endif // GEOMETRY_HPP


