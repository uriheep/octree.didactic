// This file is a part of the 'octree.didactic' project
//
// author : uri.heap  <u.heep@tutanota.com>
//
// license : GPL.v3+


#include <cstdio>
#include <cstdint>
#include <algorithm>

namespace  d7cA {

template<template<class> class  H, class  T>
class  OctreeObj {
  public:
    OctreeObj() : info(),
                  north( nullptr ),
		  south( nullptr ),
		  west( nullptr ),
		  east( nullptr ),
		  nw( nullptr ),
		  ne( nullptr ),
		  sw( nullptr ),
		  se( nullptr )
    { }
  public:
    H<T>       info;
    OctreeObj  *north;
    OctreeObj  *south;
    OctreeObj  *west;
    OctreeObj  *east;
    OctreeObj  *nw;
    OctreeObj  *ne;
    OctreeObj  *sw;
    OctreeObj  *se;
};

enum Direction : std::uint8_t {
  north, south, west, east, nw, ne, sw, se, north_south, west_east, nw_se, sw_ne
};

template<template<class> class  H, class T>
class  Octree {
  public:
    Octree();
    void  init( H<T> * const  arr, const long&  num, bool (*foo)( const H<T>&, const H<T>& ) ) noexcept;
    void  insert( const H<T>&  obj ) noexcept;
    ~Octree();
    const unsigned long&  getNumElements() const noexcept;
//    const H<T>&  getClosest( const Octree&, const T& ) const;
    OctreeObj<H, T> *  find( const H<T>&              ob,
                             std::size_t&             numOperations,
                             const T&                 tolerance = 0,
                             OctreeObj<H, T> * const  p = nullptr
                           ) const noexcept;
//    long  getNumOperationsFind( const H<T>&  ob, const T&  tolerance = 0 ) const noexcept;
  private:
    // copy-ctor and assignment operator are not allowed:
    Octree( const Octree& );
    Octree<H, T>&  operator=( const Octree<H, T>& );
  private:
    bool  checkEndNode_( const OctreeObj<H, T> * const ) const noexcept;
    void  getRootToEndNode_() noexcept;
    void  deleteEndNodes_() noexcept;
    void  print_( OctreeObj<H, T> *  p = nullptr ) const noexcept;
    OctreeObj<H, T> *  moveAlongNorthSouth_( const H<T>&,
                                             const T&,
                                             OctreeObj<H, T> * const,
                                             std::size_t&
                                           ) const noexcept;
    OctreeObj<H, T> *  moveAlongWestEast_( const H<T>&,
                                           const T&,
                                           OctreeObj<H, T> * const,
                                           std::size_t&
                                         ) const noexcept;
    OctreeObj<H, T> *  moveAlongNWSE_( const H<T>&,
                                       const T&,
                                       OctreeObj<H, T> * const,
                                       std::size_t&
                                     ) const noexcept;   
    OctreeObj<H, T> *  moveAlongSWNE_( const H<T>&,
                                       const T&,
                                       OctreeObj<H, T> * const,
                                       std::size_t&
                                     ) const noexcept;   
    /*
    OctreeObj<H, T> *  moveInDirection_( OctreeObj<H, T>   *p,
                            const H<T>&       ob,
                            const Direction&  direction,
                            const T&          tolerance,//  =  T(),
                            long&             numOperations
                          ) const noexcept;
    OctreeObj<H, T> *  multiMoveInDirection_( OctreeObj<H, T> *  p,
                                 const H<T>&        ob,
                                 const Direction&   direction,
                                 const T&           tolerance,//  =  T(),
                                 long&              numOperations
                               ) const noexcept;
*/
  private:
    OctreeObj<H, T>  *root_;
    unsigned long    numElements_;
};


template<template<class> class  H, class T>
Octree<H, T>::Octree() : root_( nullptr ), numElements_( 0 ) { }


template<template<class> class  H, class T>
void
Octree<H, T>::init( H<T> * const        arr,
                    const long&         num,
                    bool  (*comparePoints)( const H<T>&, const H<T>& ) 
                  ) noexcept
{
  if ( nullptr == arr
    || 0 >= num
     )
    return;

  if ( nullptr != comparePoints )
    std::sort( arr, arr + num, comparePoints );
  for ( long i = 0; i < num; ++i )
  {
    insert( arr[ i ] ); 
  }
  // balance the tree along the North-South direction:
  {
    std::size_t  numElementsInLine  =  0;
    OctreeObj<H, T>  *pTmp  =  root_;
    while ( nullptr != pTmp->south )
      pTmp  =  pTmp->south;
    while ( nullptr != pTmp->north )
    {
      pTmp  =  pTmp->north;
      ++numElementsInLine;
    }
    for ( std::size_t i = 0; i < static_cast<std::size_t>( numElementsInLine / 2 ); ++i )
    {
      root_  =  root_->north;
    }
  }
}


template<template<class> class  H, class T>
void
Octree<H, T>::insert( const H<T>&  obj ) noexcept
{
  if ( nullptr == root_ )
  {
    root_  =  new OctreeObj<H, T >;
    root_->info  =  obj;
    ++numElements_;
    return;
  }

//  OctreeObj<H, T>  *pFound  =  find( obj );

//  if ( nullptr != pFound )
//    return; // no dublicates allowed

  OctreeObj<H, T>  *pTmp  =  root_;

  const T&  x1  =  obj.x1();
  const T&  x2  =  obj.x2();
  const T&  x3  =  obj.x3();
  const T&  x4  =  obj.x4();

  while ( nullptr != pTmp->north
       && x1 >= pTmp->info.x1()
        )
  {
    pTmp  =  pTmp->north;
  }

  while ( nullptr != pTmp->south
       && x1 < pTmp->info.x1()
        )
  {
    pTmp  =  pTmp->south;
  }

  while ( nullptr != pTmp->west
       && x2 >= pTmp->info.x2()
       && x1 == pTmp->info.x1()
        )
  {
    pTmp  =  pTmp->west;
  }

  while ( nullptr != pTmp->east
       && x2 < pTmp->info.x2()
       && x1 == pTmp->info.x1()
        )
  {
    pTmp  =  pTmp->east;
  }

  while ( nullptr != pTmp->nw
       && x3 >= pTmp->info.x3()
       && x2 == pTmp->info.x2()
       && x1 == pTmp->info.x1()
        )
  {
    pTmp  =  pTmp->nw;
  }

  while ( nullptr != pTmp->se
       && x3 < pTmp->info.x3()
       && x2 == pTmp->info.x2()
       && x1 == pTmp->info.x1()
        )
  {
    pTmp  =  pTmp->se;
  }

  while ( nullptr != pTmp->sw
       && x4 >= pTmp->info.x4()
       && x1 == pTmp->info.x1()
       && x2 == pTmp->info.x2()
       && x3 == pTmp->info.x3()
        )
  {
    pTmp  =  pTmp->sw;
  }

  while ( nullptr != pTmp->ne
       && x4 < pTmp->info.x4()
       && x1 == pTmp->info.x1()
       && x2 == pTmp->info.x2()
       && x3 == pTmp->info.x3()
        )
  {
    pTmp  =  pTmp->ne;
  }

  if ( nullptr == pTmp->sw
    && x4 >= pTmp->info.x4()
    && x1 == pTmp->info.x1()
    && x2 == pTmp->info.x2()
    && x3 == pTmp->info.x3()
     )
  {
    pTmp->sw  =  new OctreeObj<H, T>;
    pTmp->sw->info  =  obj;
    pTmp->sw->ne  =  pTmp;
    ++numElements_;
    return;
  }

  if ( nullptr == pTmp->ne
    && x4 < pTmp->info.x4()
    && x1 == pTmp->info.x1()
    && x2 == pTmp->info.x2()
    && x3 == pTmp->info.x3()
     )
  {
    pTmp->ne  =  new OctreeObj<H, T>;
    pTmp->ne->info  =  obj;
    pTmp->ne->sw  =  pTmp;
    ++numElements_;
    return;
  }

  if ( nullptr == pTmp->nw
    && x3 >= pTmp->info.x3()
    && x1 == pTmp->info.x1()
    && x2 == pTmp->info.x2()
     )
  {
    pTmp->nw  =  new OctreeObj<H, T>;
    pTmp->nw->info  =  obj;
    pTmp->nw->se  =  pTmp;
    ++numElements_;
    return;
  }

  if ( nullptr == pTmp->se
    && x3 < pTmp->info.x3()
    && x1 == pTmp->info.x1()
    && x2 == pTmp->info.x2()
     )
  {
    pTmp->se  =  new OctreeObj<H, T>;
    pTmp->se->info  =  obj;
    pTmp->se->nw  =  pTmp;
    ++numElements_;
    return;
  }

  if ( nullptr == pTmp->west
    && x2 >= pTmp->info.x2()
    && x1 == pTmp->info.x1()
     )
  {
    pTmp->west  =  new OctreeObj<H, T>;
    pTmp->west->info  =  obj;
    pTmp->west->east  =  pTmp;
    ++numElements_;
    return;
  }

  if ( nullptr == pTmp->east
    && x2 < pTmp->info.x2()
    && x1 == pTmp->info.x1()
     )
  {
    pTmp->east  =  new OctreeObj<H, T>;
    pTmp->east->info  =  obj;
    pTmp->east->west  =  pTmp;
    ++numElements_;
    return;
  }

  if ( nullptr == pTmp->north
    && x1 >= pTmp->info.x1()
     )
  {
    pTmp->north  =  new OctreeObj<H, T>;
    pTmp->north->info  =  obj;
    pTmp->north->south  =  pTmp;
    ++numElements_;
    return;
  }

  if ( nullptr == pTmp->south
    && x1 < pTmp->info.x1()
     )
  {
    pTmp->south  =  new OctreeObj<H, T>;
    pTmp->south->info  =  obj;
    pTmp->south->north  =  pTmp;
    ++numElements_;
    return;
  }

  pTmp  =  nullptr;
}


template<template<class> class  H, class T>
Octree<H, T>::~Octree()
{
  if ( nullptr == root_ )
    return;

  while ( nullptr != root_ )
  {
    getRootToEndNode_();
    deleteEndNodes_();

    if ( nullptr == root_->north
      && nullptr == root_->south
      && nullptr == root_->west
      && nullptr == root_->east
      && nullptr == root_->nw
      && nullptr == root_->ne
      && nullptr == root_->sw
      && nullptr == root_->se
       )
    {
      delete  root_;
      root_  =  nullptr;
    }
  } // while ( nullptr != root_ )
}


template<template<class> class  H, class T>
const unsigned long&
Octree<H, T>::getNumElements() const noexcept
{
  return  numElements_;
}


template<template<class> class  H, class T>
OctreeObj<H, T> *
Octree<H, T>::moveAlongNorthSouth_( const H<T>&              ob,
                                    const T&                 tolerance,
                                    OctreeObj<H, T> * const  pTmpIn,
                                    std::size_t&             numOperations
                                  ) const noexcept
{
  if ( nullptr == pTmpIn )
    return  nullptr;

  OctreeObj<H, T> *  pTmp  =  pTmpIn;
  if ( nullptr != pTmp->north
    && nullptr != pTmp->south
     )
  {
    if ( std::abs( ob.x1() - pTmp->south->info.x1() ) > std::abs( ob.x1() - pTmp->north->info.x1() ) )
    {
      pTmp  =  pTmp->north;
      ++numOperations;
      OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
      if ( nullptr != pTmp1 )
        return  pTmp1;
      return  pTmp;
    }
    if ( std::abs( ob.x1() - pTmp->south->info.x1() ) < std::abs( ob.x1() - pTmp->north->info.x1() ) )
    {
      pTmp  =  pTmp->south;
      ++numOperations;
      OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
      if ( nullptr != pTmp1 )
        return  pTmp1;
      return  pTmp;
    }
    if ( std::abs( ob.x1() - pTmp->south->info.x1() ) == std::abs( ob.x1() - pTmp->north->info.x1() ) )
    {
      pTmp  =  pTmp->south;
      ++numOperations;
      OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
      if ( nullptr != pTmp1 )
        pTmp  =  pTmp1;
      const T  distTmp1  =  dist2( ob, pTmp->info );
      if ( distTmp1 <= 16 * tolerance * tolerance )
        return  pTmp;
      // ******
      pTmp  =  pTmpIn;
      pTmp  =  pTmp->north;
      ++numOperations;
      OctreeObj<H, T> * const  pTmp2  =  find( ob, numOperations, tolerance, pTmp );
      if ( nullptr != pTmp2 )
        pTmp  =  pTmp2;
      const T  distTmp2  =  dist2( ob, pTmp->info );
      if ( distTmp2 <= 16 * tolerance * tolerance )
        return  pTmp;

      return  distTmp1 <= distTmp2 ? ( nullptr == pTmp1 ? pTmpIn : pTmp1 ) : ( nullptr == pTmp2 ? pTmpIn : pTmp2 );
    }
  }
  if ( nullptr == pTmp->north
    && nullptr != pTmp->south
    && std::abs( ob.x1() - pTmp->info.x1() ) > std::abs( ob.x1() - pTmp->south->info.x1() )
     )
  {
    pTmp  =  pTmp->south;
    ++numOperations;
    OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
    if ( nullptr != pTmp1 )
      return  pTmp1;
    return  pTmp;
  }
  else
    return  pTmp; // not found
  if ( nullptr != pTmp->north
    && nullptr == pTmp->south
    && std::abs( ob.x1() - pTmp->info.x1() ) > std::abs( ob.x1() - pTmp->north->info.x1() )
     )
  {
    pTmp  =  pTmp->north;
    ++numOperations;
    OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
    if ( nullptr != pTmp1 )
      return  pTmp1;
    return  pTmp;
  }
  else
    return  pTmp; // not found
  return  pTmp; // not found
}


template<template<class> class  H, class T>
OctreeObj<H, T> *
Octree<H, T>::moveAlongWestEast_( const H<T>&              ob,
                                  const T&                 tolerance,
                                  OctreeObj<H, T> * const  pTmpIn,
                                  std::size_t&             numOperations
                                ) const noexcept
{
  if ( nullptr == pTmpIn )
    return  nullptr;

  OctreeObj<H, T> *  pTmp  =  pTmpIn;
  if ( nullptr != pTmp->west
    && nullptr != pTmp->east
     )
  {
    if ( std::abs( ob.x1() - pTmp->east->info.x1() ) > std::abs( ob.x1() - pTmp->west->info.x1() ) )
    {
      pTmp  =  pTmp->west;
      ++numOperations;
      OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
      if ( nullptr != pTmp1 )
        return  pTmp1;
      return  pTmp;
    }
    if ( std::abs( ob.x1() - pTmp->east->info.x1() ) < std::abs( ob.x1() - pTmp->west->info.x1() ) )
    {
      pTmp  =  pTmp->east;
      ++numOperations;
      OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
      if ( nullptr != pTmp1 )
        return  pTmp1;
      return  pTmp;
    }
    if ( std::abs( ob.x1() - pTmp->east->info.x1() ) == std::abs( ob.x1() - pTmp->west->info.x1() ) )
    {
      pTmp  =  pTmp->east;
      ++numOperations;
      OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
      if ( nullptr != pTmp1 )
        pTmp  =  pTmp1;
      const T  distTmp1  =  dist2( ob, pTmp->info );
      if ( distTmp1 <= 16 * tolerance * tolerance )
        return  pTmp;
      // ******
      pTmp  =  pTmpIn;
      pTmp  =  pTmp->west;
      ++numOperations;
      OctreeObj<H, T> * const  pTmp2  =  find( ob, numOperations, tolerance, pTmp );
      if ( nullptr != pTmp2 )
        pTmp  =  pTmp2;
      const T  distTmp2  =  dist2( ob, pTmp->info );
      if ( distTmp2 <= 16 * tolerance * tolerance )
        return  pTmp;

      return  distTmp1 <= distTmp2 ? ( nullptr == pTmp1 ? pTmpIn : pTmp1 ) : ( nullptr == pTmp2 ? pTmpIn : pTmp2 );
    }
  }
  if ( nullptr == pTmp->west
    && nullptr != pTmp->east
    && std::abs( ob.x1() - pTmp->info.x1() ) > std::abs( ob.x1() - pTmp->east->info.x1() )
     )
  {
    pTmp  =  pTmp->east;
    ++numOperations;
    OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
    if ( nullptr != pTmp1 )
      return  pTmp1;
    return  pTmp;
  }
  else
    return  pTmp; // not found
  if ( nullptr != pTmp->west
    && nullptr == pTmp->east
    && std::abs( ob.x1() - pTmp->info.x1() ) > std::abs( ob.x1() - pTmp->west->info.x1() )
     )
  {
    pTmp  =  pTmp->west;
    ++numOperations;
    OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
    if ( nullptr != pTmp1 )
      return  pTmp1;
    return  pTmp;
  }
  else
    return  pTmp; // not found
  return  pTmp; // not found
}


template<template<class> class  H, class T>
OctreeObj<H, T> *
Octree<H, T>::moveAlongNWSE_( const H<T>&              ob,
                              const T&                 tolerance,
                              OctreeObj<H, T> * const  pTmpIn,
                              std::size_t&             numOperations
                            ) const noexcept
{
  if ( nullptr == pTmpIn )
    return  nullptr;

  OctreeObj<H, T> *  pTmp  =  pTmpIn;
  if ( nullptr != pTmp->nw
    && nullptr != pTmp->se
     )
  {
    if ( std::abs( ob.x1() - pTmp->se->info.x1() ) > std::abs( ob.x1() - pTmp->nw->info.x1() ) )
    {
      pTmp  =  pTmp->nw;
      ++numOperations;
      OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
      if ( nullptr != pTmp1 )
        return  pTmp1;
      return  pTmp;
    }
    if ( std::abs( ob.x1() - pTmp->se->info.x1() ) < std::abs( ob.x1() - pTmp->nw->info.x1() ) )
    {
      pTmp  =  pTmp->se;
      ++numOperations;
      OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
      if ( nullptr != pTmp1 )
        return  pTmp1;
      return  pTmp;
    }
    if ( std::abs( ob.x1() - pTmp->se->info.x1() ) == std::abs( ob.x1() - pTmp->nw->info.x1() ) )
    {
      pTmp  =  pTmp->se;
      ++numOperations;
      OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
      if ( nullptr != pTmp1 )
        pTmp  =  pTmp1;
      const T  distTmp1  =  dist2( ob, pTmp->info );
      if ( distTmp1 <= 16 * tolerance * tolerance )
        return  pTmp;
      // ******
      pTmp  =  pTmpIn;
      pTmp  =  pTmp->nw;
      ++numOperations;
      OctreeObj<H, T> * const  pTmp2  =  find( ob, numOperations, tolerance, pTmp );
      if ( nullptr != pTmp2 )
        pTmp  =  pTmp2;
      const T  distTmp2  =  dist2( ob, pTmp->info );
      if ( distTmp2 <= 16 * tolerance * tolerance )
        return  pTmp;

      return  distTmp1 <= distTmp2 ? ( nullptr == pTmp1 ? pTmpIn : pTmp1 ) : ( nullptr == pTmp2 ? pTmpIn : pTmp2 );
    }
  }
  if ( nullptr == pTmp->nw
    && nullptr != pTmp->se
    && std::abs( ob.x1() - pTmp->info.x1() ) > std::abs( ob.x1() - pTmp->se->info.x1() )
     )
  {
    pTmp  =  pTmp->se;
    ++numOperations;
    OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
    if ( nullptr != pTmp1 )
      return  pTmp1;
    return  pTmp;
  }
  else
    return  pTmp; // not found
  if ( nullptr != pTmp->nw
    && nullptr == pTmp->se
    && std::abs( ob.x1() - pTmp->info.x1() ) > std::abs( ob.x1() - pTmp->nw->info.x1() )
     )
  {
    pTmp  =  pTmp->nw;
    ++numOperations;
    OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
    if ( nullptr != pTmp1 )
      return  pTmp1;
    return  pTmp;
  }
  else
    return  pTmp; // not found
  return  pTmp; // not found
}


template<template<class> class  H, class T>
OctreeObj<H, T> *
Octree<H, T>::moveAlongSWNE_( const H<T>&              ob,
                              const T&                 tolerance,
                              OctreeObj<H, T> * const  pTmpIn,
                              std::size_t&             numOperations
                            ) const noexcept
{
  if ( nullptr == pTmpIn )
    return  nullptr;

  OctreeObj<H, T> *  pTmp  =  pTmpIn;
  if ( nullptr != pTmp->sw
    && nullptr != pTmp->ne
     )
  {
    if ( std::abs( ob.x1() - pTmp->ne->info.x1() ) > std::abs( ob.x1() - pTmp->sw->info.x1() ) )
    {
      pTmp  =  pTmp->sw;
      ++numOperations;
      OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
      if ( nullptr != pTmp1 )
        return  pTmp1;
      return  pTmp;
    }
    if ( std::abs( ob.x1() - pTmp->ne->info.x1() ) < std::abs( ob.x1() - pTmp->sw->info.x1() ) )
    {
      pTmp  =  pTmp->ne;
      ++numOperations;
      OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
      if ( nullptr != pTmp1 )
        return  pTmp1;
      return  pTmp;
    }
    if ( std::abs( ob.x1() - pTmp->ne->info.x1() ) == std::abs( ob.x1() - pTmp->sw->info.x1() ) )
    {
      pTmp  =  pTmp->ne;
      ++numOperations;
      OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
      if ( nullptr != pTmp1 )
        pTmp  =  pTmp1;
      const T  distTmp1  =  dist2( ob, pTmp->info );
      if ( distTmp1 <= 16 * tolerance * tolerance )
        return  pTmp;
      // ******
      pTmp  =  pTmpIn;
      pTmp  =  pTmp->sw;
      ++numOperations;
      OctreeObj<H, T> * const  pTmp2  =  find( ob, numOperations, tolerance, pTmp );
      if ( nullptr != pTmp2 )
        pTmp  =  pTmp2;
      const T  distTmp2  =  dist2( ob, pTmp->info );
      if ( distTmp2 <= 16 * tolerance * tolerance )
        return  pTmp;

      return  distTmp1 <= distTmp2 ? ( nullptr == pTmp1 ? pTmpIn : pTmp1 ) : ( nullptr == pTmp2 ? pTmpIn : pTmp2 );
    }
  }
  if ( nullptr == pTmp->sw
    && nullptr != pTmp->ne
    && std::abs( ob.x1() - pTmp->info.x1() ) > std::abs( ob.x1() - pTmp->ne->info.x1() )
     )
  {
    pTmp  =  pTmp->ne;
    ++numOperations;
    OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
    if ( nullptr != pTmp1 )
      return  pTmp1;
    return  pTmp;
  }
  else
    return  pTmp; // not found
  if ( nullptr != pTmp->sw
    && nullptr == pTmp->ne
    && std::abs( ob.x1() - pTmp->info.x1() ) > std::abs( ob.x1() - pTmp->sw->info.x1() )
     )
  {
    pTmp  =  pTmp->sw;
    ++numOperations;
    OctreeObj<H, T> * const  pTmp1  =  find( ob, numOperations, tolerance, pTmp );
    if ( nullptr != pTmp1 )
      return  pTmp1;
    return  pTmp;
  }
  else
    return  pTmp; // not found
  return  pTmp; // not found
}


template<template<class> class  H, class T>
OctreeObj<H, T> *
Octree<H, T>::find( const H<T>&              ob,
                    std::size_t&             numOperations,
                    const T&                 tolerance,
                    OctreeObj<H, T> * const  pInit
                  ) const noexcept
{
  OctreeObj<H, T> * pTmp  =  pInit;
  if ( nullptr == pInit )
    pTmp  =  root_;

  if ( nullptr == root_ )
    return  root_; // not found

  const T  dist  =  dist2( ob, pTmp->info );
  if ( dist <= 16 * tolerance * tolerance )
    return  pTmp;

  if ( nullptr != pTmp
    && tolerance >= std::abs( ob.x1() - pTmp->info.x1() )
     )
  {
    if ( nullptr != pTmp
      && tolerance >= std::abs( ob.x2() - pTmp->info.x2() )
       )
    {
      if ( nullptr != pTmp
        && tolerance >= std::abs( ob.x3() - pTmp->info.x3() )
         )
      {
        if ( nullptr != pTmp
          && tolerance >= std::abs( ob.x4() - pTmp->info.x4() )
           )
          return  pTmp;
        else
          {
            OctreeObj<H, T> * const  pTmp4  =  moveAlongSWNE_( ob, tolerance, pTmp, numOperations );
            if ( nullptr != pTmp4 )
              pTmp  =  pTmp4;
            //const T  dist  =  dist2( ob, pTmp->info );
            //if ( dist <= 16 * tolerance * tolerance )
              return  pTmp;
          }
      }
      else
        {
          OctreeObj<H, T> * const  pTmp3  =  moveAlongNWSE_( ob, tolerance, pTmp, numOperations );
          if ( nullptr != pTmp3 )
            pTmp  =  pTmp3;
          //const T  dist  =  dist2( ob, pTmp->info );
          //if ( dist <= 16 * tolerance * tolerance )
            return  pTmp;
	}
    }
    else
      {
        OctreeObj<H, T> * const  pTmp2  =  moveAlongWestEast_( ob, tolerance, pTmp, numOperations );
        if ( nullptr != pTmp2 )
	  pTmp  =  pTmp2;
        //const T  dist  =  dist2( ob, pTmp->info );
        //if ( dist <= 16 * tolerance * tolerance )
          return  pTmp;
      }
  }
  else
    {
      OctreeObj<H, T> * const  pTmp1  =  moveAlongNorthSouth_( ob, tolerance, pTmp, numOperations ); 
      if ( nullptr != pTmp1 )
        pTmp  =  pTmp1;
      //const T  dist  =  dist2( ob, pTmp->info );
      //if ( dist <= 16 * tolerance * tolerance )
        return  pTmp;
    }

  return  nullptr; // not found
}


template<template<class> class  H, class T>
bool
Octree<H, T>::checkEndNode_( const OctreeObj<H, T> * const  p ) const noexcept
{
  if ( nullptr == p )
    return  true;

  if ( nullptr != p->north
    && nullptr == p->south
    && nullptr == p->west
    && nullptr == p->east
    && nullptr == p->nw
    && nullptr == p->ne
    && nullptr == p->sw
    && nullptr == p->se
     )
    return  true;

  if ( nullptr == p->north
    && nullptr != p->south
    && nullptr == p->west
    && nullptr == p->east
    && nullptr == p->nw
    && nullptr == p->ne
    && nullptr == p->sw
    && nullptr == p->se
     )
    return  true;

  if ( nullptr == p->north
    && nullptr == p->south
    && nullptr != p->west
    && nullptr == p->east
    && nullptr == p->nw
    && nullptr == p->ne
    && nullptr == p->sw
    && nullptr == p->se
     )
    return  true;

  if ( nullptr == p->north
    && nullptr == p->south
    && nullptr == p->west
    && nullptr != p->east
    && nullptr == p->nw
    && nullptr == p->ne
    && nullptr == p->sw
    && nullptr == p->se
     )
    return  true;

  if ( nullptr == p->north
    && nullptr == p->south
    && nullptr == p->west
    && nullptr == p->east
    && nullptr != p->nw
    && nullptr == p->ne
    && nullptr == p->sw
    && nullptr == p->se
     )
    return  true;

  if ( nullptr == p->north
    && nullptr == p->south
    && nullptr == p->west
    && nullptr == p->east
    && nullptr == p->nw
    && nullptr != p->ne
    && nullptr == p->sw
    && nullptr == p->se
     )
    return  true;

  if ( nullptr == p->north
    && nullptr == p->south
    && nullptr == p->west
    && nullptr == p->east
    && nullptr == p->nw
    && nullptr == p->ne
    && nullptr != p->sw
    && nullptr == p->se
     )
    return  true;

  if ( nullptr == p->north
    && nullptr == p->south
    && nullptr == p->west
    && nullptr == p->east
    && nullptr == p->nw
    && nullptr == p->ne
    && nullptr == p->sw
    && nullptr != p->se
     )
    return  true;

  return  false;
}


template<template<class> class  H, class T>
void
Octree<H, T>::getRootToEndNode_() noexcept
{
    OctreeObj<H, T> * pPrev  =  root_;
    while ( false == checkEndNode_( root_ ) )
    {
      if ( nullptr != root_->north
	&& pPrev != root_->north
         )
      {
	pPrev  =  root_;
        root_->north->south  =  root_;
        root_  =  root_->north;
        continue;
      }
      if ( nullptr != root_->south
	&& pPrev != root_->south
         )
      {
	pPrev  =  root_;
        root_->south->north  =  root_;
        root_  =  root_->south;
        continue;
      }
      if ( nullptr != root_->west
	&& pPrev != root_->west
         )
      {
	pPrev  =  root_;
        root_->west->east  =  root_;
        root_  =  root_->west;
        continue;
      }
      if ( nullptr != root_->east
	&& pPrev != root_->east
         )
      {
	pPrev  =  root_;
        root_->east->west  =  root_;
        root_  =  root_->east;
        continue;
      }
      if ( nullptr != root_->nw
	&& pPrev != root_->nw
         )
      {
	pPrev  =  root_;
        root_->nw->se  =  root_;
        root_  =  root_->nw;
        continue;
      }
      if ( nullptr != root_->ne
	&& pPrev != root_->ne
         )
      {
	pPrev  =  root_;
        root_->ne->sw  =  root_;
        root_  =  root_->ne;
        continue;
      }
      if ( nullptr != root_->sw
	&& pPrev != root_->sw
         )
      {
	pPrev  =  root_;
        root_->sw->ne  =  root_;
        root_  =  root_->sw;
        continue;
      }
      if ( nullptr != root_->se
	&& pPrev != root_->se
         )
      {
	pPrev  =  root_;
        root_->se->nw  =  root_;
        root_  =  root_->se;
        continue;
      }
    } // white ()
}

template<template<class> class  H, class T>
void
Octree<H, T>::deleteEndNodes_() noexcept
{
    // if the tree is linear / chain:
    while ( true == checkEndNode_( root_ )
	 && nullptr != root_
	  )
    {
      if ( nullptr != root_->north
	&& true == checkEndNode_( root_->south )
         )
      {
        root_  =  root_->north;
        delete  root_->south;
        root_->south  =  nullptr;
        if ( false == checkEndNode_( root_ ) )
          return;
      }
      if ( nullptr != root_->south
	&& true == checkEndNode_( root_->north )
         )
      {
        root_  =  root_->south;
        delete  root_->north;
        root_->north  =  nullptr;
	if ( false == checkEndNode_( root_ ) )
          return;
      }
      if ( nullptr != root_->west
        && true == checkEndNode_( root_->east )
         )
      {
        root_  =  root_->west;
        delete  root_->east;
        root_->east  =  nullptr;
	if ( false == checkEndNode_( root_ ) )
          return;
      }
      if ( nullptr != root_->east
	&& true == checkEndNode_( root_->west )
         )
      {
        root_  =  root_->east;
        delete  root_->west;
        root_->west  =  nullptr;
	if ( false == checkEndNode_( root_ ) )
          return;
      }
      if ( nullptr != root_->nw
	&& true == checkEndNode_( root_->se )
         )
      {
        root_  =  root_->nw;
        delete  root_->se;
        root_->se  =  nullptr;
	if ( false == checkEndNode_( root_ ) )
          return;
      }
      if ( nullptr != root_->ne
	&& true == checkEndNode_( root_->sw )
         )
      {
        root_  =  root_->ne;
        delete  root_->sw;
        root_->sw  =  nullptr;
	if ( false == checkEndNode_( root_ ) )
          return;
      }
      if ( nullptr != root_->sw
	&& true == checkEndNode_( root_->ne )
         )
      {
        root_  =  root_->sw;
        delete  root_->ne;
        root_->ne  =  nullptr;
	if ( false == checkEndNode_( root_ ) )
          return;
      }
      if ( nullptr != root_->se
	&& true == checkEndNode_( root_->nw )
         )
      {
        root_  =  root_->se;
        delete  root_->nw;
        root_->nw  =  nullptr;
	if ( false == checkEndNode_( root_ ) )
          return;
      }
    } // while ()
}



template<template<class> class  H, class T>
void
Octree<H, T>::print_( OctreeObj<H, T> *  p ) const noexcept
{
  if ( nullptr == p )
    p  =  root_;
  if ( nullptr == root_ )
    return;

  printf( "%p\n", (void*)p->north );
  printf( "%p\n", (void*)p->south );
  printf( "%p\n", (void*)p->west );
  printf( "%p\n", (void*)p->east );
  printf( "%p\n", (void*)p->nw );
  printf( "%p\n", (void*)p->ne );
  printf( "%p\n", (void*)p->sw );
  printf( "%p\n", (void*)p->se );
  printf( "p = %f\t%f\t%f\t%f\n", p->info.x1(), p->info.x2(), p->info.x3(), p->info.x4() );
  if ( nullptr != p->north )
    printf( "north = %f\n", p->north->info.x1() );
  if ( nullptr != p->south )
    printf( "south = %f\n", p->south->info.x1() );
  if ( nullptr != p->west )
    printf( "west = %f\n", p->west->info.x1() );
  if ( nullptr != p->east )
    printf( "east = %f\n", p->east->info.x1() );
  if ( nullptr != p->nw )
    printf( "nw = %f\n", p->nw->info.x1() );
  if ( nullptr != p->se )
    printf( "se = %f\n", p->se->info.x1() );
  if ( nullptr != p->sw )
    printf( "sw = %f\n", p->sw->info.x1() );
  if ( nullptr != p->ne )
    printf( "ne = %f\n", p->ne->info.x1() );
}

template<class T>
struct  Point {
  public:
    Point() : x1_( 0 ), x2_( 0 ), x3_( 0 ), x4_( 0 ) { }
    Point( const T&  xIn,
           const T&  yIn,
	   const T&  zIn,
	   const T&  tIn
         ) : x1_( xIn ), x2_( yIn ), x3_( zIn ), x4_( tIn ) { }
    T  x1() const { return  x1_; }
    T  x2() const { return  x2_; }
    T  x3() const { return  x3_; }
    T  x4() const { return  x4_; }
    Point&  operator=( const Point&  that ) {
      if ( &that == this )
       return  *this;
      x1_  =  that.x1();
      x2_  =  that.x2();
      x3_  =  that.x3();
      x4_  =  that.x4();
      return  *this;
    }
    friend bool  operator==( const Point&  a, const Point&  b ) noexcept
    {
      if ( a.x1() == b.x1()
        && a.x2() == b.x2()
        && a.x3() == b.x3()
        && a.x4() == b.x4()
         )
      {
        return  true;
      }
      return  false;
    }
    friend T  dist2( const Point&  a, const Point&  b ) noexcept
    { 
      return  ( ( a.x1() - b.x1() ) * ( a.x1() - b.x1() )
              + ( a.x2() - b.x2() ) * ( a.x2() - b.x2() )
              + ( a.x3() - b.x3() ) * ( a.x3() - b.x3() )
              + ( a.x4() - b.x4() ) * ( a.x4() - b.x4() ) );
    }
  private:
    T  x1_, x2_, x3_, x4_;
};

template<class T, std::size_t N>
struct  PointLess {
    bool  operator()( const Point<T>&  a, const Point<T>&  b ) const noexcept
    {
      if ( 0 == N )  return  a.x1() < b.x1() ?  true : false;
      if ( 1 == N )  return  a.x2() < b.x2() ?  true : false;
      if ( 2 == N )  return  a.x3() < b.x3() ?  true : false;
      if ( 3 == N )  return  a.x4() < b.x4() ?  true : false;
      return  false;
    }
};

template<class T>
bool
comparePoints( const Point<T>&  a, const Point<T>&  b ) noexcept
{
  if ( a.x1() < b.x1() )  return  true;
  if ( a.x1() == b.x1()
    && a.x2() < b.x2() )  return  true;
  if ( a.x1() == b.x1()
    && a.x2() == b.x2()
    && a.x3() < b.x3() )  return  true;
  if ( a.x1() == b.x1()
    && a.x2() == b.x2()
    && a.x3() == b.x3()
    && a.x4() < b.x4() )  return  true;
  return  false;
}

} // namespace  7dcA
