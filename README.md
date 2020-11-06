[![Build Status](https://travis-ci.org/uriheep/octree.didactic.svg?branch=main)](https://travis-ci.org/uriheep/octree.didactic)
[![Coverage Status](https://coveralls.io/repos/github/uriheep/octree.didactic/badge.svg)](https://coveralls.io/github/uriheep/octree.didactic)
# octree.didactic

<img align="right" src = "https://github.com/uriheep/octree.didactic/blob/dev/pics/octree_logo_github.png" width="300" height="300">
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

## Dependences:

1. `<algorithm>`
2. `<cstdint>`
3. `<cstdio>`

## Build:

1. `scons` to build all the code in this project ( including unit-tests, 
benchmark and examples );
2. to incorporate the *pseudo*-Octree into any project,
just `#include "octree.hpp"`;

## Example:

( see also `examples/src/` )

```javascript
#include "octree.hpp"

...

  // construct an Octree:
  d7cA::Octree<d7cA::Point, double>  octree1;
  octree1.init( arrElements, numElements, d7cA::comparePoints<double> );

  // count the number of randomly generated elements that fall within
  // the specified tolerance from the elements of the constructed 'octree1':
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
    std::size_t  numOperations  =  0; // counts the number of shifts it took to find 'p' in the octree
    const d7cA::OctreeObj<d7cA::Point, double> * const  result  =  octree1.find( p, numOperations, tolerance );
    if ( nullptr != result )
      ++count;
  }
```

## What a pseudo-octree is:

Consider a metric space *L1* over a 4-dimensional vector field ( *L1* may be regarded as [ Manhattan distance ]( https://en.wikipedia.org/wiki/Taxicab_geometry ) ).
An element on this space is a sequence of four numbers `<n1, n2, n3, n4>` that admit all types of interpretations, e.g.: 

1. `n1` may stand for a moment of time, while 
`n2, n3, n4` may be used as Cartesian coordinates of a point in a 3D space. 
Thus, a *pseudo*-Octree would represent a time-trajectory of a 3D point.

2. `n1` may stand for a moment of time and there might be multiple elements with the same value of `n1`.
Then, `n2, n3, n4` may be interpreted as Cartesian coordinates of points belonging to a 3D-body.


