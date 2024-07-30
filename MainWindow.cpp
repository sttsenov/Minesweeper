#include "stdafx.h"

#include "MainWindow.h"

using Direction = Helpers::Directions::Direction;

bool MainWindow::_isRunning = false;
int MainWindow::_currentButtonIndex = 0;

unsigned MainWindow::_iTimer = 0;
unsigned MainWindow::_iCounter = N_MINES;

int MainWindow::_iMineFlaggedCounter = N_MINES;

HBITMAP MainWindow::_flagBitmap = NULL;
HBITMAP MainWindow::_mineBitmap = NULL;

std::wstring MainWindow::_timerText = std::to_wstring(_iTimer);
std::wstring MainWindow::_counterText = std::to_wstring(_iCounter);

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (_iMineFlaggedCounter == 0)
    {
        _isRunning = false;
        _iMineFlaggedCounter = N_MINES;
        
        MessageBox(m_hwnd, L"Congrats!", L"You won!", MB_OK);
    }

    switch (uMsg)
    {
    case WM_CREATE:
    {
        _flagBitmap = (HBITMAP)LoadImage(NULL, L"assets\\flag.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        _mineBitmap = (HBITMAP)LoadImage(NULL, L"assets\\mine.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_TIMER:
    {
        if (_isRunning)
        {
            UpdateTimer();
            InvalidateRect(m_hwnd, NULL, FALSE);
        }
    }
    break;

    case WM_COMMAND:
    {
        if (LOWORD(wParam) == RESET_BUTTON_ID)
        {
            ResetGame();

            return TRUE;
        }

        HWND hButton = (HWND)lParam;
        Button* button = FindButton(hButton);

        if (button == NULL)
            return TRUE;
        
        if (!_isRunning)
            _isRunning = true;

        ExpandNeighbours(button);
    }
    break;

    case WM_LBUTTONDOWN:
    {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        if (yPos < OFFSET_Y)
        {
            return FALSE;
        }

        Button* button = FindButton(xPos, yPos);

        if (button == NULL || !_isRunning)
            return TRUE;

        ExpandNeighbours(button, _currentButtonIndex, true);
    }
    break;

    case WM_RBUTTONDOWN:
    {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        if (yPos < OFFSET_Y)
        {
            return FALSE;
        }

        HandleFlag(xPos, yPos);
    }
    break;

    case WM_PARENTNOTIFY:
    {
        if (wParam == WM_RBUTTONDOWN)
        {
            // The absolute beauty of this is that there's no need
            // to check if we are in the grid, since this only gets called
            // by children.
            // NOTE: If children of other types are added. This will probably break :D 
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);

            HandleFlag(xPos, yPos);
        }
    }
    break;

    case WM_DRAWITEM:
    {
        std::wstring buttonText;
        DWORD buttonTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER;

        LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)(lParam);
        
        HDC hDC = pDIS->hDC;
        RECT rc = pDIS->rcItem;
        
        HBRUSH bg = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
        HPEN pn = (HPEN)GetStockObject(BLACK_PEN);
        
        SelectObject(hDC, bg);
        SelectObject(hDC, pn);

        // Draw the text of the "Reset" button
        if (LOWORD(wParam) == RESET_BUTTON_ID)
        {
            SetBkColor(hDC, RGB(192, 192, 192));
            Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);

            buttonText = L"Reset";
            DrawText(hDC, buttonText.c_str(), (int)buttonText.size(), &rc, buttonTextStyle);
            
            return TRUE;
        }

        Button* button = FindButton(pDIS->hwndItem);

        // Check if we should have a different background color
        if (button != NULL && _isRunning && 
            button->isRevealed)
        {
            // Check if we are an empty or numbered expanded cell/game button
            if (!button->hasFlag && !button->IsMine())
            {
                BKG_LIGHT_GRAY(hDC); // Sets the text background color
                SetDCBrushColor(hDC, RGB(210, 210, 210));   // Sets the buttons color
                
                bg = (HBRUSH)GetStockObject(DC_BRUSH);
                SelectObject(hDC, bg);

                Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
                
                if (button->number == 0)
                {
                    return TRUE;
                }
            }
        }
        else
        {
            BKG_GRAY(hDC);
            Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);

            // Exit early if the button cannot be found
            if (button == NULL)
            {
                return FALSE;
            }

            // Don't pain the text if the button hasn't been flagged and isn't revealed yet
            if (!button->hasFlag && !button->isRevealed)
            {
                return TRUE;
            }
        }

        if (button->hasFlag)
        {
            // Draw the flag bitmap
            Helpers::DrawImage(hDC, rc, _flagBitmap);
        }
        else if (button->IsMine())
        {
            // Draw the mine bitmap
            Helpers::DrawImage(hDC, rc, _mineBitmap);
        }
        else
        {
            if (button->number == 0)
                return TRUE;

            buttonText = std::to_wstring(button->number);
            
            Helpers::SetNumberText(hDC, button->number);

            DrawText(hDC, buttonText.c_str(), (int)buttonText.size(), &rc, buttonTextStyle);
        }

        return TRUE;
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hwnd, &ps);

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        _counterText = std::to_wstring(_iCounter);
        _timerText = std::to_wstring(_iTimer);

        TextOut(hdc, OFFSET_X / 2, _sectionHeight / 4, _counterText.c_str(), (int)_counterText.size());
        TextOut(hdc, _sectionWidth - OFFSET_X, _sectionHeight / 4, _timerText.c_str(), (int)_timerText.size());
        
        EndPaint(m_hwnd, &ps);
    }
    break;

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}

