#pragma once

#include "stdafx.h"

#include "Structures.h"

namespace Helpers
{
    template<typename T>
    void Shuffle(T& obj)
    {
        unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(std::begin(obj), std::end(obj), std::default_random_engine(seed));
    }

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