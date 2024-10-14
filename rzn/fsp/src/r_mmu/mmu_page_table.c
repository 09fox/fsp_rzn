/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_mmu.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

r_mmu_pgtbl_cfg_t const g_mmu_pagetable_array[] =
{
    /* vaddress, paddress,   size,       attribute */
    {0x00000000, 0x00000000, 0x10000000, R_MMU_PG_ATTRIBUTE_ACCESS_FAULT  },
    {0x10000000, 0x10000000, 0x00200000, R_MMU_PG_ATTRIBUTE_NORMAL_UNCACHE},
    {0x10200000, 0x10200000, 0x0FE00000, R_MMU_PG_ATTRIBUTE_ACCESS_FAULT  },
    {0x20000000, 0x20000000, 0x01200000, R_MMU_PG_ATTRIBUTE_NORMAL_UNCACHE},
    {0x21200000, 0x21200000, 0x1EE00000, R_MMU_PG_ATTRIBUTE_ACCESS_FAULT  },
    {0x40000000, 0x40000000, 0x20000000, R_MMU_PG_ATTRIBUTE_NORMAL_UNCACHE},
    {0x60000000, 0x60000000, 0x10000000, R_MMU_PG_ATTRIBUTE_ACCESS_FAULT  },
    {0x70000000, 0x70000000, 0x10000000, R_MMU_PG_ATTRIBUTE_NORMAL_UNCACHE},
    {0x80000000, 0x80000000, 0x02000000, R_MMU_PG_ATTRIBUTE_DEVICE_NGNRE  },
    {0x82000000, 0x82000000, 0x01000000, R_MMU_PG_ATTRIBUTE_ACCESS_FAULT  },
    {0x83000000, 0x83000000, 0x00200000, R_MMU_PG_ATTRIBUTE_DEVICE_NGNRNE },
    {0x83200000, 0x83200000, 0x04E00000, R_MMU_PG_ATTRIBUTE_ACCESS_FAULT  },
    {0x88000000, 0x88000000, 0x02000000, R_MMU_PG_ATTRIBUTE_DEVICE_NGNRE  },
    {0x8A000000, 0x8A000000, 0x02000000, R_MMU_PG_ATTRIBUTE_ACCESS_FAULT  },
    {0x8C000000, 0x8C000000, 0x01000000, R_MMU_PG_ATTRIBUTE_DEVICE_NGNRNE },
    {0x8D000000, 0x8D000000, 0x03000000, R_MMU_PG_ATTRIBUTE_ACCESS_FAULT  },
    {0x90000000, 0x90000000, 0x00400000, R_MMU_PG_ATTRIBUTE_DEVICE_NGNRE  },
    {0x90400000, 0x90400000, 0x01C00000, R_MMU_PG_ATTRIBUTE_ACCESS_FAULT  },
    {0x92000000, 0x92000000, 0x01000000, R_MMU_PG_ATTRIBUTE_DEVICE_NGNRE  },
    {0x93000000, 0x93000000, 0x2D000000, R_MMU_PG_ATTRIBUTE_ACCESS_FAULT  },
    {0xC0000000, 0xC0000000, 0x40000000, R_MMU_PG_ATTRIBUTE_ACCESS_FAULT  },
    {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF                       }
};

/*******************************************************************************************************************//**
 * @addtogroup MMU
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Convert virtual address to physical address.
 *
 * @param[in]  p_api_ctrl        MMU driver control block.
 * @param[in]  vaddress          Virtual address to convert.
 * @param[out] p_paddress        Pointer to store physical address.
 * @retval FSP_SUCCESS           successful
 **********************************************************************************************************************/
fsp_err_t R_MMU_VAtoPA (mmu_ctrl_t * const p_api_ctrl, uint64_t vaddress, uint64_t * p_paddress)
{
    fsp_err_t err = FSP_SUCCESS;

    FSP_PARAMETER_NOT_USED(p_api_ctrl);

    *p_paddress = vaddress;

    return err;
}

/*******************************************************************************************************************//**
 * Convert physical address to virtual address.
 *
 * @param[in]  p_api_ctrl        MMU driver control block.
 * @param[in]  paddress          Physical address to convert.
 * @param[out] p_vaddress        Pointer to store virtual address.
 * @retval FSP_SUCCESS           successful
 **********************************************************************************************************************/
fsp_err_t R_MMU_PAtoVA (mmu_ctrl_t * const p_api_ctrl, uint64_t paddress, uint64_t * p_vaddress)
{
    fsp_err_t err = FSP_SUCCESS;

    FSP_PARAMETER_NOT_USED(p_api_ctrl);

    *p_vaddress = paddress;

    return err;
}

/** @} (end addtogroup MMU) */
