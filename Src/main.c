/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <time.h>
#include "lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int board[20][10] = { 0 };
int square[2][4] = { 0 };
int nextSquare[2][4] = { 0 };

int cell_size = 15;

int score = 0;
int level = 1;
int delaytime = 750;
int id[3] = { -1 }; // initial ids
uint16_t colortable[10] = { WHITE, WHITE, LIGHTBLUE, GREEN, BLUE, YELLOW, CYAN,
GRAY, BRRED, BLACK };

int cp_row = 0; // central point row
int cp_column = 0; // central point column
int cp_list[2][2] = { 0 };
int last_position[4][2] = { 0 };
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void DrawElements() { // Basic elements in the board
	LCD_Clear(WHITE);
	POINT_COLOR = BLACK;
	LCD_DrawRectangle(5, 2, 155, 305);
	LCD_DrawRectangle(162, 2, 228, 62);
	LCD_DrawRectangle(162, 67, 228, 132);
	LCD_ShowString(165, 140, 100, 12, 12, (uint8_t*) "Score:");
	LCD_ShowxNum(205, 140, score, 4, 12, 0);
	LCD_ShowString(165, 160, 100, 12, 12, (uint8_t*) "Level:");
	LCD_ShowxNum(205, 160, level, 4, 12, 0);
}

void CreateSquare() { // Generate random new born square
	int index = rand() % 8 + 2;
	switch (index) {
	case 2:
		nextSquare[0][0] = 1, nextSquare[0][1] = 1;
		nextSquare[0][2] = 1, nextSquare[0][3] = 1;
		nextSquare[1][0] = 0, nextSquare[1][1] = 0;
		nextSquare[1][2] = 0, nextSquare[1][3] = 0;
		cp_list[1][0] = 0;
		cp_list[1][1] = 4;
		break;
	case 3:
		nextSquare[0][0] = 0, nextSquare[0][1] = 1;
		nextSquare[0][2] = 1, nextSquare[0][3] = 0;
		nextSquare[1][0] = 0, nextSquare[1][1] = 1;
		nextSquare[1][2] = 1, nextSquare[1][3] = 0;
		cp_list[1][0] = 0;
		cp_list[1][1] = 4;
		break;
	case 4:
		nextSquare[0][0] = 0, nextSquare[0][1] = 1;
		nextSquare[0][2] = 0, nextSquare[0][3] = 0;
		nextSquare[1][0] = 1, nextSquare[1][1] = 1;
		nextSquare[1][2] = 1, nextSquare[1][3] = 0;
		cp_list[1][0] = 0;
		cp_list[1][1] = 3;
		break;
	case 5:
		nextSquare[0][0] = 0, nextSquare[0][1] = 0;
		nextSquare[0][2] = 1, nextSquare[0][3] = 0;
		nextSquare[1][0] = 1, nextSquare[1][1] = 1;
		nextSquare[1][2] = 1, nextSquare[1][3] = 0;
		cp_list[1][0] = 0;
		cp_list[1][1] = 3;
		break;
	case 6:
		nextSquare[0][0] = 1, nextSquare[0][1] = 0;
		nextSquare[0][2] = 0, nextSquare[0][3] = 0;
		nextSquare[1][0] = 1, nextSquare[1][1] = 1;
		nextSquare[1][2] = 1, nextSquare[1][3] = 0;
		cp_list[1][0] = 0;
		cp_list[1][1] = 3;
		break;
	case 7:
		nextSquare[0][0] = 0, nextSquare[0][1] = 1;
		nextSquare[0][2] = 1, nextSquare[0][3] = 0;
		nextSquare[1][0] = 1, nextSquare[1][1] = 1;
		nextSquare[1][2] = 0, nextSquare[1][3] = 0;
		cp_list[1][0] = 0;
		cp_list[1][1] = 3;
		break;
	case 8:
		nextSquare[0][0] = 1, nextSquare[0][1] = 1;
		nextSquare[0][2] = 0, nextSquare[0][3] = 0;
		nextSquare[1][0] = 0, nextSquare[1][1] = 1;
		nextSquare[1][2] = 1, nextSquare[1][3] = 0;
		cp_list[1][0] = 0;
		cp_list[1][1] = 3;
		break;
	case 9:
		nextSquare[0][0] = 0, nextSquare[0][1] = 1;
		nextSquare[0][2] = 1, nextSquare[0][3] = 0;
		nextSquare[1][0] = 0, nextSquare[1][1] = 1;
		nextSquare[1][2] = 1, nextSquare[1][3] = 0;
		cp_list[1][0] = 0;
		cp_list[1][1] = 4;
		break;
	}
	id[2] = index;
}

