# octree.didactic

An implementation of a *pseudo*-Octree
with the focus on clear and comprehensible code.
Intended to serve demonstrative and didactic purposes.
The whole implementation is a single header file: `include/octree.hpp`
*( see an example below on how to use it )*

## Benchmarking

Benchmarking of the implementation is performed
with respect to *C++ STL* `std::find_if()` and `std::search()`.

![benchmark](https://github.com/uriheep/octree.didactic/blob/dev/benchmark/benchmark.png?raw=true)

Each point on the plots represents a mean time value calculated for a set of
1,000 runs. Each run comprises a search over a set of randomly generated
elements. Numbers of the randomly generated elements are given by the horizontal axis.
The full benchmarking set-up can be found in `benchmark/src/main.cpp`

**Benchmarking results:**

1. the presented pseudo-Octree implementation outperforms
   both `std::find_if` and `std::search`
   when `std::find_if` and `std::search` are compiled without optimizations flags;

2. `std::find_if` and `std::search` outperform
   the presented implementation when:
   i. compiled with the optimization flags;
   AND
   ii. applied to a sorted array;

**Conclusion:**

The presented Octree implementation should be employed if
any of the following conditions are true:

1. new elements might be inserted into a search space;
2. the values of elements of the search space are changed in an affine-like manner;


## Example:

( see also `examples/src/` )

```javascript
#include "octree.hpp"

#include <random>
#include <cstdio>

int  main() {

  // generate a set of random elements to create an Octree:
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
      ++count;
  }

  printf( "count = %d\n", count );

  return  0;
}
```

What a pseudo-octree is:
