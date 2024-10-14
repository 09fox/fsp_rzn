/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_MMU_CFG_H
#define R_MMU_CFG_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Specify whether to include code for API parameter checking. Valid settings include:
 *    - BSP_CFG_PARAM_CHECKING_ENABLE : Utilizes the system default setting from bsp_cfg.h
 *    - 1 : Includes parameter checking
 *    - 0 : Compiles out parameter checking
 */
#ifndef MMU_CFG_PARAM_CHECKING_ENABLE
 #define MMU_CFG_PARAM_CHECKING_ENABLE    (BSP_CFG_PARAM_CHECKING_ENABLE)
#endif
#endif
