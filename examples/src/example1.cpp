#include "octree.hpp"

#include <random>
#include <cstdio>

int  main() {

  // generate a set of random elements to create Octree:
  std::random_device  rd;
  std::mt19937  gen( rd() );
  std::uniform_real_distribution<>  dist1( -50, 50 );
  std::uniform_real_distribution<>  dist2( -25, 75 );

  constexpr unsigned short  numElements  =  500;
  d7cA::Point<double>  arrElements[ numElements ];

  for ( unsigned short  iElem = 0; iElem < numElements; ++iElem )
  {
    const double  x1  =  dist1( gen );
    const double  x2  =  dist1( gen );
    const double  x3  =  dist1( gen );
    const double  x4  =  dist1( gen );
    const d7cA::Point<double>  p( x1, x2, x3, x4 );
    arrElements[ iElem ]  =  p;
  }

  // construct an Octree:
  d7cA::Octree<d7cA::Point, double>  octree1;
  octree1.init( arrElements, numElements, d7cA::comparePoints<double> );

  // calculate the number of newly generated elements that fall within
  // the specified tolerance from the elements of the constructed octree1:
  // NOTE: 'tolerance' is applied to each of the four 'coordinates' of an element
  // of a tree:
  constexpr double  tolerance  =  5;

  unsigned short  count  =  0;
  for ( unsigned short  iElem = 0; iElem < numElements; ++iElem )
  {
    const double  x1  =  dist2( gen );
    const double  x2  =  dist2( gen );
    const double  x3  =  dist2( gen );
    const double  x4  =  dist2( gen );
    const d7cA::Point<double>  p( x1, x2, x3, x4 );
    std::size_t  numOperations  =  0;
    const d7cA::OctreeObj<d7cA::Point, double> * const  result  =  octree1.find( p, numOperations, tolerance );
    if ( nullptr != result )
    {
      ++count;
      printf( "p = ( %f, %f, %f, %f )\n", p.x1(), p.x2(), p.x3(), p.x4() );
      printf( "result = ( %f, %f, %f, %f )\n", result->info.x1(), result->info.x2(), result->info.x3(), result->info.x4() );
    }
  }

  printf( "count = %d\n", count );

  return  0;
}
