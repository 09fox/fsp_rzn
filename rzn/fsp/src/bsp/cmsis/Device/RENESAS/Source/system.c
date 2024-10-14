/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#if (1 == BSP_FEATURE_BSP_MASTER_MPU_REGION_TYPE)
 #define BSP_PRV_MASTER_MPU_REGION_NUM      (8)
#elif (2 == BSP_FEATURE_BSP_MASTER_MPU_REGION_TYPE)
 #define BSP_PRV_MASTER_MPU_REGION_NUM      (16)
#endif
#define BSP_PRV_M_MPU_35BIT_ADDRESS_MASK    (0x700000000UL)

#define BSP_PRV_MASTER_MPU_STADD(master,                                                         \
                                 region)     (((BSP_CFG_MPU ## master ## _STADD ## region) &~    \
                                               BSP_PRV_M_MPU_35BIT_ADDRESS_MASK) |               \
                                              (((BSP_CFG_MPU ## master ## _STADD ## region) &    \
                                                BSP_PRV_M_MPU_35BIT_ADDRESS_MASK) >> 28) |       \
                                              (BSP_CFG_MPU ## master ## _WRITE ## region << 1) | \
                                              BSP_CFG_MPU ## master ## _READ ## region)

#define BSP_PRV_MASTER_MPU_ENDADD(master,                                                     \
                                  region)    ((BSP_CFG_MPU ## master ## _ENDADD ## region) &~ \
                                              BSP_PRV_M_MPU_35BIT_ADDRESS_MASK) |             \
    (((BSP_CFG_MPU ## master ## _ENDADD ## region) &                                          \
      BSP_PRV_M_MPU_35BIT_ADDRESS_MASK) >> 28)

#if defined(__ICCARM__)
 #if BSP_CFG_C_RUNTIME_INIT
  #define BSP_PRV_SECTION_LDR_DATA_ROM_ADDRESS                  __section_begin("LDR_DATA_RBLOCK")
  #define BSP_PRV_SECTION_LDR_DATA_RAM_START                    __section_begin("LDR_DATA_WBLOCK")
  #define BSP_PRV_SECTION_LDR_DATA_RAM_END                      __section_end("LDR_DATA_WBLOCK")

  #define BSP_PRV_SECTION_LDR_DATA_BSS_START                    __section_begin("LDR_DATA_ZBLOCK")
  #define BSP_PRV_SECTION_LDR_DATA_BSS_END                      __section_end("LDR_DATA_ZBLOCK")

 #endif

 #if !(BSP_CFG_RAM_EXECUTION)
  #define BSP_PRV_SECTION_VECTOR_ROM_ADDRESS                    __section_begin("VECTOR_RBLOCK")
  #define BSP_PRV_SECTION_VECTOR_RAM_START                      __section_begin("VECTOR_WBLOCK")
  #define BSP_PRV_SECTION_VECTOR_RAM_END                        __section_end("VECTOR_WBLOCK")

  #define BSP_PRV_SECTION_USER_PRG_ROM_ADDRESS                  __section_begin("USER_PRG_RBLOCK")
  #define BSP_PRV_SECTION_USER_PRG_RAM_START                    __section_begin("USER_PRG_WBLOCK")
  #define BSP_PRV_SECTION_USER_PRG_RAM_END                      __section_end("USER_PRG_WBLOCK")

  #define BSP_PRV_SECTION_USER_DATA_ROM_ADDRESS                 __section_begin("USER_DATA_RBLOCK")
  #define BSP_PRV_SECTION_USER_DATA_RAM_START                   __section_begin("USER_DATA_WBLOCK")
  #define BSP_PRV_SECTION_USER_DATA_RAM_END                     __section_end("USER_DATA_WBLOCK")

  #define BSP_PRV_SECTION_USER_DATA_NONCACHE_ROM_ADDRESS        __section_begin("USER_DATA_NONCACHE_RBLOCK")
  #define BSP_PRV_SECTION_USER_DATA_NONCACHE_RAM_START          __section_begin("USER_DATA_NONCACHE_WBLOCK")
  #define BSP_PRV_SECTION_USER_DATA_NONCACHE_RAM_END            __section_end("USER_DATA_NONCACHE_WBLOCK")

  #define BSP_PRV_SECTION_DMAC_LINK_MODE_ROM_ADDRESS            __section_begin("DMAC_LINK_MODE_RBLOCK")
  #define BSP_PRV_SECTION_DMAC_LINK_MODE_RAM_START              __section_begin("DMAC_LINK_MODE_WBLOCK")
  #define BSP_PRV_SECTION_DMAC_LINK_MODE_RAM_END                __section_end("DMAC_LINK_MODE_WBLOCK")

  #define BSP_PRV_SECTION_SHARED_NONCACHE_BUFFER_ROM_ADDRESS    __section_begin("SHARED_NONCACHE_BUFFER_RBLOCK")
  #define BSP_PRV_SECTION_SHARED_NONCACHE_BUFFER_RAM_START      __section_begin("SHARED_NONCACHE_BUFFER_WBLOCK")
  #define BSP_PRV_SECTION_SHARED_NONCACHE_BUFFER_RAM_END        __section_end("SHARED_NONCACHE_BUFFER_WBLOCK")

  #define BSP_PRV_SECTION_NONCACHE_BUFFER_ROM_ADDRESS           __section_begin("NONCACHE_BUFFER_RBLOCK")
  #define BSP_PRV_SECTION_NONCACHE_BUFFER_RAM_START             __section_begin("NONCACHE_BUFFER_WBLOCK")
  #define BSP_PRV_SECTION_NONCACHE_BUFFER_RAM_END               __section_end("NONCACHE_BUFFER_WBLOCK")

  #define BSP_PRV_SECTION_USER_DATA_BSS_START                   __section_begin("USER_DATA_ZBLOCK")
  #define BSP_PRV_SECTION_USER_DATA_BSS_END                     __section_end("USER_DATA_ZBLOCK")

 #endif

#elif defined(__GNUC__)
 #if BSP_CFG_C_RUNTIME_INIT
  #define BSP_PRV_SECTION_LDR_DATA_ROM_ADDRESS                  &_mloader_data
  #define BSP_PRV_SECTION_LDR_DATA_RAM_START                    &__loader_data_start
  #define BSP_PRV_SECTION_LDR_DATA_RAM_END                      &__loader_data_end

  #define BSP_PRV_SECTION_LDR_DATA_BSS_START                    &__loader_bss_start
  #define BSP_PRV_SECTION_LDR_DATA_BSS_END                      &__loader_bss_end

 #endif

 #if !(BSP_CFG_RAM_EXECUTION)
  #define BSP_PRV_SECTION_VECTOR_ROM_ADDRESS                    &_mfvector
  #define BSP_PRV_SECTION_VECTOR_RAM_START                      &_fvector_start
  #define BSP_PRV_SECTION_VECTOR_RAM_END                        &_fvector_end

  #define BSP_PRV_SECTION_USER_PRG_ROM_ADDRESS                  &_mtext
  #define BSP_PRV_SECTION_USER_PRG_RAM_START                    &_text_start
  #define BSP_PRV_SECTION_USER_PRG_RAM_END                      &_text_end

  #define BSP_PRV_SECTION_USER_DATA_ROM_ADDRESS                 &_mdata
  #define BSP_PRV_SECTION_USER_DATA_RAM_START                   &_data_start
  #define BSP_PRV_SECTION_USER_DATA_RAM_END                     &_data_end

  #define BSP_PRV_SECTION_USER_DATA_NONCACHE_ROM_ADDRESS        &_mdata_noncache
  #define BSP_PRV_SECTION_USER_DATA_NONCACHE_RAM_START          &_data_noncache_start
  #define BSP_PRV_SECTION_USER_DATA_NONCACHE_RAM_END            &_data_noncache_end

  #define BSP_PRV_SECTION_DMAC_LINK_MODE_ROM_ADDRESS            &_mdmac_link_mode
  #define BSP_PRV_SECTION_DMAC_LINK_MODE_RAM_START              &_dmac_link_mode_start
  #define BSP_PRV_SECTION_DMAC_LINK_MODE_RAM_END                &_dmac_link_mode_end

  #define BSP_PRV_SECTION_SHARED_NONCACHE_BUFFER_ROM_ADDRESS    &_msncbuffer
  #define BSP_PRV_SECTION_SHARED_NONCACHE_BUFFER_RAM_START      &_sncbuffer_start
  #define BSP_PRV_SECTION_SHARED_NONCACHE_BUFFER_RAM_END        &_sncbuffer_end

  #define BSP_PRV_SECTION_NONCACHE_BUFFER_ROM_ADDRESS           &_mncbuffer
  #define BSP_PRV_SECTION_NONCACHE_BUFFER_RAM_START             &_ncbuffer_start
  #define BSP_PRV_SECTION_NONCACHE_BUFFER_RAM_END               &_ncbuffer_end

 #endif

 #define BSP_PRV_SECTION_USER_DATA_BSS_START                    &__bss_start__
 #define BSP_PRV_SECTION_USER_DATA_BSS_END                      &__bss_end__

#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU
 * @{
 **********************************************************************************************************************/

/** System Clock Frequency (Core Clock) */
uint32_t SystemCoreClock = 0U;

/** @} (end addtogroup BSP_MCU) */

#if defined(__ICCARM__)
 #if BSP_CFG_C_RUNTIME_INIT
  #pragma section="LDR_DATA_RBLOCK"
  #pragma section="LDR_DATA_WBLOCK"
  #pragma section="LDR_DATA_ZBLOCK"

 #endif

 #if !(BSP_CFG_RAM_EXECUTION)
  #pragma section="VECTOR_RBLOCK"
  #pragma section="VECTOR_WBLOCK"

  #pragma section="USER_PRG_RBLOCK"
  #pragma section="USER_PRG_WBLOCK"

  #pragma section="USER_DATA_RBLOCK"
  #pragma section="USER_DATA_WBLOCK"
  #pragma section="USER_DATA_ZBLOCK"

  #pragma section="USER_DATA_NONCACHE_RBLOCK"
  #pragma section="USER_DATA_NONCACHE_WBLOCK"

  #pragma section="DMAC_LINK_MODE_RBLOCK"
  #pragma section="DMAC_LINK_MODE_WBLOCK"

  #pragma section="SHARED_NONCACHE_BUFFER_RBLOCK"
  #pragma section="SHARED_NONCACHE_BUFFER_WBLOCK"

  #pragma section="NONCACHE_BUFFER_RBLOCK"
  #pragma section="NONCACHE_BUFFER_WBLOCK"

 #endif

#elif defined(__GNUC__)
 #if BSP_CFG_C_RUNTIME_INIT
extern void * _mloader_data;
extern void * __loader_data_start;
extern void * __loader_data_end;

extern void * __loader_bss_start;
extern void * __loader_bss_end;

extern void (* __preinit_array_start[])(void);
extern void (* __preinit_array_end[])(void);
extern void (* __init_array_start[])(void);
extern void (* __init_array_end[])(void);

 #endif

 #if !(BSP_CFG_RAM_EXECUTION)
extern void * _mfvector;
extern void * _fvector_start;
extern void * _fvector_end;

extern void * _mtext;
extern void * _text_start;
extern void * _text_end;

extern void * _mdata;
extern void * _data_start;
extern void * _data_end;

extern void * _mdata_noncache;
extern void * _data_noncache_start;
extern void * _data_noncache_end;

extern void * _mdmac_link_mode;
extern void * _dmac_link_mode_start;
extern void * _dmac_link_mode_end;

extern void * _msncbuffer;
extern void * _sncbuffer_start;
extern void * _sncbuffer_end;

extern void * _mncbuffer;
extern void * _ncbuffer_start;
extern void * _ncbuffer_end;

 #endif

extern void * __bss_start__;
extern void * __bss_end__;

#endif

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/
#if defined(__ICCARM__)
 #if BSP_CFG_C_RUNTIME_INIT
extern void __iar_data_init3(void);

 #endif
#endif

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/
#if (1 == BSP_FEATURE_BSP_MASTER_MPU0_SUPPORTED)
const uint32_t g_bsp_master_mpu0_cfg[BSP_PRV_MASTER_MPU_REGION_NUM][2] =
{
    {BSP_PRV_MASTER_MPU_STADD(0, 0),  BSP_PRV_MASTER_MPU_ENDADD(0, 0) },
    {BSP_PRV_MASTER_MPU_STADD(0, 1),  BSP_PRV_MASTER_MPU_ENDADD(0, 1) },
    {BSP_PRV_MASTER_MPU_STADD(0, 2),  BSP_PRV_MASTER_MPU_ENDADD(0, 2) },
    {BSP_PRV_MASTER_MPU_STADD(0, 3),  BSP_PRV_MASTER_MPU_ENDADD(0, 3) },
    {BSP_PRV_MASTER_MPU_STADD(0, 4),  BSP_PRV_MASTER_MPU_ENDADD(0, 4) },
    {BSP_PRV_MASTER_MPU_STADD(0, 5),  BSP_PRV_MASTER_MPU_ENDADD(0, 5) },
    {BSP_PRV_MASTER_MPU_STADD(0, 6),  BSP_PRV_MASTER_MPU_ENDADD(0, 6) },
    {BSP_PRV_MASTER_MPU_STADD(0, 7),  BSP_PRV_MASTER_MPU_ENDADD(0, 7) },
 #if (2 == BSP_FEATURE_BSP_MASTER_MPU_REGION_TYPE)
    {BSP_PRV_MASTER_MPU_STADD(0, 8),  BSP_PRV_MASTER_MPU_ENDADD(0, 8) },
    {BSP_PRV_MASTER_MPU_STADD(0, 9),  BSP_PRV_MASTER_MPU_ENDADD(0, 9) },
    {BSP_PRV_MASTER_MPU_STADD(0, 10), BSP_PRV_MASTER_MPU_ENDADD(0, 10)},
    {BSP_PRV_MASTER_MPU_STADD(0, 11), BSP_PRV_MASTER_MPU_ENDADD(0, 11)},
    {BSP_PRV_MASTER_MPU_STADD(0, 12), BSP_PRV_MASTER_MPU_ENDADD(0, 12)},
    {BSP_PRV_MASTER_MPU_STADD(0, 13), BSP_PRV_MASTER_MPU_ENDADD(0, 13)},
    {BSP_PRV_MASTER_MPU_STADD(0, 14), BSP_PRV_MASTER_MPU_ENDADD(0, 14)},
    {BSP_PRV_MASTER_MPU_STADD(0, 15), BSP_PRV_MASTER_MPU_ENDADD(0, 15)},
 #endif
};
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU1_SUPPORTED)
const uint32_t g_bsp_master_mpu1_cfg[BSP_PRV_MASTER_MPU_REGION_NUM][2] =
{
    {BSP_PRV_MASTER_MPU_STADD(1, 0),  BSP_PRV_MASTER_MPU_ENDADD(1, 0) },
    {BSP_PRV_MASTER_MPU_STADD(1, 1),  BSP_PRV_MASTER_MPU_ENDADD(1, 1) },
    {BSP_PRV_MASTER_MPU_STADD(1, 2),  BSP_PRV_MASTER_MPU_ENDADD(1, 2) },
    {BSP_PRV_MASTER_MPU_STADD(1, 3),  BSP_PRV_MASTER_MPU_ENDADD(1, 3) },
    {BSP_PRV_MASTER_MPU_STADD(1, 4),  BSP_PRV_MASTER_MPU_ENDADD(1, 4) },
    {BSP_PRV_MASTER_MPU_STADD(1, 5),  BSP_PRV_MASTER_MPU_ENDADD(1, 5) },
    {BSP_PRV_MASTER_MPU_STADD(1, 6),  BSP_PRV_MASTER_MPU_ENDADD(1, 6) },
    {BSP_PRV_MASTER_MPU_STADD(1, 7),  BSP_PRV_MASTER_MPU_ENDADD(1, 7) },
 #if (2 == BSP_FEATURE_BSP_MASTER_MPU_REGION_TYPE)
    {BSP_PRV_MASTER_MPU_STADD(1, 8),  BSP_PRV_MASTER_MPU_ENDADD(1, 8) },
    {BSP_PRV_MASTER_MPU_STADD(1, 9),  BSP_PRV_MASTER_MPU_ENDADD(1, 9) },
    {BSP_PRV_MASTER_MPU_STADD(1, 10), BSP_PRV_MASTER_MPU_ENDADD(1, 10)},
    {BSP_PRV_MASTER_MPU_STADD(1, 11), BSP_PRV_MASTER_MPU_ENDADD(1, 11)},
    {BSP_PRV_MASTER_MPU_STADD(1, 12), BSP_PRV_MASTER_MPU_ENDADD(1, 12)},
    {BSP_PRV_MASTER_MPU_STADD(1, 13), BSP_PRV_MASTER_MPU_ENDADD(1, 13)},
    {BSP_PRV_MASTER_MPU_STADD(1, 14), BSP_PRV_MASTER_MPU_ENDADD(1, 14)},
    {BSP_PRV_MASTER_MPU_STADD(1, 15), BSP_PRV_MASTER_MPU_ENDADD(1, 15)},
 #endif
};
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU2_SUPPORTED)
const uint32_t g_bsp_master_mpu2_cfg[BSP_PRV_MASTER_MPU_REGION_NUM][2] =
{
    {BSP_PRV_MASTER_MPU_STADD(2, 0),  BSP_PRV_MASTER_MPU_ENDADD(2, 0) },
    {BSP_PRV_MASTER_MPU_STADD(2, 1),  BSP_PRV_MASTER_MPU_ENDADD(2, 1) },
    {BSP_PRV_MASTER_MPU_STADD(2, 2),  BSP_PRV_MASTER_MPU_ENDADD(2, 2) },
    {BSP_PRV_MASTER_MPU_STADD(2, 3),  BSP_PRV_MASTER_MPU_ENDADD(2, 3) },
    {BSP_PRV_MASTER_MPU_STADD(2, 4),  BSP_PRV_MASTER_MPU_ENDADD(2, 4) },
    {BSP_PRV_MASTER_MPU_STADD(2, 5),  BSP_PRV_MASTER_MPU_ENDADD(2, 5) },
    {BSP_PRV_MASTER_MPU_STADD(2, 6),  BSP_PRV_MASTER_MPU_ENDADD(2, 6) },
    {BSP_PRV_MASTER_MPU_STADD(2, 7),  BSP_PRV_MASTER_MPU_ENDADD(2, 7) },
 #if (2 == BSP_FEATURE_BSP_MASTER_MPU_REGION_TYPE)
    {BSP_PRV_MASTER_MPU_STADD(2, 8),  BSP_PRV_MASTER_MPU_ENDADD(2, 8) },
    {BSP_PRV_MASTER_MPU_STADD(2, 9),  BSP_PRV_MASTER_MPU_ENDADD(2, 9) },
    {BSP_PRV_MASTER_MPU_STADD(2, 10), BSP_PRV_MASTER_MPU_ENDADD(2, 10)},
    {BSP_PRV_MASTER_MPU_STADD(2, 11), BSP_PRV_MASTER_MPU_ENDADD(2, 11)},
    {BSP_PRV_MASTER_MPU_STADD(2, 12), BSP_PRV_MASTER_MPU_ENDADD(2, 12)},
    {BSP_PRV_MASTER_MPU_STADD(2, 13), BSP_PRV_MASTER_MPU_ENDADD(2, 13)},
    {BSP_PRV_MASTER_MPU_STADD(2, 14), BSP_PRV_MASTER_MPU_ENDADD(2, 14)},
    {BSP_PRV_MASTER_MPU_STADD(2, 15), BSP_PRV_MASTER_MPU_ENDADD(2, 15)},
 #endif
};
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU3_SUPPORTED)
const uint32_t g_bsp_master_mpu3_cfg[BSP_PRV_MASTER_MPU_REGION_NUM][2] =
{
    {BSP_PRV_MASTER_MPU_STADD(3, 0),  BSP_PRV_MASTER_MPU_ENDADD(3, 0) },
    {BSP_PRV_MASTER_MPU_STADD(3, 1),  BSP_PRV_MASTER_MPU_ENDADD(3, 1) },
    {BSP_PRV_MASTER_MPU_STADD(3, 2),  BSP_PRV_MASTER_MPU_ENDADD(3, 2) },
    {BSP_PRV_MASTER_MPU_STADD(3, 3),  BSP_PRV_MASTER_MPU_ENDADD(3, 3) },
    {BSP_PRV_MASTER_MPU_STADD(3, 4),  BSP_PRV_MASTER_MPU_ENDADD(3, 4) },
    {BSP_PRV_MASTER_MPU_STADD(3, 5),  BSP_PRV_MASTER_MPU_ENDADD(3, 5) },
    {BSP_PRV_MASTER_MPU_STADD(3, 6),  BSP_PRV_MASTER_MPU_ENDADD(3, 6) },
    {BSP_PRV_MASTER_MPU_STADD(3, 7),  BSP_PRV_MASTER_MPU_ENDADD(3, 7) },
 #if (2 == BSP_FEATURE_BSP_MASTER_MPU_REGION_TYPE)
    {BSP_PRV_MASTER_MPU_STADD(3, 8),  BSP_PRV_MASTER_MPU_ENDADD(3, 8) },
    {BSP_PRV_MASTER_MPU_STADD(3, 9),  BSP_PRV_MASTER_MPU_ENDADD(3, 9) },
    {BSP_PRV_MASTER_MPU_STADD(3, 10), BSP_PRV_MASTER_MPU_ENDADD(3, 10)},
    {BSP_PRV_MASTER_MPU_STADD(3, 11), BSP_PRV_MASTER_MPU_ENDADD(3, 11)},
    {BSP_PRV_MASTER_MPU_STADD(3, 12), BSP_PRV_MASTER_MPU_ENDADD(3, 12)},
    {BSP_PRV_MASTER_MPU_STADD(3, 13), BSP_PRV_MASTER_MPU_ENDADD(3, 13)},
    {BSP_PRV_MASTER_MPU_STADD(3, 14), BSP_PRV_MASTER_MPU_ENDADD(3, 14)},
    {BSP_PRV_MASTER_MPU_STADD(3, 15), BSP_PRV_MASTER_MPU_ENDADD(3, 15)},
 #endif
};
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU4_SUPPORTED)
const uint32_t g_bsp_master_mpu4_cfg[BSP_PRV_MASTER_MPU_REGION_NUM][2] =
{
    {BSP_PRV_MASTER_MPU_STADD(4, 0),  BSP_PRV_MASTER_MPU_ENDADD(4, 0) },
    {BSP_PRV_MASTER_MPU_STADD(4, 1),  BSP_PRV_MASTER_MPU_ENDADD(4, 1) },
    {BSP_PRV_MASTER_MPU_STADD(4, 2),  BSP_PRV_MASTER_MPU_ENDADD(4, 2) },
    {BSP_PRV_MASTER_MPU_STADD(4, 3),  BSP_PRV_MASTER_MPU_ENDADD(4, 3) },
    {BSP_PRV_MASTER_MPU_STADD(4, 4),  BSP_PRV_MASTER_MPU_ENDADD(4, 4) },
    {BSP_PRV_MASTER_MPU_STADD(4, 5),  BSP_PRV_MASTER_MPU_ENDADD(4, 5) },
    {BSP_PRV_MASTER_MPU_STADD(4, 6),  BSP_PRV_MASTER_MPU_ENDADD(4, 6) },
    {BSP_PRV_MASTER_MPU_STADD(4, 7),  BSP_PRV_MASTER_MPU_ENDADD(4, 7) },
 #if (2 == BSP_FEATURE_BSP_MASTER_MPU_REGION_TYPE)
    {BSP_PRV_MASTER_MPU_STADD(4, 8),  BSP_PRV_MASTER_MPU_ENDADD(4, 8) },
    {BSP_PRV_MASTER_MPU_STADD(4, 9),  BSP_PRV_MASTER_MPU_ENDADD(4, 9) },
    {BSP_PRV_MASTER_MPU_STADD(4, 10), BSP_PRV_MASTER_MPU_ENDADD(4, 10)},
    {BSP_PRV_MASTER_MPU_STADD(4, 11), BSP_PRV_MASTER_MPU_ENDADD(4, 11)},
    {BSP_PRV_MASTER_MPU_STADD(4, 12), BSP_PRV_MASTER_MPU_ENDADD(4, 12)},
    {BSP_PRV_MASTER_MPU_STADD(4, 13), BSP_PRV_MASTER_MPU_ENDADD(4, 13)},
    {BSP_PRV_MASTER_MPU_STADD(4, 14), BSP_PRV_MASTER_MPU_ENDADD(4, 14)},
    {BSP_PRV_MASTER_MPU_STADD(4, 15), BSP_PRV_MASTER_MPU_ENDADD(4, 15)},
 #endif
};
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU5_SUPPORTED)
const uint32_t g_bsp_master_mpu5_cfg[BSP_PRV_MASTER_MPU_REGION_NUM][2] =
{
    {BSP_PRV_MASTER_MPU_STADD(5, 0),  BSP_PRV_MASTER_MPU_ENDADD(5, 0) },
    {BSP_PRV_MASTER_MPU_STADD(5, 1),  BSP_PRV_MASTER_MPU_ENDADD(5, 1) },
    {BSP_PRV_MASTER_MPU_STADD(5, 2),  BSP_PRV_MASTER_MPU_ENDADD(5, 2) },
    {BSP_PRV_MASTER_MPU_STADD(5, 3),  BSP_PRV_MASTER_MPU_ENDADD(5, 3) },
    {BSP_PRV_MASTER_MPU_STADD(5, 4),  BSP_PRV_MASTER_MPU_ENDADD(5, 4) },
    {BSP_PRV_MASTER_MPU_STADD(5, 5),  BSP_PRV_MASTER_MPU_ENDADD(5, 5) },
    {BSP_PRV_MASTER_MPU_STADD(5, 6),  BSP_PRV_MASTER_MPU_ENDADD(5, 6) },
    {BSP_PRV_MASTER_MPU_STADD(5, 7),  BSP_PRV_MASTER_MPU_ENDADD(5, 7) },
 #if (2 == BSP_FEATURE_BSP_MASTER_MPU_REGION_TYPE)
    {BSP_PRV_MASTER_MPU_STADD(5, 8),  BSP_PRV_MASTER_MPU_ENDADD(5, 8) },
    {BSP_PRV_MASTER_MPU_STADD(5, 9),  BSP_PRV_MASTER_MPU_ENDADD(5, 9) },
    {BSP_PRV_MASTER_MPU_STADD(5, 10), BSP_PRV_MASTER_MPU_ENDADD(5, 10)},
    {BSP_PRV_MASTER_MPU_STADD(5, 11), BSP_PRV_MASTER_MPU_ENDADD(5, 11)},
    {BSP_PRV_MASTER_MPU_STADD(5, 12), BSP_PRV_MASTER_MPU_ENDADD(5, 12)},
    {BSP_PRV_MASTER_MPU_STADD(5, 13), BSP_PRV_MASTER_MPU_ENDADD(5, 13)},
    {BSP_PRV_MASTER_MPU_STADD(5, 14), BSP_PRV_MASTER_MPU_ENDADD(5, 14)},
    {BSP_PRV_MASTER_MPU_STADD(5, 15), BSP_PRV_MASTER_MPU_ENDADD(5, 15)},
 #endif
};
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU6_SUPPORTED)
const uint32_t g_bsp_master_mpu6_cfg[BSP_PRV_MASTER_MPU_REGION_NUM][2] =
{
    {BSP_PRV_MASTER_MPU_STADD(6, 0),  BSP_PRV_MASTER_MPU_ENDADD(6, 0) },
    {BSP_PRV_MASTER_MPU_STADD(6, 1),  BSP_PRV_MASTER_MPU_ENDADD(6, 1) },
    {BSP_PRV_MASTER_MPU_STADD(6, 2),  BSP_PRV_MASTER_MPU_ENDADD(6, 2) },
    {BSP_PRV_MASTER_MPU_STADD(6, 3),  BSP_PRV_MASTER_MPU_ENDADD(6, 3) },
    {BSP_PRV_MASTER_MPU_STADD(6, 4),  BSP_PRV_MASTER_MPU_ENDADD(6, 4) },
    {BSP_PRV_MASTER_MPU_STADD(6, 5),  BSP_PRV_MASTER_MPU_ENDADD(6, 5) },
    {BSP_PRV_MASTER_MPU_STADD(6, 6),  BSP_PRV_MASTER_MPU_ENDADD(6, 6) },
    {BSP_PRV_MASTER_MPU_STADD(6, 7),  BSP_PRV_MASTER_MPU_ENDADD(6, 7) },
 #if (2 == BSP_FEATURE_BSP_MASTER_MPU_REGION_TYPE)
    {BSP_PRV_MASTER_MPU_STADD(6, 8),  BSP_PRV_MASTER_MPU_ENDADD(6, 8) },
    {BSP_PRV_MASTER_MPU_STADD(6, 9),  BSP_PRV_MASTER_MPU_ENDADD(6, 9) },
    {BSP_PRV_MASTER_MPU_STADD(6, 10), BSP_PRV_MASTER_MPU_ENDADD(6, 10)},
    {BSP_PRV_MASTER_MPU_STADD(6, 11), BSP_PRV_MASTER_MPU_ENDADD(6, 11)},
    {BSP_PRV_MASTER_MPU_STADD(6, 12), BSP_PRV_MASTER_MPU_ENDADD(6, 12)},
    {BSP_PRV_MASTER_MPU_STADD(6, 13), BSP_PRV_MASTER_MPU_ENDADD(6, 13)},
    {BSP_PRV_MASTER_MPU_STADD(6, 14), BSP_PRV_MASTER_MPU_ENDADD(6, 14)},
    {BSP_PRV_MASTER_MPU_STADD(6, 15), BSP_PRV_MASTER_MPU_ENDADD(6, 15)},
 #endif
};
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU7_SUPPORTED)
const uint32_t g_bsp_master_mpu7_cfg[BSP_PRV_MASTER_MPU_REGION_NUM][2] =
{
    {BSP_PRV_MASTER_MPU_STADD(7, 0),  BSP_PRV_MASTER_MPU_ENDADD(7, 0) },
    {BSP_PRV_MASTER_MPU_STADD(7, 1),  BSP_PRV_MASTER_MPU_ENDADD(7, 1) },
    {BSP_PRV_MASTER_MPU_STADD(7, 2),  BSP_PRV_MASTER_MPU_ENDADD(7, 2) },
    {BSP_PRV_MASTER_MPU_STADD(7, 3),  BSP_PRV_MASTER_MPU_ENDADD(7, 3) },
    {BSP_PRV_MASTER_MPU_STADD(7, 4),  BSP_PRV_MASTER_MPU_ENDADD(7, 4) },
    {BSP_PRV_MASTER_MPU_STADD(7, 5),  BSP_PRV_MASTER_MPU_ENDADD(7, 5) },
    {BSP_PRV_MASTER_MPU_STADD(7, 6),  BSP_PRV_MASTER_MPU_ENDADD(7, 6) },
    {BSP_PRV_MASTER_MPU_STADD(7, 7),  BSP_PRV_MASTER_MPU_ENDADD(7, 7) },
 #if (2 == BSP_FEATURE_BSP_MASTER_MPU_REGION_TYPE)
    {BSP_PRV_MASTER_MPU_STADD(7, 8),  BSP_PRV_MASTER_MPU_ENDADD(7, 8) },
    {BSP_PRV_MASTER_MPU_STADD(7, 9),  BSP_PRV_MASTER_MPU_ENDADD(7, 9) },
    {BSP_PRV_MASTER_MPU_STADD(7, 10), BSP_PRV_MASTER_MPU_ENDADD(7, 10)},
    {BSP_PRV_MASTER_MPU_STADD(7, 11), BSP_PRV_MASTER_MPU_ENDADD(7, 11)},
    {BSP_PRV_MASTER_MPU_STADD(7, 12), BSP_PRV_MASTER_MPU_ENDADD(7, 12)},
    {BSP_PRV_MASTER_MPU_STADD(7, 13), BSP_PRV_MASTER_MPU_ENDADD(7, 13)},
    {BSP_PRV_MASTER_MPU_STADD(7, 14), BSP_PRV_MASTER_MPU_ENDADD(7, 14)},
    {BSP_PRV_MASTER_MPU_STADD(7, 15), BSP_PRV_MASTER_MPU_ENDADD(7, 15)},
 #endif
};
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU8_SUPPORTED)
const uint32_t g_bsp_master_mpu8_cfg[BSP_PRV_MASTER_MPU_REGION_NUM][2] =
{
    {BSP_PRV_MASTER_MPU_STADD(8, 0),  BSP_PRV_MASTER_MPU_ENDADD(8, 0) },
    {BSP_PRV_MASTER_MPU_STADD(8, 1),  BSP_PRV_MASTER_MPU_ENDADD(8, 1) },
    {BSP_PRV_MASTER_MPU_STADD(8, 2),  BSP_PRV_MASTER_MPU_ENDADD(8, 2) },
    {BSP_PRV_MASTER_MPU_STADD(8, 3),  BSP_PRV_MASTER_MPU_ENDADD(8, 3) },
    {BSP_PRV_MASTER_MPU_STADD(8, 4),  BSP_PRV_MASTER_MPU_ENDADD(8, 4) },
    {BSP_PRV_MASTER_MPU_STADD(8, 5),  BSP_PRV_MASTER_MPU_ENDADD(8, 5) },
    {BSP_PRV_MASTER_MPU_STADD(8, 6),  BSP_PRV_MASTER_MPU_ENDADD(8, 6) },
    {BSP_PRV_MASTER_MPU_STADD(8, 7),  BSP_PRV_MASTER_MPU_ENDADD(8, 7) },
 #if (2 == BSP_FEATURE_BSP_MASTER_MPU_REGION_TYPE)
    {BSP_PRV_MASTER_MPU_STADD(8, 8),  BSP_PRV_MASTER_MPU_ENDADD(8, 8) },
    {BSP_PRV_MASTER_MPU_STADD(8, 9),  BSP_PRV_MASTER_MPU_ENDADD(8, 9) },
    {BSP_PRV_MASTER_MPU_STADD(8, 10), BSP_PRV_MASTER_MPU_ENDADD(8, 10)},
    {BSP_PRV_MASTER_MPU_STADD(8, 11), BSP_PRV_MASTER_MPU_ENDADD(8, 11)},
    {BSP_PRV_MASTER_MPU_STADD(8, 12), BSP_PRV_MASTER_MPU_ENDADD(8, 12)},
    {BSP_PRV_MASTER_MPU_STADD(8, 13), BSP_PRV_MASTER_MPU_ENDADD(8, 13)},
    {BSP_PRV_MASTER_MPU_STADD(8, 14), BSP_PRV_MASTER_MPU_ENDADD(8, 14)},
    {BSP_PRV_MASTER_MPU_STADD(8, 15), BSP_PRV_MASTER_MPU_ENDADD(8, 15)},
 #endif
};
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU9_SUPPORTED)
const uint32_t g_bsp_master_mpu9_cfg[BSP_PRV_MASTER_MPU_REGION_NUM][2] =
{
    {BSP_PRV_MASTER_MPU_STADD(9, 0),  BSP_PRV_MASTER_MPU_ENDADD(9, 0) },
    {BSP_PRV_MASTER_MPU_STADD(9, 1),  BSP_PRV_MASTER_MPU_ENDADD(9, 1) },
    {BSP_PRV_MASTER_MPU_STADD(9, 2),  BSP_PRV_MASTER_MPU_ENDADD(9, 2) },
    {BSP_PRV_MASTER_MPU_STADD(9, 3),  BSP_PRV_MASTER_MPU_ENDADD(9, 3) },
    {BSP_PRV_MASTER_MPU_STADD(9, 4),  BSP_PRV_MASTER_MPU_ENDADD(9, 4) },
    {BSP_PRV_MASTER_MPU_STADD(9, 5),  BSP_PRV_MASTER_MPU_ENDADD(9, 5) },
    {BSP_PRV_MASTER_MPU_STADD(9, 6),  BSP_PRV_MASTER_MPU_ENDADD(9, 6) },
    {BSP_PRV_MASTER_MPU_STADD(9, 7),  BSP_PRV_MASTER_MPU_ENDADD(9, 7) },
 #if (2 == BSP_FEATURE_BSP_MASTER_MPU_REGION_TYPE)
    {BSP_PRV_MASTER_MPU_STADD(9, 8),  BSP_PRV_MASTER_MPU_ENDADD(9, 8) },
    {BSP_PRV_MASTER_MPU_STADD(9, 9),  BSP_PRV_MASTER_MPU_ENDADD(9, 9) },
    {BSP_PRV_MASTER_MPU_STADD(9, 10), BSP_PRV_MASTER_MPU_ENDADD(9, 10)},
    {BSP_PRV_MASTER_MPU_STADD(9, 11), BSP_PRV_MASTER_MPU_ENDADD(9, 11)},
    {BSP_PRV_MASTER_MPU_STADD(9, 12), BSP_PRV_MASTER_MPU_ENDADD(9, 12)},
    {BSP_PRV_MASTER_MPU_STADD(9, 13), BSP_PRV_MASTER_MPU_ENDADD(9, 13)},
    {BSP_PRV_MASTER_MPU_STADD(9, 14), BSP_PRV_MASTER_MPU_ENDADD(9, 14)},
    {BSP_PRV_MASTER_MPU_STADD(9, 15), BSP_PRV_MASTER_MPU_ENDADD(9, 15)},
 #endif
};
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU10_SUPPORTED)
const uint32_t g_bsp_master_mpu10_cfg[BSP_PRV_MASTER_MPU_REGION_NUM][2] =
{
    {BSP_PRV_MASTER_MPU_STADD(10, 0),  BSP_PRV_MASTER_MPU_ENDADD(10, 0) },
    {BSP_PRV_MASTER_MPU_STADD(10, 1),  BSP_PRV_MASTER_MPU_ENDADD(10, 1) },
    {BSP_PRV_MASTER_MPU_STADD(10, 2),  BSP_PRV_MASTER_MPU_ENDADD(10, 2) },
    {BSP_PRV_MASTER_MPU_STADD(10, 3),  BSP_PRV_MASTER_MPU_ENDADD(10, 3) },
    {BSP_PRV_MASTER_MPU_STADD(10, 4),  BSP_PRV_MASTER_MPU_ENDADD(10, 4) },
    {BSP_PRV_MASTER_MPU_STADD(10, 5),  BSP_PRV_MASTER_MPU_ENDADD(10, 5) },
    {BSP_PRV_MASTER_MPU_STADD(10, 6),  BSP_PRV_MASTER_MPU_ENDADD(10, 6) },
    {BSP_PRV_MASTER_MPU_STADD(10, 7),  BSP_PRV_MASTER_MPU_ENDADD(10, 7) },
 #if (2 == BSP_FEATURE_BSP_MASTER_MPU_REGION_TYPE)
    {BSP_PRV_MASTER_MPU_STADD(10, 8),  BSP_PRV_MASTER_MPU_ENDADD(10, 8) },
    {BSP_PRV_MASTER_MPU_STADD(10, 9),  BSP_PRV_MASTER_MPU_ENDADD(10, 9) },
    {BSP_PRV_MASTER_MPU_STADD(10, 10), BSP_PRV_MASTER_MPU_ENDADD(10, 10)},
    {BSP_PRV_MASTER_MPU_STADD(10, 11), BSP_PRV_MASTER_MPU_ENDADD(10, 11)},
    {BSP_PRV_MASTER_MPU_STADD(10, 12), BSP_PRV_MASTER_MPU_ENDADD(10, 12)},
    {BSP_PRV_MASTER_MPU_STADD(10, 13), BSP_PRV_MASTER_MPU_ENDADD(10, 13)},
    {BSP_PRV_MASTER_MPU_STADD(10, 14), BSP_PRV_MASTER_MPU_ENDADD(10, 14)},
    {BSP_PRV_MASTER_MPU_STADD(10, 15), BSP_PRV_MASTER_MPU_ENDADD(10, 15)},
 #endif
};
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU11_SUPPORTED)
const uint32_t g_bsp_master_mpu11_cfg[BSP_PRV_MASTER_MPU_REGION_NUM][2] =
{
    {BSP_PRV_MASTER_MPU_STADD(11, 0),  BSP_PRV_MASTER_MPU_ENDADD(11, 0) },
    {BSP_PRV_MASTER_MPU_STADD(11, 1),  BSP_PRV_MASTER_MPU_ENDADD(11, 1) },
    {BSP_PRV_MASTER_MPU_STADD(11, 2),  BSP_PRV_MASTER_MPU_ENDADD(11, 2) },
    {BSP_PRV_MASTER_MPU_STADD(11, 3),  BSP_PRV_MASTER_MPU_ENDADD(11, 3) },
    {BSP_PRV_MASTER_MPU_STADD(11, 4),  BSP_PRV_MASTER_MPU_ENDADD(11, 4) },
    {BSP_PRV_MASTER_MPU_STADD(11, 5),  BSP_PRV_MASTER_MPU_ENDADD(11, 5) },
    {BSP_PRV_MASTER_MPU_STADD(11, 6),  BSP_PRV_MASTER_MPU_ENDADD(11, 6) },
    {BSP_PRV_MASTER_MPU_STADD(11, 7),  BSP_PRV_MASTER_MPU_ENDADD(11, 7) },
 #if (2 == BSP_FEATURE_BSP_MASTER_MPU_REGION_TYPE)
    {BSP_PRV_MASTER_MPU_STADD(11, 8),  BSP_PRV_MASTER_MPU_ENDADD(11, 8) },
    {BSP_PRV_MASTER_MPU_STADD(11, 9),  BSP_PRV_MASTER_MPU_ENDADD(11, 9) },
    {BSP_PRV_MASTER_MPU_STADD(11, 10), BSP_PRV_MASTER_MPU_ENDADD(11, 10)},
    {BSP_PRV_MASTER_MPU_STADD(11, 11), BSP_PRV_MASTER_MPU_ENDADD(11, 11)},
    {BSP_PRV_MASTER_MPU_STADD(11, 12), BSP_PRV_MASTER_MPU_ENDADD(11, 12)},
    {BSP_PRV_MASTER_MPU_STADD(11, 13), BSP_PRV_MASTER_MPU_ENDADD(11, 13)},
    {BSP_PRV_MASTER_MPU_STADD(11, 14), BSP_PRV_MASTER_MPU_ENDADD(11, 14)},
    {BSP_PRV_MASTER_MPU_STADD(11, 15), BSP_PRV_MASTER_MPU_ENDADD(11, 15)},
 #endif
};
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU12_SUPPORTED)
const uint32_t g_bsp_master_mpu12_cfg[BSP_PRV_MASTER_MPU_REGION_NUM][2] =
{
    {BSP_PRV_MASTER_MPU_STADD(12, 0),  BSP_PRV_MASTER_MPU_ENDADD(12, 0) },
    {BSP_PRV_MASTER_MPU_STADD(12, 1),  BSP_PRV_MASTER_MPU_ENDADD(12, 1) },
    {BSP_PRV_MASTER_MPU_STADD(12, 2),  BSP_PRV_MASTER_MPU_ENDADD(12, 2) },
    {BSP_PRV_MASTER_MPU_STADD(12, 3),  BSP_PRV_MASTER_MPU_ENDADD(12, 3) },
    {BSP_PRV_MASTER_MPU_STADD(12, 4),  BSP_PRV_MASTER_MPU_ENDADD(12, 4) },
    {BSP_PRV_MASTER_MPU_STADD(12, 5),  BSP_PRV_MASTER_MPU_ENDADD(12, 5) },
    {BSP_PRV_MASTER_MPU_STADD(12, 6),  BSP_PRV_MASTER_MPU_ENDADD(12, 6) },
    {BSP_PRV_MASTER_MPU_STADD(12, 7),  BSP_PRV_MASTER_MPU_ENDADD(12, 7) },
 #if (2 == BSP_FEATURE_BSP_MASTER_MPU_REGION_TYPE)
    {BSP_PRV_MASTER_MPU_STADD(12, 8),  BSP_PRV_MASTER_MPU_ENDADD(12, 8) },
    {BSP_PRV_MASTER_MPU_STADD(12, 9),  BSP_PRV_MASTER_MPU_ENDADD(12, 9) },
    {BSP_PRV_MASTER_MPU_STADD(12, 10), BSP_PRV_MASTER_MPU_ENDADD(12, 10)},
    {BSP_PRV_MASTER_MPU_STADD(12, 11), BSP_PRV_MASTER_MPU_ENDADD(12, 11)},
    {BSP_PRV_MASTER_MPU_STADD(12, 12), BSP_PRV_MASTER_MPU_ENDADD(12, 12)},
    {BSP_PRV_MASTER_MPU_STADD(12, 13), BSP_PRV_MASTER_MPU_ENDADD(12, 13)},
    {BSP_PRV_MASTER_MPU_STADD(12, 14), BSP_PRV_MASTER_MPU_ENDADD(12, 14)},
    {BSP_PRV_MASTER_MPU_STADD(12, 15), BSP_PRV_MASTER_MPU_ENDADD(12, 15)},
 #endif
};
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU13_SUPPORTED)
const uint32_t g_bsp_master_mpu13_cfg[BSP_PRV_MASTER_MPU_REGION_NUM][2] =
{
    {BSP_PRV_MASTER_MPU_STADD(13, 0),  BSP_PRV_MASTER_MPU_ENDADD(13, 0) },
    {BSP_PRV_MASTER_MPU_STADD(13, 1),  BSP_PRV_MASTER_MPU_ENDADD(13, 1) },
    {BSP_PRV_MASTER_MPU_STADD(13, 2),  BSP_PRV_MASTER_MPU_ENDADD(13, 2) },
    {BSP_PRV_MASTER_MPU_STADD(13, 3),  BSP_PRV_MASTER_MPU_ENDADD(13, 3) },
    {BSP_PRV_MASTER_MPU_STADD(13, 4),  BSP_PRV_MASTER_MPU_ENDADD(13, 4) },
    {BSP_PRV_MASTER_MPU_STADD(13, 5),  BSP_PRV_MASTER_MPU_ENDADD(13, 5) },
    {BSP_PRV_MASTER_MPU_STADD(13, 6),  BSP_PRV_MASTER_MPU_ENDADD(13, 6) },
    {BSP_PRV_MASTER_MPU_STADD(13, 7),  BSP_PRV_MASTER_MPU_ENDADD(13, 7) },
 #if (2 == BSP_FEATURE_BSP_MASTER_MPU_REGION_TYPE)
    {BSP_PRV_MASTER_MPU_STADD(13, 8),  BSP_PRV_MASTER_MPU_ENDADD(13, 8) },
    {BSP_PRV_MASTER_MPU_STADD(13, 9),  BSP_PRV_MASTER_MPU_ENDADD(13, 9) },
    {BSP_PRV_MASTER_MPU_STADD(13, 10), BSP_PRV_MASTER_MPU_ENDADD(13, 10)},
    {BSP_PRV_MASTER_MPU_STADD(13, 11), BSP_PRV_MASTER_MPU_ENDADD(13, 11)},
    {BSP_PRV_MASTER_MPU_STADD(13, 12), BSP_PRV_MASTER_MPU_ENDADD(13, 12)},
    {BSP_PRV_MASTER_MPU_STADD(13, 13), BSP_PRV_MASTER_MPU_ENDADD(13, 13)},
    {BSP_PRV_MASTER_MPU_STADD(13, 14), BSP_PRV_MASTER_MPU_ENDADD(13, 14)},
    {BSP_PRV_MASTER_MPU_STADD(13, 15), BSP_PRV_MASTER_MPU_ENDADD(13, 15)},
 #endif
};
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU14_SUPPORTED)
const uint32_t g_bsp_master_mpu14_cfg[BSP_PRV_MASTER_MPU_REGION_NUM][2] =
{
    {BSP_PRV_MASTER_MPU_STADD(14, 0),  BSP_PRV_MASTER_MPU_ENDADD(14, 0) },
    {BSP_PRV_MASTER_MPU_STADD(14, 1),  BSP_PRV_MASTER_MPU_ENDADD(14, 1) },
    {BSP_PRV_MASTER_MPU_STADD(14, 2),  BSP_PRV_MASTER_MPU_ENDADD(14, 2) },
    {BSP_PRV_MASTER_MPU_STADD(14, 3),  BSP_PRV_MASTER_MPU_ENDADD(14, 3) },
    {BSP_PRV_MASTER_MPU_STADD(14, 4),  BSP_PRV_MASTER_MPU_ENDADD(14, 4) },
    {BSP_PRV_MASTER_MPU_STADD(14, 5),  BSP_PRV_MASTER_MPU_ENDADD(14, 5) },
    {BSP_PRV_MASTER_MPU_STADD(14, 6),  BSP_PRV_MASTER_MPU_ENDADD(14, 6) },
    {BSP_PRV_MASTER_MPU_STADD(14, 7),  BSP_PRV_MASTER_MPU_ENDADD(14, 7) },
 #if (2 == BSP_FEATURE_BSP_MASTER_MPU_REGION_TYPE)
    {BSP_PRV_MASTER_MPU_STADD(14, 8),  BSP_PRV_MASTER_MPU_ENDADD(14, 8) },
    {BSP_PRV_MASTER_MPU_STADD(14, 9),  BSP_PRV_MASTER_MPU_ENDADD(14, 9) },
    {BSP_PRV_MASTER_MPU_STADD(14, 10), BSP_PRV_MASTER_MPU_ENDADD(14, 10)},
    {BSP_PRV_MASTER_MPU_STADD(14, 11), BSP_PRV_MASTER_MPU_ENDADD(14, 11)},
    {BSP_PRV_MASTER_MPU_STADD(14, 12), BSP_PRV_MASTER_MPU_ENDADD(14, 12)},
    {BSP_PRV_MASTER_MPU_STADD(14, 13), BSP_PRV_MASTER_MPU_ENDADD(14, 13)},
    {BSP_PRV_MASTER_MPU_STADD(14, 14), BSP_PRV_MASTER_MPU_ENDADD(14, 14)},
    {BSP_PRV_MASTER_MPU_STADD(14, 15), BSP_PRV_MASTER_MPU_ENDADD(14, 15)},
 #endif
};
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU15_SUPPORTED)
const uint32_t g_bsp_master_mpu15_cfg[BSP_PRV_MASTER_MPU_REGION_NUM][2] =
{
    {BSP_PRV_MASTER_MPU_STADD(15, 0),  BSP_PRV_MASTER_MPU_ENDADD(15, 0) },
    {BSP_PRV_MASTER_MPU_STADD(15, 1),  BSP_PRV_MASTER_MPU_ENDADD(15, 1) },
    {BSP_PRV_MASTER_MPU_STADD(15, 2),  BSP_PRV_MASTER_MPU_ENDADD(15, 2) },
    {BSP_PRV_MASTER_MPU_STADD(15, 3),  BSP_PRV_MASTER_MPU_ENDADD(15, 3) },
    {BSP_PRV_MASTER_MPU_STADD(15, 4),  BSP_PRV_MASTER_MPU_ENDADD(15, 4) },
    {BSP_PRV_MASTER_MPU_STADD(15, 5),  BSP_PRV_MASTER_MPU_ENDADD(15, 5) },
    {BSP_PRV_MASTER_MPU_STADD(15, 6),  BSP_PRV_MASTER_MPU_ENDADD(15, 6) },
    {BSP_PRV_MASTER_MPU_STADD(15, 7),  BSP_PRV_MASTER_MPU_ENDADD(15, 7) },
 #if (2 == BSP_FEATURE_BSP_MASTER_MPU_REGION_TYPE)
    {BSP_PRV_MASTER_MPU_STADD(15, 8),  BSP_PRV_MASTER_MPU_ENDADD(15, 8) },
    {BSP_PRV_MASTER_MPU_STADD(15, 9),  BSP_PRV_MASTER_MPU_ENDADD(15, 9) },
    {BSP_PRV_MASTER_MPU_STADD(15, 10), BSP_PRV_MASTER_MPU_ENDADD(15, 10)},
    {BSP_PRV_MASTER_MPU_STADD(15, 11), BSP_PRV_MASTER_MPU_ENDADD(15, 11)},
    {BSP_PRV_MASTER_MPU_STADD(15, 12), BSP_PRV_MASTER_MPU_ENDADD(15, 12)},
    {BSP_PRV_MASTER_MPU_STADD(15, 13), BSP_PRV_MASTER_MPU_ENDADD(15, 13)},
    {BSP_PRV_MASTER_MPU_STADD(15, 14), BSP_PRV_MASTER_MPU_ENDADD(15, 14)},
    {BSP_PRV_MASTER_MPU_STADD(15, 15), BSP_PRV_MASTER_MPU_ENDADD(15, 15)},
 #endif
};
#endif

