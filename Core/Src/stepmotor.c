#include "main.h"
#include "button.h"
#include "cmsis_os.h"
#include <stdio.h>

void set_rpm(int rpm);
void stepmotor_drive(int step);

extern void delay_us(unsigned long us);
extern int get_button(GPIO_TypeDef *GPIO, uint16_t GPIO_PIN, uint8_t button_number);
extern void lcd_string(uint8_t *str);
extern void move_cursor(uint8_t row, uint8_t column);
extern void i2c_lcd_init(void);

/*
 RPM (Revolutions Per Minutes) : 분당 회전수
 1분 : 60초 : 1,000,000us(1초) x 60 = 60,000,000us
 1,000,000us(1초)
 --> 1초(1,000ms) => 1ms(1,000us) x 1,000ms ==> 1,000,000us
 4096스텝 : 1바퀴(4096스텝 이동)
 ---> 1바퀴 도는데 필요한 총 스텝 수 : 4096
 5096 / 8(0.7) ==> 512 sequence : 360도
 1 sequence(8 step) : 0.70312도
 0.70312도 x 512 sequence = 360
*/

// ---------- set_rpm(13) 으로 지정 시의 동작 상황 ----------
// 60,000,000us(1분) / 4096 / rpm
// 1126us(1스텝 idle타임) x 4096 = 4,612,096us
//                           = 4612ms
//							 = 4.6초
// 60초 / 4.6(1회전 시 소요시간 초) ==> 13회전

void set_rpm(int rpm)	// rpm 1 ~ 13
{
	delay_us(60000000 / 4096 / rpm);
	// 최대 speed 기준(13) : delay_us(1126);
}

// 시계방향으로 1회전 <--> 반시계방향으로 1회전
int prev_floor = 1;
extern osMutexId_t myMutex01Handle;

void step_motor_main_test(int floor) {
	char lcd_buff[40];

	printf("step motor : %d\nprev_floor  : %d\n", floor, prev_floor);

	i2c_lcd_init();
	sprintf(lcd_buff, "now at:     %dF", prev_floor);
	// 기존에 lock이 걸려 있으면 풀릴 때까지 기다린다.
	// control이 다른 task로 넘어 가지 않게 lock을 건다.
	if (osMutexWait(myMutex01Handle, 1000) == osOK)
	{
		move_cursor(0, 0);
		lcd_string(lcd_buff);
		osMutexRelease(myMutex01Handle);
		// unlock control이 다른 task로 넘어가도록 한다.
	}
	sprintf(lcd_buff, "heading to: %dF", floor);
	// 기존에 lock이 걸려 있으면 풀릴 때까지 기다린다.
	// control이 다른 task로 넘어 가지 않게 lock을 건다.
	if (osMutexWait(myMutex01Handle, 1000) == osOK)
	{
		move_cursor(1, 0);
		lcd_string(lcd_buff);
		osMutexRelease(myMutex01Handle);
		// unlock control이 다른 task로 넘어가도록 한다.
	}

	// if (prev_floor == floor) return;

	// Go down
	if (prev_floor > floor)
	{
		for (int f = 0; f < abs(prev_floor - floor) + 1; f++)
		{
			for (int i = 0; i < 512; i++)	// 반시계방향 1회전
			{
				for (int j = 7; j >= 0; j--)
				{
					stepmotor_drive(j);
					set_rpm(13);
				}
			}
		}
	}
	// Go up
	else
	{
		for (int f = 0; f < abs(prev_floor - floor) + 1; f++)
		{
			for (int i = 0; i < 512; i++)	// 시계방향 1회전
			{
				for (int j = 0; j < 8; j++)
				{
					stepmotor_drive(j);
					set_rpm(13);
				}
			}
		}
	}

	prev_floor = floor;

	printf("step motor : %d\nprev_floor  : %d\n", floor, prev_floor);
}

void stepmotor_drive(int step)
{
	switch(step) {
	case 0:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 0);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 0);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 0);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 0);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 0);
		break;
	case 5:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 1);
		break;
	case 6:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 1);
		break;
	case 7:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 1);
		break;
	}
}
