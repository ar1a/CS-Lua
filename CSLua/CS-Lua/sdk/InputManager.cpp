#include "InputManager.h"


HWND inputmanager::backend::procHWND;
DWORD inputmanager::backend::procID;
WNDPROC inputmanager::backend::oldWndProc;
CUR inputmanager::backend::mouse;
std::vector<mouse_callback> inputmanager::backend::mcallbacks;
std::vector<key_callback> inputmanager::backend::keycallbacks;

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp)) // ripped from windowsx
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

void inputmanager::Init()
{
	EnumWindows(backend::EnumWindowsProc, GetCurrentProcessId());

	if (backend::procHWND)
		backend::oldWndProc = (WNDPROC)SetWindowLongPtr(backend::procHWND, GWL_WNDPROC, (LONG)backend::WindowProc);
}

BOOL CALLBACK inputmanager::backend::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD procID_t;
	GetWindowThreadProcessId(hwnd, &procID_t);
	char buf[260];
	GetClassName(hwnd, buf, sizeof(buf));

	if (lParam == procID_t && !strstr(buf, "Chrome") && !strstr(buf, "Console"))
	{
		backend::procHWND = hwnd;
		return (FALSE);
	}

	return (TRUE);
}

LRESULT CALLBACK inputmanager::backend::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool bBlockInput = false;
	MouseData m;
	KeyData k;
	switch (uMsg)
	{
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		m = backend::ProcessMouseMessage(uMsg, wParam, lParam);
		bBlockInput = backend::InvokeMouse(m);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_CHAR:
		k = backend::ProcessKeyboardMessage(uMsg, wParam, lParam);
		bBlockInput = backend::InvokeKeyboard(k);
		break;
	}

	if (bBlockInput)
		return 0;
	return (CallWindowProc(backend::oldWndProc, backend::procHWND, uMsg, wParam, lParam));
}


const MouseData inputmanager::backend::ProcessMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	backend::mouse.SetPos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	MouseData m;
	m.button = MouseButton::None;
	m.event = MouseEvent::Unknown;
	m.delta = 0;
	m.pos = backend::mouse;

	switch (uMsg) {
	case WM_MOUSEMOVE:
		m.event = MouseEvent::Move;
		break;
	case WM_LBUTTONDBLCLK:
		m.event = MouseEvent::DoubleClick;
		m.button = MouseButton::Left;
		break;
	case WM_LBUTTONDOWN:
		m.event = MouseEvent::Down;
		m.button = MouseButton::Left;
		break;
	case WM_LBUTTONUP:
		m.event = MouseEvent::Up;
		m.button = MouseButton::Left;
		break;
	case WM_MBUTTONDBLCLK:
		m.event = MouseEvent::DoubleClick;
		m.button = MouseButton::Middle;
		break;
	case WM_MBUTTONDOWN:
		m.event = MouseEvent::Down;
		m.button = MouseButton::Middle;
		break;
	case WM_MBUTTONUP:
		m.event = MouseEvent::Up;
		m.button = MouseButton::Middle;
		break;
	case WM_MOUSEWHEEL:
		m.event = MouseEvent::Scroll;
		m.delta = HIWORD(wParam); // multiples of 120.
		break;
	case WM_RBUTTONDBLCLK:
		m.event = MouseEvent::DoubleClick;
		m.button = MouseButton::Right;
		break;
	case WM_RBUTTONDOWN:
		m.event = MouseEvent::Down;
		m.button = MouseButton::Right;
		break;
	case WM_RBUTTONUP:
		m.event = MouseEvent::Up;
		m.button = MouseButton::Right;
		break;

	}



	return(m);


}

const KeyData inputmanager::backend::ProcessKeyboardMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	KeyData k;
	k.key = wParam;
	k.state = KeyEvent::Unknown;
	k.prevstate = KeyEvent::Unknown;
	k.processed = false;
	k.scancode = (lParam >> 16) & ~(~0 << 7);

	switch (msg)
	{
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		k.state = KeyEvent::Down;
		k.prevstate = ((lParam >> 30) & 1) ? KeyEvent::Down : KeyEvent::Up;
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		k.state = KeyEvent::Up;
		k.prevstate = KeyEvent::Down;
		break;
	case WM_CHAR:
		k.state = KeyEvent::Down;
		k.prevstate = ((lParam >> 30) & 1) ? KeyEvent::Down : KeyEvent::Up;
		k.processed = true;
		break;
	}


	return (k);
}