#if defined(__ICCARM__)

void R_BSP_WarmStart(bsp_warm_start_event_t event);

 #pragma weak R_BSP_WarmStart

#elif defined(__GNUC__) || defined(__ARMCC_VERSION)

void R_BSP_WarmStart(bsp_warm_start_event_t event) __attribute__((weak));

#endif

#if BSP_CFG_C_RUNTIME_INIT
void bsp_loader_data_init(void);
void bsp_loader_bss_init(void);
void bsp_static_constructor_init(void);

#endif

void bsp_copy_multibyte(uintptr_t * src, uintptr_t * dst, uintptr_t bytesize);
void bsp_bss_init_multibyte(uintptr_t * src, uintptr_t bytesize);

#if !(BSP_CFG_RAM_EXECUTION)
void bsp_copy_to_ram(void);
void bsp_application_bss_init(void);

#endif

void bsp_master_mpu_init(void);
void bsp_global_system_counter_init(void);

#if BSP_FEATURE_TFU_SUPPORTED
void bsp_tfu_init(void);

 #if (1 == BSP_LP64_SUPPORT)
float g_bsp_tfu_convert_result;

 #endif

#endif

#if !BSP_CFG_PORT_PROTECT
void bsp_release_port_protect(void);

#endif

/*******************************************************************************************************************//**
 * Initialize the Master-MPU settings.
 **********************************************************************************************************************/
