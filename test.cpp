#include "geometry.hpp"
#include <cassert>
#include <iostream>

void test_vec2(){
	assert(Vec2(1, 2) + Vec2(2, 3) == Vec2(3, 5));
	assert(Vec2(1, 2) - Vec2(3, 2) == Vec2(-2, 0));
	assert(Vec2(1, 2) * 2 == Vec2(2, 4));
	assert(2 * Vec2(1, 2) == Vec2(2, 4));
	assert(Vec2(1, 2) / 2   == Vec2(0.5, 1));
	
	Vec2 v;
	v = Vec2(1, 2);
	v += Vec2(2, 3);
	assert(v == Vec2(3, 5));

	v = Vec2(1, 2);
	v -= Vec2(3, 2);
	assert(v == Vec2(-2, 0));
	
	v = Vec2(1, 2);
	v *= 2;
	assert(v == Vec2(2, 4));
	
	v = Vec2(1, 2);
	v /= 2;
	assert(v == Vec2(0.5, 1));
	
	assert(Vec2(1, 2).dot(Vec2(2, 3)) == 8);
	assert(Vec2(1, 0).norm() == 1);
	assert(Vec2(0, 1).norm() == 1);
	assert(Vec2(0, -1).norm() == 1);
	assert(Vec2(2, 0).norm() == 2);
	
	v = Vec2(2, 0);
	v.normalize();
	assert(v == Vec2(1, 0));
	
	v = Vec2(2, 0);
	Vec2 w = v.unit();
	v.normalize();
	assert(v == w);
	}
	
void test_vec3(){
	// ===========================
	// operations and norm

	assert(Vec3(1, 2, 3) + Vec3(2, 3, 4) == Vec3(3, 5, 7));
	assert(Vec3(1, 2, 3) - Vec3(3, 2, 1) == Vec3(-2, 0, 2));
	assert(Vec3(1, 2, 3) * 2 == Vec3(2, 4, 6));
	assert(2 * Vec3(1, 2, 3) == Vec3(2, 4, 6));
	assert(Vec3(1, 2, 3) / 2   == Vec3(0.5, 1, 1.5));
	
	Vec3 v;
	v = Vec3(1, 2, 3);
	v += Vec3(2, 3, 4);
	assert(v == Vec3(3, 5, 7));

	v = Vec3(1, 2, 3);
	v -= Vec3(3, 2, 1);
	assert(v == Vec3(-2, 0, 2));
	
	v = Vec3(1, 2, 3);
	v *= 2;
	assert(v == Vec3(2, 4, 6));
	
	v = Vec3(1, 2, 3);
	v /= 2;
	assert(v == Vec3(0.5, 1, 1.5));
	
	assert(Vec3(1, 2, 3).dot(Vec3(1, 2, 3)) == 14);
	assert(Vec3(1, 0, 0).norm() == 1);
	assert(Vec3(0, 0, 1).norm() == 1);
	assert(Vec3(0, 0, -1).norm() == 1);
	assert(Vec3(2, 0, 0).norm() == 2);
	
	v = Vec3(2, 0, 0);
	v.normalize();
	assert(v == Vec3(1, 0, 0));
	
	v = Vec3(2, 0, 0);
	Vec3 w = v.unit();
	v.normalize();
	assert(v == w);
	
	// ===========================
	// rotation
	v = Vec3(1, 1, 0);
	Vec3 o(0, 0, 0);
	Vec3 u(0, 0, 1);
	Vec3 v2 = rotate_around_axis(v, o, u, M_PI);
	assert(get_distance(v2, Vec3(-1, -1, 0))<1e-10);
	
	// ===========================
	// distance
	v = Vec3(1, 0, 0);
	w = Vec3(-1, 0, 0);
	assert(get_distance(v, w)==2);
	
	//===========================
	//angle
	v = Vec3(1, 0, 0);
	w = Vec3(0, 1, 0);
	assert(fabs(get_angle(v, w) - M_PI/2)<1e-10);
	
	v = Vec3(1, 0, 0);
	w = Vec3(1, 1, 0);
	assert(fabs(get_angle(v, w) - M_PI/4)<1e-10);
	}

void test(){
	test_vec3();
	test_vec2();
	}
	
int main(){
	test();
	return 0;
	}

