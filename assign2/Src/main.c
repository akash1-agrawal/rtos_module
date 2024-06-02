/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "led.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void vLedPeriodicTask1(void *param)
{
	portTickType wakeTime = xTaskGetTickCount();
	while(1)
	{
		led_toggle_g();
		vTaskDelayUntil(&wakeTime ,500/ portTICK_RATE_MS);
	}
	vTaskDelete(NULL);
}

void vLedAperiodicTask2(void *param)
{
	while(1)
	{
		led_toggle_r();
		vTaskDelay(1000/ portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
}

void vLedPeriodicTask3(void *param)
{
	portTickType wakeTime = xTaskGetTickCount();
	while(1)
	{
		led_toggle_b();
		vTaskDelayUntil(&wakeTime,1500/portTICK_RATE_MS);
	}
	vTaskDelete(NULL);
}

void VLedAperioadicTask4(void *param)
{
	while(1)
	{
		led_toggle_o();
		vTaskDelay(2000/portTICK_RATE_MS);
	}
	vTaskDelete(NULL);
}


int main(void)
{
    BaseType_t ret;
    led_init();

    ret = xTaskCreate(vLedPeriodicTask1, "Led Toggle_G", configMINIMAL_STACK_SIZE,NULL,1,NULL);
    if(ret!=pdTRUE)
    	while(1);

    ret = xTaskCreate(vLedAperiodicTask2, "Led Toggle_R", configMINIMAL_STACK_SIZE, NULL,2, NULL);
    if(ret!=pdTRUE)
    	while(1);

    ret = xTaskCreate(vLedPeriodicTask3, "Led Toggle_B", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
    if(ret!=pdTRUE)
    	while(1);

    ret = xTaskCreate(VLedAperioadicTask4, "Led Toggle_O", configMINIMAL_STACK_SIZE, NULL, 4, NULL);
    if(ret!=pdTRUE)
    	while(1);

    vTaskStartScheduler();
    while(1);
    return 0;
}
