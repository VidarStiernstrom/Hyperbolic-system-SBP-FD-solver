#pragma once

#include<petscsystypes.h>
#include <tuple>

namespace sbp {
  /**
  * Central first derivative SBP operator. The class holds the stencil weights for the interior
  * stencil and the closure stencils and contains method for applying the operator to a grid function
  * vector. The stencils of the operator are all declared at compile time, which (hopefully) should
  * allow the compiler to perform extensive optimization on the apply methods.
  **/
  template <PetscInt interior_width, PetscInt n_closures, PetscInt closure_width>
  class D1_central{
  private:
    // Stencils defining the operator. The stencils are declared at compile time
    const PetscScalar interior_stencil[interior_width];
    const PetscScalar closure_stencils[n_closures][closure_width];

  public:
    constexpr D1_central(); //See implementations for specific stencils
    // TODO: Figure out how to pass static arrays holding the stencils at construction.
    // Then could have a single templated constructor initializing the members from the arguments
    // and the specific operators (of a given order) would be nicely defined in 
    // make_diff_ops.h.
    // constexpr D1_central(const PetscScalar (*i_s)[interior_width], const PetscScalar (**c_s)[n_closures][closure_width]);

    /**
    * Convenience function returning the ranges (interior_width,n_closures,closure_width)
    **/
    inline constexpr std::tuple<PetscInt,PetscInt,PetscInt> get_ranges() const
    {
      return std::make_tuple(interior_width,n_closures,closure_width);
    };

    //TODO: The pointer to pointer layout may prevent compiler optimization for e.g vectorization since it is not clear whether
    //      the memory is contiguous or not. We should switch to a flat array layout, once we get something running for systems in 2D.
    
    //=============================================================================
    // 1D functions
    //=============================================================================

    /**
    * Computes the derivative in x-direction of a multicomponent 1D grid function v[i][comp] for an index i within the set of left closure points.
    * Input:  v     - Multicomponent 1D grid function v (typically obtained via DMDAVecGetArrayDOF)
    *         hi    - inverse grid spacing
    *         i     - Grid index in x-direction. Index must be within the set of left closure points
    *         comp  - grid function component.
    *
    * Output: derivative v_x[i][comp]
    **/
    inline PetscScalar apply_left(const PetscScalar *const *const v, const PetscScalar hi, const PetscInt i, const PetscInt comp) const
    {
      PetscScalar u = 0;
      for (PetscInt is = 0; is<closure_width; is++)
      {
        u += closure_stencils[i][is]*v[is][comp];
      }
      return hi*u;
    };

    /**
    * Computes the derivative in x-direction of a multicomponent 1D grid function v[i][comp] for an index i within the set of interior points.
    * Input:  v     - Multicomponent 1D grid function v (typically obtained via DMDAVecGetArrayDOF)
    *         hi    - inverse grid spacing
    *         i     - Grid index in x-direction. Index must be within the set of interior points
    *         comp  - grid function component.
    *
    * Output: derivative v_x[i][comp]
    **/
    inline PetscScalar apply_interior(const PetscScalar *const *const v, const PetscScalar hi, const PetscInt i, const PetscInt comp) const
    {
      PetscScalar u = 0;
      for (PetscInt is = 0; is<interior_width; is++)
      {
        u += interior_stencil[is]*v[(i-(interior_width-1)/2+is)][comp];
      }
      return hi*u;
    };

    /**
    * Computes the derivative in x-direction of a multicomponent 1D grid function v[i][comp] for an index i within the set of right closure points.
    * Input:  v     - Multicomponent 1D grid function v (typically obtained via DMDAVecGetArrayDOF)
    *         hi    - inverse grid spacing
    *         i     - Grid index in x-direction. Index must be within the set of right closure points.
    *         comp  - grid function component.
    *
    * Output: derivative v_x[i][comp]
    **/
    inline PetscScalar apply_right(const PetscScalar *const *const v, const PetscScalar hi, const PetscInt N, const PetscInt i, const PetscInt comp) const
    {
      PetscScalar u = 0;
      for (PetscInt is = 0; is < closure_width; is++)
      {
        u -= closure_stencils[N-i-1][closure_width-is-1]*v[(N-closure_width+is)][comp];
      }
      return hi*u;
    };

    //=============================================================================
    // 2D functions
    //=============================================================================

    /**
    * Computes the derivative in x-direction of a multicomponent 2D grid function v[j][i][comp] for an index i within the set of left closure points.
    * Input:
    *
    * Output:
    * Derivative v_x[j][i][comp]
    **/
    inline PetscScalar apply_2D_x_left(const PetscScalar *const *const *const v, const PetscScalar hix, const PetscInt i, const PetscInt j, const PetscInt comp) const
    {
      PetscScalar u = 0;
      for (PetscInt is = 0; is<closure_width; is++)
      {
        u += closure_stencils[i][is]*v[j][is][comp];
      }
      return hix*u;
    };

