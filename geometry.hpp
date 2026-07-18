// ==============================================================================
// MIT No Attribution
// 
// Copyright 2025 Thibault Fillion
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// ==============================================================================

// ==============================================================================
// Very simple header only include for some basic geometry classes and functions.
// 
// classes:
// - Vec2: two dimensions vector class
// - Vec3: three dimensions vector class
// - SphericalCoordinates: POD for spherical coordinates
// 
// functions:
// - rotate_around_axis: rotate a point around an axis by some angle
// - get_distance: returns the distance between two points
// - get_angle: returns the angle formed between two vectors
// - test_box_sphere_intersection: test the intersection between
//   a sphere and a box.
// - spherical_to_cartesian: spherical to Cartesian coordinates conversion
// - cartesian_to_spherical: Cartesian to spherical coordinates conversion
// ==============================================================================

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
	bool operator == (const Vec2 & v) const {return (x == v.x and y == v.y);}
	double norm() const {return std::sqrt(x * x + y * y);}
	double dot(const Vec2 & v) const {return v.x * x + v.y * y;}
	Vec2 unit() const {double n = norm(); return Vec2(x / n, y / n);}
	Vec2 & normalize(){double n = norm(); x /= n; y /= n; return *this;}
	};

inline Vec2 operator + (const Vec2 & v){return v;}
inline Vec2 operator - (const Vec2 & v){return Vec2(-v.x, -v.y);}
inline Vec2 operator + (const Vec2 & a, const Vec2 & b){return Vec2(a.x + b.x, a.y + b.y);}
inline Vec2 operator - (const Vec2 & a, const Vec2 & b){return Vec2(a.x - b.x, a.y - b.y);}
inline Vec2 operator * (const Vec2 & v, double s){return Vec2(v.x * s, v.y * s);}
inline Vec2 operator * (double s, const Vec2 & v){return Vec2(v.x * s, v.y * s);}
inline Vec2 operator / (const Vec2 & v, double s){return Vec2(v.x / s, v.y / s);}

class Vec3{
	public:
	double x, y, z;
	
	Vec3(double x=0, double y=0, double z=0): x(x), y(y), z(z) {}	
	Vec3 & operator += (const Vec3 & v){x += v.x; y += v.y; z += v.z; return *this;}
	Vec3 & operator -= (const Vec3 & v){x -= v.x; y -= v.y; z -= v.z; return *this;}
	Vec3 & operator *= (double s){x *= s; y *= s; z *= s; return *this;}
	Vec3 & operator /= (double s){x /= s; y /= s; z /= s; return *this;}
	bool operator == (const Vec3 & v) const {return (x == v.x and y == v.y and z == v.z);} 
	double norm() const {return std::sqrt(x * x + y * y + z * z);}
	double dot(const Vec3 & v) const {return v.x * x + v.y * y + v.z * z;}
	Vec3 cross(const Vec3 & v) const {
		// adapted from reference:
		// Cross product. (accessed online on the 10th of October 2025). 
		// Wikipedia. https://en.wikipedia.org/wiki/Axis%E2%80%93angle_representation
		return Vec3(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x
			);
		}
	Vec3 unit() const {double n = norm(); return Vec3(x / n, y / n, z / n);}
	Vec3 & normalize(){double n = norm(); x /= n; y /= n; z /= n; return *this;}
	};

inline Vec3 operator + (const Vec3 & v){return v;}
inline Vec3 operator - (const Vec3 & v){return Vec3(-v.x, -v.y, -v.z);}
inline Vec3 operator + (const Vec3 & a, const Vec3 & b){return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);}
inline Vec3 operator - (const Vec3 & a, const Vec3 & b){return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);}
inline Vec3 operator * (const Vec3 & v, double s){return Vec3(v.x * s, v.y * s, v.z * s);}
inline Vec3 operator * (double s, const Vec3 & v){return Vec3(v.x * s, v.y * s, v.z * s);}
inline Vec3 operator / (const Vec3 & v, double s){return Vec3(v.x / s, v.y / s, v.z / s);}

inline double get_angle(const Vec3 & a, const Vec3 & b){
	return std::acos(std::max(-1., std::min(1., a.dot(b) / (a.norm() * b.norm()))));
	}

