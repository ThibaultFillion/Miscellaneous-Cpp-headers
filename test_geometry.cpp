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

#include <cassert>
#include <iostream>
#include <vector>

#include "geometry.hpp"
#include "numerics.hpp"

void assert_results(const std::vector<bool> & results){
	for(size_t i=0; i<results.size(); i++){
		std::string msg = results[i] ? "success" : "fail";
		std::cout<<"\tcase "<<i<<": "<<msg<<std::endl;
		assert(results[i]);
		}
	}

void test_vec2(){
	std::cout<<"test_vec2"<<std::endl;
	
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
	std::cout<<"test_vec3"<<std::endl;
	
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

void test_test_box_sphere_intersection(){
	std::cout<<"test_test_box_sphere_intersection"<<std::endl;
	
	double r = 1.2;
	Vec3 p0(1, 2, 3); // min corner
	Vec3 dp(3.1, 7, 1); // extent (strictly positive)
	Vec3 p1 = p0 + dp; // max corner
	Vec3 v = p1 - p0; // p0 to p1 vec
	Vec3 u = v/v.norm(); // unit vec for v
	Vec3 c1 = p1 + 0.9 * r * u; // center of sphere 1
	Vec3 c2 = p1 + 1.1 * r * u;	// center of sphere 2
	Vec3 c3 = p0 - 0.9 * r * u; // center of sphere 3
	Vec3 c4 = p0 - 1.1 * r * u;	// center of sphere 4
	assert(test_box_sphere_intersection(p0, p1, c1, r));
	assert(test_box_sphere_intersection(p0, p1, c2, r) == false);	
	assert(test_box_sphere_intersection(p0, p1, c3, r));
	assert(test_box_sphere_intersection(p0, p1, c4, r) == false);
	}

void test_spherical_to_or_from_cartesian(){
	std::cout<<"test_spherical_to_or_from_cartesian"<<std::endl;
	
	Vec3 v1(0, 1, 0);
	Vec3 v2(0.5, 0, 0);
	Vec3 v3(0, 0, 2);
	Vec3 v4(0, 0, 0);
	Vec3 v5(0, 0, -1);
	Vec3 v6(-1, 0, 0);
	Vec3 v7(-1, -1, 0);
	
	Vec3 v8(1.234, 3.42, 288);
	Vec3 v9(-1.234, 3.42, 288);
	Vec3 v10(1.234, -3.42, 288);
	Vec3 v11(1.234, 3.42, -288);
	Vec3 v12(-1.234, -3.42, 288);
	Vec3 v13(1.234, -3.42, -288);
	Vec3 v14(-1.234, 3.42, -288);
	Vec3 v15(-1.234, 3.42, -288);

	SphericalCoordinates c1 = cartesian_to_spherical(v1);
	SphericalCoordinates c2 = cartesian_to_spherical(v2);
	SphericalCoordinates c3 = cartesian_to_spherical(v3);
	SphericalCoordinates c4 = cartesian_to_spherical(v4);
	SphericalCoordinates c5 = cartesian_to_spherical(v5);
	SphericalCoordinates c6 = cartesian_to_spherical(v6);
	SphericalCoordinates c7 = cartesian_to_spherical(v7);
	
	std::vector<Vec3> all_v = {
		v1, v2, v3, v4, v5, v6, v7, 
	    v8, v9, v10, v11, v12, v13,
		v14, v15
		}; 
		
	bool validate_spherical_to_cartesian = true;
	for(auto & v: all_v){
		Vec3 v2 = spherical_to_cartesian(cartesian_to_spherical(v));
		bool same_vec = 
			are_abs_close(v.x, v2.x, 1e-10) and
			are_abs_close(v.y, v2.y, 1e-10) and
			are_abs_close(v.z, v2.z, 1e-10);
		validate_spherical_to_cartesian &= same_vec;
		}
	
	std::vector<bool> results {
		are_rel_close(c1.r, 1) and are_rel_close(c1.phi, M_PI / 2) and are_rel_close(c1.theta, M_PI / 2),	
		are_rel_close(c2.r, 0.5) and are_abs_close(c2.phi, 0, 1e-10) and are_rel_close(c2.theta, M_PI / 2), 
		are_rel_close(c3.r, 2) and c3.phi == 0 and c3.theta == 0,
		are_abs_close(c4.r, 0, 1e-10) and c4.phi == 0 and c4.theta == 0,
		are_rel_close(c5.r, 1) and c5.phi == 0 and are_rel_close(c5.theta, M_PI),
		are_rel_close(c6.r, 1) and are_rel_close(c6.phi, M_PI) and are_rel_close(c6.theta, M_PI / 2), 		
		are_rel_close(c7.r, std::sqrt(2)) and 
			(are_rel_close(c7.phi, 5 * M_PI / 4) or are_rel_close(c7.phi, -3 * M_PI / 4)) and 
			are_rel_close(c7.theta, M_PI / 2),
		validate_spherical_to_cartesian
		};
	
	assert_results(results);
	}

void test(){
	test_vec3();
	test_vec2();
	test_test_box_sphere_intersection();
	test_spherical_to_or_from_cartesian();
	}
	
int main(){
	test();
	return 0;
	}
