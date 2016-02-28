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

#include "trec_track.hh"
#include "trec_strip_geometry.hh"
#include "trec_hits_positions.hh"
#include "trec_defines.hh"

namespace TREC {

class TrackCoordinates {
public:
	TrackCoordinates(const HitsPositions& hits_data);
	TrackCoordinates(const TrackCoordinates& src);
	TrackCoordinates& operator=(const TrackCoordinates& src);

	void calculate_tracks( bool& main, bool& full);
	void get_tracks( TrackXYPair& track_main, TrackXYPair& track_full) const;
	TrackXYPair get_track(bool type) const;

private:
	void calculate_coordinates(const HitsPositions& hits);
	int check_one_cluster( const HitsVector& si_plane_hits,
		HitsVector::const_iterator& begin,
		HitsVector::const_iterator& end) const;
	bool check_tracks_within_trajectory() const;
	double multistrip_cluster_sigma( StripGeometryType,
		HitsVector::const_iterator& begin,
		HitsVector::const_iterator& end);
	double multistrip_cluster_coordinate( StripGeometryType,
		HitsVector::const_iterator& begin,
		HitsVector::const_iterator& end);

	int find_coordinate( StripGeometryType, const HitsVector& si_plane_hits);
	void calculate_main_track(bool);
	void calculate_full_track(bool);

	std::pair< double, double> xy1_; // coordinate
	std::pair< bool, bool> xy1_ok_; // state
	std::pair< double, double> xy2_; // coordinate
	std::pair< bool, bool> xy2_ok_; // state
	std::pair< double, double> xy3_; // coordinate
	std::pair< bool, bool> xy3_ok_; // state

	TrackXYPair main_track_;
	TrackXYPair full_track_;
};

inline
void
TrackCoordinates::get_tracks( TrackXYPair& track_main,
	TrackXYPair& track_full) const
{
	track_main = main_track_;
	track_full = full_track_;
}

inline
TrackXYPair
TrackCoordinates::get_track(bool type) const
{
	return type ? full_track_ : main_track_;
}

} // namespace TREC
