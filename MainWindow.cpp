#include "stdafx.h"

#include "MainWindow.h"

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
    // TODO: Use this to implement keyboard button clicks
    //case WM_CHAR:

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
        Button* button = FindButton(hButton, _currentButtonIndex);

        if (button == NULL)
            return TRUE;
        
        if (!_isRunning)
            _isRunning = true;

        ExpandNeighbours(button);
            
        InvalidateRect(m_hwnd, NULL, TRUE);
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
            if (_flagBitmap == NULL)
            {
                return FALSE;
            }

            // Offsets
            RECT rect = rc;
            rect.right -= 5;
            rect.left -= 1;
            rect.top -= 1;
            rect.bottom -= 1;

            HBRUSH brush = CreatePatternBrush(_flagBitmap);
            
            FillRect(hDC, &rect, brush);
            DeleteObject(brush);
        }
        else if (button->IsMine())
        {
            // Draw the mine bitmap
            if (_mineBitmap == NULL)
            {
                return FALSE;
            }

            // Offsets
            RECT rect = rc;
            rect.right -= 5;
            rect.left -= 1;
            rect.top -= 1;
            rect.bottom -= 1;

            HBRUSH brush = CreatePatternBrush(_mineBitmap);

            FillRect(hDC, &rect, brush);
            DeleteObject(brush);
        }
        else
        {
            if (button->number == 0)
                return TRUE;

            buttonText = std::to_wstring(button->number);
            
            switch (button->number)
            {
            case 1:
                TEXT_BLUE(hDC);
                break;
            case 2:
                TEXT_DARK_GREEN(hDC);
                break;
            case 3:
                TEXT_RED(hDC);
                break;
            case 4:
                TEXT_DARK_BLUE(hDC);
                break;
            case 5:
                TEXT_DARK_RED(hDC);
                break;
            case 6:
                TEXT_CYAN(hDC);
                break;
            default:
                TEXT_BLACK(hDC);
                break;
            }

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

Button* MainWindow::FindButton(HWND& hButton, int& index /* = _currentButtonIndex */)
{
    for (int i = 0; i < buttons.size(); ++i)
    {
        if (buttons.at(i).handle == hButton)
        {
            index = i;
            return &buttons.at(i);
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

void MainWindow::ExpandNeighbours(Button* button , int& index /*  = _currentButtonIndex */)
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
    else if (button->number == 0)
    {
        bool isCellExpanded{ false };
        
        _expandedCells.push_back(index);
        std::vector<int> neighbours = FindGridNeighbours(index);

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
            ExpandNeighbours(&neighbourButton, neighbour);
        }
    }

    _expandedCells.push_back(index);
    EnableWindow(button->handle, FALSE);
}

std::vector<int> MainWindow::FindGridNeighbours(int& index)
{
    std::vector<int> neighbours;

    int checkRight  = index + 1;
    int checkLeft   = index - 1;

    int checkBottom = index + N_COLUMNS;
    int checkTop    = index - N_COLUMNS;

    // We can go right and be on the same row
    if (checkRight <= GRID_SIZE && checkRight % N_COLUMNS != 0)
    {
        // RIGHT
        neighbours.push_back(checkRight);
    }

    // We can go left and be on the same row
    if (checkLeft >= 0 && index % N_COLUMNS != 0)
    {
        // LEFT
        neighbours.push_back(checkLeft);
    }

    // There's a row below
    if (checkBottom < GRID_SIZE)
    {
        // BOTTOM
        neighbours.push_back(checkBottom);

        int checkBottomRight    = index + N_COLUMNS + 1;
        int checkBottomLeft     = index + N_COLUMNS - 1;

        if (checkBottomRight % N_COLUMNS != 0)
        {
            // BOTTOM RIGHT
            neighbours.push_back(checkBottomRight);
        }

        if (index % N_COLUMNS != 0)
        {
            // BOTTOM LEFT
            neighbours.push_back(checkBottomLeft);
        }
    }

    // There's a row above
    if (checkTop > 0)
    {
        // TOP
        neighbours.push_back(checkTop);

        int checkTopRight   = index - N_COLUMNS + 1;
        int checkTopLeft    = index - N_COLUMNS - 1;

        if (checkTopRight % N_COLUMNS != 0)
        {
            // TOP RIGHT
            neighbours.push_back(checkTopRight);
        }

        if (index % N_COLUMNS != 0)
        {
            // TOP LEFT
            neighbours.push_back(checkTopLeft);
        }
    }

    return neighbours;
}

void MainWindow::HandleFlag(int& xPos, int& yPos)
{
    if (!_isRunning)
    {
        // Game is not running, what are you doing here?
        return;
    }

    long trackerX = 0;
    long trackerY = _sectionHeight;

    for (int i = 0; i < buttons.size(); ++i)
    {
        bool inBoundsX = trackerX <= xPos && xPos <= trackerX + BUTTON_WIDTH;
        bool inBoundsY = trackerY <= yPos && yPos <= trackerY + BUTTON_HEIGHT;

        // Check if we are in the button
        if (inBoundsX && inBoundsY)
        {
            Button& button = buttons.at(i);

            // Remove previously set flag
            if (button.hasFlag)
            {
                button.hasFlag = false;

                if (button.IsMine())
                {
                    ++_iMineFlaggedCounter;
                }

                IncrementCounter();

                EnableWindow(button.handle, TRUE);
            }
            // Add a flag
            else
            {
                button.hasFlag = true;

                if (button.IsMine())
                {
                    --_iMineFlaggedCounter;
                }

                DecrementCounter();

                EnableWindow(button.handle, FALSE);
            }

            InvalidateRect(m_hwnd, NULL, TRUE);

            break;
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
    bool blocks[GRID_SIZE] = { 0 };
    std::fill_n(blocks, N_MINES, true);

    Shuffle<bool[GRID_SIZE]>(blocks);

    // Reset all buttons |  Clear number values and place bombs
    for (int i = 0; i < buttons.size(); ++i)
    {
        Button& button = buttons.at(i);

        button.hasFlag = false;
        button.isRevealed = false;

        button.number = blocks[counter] ? -1 : 0;
        counter++;
    }

    // Re-calculate distance
    CalculateDistance();

    for (Button& button : buttons)
    {
        EnableWindow(button.handle, TRUE);
        InvalidateRect(button.handle, NULL, TRUE);
    }

    InvalidateRect(m_hwnd, NULL, TRUE);
}