Button* MainWindow::FindButton(HWND& hButton)
{
    for (int i = 0; i < buttons.size(); ++i)
    {
        if (buttons.at(i).handle == hButton)
        {
            _currentButtonIndex = i;
            return &buttons.at(i);
        }
    }

    return NULL;
}

Button* MainWindow::FindButton(int& xPos, int& yPos)
{
    long trackerX = 0;
    long trackerY = _sectionHeight;

    for (int i = 0; i < buttons.size(); ++i)
    {
        bool inBoundsX = trackerX <= xPos && xPos <= trackerX + BUTTON_WIDTH;
        bool inBoundsY = trackerY <= yPos && yPos <= trackerY + BUTTON_HEIGHT;

        // Check if we are in the button
        if (inBoundsX && inBoundsY)
        {
            _currentButtonIndex = i;
            return &buttons.at(i);
        }

        // Update X
        trackerX += BUTTON_WIDTH;

        // Check if we are on a new row
        if ((i + 1) % N_COLUMNS == 0)
        {
            trackerX = 0;
            trackerY += BUTTON_HEIGHT;
        }
    }

    return NULL;
}

void MainWindow::CalculateDistance()
{
    std::vector<int> cellNeighbours;

    for (int i = 0; i < GRID_SIZE; ++i)
    {
        Button& button = buttons.at(i);

        if (button.IsMine())
        {
            continue;
        }

        // Grab neighbours and check if they are mines
        cellNeighbours = FindGridNeighbours(i);

        for (int& neighbourIndex : cellNeighbours)
        {
            button.number += buttons.at(neighbourIndex).IsMine();
        }
    }
}

