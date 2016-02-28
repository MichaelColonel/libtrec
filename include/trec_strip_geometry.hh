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

#include "trec_defines.hh"

namespace TREC {

// Micro strips detector plane type
enum StripGeometryType {
	MSD_ER, // error - no plane
	MSD_Y1,
	MSD_X1,
	MSD_Y2,
	MSD_X2,
	MSD_Y3,
	MSD_X3,
	MSD__U,
	MSD__V
};

// Micro strips detector plane geometry parameters
const struct StripGeometry {
	static int index(StripGeometryType); // get plane index from type
	static StripGeometryType index(int); // get plane type from index
	static const StripGeometry* get(StripGeometryType);
	static void load(const char* filename);

	double z; // position z (um)
	double angle; // angle of the detector (degree)
	double offset; // pitch offset (um) -- reserved
	double angle_diff; // angle corrections (degree) -- reserved
	double phi_diff; // reserved
	double sigma; // sigma multiple scattering + alingment (um) -- reserved
	double t; // detector thickness (um)
	double x; // half size of square detector (um)
	int strips; // number of strips
	double pitch; // pitch size (um)
	double dx; // detector offset (um) -- reserved
} strip_geometry_[TREC_NUMBER_OF_SILICON_DETECTORS] = {
	{  -52000., 180.0, 0.0, 0.0, 0.0, 0.0, 300., 30000., 300, 200., 0. },
	{  -50000.,  90.0, 0.0, 0.0, 0.0, 0.0, 300., 30000., 300, 200., 0. },
	{  298000., 180.0, 0.0, 0.0, 0.0, 0.0, 300., 30000., 300, 200., 0. },
	{  300000.,  90.0, 0.0, 0.0, 0.0, 0.0, 300., 30000., 300, 200., 0. },
	{ 1300000., 180.0, 0.0, 0.0, 0.0, 0.0, 300., 30000., 300, 200., 0. },
	{ 1302000.,  90.0, 0.0, 0.0, 0.0, 0.0, 300., 30000., 300, 200., 0. },
	{ 1320000., -10.5, 0.0, 0.0, 0.0, 0.0, 300., 30000., 300, 200., 0. },
	{ 1322000.,  10.5, 0.0, 0.0, 0.0, 0.0, 300., 30000., 300, 200., 0. }
};

inline
int
StripGeometry::index(StripGeometryType type)
{
	int pos = -1;

	switch (type) {
	case MSD_Y1:
		pos = 0;
		break;
	case MSD_X1:
		pos = 1;
		break;
	case MSD_Y2:
		pos = 2;
		break;
	case MSD_X2:
		pos = 3;
		break;
	case MSD_Y3:
		pos = 4;
		break;
	case MSD_X3:
		pos = 5;
		break;
	case MSD__U:
		pos = 6;
		break;
	case MSD__V:
		pos = 7;
		break;
	case MSD_ER:
	default:
		break;
	}
	return pos;
}

inline
StripGeometryType
StripGeometry::index(int pos)
{
	StripGeometryType type = MSD_ER;

	switch (pos) {
	case 0:
		type = MSD_Y1;
		break;
	case 1:
		type = MSD_X1;
		break;
	case 2:
		type = MSD_Y2;
		break;
	case 3:
		type = MSD_X2;
		break;
	case 4:
		type = MSD_Y3;
		break;
	case 5:
		type = MSD_X3;
		break;
	case 6:
		type = MSD__U;
		break;
	case 7:
		type = MSD__V;
		break;
	default:
		break;
	}
	return type;
}

} // namespace TREC
