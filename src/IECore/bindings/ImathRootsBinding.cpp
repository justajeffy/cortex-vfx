//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008-2009, Image Engine Design Inc. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//     * Neither the name of Image Engine Design nor the names of any
//       other contributors to this software may be used to endorse or
//       promote products derived from this software without specific prior
//       written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////

// This include needs to be the very first to prevent problems with warnings
// regarding redefinition of _POSIX_C_SOURCE
#include "boost/python.hpp"

#include "OpenEXR/ImathRoots.h"

#include "IECore/bindings/ImathRootsBinding.h"

using namespace boost::python;
using namespace Imath;

namespace IECore
{

tuple solveLinearWrapper( double a, double b )
{
	double x;
	int s = solveLinear( a, b, x );
	switch( s )
	{
		case 0 :
			return tuple();
		case 1 :
			return make_tuple( x );
		default :
			PyErr_SetString( PyExc_ArithmeticError, "Infinite solutions." );
			throw_error_already_set();
			return tuple(); // should never get here
	}
}

tuple solveQuadraticWrapper( double a, double b, double c )
{
	double x[2];
	int s = solveQuadratic( a, b, c, x );
	switch( s )
	{
		case 0 :
			return tuple();
		case 1 :
			return make_tuple( x[0] );
		case 2 :
			return make_tuple( x[0], x[1] );
		default :
			PyErr_SetString( PyExc_ArithmeticError, "Infinite solutions." );
			throw_error_already_set();
			return tuple(); // should never get here
	}
}

tuple solveCubicWrapper( double a, double b, double c, double d )
{
	double x[3];
	int s = solveCubic( a, b, c, d, x );
	switch( s )
	{
		case 0 :
			return tuple();
		case 1 :
			return make_tuple( x[0] );
		case 2 :
			return make_tuple( x[0], x[1] );
		case 3 :
			return make_tuple( x[0], x[1], x[2] );
		default :
			PyErr_SetString( PyExc_ArithmeticError, "Infinite solutions." );
			throw_error_already_set();
			return tuple(); // should never get here
	}
}

void bindImathRoots()
{
	def( "solveLinear", &solveLinearWrapper );
	def( "solveQuadratic", &solveQuadraticWrapper );
	def( "solveCubic", &solveCubicWrapper );
}

}