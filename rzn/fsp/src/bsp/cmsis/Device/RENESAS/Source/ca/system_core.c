/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include <string.h>
#include "bsp_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ICC_SRE_EL3_BIT_Enable                  (1 << 3)
#define ICC_SRE_EL3_BIT_DIB                     (1 << 2)
#define ICC_SRE_EL3_BIT_DFB                     (1 << 1)
#define ICC_SRE_EL3_BIT_SRE                     (1 << 0)
#define ICC_SRE_EL1_BIT_SRE                     (1 << 0)
#define ICC_IGRPEN1_EL3_BIT_EnableGrp1S         (1 << 1)
#define ICC_IGRPEN1_EL3_BIT_EnableGrp1NS        (1 << 0)
#define ICC_PMR_EL1_VALID_INTERRUPT_PRIORITY    (0xFF)

#define GICD_CTLR_BIT_ARE_NS                    (1 << 5)
#define GICD_CTLR_BIT_ARE_S                     (1 << 4)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/
void bsp_memory_protect_setting(void);
void bsp_irq_cfg_common(void);

/*******************************************************************************************************************//**
 * Initialize memory protection settings.
 **********************************************************************************************************************/
void bsp_memory_protect_setting (void)
{
    R_BSP_CacheInvalidateAll();

    R_BSP_CacheEnableMemoryProtect();
    R_BSP_CacheEnableInst();
    R_BSP_CacheEnableData();
}

/*******************************************************************************************************************//**
 * Initialize common configuration settings for interrupts
 **********************************************************************************************************************/
void bsp_irq_cfg_common (void)
{
    uint64_t reg_value = 0;

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_SYSTEM);

    R_SSC->SSTPCR5_b.GIC600_REQ = 0;
    FSP_HARDWARE_REGISTER_WAIT(R_SSC->SSTPCR5_b.GIC600_ACK, 0);

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_SYSTEM);

    /* ICC_SRE_EL3 */
    reg_value  = 0;
    reg_value |= ICC_SRE_EL3_BIT_Enable;
    reg_value |= ICC_SRE_EL3_BIT_DIB;
    reg_value |= ICC_SRE_EL3_BIT_DFB;
    reg_value |= ICC_SRE_EL3_BIT_SRE;
    __set_CP(3, 6, reg_value, 12, 12, 5); // ICC_SRE_EL3

    /* ICC_SRE_EL1 */
    reg_value  = 0;
    reg_value |= ICC_SRE_EL1_BIT_SRE;
    __set_CP(3, 0, reg_value, 12, 12, 5); // ICC_SRE_EL1

    /* Separate interrupt priority value fields in Group0, Group1 */
    reg_value = 3;                        // Group priority [7:4], Sub priority [3:0]
    __set_CP(3, 0, reg_value, 12, 8, 3);  // ICC_BPR0_EL1
    __set_CP(3, 0, reg_value, 12, 12, 3); // ICC_BPR1_EL1

    /* Set None-secure and secure Affinity Routing Enable */
    reg_value = GICD_CTLR_BIT_ARE_NS | GICD_CTLR_BIT_ARE_S;
    R_BSP_GICD_AffinityRouteEnable((bsp_gicd_ctlr_bit_t) reg_value);

    R_BSP_GICR_Enable();

    R_BSP_GICR_SetSgiPpiSecurityLine(BSP_GIC_IGROUPR_G1NS);

    /* Set interrupt priority mask, priority from 0 to 255 is valid */
    reg_value = ICC_PMR_EL1_VALID_INTERRUPT_PRIORITY;
    __set_CP(3, 0, reg_value, 4, 6, 0); // ICC_PMR_EL1

    /* Set Group1 None-Secure to argument for SPI */
    R_BSP_GICD_SetSpiSecurityAll(BSP_GIC_IGROUPR_G1NS);

    /* Group1 None-secure and Secure are enabled */
    reg_value = ICC_IGRPEN1_EL3_BIT_EnableGrp1S | ICC_IGRPEN1_EL3_BIT_EnableGrp1NS;
    __set_CP(3, 6, reg_value, 12, 12, 7); // ICC_IGRPEN1_EL3
}
