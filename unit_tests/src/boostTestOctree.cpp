// This file is a part of the 'octree.didactic' project
//
// author : uri.heap  <u.heep@tutanota.com>
//
// license : GPL.v3+
//
// test suite: testOctree

#include <boost/test/unit_test.hpp>

#include "octree.hpp"

#include <cstddef>
#include <random>


BOOST_AUTO_TEST_SUITE( testOctree )

BOOST_AUTO_TEST_CASE( testOctree0 )
{
  const d7cA::Octree<d7cA::Point, double>  octree;

  constexpr unsigned short  numElementsMustBe  =  0;
  const unsigned short&  numElements  =  octree.getNumElements();

  BOOST_CHECK_EQUAL( numElements, numElementsMustBe );

  const d7cA::Point<double>  point1( 1, 1, 1, 1 );
  std::size_t  numOperations1  =  0;
  const d7cA::OctreeObj<d7cA::Point, double>  * const  pFound1  =  octree.find( point1, numOperations1 );

  BOOST_CHECK_EQUAL( nullptr, pFound1 );

  const d7cA::Point<double>  point2;
  std::size_t  numOperations2  =  0;
  const d7cA::OctreeObj<d7cA::Point, double>  * const  pFound2  =  octree.find( point2, numOperations2 );

  BOOST_CHECK_EQUAL( nullptr, pFound2 );
}

