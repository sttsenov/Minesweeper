#pragma once

#include "stdafx.h"

#include "Helpers.h"

template <class DERIVED_TYPE>
class BaseWindow
{
public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DERIVED_TYPE* pThis = NULL;

        if (uMsg == WM_NCCREATE)
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            pThis->m_hwnd = hwnd;

            SetTimer(hwnd,              // handle to main window 
                100,                    // timer identifier 
                1000,                   // 1-second interval 
                (TIMERPROC)NULL);       // no timer callback 
        }
        else
        {
            pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }
        if (pThis)
        {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    BaseWindow() : m_hwnd(NULL) { }

    BOOL Create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        Dimensions dimensions = Dimensions(),
        HWND hWndParent = 0,
        HMENU hMenu = 0
    )
    {
        WNDCLASS wc = { 0 };

        wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = ClassName();
        wc.hIcon = (HICON)LoadImage(NULL, L"assets\\mine.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);

        DWORD err = GetLastError();

        RegisterClass(&wc);

        // Populate this window
        m_hwnd = CreateWindowEx(
            dwExStyle, ClassName(), lpWindowName, dwStyle, dimensions.x, dimensions.y,
            dimensions.width, dimensions.height, hWndParent, hMenu, GetModuleHandle(NULL), this
        );

        return (m_hwnd ? TRUE : FALSE);
    }

    // Buttons
    BOOL CreateButton(
        PCWSTR lpWindowName,
        PCWSTR lpWindowText,
        HWND& hPushButton,
        HMENU hMenu = NULL,
        Dimensions dimensions = Dimensions(0, 0, 50, 50)
    )
    {
        DWORD dwStyle = WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW;
        HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE);

        hPushButton = CreateWindow(
            lpWindowName, lpWindowText, dwStyle, dimensions.x, dimensions.y,
            dimensions.width, dimensions.height, Window(), hMenu, hInstance, this
        );

        return (hPushButton ? TRUE : FALSE);
    }

    BOOL CreateGameButton(
        PCWSTR lpWindowName,
        PCWSTR lpWindowText,
        Dimensions dimensions   = Dimensions(0, 0, 50, 50),
        // Tracks how close we are to a mine.
        int8_t numberValue      = 0     // NOTE: -1 means that we are mine
        ) 
    {
        HWND hPushButton;
        CreateButton(lpWindowName, lpWindowText, hPushButton, NULL, dimensions);

        if (!hPushButton)
        {
            return FALSE;
        }

        buttons.push_back(Button(hPushButton, numberValue));
        return TRUE;
    }

    HWND Window() const { return m_hwnd; }

    std::vector<Button>& GetGameButtons() { return buttons; }

protected:

    virtual PCWSTR  ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HWND m_hwnd;
    std::vector<Button> buttons;
};