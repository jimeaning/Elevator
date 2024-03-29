#include "main.h"

int dotmatrix_main_test(void);
// void init_dotmatrix(void);
void init_dotmatrix(int floor);
extern volatile int TIM10_dotmatrix;

GPIO_TypeDef *col_port[] =
{
		COL1_GPIO_Port, COL2_GPIO_Port, COL3_GPIO_Port, COL4_GPIO_Port,
		COL5_GPIO_Port, COL6_GPIO_Port, COL7_GPIO_Port, COL8_GPIO_Port
};

GPIO_TypeDef *row_port[] =
{
		ROW1_GPIO_Port, ROW2_GPIO_Port, ROW3_GPIO_Port, ROW4_GPIO_Port,
		ROW5_GPIO_Port, ROW6_GPIO_Port, ROW7_GPIO_Port, ROW8_GPIO_Port
};

uint16_t row_pin[] =
{
		ROW1_Pin, ROW2_Pin, ROW3_Pin, ROW4_Pin,
		ROW5_Pin, ROW6_Pin, ROW7_Pin, ROW8_Pin
};

uint16_t col_pin[] =
{
		COL1_Pin, COL2_Pin, COL3_Pin, COL4_Pin,
		COL5_Pin, COL6_Pin, COL7_Pin, COL8_Pin
};

const uint8_t blank[8] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};

uint8_t number_data[20][10] =
{
	{
		0b01110000,	//0
		0b10001000,
		0b10011000,
		0b10101000,
		0b11001000,
		0b10001000,
		0b01110000,
	    0b00000000
	},
	{
		0b01000000,	//1
		0b11000000,
		0b01000000,
		0b01000000,
		0b01000000,
		0b01000000,
		0b11100000,
	},
	{
		0b01110000,	//2
		0b10001000,
		0b00001000,
		0b00010000,
		0b00100000,
		0b01000000,
		0b11111000,
	},
	{
		0b11111000,	//3
	    0b00010000,
		0b00100000,
		0b00010000,
		0b00001000,
		0b10001000,
		0b01110000,
	},
	{
		0b00010000,	//4
		0b00110000,
		0b01010000,
		0b10010000,
		0b11111000,
		0b00010000,
		0b00010000,
	},
	{
		0b11111000,	//5
		0b10000000,
		0b11110000,
		0b00001000,
		0b00001000,
		0b10001000,
		0b01110000,
	}
};

uint8_t elev[3][10] =
{
	{
		0b00011000, // 위쪽 화살표
		0b00011000,
		0b00111100,
		0b01111110,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000
	},
	{
		0b00011000, // 아래쪽 화살표
		0b00011000,
		0b00011000,
		0b00011000,
		0b01111110,
		0b00111100,
		0b00011000,
		0b00011000
	}
};

unsigned char display_data[8]; 	// 최종 8x8 출력할 데이터
unsigned char scroll_buffer[50][8] = {0};	// 스크롤 할 데이터가 들어 있는 버퍼

int number_of_character = 2;	// 출력할 문자 개수  1장 :arraw 2장 : 층수

// orginal int number_of_character = 11;	// 출력할 문자 개수

// 초기화 작업
// 1. display_data에 number_data[0]에 있는 내용 복사
// 2. number_data를 scroll_buffer에 복사
// 3. dotmatrix의 led를 off
#if 1
int cur_floor = 1;

void init_dotmatrix(int floor)
{
	int arrow;		// 0 : upper arrow, 1: lower arrow

	if (cur_floor < floor) arrow = 0;
	else arrow = 1;

	cur_floor = floor;

//	for (int i = 0; i < 8; i++)
//	{
//		display_data[i] = elev[arrow][i];
//	}

	for (int j = 0; j < 8; j++)
	{
		scroll_buffer[0][j] = elev[arrow][j];
		scroll_buffer[1][j] = number_data[floor][j];
	}

	for (int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(col_port[i], col_pin[i], 1);	// led all off
	}
    TIM10_dotmatrix=0;
}

