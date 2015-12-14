#pragma once

// Windows
#include <Windows.h>
#include <vector>

struct CUR {
	int x, y;
	CUR() {
		this->SetPos(0, 0);
	}
	CUR(int x, int y) {
		this->SetPos(x, y);
	}

	inline void SetPos(int x, int y) { this->x = x; this->y = y; }
};

enum struct MouseEvent : unsigned int {
	Unknown,
	Move,
	Down,
	Up,
	DoubleClick,
	Scroll
};

enum struct KeyEvent : unsigned int {
	Unknown,
	Down,
	Up
};

enum struct MouseButton : unsigned int {
	None,
	Left,
	Right,
	Middle
};


enum struct Keys : unsigned long {
	Backspace = 0x08,
	Tab,
	Enter = 0x0D,
	Shift = 0x10,
	Control,
	Alt,
	CapsLock = 0x14,
	Spacebar = 0x20,
	PageUp,
	PageDown,
	End,
	Home,
	Left,
	Up,
	Right,
	Down,
	Insert = 0x2D,
	Delete,
	Numpad0 = 0x60,
	Numpad1,
	Numpad2,
	Numpad3,
	Numpad4,
	Numpad5,
	Numpad6,
	Numpad7,
	Numpad8,
	Numpad9,
	Multiply,
	Add,
	Subtract = 0x6D,
	Decimal,
	Divide,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	Numlock = 0x90,
	Scrolllock
};

struct MouseData {
	MouseEvent event;
	MouseButton button;
	CUR pos;
	int delta;
};

struct KeyData {
	unsigned int key;
	unsigned int scancode;
	KeyEvent state;
	KeyEvent prevstate;
	bool processed;

	bool IsDown() const
	{
		return (state == KeyEvent::Down && prevstate == KeyEvent::Up);
	}
};

typedef bool(*mouse_callback)(const MouseData &);
typedef bool(*key_callback)(const KeyData &);

namespace inputmanager {
	namespace backend {
		extern HWND procHWND;
		extern DWORD procID;
		extern WNDPROC oldWndProc;

		extern CUR mouse;

		extern std::vector<mouse_callback> mcallbacks;
		inline bool InvokeMouse(const MouseData &m) {
			bool ret = false;
			for (std::vector<mouse_callback>::iterator it = backend::mcallbacks.begin(); it != backend::mcallbacks.end(); ++it) {
				mouse_callback callback = *it;
				if (callback)
					if (callback(m))
						ret = true;
			}
			return (ret);
		}

		extern std::vector<key_callback> keycallbacks;
		inline bool InvokeKeyboard(const KeyData &k) {
			bool ret = false;
			for (std::vector<key_callback>::iterator it = backend::keycallbacks.begin(); it != backend::keycallbacks.end(); ++it) {
				key_callback callback = *it;
				if (callback)
					if (callback(k))
						ret = true;
			}
			return (ret);
		}

		BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
		LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		const MouseData ProcessMouseMessage(UINT msg, WPARAM wParam, LPARAM lParam);
		const KeyData ProcessKeyboardMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	}
	void Init();

	inline void AddMouseCallback(mouse_callback m) { backend::mcallbacks.push_back(m); }
	inline void AddKeyboardCallback(key_callback k) { backend::keycallbacks.push_back(k); }

	inline int GetCursorX() { return (backend::mouse.x); }
	inline int GetCursorY() { return (backend::mouse.y); }
	inline CUR GetCursor() { return (backend::mouse); }
};