void MainWindow::ExpandNeighbours(Button* button , int& index /*  = _currentButtonIndex */, bool expandNumbers /* = false */)
{
    if (button == NULL)
        return;

    button->isRevealed = true;
    
    if (button->IsMine())
    {
        // Invalidate all cells/game buttons
        for (int i = 0; i < GRID_SIZE; ++i)
        {
            Button& buttonRef = buttons.at(i);
            EnableWindow(buttonRef.handle, FALSE);

            // Reveal all of the mines
            if (buttonRef.IsMine())
            {
                buttonRef.isRevealed = true;
            }

            InvalidateRect(buttonRef.handle, NULL, TRUE);
        }
        
        // Stop the game
        _isRunning = false;
        
        return;
    }
    else if (button->number == 0 || expandNumbers)
    {
        bool isCellExpanded{ false };
        _expandedCells.push_back(index);
        
        std::vector<int> neighbours = FindGridNeighbours(index);
        int flaggedNeighbours = GetNeighboursFlags(neighbours);

        // Enough number of flags should be places to double expand, 
        // exit early if they are not
        if (expandNumbers && button->number != flaggedNeighbours)
        {
            EnableWindow(button->handle, FALSE);
            InvalidateRect(button->handle, NULL, TRUE);

            return;
        }

        for (int& neighbour : neighbours)
        {
            isCellExpanded = false;

            // If already expanded, then don't bother
            for (int& expandedCell : _expandedCells)
            {
                if (neighbour == expandedCell)
                {
                    isCellExpanded = true;
                    break;
                }
            }

            if (isCellExpanded)
                continue;

            Button& neighbourButton = buttons.at(neighbour);

            // Don't expand button that has a flag
            if (neighbourButton.hasFlag)
                continue;

            ExpandNeighbours(&neighbourButton, neighbour, false);
        }
    }

    _expandedCells.push_back(index);

    EnableWindow(button->handle, FALSE);
    InvalidateRect(button->handle, NULL, TRUE);
}

std::vector<int> MainWindow::FindGridNeighbours(int& index)
{
    std::vector<int> neighbours;
    const Direction directions[] = { Direction::RIGHT, Direction::LEFT, Direction::BOTTOM, Direction::TOP };

    for (Direction direction : directions)
    {
        Helpers::Directions::GetNeighbours(neighbours, index, direction);
    }

    return neighbours;
}

int MainWindow::GetNeighboursFlags(std::vector<int>& neighbours)
{
    int flags{ 0 };

    for (const int& neighbour : neighbours)
    {
        if (buttons.at(neighbour).hasFlag)
        {
            flags++;
        }
    }

    return flags;
}

void MainWindow::HandleFlag(int& xPos, int& yPos)
{
    if (!_isRunning)
    {
        // Game is not running, what are you doing here?
        return;
    }

    Button* button = FindButton(xPos, yPos);

    if (button == NULL || button->isRevealed)
    {
        return;
    }

    // Remove previously set flag
    if (button->hasFlag)
    {
        button->hasFlag = false;

        if (button->IsMine())
        {
            ++_iMineFlaggedCounter;
        }

        IncrementCounter();

        EnableWindow(button->handle, TRUE);
    }
    // Add a flag
    else
    {
        button->hasFlag = true;

        if (button->IsMine())
        {
            --_iMineFlaggedCounter;
        }

        DecrementCounter();

        EnableWindow(button->handle, FALSE);
    }

    InvalidateRect(button->handle, NULL, TRUE);
}

void MainWindow::ResetGame()
{
    // Rest all the variables that are actually imports
    _isRunning = false;
    _currentButtonIndex = 0;

    _iTimer = 0;
    _iCounter = N_MINES;

    _iMineFlaggedCounter = N_MINES;

    _expandedCells.clear();

    int counter{ 0 };
    Helpers::Shuffle<bool[GRID_SIZE]>(mineBlocks);

    // Reset all buttons |  Clear number values and place bombs
    for (int i = 0; i < buttons.size(); ++i)
    {
        Button& button = buttons.at(i);

        button.hasFlag = false;
        button.isRevealed = false;

        button.number = mineBlocks[counter] ? -1 : 0;
        counter++;
    }

    // Re-calculate distance
    CalculateDistance();

    for (Button& button : buttons)
    {
        EnableWindow(button.handle, TRUE);
        InvalidateRect(button.handle, NULL, TRUE);
    }
}