void Convert() { // Convert id and cp_list
	id[0] = id[1];
	cp_row = cp_list[0][0];
	cp_column = cp_list[0][1];
	id[1] = id[2];
	cp_list[0][0] = cp_list[1][0];
	cp_list[0][1] = cp_list[1][1];
}

void BringNexttoBoard() {  // Bring the next block to the board
	int row = 0;
	int column = 0;
	for (row = 0; row < 2; row++) {
		for (column = 0; column < 4; column++) {
			board[row][column + 3] = square[row][column];
		}
	}
}

void DrawNewBornBlock() { // Draw the new born block
	int row = 0;
	int col = 0;
	for (row = 0; row < 2; row++) {
		for (col = 0; col < 4; col++) {
			int x1 = col * cell_size + 165;
			int y1 = row * cell_size + 85;
			LCD_Fill(x1, y1, x1 + cell_size, y1 + cell_size, WHITE);
		}
	}
	for (row = 0; row < 2; row++) {
		for (col = 0; col < 4; col++) {
			if (nextSquare[row][col] == 1) {
				int x1 = col * cell_size + 165;
				int y1 = row * cell_size + 85;
				LCD_Fill(x1, y1, x1 + cell_size, y1 + cell_size,
						colortable[id[2]]);
				LCD_Fill(x1, y1, x1 + cell_size, y1 + 1, BLACK);
				LCD_Fill(x1, y1, x1 + 1, y1 + cell_size, BLACK);
				LCD_Fill(x1 + cell_size - 1, y1, x1 + cell_size, y1 + cell_size,
								BLACK);
				LCD_Fill(x1, y1 + cell_size - 1, x1 + cell_size, y1 + cell_size,
								BLACK);
			}
		}
	}
	if (id[2] == 9) { // TNT
		LCD_Fill(188, 93, 202, 98, RED);
		LCD_Fill(193, 98, 197, 110, RED);
	}
}

void DrawNextBlock() {  // Draw the next block
	int row = 0;
	int col = 0;
	for (row = 0; row < 2; row++) {
		for (col = 0; col < 4; col++) {
			square[row][col] = nextSquare[row][col];
		}
	}
	for (row = 0; row < 2; row++) {
		for (col = 0; col < 4; col++) {
			int x1 = col * cell_size + 165;
			int y1 = row * cell_size + 20;
			LCD_Fill(x1, y1, x1 + cell_size, y1 + cell_size, WHITE);
		}
	}
	for (row = 0; row < 2; row++) {
		for (col = 0; col < 4; col++) {
			if (square[row][col] == 1) {
				int x1 = col * cell_size + 165;
				int y1 = row * cell_size + 20;
				LCD_Fill(x1, y1, x1 + cell_size, y1 + cell_size,
						colortable[id[1]]);
				LCD_Fill(x1, y1, x1 + cell_size, y1 + 1, BLACK);
				LCD_Fill(x1, y1, x1 + 1, y1 + cell_size, BLACK);
				LCD_Fill(x1 + cell_size - 1, y1, x1 + cell_size, y1 + cell_size,
								BLACK);
				LCD_Fill(x1, y1 + cell_size - 1, x1 + cell_size, y1 + cell_size,
								BLACK);
			}
		}
	}
	if (id[1] == 9) {
		LCD_Fill(188, 28, 202, 33, RED);
		LCD_Fill(193, 33, 197, 45, RED);
	}
}

