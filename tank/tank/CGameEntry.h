#pragma once
class CGameEntry
{
public:
	CGameEntry();
	virtual ~CGameEntry();

public:
	virtual void Draw(Gdiplus::Graphics& gh) = 0; // ´¿Ðéº¯Êý
};
