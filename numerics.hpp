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
// Miscellaneous number-related functions.
// 
// functions:
// - are_abs_close: compare double floating point (absolute tolerance) 
// - are_rel_close: compare double floating point (relative tolerance)
// - get_sign: returns the sign of a number
// - compute_sum: sum elements of a container
// - compute_average: average elements of a container
// - compute_stdev: compute standard deviation for elements of a container
// - make_linscale: generate a range of value in a specified interval
// - make_logscale: generate a range of value in a specified interval
//   in logarithmic scale
// ==============================================================================

#ifndef NUMERICS_HPP
#define NUMERICS_HPP

#include <cmath>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <numeric>

bool are_abs_close(double a, double b, double abs_tol){
	// absolute double floating point comparison.
	//  
	// Reference:
	// Chrispopher Barker. (2015). PEP 485 - A function for testing
	// approximate equality. Python Enhancement Proposals.
	// https://peps.python.org/pep-0485/#defaults

	bool success = std::abs(a - b) < abs_tol;
	return success;
	}

bool are_rel_close(double a, double b, double rel_tol = 1e-9){
	// relative double floating point comparison.
	//
	// Reference:
	// Chrispopher Barker. (2015). PEP 485 - A function for testing
	// approximate equality. Python Enhancement Proposals.
	// https://peps.python.org/pep-0485/#defaults

	bool success = std::abs(a - b) < rel_tol * std::max(std::abs(a), std::abs(b));
	return success;
	}

template <typename T>
int get_sign(const T & val){
	// returns the sign of an integer:
	//   1 if val>=0
	//  -1 otherwise
	
	int sign = -1 + (val >= 0) * 2;
	return sign;
	}

template <typename T_container>
auto compute_sum(const T_container & container){
	// returns the sum of the elements inside the container.
	// should work on both numeric types and 
	// Vec2/Vec3.
	// in the case of bool, the return type is int.
	
	using T_in = typename T_container::value_type;
	using T_out = typename std::conditional<std::is_same<T_in, bool>::value, int, T_in>::type;
	
	T_out sum = std::accumulate(
		container.begin(), 
		container.end(), 
		T_out()
		);
	return sum;
	}

template <typename T_container>
auto compute_average(const T_container & container){
	// returns the arithmetic average of the 
	// elements inside the container.
	// should work on both numeric types and 
	// Vec2/Vec3.
	// integer types and bool
	// are promoted to double.
	
	auto sum = compute_sum(container);
	
	using T_in = decltype(sum);
	using T_out = typename std::conditional<std::is_integral<T_in>::value, double, T_in>::type;
	
	T_out average = static_cast<T_out>(sum) / container.size();
	return average;
	}

template <typename T_container>
auto compute_stdev(const T_container & container){
	// returns the standard deviation to the mean
	// of the elements inside the container.
	// should work only on numeric types and 
	// not on Vec2/Vec3.
	// integer types and bool
	// are promoted to double.
	
	auto average = compute_average(container);
	
	using T_out = decltype(average);
	
	T_out sqres_sum(0);
	for(auto & v: container){
		sqres_sum += std::pow(T_out(v) - average, 2);
		}
	
	T_out stdev = std::sqrt(sqres_sum / container.size());
	return stdev;
	}
	
std::vector<double> make_linscale(double vmin, double vmax, int nval){
	// generates a range of nval values from
	// vmin to vmax (included) on a linear scale
	// in a std::vector of double
	
	double delta = (vmax - vmin) / (nval - 1);
	std::vector<double> v(nval);
	
	for(int i = 0; i < nval; i++){
		v[i] = vmin + delta * i;
		}
	
	return v;
	}

std::vector<double> make_logscale(double vmin, double vmax, int nval){
	// generates a range of nval values from
	// vmin to vmax (included) on a log10 scale
	// in a std::vector of double
	
	double logvmin = std::log10(vmin);
	double logvmax = std::log10(vmax);
	double logdelta = (logvmax - logvmin) / (nval - 1);
	std::vector<double> v(nval);
	
	for(int i = 0; i < nval; i++){
		v[i] = std::pow(10, logvmin + logdelta * i);
		}
	
	return v;
	}
	
#endif // NUMERICS_HPP