BOOST_AUTO_TEST_CASE( testOctree1 )
{
  std::random_device  rd;
  std::mt19937        gen( rd() );
  // distribution for individual spatial coordinates:
  std::uniform_real_distribution<>  dist( -50, +50 );
  // distribution for numbers of points that will have
  // the same values of certain coordinates:
  std::uniform_int_distribution<>   distInt( 1, 10 );
  // choose what coordinates will have the same values:
  // 0 -> x1
  // 1 -> x2
  // 2 -> x3
  // 3 -> x4
  // 4 -> x1 & x2
  // 5 -> x1 & x3
  // 6 -> x1 & x4
  // 7 -> x2 & x3
  // 8 -> x2 & x4
  // 9 -> x3 & x4
  // 10 -> x1 & x2 & x3
  // 11 -> x2 & x3 & x4
  // 12 -> x3 & x4 & x1
  // 13 -> x4 & x1 & x2
  // 14 -> all coords values are different
  std::uniform_int_distribution<>   distCoord( 0, 14 );
  constexpr std::size_t  numPoints  =  10000;
  d7cA::Point<double>  * arrPoints  =  new d7cA::Point<double> [ numPoints ];

  for ( std::size_t i = 0; i < numPoints; )
  {
    const std::size_t  numPointsSameCoordValue  =  distInt( gen );
    const std::size_t  indexCoord  =  distCoord( gen );
    if ( 0 == indexCoord )
    {
      const double  x1  =  dist( gen );
      for ( std::size_t j = 0; j < numPointsSameCoordValue && i < numPoints; ++j )
      {
        const double  x2  =  dist( gen );
        const double  x3  =  dist( gen );
        const double  x4  =  dist( gen );
        const d7cA::Point<double>  p( x1, x2, x3, x4 );
        arrPoints[ i ]  =  p;
        ++i;
      }
    }
    if ( 1 == indexCoord )
    {
      const double  x2  =  dist( gen );
      for ( std::size_t j = 0; j < numPointsSameCoordValue && i < numPoints; ++j )
      {
        const double  x1  =  dist( gen );
        const double  x3  =  dist( gen );
        const double  x4  =  dist( gen );
        const d7cA::Point<double>  p( x1, x2, x3, x4 );
        arrPoints[ i ]  =  p;
        ++i;
      }
    }
    if ( 2 == indexCoord )
    {
      const double  x3  =  dist( gen );
      for ( std::size_t j = 0; j < numPointsSameCoordValue && i < numPoints; ++j )
      {
        const double  x1  =  dist( gen );
        const double  x2  =  dist( gen );
        const double  x4  =  dist( gen );
        const d7cA::Point<double>  p( x1, x2, x3, x4 );
        arrPoints[ i ]  =  p;
        ++i;
      }
    }
    if ( 3 == indexCoord )
    {
      const double  x4  =  dist( gen );
      for ( std::size_t j = 0; j < numPointsSameCoordValue && i < numPoints; ++j )
      {
        const double  x1  =  dist( gen );
        const double  x2  =  dist( gen );
        const double  x3  =  dist( gen );
        const d7cA::Point<double>  p( x1, x2, x3, x4 );
        arrPoints[ i ]  =  p;
        ++i;
      }
    }
    if ( 4 == indexCoord )
    {
      const double  x1  =  dist( gen );
      const double  x2  =  dist( gen );
      for ( std::size_t j = 0; j < numPointsSameCoordValue && i < numPoints; ++j )
      {
        const double  x3  =  dist( gen );
        const double  x4  =  dist( gen );
        const d7cA::Point<double>  p( x1, x2, x3, x4 );
        arrPoints[ i ]  =  p;
        ++i;
      }
    }
    if ( 5 == indexCoord )
    {
      const double  x1  =  dist( gen );
      const double  x3  =  dist( gen );
      for ( std::size_t j = 0; j < numPointsSameCoordValue && i < numPoints; ++j )
      {
        const double  x2  =  dist( gen );
        const double  x4  =  dist( gen );
        const d7cA::Point<double>  p( x1, x2, x3, x4 );
        arrPoints[ i ]  =  p;
        ++i;
      }
    }
    if ( 6 == indexCoord )
    {
      const double  x1  =  dist( gen );
      const double  x4  =  dist( gen );
      for ( std::size_t j = 0; j < numPointsSameCoordValue && i < numPoints; ++j )
      {
        const double  x2  =  dist( gen );
        const double  x3  =  dist( gen );
        const d7cA::Point<double>  p( x1, x2, x3, x4 );
        arrPoints[ i ]  =  p;
        ++i;
      }
    }
    if ( 7 == indexCoord )
    {
      const double  x2  =  dist( gen );
      const double  x3  =  dist( gen );
      for ( std::size_t j = 0; j < numPointsSameCoordValue && i < numPoints; ++j )
      {
        const double  x1  =  dist( gen );
        const double  x4  =  dist( gen );
        const d7cA::Point<double>  p( x1, x2, x3, x4 );
        arrPoints[ i ]  =  p;
        ++i;
      }
    }
    if ( 8 == indexCoord )
    {
      const double  x2  =  dist( gen );
      const double  x4  =  dist( gen );
      for ( std::size_t j = 0; j < numPointsSameCoordValue && i < numPoints; ++j )
      {
        const double  x1  =  dist( gen );
        const double  x3  =  dist( gen );
        const d7cA::Point<double>  p( x1, x2, x3, x4 );
        arrPoints[ i ]  =  p;
        ++i;
      }
    }
    if ( 9 == indexCoord )
    {
      const double  x3  =  dist( gen );
      const double  x4  =  dist( gen );
      for ( std::size_t j = 0; j < numPointsSameCoordValue && i < numPoints; ++j )
      {
        const double  x1  =  dist( gen );
        const double  x2  =  dist( gen );
        const d7cA::Point<double>  p( x1, x2, x3, x4 );
        arrPoints[ i ]  =  p;
        ++i;
      }
    }
    if ( 10 == indexCoord )
    {
      const double  x1  =  dist( gen );
      const double  x2  =  dist( gen );
      const double  x3  =  dist( gen );
      for ( std::size_t j = 0; j < numPointsSameCoordValue && i < numPoints; ++j )
      {
        const double  x4  =  dist( gen );
        const d7cA::Point<double>  p( x1, x2, x3, x4 );
        arrPoints[ i ]  =  p;
        ++i;
      }
    }
    if ( 11 == indexCoord )
    {
      const double  x2  =  dist( gen );
      const double  x3  =  dist( gen );
      const double  x4  =  dist( gen );
      for ( std::size_t j = 0; j < numPointsSameCoordValue && i < numPoints; ++j )
      {
        const double  x1  =  dist( gen );
        const d7cA::Point<double>  p( x1, x2, x3, x4 );
        arrPoints[ i ]  =  p;
        ++i;
      }
    }
    if ( 12 == indexCoord )
    {
      const double  x3  =  dist( gen );
      const double  x4  =  dist( gen );
      const double  x1  =  dist( gen );
      for ( std::size_t j = 0; j < numPointsSameCoordValue && i < numPoints; ++j )
      {
        const double  x2  =  dist( gen );
        const d7cA::Point<double>  p( x1, x2, x3, x4 );
        arrPoints[ i ]  =  p;
        ++i;
      }
    }
    if ( 13 == indexCoord )
    {
      const double  x4  =  dist( gen );
      const double  x1  =  dist( gen );
      const double  x2  =  dist( gen );
      for ( std::size_t j = 0; j < numPointsSameCoordValue && i < numPoints; ++j )
      {
        const double  x3  =  dist( gen );
        const d7cA::Point<double>  p( x1, x2, x3, x4 );
        arrPoints[ i ]  =  p;
        ++i;
      }
    }
    if ( 14 == indexCoord )
    {
      const double  x1  =  dist( gen );
      const double  x2  =  dist( gen );
      const double  x3  =  dist( gen );
      const double  x4  =  dist( gen );
      const d7cA::Point<double>  p( x1, x2, x3, x4 );
      if ( i < numPoints )
        arrPoints[ i ]  =  p;
      ++i;
    }
  }

  d7cA::Octree<d7cA::Point, double>  octree;
  octree.init( arrPoints, numPoints, d7cA::comparePoints<double> );

  const std::size_t  numElements  =  octree.getNumElements();
  BOOST_CHECK_EQUAL( numElements, numPoints );

  // test the Octree::find() method:
  std::uniform_real_distribution<>  distTolerance( 0, 3 );
  std::uniform_real_distribution<>  distVariation( -1.5, +1.5 );
  std::uniform_int_distribution<>   distVarChoice( 0, 4 );
  for ( std::size_t i = 0; i < numPoints; ++i )
  {
    const int  var  =  distVarChoice( gen );
    if ( 0 == var ) // only x1 is variated
    {
      const double  tolerance  =  distTolerance( gen );//1.8
      const double  variation  =  distVariation( gen );//1.24

      const double  x1  =  arrPoints[ i ].x1() + variation;
      const double  x2  =  arrPoints[ i ].x2();
      const double  x3  =  arrPoints[ i ].x3();
      const double  x4  =  arrPoints[ i ].x4();
      const d7cA::Point<double>  p( x1, x2, x3, x4 );
      std::size_t  numOperations  =  0;
      const d7cA::OctreeObj<d7cA::Point, double> * const  result  =  octree.find( p, numOperations, tolerance );

//printf( "numOperations = %lu\n", numOperations );

      // 'tolerance' is applied independently to each coordinate.
      if ( tolerance >= std::abs( variation ) )
      {
        BOOST_CHECK( nullptr != result );
      }
      else
        {
          bool  someOtherPointAppearedWithinTolerance  =  false;
          if ( nullptr != result )
          {
            if ( tolerance >= std::abs( p.x1() - result->info.x1() )
              && tolerance >= std::abs( p.x2() - result->info.x2() )
              && tolerance >= std::abs( p.x3() - result->info.x3() )
              && tolerance >= std::abs( p.x4() - result->info.x4() )
               )
              someOtherPointAppearedWithinTolerance  =  true;
          }
          BOOST_CHECK( nullptr == result || true == someOtherPointAppearedWithinTolerance );
	}
    }
    // ************
    if ( 1 == var ) // only x2 is variated
    {
      const double  tolerance  =  distTolerance( gen );
      const double  variation  =  distVariation( gen );

      const double  x1  =  arrPoints[ i ].x1();
      const double  x2  =  arrPoints[ i ].x2() + variation;
      const double  x3  =  arrPoints[ i ].x3();
      const double  x4  =  arrPoints[ i ].x4();
      const d7cA::Point<double>  p( x1, x2, x3, x4 );
      std::size_t  numOperations  =  0;
      const d7cA::OctreeObj<d7cA::Point, double> * const  result  =  octree.find( p, numOperations, tolerance );

      // 'tolerance' is applied independently to each coordinate.
      if ( tolerance >= std::abs( variation ) )
      {
        BOOST_CHECK( nullptr != result );
      }
      else
        {
          bool  someOtherPointAppearedWithinTolerance  =  false;
          if ( nullptr != result )
          {
            if ( tolerance >= std::abs( p.x1() - result->info.x1() )
              && tolerance >= std::abs( p.x2() - result->info.x2() )
              && tolerance >= std::abs( p.x3() - result->info.x3() )
              && tolerance >= std::abs( p.x4() - result->info.x4() )
               )
              someOtherPointAppearedWithinTolerance  =  true;
          }
          BOOST_CHECK( nullptr == result || true == someOtherPointAppearedWithinTolerance );
	}
    }
    // ************
    if ( 2 == var ) // only x3 is variated
    {
      const double  tolerance  =  distTolerance( gen );
      const double  variation  =  distVariation( gen );

      const double  x1  =  arrPoints[ i ].x1();
      const double  x2  =  arrPoints[ i ].x2();
      const double  x3  =  arrPoints[ i ].x3() + variation;
      const double  x4  =  arrPoints[ i ].x4();
      const d7cA::Point<double>  p( x1, x2, x3, x4 );
      std::size_t  numOperations  =  0;
      const d7cA::OctreeObj<d7cA::Point, double> * const  result  =  octree.find( p, numOperations, tolerance );

      // 'tolerance' is applied independently to each coordinate.
      if ( tolerance >= std::abs( variation ) )
      {
        BOOST_CHECK( nullptr != result );
      }
      else
        {
          bool  someOtherPointAppearedWithinTolerance  =  false;
          if ( nullptr != result )
          {
            if ( tolerance >= std::abs( p.x1() - result->info.x1() )
              && tolerance >= std::abs( p.x2() - result->info.x2() )
              && tolerance >= std::abs( p.x3() - result->info.x3() )
              && tolerance >= std::abs( p.x4() - result->info.x4() )
               )
              someOtherPointAppearedWithinTolerance  =  true;
          }
          BOOST_CHECK( nullptr == result || true == someOtherPointAppearedWithinTolerance );
	}
    }
    // ************
    if ( 3 == var ) // only x4 is variated
    {
      const double  tolerance  =  distTolerance( gen );
      const double  variation  =  distVariation( gen );

      const double  x1  =  arrPoints[ i ].x1();
      const double  x2  =  arrPoints[ i ].x2();
      const double  x3  =  arrPoints[ i ].x3();
      const double  x4  =  arrPoints[ i ].x4() + variation;
      const d7cA::Point<double>  p( x1, x2, x3, x4 );
      std::size_t  numOperations  =  0;
      const d7cA::OctreeObj<d7cA::Point, double> * const  result  =  octree.find( p, numOperations, tolerance );

      // 'tolerance' is applied independently to each coordinate.
      if ( tolerance >= std::abs( variation ) )
      {
        BOOST_CHECK( nullptr != result );
      }
      else
        {
          bool  someOtherPointAppearedWithinTolerance  =  false;
          if ( nullptr != result )
          {
            if ( tolerance >= std::abs( p.x1() - result->info.x1() )
              && tolerance >= std::abs( p.x2() - result->info.x2() )
              && tolerance >= std::abs( p.x3() - result->info.x3() )
              && tolerance >= std::abs( p.x4() - result->info.x4() )
               )
              someOtherPointAppearedWithinTolerance  =  true;
          }
          BOOST_CHECK( nullptr == result || true == someOtherPointAppearedWithinTolerance );
	}
    }
    // ************
    if ( 4 == var ) // all the x1, x2, x3 and x4 is variated independently
    {
      const double  tolerance  =  distTolerance( gen );
      double  aVariation[ 4 ]  =  { 0, 0, 0, 0 };
      for ( short i = 0; i < 4; ++i )
        aVariation[ i ]  =  distVariation( gen );

      const double  x1  =  arrPoints[ i ].x1() + aVariation[ 0 ];
      const double  x2  =  arrPoints[ i ].x2() + aVariation[ 1 ];
      const double  x3  =  arrPoints[ i ].x3() + aVariation[ 2 ];
      const double  x4  =  arrPoints[ i ].x4() + aVariation[ 3 ];
      const d7cA::Point<double>  p( x1, x2, x3, x4 );
      std::size_t  numOperations  =  0;
      const d7cA::OctreeObj<d7cA::Point, double> * const  result  =  octree.find( p, numOperations, tolerance );

      double  variationMax  =  std::abs( aVariation[ 0 ] );
      for ( short i = 1; i < 4; ++i )
        if ( variationMax < std::abs( aVariation[ i ] ) )
          variationMax  =  std::abs( aVariation[ i ] );
      // 'tolerance' is applied independently to each coordinate.
      if ( tolerance >= std::abs( variationMax ) )
      {
        BOOST_CHECK( nullptr != result );
      }
      else
        {
          bool  someOtherPointAppearedWithinTolerance  =  false;
          if ( nullptr != result )
          {
            if ( tolerance >= std::abs( p.x1() - result->info.x1() )
              && tolerance >= std::abs( p.x2() - result->info.x2() )
              && tolerance >= std::abs( p.x3() - result->info.x3() )
              && tolerance >= std::abs( p.x4() - result->info.x4() )
               )
              someOtherPointAppearedWithinTolerance  =  true;
          }
          BOOST_CHECK( nullptr == result || true == someOtherPointAppearedWithinTolerance );
	}
    }

  }
/*
  std::sort( arrPoints, arrPoints + numPoints, d7cA::comparePoints<double> );
  for ( std::size_t i = 0; i < numPoints; ++i )
    printf( "%f\t%f\t%f\t%f\n", arrPoints[ i ].x1(), arrPoints[ i ].x2(), arrPoints[ i ].x3(), arrPoints[ i ].x4() );
*/
  delete [] arrPoints;
  arrPoints  =  nullptr;
}