void bsp_master_mpu_init (void)
{
    /* Disable register protection for Master-MPU related registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_SYSTEM);

    for (uint8_t region_num = 0; region_num < BSP_PRV_MASTER_MPU_REGION_NUM; region_num++)
    {
#if (1 == BSP_FEATURE_BSP_MASTER_MPU0_SUPPORTED)
        R_MPU0->RGN[region_num].STADD  = g_bsp_master_mpu0_cfg[region_num][0];
        R_MPU0->RGN[region_num].ENDADD = g_bsp_master_mpu0_cfg[region_num][1];
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU1_SUPPORTED)
        R_MPU1->RGN[region_num].STADD  = g_bsp_master_mpu1_cfg[region_num][0];
        R_MPU1->RGN[region_num].ENDADD = g_bsp_master_mpu1_cfg[region_num][1];
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU2_SUPPORTED)
        R_MPU2->RGN[region_num].STADD  = g_bsp_master_mpu2_cfg[region_num][0];
        R_MPU2->RGN[region_num].ENDADD = g_bsp_master_mpu2_cfg[region_num][1];
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU3_SUPPORTED)
        R_MPU3->RGN[region_num].STADD  = g_bsp_master_mpu3_cfg[region_num][0];
        R_MPU3->RGN[region_num].ENDADD = g_bsp_master_mpu3_cfg[region_num][1];
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU4_SUPPORTED)
        R_MPU4->RGN[region_num].STADD  = g_bsp_master_mpu4_cfg[region_num][0];
        R_MPU4->RGN[region_num].ENDADD = g_bsp_master_mpu4_cfg[region_num][1];
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU5_SUPPORTED)
        R_MPU5->RGN[region_num].STADD  = g_bsp_master_mpu5_cfg[region_num][0];
        R_MPU5->RGN[region_num].ENDADD = g_bsp_master_mpu5_cfg[region_num][1];
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU6_SUPPORTED)
        R_MPU6->RGN[region_num].STADD  = g_bsp_master_mpu6_cfg[region_num][0];
        R_MPU6->RGN[region_num].ENDADD = g_bsp_master_mpu6_cfg[region_num][1];
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU7_SUPPORTED)
        R_MPU7->RGN[region_num].STADD  = g_bsp_master_mpu7_cfg[region_num][0];
        R_MPU7->RGN[region_num].ENDADD = g_bsp_master_mpu7_cfg[region_num][1];
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU8_SUPPORTED)
        R_MPU8->RGN[region_num].STADD  = g_bsp_master_mpu8_cfg[region_num][0];
        R_MPU8->RGN[region_num].ENDADD = g_bsp_master_mpu8_cfg[region_num][1];
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU9_SUPPORTED)
        R_MPU9->RGN[region_num].STADD  = g_bsp_master_mpu9_cfg[region_num][0];
        R_MPU9->RGN[region_num].ENDADD = g_bsp_master_mpu9_cfg[region_num][1];
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU10_SUPPORTED)
        R_MPU10->RGN[region_num].STADD  = g_bsp_master_mpu10_cfg[region_num][0];
        R_MPU10->RGN[region_num].ENDADD = g_bsp_master_mpu10_cfg[region_num][1];
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU11_SUPPORTED)
        R_MPU11->RGN[region_num].STADD  = g_bsp_master_mpu11_cfg[region_num][0];
        R_MPU11->RGN[region_num].ENDADD = g_bsp_master_mpu11_cfg[region_num][1];
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU12_SUPPORTED)
        R_MPU12->RGN[region_num].STADD  = g_bsp_master_mpu12_cfg[region_num][0];
        R_MPU12->RGN[region_num].ENDADD = g_bsp_master_mpu12_cfg[region_num][1];
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU13_SUPPORTED)
        R_MPU13->RGN[region_num].STADD  = g_bsp_master_mpu13_cfg[region_num][0];
        R_MPU13->RGN[region_num].ENDADD = g_bsp_master_mpu13_cfg[region_num][1];
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU14_SUPPORTED)
        R_MPU14->RGN[region_num].STADD  = g_bsp_master_mpu14_cfg[region_num][0];
        R_MPU14->RGN[region_num].ENDADD = g_bsp_master_mpu14_cfg[region_num][1];
#endif
#if (1 == BSP_FEATURE_BSP_MASTER_MPU15_SUPPORTED)
        R_MPU15->RGN[region_num].STADD  = g_bsp_master_mpu15_cfg[region_num][0];
        R_MPU15->RGN[region_num].ENDADD = g_bsp_master_mpu15_cfg[region_num][1];
#endif
    }

    /* Enable register protection for Master-MPU related registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_SYSTEM);
}

/*******************************************************************************************************************//**
 * Initialize global system counter. The counter is enabled and is incrementing.
 **********************************************************************************************************************/
void bsp_global_system_counter_init (void)
{
    /* Initialize registers related the global system counter. */
    R_GSC->CNTCR  &= (uint32_t) (~R_GSC_CNTCR_EN_Msk);
    R_GSC->CNTFID0 = BSP_GLOBAL_SYSTEM_COUNTER_CLOCK_HZ;
    R_GSC->CNTCVL  = 0;
    R_GSC->CNTCVU  = 0;
    R_GSC->CNTCR  |= R_GSC_CNTCR_EN_Msk;
}

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.
 * This function is declared as a weak symbol higher up in this file because it is meant to be overridden by a user
 * implemented version. One of the main uses for this function is to call functional safety code during the startup
 * process. To use this function just copy this function into your own code and modify it to meet your needs.
 *
 * @param[in]  event    Where the code currently is in the start up process
 *
 * @note All programs that are executed when a BSP_WARM_START_RESET, BSP_WARM_START_POST_CLOCK, or
 *       BSP_WARM_START_POST_LOADER_INIT event occurs must be placed in
 *       the LOADER section(BTCM for CR52 core, SystemRAM for CA55 core).
 *       These events occur before the copy of the application program in the startup code is executed,
 *       so the application program is on ROM and cannot be executed at that time.
 *       The FSP linker script specifies that the .warm_start section be placed in the LOADER section.
 *       Adding a section specification to the definition of a function or
 *       variable makes it easier to place it in the LOADER section.
 **********************************************************************************************************************/
void R_BSP_WarmStart (bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
        /* C runtime environment has not been setup so you cannot use globals. System clocks are not setup. */
    }

    if (BSP_WARM_START_POST_CLOCK == event)
    {
        /* C runtime environment has not been setup so you cannot use globals. Clocks have been initialized. */
    }
    else if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment, system clocks, and pins are all setup. */
    }
    else if (BSP_WARM_START_POST_LOADER_INIT == event)
    {
        /* The variables used in the loader program have been initialized. */
    }
    else
    {
        /* Do nothing */
    }
}

