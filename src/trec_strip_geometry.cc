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

const std::string Silicon("Silicon");
const std::string Log("Log");
const std::string Phys("Phys");
const std::string Parallel("Parallel");
const std::string Division("Division");
const std::string Sensitive("Sensitive");
const std::string Detector("Detector");

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

StripGeometryMap
StripGeometry::create()
{
	StripGeometryMap map;
	
	map[MSD_Y1] = StripGeometryPair( strip_geometry_[0], create(MSD_Y1));
	map[MSD_X1] = StripGeometryPair( strip_geometry_[1], create(MSD_X1));
	map[MSD_Y2] = StripGeometryPair( strip_geometry_[2], create(MSD_Y2));
	map[MSD_X2] = StripGeometryPair( strip_geometry_[3], create(MSD_X2));
	map[MSD_Y3] = StripGeometryPair( strip_geometry_[4], create(MSD_Y3));
	map[MSD_X3] = StripGeometryPair( strip_geometry_[5], create(MSD_X3));
	map[MSD__U] = StripGeometryPair( strip_geometry_[6], create(MSD__U));
	map[MSD__V] = StripGeometryPair( strip_geometry_[7], create(MSD__V));

	return map;
}

StripNamesMap
StripGeometry::create_names()
{
	StripNamesMap map;

	map[MSD_Y1] = create(MSD_Y1);
	map[MSD_X1] = create(MSD_X1);
	map[MSD_Y2] = create(MSD_Y2);
	map[MSD_X2] = create(MSD_X2);
	map[MSD_Y3] = create(MSD_Y3);
	map[MSD_X3] = create(MSD_X3);
	map[MSD__U] = create(MSD__U);
	map[MSD__V] = create(MSD__V);

	return map;
}

StripGeometryNames
StripGeometry::create(StripGeometryType type)
{
	StripGeometryNames names;
	std::string value;
	switch (type) {
	case MSD_Y1:
		value = std::string("Y1");
		break;
	case MSD_X1:
		value = std::string("X1");
		break;
	case MSD_Y2:
		value = std::string("Y2");
		break;
	case MSD_X2:
		value = std::string("X2");
		break;
	case MSD_Y3:
		value = std::string("Y3");
		break;
	case MSD_X3:
		value = std::string("X3");
		break;
	case MSD__U:
		value = std::string("U");
		break;
	case MSD__V:
		value = std::string("V");
		break;
	case MSD_ER: // error - no value
	default:
		break;
	}
	
	if (!value.empty()) {
		names.body_name = Silicon + value;
		names.logical_name = Silicon + value + Log;
		names.physical_name = Silicon + value + Phys;
		names.parallel_body_name = Silicon + value + Parallel;
		names.parallel_logical_name = Silicon + value + Log + Parallel;
		names.parallel_physical_name = Silicon + value + Phys + Parallel;
		names.body_devision_name = Silicon + value + Division + Parallel;
		names.logical_devision_name = Silicon + value + Log + Division + Parallel;
		names.physical_devision_name = Silicon + value + Phys + Division + Parallel;
		names.functional_detector_name = Detector + value;
		names.sensitive_detector_name = Sensitive + Detector + value;
	}
	return names;
}

} // namespace TREC
