// This file is a part of the 'octree.didactic' project
//
// author : uri.heap  <u.heep@tutanota.com>
//
// license : GPL.v3+
//


#include "octree.hpp"

#include <algorithm>
#include <iterator>
#include <cstddef>
#include <random>
#include <chrono>
#include <cstdio>
#include <vector>



static
void
run_( const std::size_t&  numPointsInit,
      const std::size_t&  numPointsDelta,
      const std::size_t&  numIncrementsPoints,
      const std::size_t&  numRuns,
      const std::string&  filename
    ) noexcept;



template<class T>
struct  Predicate_ {
  public:
    Predicate_( const d7cA::Point<T>&  p,
                const T&               tolerance
              ) : p_( p ),
                  tolerance_( tolerance )
    { }
    bool  operator()( const d7cA::Point<T>&  p )
    {
      if ( tolerance_ >= std::abs( p.x1() - p_.x1() )
        && tolerance_ >= std::abs( p.x2() - p_.x2() )
        && tolerance_ >= std::abs( p.x3() - p_.x3() )
        && tolerance_ >= std::abs( p.x4() - p_.x4() )
         )
      {
        return  true;
      }
      return  false;
    }
  private:
    d7cA::Point<T>  p_;
    T               tolerance_;
};


template<class T>
struct  PredicateSearch_ {
  public:
    PredicateSearch_(
                      const T&  tolerance
                    ) :
                        tolerance_( tolerance )
    { }
    bool  operator()( const d7cA::Point<T>&  p1, const d7cA::Point<T>&  p2 )
    {
      if ( tolerance_ >= std::abs( p1.x1() - p2.x1() )
        && tolerance_ >= std::abs( p1.x2() - p2.x2() )
        && tolerance_ >= std::abs( p1.x3() - p2.x3() )
        && tolerance_ >= std::abs( p1.x4() - p2.x4() )
         )
      {
        return  true;
      }
      return  false;
    }
  private:
    T  tolerance_;
};


int  main( int  argc, char * argv[] ) {

  if ( 2 != argc )
    return  0;

  const std::string  filename( argv[ 1 ] );

  const std::size_t  numPointsInit  =  100;
  const std::size_t  numPointsDelta  =  50;
  const std::size_t  numIncrementsPoints  =  100;
  const std::size_t  numRuns  =  1000;
  run_( numPointsInit, numPointsDelta, numIncrementsPoints, numRuns, filename );

  return  0;
}



