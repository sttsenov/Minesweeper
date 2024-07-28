#pragma once

#include "stdafx.h"

#include "Structures.h"

namespace Helpers
{
    void DrawImage(HDC& hDC, RECT& rc, HBITMAP& image);

    void SetNumberText(HDC& hDC, int8_t& number);

    namespace Directions
    {
        enum Direction
        {
            RIGHT,
            LEFT,
            BOTTOM,
            TOP
        };

        void GetNeighbours(std::vector<int>& neighbours, int& index, Direction direction);

        void GetNeighboursTop(std::vector<int>& neighbours, int& index);
        void GetNeighboursBottom(std::vector<int>& neighbours, int& index);
        void GetNeighboursLeft(std::vector<int>& neighbours, int& index);
        void GetNeighboursRight(std::vector<int>& neighbours, int& index);
    }
}