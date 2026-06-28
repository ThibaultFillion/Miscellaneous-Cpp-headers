#include <vector>
#include <cstdint>
#include <numeric>
#include <deque>
#include <array>
#include <cassert>
#include <iostream>
#include "geometry.hpp"
#include "numerics.hpp"

void assert_results(const std::vector<bool> & results){
	for(size_t i=0; i<results.size(); i++){
		std::string msg = results[i] ? "success" : "fail";
		std::cout<<"\tcase "<<i<<": "<<msg<<std::endl;
		assert(results[i]);
		}
	}

void test_are_abs_close(){
	std::cout<<"test_are_abs_close:"<<std::endl;
	
	double a = 16876;
	double b = 16800;
	double c = 1687;
	
	std::vector<bool> results(4);
	results[0] = not are_abs_close(a, b, 1);
	results[1] = are_abs_close(a, b, 100);
	results[2] = not are_abs_close(a, c, 100);
	results[3] = are_abs_close(a, b, 1e4);
	
	assert_results(results);
	}

void test_are_rel_close(){
	std::cout<<"test_are_rel_close:"<<std::endl;
	
	double a = 1;
	double b = 1.00000007;
	double c = 1.0000000007;
	
	std::vector<bool> results(4);
	results[0] = not are_rel_close(a, b);
	results[1] = are_rel_close(a, c);
	results[2] = are_rel_close(a, b, 1e-6);
	results[3] = not are_rel_close(a, b, 1e-12);
	
	assert_results(results);
	}
	
void test_compute_sum(){
	std::cout<<"test_compute_sum:"<<std::endl;
	
	std::vector<int>      v1 {1, 2};
	std::vector<uint16_t> v2 {1, 2};
	std::vector<double>   v3 {1, 2};
	std::vector<float>    v4 {1, 2};
	std::vector<Vec2>     v5 {Vec2(1, 2), Vec2(2, 3)};
	std::vector<Vec3>     v6 {Vec3(1, 2, 3), Vec3(2, 3, 4)};
	std::array<double, 2> v7 {1, 2};
	std::deque<double>    v8 {1, 2};
	std::deque<bool>      v9 {true, false, true, true};
	
	int      sum1 = compute_sum(v1);
	uint16_t sum2 = compute_sum(v2);
	double   sum3 = compute_sum(v3);
	float    sum4 = compute_sum(v4);
	Vec2     sum5 = compute_sum(v5);
	Vec3     sum6 = compute_sum(v6);
	double   sum7 = compute_sum(v7);
	double   sum8 = compute_sum(v8);
	int      sum9 = compute_sum(v9);
	
	std::vector<bool> results {
		sum1==3,
		sum2==3,
		are_rel_close(sum3, 3),
		are_rel_close(sum4, 3),
		are_rel_close(sum5.x, 3) and are_rel_close(sum5.y, 5),
		are_rel_close(sum6.x, 3) and are_rel_close(sum6.y, 5) and are_rel_close(sum6.z, 7),
		are_rel_close(sum7, 3),
		are_rel_close(sum8, 3),
		sum9==3
		};
	
	assert_results(results);
	}

void test_compute_average(){
	std::cout<<"test_compute_average:"<<std::endl;
	
	std::vector<int>      v1 {1, 2};
	std::vector<uint16_t> v2 {1, 2};
	std::vector<double>   v3 {1, 2};
	std::vector<float>    v4 {1, 2};
	std::vector<Vec2>     v5 {Vec2(1, 2), Vec2(2, 3)};
	std::vector<Vec3>     v6 {Vec3(1, 2, 3), Vec3(2, 3, 4)};
	std::array<double, 2> v7 {1, 2};
	std::deque<double>    v8 {1, 2};
	std::deque<bool>      v9 {true, false, true, true};
	
	auto avg1 = compute_average(v1);
	auto avg2 = compute_average(v2);
	auto avg3 = compute_average(v3);
	auto avg4 = compute_average(v4);
	auto avg5 = compute_average(v5);
	auto avg6 = compute_average(v6);
	auto avg7 = compute_average(v7);
	auto avg8 = compute_average(v8);
	auto avg9 = compute_average(v9);
	
	std::vector<bool> results {
		are_rel_close(avg1, 1.5),
		are_rel_close(avg2, 1.5),
		are_rel_close(avg3, 1.5),
		are_rel_close(avg4, 1.5),
		are_rel_close(avg5.x, 1.5) and are_rel_close(avg5.y, 2.5),
		are_rel_close(avg6.x, 1.5) and are_rel_close(avg6.y, 2.5) and are_rel_close(avg6.z, 3.5),
		are_rel_close(avg7, 1.5),
		are_rel_close(avg8, 1.5),
		are_rel_close(avg9, 0.75)
		};
	
	assert_results(results);
	}

