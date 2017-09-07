void set_irq_as_output(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Pull IRQ high */
  GPIO_InitStructure.Pin = BNRG_SPI_IRQ_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = BNRG_SPI_IRQ_SPEED;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BNRG_SPI_IRQ_PORT, &GPIO_InitStructure);
  HAL_GPIO_WritePin(BNRG_SPI_IRQ_PORT, BNRG_SPI_IRQ_PIN, GPIO_PIN_SET);
}

/**
* @brief  Set the IRQ in input mode.
* @param  None
* @retval None
*/
void set_irq_as_input(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* IRQ input */  
  GPIO_InitStructure.Pin = BNRG_SPI_IRQ_PIN;
  GPIO_InitStructure.Mode = BNRG_SPI_IRQ_MODE;
  GPIO_InitStructure.Pull = GPIO_PULLDOWN;
  GPIO_InitStructure.Speed = BNRG_SPI_IRQ_SPEED;
  GPIO_InitStructure.Alternate = BNRG_SPI_IRQ_ALTERNATE;    
  HAL_GPIO_Init(BNRG_SPI_IRQ_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pull = BNRG_SPI_IRQ_PULL;
  HAL_GPIO_Init(BNRG_SPI_IRQ_PORT, &GPIO_InitStructure);
}

/**
* @brief  Utility function for delay
* @param  None
* @retval None
* NOTE: TODO: implement with clock-independent function.
*/
static void us150Delay(void)
{
#if SYSCLK_FREQ == 4000000
  for(volatile int i = 0; i < 35; i++)__NOP();
#elif SYSCLK_FREQ == 32000000
  for(volatile int i = 0; i < 420; i++)__NOP();
#elif SYSCLK_FREQ == 80000000
  for(volatile int i = 0; i < 1072; i++)__NOP();
#elif SYSCLK_FREQ == 84000000
  for(volatile int i = 0; i < 1125; i++)__NOP();
#elif SYSCLK_FREQ == 168000000
  for(volatile int i = 0; i < 2250; i++)__NOP();
#else
#error Implement delay function.
#endif    
}

/**
* @brief  Enable SPI IRQ.
* @param  None
* @retval None
*/
void Enable_SPI_IRQ(void)
{
  HAL_NVIC_EnableIRQ(BNRG_SPI_EXTI_IRQn);  
}

/**
* @brief  Disable SPI IRQ.
* @param  None
* @retval None
*/
void Disable_SPI_IRQ(void)
{ 
  HAL_NVIC_DisableIRQ(BNRG_SPI_EXTI_IRQn);
}

/**
* @brief  Clear Pending SPI IRQ.
* @param  None
* @retval None
*/
void Clear_SPI_IRQ(void)
{
  HAL_NVIC_ClearPendingIRQ(BNRG_SPI_EXTI_IRQn);
}

/**
* @brief  Clear EXTI (External Interrupt) line for SPI IRQ.
* @param  None
* @retval None
*/
void Clear_SPI_EXTI_Flag(void)
{  
  __HAL_GPIO_EXTI_CLEAR_IT(BNRG_SPI_EXTI_PIN);  
}
