/*
 * MIT License
 * 
 * Copyright (c) 2021 WuK
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/*
 * Include files
 */
#include "hplai.hh"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef STDC_HEADERS
    void HPLAI_papanllN(
        HPLAI_T_panel *PANEL,
        const int M,
        const int N,
        const int ICOFF,
        HPLAI_T_AFLOAT *WORK)
#else
void HPLAI_papanllN(PANEL, M, N, ICOFF, WORK)
    HPLAI_T_panel *PANEL;
const int M;
const int N;
const int ICOFF;
HPLAI_T_AFLOAT *WORK;
#endif
    {
        /* 
 * Purpose
 * =======
 *
 * HPLAI_papanllN factorizes  a panel of columns that is a sub-array of a
 * larger one-dimensional panel A  using the Left-looking variant of the
 * usual one-dimensional algorithm.  The lower triangular N0-by-N0 upper
 * block of the panel is stored in no-transpose form (i.e. just like the
 * input matrix itself).
 *  
 * Bi-directional  exchange  is  used  to  perform  the  swap::broadcast
 * operations  at once  for one column in the panel.  This  results in a
 * lower number of slightly larger  messages than usual.  On P processes
 * and assuming bi-directional links,  the running time of this function
 * can be approximated by (when N is equal to N0):
 *  
 *    N0 * log_2( P ) * ( lat + ( 2*N0 + 4 ) / bdwth ) +
 *    N0^2 * ( M - N0/3 ) * gam2-3
 *  
 * where M is the local number of rows of  the panel, lat and bdwth  are
 * the latency and bandwidth of the network for  HPLAI_T_AFLOAT  precision  real
 * words, and  gam2-3  is  an estimate of the  Level 2 and Level 3  BLAS
 * rate of execution. The  recursive  algorithm  allows indeed to almost
 * achieve  Level 3 BLAS  performance  in the panel factorization.  On a
 * large  number of modern machines,  this  operation is however latency
 * bound,  meaning  that its cost can  be estimated  by only the latency
 * portion N0 * log_2(P) * lat.  Mono-directional links will HPLAI_T_AFLOAT this
 * communication cost.
 *  
 * Note that  one  iteration of the the main loop is unrolled. The local
 * computation of the absolute value max of the next column is performed
 * just after its update by the current column. This allows to bring the
 * current column only  once through  cache at each  step.  The  current
 * implementation  does not perform  any blocking  for  this sequence of
 * BLAS operations, however the design allows for plugging in an optimal
 * (machine-specific) specialized  BLAS-like kernel.  This idea has been
 * suggested to us by Fred Gustavson, IBM T.J. Watson Research Center.
 *
 * Arguments
 * =========
 *
 * PANEL   (local input/output)          HPLAI_T_panel *
 *         On entry,  PANEL  points to the data structure containing the
 *         panel information.
 *
 * M       (local input)                 const int
 *         On entry,  M specifies the local number of rows of sub(A).
 *
 * N       (local input)                 const int
 *         On entry,  N specifies the local number of columns of sub(A).
 *
 * ICOFF   (global input)                const int
 *         On entry, ICOFF specifies the row and column offset of sub(A)
 *         in A.
 *
 * WORK    (local workspace)             HPLAI_T_AFLOAT *
 *         On entry, WORK  is a workarray of size at least 2*(4+2*N0).
 *
 * ---------------------------------------------------------------------
 */
        /*
 * .. Local Variables ..
 */
        HPLAI_T_AFLOAT *A, *L1, *L1ptr;
#ifdef HPL_CALL_VSIPL
        vsip_mview_d *Av0, *Av1, *Yv1, *Xv0, *Xv1;
#endif
        int Mm1, Nm1, curr, ii, iip1, jj, kk, lda,
            m = M, n0;
/* ..
 * .. Executable Statements ..
 */
#ifdef HPL_DETAILED_TIMING
        HPL_ptimer(HPL_TIMING_PFACT);
#endif
        A = PANEL->A;
        lda = PANEL->lda;
        L1 = PANEL->L1;
        n0 = PANEL->jb;
        curr = (int)(PANEL->grid->myrow == PANEL->prow);

        Nm1 = N - 1;
        jj = ICOFF;
        if (curr != 0)
        {
            ii = ICOFF;
            iip1 = ii + 1;
            Mm1 = m - 1;
        }
        else
        {
            ii = 0;
            iip1 = ii;
            Mm1 = m;
        }
#ifdef HPL_CALL_VSIPL
        /*
 * Admit the blocks
 */
        (void)vsip_blockadmit_d(PANEL->Ablock, VSIP_TRUE);
        (void)vsip_blockadmit_d(PANEL->L1block, VSIP_TRUE);
        /*
 * Create the matrix views
 */
        Av0 = vsip_mbind_d(PANEL->Ablock, 0, 1, lda, lda, PANEL->pmat->nq);
        Xv0 = vsip_mbind_d(PANEL->L1block, 0, 1, PANEL->jb, PANEL->jb, PANEL->jb);
#endif
        /*
 * Find local absolute value max in first column and initialize WORK[0:3]
 */
        HPLAI_alocmax(PANEL, m, ii, jj, WORK);

        while (Nm1 > 0)
        {
            /*
 * Swap and broadcast the current row
 */
            HPLAI_pamxswp(PANEL, m, ii, jj, WORK);
            HPLAI_alocswpN(PANEL, ii, jj, WORK);

            L1ptr = Mptr(L1, ICOFF, jj + 1, n0);
            kk = jj + 1 - ICOFF;
            blas::trsv<HPLAI_T_AFLOAT, HPLAI_T_AFLOAT>(blas::Layout::ColMajor, blas::Uplo::Lower, blas::Op::NoTrans, blas::Diag::Unit, kk,
                                                       Mptr(L1, ICOFF, ICOFF, n0), n0, L1ptr, 1);
            /*
 * Scale  current column by its absolute value max entry  -  Update  and 
 * find local  absolute value max  in next column (Only one pass through 
 * cache for each next column).  This sequence of operations could bene-
 * fit from a specialized  blocked implementation.
 */
            if (WORK[0] != HPLAI_rzero)
                blas::scal<HPLAI_T_AFLOAT>(Mm1, HPLAI_rone / WORK[0], Mptr(A, iip1, jj, lda), 1);
#ifdef HPL_CALL_VSIPL
            /*
 * Create the matrix subviews
 */
            Av1 = vsip_msubview_d(Av0, PANEL->ii + iip1, PANEL->jj + ICOFF, Mm1, kk);
            Xv1 = vsip_msubview_d(Xv0, ICOFF, jj + 1, kk, 1);
            Yv1 = vsip_msubview_d(Av0, PANEL->ii + iip1, PANEL->jj + jj + 1, Mm1, 1);

            vsip_gemp_d(-HPLAI_rone, Av1, VSIP_MAT_NTRANS, Xv1, VSIP_MAT_NTRANS,
                        HPLAI_rone, Yv1);
            /*
 * Destroy the matrix subviews
 */
            (void)vsip_mdestroy_d(Yv1);
            (void)vsip_mdestroy_d(Xv1);
            (void)vsip_mdestroy_d(Av1);
#else
        blas::gemv<HPLAI_T_AFLOAT, HPLAI_T_AFLOAT, HPLAI_T_AFLOAT>(blas::Layout::ColMajor, blas::Op::NoTrans, Mm1, kk, -HPLAI_rone,
                                                                   Mptr(A, iip1, ICOFF, lda), lda, L1ptr, 1,
                                                                   HPLAI_rone, Mptr(A, iip1, jj + 1, lda), 1);
#endif
            HPLAI_alocmax(PANEL, Mm1, iip1, jj + 1, WORK);
            if (curr != 0)
            {
                blas::copy<HPLAI_T_AFLOAT, HPLAI_T_AFLOAT>(kk, L1ptr, 1, Mptr(A, ICOFF, jj + 1, lda), 1);
                ii = iip1;
                iip1++;
                m = Mm1;
                Mm1--;
            }
            Nm1--;
            jj++;
        }
        /*
 * Swap and broadcast last row - Scale last column by its absolute value
 * max entry
 */
        HPLAI_pamxswp(PANEL, m, ii, jj, WORK);
        HPLAI_alocswpN(PANEL, ii, jj, WORK);
        if (WORK[0] != HPLAI_rzero)
            blas::scal<HPLAI_T_AFLOAT>(Mm1, HPLAI_rone / WORK[0], Mptr(A, iip1, jj, lda), 1);
#ifdef HPL_CALL_VSIPL
        /*
 * Release the blocks
 */
        (void)vsip_blockrelease_d(vsip_mgetblock_d(Xv0), VSIP_TRUE);
        (void)vsip_blockrelease_d(vsip_mgetblock_d(Av0), VSIP_TRUE);
        /*
 * Destroy the matrix views
 */
        (void)vsip_mdestroy_d(Xv0);
        (void)vsip_mdestroy_d(Av0);
#endif
#ifdef HPL_DETAILED_TIMING
        HPL_ptimer(HPL_TIMING_PFACT);
#endif
        /*
 * End of HPLAI_papanllN
 */
    }

#ifdef __cplusplus
}
#endif
