// ==============================================================================
// MIT No Attribution
// 
// Copyright 2026 Thibault Fillion
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
// simple convenience wrapper around the
// standard random header, to handle common 
// random number generation in a compact
// and modular way.
//
// classes:
// - RandomNumberGenerator: class handling high-level RNG
//
// functions:
// - generate_rand_seed_seq: generates a sequence of rng seeds
// ==============================================================================

#ifndef RNG_HPP
#define RNG_HPP

#include<random>
#include<vector>
#include<limits>
#include<numeric>

class RandomNumberGenerator{
	private:
		std::mt19937 rng;
		std::uniform_int_distribution<int> int_dist;
		std::uniform_real_distribution<double> unit_dist;
		
	public:
		RandomNumberGenerator(uint_fast32_t seed=0):
			rng(seed),
			int_dist(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()),
			unit_dist(0, 1){
			}

		int rand_int(){
			
			// draws random int integer in
			// [minimum int, maximum int]
			
			return int_dist(rng);
			}
					
		double unit_rand(){
		
			// draws random double in [0, 1),
			// i.e 0 <= x~unit_rand < 1
			
			return unit_dist(rng);
			}
		
		bool btrial(double p){
		
			// Bernoulli trial on probability p
			
			return unit_rand() < p;
			}
		
		int draw(const std::vector<double> & weights){
			
			// draws an index (int) from 
			// a discrete distribution
			
			double total = std::accumulate(weights.begin(), weights.end(), 0.);
			return draw(weights, total);
			}
			
		int draw(const std::vector<double> & weights, double total){
			
			// draws an index (int) from 
			// a discrete distribution,
			// using the pre-computed
			// weight sum
			
			double r = unit_rand()*total;
			double cumul = 0;
			for(size_t i = 0; i < weights.size(); i++){
				cumul += weights[i];
				if(cumul >= r){
					return i;
					}
				}
			return static_cast<int>(weights.size()-1);
			}
	};

// generates a sequence of RNG seeds

std::vector<uint_fast32_t> generate_rand_seed_seq(int seq_size, uint_fast32_t seed){
	std::mt19937 rng(seed);
	std::vector<uint_fast32_t> seed_seq(seq_size);
	for(int i = 0; i < seq_size; i++){
		seed_seq[i] = rng();
		}	
	return seed_seq;
	}

#endif // RNG_HPP
