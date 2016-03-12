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

/** Class TrackCoordinates calculate tracks parameters
 * of the main (XY1-XY2) and full (XY1-XY2-XY3) tracks 
 */
class TrackCoordinates {
public:
	/** Constructor
	 * @param hits_data - hits positions of the track
	 */
	TrackCoordinates(const HitsPositions& hits_data);
	/** Copy constructor
	 */	
	TrackCoordinates(const TrackCoordinates& src);
	/** Assign operator
	 */	
	TrackCoordinates& operator=(const TrackCoordinates& src);

	/** Calculates tracks coordinates for the main and full tracks
	 * 
	 * @param main - returns <tt>true</tt> if main track parameters
	 * are successfully found, otherwise returns <tt>false</tt>
	 * 
	 * @param full - returns <tt>true</tt> if full track parameters
	 * are successfully found, otherwise returns <tt>false</tt> 
	 */	
	void calculate_tracks( bool& main, bool& full);

	/** Get main and full tracks parameteres pairs
	 * 
	 * @param track_main - main track pair
	 * 
	 * @param track_full - full track pair
	 */	
	void get_tracks( TrackXYPair& track_main, TrackXYPair& track_full) const;

	/** Get main or full tracks parameteres pair
	 * 
	 * @param type - if <tt>true</tt> returns full track pair, otherwise
	 * returns main track pair
	 */	
	TrackXYPair get_track(bool type) const;

private:
	/** Calculate tracks coordinates using hits positions 
	 * 
	 * @param hits - hits positions of the track
	 */	
	void calculate_coordinates(const HitsPositions& hits);

	/** Check if it is a one single of multistrip cluster on a plane
	 * 
	 * @param si_plane_hits - hits data on particular plane
	 * @param begin - begin iterator of the cluster
	 * @param end - end iterator of the cluster
	 * if <tt>end</tt> = <tt>si_plane_hits.begin()</tt> it is a single strip cluster
	 * 
	 * @return 0 if it is a one single of multistrip cluster, -1 otherwise
	 */	
	int check_one_cluster( const HitsVector& si_plane_hits,
		HitsVector::const_iterator& begin,
		HitsVector::const_iterator& end) const;
	
	/** Check if the full track within a trajectory
	 */
	bool check_tracks_within_trajectory() const;

	/** Get multistrip cluster sigma
	 * 
	 * @param type - particular plane type
	 * @param plane_hits - hits data on particular plane
	 * @param begin - begin iterator of the cluster
	 * @param end - end iterator of the cluster
	 * 
	 * @return sigma value
	 */	
	double multistrip_cluster_sigma( StripGeometryType type,
		const HitsVector& plane_hits,
		HitsVector::const_iterator& begin,
		HitsVector::const_iterator& end);

	/** Get multistrip cluster center/mean coordinate
	 * 
	 * @param type - particular plane type
	 * @param plane_hits - hits data on particular plane
	 * @param begin - begin iterator of the cluster
	 * @param end - end iterator of the cluster
	 * 
	 * @return sigma value
	 */	
	double multistrip_cluster_coordinate( StripGeometryType type,
		const HitsVector& plane_hits,
		HitsVector::const_iterator& begin,
		HitsVector::const_iterator& end);

	/** Find coordinate of the track on a particular plane
	 * 
	 * @param type - particular plane type
	 * @param plane_hits - hits data on particular plane
	 * 
	 * @return 0 if coordinates are found, 1 if there are no hits on the plane
	 * -1 in sace of an error
	 */
	int find_coordinate( StripGeometryType, const HitsVector& si_plane_hits);

	/** Calculate main track coordinates
	 * @param type - <tt>true</tt> of X0Z axis, <tt>false</tt> of Y0Z axis, 
	 */
	void calculate_main_track(bool);

	/** Calculate full track coordinates
	 * @param type - <tt>true</tt> of X0Z axis, <tt>false</tt> of Y0Z axis, 
	 */	
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
