/*
******************************************************************************
* @file    SensorTile_BlueNRG.c
* @author  Central Labs
* @version V1.2.0
 * @date    10-Nov-2016
* @brief   This file provides the BLE driver for the Sensor Tile
*          board
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/
#include "SensorTile_BlueNRG.h"
#include "SensorTile.h"
//#include "gp_timer.h"
//#include "debug.h"

#ifdef PRINT_CSV_FORMAT
extern volatile uint32_t ms_counter;
#endif /* PRINT_CSV_FORMAT */

/** @addtogroup BSP
 *  @{
 */

/** @addtogroup SENSORTILE
 *  @{
 */
 
/** @addtogroup SENSORTILE_BLE
* @brief This file provides a set of firmware functions to manage BLE 
 *  @{
 */

/** @defgroup SENSORTILE_BLE_Private_Defines SENSORTILE_BLE Private Defines
* @{
*/ 


/**
* @}
*/

/** @defgroup SENSORTILE_BLE_Private_Variables SENSORTILE_BLE Private Variables
* @{
*/

void * SpiHandle;

/**
* @}
*/

/** @defgroup SENSORTILE_BLE_Private_Function_Prototypes SENSORTILE_BLE Private Function Prototypes
*  @{
*/

/* Private function prototypes -----------------------------------------------*/
/*static void us150Delay(void);
void set_irq_as_output(void);
void set_irq_as_input(void);
*/
/**
* @}
*/ 

/** @defgroup SENSORTILE_BLE_Exported_Functions SENSORTILE_BLE Exported Functions
* @{
*/ 

#ifdef PRINT_CSV_FORMAT
/**
* @brief  This function is a utility to print the log time
*          in the format HH:MM:SS:MSS (DK GUI time format)
* @param  None
* @retval None
*/
void print_csv_time(void){
  uint32_t ms = ms_counter;
  PRINT_CSV("%02d:%02d:%02d.%03d", ms/(60*60*1000)%24, ms/(60*1000)%60, (ms/1000)%60, ms%1000);
}
#endif /* PRINT_CSV_FORMAT */

/**
* @brief  This function is used for low level initialization of the SPI 
*         communication with the BlueNRG Expansion Board.
* @param  hspi: SPI handle.
* @retval None
*/


static SPIConfig bluenrg_cfg = {
  NULL,
  GPIOB,
  2,
  SPI_CR1_BR_2 | SPI_CR1_BR_1,
  0
};





/**
* @brief  Writes data to a serial interface.
* @param  data1   :  1st buffer
* @param  data2   :  2nd buffer
* @param  n_bytes1: number of bytes in 1st buffer
* @param  n_bytes2: number of bytes in 2nd buffer
* @retval None
*/
void Hal_Write_Serial(const void* data1, const void* data2, int32_t n_bytes1,
                      int32_t n_bytes2)
{
  //struct timer t;
  
  //Timer_Set(&t, CLOCK_SECOND/10);
  
#ifdef PRINT_CSV_FORMAT
  print_csv_time();
  for (int i=0; i<n_bytes1; i++) {
    PRINT_CSV(" %02x", ((uint8_t *)data1)[i]);
  }
  for (int i=0; i<n_bytes2; i++) {
    PRINT_CSV(" %02x", ((uint8_t *)data2)[i]);
  }
  PRINT_CSV("\n");
#endif
  
  while(1){
    if(Sensor_IO_SPI_BlueNRG_Write(&bluenrg_cfg, (uint8_t *)data1,(uint8_t *)data2, n_bytes1, n_bytes2)==0) break;
    /*if(Timer_Expired(&t)){
      break;
    }*/
  }
}

/**
* @brief  Initializes the SPI communication with the BlueNRG
*         Expansion Board.
* @param  None
* @retval None
*/
/*void BNRG_SPI_Init(void)
{
  SpiHandle.Instance = BNRG_SPI_INSTANCE;
  SpiHandle.Init.Mode = BNRG_SPI_MODE;
  SpiHandle.Init.Direction = BNRG_SPI_DIRECTION;
  SpiHandle.Init.DataSize = BNRG_SPI_DATASIZE;
  SpiHandle.Init.CLKPolarity = BNRG_SPI_CLKPOLARITY;
  SpiHandle.Init.CLKPhase = BNRG_SPI_CLKPHASE;
  SpiHandle.Init.NSS = BNRG_SPI_NSS;
  SpiHandle.Init.FirstBit = BNRG_SPI_FIRSTBIT;
  SpiHandle.Init.TIMode = BNRG_SPI_TIMODE;
  SpiHandle.Init.CRCPolynomial = BNRG_SPI_CRCPOLYNOMIAL;
  SpiHandle.Init.BaudRatePrescaler = BNRG_SPI_BAUDRATEPRESCALER;
  SpiHandle.Init.CRCCalculation = BNRG_SPI_CRCCALCULATION;
  
  HAL_SPI_Init(&SpiHandle);
}*/

/**
* @brief  Resets the BlueNRG.
* @param  None
* @retval None
*/
/*void BlueNRG_RST(void)
{
  HAL_GPIO_WritePin(BNRG_SPI_RESET_PORT, BNRG_SPI_RESET_PIN, GPIO_PIN_RESET);
  HAL_Delay(5);
  HAL_GPIO_WritePin(BNRG_SPI_RESET_PORT, BNRG_SPI_RESET_PIN, GPIO_PIN_SET);
  HAL_Delay(5);
}*/

/**
* @brief  Reports if the BlueNRG has data for the host micro.
* @param  None
* @retval 1 if data are present, 0 otherwise
*/
// FIXME: find a better way to handle this return value (bool type? TRUE and FALSE)
/*uint8_t BlueNRG_DataPresent(void)
{
  if (HAL_GPIO_ReadPin(BNRG_SPI_EXTI_PORT, BNRG_SPI_EXTI_PIN) == GPIO_PIN_SET)
    return 1;
  else  
    return 0;
}*/ /* end BlueNRG_DataPresent() */

/**
* @brief  Activate internal bootloader using pin.
* @param  None
* @retval None
*/
/*void BlueNRG_HW_Bootloader(void)
{
  Disable_SPI_IRQ();
  set_irq_as_output();
  BlueNRG_RST();
  set_irq_as_input();
  Enable_SPI_IRQ();
}*/




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
