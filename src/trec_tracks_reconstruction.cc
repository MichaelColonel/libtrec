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

#include <iostream>
#include <algorithm>

#include <TH1.h>
#include <TH2.h>
#include <TFile.h>

#include "trec_defines.hh"
#include "trec_system_configure.hh"
#include "trec_strip_geometry.hh"
#include "trec_tracks_reconstruction.hh"

namespace TREC {

TracksReconstruction::~TracksReconstruction()
{
	if (clear_slice_) delete clear_slice_;
	if (clear_fluence_) delete clear_fluence_;
	if (clear_position_) delete clear_position_;
	if (clear_weight_) delete clear_weight_;

	if (object_slice_) delete object_slice_;
	if (object_fluence_) delete object_fluence_;
	if (object_position_) delete object_position_;
	if (object_weight_) delete object_weight_;
}

void
TracksReconstruction::form_clear_tracks_data(const FullTracksVector& clear_tracks)
{
	SharedConf conf = SystemConfigure::instance();

	int calo_slices = conf->calorimeter_slices();

	clear_slice_ = new TH1I( "slice_clear", "Slice",
		calo_slices, 0, calo_slices - 1);

	for ( size_t i = 0; i < clear_tracks.size(); ++i) {
		const int& position = clear_tracks[i].second;
		clear_slice_->Fill(position);
	}

	std::vector<double> slices(calo_slices);
	for ( int i = 1; i <= calo_slices; ++i)
		slices[i - 1] = clear_slice_->GetBinContent(i);

	std::vector<double>::iterator iter = std::max_element( slices.begin(),
		slices.end());
	
	clear_pos_max_ = std::distance( slices.begin(), iter) + 1;

	clear_position_ = new TH2D( "position_clear", "Position",
		bin_x, size_x1, size_x2, bin_y, size_y1, size_y2);

	clear_fluence_ = new TH2D( "fluence_clear", "Fluence",
		bin_x, size_x1, size_x2, bin_y, size_y1, size_y2);

	clear_weight_ = new TH2D( "weight_clear", "Weight",
		bin_x, size_x1, size_x2, bin_y, size_y1, size_y2);

	const StripGeometry* plane_y2 = StripGeometry::get(MSD_Y2);
	const StripGeometry* plane_x2 = StripGeometry::get(MSD_X2);
	const StripGeometry* plane_y3 = StripGeometry::get(MSD_Y3);
	const StripGeometry* plane_x3 = StripGeometry::get(MSD_X3);

	for ( size_t i = 0; i < clear_tracks.size(); ++i) {
		const Track& full_x = clear_tracks[i].first.first;
		const Track& full_y = clear_tracks[i].first.second;
		const int& position = clear_tracks[i].second;
	
		double w = clear_slice_->GetBinContent(position + 1) / clear_tracks.size();

		double full_x_z = (plane_x2->z + plane_x3->z) / 2.0;
		double full_y_z = (plane_y2->z + plane_y3->z) / 2.0;

		// full track (xy1-xy2-xy3) coordinates
		double fx = full_x.fit(full_x_z);
		double fy = full_y.fit(full_y_z);

		clear_position_->Fill( fx, fy, position);
		clear_fluence_->Fill( fx, fy);
		clear_weight_->Fill( fx, fy, w);
	}
}

void
TracksReconstruction::form_object_tracks_data(
	const FullTracksVector& full_tracks_clear)
{
	SharedConf conf = SystemConfigure::instance();

	int calo_slices = conf->calorimeter_slices();

	form_clear_tracks_data(full_tracks_clear);

	object_slice_ = new TH1I( "slice_object", "Slice",
		calo_slices, 0, calo_slices - 1);

	for ( size_t i = 0; i < tracks_full_.size(); ++i) {
		const int& position = tracks_full_[i].second;
		object_slice_->Fill(position);
	}

	object_fluence_ = new TH2D( "fluence_object", "Fluence",
		bin_x, size_x1, size_x2, bin_y, size_y1, size_y2);


	object_position_ = new TH2D( "position_object", "Position",
		bin_x, size_x1, size_x2, bin_y, size_y1, size_y2);


	object_weight_ = new TH2D( "weight_object", "Weight",
		bin_x, size_x1, size_x2, bin_y, size_y1, size_y2);

	const StripGeometry* plane_y2 = StripGeometry::get(MSD_Y2);
	const StripGeometry* plane_x2 = StripGeometry::get(MSD_X2);
	const StripGeometry* plane_y3 = StripGeometry::get(MSD_Y3);
	const StripGeometry* plane_x3 = StripGeometry::get(MSD_X3);

	for ( size_t i = 0; i < tracks_full_.size(); ++i) {
		const Track& full_x = tracks_full_[i].first.first;
		const Track& full_y = tracks_full_[i].first.second;
		const int& position = tracks_full_[i].second;

		if (position > clear_pos_max_ || position < object_pos_min_)
			continue;

		double full_x_z = (plane_x2->z + plane_x3->z) / 2.0;
		double full_y_z = (plane_y2->z + plane_y3->z) / 2.0;

		double w = object_slice_->GetBinContent(position + 1) / tracks_full_.size();

		// full track (xy1-xy2-xy3) coordinates
		double fx = full_x.fit(full_x_z);
		double fy = full_y.fit(full_y_z);

//		int pos = clear_pos_max_ - position;
		double pos = conf->PSET(position);
		object_position_->Fill( fx, fy, pos);
		object_fluence_->Fill( fx, fy);
		object_weight_->Fill( fx, fy, w);
	}

}

void
TracksReconstruction::reconstruct( int object_slice_min,
	int object_slice_max, int clear_slice_max)
{
	int tmp_clr_max = clear_pos_max_; // save
	int tmp_obj_max = object_pos_max_; // save
	int tmp_obj_min = object_pos_min_; // save

	object_pos_min_ = object_slice_min;
	object_pos_max_ = object_slice_max;
	clear_pos_max_ = clear_slice_max;

	form_object_tracks_data(tracks_full_);

	double p, pe, f;
	for ( int i = 1; i <= object_fluence_->GetNbinsX(); ++i) {
		for ( int j = 1; j <= object_fluence_->GetNbinsY(); ++j) {
			p = object_position_->GetBinContent( i, j);
			pe = object_position_->GetBinError( i, j);
			f = object_fluence_->GetBinContent( i, j);

			object_position_->SetBinContent( i, j, p / f);
			object_position_->SetBinError( i, j, pe / f);
		}
	}

	object_pos_min_ = tmp_obj_min; // restore
	object_pos_max_ = tmp_obj_max; // restore
	clear_pos_max_ = tmp_clr_max; // restore
}

void
TracksReconstruction::save(const char* filename) const
{
	TFile* file = new TFile( filename, "RECREATE");

	object_position_->Write();
	object_fluence_->Write();
	object_slice_->Write();
	object_weight_->Write();
	file->Close();

	delete file;
}

} // namespace TREC