BOOST_AUTO_TEST_CASE( testOctree2 )
{
  constexpr std::size_t  numPoints  =  10;
  d7cA::Point<double>  * arrPoints  =  new d7cA::Point<double> [ numPoints ];

  arrPoints[ 0 ]  =  d7cA::Point<double>( -47, -41, -14, 4 );
  arrPoints[ 1 ]  =  d7cA::Point<double>( -40, -41, 38, -6 );
  arrPoints[ 2 ]  =  d7cA::Point<double>( -32, -24, -21, -42 );
  arrPoints[ 3 ]  =  d7cA::Point<double>( -30, -41, 28, 25 );
  arrPoints[ 4 ]  =  d7cA::Point<double>( -1.6, -41, -17, -27 );
  arrPoints[ 5 ]  =  d7cA::Point<double>( 3.09, -41, -45, -28 );
  arrPoints[ 6 ]  =  d7cA::Point<double>( 13, -41, 6, -9 );
  arrPoints[ 7 ]  =  d7cA::Point<double>( 36, -28, -26, -10 );
  arrPoints[ 8 ]  =  d7cA::Point<double>( 36, 1.9, -26, -10 );
  arrPoints[ 9 ]  =  d7cA::Point<double>( 36, 39, -26, -10 );

  // numbers of shifts through the tree to find the corresponding elements:
  constexpr std::size_t  aNumOperations[ numPoints ]  =  { 3, 2, 1, 0, 4, 5, 6, 7, 8, 9 };

  d7cA::Octree<d7cA::Point, double>  octree;
  octree.init( arrPoints, numPoints, d7cA::comparePoints<double> );

  const std::size_t  numElements  =  octree.getNumElements();
  BOOST_CHECK_EQUAL( numElements, numPoints );

  // test the Octree::find() method:
  for ( std::size_t i = 0; i < numPoints; ++i )
  {
    const double  tolerance  =  0;

    const d7cA::Point<double>  p  =  arrPoints[ i ];
    std::size_t  numOperations  =  0;
    const d7cA::OctreeObj<d7cA::Point, double> * const  result  =  octree.find( p, numOperations, tolerance );

//printf( "numOperations = %lu\n", numOperations );
    BOOST_CHECK_EQUAL( numOperations, aNumOperations[ i ] );

    BOOST_CHECK( nullptr != result );
  }

  delete [] arrPoints;
  arrPoints  =  nullptr;
}