inline double get_angle(const Vec2 & a, const Vec2 & b){
	return std::acos(std::max(-1., std::min(1., a.dot(b) / (a.norm() * b.norm()))));
	}

inline double get_distance(const Vec2 & a, const Vec2 & b){
	return (a - b).norm();
	}

inline double get_distance(const Vec3 & a, const Vec3 & b){
	return (a - b).norm();
	}
	
inline Vec3 rotate_around_axis(
	const Vec3 & v, 
	const Vec3 & o,
	const Vec3 & u,
	double angle
	){
	// Rotates the vector v around the axis defined by the 
	// origin o and the unit vector by the specified angle.
	// Adapted from the Wikipedia article: 
	// Axis-angle representation. (accessed online on the 10th of October 2025). 
	// Wikipedia. https://en.wikipedia.org/wiki/Axis%E2%80%93angle_representation

	// equivalent to:
	// Vec3 v2 = o + (v - o) + sin(angle) * (u.cross(v - o)) + (1. -cos(angle)) * (u.cross(u.cross(v - o)));
	
	Vec3 p = u.cross(v - o);
	Vec3 v2 = v + std::sin(angle) * p + (1. -std::cos(angle)) * (u.cross(p));

	return v2;
	}

inline bool test_box_sphere_intersection(
	const Vec3 & box_min_pos,
	const Vec3 & box_max_pos,
	const Vec3 & sphere_pos,
	const double sphere_radius
	){
	// Tests whether a sphere, defined by its 
	// center and radius, intersects with a box,
	// defined by its min and max corners.
	// Returns true is applicable, false otherwise.
	//
	// Reference:
	// James Arvo. (1995). A simple method for box-sphere
	// intersection testing. In Andrew S. Glassner (Ed.),
	// Graphics gems (1st ed., pp. 335-339). Academic press.
	// https://doi.org/10.1016/B978-0-08-050753-8.50071-1

	// closest point p to the sphere within the box 	
	Vec3 p(
		std::max(box_min_pos.x, std::min(box_max_pos.x, sphere_pos.x)),
		std::max(box_min_pos.y, std::min(box_max_pos.y, sphere_pos.y)),
		std::max(box_min_pos.z, std::min(box_max_pos.z, sphere_pos.z))
		);
	
	// squared distance d between the sphere center 
	// (sphere_pos) and p
	double sd = 
		std::pow(p.x - sphere_pos.x, 2) +
		std::pow(p.y - sphere_pos.y, 2) +
		std::pow(p.z - sphere_pos.z, 2);
	
	// intersection if sd <= sphere_radius^2
	return sd <= sphere_radius*sphere_radius;
	}

struct SphericalCoordinates{
	double phi;
	double theta;
	double r;
	};

inline Vec3 spherical_to_cartesian(const SphericalCoordinates & c){
	// converts spherical coordinates (SphericalCoordinates)
	// into Cartesian coordinates (Vec3)
	//
	// adapted from:
	// Spherical coordinate system. Wikipedia.
	// accessed online: 1st of july, 2026.
	// https://en.wikipedia.org/wiki/Spherical_coordinate_system
	
	Vec3 v(
		c.r * std::sin(c.theta) * std::cos(c.phi),
		c.r * std::sin(c.theta) * std::sin(c.phi),
		c.r * std::cos(c.theta)
		);
	
	return v;
	}

inline SphericalCoordinates cartesian_to_spherical(const Vec3 & v){
	// converts Cartesian coordinates (Vec3)
	// into spherical coordinates (SphericalCoordinates)
	//
	// adapted from:
	// Spherical coordinate system. Wikipedia.
	// accessed online: 1st of july, 2026.
	// https://en.wikipedia.org/wiki/Spherical_coordinate_system
	
	double norm = v.norm();
	double norm_xy = std::sqrt(v.x * v.x + v.y * v.y);
	int y_sign = -1 + (v.y >= 0) * 2;
	
	if(norm == 0){
		return {0, 0, 0};
		}

	SphericalCoordinates c;
	c.r = norm;	
	c.theta = std::acos(v.z / c.r);
	
	if(norm_xy == 0){
		c.phi = 0;
		}
	else{
		c.phi = y_sign * std::acos(v.x / norm_xy);
		}
	
	return c;
	}

#endif // GEOMETRY_HPP


