#include "pch.h"
#include "CGameMenuBackground.h"
#include "CGame.h"

CGameMenuBackground::CGameMenuBackground()
{
	m_imgBackground = Image::FromFile(_T("menu_background.png"));
}

CGameMenuBackground::~CGameMenuBackground()
{

}

void CGameMenuBackground::SetParent(CGame * g)
{
	m_pParent = g;
}

void CGameMenuBackground::Draw(Graphics& gh)
{
	gh.DrawImage(m_imgBackground, m_rect);
}
