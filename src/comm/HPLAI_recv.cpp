/*
 * Include files
 */
#include "hplai.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Do not use  MPI user-defined data types no matter what.  This routine
 * is used for small contiguous messages.
 */
#ifdef HPL_USE_MPI_DATATYPE
#undef HPL_USE_MPI_DATATYPE
#endif

#ifdef STDC_HEADERS
   int HPLAI_recv(
       HPLAI_T_AFLOAT *RBUF,
       int RCOUNT,
       int SRC,
       int RTAG,
       MPI_Comm COMM)
#else
int HPLAI_recv(RBUF, RCOUNT, SRC, RTAG, COMM)
    HPLAI_T_AFLOAT *RBUF;
int RCOUNT;
int SRC;
int RTAG;
MPI_Comm COMM;
#endif
   {
      /*
 * .. Local Variables ..
 */
      MPI_Status status;
#ifdef HPL_USE_MPI_DATATYPE
      MPI_Datatype type;
#endif
      int ierr;
      /* ..
 * .. Executable Statements ..
 */
      if (RCOUNT <= 0)
         return (HPL_SUCCESS);

#ifdef HPL_USE_MPI_DATATYPE
      ierr = MPI_Type_contiguous(1LL * sizeof(HPLAI_T_AFLOAT) * RCOUNT, MPI_BYTE, &type);
      if (ierr == MPI_SUCCESS)
         ierr = MPI_Type_commit(&type);
      if (ierr == MPI_SUCCESS)
         ierr = MPI_Recv((void *)(RBUF), 1, type, SRC, RTAG, COMM,
                         &status);
      if (ierr == MPI_SUCCESS)
         ierr = MPI_Type_free(&type);
#else
   ierr = MPI_Recv((void *)(RBUF), 1LL * sizeof(HPLAI_T_AFLOAT) * RCOUNT, MPI_BYTE, SRC, RTAG,
                   COMM, &status);
#endif
      return ((ierr == MPI_SUCCESS ? HPL_SUCCESS : HPL_FAILURE));
      /*
 * End of HPL_recv
 */
   }

#ifdef __cplusplus
}
#endif
