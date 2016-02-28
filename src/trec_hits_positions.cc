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

#include <fstream>
#include <algorithm>
#include <numeric>

#include "trec_defines.hh"
#include "trec_constants.hh"
#include "trec_strip_geometry.hh"
#include "trec_hits_positions.hh"

namespace TREC {

HitsPositions::HitsPositions()
{
}

HitsPositions::HitsPositions(const HitsVector& calorimeter_hits)
	:
	calorimeter_hits_(calorimeter_hits)
{
}

HitsPositions::HitsPositions(const HitsPositions& src)
	:
	strips_numbers_(src.strips_numbers_),
	calorimeter_hits_(src.calorimeter_hits_)
{
}

HitsPositions::~HitsPositions()
{
}

HitsPositions&
HitsPositions::operator=(const HitsPositions& src)
{
	this->strips_numbers_ = src.strips_numbers_;
	this->calorimeter_hits_ = src.calorimeter_hits_;
	
	return *this;
}

bool
HitsPositions::operator==(const HitsPositions& src) const
{
	bool strips = (strips_numbers_ == src.strips_numbers_);
	bool calo = (calorimeter_hits_ == src.calorimeter_hits_);
	return (strips && calo);
}

bool
HitsPositions::operator<(const HitsPositions& src) const
{
	int this_calo = calorimeter_position();
	int src_calo = src.calorimeter_position();

	return (this_calo < src_calo);
}

void
HitsPositions::add_plane_hits( StripGeometryType type,
	const HitsVector& hits)
{
	NumbersVector num = hits_2_numbers(hits);
	strips_numbers_[type] = num;
}

void
HitsPositions::add_calorimeter_hits(const HitsVector& hits)
{
	calorimeter_hits_ = hits;
}

HitsVector
HitsPositions::numbers_2_hits(StripGeometryType type) const
{
	HitsVector hits;

	const NumbersVector& num = strips_numbers_.at(type);
	if (num.size()) {
		const StripGeometry* geom = StripGeometry::get(type);
		hits.resize( geom->strips, false);
		for ( NumbersVector::const_iterator it = num.begin();
			it != num.end(); ++it) {
			hits[*it] = true;
		}
	}
	return hits;
}

NumbersVector
HitsPositions::hits_2_numbers(const HitsVector& hits) const
{
	NumbersVector num;
	unsigned int values = std::accumulate( hits.begin(), hits.end(), 0);
	if (values) {
		num.reserve(values);
		for ( HitsVector::const_iterator it = hits.begin();
			it != hits.end(); ++it) {
			unsigned int pos = std::distance( hits.begin(), it);
			if (*it) num.push_back(pos);
		}
	}
	return num;
}

bool
HitsPositions::calorimeter_empty() const
{
	const HitsVector& calo = calorimeter_hits_;
 
	if (calo.size() == 0)
		return true;

	unsigned int values = std::accumulate( calo.begin(), calo.end(), 0);

	return (values == 0);
}

int
HitsPositions::calorimeter_position() const
{
	int pos = -1;
	if (!calorimeter_empty()) {
		const HitsVector& calo = calorimeter_hits_;
		HitsVector border( border_down, border_down + border_size);
		HitsVector::const_iterator it = std::find_end( calo.begin(),
			calo.end(), border.begin(), border.end());
		pos = (it != calo.end()) ? std::distance( calo.begin(), it) : -1;
	}
	return pos;
}

void
HitsPositions::save( const char* filename, const HitsPositionsVector& hits)
{
	// dump data
	std::ofstream dump(filename);

	size_t hits_size = hits.size();

	dump.write( (char *)&hits_size, sizeof(size_t));

	for ( HitsPositionsVector::const_iterator iter = hits.begin();
		iter != hits.end(); ++iter)
	{
		dump << *iter;
	}
	dump.close();
}

void
HitsPositions::load( const char* filename, HitsPositionsVector& hits)
{
	// dump data
	std::ifstream dump(filename);

	size_t hits_size = 0;

	dump.read( (char *)&hits_size, sizeof(size_t));
	hits.resize(hits_size);

	for ( HitsPositionsVector::iterator iter = hits.begin();
		iter != hits.end(); ++iter)
	{
		dump >> *iter;
	}
	dump.close();
}

std::ostream&
operator<<( std::ostream& s, const HitsPositions& obj)
{
	const StripsNumbersMap& hitmap = obj.strips_numbers_;

	// save map size
	size_t strips_size = hitmap.size();
	s.write( (char *)&strips_size, sizeof(size_t));

	for ( StripsNumbersMap::const_iterator iter = hitmap.begin();
		iter != hitmap.end(); ++iter) {

		// save plane index
		unsigned int pos = StripGeometry::index(iter->first);
		s.write( (char *)&pos, sizeof(unsigned int));

		// save plane hits positions size
		const NumbersVector& num = iter->second;
		size_t numsize = num.size();
		s.write( (char *)&numsize, sizeof(size_t));

		// save plane hits positions values
		for ( NumbersVector::const_iterator it = num.begin();
			it != num.end(); ++it) {
			NumbersVector::value_type val = *it;
			s.write( (char *)&val, sizeof(NumbersVector::value_type));
		}
	}

	const HitsVector& calo = obj.calorimeter_hits_;

	// save calorimeter size
	size_t calorimeter_size = calo.size();
	s.write( (char *)&calorimeter_size, sizeof(size_t));

	// save calorimeter hits values
	for ( HitsVector::const_iterator it = calo.begin();
		it != calo.end(); ++it) {
			HitsVector::value_type val = *it;
			s.write( (char *)&val, sizeof(HitsVector::value_type));
	}

	return s;
}

std::istream&
operator>>( std::istream& s, HitsPositions& obj)
{
	StripsNumbersMap& hitmap = obj.strips_numbers_;

	// load map size
	size_t strips_size;
	s.read( (char *)&strips_size, sizeof(size_t));

	for ( size_t i = 0; i < strips_size; ++i) {
		// load plane index
		unsigned int ind;
		s.read( (char *)&ind, sizeof(unsigned int));
		StripGeometryType type = StripGeometry::index(ind);
		
		// load plane hits positions size
		size_t numsize;
		s.read( (char *)&numsize, sizeof(size_t));

		NumbersVector num;
		if (numsize > 0) {
			num.resize(numsize);
			for ( size_t j = 0; j < numsize; ++j) {
				// load plane hit position value
				NumbersVector::value_type val = -1;
				s.read( (char *)&val, sizeof(NumbersVector::value_type));
				num[j] = val;
			}
		}
		hitmap[type] = num;
	}

	// load calorimeter size
	size_t calosize = 0;
	s.read( (char *)&calosize, sizeof(size_t));

	if (calosize > 0) {
		HitsVector& calo = obj.calorimeter_hits_;
		calo.resize(calosize);
		
		for ( size_t i = 0; i < calosize; ++i) {
			// load calorimeter hit value
			HitsVector::value_type val = 0;
			s.read( (char *)&val, sizeof(HitsVector::value_type));
			calo[i] = val;
		}
	}

	return s;
}

} // namespace TREC