/** @} (end addtogroup BSP_MCU) */

#if BSP_CFG_C_RUNTIME_INIT

/*******************************************************************************************************************//**
 * Copy the loader data block from external Flash to internal RAM.
 **********************************************************************************************************************/
void bsp_loader_data_init (void)
{
 #if (!defined(__GNUC__) || !(BSP_CFG_RAM_EXECUTION))

    /* Define destination/source address pointer and block size */
    uintptr_t * src;
    uintptr_t * dst;
    uintptr_t   size;

    /* Copy loader data block */
    src  = (uintptr_t *) BSP_PRV_SECTION_LDR_DATA_ROM_ADDRESS;
    dst  = (uintptr_t *) BSP_PRV_SECTION_LDR_DATA_RAM_START;
    size = (uintptr_t) BSP_PRV_SECTION_LDR_DATA_RAM_END - (uintptr_t) BSP_PRV_SECTION_LDR_DATA_RAM_START;
    bsp_copy_multibyte(src, dst, size);
 #endif
}

/*******************************************************************************************************************//**
 * Clear the loader bss block in internal RAM.
 **********************************************************************************************************************/
void bsp_loader_bss_init (void)
{
    /* Define source address pointer and block size */
    uintptr_t * src;
    uintptr_t   size;

    /* Clear loader bss block. */
    src  = (uintptr_t *) BSP_PRV_SECTION_LDR_DATA_BSS_START;
    size = (uintptr_t) BSP_PRV_SECTION_LDR_DATA_BSS_END - (uintptr_t) BSP_PRV_SECTION_LDR_DATA_BSS_START;
    bsp_bss_init_multibyte(src, size);

 #if BSP_CFG_RAM_EXECUTION
  #if defined(__ICCARM__)

    /* Initialize the application data and clear the application bss.
     * This code is for RAM Execution. If you want to boot with ROM,
     * enable app_copy and app_bss_init, and disable this code.
     * Also need to change icf file. */
    __iar_data_init3();
  #elif defined(__GNUC__)

    /* Clear application bss block. */
    src  = (uintptr_t *) BSP_PRV_SECTION_USER_DATA_BSS_START;
    size = (uintptr_t) BSP_PRV_SECTION_USER_DATA_BSS_END - (uintptr_t) BSP_PRV_SECTION_USER_DATA_BSS_START;
    bsp_bss_init_multibyte(src, size);
  #endif
 #endif
}

