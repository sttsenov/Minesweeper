#include "stdafx.h"

#include "MainWindow.h"

 int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	 // Main Window Dimensions
	 MainWindow mainWindow;

	 int mainWindowWidth = (BUTTON_WIDTH * N_COLUMNS) + 16; // I wish I knew where that 16 came from...
	 int mainWindowHeight = OFFSET_Y + (BUTTON_HEIGHT * N_ROWS) + 39; // Again...wtf
	 
	 DWORD mainWindowStyle = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME | WS_CLIPCHILDREN;
	 Dimensions mainWindowD{ CW_USEDEFAULT, CW_USEDEFAULT, mainWindowWidth, mainWindowHeight };

	 // Reset Button Dimensions
	 int offsetY = static_cast<int>(OFFSET_Y);
	 int resetButtonX = (mainWindowWidth / 2) - (BUTTON_WIDTH * 3);
	 int resetButtonY = offsetY - (BUTTON_HEIGHT + (BUTTON_HEIGHT / 2));
	 
	 Dimensions resetButtonDimensions{ resetButtonX, resetButtonY, BUTTON_WIDTH * 3, BUTTON_HEIGHT };

	 // Cell/Game Button Dimensions
	 Dimensions dimensions{ 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT };
	 Dimensions topPanelDimensions{ 0, 0, mainWindowWidth, mainWindowHeight };

	 // Create the main window
	 if (!mainWindow.Create(L"MineSweeper", mainWindowStyle, 0, mainWindowD))
	 {
		 return -1;
	 }

	 // Add top panel section 
	 mainWindow.SetSectionWidth(mainWindowWidth);
	 mainWindow.SetSectionHeight(offsetY);

	 // Add reset button
	 HWND hResetButton;
	 if (!mainWindow.CreateButton(L"button", L"Reset", hResetButton, (HMENU)RESET_BUTTON_ID, resetButtonDimensions))
	 {
		 return -1;
	 }

	 // ----------------------------------------

	 // Q: There is no reason why I should have a vector and not a 2d array, right?
	 // A: There isn't but I can't be bothered to change all of this, especially since it works

	 // Populate blocks:
	 // If the block is set to "true" we expect a mine
	 int8_t plantMine{ 0 };
	 int counter{ 0 };

	 Helpers::Shuffle<bool[GRID_SIZE]>(mainWindow.mineBlocks);
	 
	 // ----------------------------------------

	 // Create a grid
	 for (unsigned rows = 0; rows < N_ROWS; ++rows)
	 {
		 dimensions.y = rows * dimensions.height + OFFSET_Y;

		 for (unsigned columns = 0; columns < N_COLUMNS; ++columns)
		 {
			 dimensions.x = columns * dimensions.width;

			 // Check if we expect block to be a mine
			 plantMine = mainWindow.mineBlocks[counter] ? -1 : 0;

			 // Populate the bombs first
			 if (!mainWindow.CreateGameButton(L"button", L"", dimensions, plantMine))
			 {
				 return -1;
			 }

			 counter++;
		 }
	 }

	 // Go through the buttons and create numbers
	 mainWindow.CalculateDistance();

	 // Redraw the whole thing
	ShowWindow(mainWindow.Window(), nCmdShow);

	// Run the message loop.
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}