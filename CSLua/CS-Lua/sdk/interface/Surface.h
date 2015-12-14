#pragma once
#include "InterfaceBase.h"
/*
	DrawSetColor
	DrawFilledRect
	DrawOutlinedRect
	DrawLine
	CreateFont
	SetFontGlyphSet
	SetDrawTextFont
	SetDrawTextColor
	SetDrawTextPos
	DrawPrintText

*/
class CSurface : public InterfaceBase
{
public:
	void DrawSetcolor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* fn)(void*, int, int, int, int);
		VMT::getvfunc<fn>(this, 15)(this, r, g, b, a);
	}

	void DrawFilledRect(int x, int y, int x2, int y2)
	{
		typedef void(__thiscall* fn)(void*, int, int, int, int);
		VMT::getvfunc<fn>(this, 16)(this, x, y, x2, y2);
	}

	void DrawOutlinedRect(int x, int y, int x2, int y2)
	{

		typedef void(__thiscall* fn)(void*, int, int, int, int);
		VMT::getvfunc<fn>(this, 18)(this, x, y, x2, y2);
	}

	void DrawLine(int x, int y, int x2, int y2)
	{

		typedef void(__thiscall* fn)(void*, int, int, int, int);
		VMT::getvfunc<fn>(this, 19)(this, x, y, x2, y2);
	}
#undef CreateFont
	unsigned int CreateFont()
	{
		typedef unsigned int(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 71)(this);
	}

	bool SetFontGlyphSet(unsigned long font, char const* windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int rangeMin = 0, int rangeMax = 0)
	{
		typedef bool(__thiscall* fn)(void*, unsigned int, const char*, int, int, int, int, int, int, int);
		return VMT::getvfunc<fn>(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, rangeMin, rangeMax);
	}
	void SetTextFont(unsigned long font)
	{
		typedef void(__thiscall* fn)(void*, unsigned long);
		VMT::getvfunc<fn>(this, 23)(this, font);
	}

	void SetTextColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* fn)(void*, int, int, int, int);
		VMT::getvfunc<fn>(this, 25)(this, r, g, b, a);
	}

	void SetTextPos(int x, int y)
	{
		typedef void(__thiscall* fn)(void*, int, int);
		VMT::getvfunc<fn>(this, 26)(this, x, y);
	}

#undef DrawText
	void DrawText(const wchar_t* Text, int Len, int DrawType = 0)
	{
		typedef void(__thiscall* fn)(void*, wchar_t const*, int, int);
		VMT::getvfunc<fn>(this, 28)(this, Text, Len, DrawType);
	}
	
}; extern CSurface* g_pSurface;

enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};