#endif

/*******************************************************************************************************************//**
 * Copy the memory block from Source address to Destination address by the multi byte unit.
 **********************************************************************************************************************/
void bsp_copy_multibyte (uintptr_t * src, uintptr_t * dst, uintptr_t bytesize)
{
    uintptr_t i;
    uintptr_t cnt;

    uintptr_t bytesize_mod;
    uint8_t * src_single_byte;
    uint8_t * dst_single_byte;

    if (0 != bytesize)
    {
        /* Copy Count in multi byte unit */
        cnt = bytesize / sizeof(uintptr_t);

        for (i = 0; i < cnt; i++)
        {
            *dst++ = *src++;
        }

        /* Copy Count in single byte unit */
        bytesize_mod = bytesize % sizeof(uintptr_t);

        if (0 != bytesize_mod)
        {
            src_single_byte = (uint8_t *) src;
            dst_single_byte = (uint8_t *) dst;

            for (i = 0; i < bytesize_mod; i++)
            {
                *dst_single_byte++ = *src_single_byte++;
            }
        }
        else
        {
            /* Do nothing */
        }

        /* Ensuring data-changing */
        __asm volatile ("DSB SY");
    }
    else
    {
        /* Do nothing */
    }
}

/*******************************************************************************************************************//**
 * Clear the bss block by the multi byte unit.
 **********************************************************************************************************************/