static
void
run_( const std::size_t&  numPointsInit,
      const std::size_t&  numPointsDelta,
      const std::size_t&  numIncrementsPoints,
      const std::size_t&  numRuns,
      const std::string&  filename
    ) noexcept
{
  if ( 0 == numRuns )
    return;

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

  FILE  *pF  =  nullptr;
  pF  =  fopen( filename.c_str(), "a" );
  if ( nullptr == pF )
    return;

  for ( std::size_t  iInc = 0; iInc < numIncrementsPoints; ++iInc )
  {
    const std::size_t  numPoints  =  numPointsInit + iInc * numPointsDelta;
if ( 2100 > numPoints )
	continue;

    std::vector<double>  vecTimeOctree( numRuns, 0 );
    std::vector<double>  vecTimeFindIf( numRuns, 0 );
    std::vector<double>  vecTimeSearch( numRuns, 0 );
    std::vector<double>  vecTimeFindIfSort( numRuns, 0 );
    std::vector<double>  vecTimeSearchSort( numRuns, 0 );

    for ( std::size_t  iRun = 0; iRun < numRuns; ++iRun )
    {
      d7cA::Point<double>  * arrPoints  =  new d7cA::Point<double> [ numPoints ];
      d7cA::Point<double>  * arrPointsInit  =  new d7cA::Point<double> [ numPoints ];
      d7cA::Point<double>  * arrPointsSorted  =  new d7cA::Point<double> [ numPoints ];

      double  meanTimeOctree  =  0;
      double  meanTimeFindIf  =  0;
      double  meanTimeSearch  =  0;
      double  meanTimeFindIfSort  =  0;
      double  meanTimeSearchSort  =  0;

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

      for ( std::size_t  iPoint = 0; iPoint < numPoints; ++iPoint )
      {
        arrPointsInit[ iPoint ]  =  arrPoints[ iPoint ];
        arrPointsSorted[ iPoint ]  =  arrPoints[ iPoint ];
      }

      std::sort( arrPoints, arrPoints + numPoints, d7cA::comparePoints<double> );

      d7cA::Octree<d7cA::Point, double>  octree;
      octree.init( arrPoints, numPoints, d7cA::comparePoints<double> );

      // benchmark the Octree::find() method:
      std::uniform_real_distribution<>  distTolerance( 0, 3 );
      std::uniform_real_distribution<>  distVariation( -1.5, +1.5 );
      std::uniform_int_distribution<>   distVarChoice( 0, 4 );
      for ( std::size_t i = 0; i < numPoints; ++i )
      {
        const int  var  =  distVarChoice( gen );
        if ( 0 == var ) // only x1 is variated
        {
          const double  tolerance  =  distTolerance( gen );
          const double  variation  =  distVariation( gen );

          const double  x1  =  arrPoints[ i ].x1() + variation;
          const double  x2  =  arrPoints[ i ].x2();
          const double  x3  =  arrPoints[ i ].x3();
          const double  x4  =  arrPoints[ i ].x4();
          const d7cA::Point<double>  p( x1, x2, x3, x4 );
          std::size_t  numOperations  =  0;

          // ******
          const auto  startOctree  =  std::chrono::steady_clock::now();
          const d7cA::OctreeObj<d7cA::Point, double> * const  result  =  octree.find( p, numOperations, tolerance );
          const auto  endOctree  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffOctree  =  endOctree - startOctree;
          const double  timeOctree  =  std::chrono::duration_cast<std::chrono::microseconds>( diffOctree ).count();
          meanTimeOctree  +=  timeOctree;
          // ******

          // ******
          const auto  startFindIf  =  std::chrono::steady_clock::now();
          const auto  pFindIf  =  std::find_if( arrPoints, arrPoints + numPoints, Predicate_<double>( p, tolerance ) );
          const auto  endFindIf  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffFindIf  =  endFindIf - startFindIf;
          const double  timeFindIf  =  std::chrono::duration_cast<std::chrono::microseconds>( diffFindIf ).count();
          meanTimeFindIf  +=  timeFindIf;
          // ******
          // ******
          const d7cA::Point<double>  pS[ 1 ]  =  { p };
          const auto  startSearch  =  std::chrono::steady_clock::now();
          const auto  pSearch  =  std::search( arrPoints, arrPoints + numPoints, pS, pS + 1, PredicateSearch_<double>( tolerance ) );
          const auto  endSearch  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffSearch  =  endSearch - startSearch;
          const double  timeSearch  =  std::chrono::duration_cast<std::chrono::microseconds>( diffSearch ).count();
	  meanTimeSearch  +=  timeSearch;
          // ******
          // include sorting:
          // ******
	  for ( std::size_t iPoint = 0; iPoint < numPoints; ++iPoint )
            arrPointsSorted[ iPoint ]  =  arrPointsInit[ iPoint ];
          // ******
          const auto  startFindIfSort  =  std::chrono::steady_clock::now();
          std::sort( arrPointsSorted, arrPointsSorted + numPoints, d7cA::comparePoints<double> );
	  const auto  pFindIfSort  =  std::find_if( arrPointsSorted, arrPointsSorted + numPoints, Predicate_<double>( p, tolerance ) );
          const auto  endFindIfSort  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffFindIfSort  =  endFindIfSort - startFindIfSort;
          const double  timeFindIfSort  =  std::chrono::duration_cast<std::chrono::microseconds>( diffFindIfSort ).count();
          meanTimeFindIfSort  +=  timeFindIfSort;
          // ******
          // ******
	  for ( std::size_t iPoint = 0; iPoint < numPoints; ++iPoint )
            arrPointsSorted[ iPoint ]  =  arrPointsInit[ iPoint ];
          const auto  startSearchSort  =  std::chrono::steady_clock::now();
          std::sort( arrPointsSorted, arrPointsSorted + numPoints, d7cA::comparePoints<double> );
          const d7cA::Point<double>  pSSort[ 1 ]  =  { p };
	  const auto  pSearchSort  =  std::search( arrPointsSorted, arrPointsSorted + numPoints, pSSort, pSSort + 1, PredicateSearch_<double>( tolerance ) );
          const auto  endSearchSort  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffSearchSort  =  endSearchSort - startSearchSort;
          const double  timeSearchSort  =  std::chrono::duration_cast<std::chrono::microseconds>( diffSearchSort ).count();
	  meanTimeSearchSort  +=  timeSearchSort;
          // ******

          // 'tolerance' is applied independently to each coordinate.
          if ( nullptr != result
            && arrPoints + numPoints != pFindIf
            && arrPoints + numPoints != pSearch
            && arrPoints + numPoints != pFindIfSort
            && arrPoints + numPoints != pSearchSort
             )
          {
            if ( tolerance < std::abs( p.x1() - result->info.x1() )
              || tolerance < std::abs( p.x2() - result->info.x2() )
              || tolerance < std::abs( p.x3() - result->info.x3() )
              || tolerance < std::abs( p.x4() - result->info.x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pFindIf->x1() )
              || tolerance < std::abs( p.x2() - pFindIf->x2() )
              || tolerance < std::abs( p.x3() - pFindIf->x3() )
              || tolerance < std::abs( p.x4() - pFindIf->x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pSearch->x1() )
              || tolerance < std::abs( p.x2() - pSearch->x2() )
              || tolerance < std::abs( p.x3() - pSearch->x3() )
              || tolerance < std::abs( p.x4() - pSearch->x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pFindIfSort->x1() )
              || tolerance < std::abs( p.x2() - pFindIfSort->x2() )
              || tolerance < std::abs( p.x3() - pFindIfSort->x3() )
              || tolerance < std::abs( p.x4() - pFindIfSort->x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pSearchSort->x1() )
              || tolerance < std::abs( p.x2() - pSearchSort->x2() )
              || tolerance < std::abs( p.x3() - pSearchSort->x3() )
              || tolerance < std::abs( p.x4() - pSearchSort->x4() )
               )
              printf( "error\n" );
          }
          if ( ( nullptr == result
              && arrPoints + numPoints != pFindIf )
            || ( nullptr != result
              && arrPoints + numPoints == pFindIf )
            || ( nullptr == result
              && arrPoints + numPoints != pSearch )
            || ( nullptr != result
              && arrPoints + numPoints == pSearch )
            || ( arrPoints + numPoints == pFindIf
              && arrPoints + numPoints != pSearch )
            || ( arrPoints + numPoints != pFindIf
              && arrPoints + numPoints == pSearch )
             )
            printf( "error\n" );
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

          // ******
          const auto  startOctree  =  std::chrono::steady_clock::now();
          const d7cA::OctreeObj<d7cA::Point, double> * const  result  =  octree.find( p, numOperations, tolerance );
          const auto  endOctree  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffOctree  =  endOctree - startOctree;
          const double  timeOctree  =  std::chrono::duration_cast<std::chrono::microseconds>( diffOctree ).count();
          meanTimeOctree  +=  timeOctree;
          // ******

          // ******
          const auto  startFindIf  =  std::chrono::steady_clock::now();
          const auto  pFindIf  =  std::find_if( arrPoints, arrPoints + numPoints, Predicate_<double>( p, tolerance ) );
          const auto  endFindIf  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffFindIf  =  endFindIf - startFindIf;
          const double  timeFindIf  =  std::chrono::duration_cast<std::chrono::microseconds>( diffFindIf ).count();
          meanTimeFindIf  +=  timeFindIf;
          // ******
          // ******
          const d7cA::Point<double>  pS[ 1 ]  =  { p };
          const auto  startSearch  =  std::chrono::steady_clock::now();
          const auto  pSearch  =  std::search( arrPoints, arrPoints + numPoints, pS, pS + 1, PredicateSearch_<double>( tolerance ) );
          const auto  endSearch  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffSearch  =  endSearch - startSearch;
          const double  timeSearch  =  std::chrono::duration_cast<std::chrono::microseconds>( diffSearch ).count();
	  meanTimeSearch  +=  timeSearch;
          // ******
          // include sorting:
          // ******
	  for ( std::size_t iPoint = 0; iPoint < numPoints; ++iPoint )
            arrPointsSorted[ iPoint ]  =  arrPointsInit[ iPoint ];
          // ******
          const auto  startFindIfSort  =  std::chrono::steady_clock::now();
          std::sort( arrPointsSorted, arrPointsSorted + numPoints, d7cA::comparePoints<double> );
	  const auto  pFindIfSort  =  std::find_if( arrPointsSorted, arrPointsSorted + numPoints, Predicate_<double>( p, tolerance ) );
          const auto  endFindIfSort  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffFindIfSort  =  endFindIfSort - startFindIfSort;
          const double  timeFindIfSort  =  std::chrono::duration_cast<std::chrono::microseconds>( diffFindIfSort ).count();
          meanTimeFindIfSort  +=  timeFindIfSort;
          // ******
          // ******
	  for ( std::size_t iPoint = 0; iPoint < numPoints; ++iPoint )
            arrPointsSorted[ iPoint ]  =  arrPointsInit[ iPoint ];
          const auto  startSearchSort  =  std::chrono::steady_clock::now();
          std::sort( arrPointsSorted, arrPointsSorted + numPoints, d7cA::comparePoints<double> );
          const d7cA::Point<double>  pSSort[ 1 ]  =  { p };
	  const auto  pSearchSort  =  std::search( arrPointsSorted, arrPointsSorted + numPoints, pSSort, pSSort + 1, PredicateSearch_<double>( tolerance ) );
          const auto  endSearchSort  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffSearchSort  =  endSearchSort - startSearchSort;
          const double  timeSearchSort  =  std::chrono::duration_cast<std::chrono::microseconds>( diffSearchSort ).count();
	  meanTimeSearchSort  +=  timeSearchSort;
          // ******
          // 'tolerance' is applied independently to each coordinate.
          if ( nullptr != result
            && arrPoints + numPoints != pFindIf
            && arrPoints + numPoints != pSearch
            && arrPoints + numPoints != pFindIfSort
            && arrPoints + numPoints != pSearchSort
             )
          {
            if ( tolerance < std::abs( p.x1() - result->info.x1() )
              || tolerance < std::abs( p.x2() - result->info.x2() )
              || tolerance < std::abs( p.x3() - result->info.x3() )
              || tolerance < std::abs( p.x4() - result->info.x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pFindIf->x1() )
              || tolerance < std::abs( p.x2() - pFindIf->x2() )
              || tolerance < std::abs( p.x3() - pFindIf->x3() )
              || tolerance < std::abs( p.x4() - pFindIf->x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pSearch->x1() )
              || tolerance < std::abs( p.x2() - pSearch->x2() )
              || tolerance < std::abs( p.x3() - pSearch->x3() )
              || tolerance < std::abs( p.x4() - pSearch->x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pFindIfSort->x1() )
              || tolerance < std::abs( p.x2() - pFindIfSort->x2() )
              || tolerance < std::abs( p.x3() - pFindIfSort->x3() )
              || tolerance < std::abs( p.x4() - pFindIfSort->x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pSearchSort->x1() )
              || tolerance < std::abs( p.x2() - pSearchSort->x2() )
              || tolerance < std::abs( p.x3() - pSearchSort->x3() )
              || tolerance < std::abs( p.x4() - pSearchSort->x4() )
               )
              printf( "error\n" );
          }
          if ( ( nullptr == result
              && arrPoints + numPoints != pFindIf )
            || ( nullptr != result
              && arrPoints + numPoints == pFindIf )
            || ( nullptr == result
              && arrPoints + numPoints != pSearch )
            || ( nullptr != result
              && arrPoints + numPoints == pSearch )
            || ( arrPoints + numPoints == pFindIf
              && arrPoints + numPoints != pSearch )
            || ( arrPoints + numPoints != pFindIf
              && arrPoints + numPoints == pSearch )
             )
            printf( "error\n" );
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

          // ******
          const auto  startOctree  =  std::chrono::steady_clock::now();
          const d7cA::OctreeObj<d7cA::Point, double> * const  result  =  octree.find( p, numOperations, tolerance );
          const auto  endOctree  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffOctree  =  endOctree - startOctree;
          const double  timeOctree  =  std::chrono::duration_cast<std::chrono::microseconds>( diffOctree ).count();
          meanTimeOctree  +=  timeOctree;
          // ******

          // ******
          const auto  startFindIf  =  std::chrono::steady_clock::now();
          const auto  pFindIf  =  std::find_if( arrPoints, arrPoints + numPoints, Predicate_<double>( p, tolerance ) );
          const auto  endFindIf  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffFindIf  =  endFindIf - startFindIf;
          const double  timeFindIf  =  std::chrono::duration_cast<std::chrono::microseconds>( diffFindIf ).count();
          meanTimeFindIf  +=  timeFindIf;
          // ******
          // ******
          const d7cA::Point<double>  pS[ 1 ]  =  { p };
          const auto  startSearch  =  std::chrono::steady_clock::now();
          const auto  pSearch  =  std::search( arrPoints, arrPoints + numPoints, pS, pS + 1, PredicateSearch_<double>( tolerance ) );
          const auto  endSearch  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffSearch  =  endSearch - startSearch;
          const double  timeSearch  =  std::chrono::duration_cast<std::chrono::microseconds>( diffSearch ).count();
	  meanTimeSearch  +=  timeSearch;
          // ******
          // include sorting:
          // ******
	  for ( std::size_t iPoint = 0; iPoint < numPoints; ++iPoint )
            arrPointsSorted[ iPoint ]  =  arrPointsInit[ iPoint ];
          // ******
          const auto  startFindIfSort  =  std::chrono::steady_clock::now();
          std::sort( arrPointsSorted, arrPointsSorted + numPoints, d7cA::comparePoints<double> );
	  const auto  pFindIfSort  =  std::find_if( arrPointsSorted, arrPointsSorted + numPoints, Predicate_<double>( p, tolerance ) );
          const auto  endFindIfSort  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffFindIfSort  =  endFindIfSort - startFindIfSort;
          const double  timeFindIfSort  =  std::chrono::duration_cast<std::chrono::microseconds>( diffFindIfSort ).count();
          meanTimeFindIfSort  +=  timeFindIfSort;
          // ******
          // ******
	  for ( std::size_t iPoint = 0; iPoint < numPoints; ++iPoint )
            arrPointsSorted[ iPoint ]  =  arrPointsInit[ iPoint ];
          const auto  startSearchSort  =  std::chrono::steady_clock::now();
          std::sort( arrPointsSorted, arrPointsSorted + numPoints, d7cA::comparePoints<double> );
          const d7cA::Point<double>  pSSort[ 1 ]  =  { p };
	  const auto  pSearchSort  =  std::search( arrPointsSorted, arrPointsSorted + numPoints, pSSort, pSSort + 1, PredicateSearch_<double>( tolerance ) );
          const auto  endSearchSort  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffSearchSort  =  endSearchSort - startSearchSort;
          const double  timeSearchSort  =  std::chrono::duration_cast<std::chrono::microseconds>( diffSearchSort ).count();
	  meanTimeSearchSort  +=  timeSearchSort;
          // ******
          // 'tolerance' is applied independently to each coordinate.
          if ( nullptr != result
            && arrPoints + numPoints != pFindIf
            && arrPoints + numPoints != pSearch
            && arrPoints + numPoints != pFindIfSort
            && arrPoints + numPoints != pSearchSort
             )
          {
            if ( tolerance < std::abs( p.x1() - result->info.x1() )
              || tolerance < std::abs( p.x2() - result->info.x2() )
              || tolerance < std::abs( p.x3() - result->info.x3() )
              || tolerance < std::abs( p.x4() - result->info.x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pFindIf->x1() )
              || tolerance < std::abs( p.x2() - pFindIf->x2() )
              || tolerance < std::abs( p.x3() - pFindIf->x3() )
              || tolerance < std::abs( p.x4() - pFindIf->x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pSearch->x1() )
              || tolerance < std::abs( p.x2() - pSearch->x2() )
              || tolerance < std::abs( p.x3() - pSearch->x3() )
              || tolerance < std::abs( p.x4() - pSearch->x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pFindIfSort->x1() )
              || tolerance < std::abs( p.x2() - pFindIfSort->x2() )
              || tolerance < std::abs( p.x3() - pFindIfSort->x3() )
              || tolerance < std::abs( p.x4() - pFindIfSort->x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pSearchSort->x1() )
              || tolerance < std::abs( p.x2() - pSearchSort->x2() )
              || tolerance < std::abs( p.x3() - pSearchSort->x3() )
              || tolerance < std::abs( p.x4() - pSearchSort->x4() )
               )
              printf( "error\n" );
          }
          if ( ( nullptr == result
              && arrPoints + numPoints != pFindIf )
            || ( nullptr != result
              && arrPoints + numPoints == pFindIf )
            || ( nullptr == result
              && arrPoints + numPoints != pSearch )
            || ( nullptr != result
              && arrPoints + numPoints == pSearch )
            || ( arrPoints + numPoints == pFindIf
              && arrPoints + numPoints != pSearch )
            || ( arrPoints + numPoints != pFindIf
              && arrPoints + numPoints == pSearch )
             )
            printf( "error\n" );
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

          // ******
          const auto  startOctree  =  std::chrono::steady_clock::now();
          const d7cA::OctreeObj<d7cA::Point, double> * const  result  =  octree.find( p, numOperations, tolerance );
          const auto  endOctree  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffOctree  =  endOctree - startOctree;
          const double  timeOctree  =  std::chrono::duration_cast<std::chrono::microseconds>( diffOctree ).count();
          meanTimeOctree  +=  timeOctree;
          // ******

          // ******
          const auto  startFindIf  =  std::chrono::steady_clock::now();
          const auto  pFindIf  =  std::find_if( arrPoints, arrPoints + numPoints, Predicate_<double>( p, tolerance ) );
          const auto  endFindIf  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffFindIf  =  endFindIf - startFindIf;
          const double  timeFindIf  =  std::chrono::duration_cast<std::chrono::microseconds>( diffFindIf ).count();
          meanTimeFindIf  +=  timeFindIf;
          // ******
          // ******
          const d7cA::Point<double>  pS[ 1 ]  =  { p };
          const auto  startSearch  =  std::chrono::steady_clock::now();
          const auto  pSearch  =  std::search( arrPoints, arrPoints + numPoints, pS, pS + 1, PredicateSearch_<double>( tolerance ) );
          const auto  endSearch  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffSearch  =  endSearch - startSearch;
          const double  timeSearch  =  std::chrono::duration_cast<std::chrono::microseconds>( diffSearch ).count();
	  meanTimeSearch  +=  timeSearch;
          // ******
          // include sorting:
          // ******
	  for ( std::size_t iPoint = 0; iPoint < numPoints; ++iPoint )
            arrPointsSorted[ iPoint ]  =  arrPointsInit[ iPoint ];
          // ******
          const auto  startFindIfSort  =  std::chrono::steady_clock::now();
          std::sort( arrPointsSorted, arrPointsSorted + numPoints, d7cA::comparePoints<double> );
	  const auto  pFindIfSort  =  std::find_if( arrPointsSorted, arrPointsSorted + numPoints, Predicate_<double>( p, tolerance ) );
          const auto  endFindIfSort  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffFindIfSort  =  endFindIfSort - startFindIfSort;
          const double  timeFindIfSort  =  std::chrono::duration_cast<std::chrono::microseconds>( diffFindIfSort ).count();
          meanTimeFindIfSort  +=  timeFindIfSort;
          // ******
          // ******
	  for ( std::size_t iPoint = 0; iPoint < numPoints; ++iPoint )
            arrPointsSorted[ iPoint ]  =  arrPointsInit[ iPoint ];
          const auto  startSearchSort  =  std::chrono::steady_clock::now();
          std::sort( arrPointsSorted, arrPointsSorted + numPoints, d7cA::comparePoints<double> );
          const d7cA::Point<double>  pSSort[ 1 ]  =  { p };
	  const auto  pSearchSort  =  std::search( arrPointsSorted, arrPointsSorted + numPoints, pSSort, pSSort + 1, PredicateSearch_<double>( tolerance ) );
          const auto  endSearchSort  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffSearchSort  =  endSearchSort - startSearchSort;
          const double  timeSearchSort  =  std::chrono::duration_cast<std::chrono::microseconds>( diffSearchSort ).count();
	  meanTimeSearchSort  +=  timeSearchSort;
          // ******
          // 'tolerance' is applied independently to each coordinate.
          if ( nullptr != result
            && arrPoints + numPoints != pFindIf
            && arrPoints + numPoints != pSearch
            && arrPoints + numPoints != pFindIfSort
            && arrPoints + numPoints != pSearchSort
             )
          {
            if ( tolerance < std::abs( p.x1() - result->info.x1() )
              || tolerance < std::abs( p.x2() - result->info.x2() )
              || tolerance < std::abs( p.x3() - result->info.x3() )
              || tolerance < std::abs( p.x4() - result->info.x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pFindIf->x1() )
              || tolerance < std::abs( p.x2() - pFindIf->x2() )
              || tolerance < std::abs( p.x3() - pFindIf->x3() )
              || tolerance < std::abs( p.x4() - pFindIf->x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pSearch->x1() )
              || tolerance < std::abs( p.x2() - pSearch->x2() )
              || tolerance < std::abs( p.x3() - pSearch->x3() )
              || tolerance < std::abs( p.x4() - pSearch->x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pFindIfSort->x1() )
              || tolerance < std::abs( p.x2() - pFindIfSort->x2() )
              || tolerance < std::abs( p.x3() - pFindIfSort->x3() )
              || tolerance < std::abs( p.x4() - pFindIfSort->x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pSearchSort->x1() )
              || tolerance < std::abs( p.x2() - pSearchSort->x2() )
              || tolerance < std::abs( p.x3() - pSearchSort->x3() )
              || tolerance < std::abs( p.x4() - pSearchSort->x4() )
               )
              printf( "error\n" );
          }
          if ( ( nullptr == result
              && arrPoints + numPoints != pFindIf )
            || ( nullptr != result
              && arrPoints + numPoints == pFindIf )
            || ( nullptr == result
              && arrPoints + numPoints != pSearch )
            || ( nullptr != result
              && arrPoints + numPoints == pSearch )
            || ( arrPoints + numPoints == pFindIf
              && arrPoints + numPoints != pSearch )
            || ( arrPoints + numPoints != pFindIf
              && arrPoints + numPoints == pSearch )
             )
            printf( "error\n" );
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

          // ******
          const auto  startOctree  =  std::chrono::steady_clock::now();
          const d7cA::OctreeObj<d7cA::Point, double> * const  result  =  octree.find( p, numOperations, tolerance );
          const auto  endOctree  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffOctree  =  endOctree - startOctree;
          const double  timeOctree  =  std::chrono::duration_cast<std::chrono::microseconds>( diffOctree ).count();
          meanTimeOctree  +=  timeOctree;
          // ******

          // ******
          const auto  startFindIf  =  std::chrono::steady_clock::now();
          const auto  pFindIf  =  std::find_if( arrPoints, arrPoints + numPoints, Predicate_<double>( p, tolerance ) );
          const auto  endFindIf  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffFindIf  =  endFindIf - startFindIf;
          const double  timeFindIf  =  std::chrono::duration_cast<std::chrono::microseconds>( diffFindIf ).count();
          meanTimeFindIf  +=  timeFindIf;
          // ******
          // ******
          const d7cA::Point<double>  pS[ 1 ]  =  { p };
          const auto  startSearch  =  std::chrono::steady_clock::now();
          const auto  pSearch  =  std::search( arrPoints, arrPoints + numPoints, pS, pS + 1, PredicateSearch_<double>( tolerance ) );
          const auto  endSearch  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffSearch  =  endSearch - startSearch;
          const double  timeSearch  =  std::chrono::duration_cast<std::chrono::microseconds>( diffSearch ).count();
	  meanTimeSearch  +=  timeSearch;
          // ******
          // include sorting:
          // ******
	  for ( std::size_t iPoint = 0; iPoint < numPoints; ++iPoint )
            arrPointsSorted[ iPoint ]  =  arrPointsInit[ iPoint ];
          // ******
          const auto  startFindIfSort  =  std::chrono::steady_clock::now();
          std::sort( arrPointsSorted, arrPointsSorted + numPoints, d7cA::comparePoints<double> );
	  const auto  pFindIfSort  =  std::find_if( arrPointsSorted, arrPointsSorted + numPoints, Predicate_<double>( p, tolerance ) );
          const auto  endFindIfSort  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffFindIfSort  =  endFindIfSort - startFindIfSort;
          const double  timeFindIfSort  =  std::chrono::duration_cast<std::chrono::microseconds>( diffFindIfSort ).count();
          meanTimeFindIfSort  +=  timeFindIfSort;
          // ******
          // ******
	  for ( std::size_t iPoint = 0; iPoint < numPoints; ++iPoint )
            arrPointsSorted[ iPoint ]  =  arrPointsInit[ iPoint ];
          const auto  startSearchSort  =  std::chrono::steady_clock::now();
          std::sort( arrPointsSorted, arrPointsSorted + numPoints, d7cA::comparePoints<double> );
          const d7cA::Point<double>  pSSort[ 1 ]  =  { p };
	  const auto  pSearchSort  =  std::search( arrPointsSorted, arrPointsSorted + numPoints, pSSort, pSSort + 1, PredicateSearch_<double>( tolerance ) );
          const auto  endSearchSort  =  std::chrono::steady_clock::now();
          const std::chrono::duration<double>  diffSearchSort  =  endSearchSort - startSearchSort;
          const double  timeSearchSort  =  std::chrono::duration_cast<std::chrono::microseconds>( diffSearchSort ).count();
	  meanTimeSearchSort  +=  timeSearchSort;
          // ******
          // 'tolerance' is applied independently to each coordinate.
          if ( nullptr != result
            && arrPoints + numPoints != pFindIf
            && arrPoints + numPoints != pSearch
            && arrPoints + numPoints != pFindIfSort
            && arrPoints + numPoints != pSearchSort
             )
          {
            if ( tolerance < std::abs( p.x1() - result->info.x1() )
              || tolerance < std::abs( p.x2() - result->info.x2() )
              || tolerance < std::abs( p.x3() - result->info.x3() )
              || tolerance < std::abs( p.x4() - result->info.x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pFindIf->x1() )
              || tolerance < std::abs( p.x2() - pFindIf->x2() )
              || tolerance < std::abs( p.x3() - pFindIf->x3() )
              || tolerance < std::abs( p.x4() - pFindIf->x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pSearch->x1() )
              || tolerance < std::abs( p.x2() - pSearch->x2() )
              || tolerance < std::abs( p.x3() - pSearch->x3() )
              || tolerance < std::abs( p.x4() - pSearch->x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pFindIfSort->x1() )
              || tolerance < std::abs( p.x2() - pFindIfSort->x2() )
              || tolerance < std::abs( p.x3() - pFindIfSort->x3() )
              || tolerance < std::abs( p.x4() - pFindIfSort->x4() )
               )
              printf( "error\n" );
            if ( tolerance < std::abs( p.x1() - pSearchSort->x1() )
              || tolerance < std::abs( p.x2() - pSearchSort->x2() )
              || tolerance < std::abs( p.x3() - pSearchSort->x3() )
              || tolerance < std::abs( p.x4() - pSearchSort->x4() )
               )
              printf( "error\n" );
          }
          if ( ( nullptr == result
              && arrPoints + numPoints != pFindIf )
            || ( nullptr != result
              && arrPoints + numPoints == pFindIf )
            || ( nullptr == result
              && arrPoints + numPoints != pSearch )
            || ( nullptr != result
              && arrPoints + numPoints == pSearch )
            || ( arrPoints + numPoints == pFindIf
              && arrPoints + numPoints != pSearch )
            || ( arrPoints + numPoints != pFindIf
              && arrPoints + numPoints == pSearch )
             )
            printf( "error\n" );
        }
      } // for ( i < numPoints )

      meanTimeOctree  /=  numPoints;
      meanTimeFindIf  /=  numPoints;
      meanTimeSearch  /=  numPoints;
      meanTimeFindIfSort  /=  numPoints;
      meanTimeSearchSort  /=  numPoints;

      vecTimeOctree[ iRun ]  =  meanTimeOctree;
      vecTimeFindIf[ iRun ]  =  meanTimeFindIf;
      vecTimeSearch[ iRun ]  =  meanTimeSearch;
      vecTimeFindIfSort[ iRun ]  =  meanTimeFindIfSort;
      vecTimeSearchSort[ iRun ]  =  meanTimeSearchSort;

      delete [] arrPoints;
      arrPoints  =  nullptr;
      delete [] arrPointsInit;
      arrPointsInit  =  nullptr;
      delete [] arrPointsSorted;
      arrPointsSorted  =  nullptr;
    } // for ( iRun )

    double  meanTimeOctree  =  0;
    double  meanTimeFindIf  =  0;
    double  meanTimeSearch  =  0;
    double  meanTimeFindIfSort  =  0;
    double  meanTimeSearchSort  =  0;
    for ( std::size_t iRun = 0; iRun < numRuns; ++iRun )
    {
      meanTimeOctree  +=  vecTimeOctree[ iRun ];
      meanTimeFindIf  +=  vecTimeFindIf[ iRun ];
      meanTimeSearch  +=  vecTimeSearch[ iRun ];
      meanTimeFindIfSort  +=  vecTimeFindIfSort[ iRun ];
      meanTimeSearchSort  +=  vecTimeSearchSort[ iRun ];
    }
    meanTimeOctree  /=  numRuns;
    meanTimeFindIf  /=  numRuns;
    meanTimeSearch  /=  numRuns;
    meanTimeFindIfSort  /=  numRuns;
    meanTimeSearchSort  /=  numRuns;

    double  sdTimeOctree  =  0;
    double  sdTimeFindIf  =  0;
    double  sdTimeSearch  =  0;
    double  sdTimeFindIfSort  =  0;
    double  sdTimeSearchSort  =  0;
    for ( std::size_t iRun = 0; iRun < numRuns; ++iRun )
    {
      sdTimeOctree  +=  ( vecTimeOctree[ iRun ] - meanTimeOctree ) * ( vecTimeOctree[ iRun ] - meanTimeOctree );
      sdTimeFindIf  +=  ( vecTimeFindIf[ iRun ] - meanTimeFindIf ) * ( vecTimeFindIf[ iRun ] - meanTimeFindIf );
      sdTimeSearch  +=  ( vecTimeSearch[ iRun ] - meanTimeSearch ) * ( vecTimeSearch[ iRun ] - meanTimeSearch );
      sdTimeFindIfSort  +=  ( vecTimeFindIfSort[ iRun ] - meanTimeFindIfSort ) * ( vecTimeFindIfSort[ iRun ] - meanTimeFindIfSort );
      sdTimeSearchSort  +=  ( vecTimeSearchSort[ iRun ] - meanTimeSearchSort ) * ( vecTimeSearchSort[ iRun ] - meanTimeSearchSort );
    }
    sdTimeOctree  /=  numRuns;
    sdTimeFindIf  /=  numRuns;
    sdTimeSearch  /=  numRuns;
    sdTimeFindIfSort  /=  numRuns;
    sdTimeSearchSort  /=  numRuns;
    sdTimeOctree  =  std::sqrt( sdTimeOctree );
    sdTimeFindIf  =  std::sqrt( sdTimeFindIf );
    sdTimeSearch  =  std::sqrt( sdTimeSearch );
    sdTimeFindIfSort  =  std::sqrt( sdTimeFindIfSort );
    sdTimeSearchSort  =  std::sqrt( sdTimeSearchSort );

    // ******
    fprintf( pF, "%lu\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", numPoints, meanTimeOctree, sdTimeOctree, meanTimeFindIf, sdTimeFindIf, meanTimeSearch, sdTimeSearch, meanTimeFindIfSort, sdTimeFindIfSort, meanTimeSearchSort, sdTimeSearchSort );
    printf( "%lu\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", numPoints, meanTimeOctree, sdTimeOctree, meanTimeFindIf, sdTimeFindIf, meanTimeSearch, sdTimeSearch, meanTimeFindIfSort, sdTimeFindIfSort, meanTimeSearchSort, sdTimeSearchSort );
  } // for ( iInc )

  fclose( pF );
  pF  =  nullptr;
}