BOOST_AUTO_TEST_CASE( testOctree3 )
{
  constexpr std::size_t  numPoints  =  10;
  d7cA::Point<double>   arrPoints[ numPoints ];

  arrPoints[ 0 ]  =  d7cA::Point<double>( -26, -48, 15, -11 );
  arrPoints[ 1 ]  =  d7cA::Point<double>( -26, -33, 15, -11 );
  arrPoints[ 2 ]  =  d7cA::Point<double>( -26, -27, 15, -11 );
  arrPoints[ 3 ]  =  d7cA::Point<double>( -26, -8, 15, -11 );
  arrPoints[ 4 ]  =  d7cA::Point<double>( -26, 9, 15, -11 );
  arrPoints[ 5 ]  =  d7cA::Point<double>( -26, 10, 15, -11 );
  arrPoints[ 6 ]  =  d7cA::Point<double>( -26, 17.4, 15, -11 );
  arrPoints[ 7 ]  =  d7cA::Point<double>( -26, -17.5, 15, -11 );
  arrPoints[ 8 ]  =  d7cA::Point<double>( -26, 39, 15, -11 );
  arrPoints[ 9 ]  =  d7cA::Point<double>( -21, 43, -37, -6 );

  // numbers of shifts through the tree to find the corresponding elements:
  constexpr std::size_t  aNumOperations[ numPoints ]  =  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 1 };

  d7cA::Octree<d7cA::Point, double>  octree;
  octree.init( arrPoints, numPoints, d7cA::comparePoints<double> );

  const std::size_t  numElements  =  octree.getNumElements();
  BOOST_CHECK_EQUAL( numElements, numPoints );

  // test the Octree::find() method:
  for ( std::size_t i = 0; i < numPoints; ++i )
  {
    const double  tolerance  =  0;

    const d7cA::Point<double>  p  =  arrPoints[ i ];
    std::size_t  numOperations  =  0;
    const d7cA::OctreeObj<d7cA::Point, double> * const  result  =  octree.find( p, numOperations, tolerance );

//printf( "numOperations = %lu\n", numOperations );
    BOOST_CHECK_EQUAL( numOperations, aNumOperations[ i ] );

    BOOST_CHECK( nullptr != result );
  }
}


BOOST_AUTO_TEST_SUITE_END()
