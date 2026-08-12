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
// header only implementing conversion between
// common physical units.
//
// function: 
// - convert: convert a value from one physical unit to another.
// - get_conversion_factor: get the conversion factor between
//   two physical units.
// ==============================================================================

#ifndef PHYSICAL_UNITS_HPP
#define PHYSICAL_UNITS_HPP

#include <string_view>
#include <map>
#include <cmath>

struct PhysicalUnits{
	int s;
	int mol;
	int m;
	int g;
	int K;
	double factor;
	};

class PhysicalUnitsParser{
	private:
	
		PhysicalUnits output;
		
		inline static const double femto = 1e-15;
		inline static const double pico  = 1e-12;
		inline static const double nano  = 1e-9;
		inline static const double micro = 1e-6;
		inline static const double milli = 1e-3;
		inline static const double centi = 1e-2;
		inline static const double deci  = 1e-1;
		inline static const double uni   = 1e0;
		inline static const double kilo  = 1e3;

		enum class Unit{
			Meter,
			Liter,
			Joule,
			Watt,
			Mole,
			Molar,
			Second,
			Kelvin,
			Newton,
			Gram,
			Pascal
			};

		static const std::map<std::string, std::pair<double, Unit>> unit_dict;		
		std::string_view src;
		size_t next_block_start;
		size_t block_start;
		size_t block_end;
		bool invert_block;
		bool invert_next_block;
		bool last_block;
		
		std::string_view strip_whitespace(const std::string_view & str){
			
			if(str.size() == 0){
				return str;
				}
			
			// leading whitespaces
			
			size_t strip_start = 0;
			
			for(size_t i=0; i<str.size(); i++){
				if(str[i] == ' '){
					strip_start = i+1;
					}
				else{
					break;
					}
				}
						
			if(strip_start == str.size()){
				return str.substr(0, 0);
				}
			
			// trailing whitespaces
			
			size_t strip_end = str.size();
			
			for(size_t i=str.size()-1; i>strip_start; i--){
				if(str[i] == ' '){
					strip_end = i;
					}
				else{
					break;
					}
				}
			
			return str.substr(strip_start, strip_end - strip_start);
			}
		
		void register_block(const std::string_view & symbol, int exponent){
			auto pair = unit_dict.find(std::string(symbol));
			
			if(pair == unit_dict.end()){
				throw std::logic_error("undefined unit symbol \"" + std::string(symbol) + "\"");
				}
			
			const std::pair<double, Unit> & entry = pair->second;
			
			switch(entry.second){
				case Unit::Meter:{
					output.m += exponent;
					output.factor *= std::pow(entry.first, exponent);
					break;
					}
				case Unit::Liter:{
					// liter = (0.1m)^3
					output.m += 3*exponent;
					output.factor *= std::pow(1e-1*pow(entry.first, 1.0/3.0), 3*exponent);
					break;
					}
				case Unit::Joule:{
					// joule = kg.m^2/s^2
					output.m += 2*exponent;
					output.g += exponent;
					output.s -= 2*exponent;
					output.factor *= std::pow(kilo*entry.first, exponent);
					break;
					}
				case Unit::Watt:{
					// watt = J/s = kg.m^2/s^3
					output.m += 2*exponent;
					output.g += exponent;
					output.s -= 3*exponent;
					output.factor *= std::pow(kilo*entry.first, exponent);
					break;
					}
				case Unit::Mole:{
					output.mol += exponent;
					output.factor *= std::pow(entry.first, exponent);
					break;
					}
				case Unit::Molar:{
					// molar = (mol/L)
					// mol
					output.mol += exponent;
					output.factor *= std::pow(entry.first, exponent);
					// 1/L
					output.m -= 3*exponent;
					output.factor /= std::pow(1e-1*pow(1.0, 1.0/3.0), 3*exponent);
					break;
					}
				case Unit::Second:{
					output.s += exponent;
					output.factor *= std::pow(entry.first, exponent);
					break;
					}
				case Unit::Kelvin:{
					output.K += exponent;
					output.factor *= std::pow(entry.first, exponent);
					break;
					}
				case Unit::Newton:{
					// newton = kg.m/s^2
					output.m += exponent;
					output.g += exponent;
					output.s -= 2*exponent;
					output.factor *= std::pow(kilo*entry.first, exponent);
					break;
					}
				case Unit::Gram:{
					output.g += exponent;
					output.factor *= std::pow(entry.first, exponent);
					break;
					}
				case Unit::Pascal:{
					// pascal = kg/m/s^2
					output.m -= exponent;
					output.g += exponent;
					output.s -= 2*exponent;
					output.factor *= std::pow(kilo*entry.first, exponent);
					break;					
					}
				};
			}
		
