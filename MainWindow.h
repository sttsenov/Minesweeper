#pragma once
#include "BaseWindow.h"

#include <chrono>
#include <random>
#include <algorithm>

class MainWindow : public BaseWindow<MainWindow>
{
public:
	PCWSTR	ClassName() const { return L"Main Window!"; };
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void	CalculateDistance();

	void	SetSectionHeight(int& height) { _sectionHeight = height; }
	void	SetSectionWidth(int& width) { _sectionWidth = width; }

	template<typename T>
	void	Shuffle(T& obj)
	{
		unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
		std::shuffle(std::begin(obj), std::end(obj), std::default_random_engine(seed));
	}

private:
	void DecrementCounter()
	{
		if (_iCounter > 0)
			--_iCounter;
	}

	void IncrementCounter()
	{
		if (_iCounter < N_MINES)
			++_iCounter;
	}

	void UpdateTimer()
	{
		// Update the timer every second
		if (_iTimer < 1000)
			++_iTimer;
	}
	
	Button* FindButton(HWND& hButton, int& index = _currentButtonIndex);
	std::vector<int> FindGridNeighbours(int& index);

	void ExpandNeighbours(Button* button, int& index = _currentButtonIndex);

	void HandleFlag(int& xPos, int& yPos);
	void ResetGame();

	// Keeps some state information
	static bool			_isRunning;
	static int			_currentButtonIndex;

	// Used to display the "flagged mines" to the user
	static unsigned		_iCounter;
	static std::wstring	_counterText;

	// Actually tracks flagged mines
	static int			_iMineFlaggedCounter;

	// Displays the timer
	static unsigned		_iTimer;
	static std::wstring	_timerText;

	// Tracks cells/game buttons that have been clicked
	std::vector<int>	_expandedCells;

	int					_sectionHeight{ 0 };
	int					_sectionWidth{ 0 };

	static HBITMAP		_flagBitmap;
	static HBITMAP		_mineBitmap;
};