#pragma once

#include <petscsystypes.h>
#include <array>
#include "grids/grid_function.h"


namespace sbp{
  //=============================================================================
  // 1D functions
  //=============================================================================

  template <class SbpInterpolator>
  inline PetscErrorCode apply_F2C(const SbpInterpolator& ICF, PetscScalar **src, PetscScalar **dst, PetscInt i_start, PetscInt i_end, const PetscInt N)
  {
    int i, dof;
    const PetscInt ndofs = 8;

    const auto [F2C_nc, C2F_nc] = ICF.get_ranges();
    if (i_start == 0) {
      for (i = 0; i < F2C_nc; i++) 
      { 
        for (dof = 0; dof < ndofs; dof++) {
          dst[i][dof] = ICF.F2C_apply_left(src, i, dof);  
        }
      }

      for (i = F2C_nc; i < i_end; i++) 
      { 
        for (dof = 0; dof < ndofs; dof++) {
          dst[i][dof] = ICF.F2C_apply_interior(src, i, dof);
        }
      }
    } else if (i_end == N) {

      for (i = i_start; i < i_end-F2C_nc; i++) 
      { 
        for (dof = 0; dof < ndofs; dof++) {
          dst[i][dof] = ICF.F2C_apply_interior(src, i, dof);
        }
      }

      for (i = i_end - F2C_nc; i < i_end; i++) 
      { 
        for (dof = 0; dof < ndofs; dof++) {
          dst[i][dof] = ICF.F2C_apply_right(src, i_end, i, dof);
        }
      }
    } else {
      for (i = i_start; i < i_end; i++) 
      { 
        for (dof = 0; dof < ndofs; dof++) {
          dst[i][dof] = ICF.F2C_apply_interior(src, i, dof);
        }
      }
    }
    return 0;
  }

  template <class SbpInterpolator>
  inline PetscErrorCode apply_C2F(const SbpInterpolator& ICF, PetscScalar **src, PetscScalar **dst, PetscInt i_start, PetscInt i_end, const PetscInt N)
  {
    int i, dof;
    const PetscInt ndofs = 8;

    const auto [F2C_nc, C2F_nc] = ICF.get_ranges();
    if (i_start == 0) {
      for (i = 0; i < C2F_nc; i++) 
      { 
        for (dof = 0; dof < ndofs; dof++) {
          dst[i][dof] = ICF.C2F_apply_left(src, i, dof);
        }
      }

      for (i = C2F_nc; i < i_end; i++) 
      { 
        if (i % 2 == 0) {
          for (dof = 0; dof < ndofs; dof++) {
            dst[i][dof] = ICF.C2F_even_apply_interior(src, i, dof);
          }
        } else {
          for (dof = 0; dof < ndofs; dof++) {
            dst[i][dof] = ICF.C2F_odd_apply_interior(src, i, dof);
          }
        }
      }
    } else if (i_end == N) {
      for (i = i_start; i < i_end-C2F_nc; i++) 
      { 
        if (i % 2 == 0) {
          for (dof = 0; dof < ndofs; dof++) {
            dst[i][dof] = ICF.C2F_even_apply_interior(src, i, dof);
          }
        } else {
          for (dof = 0; dof < ndofs; dof++) {
            dst[i][dof] = ICF.C2F_odd_apply_interior(src, i, dof);
          }
        }
      }

      for (i = i_end - C2F_nc; i < i_end; i++) 
      { 
        for (dof = 0; dof < ndofs; dof++) {
          dst[i][dof] = ICF.C2F_apply_right(src, N, i, dof);
        }
      }
    } else {
      for (i = i_start; i < i_end; i++) 
      { 
        if (i % 2 == 0) {
          for (dof = 0; dof < ndofs; dof++) {
            dst[i][dof] = ICF.C2F_even_apply_interior(src, i, dof);
          }
        } else {
          for (dof = 0; dof < ndofs; dof++) {
            dst[i][dof] = ICF.C2F_odd_apply_interior(src, i, dof);
          }
        }
      }
    }

    return 0;
  }

  inline PetscScalar ref_imp_apply_D_time(const PetscScalar D_time[4][4], PetscScalar **src, PetscInt i, PetscInt tcomp, PetscInt dof)
  {
    return D_time[tcomp][0]*src[i][0+dof] + D_time[tcomp][1]*src[i][2+dof] + D_time[tcomp][2]*src[i][4+dof] + D_time[tcomp][3]*src[i][6+dof];
  }

