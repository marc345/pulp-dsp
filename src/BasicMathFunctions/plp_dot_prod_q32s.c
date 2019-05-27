/* =====================================================================
 * Project:      PULP DSP Library
 * Title:        plp_dot_prod_int32_scalar.c
 * Description:  32-bit integer scalar dot product
 *
 * $Date:        16. May 2019
 * $Revision:    V0
 *
 * Target Processor: PULP cores
 * ===================================================================== */
/*
 * Copyright (C) 2019 Xiaying Wang, IIS, ETH Zurich. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "plp_math.h"


/**
  @ingroup groupMath
 */

/**
  @addtogroup BasicDotProd
  @{
 */

/**
  @brief Glue code for scalar dot product of 32-bit fixed point vectors.
  @param[in]  pSrcA      points to the first input vector
  @param[in]  pSrcB      points to the second input vector
  @param[in]  blockSize  number of samples in each vector
  @param[in]  deciPoint  decimal point for right shift
  @param[out] result     output result returned here
  @return        none
 */

void plp_dot_prod_q32s(
                         const int32_t * pSrcA,
                         const int32_t * pSrcB,
                         uint32_t blockSize,
                         uint32_t deciPoint,
                         int32_t * pRes) {


  if (rt_cluster_id() == ARCHI_FC_CID){
    plp_dot_prod_q32s_rv32im(pSrcA, pSrcB, blockSize, deciPoint, pRes);
  }
  else{
    plp_dot_prod_q32s_xpulpv2(pSrcA, pSrcB, blockSize, deciPoint, pRes);
  }

}

/**
  @} end of BasicDotProd group
 */

