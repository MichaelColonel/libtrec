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
#include "trec_track.hh"

class TH1I;
class TH1D;
class TH2D;

namespace TREC {

class TracksReconstruction {

public:
	// main and full tracks with object
	TracksReconstruction( const MainTracksVector& main_tracks_object,
		const FullTracksVector& full_tracks_object,
		int size_x1 = TREC_SIZE_X_MIN, int size_x2 = TREC_SIZE_X_MAX,
		int size_y1 = TREC_SIZE_Y_MIN, int size_y2 = TREC_SIZE_Y_MAX,
		int bin_x = TREC_BINS_X, int bin_y = TREC_BINS_Y);

	virtual ~TracksReconstruction();

	void reconstruct( int object_slice_min, int object_slice_max,
		int clear_slice_max);

	void save(const char* filename = "reconstruct.root") const;

private:
	void form_clear_tracks_data(const FullTracksVector& clear_tracks);
	void form_object_tracks_data(const FullTracksVector& object_tracks);

	const MainTracksVector& tracks_main_;
	const FullTracksVector& tracks_full_;
	TH1I* clear_slice_;
	TH1I* object_slice_;
	TH2D* clear_position_;
	TH2D* object_position_;
	TH1D* object_position_profile_;
	TH2D* clear_fluence_;
	TH2D* object_fluence_;
	TH1D* object_fluence_profile_;
	TH2D* clear_weight_;
	TH2D* object_weight_;
	int clear_pos_min_;
	int clear_pos_max_;
	int object_pos_min_;
	int object_pos_max_;
	int size_x1;
	int size_x2;
	int size_y1;
	int size_y2;
	int bin_x;
	int bin_y;
};

inline
TracksReconstruction::TracksReconstruction( const MainTracksVector& main,
	const FullTracksVector& full,
	int pos_x1, int pos_x2, int pos_y1,
		int pos_y2, int bins_x, int bins_y)
	:
	tracks_main_(main),
	tracks_full_(full),
	clear_slice_(0),
	object_slice_(0),
	clear_position_(0),
	object_position_(0),
	object_position_profile_(0),
	clear_fluence_(0),
	object_fluence_profile_(0),
	clear_weight_(0),
	object_weight_(0),
	clear_pos_min_(-1),
	clear_pos_max_(-1),
	object_pos_min_(-1),
	object_pos_max_(-1),
	size_x1(pos_x1),
	size_x2(pos_x2),
	size_y1(pos_y1),
	size_y2(pos_y2),
	bin_x(bins_x),
	bin_y(bins_y)
{
}

} // namespace CarbonIonRadiography
