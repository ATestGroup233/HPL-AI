#ifndef HPLAI_PGESV_HH
#define HPLAI_PGESV_HH
/*
 * ---------------------------------------------------------------------
 * Include files
 * ---------------------------------------------------------------------
 */

#ifdef __cplusplus
#include <blas.hh>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#include "hpl_pgesv.h"

#include "hplai_misc.h"
#include "hplai_auxil.h"

#include "hplai_grid.h"
#include "hplai_comm.h"
#include "hplai_panel.h"
#include "hplai_pfact.h"

void                             HPLAI_pipid
STDC_ARGS( (
   HPLAI_T_panel *,
   int *,
   int *
) );
void                             HPLAI_plindx0
STDC_ARGS( (
   HPLAI_T_panel *,
   const int,
   int *,
   int *,
   int *,
   int *
) );
void                             HPLAI_palaswp00N
STDC_ARGS( (
   HPLAI_T_panel *,
   int *,
   HPLAI_T_panel *,
   const int
) );
void                             HPLAI_palaswp00T
STDC_ARGS( (
   HPLAI_T_panel *,
   int *,
   HPLAI_T_panel *,
   const int
) );

#define HPLAI_perm HPL_perm
#define HPLAI_logsort HPL_logsort
void                             HPLAI_plindx10
STDC_ARGS( (
   HPLAI_T_panel *,
   const int,
   const int *,
   int *,
   int *,
   int *
) );
void                             HPLAI_plindx1
STDC_ARGS( (
   HPLAI_T_panel *,
   const int,
   const int *,
   int *,
   int *,
   int *,
   int *,
   int *,
   int *,
   int *,
   int *
) );
void                             HPLAI_spreadN
STDC_ARGS( (
   HPLAI_T_panel *,
   int *,
   HPLAI_T_panel *,
   const blas::Side,
   const int,
   HPLAI_T_AFLOAT *,
   const int,
   const int,
   const int *,
   const int *,
   const int *
) );
void                             HPLAI_spreadT
STDC_ARGS( (
   HPLAI_T_panel *,
   int *,
   HPLAI_T_panel *,
   const blas::Side,
   const int,
   HPLAI_T_AFLOAT *,
   const int,
   const int,
   const int *,
   const int *,
   const int *
) );
void                             HPLAI_equil
STDC_ARGS( (
   HPLAI_T_panel *,
   int *,
   HPLAI_T_panel *,
   const blas::Op,
   const int,
   HPLAI_T_AFLOAT *,
   const int,
   int *,
   const int *,
   const int *,
   int *
) );
void                             HPLAI_rollN
STDC_ARGS( (
   HPLAI_T_panel *,
   int *,
   HPLAI_T_panel *,
   const int,
   HPLAI_T_AFLOAT *,
   const int,
   const int *,
   const int *,
   const int *
) );
void                             HPLAI_rollT
STDC_ARGS( (
   HPLAI_T_panel *,
   int *,
   HPLAI_T_panel *,
   const int,
   HPLAI_T_AFLOAT *,
   const int,
   const int *,
   const int *,
   const int *
) );
void                             HPLAI_palaswp01N
STDC_ARGS( (
   HPLAI_T_panel *,
   int *,
   HPLAI_T_panel *,
   const int
) );
void                             HPLAI_palaswp01T
STDC_ARGS( (
   HPLAI_T_panel *,
   int *,
   HPLAI_T_panel *,
   const int
) );

void                             HPLAI_paupdateNN
STDC_ARGS( (
   HPLAI_T_panel *,
   int *,
   HPLAI_T_panel *,
   const int
) );
void                             HPLAI_paupdateNT
STDC_ARGS( (
   HPLAI_T_panel *,
   int *,
   HPLAI_T_panel *,
   const int
) );
void                             HPLAI_paupdateTN
STDC_ARGS( (
   HPLAI_T_panel *,
   int *,
   HPLAI_T_panel *,
   const int
) );
void                             HPLAI_paupdateTT
STDC_ARGS( (
   HPLAI_T_panel *,
   int *,
   HPLAI_T_panel *,
   const int
) );


void                             HPLAI_pdgesv
STDC_ARGS( (
   HPLAI_T_grid *,
   HPLAI_T_palg *,
   HPL_T_pmat *
) );

void                             HPLAI_pagesv0
STDC_ARGS( (
   HPLAI_T_grid *,
   HPLAI_T_palg *,
   HPLAI_T_pmat *
) );
void                             HPLAI_pagesvK1
STDC_ARGS( (
   HPLAI_T_grid *,
   HPLAI_T_palg *,
   HPLAI_T_pmat *
) );
void                             HPLAI_pagesvK2
STDC_ARGS( (
   HPLAI_T_grid *,
   HPLAI_T_palg *,
   HPLAI_T_pmat *
) );
void                             HPLAI_pagesv
STDC_ARGS( (
   HPLAI_T_grid *,
   HPLAI_T_palg *,
   HPLAI_T_pmat *
) );
 
void                             HPLAI_patrsv
STDC_ARGS( (
   HPLAI_T_grid *,
   HPLAI_T_pmat *
) );

#ifdef __cplusplus
}
#endif

#endif
/*
 * End of hplai_pgesv.hh
 */