void test_compute_stdev(){
	std::cout<<"test_compute_stdev:"<<std::endl;
	
	std::vector<int>      v1 {1, 2};
	std::vector<uint16_t> v2 {1, 2};
	std::vector<double>   v3 {1, 2};
	std::vector<float>    v4 {1, 2};
	std::array<double, 2> v5 {1, 2};
	std::deque<double>    v6 {1, 2};
	std::deque<bool>      v7 {true, false, true, true};
	
	auto stdev1 = compute_stdev(v1);
	auto stdev2 = compute_stdev(v2);
	auto stdev3 = compute_stdev(v3);
	auto stdev4 = compute_stdev(v4);
	auto stdev5 = compute_stdev(v5);
	auto stdev6 = compute_stdev(v6);
	auto stdev7 = compute_stdev(v7);
	
	std::vector<bool> results {
		are_rel_close(stdev1, 0.5),
		are_rel_close(stdev2, 0.5),
		are_rel_close(stdev3, 0.5),
		are_rel_close(stdev4, 0.5),
		are_rel_close(stdev5, 0.5),
		are_rel_close(stdev6, 0.5),
		are_rel_close(stdev7, 0.433012701892)
		};
	
	assert_results(results);
	}
	
void test_make_linscale(){
	std::cout<<"test_make_linscale"<<std::endl;
	
	std::vector<double> v1 = make_linscale(1, 3, 4);
	std::vector<double> v2 = make_linscale(2, 5, 4);
	
	bool result1 =
		v1.size() == 4 and
		are_rel_close(v1[0], 1) and
		are_rel_close(v1[1], 1.6666666666666666) and
		are_rel_close(v1[2], 2.3333333333333333) and
		are_rel_close(v1[3], 3);

	bool result2 =
		v2.size() == 4 and
		are_rel_close(v2[0], 2) and
		are_rel_close(v2[1], 3) and
		are_rel_close(v2[2], 4) and
		are_rel_close(v2[3], 5);
	
	std::vector<bool> results = {
		result1,
		result2
		};
	
	assert_results(results);
	}

void test_make_logscale(){
	std::cout<<"test_make_logscale"<<std::endl;
	
	std::vector<double> v1 = make_logscale(1e-1, 1e1, 4);
	std::vector<double> v2 = make_logscale(1e2, 1e5, 4);
	
	bool result1 =
		v1.size() == 4 and
		are_rel_close(v1[0], 1e-1) and
		are_rel_close(v1[1], std::pow(10, -0.333333333333333)) and
		are_rel_close(v1[2], std::pow(10, +0.333333333333333)) and
		are_rel_close(v1[3], 1e1);

	bool result2 =
		v2.size() == 4 and
		are_rel_close(v2[0], 1e2) and
		are_rel_close(v2[1], 1e3) and
		are_rel_close(v2[2], 1e4) and
		are_rel_close(v2[3], 1e5);
	
	std::vector<bool> results = {
		result1,
		result2
		};
	
	assert_results(results);
	}

int main(){
	test_are_abs_close();
	test_are_rel_close();
	test_compute_sum();
	test_compute_average();
	test_compute_stdev();
	test_make_linscale();
	test_make_logscale();
	return 0;
	}