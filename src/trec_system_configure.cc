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

#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>

#include "trec_defines.hh"
#include "trec_system_configure.hh"
#include "trec_ccmath.h"

#define LINEAR_FIT 2

#define N1   5
#define N2   7

namespace {

// calorimeter slice = 1.5 mm in G4_POLYSTYRENE
const struct EnergyPositionData {
	double energy; // MeV/u
	int position; // calorimeter slice + 1
} ps_epdata[N1] = {
	{ 180.0, 44 },
	{ 280.0, 96 },
	{ 380.0, 159 },
	{ 455.0, 213 },
	{ 480.0, 232 }
};

// calorimeter slice = 1.5 mm in G4_WATER
const struct EnergyPositionData water_epdata[N1] = {
	{ 180.0, 46 },
	{ 280.0, 100 },
	{ 380.0, 166 },
	{ 455.0, 223 },
	{ 480.0, 243 }
};

// (455 MeV/u) G4_POLYSTYRENE object and calorimeter
const struct ThicknessPositionData {
	double thickness; // cm
	int position; // calorimeter slice + 1
} ps_ps_tpdata[N2] = {
	{ 3.0, 193 },
	{ 6.0, 173 },
	{ 10.0, 147 },
	{ 15.0, 113 },
	{ 20.0, 80 },
	{ 25.0, 47 },
	{ 30.0, 13 }
};

// (455 MeV/u) G4_POLYSTYRENE object and G4_WATER calorimeter
const struct ThicknessPositionData ps_water_tpdata[N1] = {
	{ 6.0, 173 },
	{ 10.0, 153 },
	{ 20.0, 84 },
	{ 25.0, 49 },
	{ 30.0, 14 }
};

const double calo_x = TREC_SIZE_CALORIMETER * CLHEP::mm / 2.0;  // half size
const double calo_y = calo_x;  // half size
const double calo_z = TREC_SIZE_CALORIMETER_THICKNESS * CLHEP::mm / 2.0; // half size
const double calo_slice_z = TREC_SIZE_CALORIMETER_SLICE_THICKNESS * CLHEP::um / 2.0; // half size
const int calo_slices = static_cast<int>(calo_z / calo_slice_z);

const double sigma_xy1 = TREC_SIZE_SILICON_STRIP / sqrt(12.);

const double tension = 0.6;

} // namespace

namespace TREC {

SharedConf SystemConfigure::instance_;

SharedConf
SystemConfigure::instance() 
{
	return instance_;
}

SharedConf
SystemConfigure::instance(const char* filename)
{
	instance_ = SharedConf(new SystemConfigure(filename));

	return instance_;
}

SystemConfigure::SystemConfigure(const char*)
	:
	calorimeter_slices_(calo_slices),
	calorimeter_slice_size_(calo_slice_z * 2.0),
	clear_energy_(0), // clear bregg peak energy values  
	clear_position_(0), // clear bregg peak position - energy spline
	clear_spline_(0), // clear bregg peak position - energy spline
	clear_points_(0),
	material_position_(0), // object thickness - bregg peak position linear fit
	material_points_(0),
	energy_(455.0)
{
	calculate_position_2_pset();
}

SystemConfigure::~SystemConfigure()
{
	if (clear_energy_) delete [] clear_energy_;
	if (clear_position_) delete [] clear_position_;
	if (clear_spline_) delete [] clear_spline_;
	if (material_position_) delete [] material_position_;
}

void
SystemConfigure::calorimeter_sizes( double& size_x, double& size_y,
	double& size_z) const
{
	size_x = calo_z;
	size_y = calo_y;
	size_z = calo_z;
}

void
SystemConfigure::calorimeter_slice_sizes( double& size_x, double& size_y,
	double& size_z) const
{
	size_x = calo_z;
	size_y = calo_y;
	size_z = calo_slice_z;
}

void
SystemConfigure::calculate_calorimeter_wepl()
{
	// welp = (bp0 - bp) / d
	// Use ps_water_tpdata for pb, d and water_epdata for energy dependency pb0
}

double
SystemConfigure::PSET(int slice) const
{
	double slice_clear = ccm_splfit( energy_, clear_energy_,
		clear_position_, clear_spline_, clear_points_ - 1, tension);
//	double dx1 = (slice - material_position_[0]) / material_position_[1]; // cm

	double dx2 = (slice - slice_clear) /  material_position_[1]; // cm
	
	return dx2;
}

void
SystemConfigure::calculate_position_2_pset()
{
	// welp = (bp0 - bp) / d
	// Use ps_water_tpdata for pb, d and water_epdata for energy dependency pb0

	int i, j;
	clear_energy_ = new double[N1];
	clear_position_ = new double[N1];
	clear_spline_ = new double[N1];

	material_position_ = new double[N2];
	double *tmp = new double[LINEAR_FIT * N2];

	for ( i = 0; i < N1; ++i) {
		clear_energy_[i] = ps_epdata[i].energy;
		clear_position_[i] = ps_epdata[i].position;
	}

	clear_points_ = N1;
	ccm_cspl( clear_energy_, clear_position_, clear_spline_,
		clear_points_ - 1, tension);

/******
	for ( i = 440; i < 460; ++i) {
		double v = ccm_splfit( (double)i, x, y, f, n, tension);
		printf( "Energy: %d, position %d\n", i, (int)v);
	}
******/

	for ( i = 0; i < N2; ++i) {
		material_position_[i] = ps_ps_tpdata[i].position;
		for ( j = 0; j < LINEAR_FIT; ++j) {
			tmp[i * LINEAR_FIT + j] = pow( ps_ps_tpdata[i].thickness, j);
		}
	}

	material_points_ = N2;

//	matprt( tmp, N2, LINEAR_FIT, " %9.4f");

	std::cout << "Compute least squares coefficients via QR reduction." << std::endl;
	/* double t = */ ccm_qrlsq( tmp, material_position_, material_points_, LINEAR_FIT, &i);
//	printf( " ssq = %.3e\n", t);
	if (i == -1)
		std::cerr << "singular reduced matrix" << std::endl;
	else {
//		std::cout << "best fit: " << " Y = " << material_position_[0];
//		std::cout << " + " << material_position_[1] << " X" << std::endl;
		
//		matprt( tmp, N2, LINEAR_FIT, " %9.4f");

//		printf("cf-out:\n");
//		matprt( y_lft, 1, LINEAR_FIT, " %10.6f");
//		printf("best fit: Y = %g + %g X\n", y_lft[0], y_lft[1]);
	}

	delete [] tmp;
}

} // namespace TREC