void bsp_bss_init_multibyte (uintptr_t * src, uintptr_t bytesize)
{
    uintptr_t i;
    uintptr_t cnt;
    uintptr_t zero = 0;

    uintptr_t bytesize_mod;
    uint8_t * src_single_byte;
    uint8_t   zero_single_byte = 0;

    if (0 != bytesize)
    {
        /* Clear Count in multi byte unit */
        cnt = bytesize / sizeof(uintptr_t);

        for (i = 0; i < cnt; i++)
        {
            *src++ = zero;
        }

        /* Clear Count in single byte unit */
        bytesize_mod = bytesize % sizeof(uintptr_t);

        if (0 != bytesize_mod)
        {
            src_single_byte = (uint8_t *) src;

            for (i = 0; i < bytesize_mod; i++)
            {
                *src_single_byte++ = zero_single_byte;
            }
        }
        else
        {
            /* Do nothing */
        }

        /* Ensuring data-changing */
        __asm volatile ("DSB SY");
    }
    else
    {
        /* Do nothing */
    }
}

#if !(BSP_CFG_RAM_EXECUTION)

/*******************************************************************************************************************//**
 * Copy the application program block from external Flash to internal RAM.
 **********************************************************************************************************************/
void bsp_copy_to_ram (void)
{
    /* Define destination/source address pointer and block size */
    uintptr_t * src;
    uintptr_t * dst;
    uintptr_t   size;

    /* Copy exception vector block */
    src  = (uintptr_t *) BSP_PRV_SECTION_VECTOR_ROM_ADDRESS;
    dst  = (uintptr_t *) BSP_PRV_SECTION_VECTOR_RAM_START;
    size = (uintptr_t) BSP_PRV_SECTION_VECTOR_RAM_END - (uintptr_t) BSP_PRV_SECTION_VECTOR_RAM_START;
    bsp_copy_multibyte(src, dst, size);

    /* Copy user program block */
    src  = (uintptr_t *) BSP_PRV_SECTION_USER_PRG_ROM_ADDRESS;
    dst  = (uintptr_t *) BSP_PRV_SECTION_USER_PRG_RAM_START;
    size = (uintptr_t) BSP_PRV_SECTION_USER_PRG_RAM_END - (uintptr_t) BSP_PRV_SECTION_USER_PRG_RAM_START;
    bsp_copy_multibyte(src, dst, size);

    /* Copy user data block */
    src  = (uintptr_t *) BSP_PRV_SECTION_USER_DATA_ROM_ADDRESS;
    dst  = (uintptr_t *) BSP_PRV_SECTION_USER_DATA_RAM_START;
    size = (uintptr_t) BSP_PRV_SECTION_USER_DATA_RAM_END - (uintptr_t) BSP_PRV_SECTION_USER_DATA_RAM_START;
    bsp_copy_multibyte(src, dst, size);

    /* Copy user data_noncache block */
    src  = (uintptr_t *) BSP_PRV_SECTION_USER_DATA_NONCACHE_ROM_ADDRESS;
    dst  = (uintptr_t *) BSP_PRV_SECTION_USER_DATA_NONCACHE_RAM_START;
    size = (uintptr_t) BSP_PRV_SECTION_USER_DATA_NONCACHE_RAM_END -
           (uintptr_t) BSP_PRV_SECTION_USER_DATA_NONCACHE_RAM_START;
    bsp_copy_multibyte(src, dst, size);

    /* Copy DMAC link mode data block. */
    src  = (uintptr_t *) BSP_PRV_SECTION_DMAC_LINK_MODE_ROM_ADDRESS;
    dst  = (uintptr_t *) BSP_PRV_SECTION_DMAC_LINK_MODE_RAM_START;
    size = (uintptr_t) BSP_PRV_SECTION_DMAC_LINK_MODE_RAM_END -
           (uintptr_t) BSP_PRV_SECTION_DMAC_LINK_MODE_RAM_START;
    bsp_copy_multibyte(src, dst, size);

    /* Copy shared non-cache buffer block. */
    src  = (uintptr_t *) BSP_PRV_SECTION_SHARED_NONCACHE_BUFFER_ROM_ADDRESS;
    dst  = (uintptr_t *) BSP_PRV_SECTION_SHARED_NONCACHE_BUFFER_RAM_START;
    size = (uintptr_t) BSP_PRV_SECTION_SHARED_NONCACHE_BUFFER_RAM_END -
           (uintptr_t) BSP_PRV_SECTION_SHARED_NONCACHE_BUFFER_RAM_START;
    bsp_copy_multibyte(src, dst, size);

    /* Copy non-cache buffer block. */
    src  = (uintptr_t *) BSP_PRV_SECTION_NONCACHE_BUFFER_ROM_ADDRESS;
    dst  = (uintptr_t *) BSP_PRV_SECTION_NONCACHE_BUFFER_RAM_START;
    size = (uintptr_t) BSP_PRV_SECTION_NONCACHE_BUFFER_RAM_END -
           (uintptr_t) BSP_PRV_SECTION_NONCACHE_BUFFER_RAM_START;
    bsp_copy_multibyte(src, dst, size);
}

