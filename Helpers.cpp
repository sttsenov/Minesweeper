#include "stdafx.h"

#include "Helpers.h"

using namespace Helpers;

void Helpers::DrawImage(HDC& hDC, RECT& rc, HBITMAP& image)
{
    if (image == NULL)
    {
        return;
    }

    // Offsets
    RECT rect = rc;
    rect.right -= 5;
    rect.left -= 1;
    rect.top -= 1;
    rect.bottom -= 1;

    HBRUSH brush = CreatePatternBrush(image);

    FillRect(hDC, &rect, brush);
    DeleteObject(brush);
}

void Helpers::SetNumberText(HDC& hDC, int8_t& number)
{
    switch (number)
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
}

void Directions::GetNeighbours(
    std::vector<int>& neighbours, 
    int& index, 
    Directions::Direction direction)
{
    switch (direction)
    {
    case RIGHT:
        GetNeighboursRight(neighbours, index);
        break;
    case LEFT:
        GetNeighboursLeft(neighbours, index);
        break;
    case BOTTOM:
        GetNeighboursBottom(neighbours, index);
        break;
    case TOP:
        GetNeighboursTop(neighbours, index);
        break;
    default:
        return;
    }
}

void Directions::GetNeighboursTop(std::vector<int> & neighbours, int& index)
{
    int checkTop = index - N_COLUMNS;

    // There's a row above
    if (checkTop > 0)
    {
        // TOP
        neighbours.push_back(checkTop);

        int checkTopRight = index - N_COLUMNS + 1;
        int checkTopLeft = index - N_COLUMNS - 1;

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
}

void Directions::GetNeighboursBottom(std::vector<int>& neighbours, int& index)
{
    int checkBottom = index + N_COLUMNS;

    // There's a row below
    if (checkBottom < GRID_SIZE)
    {
        // BOTTOM
        neighbours.push_back(checkBottom);

        int checkBottomRight = index + N_COLUMNS + 1;
        int checkBottomLeft = index + N_COLUMNS - 1;

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
}

void Directions::GetNeighboursLeft(std::vector<int>& neighbours, int& index)
{
    int checkLeft = index - 1;

    // We can go left and be on the same row
    if (checkLeft >= 0 && index % N_COLUMNS != 0)
    {
        // LEFT
        neighbours.push_back(checkLeft);
    }
}

void Directions::GetNeighboursRight(std::vector<int>& neighbours, int& index)
{
    int checkRight = index + 1;

    // We can go right and be on the same row
    if (checkRight <= GRID_SIZE && checkRight % N_COLUMNS != 0)
    {
        // RIGHT
        neighbours.push_back(checkRight);
    }
}