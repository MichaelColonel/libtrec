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

#include <gsl/gsl_fit.h>

#include "trec_ccmath.h"
#include "trec_track.hh"

namespace TREC {
	
Track
Track::create( double* z, double* f, double* w, int n)
{
	double c0, c1, cov00, cov01, cov11, chisq;
	gsl_fit_wlinear( z, 1, w, 1, f, 1, n, 
		&c0, &c1, &cov00, &cov01, &cov11, &chisq);
	
	return Track( c1, c0, cov00, cov01, cov11);
}

Track
Track::create( double* z, double* f, int n)
{
	int id;
	const int fit = 2;
	double* tmp = new double[n * fit];
	double* ff = new double[n];

	for ( int i = 0; i < n; ++i) {
		ff[i] = f[i]; // just copy

//		for ( int j = 0; j < fit; ++j)
//			tmp[i * fit + j] = pow( z[i], j);

		tmp[i * fit] = 1.0;
		tmp[i * fit + 1] = z[i];
	}

	/* double t = */ ccm_qrlsq( tmp, ff, n, fit, &id);

	Track res = (id == -1) ? Track( 0.0, 0.0) : Track( ff[1], ff[0]);

	delete [] tmp;
	delete [] ff;
	
	return res;
}

double
Track::fit(double z) const
{
	double v = 0.0, v_err = 0.0;

	if (track_with_errors_)
		gsl_fit_linear_est( z, b_, a_, cov00_, cov01_, cov11_, &v, &v_err);
	else
		v = a_ * z + b_;

	return v;
}

std::pair< double, double>
Track::fit_error(double z) const
{
	double v = 0.0, v_err = 0.0;

	if (track_with_errors_)
		gsl_fit_linear_est( z, b_, a_, cov00_, cov01_, cov11_, &v, &v_err);
	else
		v = a_ * z + b_;

	return std::make_pair( v, v_err);
}

std::ostream&
operator<<( std::ostream& s, const Track& obj)
{
	s.write( (char *)&obj.a_, sizeof(double));
	s.write( (char *)&obj.b_, sizeof(double));
	s.write( (char *)&obj.cov00_, sizeof(double));
	s.write( (char *)&obj.cov01_, sizeof(double));
	s.write( (char *)&obj.cov11_, sizeof(double));
	s.write( (char *)&obj.track_with_errors_, sizeof(bool));

	return s;
}

std::istream&
operator>>( std::istream& s, Track& obj)
{
	s.read( (char *)&obj.a_, sizeof(double));
	s.read( (char *)&obj.b_, sizeof(double));
	s.read( (char *)&obj.cov00_, sizeof(double));
	s.read( (char *)&obj.cov01_, sizeof(double));
	s.read( (char *)&obj.cov11_, sizeof(double));
	s.read( (char *)&obj.track_with_errors_, sizeof(bool));

	return s;
}

} // namespace TREC
