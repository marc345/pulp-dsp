/* =====================================================================
 * Project:      PULP DSP Library
 * Title:        plp_mat_add_stride_i8s_xpulpv2.c
 * Description:  8-bit integer strided matrix addition for XPULPV2
 *
 * $Date:        1. July 2020
 * $Revision:    V0
 *
 * Target Processor: PULP cores
 * ===================================================================== */
/*
 * Copyright (C) 2020 ETH Zurich and University of Bologna.
 *
 * Author: Tibor Schneider, ETH Zurich
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
  @ingroup MatAddStride
 */

/**
  @addtogroup MatAddStrideKernels
  @{
 */

/**
  @brief strided matrix addition of 8-bit integer matrices kernel for XPULPV2 extension.
  @param[in]  pSrcA   Points to the first input matrix
  @param[in]  pSrcB   Points to the second input matrix
  @param[in]  M       Height of all matrices
  @param[in]  N       Width of all matrices
  @param[in]  strideA Stride of matrix A (elements between each row)
  @param[in]  strideB Stride of matrid B (elements between each row)
  @param[in]  strideY Stride of output matrix (elements between each row)
  @param[out] pDst    Points to the output matrix
  @return     none
 */

void plp_mat_add_stride_i8s_xpulpv2(const int8_t *__restrict__ pSrcA,
                                    const int8_t *__restrict__ pSrcB,
                                    uint32_t M,
                                    uint32_t N,
                                    uint32_t strideA,
                                    uint32_t strideB,
                                    uint32_t strideY,
                                    int8_t *__restrict__ pDst) {

//#define BASIC_VERSION // if used don't forget to also use the undefine at end of file
#ifdef BASIC_VERSION

    uint32_t m, n; // loop counters

    for (m = 0; m < M; m++) {
        for (n = 0; n < N; n++) {
            pDst[m * strideY + n] = pSrcA[m * strideA + n] + pSrcB[m * strideB + n];
        }
    }

#else

    uint32_t m, n; // loop counters

    const int8_t *__restrict__ pSrcA2 = pSrcA + strideA;
    const int8_t *__restrict__ pSrcB2 = pSrcB + strideB;
    int8_t *__restrict__ pDst2 = pDst + strideY;

    unsigned int m_iter = M >> 1;
    unsigned int m_rem = M & 0x1;

    unsigned int n_iter = N >> 2;
    unsigned int n_rem = N & 0x3;

    unsigned int step_a = strideA * 2 - N;
    unsigned int step_b = strideB * 2 - N;
    unsigned int step_y = strideY * 2 - N;

    for (m = 0; m < m_iter; m++) {
        for (n = 0; n < n_iter; n++) {
            v4s a1 = *((v4s *)pSrcA);
            v4s b1 = *((v4s *)pSrcB);
            v4s a2 = *((v4s *)pSrcA2);
            v4s b2 = *((v4s *)pSrcB2);
            *((v4s *)pDst) = __ADD4(a1, b1);
            *((v4s *)pDst2) = __ADD4(a2, b2);
            pSrcA += 4;
            pSrcB += 4;
            pDst += 4;
            pSrcA2 += 4;
            pSrcB2 += 4;
            pDst2 += 4;
        }
        for (n = 0; n < n_rem; n++) {
            *pDst++ = *pSrcA++ + *pSrcB++;
            *pDst2++ = *pSrcA2++ + *pSrcB2++;
        }
        pSrcA += step_a;
        pSrcB += step_b;
        pDst += step_y;
        pSrcA2 += step_a;
        pSrcB2 += step_b;
        pDst2 += step_y;
    }

    if (m_rem) {
        for (n = 0; n < n_iter; n++) {
            v4s a = *((v4s *)pSrcA);
            v4s b = *((v4s *)pSrcB);
            *((v4s *)pDst) = __ADD4(a, b);
            pSrcA += 4;
            pSrcB += 4;
            pDst += 4;
        }
        for (n = 0; n < n_rem; n++) {
            *pDst++ = *pSrcA++ + *pSrcB++;
        }
        pSrcA += step_a;
        pSrcB += step_b;
        pDst += step_y;
    }

#endif
    //#undef BASIC_VERSION
}
/**
   @} end of MatAddStrideKernels group
*/