void DrawBlocks() { // Draw all the blocks on the board
	int xtnt = 0, ytnt = 0;
	int row = 0, col = 0;
	int num = 0, count = 0;
	for (num = 0; num < 4; num++) {
		int x1 = last_position[num][1] * cell_size + 5;
		int y1 = last_position[num][0] * cell_size + 5;
		LCD_Fill(x1, y1, x1 + cell_size, y1 + cell_size, WHITE);
	}
	for (row = 0; row < 20; row++) {
		for (col = 0; col < 10; col++) {
			if (board[row][col] == 1) {
				// living block
				int x1 = col * cell_size + 5;
				int y1 = row * cell_size + 5;
				xtnt = x1;
				ytnt = y1;
				LCD_Fill(x1, y1, x1 + cell_size, y1 + cell_size,
						colortable[id[0]]);
				last_position[count][1] = col;
				last_position[count][0] = row;
				count++;
				LCD_Fill(x1, y1, x1 + cell_size, y1 + 1, BLACK);
				LCD_Fill(x1, y1, x1 + 1, y1 + cell_size, BLACK);
				LCD_Fill(x1 + cell_size - 1, y1, x1 + cell_size, y1 + cell_size,
				BLACK);
				LCD_Fill(x1, y1 + cell_size - 1, x1 + cell_size, y1 + cell_size,
				BLACK);
			} else if (board[row][col] > 1) {
				// dead block
				int x1 = col * cell_size + 5;
				int y1 = row * cell_size + 5;
				LCD_Fill(x1, y1, x1 + cell_size, y1 + cell_size,
						colortable[board[row][col]]);
				LCD_Fill(x1, y1, x1 + cell_size, y1 + 1, BLACK);
				LCD_Fill(x1, y1, x1 + 1, y1 + cell_size, BLACK);
				LCD_Fill(x1 + cell_size - 1, y1, x1 + cell_size, y1 + cell_size,
				BLACK);
				LCD_Fill(x1, y1 + cell_size - 1, x1 + cell_size, y1 + cell_size,
				BLACK);
			}
		}
	}
	if (id[0] == 9) {
		xtnt = xtnt - 15;
		ytnt = ytnt - 15;
		LCD_Fill(xtnt + 8, ytnt + 8, xtnt + 22, ytnt + 13, RED);
		LCD_Fill(xtnt + 13, ytnt + 13, xtnt + 17, ytnt + 25, RED);
	}
}

void Initial() { // Initialize blocks at the beginning
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
			board[i][j] = 0;
		}
	}
	for (int i = 0; i < 3; i++) {
		BringNexttoBoard();
		Convert();
		DrawBlocks();
		DrawNextBlock();
		CreateSquare();
		DrawNewBornBlock();
	}
}

int CanSqureDown() { // Decide whether land
	for (int i = 0; i < 10; i++) {
		if (board[19][i] == 1) {
			return 0;
		}
	}
	for (int i = 18; i >= 0; i--) {
		for (int j = 0; j < 10; j++) {
			if (board[i][j] == 1 && board[i + 1][j] >= 2) {
				return 0;
			}
		}
	}
	return 1;
}

void SqureDown() { // Land one cell
	for (int i = 18; i >= 0; i--) {
		for (int j = 0; j < 10; j++) {
			if (board[i][j] == 1) {
				board[i + 1][j] = board[i][j];
				board[i][j] = 0;
			}
		}
	}
	cp_row++;
}

void TurnDeath() { // Landing block turn death
	int i = 0, j = 0;
	for (i = 0; i < 20; i++) {
		for (j = 0; j < 10; j++) {
			if (board[i][j] == 1) {
				board[i][j] = id[0];
			}
		}
	}
}

int CanShiftRight() { // Decide whether can shift right
	int i = 0, j = 0;
	for (i = 0; i < 20; i++) {
		if (board[i][9] == 1) {
			return 0;
		}
	}
	for (i = 0; i < 20; i++) {
		for (j = 9; j >= 0; j--) {
			if (board[i][j] == 1 && board[i][j + 1] > 1) {
				return 0;
			}
		}
	}
	return 1;
}

void ShiftRight() {
	if (CanShiftRight()) {
		int i = 0, j = 0;
		for (i = 0; i < 20; i++) {
			for (j = 9; j >= 0; j--) {
				if (board[i][j] == 1) {
					board[i][j + 1] = board[i][j];
					board[i][j] = 0;
				}
			}
		}
		cp_column++;
	}
}