#else  // orginal
void init_dotmatrix(void)
{
	for (int i = 0; i < 8; i++)
	{
		display_data[i] = number_data[i];
	}
	for (int i = 1; i < number_of_character + 1; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			scroll_buffer[i][j] = number_data[i-1][j];
		}
	}
	for (int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(col_port[i], col_pin[i], 1);	// led all off
	}
}
#endif
void write_column_data(int col)
{
	for (int i = 0; i < 8; i++)
	{
		if (i == col)
			HAL_GPIO_WritePin(col_port[i], col_pin[i], 0); // on
		else HAL_GPIO_WritePin(col_port[i], col_pin[i], 1); // off
	}
}

// 0b00111100
void write_row_data(unsigned char data)
{
	unsigned char d;	// 오리지널 데이터 변형을 막기 위해

	d = data;
	for (int i = 0; i < 8; i++)
	{
		if (d & (1 << i))	// 1인 경우
			HAL_GPIO_WritePin(row_port[i], row_pin[i], 1);
		else HAL_GPIO_WritePin(row_port[i], row_pin[i], 0);
	}
}

#if 1
// scroll 문자 출력 프로그램
int dotmatrix_main_test(void)
{
	static int count = 0; 	// static으로 지역변수를 전역변수처럼 사용할 수 있음
	static int index = 0;	// column count, scroll_buffer의 2차원 index 값

	// 처음 시작 시 past_time = 0;
	if (TIM10_dotmatrix >= 50)	// 500ms scroll
	{
		TIM10_dotmatrix=0;

		for (int i = 0; i < 8; i++)
		{
			display_data[i] = (scroll_buffer[index][i] >> count) | (scroll_buffer[index + 1][i] << 8 - count);
		}
		if (++count == 8)	// 8칼럼을 다 처리 했으면 다음 스크롤 버퍼로 이동
		{
			count = 0;
			index++;	// 다음 스크롤 버퍼로 이동
			if (index == number_of_character + 1) index = 0;
			// 11개의 문자를 다 처리했으면 0번 scroll_buffer를 처리하기 위해 이동한다
		}
	}
	for (int i = 0; i < 8; i++)
	{
		// 공통 양극 방식 (common anode)
		// column에는 0을, row에는 1을 출력해야 해당 LED가 켜진다
		write_column_data(i);
		write_row_data(display_data[i]);

		HAL_Delay(1);
	}
}
#else // orginal
// scroll 문자 출력 프로그램
int dotmatrix_main_test(void)
{
	static int count = 0; 	// static으로 지역변수를 전역변수처럼 사용할 수 있음
	static int index = 0;	// column count, scroll_buffer의 2차원 index 값
	static uint32_t past_time = 0;	// 이전 tick값 저장

	uint32_t now = HAL_GetTick();	// 1ms
	// 처음 시작 시 past_time = 0;
	if (now - past_time >= 500)	// 500ms scroll
	{
		past_time = now;
		for (int i = 0; i < 8; i++)
		{
			display_data[i] = (scroll_buffer[index][i] >> count) | (scroll_buffer[index + 1][i] << 8 - count);
		}
		if (++count == 8)	// 8칼럼을 다 처리 했으면 다음 스크롤 버퍼로 이동
		{
			count = 0;
			index++;	// 다음 스크롤 버퍼로 이동
			if (index == number_of_character + 1) index = 0;
			// 11개의 문자를 다 처리했으면 0번 scroll_buffer를 처리하기 위해 이동한다
		}
	}
	for (int i = 0; i < 8; i++)
	{
		// 공통 양극 방식 (common anode)
		// column에는 0을, row에는 1을 출력해야 해당 LED가 켜진다
		write_column_data(i);
		write_row_data(display_data[i]);

		HAL_Delay(1);
	}

	return 0;
}
#endif
