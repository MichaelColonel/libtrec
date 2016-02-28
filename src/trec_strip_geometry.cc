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

#include "trec_strip_geometry.hh"

namespace {

} // namespace

namespace TREC {

const StripGeometry*
StripGeometry::get(StripGeometryType type)
{
	const StripGeometry* geom = 0;

	switch (type) {
	case MSD_Y1:
		geom = &strip_geometry_[0];
		break;
	case MSD_X1:
		geom = &strip_geometry_[1];
		break;
	case MSD_Y2:
		geom = &strip_geometry_[2];
		break;
	case MSD_X2:
		geom = &strip_geometry_[3];
		break;
	case MSD_Y3:
		geom = &strip_geometry_[4];
		break;
	case MSD_X3:
		geom = &strip_geometry_[5];
		break;
	case MSD__U:
		geom = &strip_geometry_[6];
		break;
	case MSD__V:
		geom = &strip_geometry_[7];
		break;
	case MSD_ER: // error - no value
	default:
		break;
	}

	return geom;
}

} // namespace TREC
