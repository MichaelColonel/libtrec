/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#pragma once

#include <map>
#include <vector>
#include <tr1/memory>
#include <boost/noncopyable.hpp>

namespace TREC {

class SystemConfigure;
typedef std::tr1::shared_ptr<SystemConfigure> SharedConf;

class SystemConfigure : private boost::noncopyable  {
public:
	virtual ~SystemConfigure();
	// Get object instance only
	static SharedConf instance();
	// Make & Get instance
	static SharedConf instance(const char* filename);
	int calorimeter_slices() const { return calorimeter_slices_; }
	void calorimeter_sizes( double& size_x, double& size_y,
		double& size_z) const;
	void calorimeter_slice_sizes( double& size_x, double& size_y,
		double& size_z) const;
	void strip_planes_sigma( double& sigma_xy1, 
		double& sigma_xy2, double& sigma_xy3) const;

	double PSET(int calorimeter_slice) const; // cm
	double WET(int calorimeter_slice) const; // cm
	double WEPL(int calorimeter_slice) const;

private:
	SystemConfigure(const char* filename);
	void calculate_position_2_wet();
	void calculate_position_2_pset();
	void calculate_position_2_wepl();
	void calculate_calorimeter_wepl();
	void calculate_wet_2_wepl();

	static SharedConf instance_;
	int calorimeter_slices_;
	double calorimeter_slice_size_;
	std::map< double, unsigned int> energy_slice_map_;
	std::map< double, unsigned int> thickness_slice_map_;

	double* clear_energy_; // clear bregg peak energy values  
	double* clear_position_; // clear bregg peak position - energy spline
	double* clear_spline_; // clear bregg peak position - energy spline
	int clear_points_;
	double* material_position_; // object thickness - bregg peak position spline
	int material_points_;
	double energy_;
};

} // namespace TREC