  template <class SbpDerivative, class SbpInvQuad, typename VelocityFunction>
  inline PetscErrorCode ref_imp_apply_left(const PetscScalar D_time[4][4], const SbpDerivative& D1, const SbpInvQuad& HI, VelocityFunction&& a, PetscScalar **src, PetscScalar **dst, const PetscInt N, const PetscScalar hi, const PetscInt n_closures)
  {
    int i, tcomp;

    i = 0;
    // Compute first component at boundary point, only time derivative
    for (tcomp = 0; tcomp < 4; tcomp++) {
      dst[i][2*tcomp] = ref_imp_apply_D_time(D_time, src, i, tcomp, 0);
    }

    // Set first comp in src to zero at boundary point
    for (tcomp = 0; tcomp < 4; tcomp++) {
      src[i][2*tcomp] = 0;
    }

    // Compute second component at boundary point
    for (tcomp = 0; tcomp < 4; tcomp++) {
      dst[i][2*tcomp+1] = ref_imp_apply_D_time(D_time, src, i, tcomp, 1) - D1.apply_left(src,hi,i,2*tcomp);
    }
    
    // Compute inner points
    for (i = 1; i < n_closures; i++)
    {
      for (tcomp = 0; tcomp < 4; tcomp++) {
        dst[i][2*tcomp] = ref_imp_apply_D_time(D_time, src, i, tcomp, 0) - D1.apply_left(src,hi,i,2*tcomp+1);
        dst[i][2*tcomp+1] = ref_imp_apply_D_time(D_time, src, i, tcomp, 1) - D1.apply_left(src,hi,i,2*tcomp);
      }
    }

    return 0;
  }

  template <class SbpDerivative, class SbpInvQuad, typename VelocityFunction>
  inline PetscErrorCode ref_imp_apply_right(const PetscScalar D_time[4][4], const SbpDerivative& D1, const SbpInvQuad& HI, VelocityFunction&& a, PetscScalar **src, PetscScalar **dst, const PetscInt N, const PetscScalar hi, const PetscInt n_closures)
  {
    int i, tcomp;

    i = N-1;

    // Compute first component, only time derivative
    for (tcomp = 0; tcomp < 4; tcomp++) {
      dst[i][2*tcomp] = ref_imp_apply_D_time(D_time, src, i, tcomp, 0);
    }

    // Set first comp in src to zero at boundary point
    for (tcomp = 0; tcomp < 4; tcomp++) {
      src[i][2*tcomp] = 0;
    }

    // Compute second component
    for (tcomp = 0; tcomp < 4; tcomp++) {
      dst[i][2*tcomp+1] = ref_imp_apply_D_time(D_time, src, i, tcomp, 1) - D1.apply_right(src,hi,N,i,2*tcomp);
    }

    for (i = N-n_closures; i < N-1; i++)
    {
      for (tcomp = 0; tcomp < 4; tcomp++) {
        dst[i][2*tcomp] = ref_imp_apply_D_time(D_time, src, i, tcomp, 0) - D1.apply_right(src,hi,N,i,2*tcomp+1);
        dst[i][2*tcomp+1] = ref_imp_apply_D_time(D_time, src, i, tcomp, 1) - D1.apply_right(src,hi,N,i,2*tcomp);  
      }
    }
    return 0;
  }

  template <class SbpDerivative, class SbpInvQuad, typename VelocityFunction>
  inline PetscErrorCode ref_imp_apply_interior(const PetscScalar D_time[4][4], const SbpDerivative& D1, const SbpInvQuad& HI, VelocityFunction&& a, PetscScalar **src, PetscScalar **dst, PetscInt i_start, PetscInt i_end, const PetscInt N, const PetscScalar hi, const PetscInt n_closures)
  {
    int i, tcomp;

    for (i = i_start; i < i_end; i++)
    {
      for (tcomp = 0; tcomp < 4; tcomp++) {
        dst[i][2*tcomp] = ref_imp_apply_D_time(D_time, src, i, tcomp, 0) - D1.apply_interior(src,hi,i,2*tcomp+1);  
        dst[i][2*tcomp+1] = ref_imp_apply_D_time(D_time, src, i, tcomp, 1) - D1.apply_interior(src,hi,i,2*tcomp);  
      }
    }
    return 0;
  }

  template <class SbpDerivative, class SbpInvQuad, typename VelocityFunction>
  inline PetscErrorCode ref_imp_apply_all(const PetscScalar D_time[4][4], const SbpDerivative& D1, const SbpInvQuad& HI, VelocityFunction&& a, PetscScalar **src, PetscScalar **dst, PetscInt i_start, PetscInt i_end, const PetscInt N, const PetscScalar hi, const PetscScalar Tend)
  {
    const auto [iw, n_closures, closure_width] = D1.get_ranges();

    if (i_start == 0) 
    {
      ref_imp_apply_left(D_time, D1, HI, a, src, dst, N, hi, n_closures);
      ref_imp_apply_interior(D_time, D1, HI, a, src, dst, n_closures, i_end, N, hi, n_closures);
    } else if (i_end == N) 
    {
      ref_imp_apply_right(D_time, D1, HI, a, src, dst, N, hi, n_closures);
      ref_imp_apply_interior(D_time, D1, HI, a, src, dst, i_start, N-n_closures, N, hi, n_closures);
    } else 
    {
      ref_imp_apply_interior(D_time, D1, HI, a, src, dst, i_start, i_end, N, hi, n_closures);
    }
    return 0;
  }  

} //namespace sbp