int CanShiftLeft() { // Decide whether can shift left
	int i = 0, j = 0;
	for (i = 0; i < 20; i++) {
		if (board[i][0] == 1) {
			return 0;
		}
	}
	for (i = 0; i < 20; i++) {
		for (j = 0; j < 10; j++) {
			if (board[i][j] == 1 && board[i][j - 1] > 1) {
				return 0;
			}
		}
	}
	return 1;
}

void ShiftLeft() {
	if (CanShiftLeft()) {
		int i = 0, j = 0;
		for (i = 0; i < 20; i++) {
			for (j = 0; j < 10; j++) {
				if (board[i][j] == 1) {
					board[i][j - 1] = board[i][j];
					board[i][j] = 0;
				}
			}
		}
		cp_column--;
	}
}

int NormalShapeVerify() {
	if (id[0] == 3)
		return 1;
	if(id[0]==9)
		return 0;
	else {
		int i = 0, j = 0;
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				if (board[i][j] > 1)
					return 0;
			}
		}
	}
	if (cp_column < 0)
		cp_column = 0;
	else if (cp_column > 7)
		cp_column = 7;

	return 1;

}

void NormalShapChange() {
	if (id[0] == 3 ||id[0]==9)
		return;
	else {
		int i = 0, j = 0;
		int tempAry[3][3] = { 0 };
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				if (board[cp_row + i][cp_column + j] == 1)
					tempAry[i][j] = board[cp_row + i][cp_column + j];
			}
		}
		int temp2Ary[3][3] = { 0 };
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				temp2Ary[i][j] = tempAry[2 - j][i];
			}
		}
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				if (temp2Ary[i][j] == 1 && board[cp_row + i][cp_column + j] > 1)
					return;
			}
		}
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				if (board[cp_row + i][cp_column + j] == 1)
					board[cp_row + i][cp_column + j] = 0;
			}
		}
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				if (temp2Ary[i][j] == 1)
					board[cp_row + i][cp_column + j] = 1;
			}
		}

	}

}

int LongShapeVerify() {
	if (cp_row == 0 || cp_column == 0 || cp_column >= 8)
		return 0;
	int i = 0, j = 0;
	for (i = 1; i < 4; i++) { // |==>--, right
		if (board[cp_row][cp_column + i] > 1 || cp_column + i > 9)
			break;
	}
	for (j = 1; j < 4; j++) { // --==>|, left
		if (board[cp_row][cp_column - j] > 1 || cp_column - j < 0)
			break;
	}
	if (i + j < 5)
		return 0;
	return 1;
}

void LongShapeChange() {
	if (board[cp_row][cp_column - 1] == 1) { // -- ==>|
		if (cp_row + 2 > 19)
			return;
		board[cp_row][cp_column - 1] = 0;
		board[cp_row][cp_column + 1] = 0;
		board[cp_row][cp_column + 2] = 0;

		board[cp_row - 1][cp_column] = 1;
		board[cp_row + 1][cp_column] = 1;
		board[cp_row + 2][cp_column] = 1;

	} else { // | ==> --
		if (!(board[cp_row][cp_column - 1] + board[cp_row][cp_column + 1]
				+ board[cp_row][cp_column + 2] >= 6)) {
			if (cp_column + 2 > 9)
				return;
			board[cp_row - 1][cp_column] = 0;
			board[cp_row + 1][cp_column] = 0;
			board[cp_row + 2][cp_column] = 0;

			board[cp_row][cp_column - 1] = 1;
			board[cp_row][cp_column + 1] = 1;
			board[cp_row][cp_column + 2] = 1;
		}
	}
}

void ChangeVerify() {
	switch (id[0]) {
	case 2:
		if (LongShapeVerify())
			LongShapeChange();
		break;
	default:
		if (NormalShapeVerify())
			NormalShapChange();
		break;
	}
	DrawBlocks();
	LCD_DrawRectangle(5, 2, 155, 305);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch (GPIO_Pin) {
	case KEY0_Pin:
		ShiftRight();
		HAL_Delay(75);
		break;
	case KEY1_Pin:
		ShiftLeft();
		HAL_Delay(75);
		break;
	case KEY_WK_Pin:
		HAL_Delay(125);
		ChangeVerify();
		HAL_Delay(125);
		break;
	default:
		break;
	}

}

