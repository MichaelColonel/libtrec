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

#include <vector>
#include <map>
#include <fstream>

#include "trec_strip_geometry.hh"

namespace TREC {

typedef std::vector<bool> HitsVector;
typedef std::vector<unsigned int> NumbersVector;

typedef std::map< StripGeometryType, HitsVector > StripsHitsMap;
typedef std::map< StripGeometryType, NumbersVector > StripsNumbersMap;

class HitsPositions;
typedef std::vector<HitsPositions> HitsPositionsVector;

/** Class HitsPositions stores position of hits in silicon planes
 * and calorimeter
 *
 */
class HitsPositions {

friend std::ostream& operator<<( std::ostream& s, const HitsPositions& obj);
friend std::istream& operator>>( std::istream& s, HitsPositions& obj);

friend class TrackCoordinates;

public:
	/** Empty constructor
	 */
	HitsPositions();
	/** Constructor
	 * @param calorimeter_hits - hits vector in calorimeter slices
	 */
	HitsPositions(const HitsVector& calorimeter_hits);
	/** Copy constructor
	 */
	HitsPositions(const HitsPositions& src);
	virtual ~HitsPositions();
	/** Assignment operator
	 */
	HitsPositions& operator=(const HitsPositions& src);
	
	bool operator==(const HitsPositions& src) const;
	bool operator<(const HitsPositions& src) const;

	/** Add hits in particular plane
	 * @param type - particular silicon plane type
	 * @param hits - hits positions in the particular plane
	 */
	void add_plane_hits( StripGeometryType type, const HitsVector& hits);

	/** Add hits in calorimeter slices
	 * @param hits - hits vector in calorimeter slices
	 */	
	void add_calorimeter_hits(const HitsVector& hits);

	/** Check if calorimeter hits is empty or not
	 * @return true if hits is not empty, false otherwise
	 */
	bool calorimeter_empty() const;

	/** Return calorimeter hit position slice number
	 * @return hit position slice number if calorimter hits is not empty,
	 * or -1 otherwise
	 */
	int calorimeter_position() const;

	/** Save vector of HitsPositions into file
	 * @param filename - name of the file
	 * @param hits - vector of HitsPositions
	 */
	static void save( const char* filename, const HitsPositionsVector& hits);

	/** Load vector of HitsPositions from file
	 * @param filename - name of the file
	 * @param hits - vector of HitsPositions
	 */
	static void load( const char* filename, HitsPositionsVector& hits);

private:
	/** Transform plane hits position indexes to hits vector
	 * @param type - particular silicon plane type
	 * @return vector of hits in partiicular silicon plane
	 */
	HitsVector numbers_2_hits(StripGeometryType type) const;

	/** Transform hits vector to plane position indexes
	 * @param hits - hits vector
	 * @return vector of plane position indexes
	 */
	NumbersVector hits_2_numbers(const HitsVector& hits) const;

	StripsNumbersMap strips_numbers_;
	HitsVector calorimeter_hits_;
};

} // namespace TREC
