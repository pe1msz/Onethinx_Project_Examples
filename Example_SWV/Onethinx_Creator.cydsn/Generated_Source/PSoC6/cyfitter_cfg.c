
/*******************************************************************************
* File Name: cyfitter_cfg.c
* 
* PSoC Creator  4.4
*
* Description:
* This file contains device initialization code.
* Except for the user defined sections in CyClockStartupError(), this file should not be modified.
* This file was automatically generated by PSoC Creator and is ported for universal use by a Onethinx Tool (c).
*
********************************************************************************
* Copyright (c) 2007-2020 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include <string.h>
#include "cydevice_trm.h"
#include "cyfitter.h"
#include "gpio/cy_gpio.h"
#include "syslib/cy_syslib.h"
#include "cyfitter_cfg.h"
#include "cyapicallbacks.h"
#include "sysclk/cy_sysclk.h"
#include "systick/cy_systick.h"
#include "ble/cy_ble_clk.h"

#define CY_NEED_CYCLOCKSTARTUPERROR 1
#include "syspm/cy_syspm.h"


#if defined(__GNUC__) || defined(__ARMCC_VERSION)
    #define CYPACKED 
    #define CYPACKED_ATTR __attribute__ ((packed))
    #define CYALIGNED __attribute__ ((aligned))
    #define CY_CFG_UNUSED __attribute__ ((unused))
    #ifndef CY_CFG_SECTION
        #define CY_CFG_SECTION __attribute__ ((section(".psocinit")))
    #endif
    
    #if defined(__ARMCC_VERSION)
        #define CY_CFG_MEMORY_BARRIER() __memory_changed()
    #else
        #define CY_CFG_MEMORY_BARRIER() __sync_synchronize()
    #endif
    
#elif defined(__ICCARM__)
    #include <intrinsics.h>

    #define CYPACKED __packed
    #define CYPACKED_ATTR 
    #define CYALIGNED _Pragma("data_alignment=4")
    #define CY_CFG_UNUSED _Pragma("diag_suppress=Pe177")
    #define CY_CFG_SECTION _Pragma("location=\".psocinit\"")
    
    #define CY_CFG_MEMORY_BARRIER() __DMB()
    
#else
    #error Unsupported toolchain
#endif

#ifndef CYCODE
    #define CYCODE
#endif
#ifndef CYDATA
    #define CYDATA
#endif
#ifndef CYFAR
    #define CYFAR
#endif
#ifndef CYXDATA
    #define CYXDATA
#endif


CY_CFG_UNUSED
static void CYMEMZERO(void *s, size_t n);
CY_CFG_UNUSED
static void CYMEMZERO(void *s, size_t n)
{
	(void)memset(s, 0, n);
}
CY_CFG_UNUSED
static void CYCONFIGCPY(void *dest, const void *src, size_t n);
CY_CFG_UNUSED
static void CYCONFIGCPY(void *dest, const void *src, size_t n)
{
	(void)memcpy(dest, src, n);
}
CY_CFG_UNUSED
static void CYCONFIGCPYCODE(void *dest, const void *src, size_t n);
CY_CFG_UNUSED
static void CYCONFIGCPYCODE(void *dest, const void *src, size_t n)
{
	(void)memcpy(dest, src, n);
}




/* Clock startup error codes                                                   */
#define CYCLOCKSTART_NO_ERROR    0u
#define CYCLOCKSTART_XTAL_ERROR  1u
#define CYCLOCKSTART_32KHZ_ERROR 2u
#define CYCLOCKSTART_PLL_ERROR   3u
#define CYCLOCKSTART_FLL_ERROR   4u
#define CYCLOCKSTART_WCO_ERROR   5u


#ifdef CY_NEED_CYCLOCKSTARTUPERROR
/*******************************************************************************
* Function Name: CyClockStartupError
********************************************************************************
* Summary:
*  If an error is encountered during clock configuration (crystal startup error,
*  PLL lock error, etc.), the system will end up here.  Unless reimplemented by
*  the customer, this function will stop in an infinite loop.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
CY_CFG_UNUSED
static void CyClockStartupError(uint8 errorCode);
CY_CFG_UNUSED
static void CyClockStartupError(uint8 errorCode)
{
    /* To remove the compiler warning if errorCode not used.                */
    errorCode = errorCode;

    /* If we have a clock startup error (bad MHz crystal, PLL lock, etc.),  */
    /* we will end up here to allow the customer to implement something to  */
    /* deal with the clock condition.                                       */