    /**
    * Computes the derivative in y-direction of a multicomponent 2D grid function v[j][i][comp] for an index j within the set of left closure points.
    * Input:
    *
    * Output:
    * Derivative v_y[j][i][comp]
    **/
    inline PetscScalar apply_2D_y_left(const PetscScalar *const *const *const v, const PetscScalar hiy, const PetscInt i, const PetscInt j, const PetscInt comp) const
    {
      PetscScalar u = 0;
      for (PetscInt is = 0; is<closure_width; is++)
      {
        u += closure_stencils[j][is]*v[is][i][comp];
      }
      return hiy*u;
    };

    /**
    * Computes the derivative in x-direction of a multicomponent 2D grid function v[j][i][comp] for an index i within the set of interior points.
    * Input:
    *
    * Output:
    * Derivative v_x[j][i][comp]
    **/
    inline PetscScalar apply_2D_x_interior(const PetscScalar *const *const *const v, const PetscScalar hix, const PetscInt i, const PetscInt j, const PetscInt comp) const
    {
      PetscScalar u = 0;
      for (PetscInt is = 0; is<interior_width; is++)
      {
        u += interior_stencil[is]*v[j][i-(interior_width-1)/2+is][comp];
      }
      return hix*u;
    };

    /**
    * Computes the derivative in y-direction of a multicomponent 2D grid function v[j][i][comp] for an index j within the set of interior points.
    * Input:
    *
    * Output:
    * Derivative v_y[j][i][comp]
    **/
    inline PetscScalar apply_2D_y_interior(const PetscScalar *const *const *const v, const PetscScalar hiy, const PetscInt i, const PetscInt j, const PetscInt comp) const
    {
      PetscScalar u = 0;
      for (PetscInt is = 0; is<interior_width; is++)
      {
        u += interior_stencil[is]*v[j-(interior_width-1)/2+is][i][comp];
      }
      return hiy*u;
    };

    /**
    * Computes the derivative in y-direction of a multicomponent 2D grid function v[j][i][comp] for an index i within the set of right closure points.
    * Input:
    *
    * Output:
    * Derivative v_y[j][i][comp]
    **/
    inline PetscScalar apply_2D_x_right(const PetscScalar *const *const *const v, const PetscScalar hix, const PetscInt Nx, const PetscInt i, const PetscInt j, const PetscInt comp) const
    {
      PetscScalar u = 0;
      for (PetscInt is = 0; is < closure_width; is++)
      {
        u -= closure_stencils[Nx-i-1][closure_width-is-1]*v[j][Nx-closure_width+is][comp];
      }
      return hix*u;
    };

    /**
    * Computes the derivative in y-direction of a multicomponent 2D grid function v[j][i][comp] for an index j within the set of right closure points.
    * Input:
    *
    * Output:
    * Derivative v_y[j][i][comp]
    **/
    inline PetscScalar apply_2D_y_right(const PetscScalar *const *const *const v, const PetscScalar hiy, const PetscInt Ny, const PetscInt i, const PetscInt j, const PetscInt comp) const
    {
      PetscScalar u = 0;
      for (PetscInt is = 0; is < closure_width; is++)
      {
        u -= closure_stencils[Ny-j-1][closure_width-is-1]*v[Ny-closure_width+is][i][comp];
      }
      return hiy*u;
    };
  };

  //=============================================================================
  // Operator definitions
  //=============================================================================

  /** 
  * Standard 2nd order central
  **/
  template <>
  constexpr D1_central<3,1,2>::D1_central()
    : interior_stencil{-1./2, 0, 1./2}, 
      closure_stencils{{-1., 1}}
  {}

  /** 
  * Standard 4th order central
  **/
  template <>
  constexpr D1_central<5,4,6>::D1_central()
    : interior_stencil{1./12, -2./3, 0., 2./3, -1./12}, 
      closure_stencils{
        {-24./17, 59./34, -4./17,  -3./34, 0, 0},
        {-1./2, 0, 1./2,  0, 0, 0},
        {4./43, -59./86, 0, 59./86, -4./43, 0},
        {3./98, 0, -59./98, 0, 32./49, -4./49}}
  {}

  /** 
  * Standard 6th order central
  **/
  template <>
  constexpr D1_central<7,6,9>::D1_central()
    : interior_stencil{-1./60, 3./20, -3./4, 0, 3./4, -3./20, 1./60}, 
      closure_stencils{
        {-21600./13649, 104009./54596,  30443./81894,  -33311./27298, 16863./27298, -15025./163788, 0, 0, 0},
        {-104009./240260, 0, -311./72078,  20229./24026, -24337./48052, 36661./360390, 0, 0, 0},
        {-30443./162660, 311./32532, 0, -11155./16266, 41287./32532, -21999./54220, 0, 0, 0},
        {33311./107180, -20229./21436, 485./1398, 0, 4147./21436, 25427./321540, 72./5359, 0, 0},
        {-16863./78770, 24337./31508, -41287./47262, -4147./15754, 0, 342523./472620, -1296./7877, 144./7877, 0},
        {15025./525612, -36661./262806, 21999./87602, -25427./262806, -342523./525612, 0, 32400./43801, -6480./43801, 720./43801}}
  {}
} //End namespace sbp