/*******************************************************************************
* Copyright (C) 2020-2022, CVA Systems (R),All Rights Reserved.
*
* File Name: cfg-global.h
*
* Author: KC
*
* Created: 2020-08-25
*
*  Abstract:
*
* Reference:
*
* Version: 0.1:
*******************************************************************************/

#ifndef __GLOBAL_CONFIG_H__
#define __GLOBAL_CONFIG_H__

/*******************************************************************************
* 1.Included files
*******************************************************************************/


/*******************************************************************************
* 2.Global constant and macro definitions using #define
*******************************************************************************/
//TEST_FOR_PC_TOOL_SCRIPT OR OTHER MUST ENABLE ONE
#define TEST_FOR_PC_TOOL_SCRIPT                   0
#define TEST_FOR_CONVDONE                         1
#define TEST_FOR_COMPENSATION                     0
#define TEST_FOR_PROX_FAR                         0
#define TEST_FOR_PROX_FAR_AND                     0
#define TEST_FOR_IDLE                             0
#define TEST_FOR_AUTOCOMP_SINGLE                  0
#define TEST_FOR_AUTOCOMP_GENERAL                 0
#define TEST_FOR_AVGFREEZE_DIS                    0
#define TEST_FOR_HYST                             0
#define TEST_FOR_FORCE_CC                         0
#define TEST_FOR_STARTUP_DETECT_START             0
#define TEST_FOR_STARTUP_DETECT_ANY               0
#define TEST_FOR_STEADY                           0
#define TEST_FOR_REF_COR                          0
#define TEST_FOR_REF                              0
#define TEST_FOR_IRQCFG_PROG0                     0
#define TEST_FOR_HOSTCOMP                         0
#define TEST_FOR_SLEEP_WAKEUP                     0
#define TEST_FOR_RESET                            0
#define TEST_FOR_TEMP_CAP                         0
#define TEST_FOR_REF_TEMP                         0
#define TEST_FOR_STATE_SWITCH                     0
#define TEST_FOR_PRESSURE_I2C                     0
#define TEST_FOR_EFUSE                            0
#define TEST_ADC_TEST                             0
#define TEST_ADC_TEST2                            0
#define TEST_FOR_TEMP_ADC                         0
/*******************************************************************************
* 3.Global structures, unions and enumerations using typedef
*******************************************************************************/

/*******************************************************************************
* 4.Global variable extern declarations
*******************************************************************************/

/*******************************************************************************
* 5.Global function prototypes
*******************************************************************************/

#endif //GLOBAL_CONFIG_H