#ifdef CY_CFG_CLOCK_STARTUP_ERROR_CALLBACK
    CY_CFG_Clock_Startup_ErrorCallback();
#else
    while(1) {}
#endif /* CY_CFG_CLOCK_STARTUP_ERROR_CALLBACK */
}
#endif

#define CY_CFG_BASE_ADDR_COUNT 4u
CYPACKED typedef struct
{
	uint8 offset;
	uint8 value;
} CYPACKED_ATTR cy_cfg_addrvalue_t;



/*******************************************************************************
* Function Name: cfg_write_bytes32
********************************************************************************
* Summary:
*  This function is used for setting up the chip configuration areas that
*  contain relatively sparse data.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
static void cfg_write_bytes32(const uint32 addr_table[], const cy_cfg_addrvalue_t data_table[]);
static void cfg_write_bytes32(const uint32 addr_table[], const cy_cfg_addrvalue_t data_table[])
{
	/* For 32-bit little-endian architectures */
	uint32 i, j = 0u;
	for (i = 0u; i < CY_CFG_BASE_ADDR_COUNT; i++)
	{
		uint32 baseAddr = addr_table[i];
		uint8 count = (uint8)baseAddr;
		baseAddr &= 0xFFFFFF00u;
		while (count != 0u)
		{
			CY_SET_REG8((void *)(baseAddr + data_table[j].offset), data_table[j].value);
			j++;
			count--;
		}
	}
}
static void ClockInit(void);
static void ClockInit(void)
{
	uint32_t status;

	/* Enable all source clocks */
	status = CY_RET_SUCCESS;          // Removed enabling of WCO as it is already enabled by the Onethinx Core: status = Cy_SysClk_WcoEnable(500000u);
	if (CY_RET_SUCCESS != status)
	{
		CyClockStartupError(CYCLOCKSTART_WCO_ERROR);
	}
	Cy_SysClk_ClkLfSetSource(CY_SYSCLK_CLKLF_IN_WCO);
	{
		cy_stc_ble_bless_eco_cfg_params_t bleCfg =
		{
			.ecoXtalStartUpTime = (785 / 31.25),
			.loadCap = ((9.2 - 7.5) / 0.075),
			.ecoFreq = CY_BLE_BLESS_ECO_FREQ_32MHZ,
			.ecoSysDiv = CY_BLE_SYS_ECO_CLK_DIV_1
		};
		Cy_BLE_EcoStart(&bleCfg);
	}

	/* Configure CPU clock dividers */
	Cy_SysClk_ClkFastSetDivider(0u);
	Cy_SysClk_ClkPeriSetDivider(0u);
	Cy_SysClk_ClkSlowSetDivider(5u);

	/* Configure LF & HF clocks */
	Cy_SysClk_ClkHfSetSource(0u, CY_SYSCLK_CLKHF_IN_CLKPATH0);
	Cy_SysClk_ClkHfSetDivider(0u, CY_SYSCLK_CLKHF_NO_DIVIDE);
	Cy_SysClk_ClkHfEnable(0u);
	Cy_SysClk_ClkHfSetSource(1u, CY_SYSCLK_CLKHF_IN_CLKPATH1);
	Cy_SysClk_ClkHfSetDivider(1u, CY_SYSCLK_CLKHF_NO_DIVIDE);
	Cy_SysClk_ClkHfEnable(1u);

	/* Configure Path Clocks */
	Cy_SysClk_ClkPathSetSource(1, CY_SYSCLK_CLKPATH_IN_WCO);
	Cy_SysClk_ClkPathSetSource(2, CY_SYSCLK_CLKPATH_IN_IMO);
	Cy_SysClk_ClkPathSetSource(3, CY_SYSCLK_CLKPATH_IN_IMO);
	Cy_SysClk_ClkPathSetSource(4, CY_SYSCLK_CLKPATH_IN_IMO);
	Cy_SysClk_ClkPathSetSource(0, CY_SYSCLK_CLKPATH_IN_IMO);
	{
		const cy_stc_fll_manual_config_t fllConfig = 
		{
			.fllMult =         504u,
			.refDiv =          42u,
			.ccoRange =        CY_SYSCLK_FLL_CCO_RANGE2,
			.enableOutputDiv = true,
			.lockTolerance =   10u,
			.igain =           8u,
			.pgain =           7u,
			.settlingCount =   8u,
			.outputMode =      CY_SYSCLK_FLLPLL_OUTPUT_AUTO,
			.cco_Freq =        198u
		};
		status = Cy_SysClk_FllManualConfigure(&fllConfig);
		if (CY_RET_SUCCESS != status)
		{
			CyClockStartupError(CYCLOCKSTART_FLL_ERROR);
		}
	}
	SRSS->CLK_TRIM_CCO_CTL |= 1u << 31;
	status = Cy_SysClk_FllEnable(200000u);
	if (CY_RET_SUCCESS != status)
	{
		CyClockStartupError(CYCLOCKSTART_FLL_ERROR);
	}

	/* Configure miscellaneous clocks */
	Cy_SysClk_ClkTimerSetSource(CY_SYSCLK_CLKTIMER_IN_HF0_DIV8);
	Cy_SysClk_ClkTimerSetDivider(99);
	Cy_SysClk_ClkTimerEnable();
	Cy_SysClk_ClkPumpSetSource(CY_SYSCLK_PUMP_IN_CLKPATH0);
	Cy_SysClk_ClkPumpSetDivider(CY_SYSCLK_PUMP_DIV_2);
	Cy_SysClk_ClkPumpEnable();
	Cy_SysClk_ClkBakSetSource(CY_SYSCLK_BAK_IN_WCO);
	Cy_SysTick_SetClockSource(CY_SYSTICK_CLOCK_SOURCE_CLK_IMO);

	/* Disable clocks started by default */
	Cy_SysClk_IloDisable();

	/* Set memory wait states based on 48 MHz HFClk[0] */
	Cy_SysLib_SetWaitStates(false, 48);
}


