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
#include <utility>

namespace TREC {

class Track;
typedef std::pair< Track, Track > TrackXYPair;
typedef std::pair< TrackXYPair, double > TracksEnergyPair;
typedef std::pair< TrackXYPair, int > TracksPositionPair;
typedef std::vector<TrackXYPair> MainTracksVector;
typedef std::vector<TracksPositionPair> FullTracksVector;

class Track {
	
friend std::ostream& operator<<( std::ostream&, const Track&);
friend std::istream& operator>>( std::istream&, Track&);

public:
	Track( double aa = 0.0, double bb = 0.0, double cov00 = 0.0,
		double cov01 = 0.0, double cov11 = 0.0); // GSL
	Track(const Track& src);
	virtual ~Track();
	Track& operator=(const Track& src);
	bool operator==(const Track& src) const;
	bool operator<(const Track& src) const;

	double a() const { return a_; }
	double b() const { return b_; }
	double fit(double z) const; // GSL
	bool empty() const;
	std::pair< double, double> fit_error(double z) const; // GSL

	static Track create( double* z, double* f, int n);
	static Track create( double* z, double* f, double* w, int n);
private:
	double a_;
	double b_;
	double cov00_; // parameter 1
	double cov01_; // parameter 2
	double cov11_; // parameter 3
};

inline
Track::Track( double aa, double bb, double cov00,
	double cov01, double cov11)
	:
	a_(aa),
	b_(bb),
	cov00_(cov00),
	cov01_(cov01),
	cov11_(cov11)
{
}

inline
Track::Track(const Track& src)
	:
	a_(src.a_),
	b_(src.b_),
	cov00_(src.cov00_),
	cov01_(src.cov01_),
	cov11_(src.cov11_)
{
} 

inline
Track::~Track()
{
}

inline
Track&
Track::operator=(const Track& src)
{
	a_ = src.a_;
	b_ = src.b_;
	cov00_ = src.cov00_;
	cov01_ = src.cov01_;
	cov11_ = src.cov11_;
	return *this;
}

inline
bool
Track::operator==(const Track& src) const
{
	bool a1 = (a_ == src.a_);
	bool a2 = (b_ == src.b_);
	bool a3 = (cov00_ == src.cov00_);
	bool a4 = (cov01_ == src.cov01_);
	bool a5 = (cov11_ == src.cov11_);
	bool b1 = (a1 && a2 && a3);
	bool b2 = (a4 && a5);
	return (b1 && b2);
}

inline
bool
Track::operator<(const Track& src) const
{
	bool a1 = (a_ < src.a_);
	bool a2 = (b_ < src.b_);

	return (a1 && a2);
}

inline
bool
Track::empty() const
{
	bool a1 = (a_ == 0.0);
	bool a2 = (b_ == 0.0);
	bool a3 = (cov00_ == 0.0);
	bool a4 = (cov01_ == 0.0);
	bool a5 = (cov11_ == 0.0);
	bool b1 = (a1 && a2 && a3);
	bool b2 = (a4 && a5);
	return (b1 && b2);
}

} // namespace TREC
