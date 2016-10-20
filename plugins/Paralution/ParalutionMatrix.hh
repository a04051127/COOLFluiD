// Copyright (C) 2012 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef COOLFluiD_Numerics_Paralution_ParalutionMatrix_hh
#define COOLFluiD_Numerics_Paralution_ParalutionMatrix_hh

//////////////////////////////////////////////////////////////////////////////

#include <paralution.hpp>

#include "MathTools/RealVector.hh"
#include "Framework/LSSMatrix.hh"
#include "Framework/DataSocketSink.hh"
// #include "Paralution/ParalutionVector.hh"

//////////////////////////////////////////////////////////////////////////////

namespace COOLFluiD {

  namespace Framework { class BlockAccumulator; }

    namespace Paralution {

//////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a proxy for the PETSC Mat object.
 *
 * @author Isaac Alonso
 * @author Andrea Lani
 *
 */
class ParalutionMatrix : public Framework::LSSMatrix {

public: // functions

  /**
   * Default constructor without arguments.
   */
  ParalutionMatrix();

  /**
   * Destructor.
   */
  ~ParalutionMatrix();

  /**
   * Create a sequential sparse matrix
   * @param m number of Rows of the matrix
   * @param n number of Columns of the matrix
   * @param nz maximum number of non-zero entries
   * @param nnz exact number of non-zero entries in the diagonal portion of the local matrix for each row
   */
  void createSeqAIJ(const CFint m,
		    const CFint n,
		    const CFint nz,
		    const CFint* nnz,
		    const char* name = CFNULL);
  
  /**
   * Create a sequential block sparse matrix
   * @param blocksize size of the block matrix
   * @param m number of Rows of the matrix
   * @param n number of Columns of the matrix
   * @param nz maximum number of non-zero entries
   * @param nnz exact number of non-zero entries in the diagonal portion of the local matrix for each row
   */
  void createSeqBAIJ(const CFuint blockSize,
		     const CFint m,
		     const CFint n,
		     const CFint nz,
		     const CFint* nnz,
		     const char* name = CFNULL);
  
  /**
   * Create a parallel sparse matrix
   * @param m localNumber of Rows of the matrix
   * @param n local Number of Columns of the matrix
   * @param M Global Number of Rows of the matrix
   * @param N Global Number of Columns of the matrix
   * @param dnz maximum number of non-zero entries in the diagonal portion of the local matrix
   * @param dnnz exact number of non-zero entries in the diagonal portion of the local matrix for each row
   * @param onz maximum number of non-zero entries in the 'out of diagonal' portion of the local matrix
   * @param onnz exact number of non-zero entries in the 'out-of-diagonal' portion of the local matrix for each row
   */
#ifdef CF_HAVE_MPI
  void createParAIJ(MPI_Comm comm,
		    const CFint m,
		    const CFint n,
		    const CFint M,
		    const CFint N,
		    const CFint dnz,
		    const CFint* dnnz,
		    const CFint onz,
		    const CFint* onnz,
		    const char* name = CFNULL);
#endif
  
  /**
   * Create a parallel block sparse matrix
   * @param blocksize size of the block matrices
   * @param m local Number of Rows of the matrix
   * @param n local Number of Columns of the matrix
   * @param M Global Number of Rows of the matrix
   * @param N Global Number of Columns of the matrix
   * @param dnz maximum number of non-zero entries in the diagonal portion of the local matrix
   * @param dnnz exact number of non-zero entries in the diagonal portion of the local matrix for each row
   * @param onz maximum number of non-zero entries in the 'out of diagonal' portion of the local matrix
   * @param onnz exact number of non-zero entries in the 'out-of-diagonal' portion of the local matrix for each row
   */
#ifdef CF_HAVE_MPI
  void createParBAIJ(MPI_Comm comm,
		     const CFuint blockSize,
		     const CFint m,
		     const CFint n,
		     const CFint M,
		     const CFint N,
		     const CFint dnz,
		     const CFint* dnnz,
		     const CFint onz,
		     const CFint* onnz,
		     const char* name = CFNULL);
#endif
  



  void createAIJ(const CFuint BlockSize,
                 const CFuint nbRows,
                 const CFuint nbCols);


