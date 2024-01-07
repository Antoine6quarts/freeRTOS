
#include <stdbool.h>
#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "../Components/ili9341/ili9341.h"
#include "devices.h"
#include "main.h"


/* ******************************************************
 * 		Prototypes
 * ******************************************************/


/* ******************************************************
 * 		Global Context
 * ******************************************************/
int16_t previous = 10;
int cycle = 0;
uint16_t sensor = 0;

/* ******************************************************
 * 		Entry Point
 * ******************************************************/

int main(void)
{
	/* Sensor */
	char msg[20] = {0};

	bool toggled = false;
	uint32_t back_color = LCD_COLOR_LIGHTGRAY;
	uint32_t text_color = LCD_COLOR_BLACK;

	setup();						// init devices

	LCDdisplayInit(COLORS_NORMAL);  // init display

	for( ;; )
	{
		if (BSP_PB_GetState(BUTTON_KEY) != 0)
		{
			toggled = !toggled;
			BSP_LED_Toggle(LED3);
			LCDdisplayInit(toggled ? COLORS_REVERSE : COLORS_NORMAL);
			back_color = toggled ? LCD_COLOR_DARKGRAY : LCD_COLOR_LIGHTGRAY;
			text_color = toggled ? LCD_COLOR_WHITE : LCD_COLOR_BLACK;
		}

		HAL_Delay(500);
		BSP_LCD_SetBackColor(back_color);
		snprintf(msg, 20, "%d", cycle);
		LCDdisplayText(8, LCD_COLOR_GREEN, msg);
	}
}

/* ******************************************************
 * 	FreeRTOS
 * ******************************************************/


/* ******************************************************
 * 		Callbacks
 * ******************************************************/

void TIM2_IRQHandler(void)
{
	if (__HAL_TIM_GET_ITSTATUS(&gTimHandle, TIM_IT_UPDATE))
	{
		__HAL_TIM_CLEAR_IT(&gTimHandle, TIM_IT_UPDATE);
	}
}

void ADC_IRQHandler(void)
{
	if (__HAL_ADC_GET_IT_SOURCE(&gAdcHandle, ADC_IT_EOC))
	{
		sensor = HAL_ADC_GetValue(&gAdcHandle);
		cycle++;
		__HAL_ADC_CLEAR_FLAG(&gAdcHandle, ADC_IT_EOC);
	}
}

/* ******************************************************
 * 	Functions
 * ******************************************************/

void setup(void)
{
	HAL_Init();
	SystemClock_Config();
	initLCD();
	initGPIO();
	initADC();
	initTimer();
}


