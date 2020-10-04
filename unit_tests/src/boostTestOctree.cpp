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
  const d7cA::OctreeObj<d7cA::Point, double>  * const  pFound1  =  octree.find( point1 );

  BOOST_CHECK_EQUAL( nullptr, pFound1 );

  const d7cA::Point<double>  point2;
  const d7cA::OctreeObj<d7cA::Point, double>  * const  pFound2  =  octree.find( point2 );

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
/*
  const d7cA::Point<double>  p1( -45, -12, -7, -42 );
  const d7cA::Point<double>  p2( -45, -12, -7, 4 );
  const d7cA::Point<double>  p3( -45, -12, -7, 20 );
  const d7cA::Point<double>  p4( -39, -16, 0, 36 );
  const d7cA::Point<double>  p5( -30, 22, -40, -30 );
  const d7cA::Point<double>  p6( -27, 38.17, -40, -30 );
  const d7cA::Point<double>  p7( 0, 22, -40, -30 );
  const d7cA::Point<double>  p8( 5, 38.84, -40, -30 );
  const d7cA::Point<double>  p9( 26, -18, -40, -30 );
  const d7cA::Point<double>  p10( 41, 1.35, -40, -30 );
*/
/*
  const d7cA::Point<double>  p1( -33, 48, -33, 9 );
  const d7cA::Point<double>  p2( -23, 27, 21, -13 );
  const d7cA::Point<double>  p3( -6, -37, -4, 13 );
  const d7cA::Point<double>  p4( -6, -32, -14, 22 );
  const d7cA::Point<double>  p5( -6, -28, -21, -41 );
  const d7cA::Point<double>  p6( -6, -20, -38, 41 );
  const d7cA::Point<double>  p7( -6, 4, 31, 46 );
  const d7cA::Point<double>  p8( -6, 43, 31, -42 );
  const d7cA::Point<double>  p9( -6, 45, -24, -10 );
  const d7cA::Point<double>  p10( 25, -48, -41, -7 );
*/
/*
  arrPoints[ 0 ]  =  p1;
  arrPoints[ 1 ]  =  p2;
  arrPoints[ 2 ]  =  p3;
  arrPoints[ 3 ]  =  p4;
  arrPoints[ 4 ]  =  p5;
  arrPoints[ 5 ]  =  p6;
  arrPoints[ 6 ]  =  p7;
  arrPoints[ 7 ]  =  p8;
  arrPoints[ 8 ]  =  p9;
  arrPoints[ 9 ]  =  p10;
*/

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
    const double  x1  =  dist( gen );
    const double  x2  =  dist( gen );
    const double  x3  =  dist( gen );
    const double  x4  =  dist( gen );
    const d7cA::Point<double>  p( x1, x2, x3, x4 );
    arrPoints[ i ]  =  p;
    ++i;
  }

//  std::sort( arrPoints, arrPoints + numPoints, d7cA::comparePoints<double> );

//  for ( std::size_t i = 0; i < numPoints; ++i )
//    printf( "%lu\t%f\t%f\t%f\t%f\n", i, arrPoints[ i ].x1(), arrPoints[ i ].x2(), arrPoints[ i ].x3(), arrPoints[ i ].x4() );
 
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
    //const int  var  =  distVarChoice( gen );
    //if ( 0 == var )
    {
      const double  tolerance  =  3;//distTolerance( gen );
//      const double  variation  =  distVariation( gen );
      const double  x1  =  arrPoints[ i ].x1();// + variation;
      const double  x2  =  arrPoints[ i ].x2();
      const double  x3  =  arrPoints[ i ].x3();
      const double  x4  =  arrPoints[ i ].x4();
      const d7cA::Point<double>  p( x1, x2, x3, x4 );
      const d7cA::OctreeObj<d7cA::Point, double> * const  result  =  octree.find( p, tolerance );
      if ( tolerance >= dist2( p, arrPoints[ i ] ) )
        BOOST_CHECK( nullptr != result );
      else
        BOOST_CHECK( nullptr == result );
    }

  }

  delete [] arrPoints;
  arrPoints  =  nullptr;
}

BOOST_AUTO_TEST_SUITE_END()
