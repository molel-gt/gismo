/** @file gsBlockOp.h

    @brief Simple class create a block preconditioner structure.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): J. Sogn
*/
#pragma once

#include <gsCore/gsExport.h>
#include <gsCore/gsLinearAlgebra.h>
#include <gsSolver/gsLinearOperator.h>

namespace gismo
{

/** \brief Simple class create a block operator structure.
 *
 * This class represents a linear operator \f$C\f$ having block structure:
 * \f[
 *   C =
         \left( \begin{array}{cccc}
         C_{00} & C_{01} & \ldots & C_{0m}  \\
         C_{10} & C_{11} & \ldots & C_{1m}  \\
         \vdots & \vdots & \ddots & \vdots  \\
         C_{n0} & C_{n1} & \ldots & C_{nm}
         \end{array}
         \right),
   \f]
 * where \f$C_{ij}\f$ are themselves gsLinearOperators.
 *
 * The number of blocks (m and n) are specified in the constructor. The blocks \f$C_{ij}\f$ are
 * defined using addOperator(i,j,...). Unspecified blocks are considered to be 0.
 *
 * \ingroup Solver
 */
class GISMO_EXPORT gsBlockOp : public gsLinearOperator
{
public:

    /// Shared pointer for gsBlockOp
    typedef memory::shared_ptr< gsBlockOp > Ptr;

    /// Unique pointer for gsBlockOp
    typedef memory::unique< gsBlockOp >::ptr uPtr;
    
    /// Base class
    typedef memory::shared_ptr< gsLinearOperator > BasePtr;    
    
    /// Constructor. Takes the number of blocks (nRows, nCols). Provide the contents of the blocks with addOperator
    gsBlockOp(index_t nRows, index_t nCols);
    
    /// Make function returning a shared pointer
    static Ptr make(index_t nRows, index_t nCols) { return shared( new gsBlockOp(nRows,nCols) ); }

    /**
     * @brief Add a preconditioner \f$C_{ij}\f$ to the block structure
     * @param row row position in the block operator
     * @param col column position in the block operator
     * @param op shared pointer to the operator
     */
    void addOperator(index_t row, index_t col, const BasePtr& op);

    /**
     * @brief Apply the correct segment of the input vector on the preconditioners in the block structure and store the result.
     * @param input  Input vector
     * @param result Result vector
     */
    void apply(const gsMatrix<real_t> & input, gsMatrix<real_t> & result) const;

    index_t rows() const {return blockTargetPositions.sum();}
    index_t cols() const {return blockInputPositions.sum() ;}

private:

    Eigen::Array<BasePtr, Dynamic, Dynamic> blockPrec;

    //Contains the size of the target vector for each block
    gsVector<index_t> blockTargetPositions;
    //Contains the size of the input vector for each block
    gsVector<index_t> blockInputPositions;

};

} // namespace gismo