void ChangeSpeed() {
	if (score >= 200 && score < 400) {
		delaytime = 500;
		level = 2;
	}
	if (score >= 400) {
		delaytime = 300;
		level = 3;
	}
}

void EliminateLine() {
	int i = 0, j = 0, k = 0;
	int flag = 1;
	int cflag = 0;
	for (i = 19; i >= 0; i--) {
		for (j = 0; j < 10; j++) {
			if (board[i][j] <= 1) {
				flag = 0;
				break;
			}
		}
		if (flag) {
			for (k = i - 1; k >= 0; k--) {
				for (j = 0; j < 10; j++) {
					board[k + 1][j] = board[k][j];
				}
			}
			cflag = 1;
			score += 100;
			ChangeSpeed();
			LCD_ShowxNum(205, 140, score, 4, 12, 0);
			LCD_ShowxNum(205, 160, level, 4, 12, 0);
			i++;
		}
		flag = 1;
	}
	if (cflag) {
		int row = 0, col = 0;
		for (row = 0; row < 20; row++) {
			for (col = 0; col < 10; col++) {
				int x1 = col * cell_size + 5;
				int y1 = row * cell_size + 5;
				LCD_Fill(x1, y1, x1 + cell_size, y1 + cell_size,
						colortable[board[row][col]]);
			}
		}
	}
}

int isGameOver() {
	int i = 0;
	for (i = 0; i < 10; i++) {
		if (board[0][i] >= 2) {
			return 1;
		}
	}
	return 0;
}

void Bomb() {
	int i = 0, j = 0;
	for (i = cp_row - 1; i <= cp_row + 2; i++) {
		for (j = cp_column - 1; j <= cp_column + 2; j++) {
			if (i >= 0 && i < 20 && j >= 0 && j < 10) {
				if (board[i][j] >= 2) {
					score += 20;
				}
				board[i][j] = 0;
				int x = j * cell_size + 5;
				int y = i * cell_size + 5;
				LCD_Fill(x, y, x + cell_size, y + cell_size, RED);
			}
		}
	}
	HAL_Delay(500);
	ChangeSpeed();
	LCD_ShowxNum(205, 140, score, 4, 12, 0);
	LCD_ShowxNum(205, 160, level, 4, 12, 0);
	for (i = cp_row - 1; i <= cp_row + 2; i++) {
		for (j = cp_column - 1; j <= cp_column + 2; j++) {
			if (i >= 0 && i < 20 && j >= 0 && j < 10) {
				int x = j * cell_size + 5;
				int y = i * cell_size + 5;
				LCD_Fill(x, y, x + cell_size, y + cell_size, WHITE);
			}
		}
	}
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */
	LCD_Init();
	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_ADC1_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
// Start screen
	LCD_Clear(BLUE);
	POINT_COLOR = RED;
	LCD_ShowString(10, 65, 200, 24, 24, (uint8_t*) "Welcome to TETRIS");
	LCD_ShowString(10, 125, 220, 16, 16, (uint8_t*) "Game starts in 3 seconds");
	HAL_Delay(3000);

	uint16_t iSeed = HAL_ADC_GetValue(&hadc1);
	srand(iSeed); // random seed
//Start Game
	DrawElements();
	Initial();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */
		/* USER CODE BEGIN 3 */
		if (CanSqureDown()) {
			SqureDown();
			DrawBlocks();
		} else {
			if (id[0] == 9) {
				Bomb();
			} else {
				TurnDeath();
				EliminateLine();
				if (isGameOver()) {
					LCD_Clear(BLUE);
					LCD_ShowString(10, 65, 200, 24, 24, (uint8_t*) "YOU DEAD");
					break;
				}
			}
			BringNexttoBoard();
			Convert();
			DrawBlocks();
			DrawNextBlock();
			CreateSquare();
			DrawNewBornBlock();
		}
		LCD_DrawRectangle(5, 2, 155, 305);
		HAL_Delay(delaytime);
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