		void search_next_block(){
			for(size_t i=block_start; i<src.size(); i++){
				if(src[i] == '/' or src[i] == '.'){
					block_end = i;
					next_block_start = i+1;
					invert_next_block = (src[i] == '/');
					return;
					}
				}
			last_block = true;
			block_end = src.size();
			}
		
		void parse_block(){
			size_t block_size = block_end - block_start;
			auto block = src.substr(
				block_start,
				block_size
				);
			std::string_view symbol;
			int exponent = 1;
			size_t exponent_pos = block.find('^');
			if(exponent_pos != std::string::npos){
				symbol  = block.substr(0, exponent_pos);
				std::string exp_str = std::string(block.substr(exponent_pos+1, block_size));
				try{
					exponent = std::stoi(exp_str);
					}
				catch(std::invalid_argument){
					throw std::logic_error("invalid unit exponent: \""+exp_str+"\"");
					}
				}
			else{
				symbol = block;
				}
			
			if(invert_block){
				exponent = -exponent;
				}
			
			symbol = strip_whitespace(symbol);
			register_block(symbol, exponent);
			}
		
		static std::map<std::string, std::pair<double, Unit>> create_unit_dict(){
			return {
				// meter
				{"fm", {femto, Unit::Meter}},
				{"pm", {pico, Unit::Meter}},
				{"nm", {nano, Unit::Meter}},
				{"um", {micro, Unit::Meter}},
				{"µm", {micro, Unit::Meter}},
				{"mm", {milli, Unit::Meter}},
				{"cm", {centi, Unit::Meter}},
				{"dm", {deci, Unit::Meter}},
				{"m",  {uni, Unit::Meter}},
				{"km", {kilo, Unit::Meter}},

				// liter (with uppercase L)
				{"fL", {femto, Unit::Liter}},
				{"pL", {pico, Unit::Liter}},
				{"nL", {nano, Unit::Liter}},
				{"uL", {micro, Unit::Liter}},
				{"µL", {micro, Unit::Liter}},
				{"mL", {milli, Unit::Liter}},
				{"cL", {centi, Unit::Liter}},
				{"dL", {deci, Unit::Liter}},
				{"L",  {uni, Unit::Liter}},
				{"kL", {kilo, Unit::Liter}},
				
				// liter (with lowercase L)
				{"fl", {femto, Unit::Liter}},
				{"pl", {pico, Unit::Liter}},
				{"nl", {nano, Unit::Liter}},
				{"ul", {micro, Unit::Liter}},
				{"µl", {micro, Unit::Liter}},
				{"ml", {milli, Unit::Liter}},
				{"cl", {centi, Unit::Liter}},
				{"dl", {deci, Unit::Liter}},
				{"l",  {uni, Unit::Liter}},
				{"kl", {kilo, Unit::Liter}},

				// molar
				{"fM", {femto, Unit::Molar}},
				{"pM", {pico, Unit::Molar}},
				{"nM", {nano, Unit::Molar}},
				{"uM", {micro, Unit::Molar}},
				{"µM", {micro, Unit::Molar}},
				{"mM", {milli, Unit::Molar}},
				{"cM", {centi, Unit::Molar}},
				{"dM", {deci, Unit::Molar}},
				{"M",  {uni, Unit::Molar}},
				{"kM", {kilo, Unit::Molar}},

				// mole
				{"fmol", {femto, Unit::Mole}},
				{"pmol", {pico, Unit::Mole}},
				{"nmol", {nano, Unit::Mole}},
				{"umol", {micro, Unit::Mole}},
				{"µmol", {micro, Unit::Mole}},
				{"mmol", {milli, Unit::Mole}},
				{"cmol", {centi, Unit::Mole}},
				{"dmol", {deci, Unit::Mole}},
				{"mol",  {uni, Unit::Mole}},
				{"kmol", {kilo, Unit::Mole}},

				// gram
				{"fg", {femto, Unit::Gram}},
				{"pg", {pico, Unit::Gram}},
				{"ng", {nano, Unit::Gram}},
				{"ug", {micro, Unit::Gram}},
				{"µg", {micro, Unit::Gram}},
				{"mg", {milli, Unit::Gram}},
				{"cg", {centi, Unit::Gram}},
				{"dg", {deci, Unit::Gram}},
				{"g",  {uni, Unit::Gram}},
				{"kg", {kilo, Unit::Gram}},

				// second
				{"fs", {femto, Unit::Second}},
				{"ps", {pico, Unit::Second}},
				{"ns", {nano, Unit::Second}},
				{"us", {micro, Unit::Second}},
				{"µs", {micro, Unit::Second}},
				{"ms", {milli, Unit::Second}},
				{"s",  {uni, Unit::Second}},
				{"ks", {kilo, Unit::Second}},

				{"min", {60.0, Unit::Second}},
				{"h",   {3600.0, Unit::Second}},
				{"day", {86400.0, Unit::Second}},

				// kelvin
				{"fK", {femto, Unit::Kelvin}},
				{"pK", {pico, Unit::Kelvin}},
				{"nK", {nano, Unit::Kelvin}},
				{"uK", {micro, Unit::Kelvin}},
				{"µK", {micro, Unit::Kelvin}},
				{"mK", {milli, Unit::Kelvin}},
				{"K",  {uni, Unit::Kelvin}},
				{"kK", {kilo, Unit::Kelvin}},

				// joule
				{"fJ", {femto, Unit::Joule}},
				{"pJ", {pico, Unit::Joule}},
				{"nJ", {nano, Unit::Joule}},
				{"uJ", {micro, Unit::Joule}},
				{"µJ", {micro, Unit::Joule}},
				{"mJ", {milli, Unit::Joule}},
				{"J",  {uni, Unit::Joule}},
				{"kJ", {kilo, Unit::Joule}},

				//watt
				{"fW", {femto, Unit::Watt}},
				{"pW", {pico, Unit::Watt}},
				{"nW", {nano, Unit::Watt}},
				{"uW", {micro, Unit::Watt}},
				{"µW", {micro, Unit::Watt}},
				{"mW", {milli, Unit::Watt}},
				{"W",  {uni, Unit::Watt}},
				{"kW", {kilo, Unit::Watt}},

				//newton
				{"fN", {femto, Unit::Newton}},
				{"pN", {pico, Unit::Newton}},
				{"nN", {nano, Unit::Newton}},
				{"uN", {micro, Unit::Newton}},
				{"µN", {micro, Unit::Newton}},
				{"mN", {milli, Unit::Newton}},
				{"N",  {uni, Unit::Newton}},
				{"kN", {kilo, Unit::Newton}},

				//pascal
				{"fPa", {femto, Unit::Pascal}},
				{"pPa", {pico, Unit::Pascal}},
				{"nPa", {nano, Unit::Pascal}},
				{"uPa", {micro, Unit::Pascal}},
				{"µPa", {micro, Unit::Pascal}},
				{"mPa", {milli, Unit::Pascal}},
				{"Pa",  {uni, Unit::Pascal}},
				{"kPa", {kilo, Unit::Pascal}},
				};
			}
			