/*******************************************************************************************************************//**
 * Clear the application bss block in internal RAM.
 **********************************************************************************************************************/
void bsp_application_bss_init (void)
{
    /* Define source address pointer and block size */
    uintptr_t * src;
    uintptr_t   size;

    /* Clear application bss block. */
    src  = (uintptr_t *) BSP_PRV_SECTION_USER_DATA_BSS_START;
    size = (uintptr_t) BSP_PRV_SECTION_USER_DATA_BSS_END - (uintptr_t) BSP_PRV_SECTION_USER_DATA_BSS_START;
    bsp_bss_init_multibyte(src, size);
}

#endif

#if BSP_FEATURE_TFU_SUPPORTED
void bsp_tfu_init (void)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
    R_BSP_MODULE_START(FSP_IP_TFU, 0U);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);
}

#endif

#if !BSP_CFG_PORT_PROTECT
void bsp_release_port_protect (void)
{
    /** When writing to the PRCR register the upper 8-bits must be the correct key. Set lower bits to 0 to
     * disable writes. */
    R_RWP_NS->PRCRN = ((R_RWP_NS->PRCRN | BSP_IO_PRV_PRCR_KEY) | BSP_IO_REG_PROTECT_GPIO);
    R_RWP_S->PRCRS  = ((R_RWP_S->PRCRS | BSP_IO_PRV_PRCR_KEY) | BSP_IO_REG_PROTECT_GPIO);
}

