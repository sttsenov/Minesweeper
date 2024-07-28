#pragma once
#include "stdafx.h"

struct Dimensions
{
    Dimensions() : x(CW_USEDEFAULT), y(CW_USEDEFAULT), width(CW_USEDEFAULT), height(CW_USEDEFAULT) {}
    Dimensions(int newX, int newY, int newWidth, int newHeight) : x(newX), y(newY), width(newWidth), height(newHeight) {}

    int x;
    int y;
    int width;
    int height;
};

struct Button
{
    Button() : handle(NULL), number(0), hasFlag(false), isRevealed(false) {}
    Button(HWND nHandle, int8_t nNumber) : handle(nHandle), number(nNumber)
    {
        hasFlag = false;
        isRevealed = false;
    }

    bool IsMine() { return (number < 0); }

    HWND handle;
    int8_t number;

    bool hasFlag{ false };
    bool isRevealed{ false };
};