/* Analog API Functions */


/*******************************************************************************
* Function Name: AnalogSetDefault
********************************************************************************
*
* Summary:
*  Sets up the analog portions of the chip to default values based on chip
*  configuration options from the project.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void AnalogSetDefault(void);
static void AnalogSetDefault(void)
{
	CY_SET_REG32(CYREG_PASS_AREF_AREF_CTRL, 0x80110001u);
}




/*******************************************************************************
* Function Name: Cy_SystemInit
********************************************************************************
* Summary:
*  This function is called by the start-up code for the selected device. It
*  performs all of the necessary device configuration based on the design
*  settings.  This includes settings from the Design Wide Resources (DWR) such
*  as Clocks and Pins as well as any component configuration that is necessary.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/

void Cy_SystemInit(void)
{
	/* Removed Onethinx Core conflicting code by Onethinx CoreStripper */
	//	/* Set worst case memory wait states (150 MHz), ClockInit() will update */
	Cy_SysLib_SetWaitStates(false, 150);
	//	Cy_SysLib_SetWaitStates(false, 150);
	//
	//	if(0u == Cy_SysLib_GetResetReason()) /* POR, XRES, or BOD */
	//	{
	//		Cy_SysLib_ResetBackupDomain();
	//		Cy_SysClk_IloDisable();
	//		Cy_SysClk_IloEnable();
	//	}
	//
	//	/* Power Mode */
	//	Cy_SysPm_LdoSetVoltage(CY_SYSPM_LDO_VOLTAGE_1_1V);
	//
	//	/* PMIC Control */
	//	Cy_SysPm_UnlockPmic();
	//	Cy_SysPm_DisablePmicOutput();
	/* Resume non-conflicting code by Onethinx CoreStripper */

	/* Pin0_0 configuration */
	{
	    const cy_stc_gpio_pin_config_t pin0_0_cfg =
	    {
	        .outVal    = 0x00u,
	        .driveMode = 0x00u,
	        .hsiom     = P0_0_GPIO,
	    };
	    (void)Cy_GPIO_Pin_Init(GPIO_PRT0, 0, &pin0_0_cfg);
	}

	/* Pin0_1 configuration */
	{
	    const cy_stc_gpio_pin_config_t pin0_1_cfg =
	    {
	        .outVal    = 0x00u,
	        .driveMode = 0x00u,
	        .hsiom     = P0_1_GPIO,
	    };
	    (void)Cy_GPIO_Pin_Init(GPIO_PRT0, 1, &pin0_1_cfg);
	}

	/* Clock */
	ClockInit();
	{
		static const uint32 CYCODE cy_cfg_addr_table[] = {
			0x40340401u, /* Base address: 0x40340400 Count: 1 */
			0x40342A5Fu, /* Base address: 0x40342A00 Count: 95 */
			0x40347201u, /* Base address: 0x40347200 Count: 1 */
			0x40347802u, /* Base address: 0x40347800 Count: 2 */
		};

		static const cy_cfg_addrvalue_t CYCODE cy_cfg_data_table[] = {
			{0x2Cu, 0x0Fu},
			{0x03u, 0x8Cu},
			{0x04u, 0x03u},
			{0x06u, 0x0Cu},
			{0x07u, 0x08u},
			{0x08u, 0x03u},
			{0x0Au, 0x14u},
			{0x0Eu, 0x18u},
			{0x11u, 0xACu},
			{0x13u, 0x43u},
			{0x15u, 0x31u},
			{0x17u, 0xC6u},
			{0x19u, 0x31u},
			{0x1Bu, 0x4Eu},
			{0x1Du, 0x30u},
			{0x1Fu, 0x40u},
			{0x22u, 0x05u},
			{0x23u, 0x53u},
			{0x26u, 0x08u},
			{0x2Cu, 0x1Au},
			{0x2Eu, 0x04u},
			{0x31u, 0x0Fu},
			{0x32u, 0x1Cu},
			{0x34u, 0x03u},
			{0x35u, 0xF0u},
			{0x38u, 0x20u},
			{0x3Au, 0x08u},
			{0x3Bu, 0x22u},
			{0x3Eu, 0x10u},
			{0x40u, 0x02u},
			{0x45u, 0x04u},
			{0x49u, 0xFFu},
			{0x4Au, 0x07u},
			{0x4Bu, 0xFFu},
			{0x4Cu, 0x40u},
			{0x4Du, 0x20u},
			{0x4Eu, 0xF0u},
			{0x4Fu, 0x05u},
			{0x50u, 0x08u},
			{0x5Cu, 0x04u},
			{0x5Du, 0x04u},
			{0x5Eu, 0x04u},
			{0x60u, 0x99u},
			{0x61u, 0x09u},
			{0x63u, 0x01u},
			{0x64u, 0x40u},
			{0x65u, 0xA8u},
			{0x66u, 0x40u},
			{0x67u, 0x20u},
			{0x80u, 0x40u},
			{0x82u, 0x80u},
			{0x84u, 0x10u},
			{0x88u, 0xB0u},
			{0x8Au, 0x0Cu},
			{0x8Cu, 0x09u},
			{0x8Eu, 0x10u},
			{0x91u, 0x01u},
			{0x95u, 0x02u},
			{0x9Au, 0x03u},
			{0x9Eu, 0x80u},
			{0xA0u, 0x14u},
			{0xA4u, 0xACu},
			{0xA6u, 0x11u},
			{0xACu, 0x1Cu},
			{0xAEu, 0xA2u},
			{0xB0u, 0x80u},
			{0xB1u, 0x02u},
			{0xB4u, 0x1Fu},
			{0xB5u, 0x01u},
			{0xB6u, 0x60u},
			{0xB8u, 0x80u},
			{0xBEu, 0x01u},
			{0xBFu, 0x11u},
			{0xC0u, 0x31u},
			{0xC1u, 0x06u},
			{0xC5u, 0xC0u},
			{0xC6u, 0x0Bu},
			{0xC7u, 0xE0u},
			{0xC8u, 0x26u},
			{0xC9u, 0xFFu},
			{0xCAu, 0xFFu},
			{0xCBu, 0xFFu},
			{0xCEu, 0xF0u},
			{0xCFu, 0x44u},
			{0xD0u, 0x0Cu},
			{0xD6u, 0x01u},
			{0xDCu, 0x04u},
			{0xDDu, 0x04u},
			{0xDEu, 0x04u},
			{0xDFu, 0x04u},
			{0xE0u, 0x09u},
			{0xE1u, 0x99u},
			{0xE3u, 0x01u},
			{0xEAu, 0xC0u},
			{0xEEu, 0x40u},
			{0xEFu, 0x02u},
			{0xA8u, 0x01u},
			{0x04u, 0x01u},
			{0x19u, 0x01u},
		};



		CYPACKED typedef struct {
			void *address;
			uint16 size;
		} CYPACKED_ATTR cfg_memset_t;


		CYPACKED typedef struct {
			void *dest;
			const void *src;
			size_t size;
		} CYPACKED_ATTR cfg_memcpy_t;

		static const cfg_memset_t CYCODE cfg_memset_list[] = {
			/* address, size */
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR0_UDBSNG0_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR0_UDBSNG1_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR1_UDBSNG0_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR1_UDBSNG1_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR2_UDBSNG0_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR2_UDBSNG1_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR3_UDBSNG0_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR3_UDBSNG1_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR4_UDBSNG0_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR4_UDBSNG1_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR5_UDBSNG0_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR5_UDBSNG1_BASE), 116u},
		};

		/* UDB_UDBPAIR0_ROUTE Address: CYDEV_UDB_UDBPAIR0_ROUTE_BASE Size (bytes): 144 */
		static const uint8 CYCODE BS_UDB_UDBPAIR0_ROUTE_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_UDBPAIR1_ROUTE Address: CYDEV_UDB_UDBPAIR1_ROUTE_BASE Size (bytes): 144 */
		static const uint8 CYCODE BS_UDB_UDBPAIR1_ROUTE_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_UDBPAIR2_ROUTE Address: CYDEV_UDB_UDBPAIR2_ROUTE_BASE Size (bytes): 144 */
		static const uint8 CYCODE BS_UDB_UDBPAIR2_ROUTE_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_UDBPAIR3_ROUTE Address: CYDEV_UDB_UDBPAIR3_ROUTE_BASE Size (bytes): 144 */
		static const uint8 CYCODE BS_UDB_UDBPAIR3_ROUTE_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_UDBPAIR4_ROUTE Address: CYDEV_UDB_UDBPAIR4_ROUTE_BASE Size (bytes): 144 */
		static const uint8 CYCODE BS_UDB_UDBPAIR4_ROUTE_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x01u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x10u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_UDBPAIR5_ROUTE Address: CYDEV_UDB_UDBPAIR5_ROUTE_BASE Size (bytes): 144 */
		static const uint8 CYCODE BS_UDB_UDBPAIR5_ROUTE_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x10u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x22u, 0x36u, 0xFFu, 0x15u, 
			0x44u, 0x0Fu, 0xFFu, 0xFFu, 0xFFu, 0x34u, 0xFFu, 0xFFu, 0xF5u, 0xF6u, 0x2Fu, 0x34u, 0x27u, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0x00u, 0x00u, 0x00u, 0x2Fu, 0x36u, 0xFFu, 0xFFu, 0x22u, 0x6Fu, 0xFFu, 0x1Fu, 0x44u, 0x46u, 0xFFu, 0x02u, 
			0x3Fu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x01u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0x83u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x13u, 0x64u, 0x57u, 0x11u, 0x91u, 0x37u, 
			0x11u, 0x11u, 0x61u, 0x11u, 0x3Du, 0x11u, 0x11u, 0x13u, 0x11u, 0x13u, 0x14u, 0x11u, 0x21u, 0x11u, 0x21u, 0x12u, 
			0x11u, 0x83u, 0x15u, 0x11u, 0x11u, 0x11u, 0x11u, 0x41u, 0x11u, 0x14u, 0x11u, 0x11u, 0x11u, 0x41u, 0x11u, 0x11u};

		/* UDB_DSI0 Address: CYDEV_UDB_DSI0_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI0_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_DSI1 Address: CYDEV_UDB_DSI1_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI1_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_DSI2 Address: CYDEV_UDB_DSI2_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI2_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_DSI3 Address: CYDEV_UDB_DSI3_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI3_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_DSI4 Address: CYDEV_UDB_DSI4_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI4_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_DSI5 Address: CYDEV_UDB_DSI5_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI5_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_DSI6 Address: CYDEV_UDB_DSI6_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI6_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_DSI7 Address: CYDEV_UDB_DSI7_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI7_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_DSI8 Address: CYDEV_UDB_DSI8_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI8_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_DSI9 Address: CYDEV_UDB_DSI9_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI9_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_DSI10 Address: CYDEV_UDB_DSI10_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI10_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x02u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x0Fu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x1Du, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_DSI11 Address: CYDEV_UDB_DSI11_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI11_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		static const cfg_memcpy_t CYCODE cfg_memcpy_list [] = {
			/* dest, src, size */
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR0_ROUTE_BASE), BS_UDB_UDBPAIR0_ROUTE_VAL, 144u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR1_ROUTE_BASE), BS_UDB_UDBPAIR1_ROUTE_VAL, 144u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR2_ROUTE_BASE), BS_UDB_UDBPAIR2_ROUTE_VAL, 144u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR3_ROUTE_BASE), BS_UDB_UDBPAIR3_ROUTE_VAL, 144u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR4_ROUTE_BASE), BS_UDB_UDBPAIR4_ROUTE_VAL, 144u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR5_ROUTE_BASE), BS_UDB_UDBPAIR5_ROUTE_VAL, 144u},
			{(void CYFAR *)(CYDEV_UDB_DSI0_BASE), BS_UDB_DSI0_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI1_BASE), BS_UDB_DSI1_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI2_BASE), BS_UDB_DSI2_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI3_BASE), BS_UDB_DSI3_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI4_BASE), BS_UDB_DSI4_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI5_BASE), BS_UDB_DSI5_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI6_BASE), BS_UDB_DSI6_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI7_BASE), BS_UDB_DSI7_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI8_BASE), BS_UDB_DSI8_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI9_BASE), BS_UDB_DSI9_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI10_BASE), BS_UDB_DSI10_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI11_BASE), BS_UDB_DSI11_VAL, 124u},
		};

		size_t i;

		/* Power on the UDB array */
		CY_SET_REG32(0x402101F0u, 0x05FA0003u);

		/* Zero out critical memory blocks before beginning configuration */
		for (i = 0u; i < (sizeof(cfg_memset_list)/sizeof(cfg_memset_list[0])); i++)
		{
			const cfg_memset_t *ms = &cfg_memset_list[i];
			CYMEMZERO(ms->address, ms->size);
		}

		/* Copy device configuration data into registers */
		for (i = 0u; i < (sizeof(cfg_memcpy_list)/sizeof(cfg_memcpy_list[0])); i++)
		{
			const cfg_memcpy_t *mc = &cfg_memcpy_list[i];
			CYCONFIGCPYCODE(mc->dest, mc->src, mc->size);
		}

		cfg_write_bytes32(cy_cfg_addr_table, cy_cfg_data_table);

		/* Enable UDB array and digital routing */
		CY_SET_REG32((void *)0x40347900u, CY_GET_REG32((void *)0x40347900u) | 0x06u);
	}

	/* Port0 configuration */
	{
	    const cy_stc_gpio_prt_config_t port0_cfg =
	    {
	        .out        = 0x00000000u,
	        .intrMask   = 0x00000000u,
	        .intrCfg    = 0x00000000u,
	        .cfg        = 0x00000000u,
	        .cfgIn      = 0x00000000u,
	        .cfgOut     = 0x00000000u,
	        .cfgSIO     = 0x00000000u,
	        .sel0Active = 0x00000000u,
	        .sel1Active = 0x00000000u,
	    };
	    (void)Cy_GPIO_Port_Init(GPIO_PRT0, &port0_cfg);
	}

	/* Port9 configuration */
	{
	    const cy_stc_gpio_prt_config_t port9_cfg =
	    {
	        .out        = 0x00000001u,
	        .intrMask   = 0x00000000u,
	        .intrCfg    = 0x00000000u,
	        .cfg        = 0x00000006u,
	        .cfgIn      = 0x00000000u,
	        .cfgOut     = 0x00000000u,
	        .cfgSIO     = 0x00000000u,
	        .sel0Active = 0x00000003u,
	        .sel1Active = 0x00000000u,
	    };
	    (void)Cy_GPIO_Port_Init(GPIO_PRT9, &port9_cfg);
	}


	/* Perform basic analog initialization to defaults */
	AnalogSetDefault();

}