#endif

/*******************************************************************************************************************//**
 * Initialize static constructors.
 **********************************************************************************************************************/
#if BSP_CFG_C_RUNTIME_INIT
void bsp_static_constructor_init (void)
{
 #if defined(__ICCARM__)
  #if !(BSP_CFG_RAM_EXECUTION)

    /* In the case of ROM boot, initialization of static constructors is performed by __iar_data_init3(). */
    __iar_data_init3();
  #endif
 #elif defined(__GNUC__)
    intptr_t count;
    intptr_t i;

    count = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < count; i++)
    {
        __preinit_array_start[i]();
    }

    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++)
    {
        __init_array_start[i]();
    }
 #endif
}

#endif

/* This vector table is for SGI and PPI interrupts. */
BSP_DONT_REMOVE fsp_vector_t g_sgi_ppi_vector_table[BSP_CORTEX_VECTOR_TABLE_ENTRIES] =
{
    NULL,                              /* INTID0    : SOFTWARE_GENERATE_INT0                   */
    NULL,                              /* INTID1    : SOFTWARE_GENERATE_INT1                   */
    NULL,                              /* INTID2    : SOFTWARE_GENERATE_INT2                   */
    NULL,                              /* INTID3    : SOFTWARE_GENERATE_INT3                   */
    NULL,                              /* INTID4    : SOFTWARE_GENERATE_INT4                   */
    NULL,                              /* INTID5    : SOFTWARE_GENERATE_INT5                   */
    NULL,                              /* INTID6    : SOFTWARE_GENERATE_INT6                   */
    NULL,                              /* INTID7    : SOFTWARE_GENERATE_INT7                   */
    NULL,                              /* INTID8    : SOFTWARE_GENERATE_INT8                   */
    NULL,                              /* INTID9    : SOFTWARE_GENERATE_INT9                   */
    NULL,                              /* INTID10   : SOFTWARE_GENERATE_INT10                  */
    NULL,                              /* INTID11   : SOFTWARE_GENERATE_INT11                  */
    NULL,                              /* INTID12   : SOFTWARE_GENERATE_INT12                  */
    NULL,                              /* INTID13   : SOFTWARE_GENERATE_INT13                  */
    NULL,                              /* INTID14   : SOFTWARE_GENERATE_INT14                  */
    NULL,                              /* INTID15   : SOFTWARE_GENERATE_INT15                  */
    NULL,                              /* INTID16   : RESERVED                                 */
    NULL,                              /* INTID17   : RESERVED                                 */
    NULL,                              /* INTID18   : RESERVED                                 */
    NULL,                              /* INTID19   : RESERVED                                 */
    NULL,                              /* INTID20   : RESERVED                                 */
    NULL,                              /* INTID21   : RESERVED                                 */
    NULL,                              /* INTID22   : DEBUG_COMMUNICATIONS_CHANNEL_INT         */
    NULL,                              /* INTID23   : PERFORMANCE_MONITOR_COUNTER_OVERFLOW_INT */
    NULL,                              /* INTID24   : CROSS_TRIGGER_INTERFACE_INT              */
    NULL,                              /* INTID25   : VIRTUAL_CPU_INTERFACE_MAINTENANCE_INT    */
    NULL,                              /* INTID26   : HYPERVISOR_TIMER_INT                     */
    NULL,                              /* INTID27   : VIRTUAL_TIMER_INT                        */
    NULL,                              /* INTID28   : RESERVED                                 */
    NULL,                              /* INTID29   : RESERVED                                 */
    NULL,                              /* INTID30   : NON-SECURE_PHYSICAL_TIMER_INT            */
    NULL,                              /* INTID31   : RESERVED                                 */
};