  void createCSR(std::valarray<CFint> allNonZero, CFuint nbEqs);


  /**
  * Assembly the matrix from the COO vectors
  */
  void finalAssembly(CFuint size){
  //   CFint *rowPtr = _row.ptr();
  //   CFint *colPtr = _col.ptr();
  //   CFreal *valPtr = _val.ptr();
     m_mat.SetDataPtrCSR(&_rowoff, &_col, &_val, "SystemMatrix", _size, size, size);
  }





  /**
   * Start to assemble the matrix
   */
  void beginAssembly(LSSMatrix::LSSMatrixAssemblyType assemblyType)
  {
    // MatAssemblyType matAssType = (assemblyType == FLUSH_ASSEMBLY) ?
    //   MAT_FLUSH_ASSEMBLY : MAT_FINAL_ASSEMBLY;
    // CF_CHKERRCONTINUE(MatAssemblyBegin(m_mat, matAssType));

    // some sanity checks 
    /*MatInfo info;
    CF_CHKERRCONTINUE(MatGetInfo(m_mat,MAT_LOCAL,&info));
    std::cout << "### ParalutionMatrix::beginAssembly() => assembly report ###" << std::endl;
    std::cout << "### info.mallocs      = " << info.mallocs << std::endl; 
    std::cout << "### info.nz_allocated = " << info.nz_allocated << std::endl;  
    std::cout << "### info.nz_unneeded  = " << info.nz_unneeded << std::endl;    
    std::cout << "### info.nz_used      = " << info.nz_used << std::endl;
    std::cout << "### info.assemblies   = " << info.assemblies << std::endl;
    */
  }

  /**
   * Finish to assemble the matrix
   */
  void endAssembly(LSSMatrix::LSSMatrixAssemblyType assemblyType)
  {
    // MatAssemblyType matAssType = (assemblyType == FLUSH_ASSEMBLY) ?
    //   MAT_FLUSH_ASSEMBLY : MAT_FINAL_ASSEMBLY;
    // CF_CHKERRCONTINUE(MatAssemblyEnd(m_mat, matAssType));
  }

  /**
   * Print this matrix
   */
  void printToScreen() const;

  /**
   * Print this matrix to a file
   */
  void printToFile(const char* fileName) const;

  /**
   * Set all the elements of the Vector equal to the given value
   */
  void setValue(const CFint im,
                const CFint in,
                const CFreal value)
  {
/*
    bool found = false;
      for (CFuint _i=0; _i<_nnz; _i++){
         if (im==_row[_i] && in ==_col[_i]){
            _val[_i] += value; found=true;
         }
      }
      if (found==false){
        _row[_nnz] = im;
        _col[_nnz] = in;
        _val[_nnz] += value;
        _nnz++;
      }
  */ 
  }

  /**
   * Set a list of values
   */
  void setValues(const CFuint m,
                 const CFint* im,
                 const CFuint n,
                 const CFint* in,
                 const CFreal* values)
  {
    // CF_CHKERRCONTINUE(MatSetValues(m_mat, m, im, n, in, values,
    //                            INSERT_VALUES));
  }

  /**
   * Add an array of values in the vector
   */
  void addValue(const CFint im,
                const CFint in,
                const CFreal value)   //TODO
  {
    // CF_CHKERRCONTINUE(MatSetValues(m_mat, 1, &im, 1, &in, &value,
    //                            ADD_VALUES));
  }


  void addtoCOO( CFuint* col, CFuint* row, CFreal* val, CFuint nnz);

  /**
   * Add a list of values
   */
  void addValues(const CFuint m,
                 const CFint* im,
                 const CFuint n,
     const CFint* in,
     const CFreal* values)
  {
    // CF_CHKERRCONTINUE(MatSetValues(m_mat, m, im, n, in, values,
    //     ADD_VALUES));
  }

  /**
   * Get a list of values
   */
  void getValue(const CFint im,
                const CFint in,
                CFreal& value)
  {
    // CF_CHKERRCONTINUE(MatGetValues(m_mat, 1, &im, 1, &in, &value));
  }

