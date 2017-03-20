/******************************************************************************

    @file    IntrOS: osport.c
    @author  Rajmund Szymanski
    @date    20.03.2017
    @brief   IntrOS port file for STM32F3 uC.

 ******************************************************************************

    IntrOS - Copyright (C) 2013 Rajmund Szymanski.

    This file is part of IntrOS distribution.

    IntrOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published
    by the Free Software Foundation; either version 3 of the License,
    or (at your option) any later version.

    IntrOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include <oskernel.h>

/* -------------------------------------------------------------------------- */

void port_sys_init( void )
{
#if OS_TIMER

/******************************************************************************
 Put here configuration of system timer for tick-less mode
*******************************************************************************/

	#if CPU_FREQUENCY/OS_FREQUENCY-1 > UINT16_MAX
	#error Incorrect Timer frequency!
	#endif

	RCC->APB1ENR |= OS_TIM_CLK_ENABLE;
	OS_TIM->PSC  = CPU_FREQUENCY/OS_FREQUENCY-1;
	OS_TIM->EGR  = TIM_EGR_UG;
	OS_TIM->CR1  = TIM_CR1_CEN;

/******************************************************************************
 End of configuration
*******************************************************************************/

#else //OS_TIMER == 0

/******************************************************************************
 Put here configuration of system timer for non-tick-less mode
*******************************************************************************/

	#if (CPU_FREQUENCY/OS_FREQUENCY-1 <= SysTick_LOAD_RELOAD_Msk)

	SysTick_Config(CPU_FREQUENCY/OS_FREQUENCY);

	#elif defined(ST_FREQUENCY) && (ST_FREQUENCY/OS_FREQUENCY-1 <= SysTick_LOAD_RELOAD_Msk)

	NVIC_SetPriority(SysTick_IRQn, 0xFF);

	SysTick->LOAD = ST_FREQUENCY/OS_FREQUENCY-1;
	SysTick->VAL  = 0U;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk|SysTick_CTRL_TICKINT_Msk;

	#else
	#error Incorrect SysTick frequency!
	#endif

/******************************************************************************
 End of configuration
*******************************************************************************/

#endif//OS_TIMER
}

/* -------------------------------------------------------------------------- */

#if OS_TIMER == 0

/******************************************************************************
 Put here the procedure of interrupt handler of system timer for non-tick-less mode
*******************************************************************************/

void SysTick_Handler( void )
{
	System.cnt++;
}

/******************************************************************************
 End of the procedure of interrupt handler
*******************************************************************************/

#endif//OS_TIMER

/* -------------------------------------------------------------------------- */