	public:
		
		PhysicalUnitsParser(const std::string & src): src(src){
			
			output.s   = 0;
			output.mol = 0;
			output.m   = 0;
			output.g   = 0;
			output.K   = 0;
			output.factor = 1.0;
			
			block_start = 0;
			last_block = false;
			invert_block = false;
			
			if(src.empty()){
				throw std::logic_error("empty unit string");
				}
			
			while(not last_block){
				search_next_block();
				parse_block();
				block_start = next_block_start;
				invert_block = invert_next_block;
				}
			}
		
		const PhysicalUnits & get_output(){
			return output;
			}
	};

const std::map<std::string, std::pair<double, PhysicalUnitsParser::Unit>> PhysicalUnitsParser::unit_dict = PhysicalUnitsParser::create_unit_dict();

inline PhysicalUnits parse_physical_units(const std::string & src){	
	PhysicalUnitsParser parser(src);
	return parser.get_output();
	}

inline bool same_physical_units_dimensions(const PhysicalUnits & src_units, const PhysicalUnits & dst_units){
	return (
		src_units.s == dst_units.s and
		src_units.mol == dst_units.mol and
		src_units.m == dst_units.m and
		src_units.g == dst_units.g and
		src_units.K == dst_units.K	
		);
	}

inline double get_conversion_factor(const std::string & src_units, const std::string & dst_units){
	PhysicalUnits src_units_obj = parse_physical_units(src_units);
	PhysicalUnits dst_units_obj = parse_physical_units(dst_units);
	
	if(not same_physical_units_dimensions(src_units_obj, dst_units_obj)){
		throw std::logic_error("conversion from \"" + src_units + "\" to \"" + dst_units + "\" is impossible");
		}
	
	return src_units_obj.factor/dst_units_obj.factor;
	}

inline double convert(double value, const std::string & src_units, const std::string & dst_units){
	// convert the value from one system of physical units to another.
	// i.e.: convert(1, "mol.m^-3/h", "M.s^-1"); 
	// raises LogicError if conversion is impossible (units mismatch), 
	// or if the units strings cannot be parsed.
	
	double factor = get_conversion_factor(src_units, dst_units);
	double converted_value = value * factor;
	return converted_value;
	}

#endif // PHYSICAL_UNITS_HPP
