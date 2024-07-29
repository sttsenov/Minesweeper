#pragma once

// ---------------------------------------
// Common includes
// ---------------------------------------
#include <vector>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>

// ---------------------------------------
// Windows Includes
// ---------------------------------------
#include "windows.h"
#include "windowsx.h"
#include "winuser.h"
#include <Commctrl.h>

// ---------------------------------------
// Background Colour Macros
// ---------------------------------------
#define BKG_GRAY(hDC)			SetBkColor(hDC, RGB(192, 192, 192));
#define BKG_LIGHT_GRAY(hDC)		SetBkColor(hDC, RGB(210, 210, 210));

// ---------------------------------------
// Text Colour Macros
// ---------------------------------------
#define TEXT_BLACK(hDC)			SetTextColor(hDC, RGB(0, 0, 0));

#define TEXT_RED(hDC)			SetTextColor(hDC, RGB(255, 0, 0));
#define TEXT_BLUE(hDC)			SetTextColor(hDC, RGB(0, 0, 255));
#define TEXT_GREEN(hDC)			SetTextColor(hDC, RGB(0, 255, 0));

#define TEXT_DARK_RED(hDC)		SetTextColor(hDC, RGB(48, 0, 0));
#define TEXT_DARK_BLUE(hDC)		SetTextColor(hDC, RGB(0, 0, 48));
#define TEXT_DARK_GREEN(hDC)	SetTextColor(hDC, RGB(0, 96, 0));

#define TEXT_CYAN(hDC)			SetTextColor(hDC, RGB(0, 48, 48));

// ---------------------------------------
// Constants Macros
// ---------------------------------------
int constexpr N_ROWS = 16;
int constexpr N_COLUMNS = 30;

int const GRID_SIZE = N_ROWS * N_COLUMNS;

long const BUTTON_WIDTH = 30;
long const BUTTON_HEIGHT = 30;

unsigned const OFFSET_X = 50;
unsigned const OFFSET_Y = 50;

unsigned const N_MINES = 99;

int const RESET_BUTTON_ID = 420;