  /**
   * Get a list of values
   */
  void getValues(const CFuint m,
		 const CFint* im,
		 const CFuint n,
		 const CFint* in,
		 CFreal* values)
  {
    // CF_CHKERRCONTINUE(MatGetValues(m_mat, m, im, n, in, values));
  }

  
  paralution::LocalMatrix<CFreal> getMatrix(){
     return m_mat;
  }

  void moveToGPU(){
    m_mat.MoveToAccelerator();
  }

  void AssignToSolver(paralution::Solver< paralution::LocalMatrix<CFreal>, paralution::LocalVector<CFreal>, CFreal >& ls){
    ls.SetOperator(m_mat);
  }


  void convertToCSR(){ m_mat.ConvertToCSR(); }
  /**
   * Set a row, diagonal and off-diagonals separate values 
   */
  virtual void setRow(CFuint row, CFreal diagval, CFreal offdiagval){
    // CFint ncols,*cols;
    // CF_CHKERRCONTINUE(MatGetRow(m_mat,row,&ncols,(const ParalutionInt**)&cols,0));
    // ParalutionScalar *val=new ParalutionScalar[ncols],dv=(ParalutionScalar)diagval,odv=(ParalutionScalar)offdiagval;
    // for(CFint i=0; i<ncols; i++) val[i] = ((CFint)row==cols[i]) ? dv : odv;
    // CF_CHKERRCONTINUE(MatSetValues(m_mat,1,(const ParalutionInt*)&row,ncols,cols,val,INSERT_VALUES));
    // finalAssembly();
    // CF_CHKERRCONTINUE(MatRestoreRow(m_mat,row,&ncols,(const ParalutionInt**)&cols,0));
    // delete(val);
  }
  
  /**
   * Set the diagonal
   */
  void setDiagonal(Framework::LSSVector& diag)
  {
    // CF_CHKERRCONTINUE(MatDiagonalSet(m_mat,
    //   dynamic_cast<ParalutionVector&>(diag).getVec(),
    //   INSERT_VALUES));
  }

  /**
   * Add to the diagonal
   */
  void addToDiagonal(Framework::LSSVector& diag)
  {
    // CF_CHKERRCONTINUE(MatDiagonalSet(m_mat,
    //       dynamic_cast<ParalutionVector&>(diag).getVec(),
    //       ADD_VALUES));
  }

  /**
   * Reset to 0 all the non-zero elements of the matrix
   */
  void resetToZeroEntries()
  {
    for (CFint i=0; i<_size; i++){
      _val[i] = 0.0;
    }
    _nnz = 0;  
    m_mat.Clear();
    std::cout << "resetToZeroEntries() \n";
    // if (!_isMatShell) {
    //   CF_CHKERRCONTINUE(MatZeroEntries(m_mat));
    // }
  }

  /**
   * Set a list of values
   */
  void setValues(const Framework::BlockAccumulator& acc);

  /**
   * Add a list of values
   */
  void addValues(const Framework::BlockAccumulator& acc);

  /**
   * Freeze the matrix structure concerning the non zero
   * locations
   */
  void freezeNonZeroStructure()
  {
   //  MatSetOption(m_mat, MAT_NEW_NONZERO_LOCATIONS, PETSC_FALSE);
   // // MatSetOption(m_mat, MAT_NEW_NONZERO_LOCATIONS_ERR, PETSC_FALSE);
  }
  
protected:

  /// socket for the states
  Framework::DataSocketSink < Framework::State* , Framework::GLOBAL > socket_states;
  
  /// socket for the nodes
  Framework::DataSocketSink < Framework::Node* , Framework::GLOBAL > socket_nodes;


private: // data

  paralution::LocalMatrix<CFreal> m_mat;

  //MathTools::CFVec<CFint> _row;
  //MathTools::CFVec<CFint> _col;
  //RealVector _val;

  CFint* _row;
  CFint* _rowoff;
  CFint* _col;
  CFreal* _val;

  CFuint _size;

  CFuint _nnz;
}; // end of class ParalutionMatrix

//////////////////////////////////////////////////////////////////////////////

    } // namespace Paralution

} // namespace COOLFluiD

//////////////////////////////////////////////////////////////////////////////

#endif // COOLFluiD_Numerics_Paralution_ParalutionMatrix_hh