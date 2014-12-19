#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <istream>

#include <ocl_wrapper.h>
#include <utl_utils.h>

#include "profile.h"



int main( int argc, char** argv )
{
    utl::Args args( argc, argv );

    size_t const numArgs = args.size();

	if ( numArgs != 4 && numArgs != 5) {std::cerr << "Usage: " << args.at( 0 ) << " dimStart dimEnd dimStep <testing>" << std::endl; return EXIT_FAILURE;}

	utl::ProfilePassManager mgr;

	size_t const f  = args.toSizet( 1 );
	size_t const l  = args.toSizet( 2 );
	size_t const s  = args.toSizet( 3 );
	bool testing = numArgs == 5 ? args.toBool(4) : false;

	const utl::Dim first = utl::Dim(f,f,f);
	const utl::Dim last  = utl::Dim(l,l,l);
	const utl::Dim step  = utl::Dim(s,s,s);

	mgr << new StudXPass1<float,utl::row_major_tag,256u,1u>    ("11.profile/profile1.cl","matvec1_rmajor", first, step, last, testing, 10);
	mgr << new StudXPass1<float,utl::column_major_tag,256u,1u> ("11.profile/profile1.cl","matvec1_cmajor", first, step, last, testing, 10);

    mgr.run();
    mgr.write( std::cout );
    
    return EXIT_SUCCESS;
}
