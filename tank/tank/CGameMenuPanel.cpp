#include "pch.h"
#include "CGameMenuPanel.h"
#include "CGame.h"

CGameMenuPanel::CGameMenuPanel()
{
	// 背景图片
	m_imgBackground = Image::FromFile(_T("menu_background.png"));

	// 菜单项一：人机大战
	m_menuItems[0].index = 0;
	for (int i = 0; i < 2; i++)
	{
		TCHAR path[MAX_PATH];
		_stprintf_s(path, _T("menu_1player_%d.png"),i);
		auto imgPtr = Image::FromFile(path);
		RectF rc(0, 300, static_cast<float>(imgPtr->GetWidth()), static_cast<float>(imgPtr->GetHeight()));
		m_menuItems[0].vImgInfoPtr.push_back(make_tuple(imgPtr, rc));
	}

	// 菜单项二：双人大战
	m_menuItems[1].index = 0;
	for (int j = 0; j < 2; j++)
	{
		TCHAR path[MAX_PATH];
		_stprintf_s(path, _T("menu_2player_%d.png"), j);
		auto imgPtr = Image::FromFile(path);
		RectF rc(400, 300, static_cast<float>(imgPtr->GetWidth()), static_cast<float>(imgPtr->GetHeight()));
		m_menuItems[1].vImgInfoPtr.push_back(make_tuple(imgPtr, rc));
	}
}

CGameMenuPanel::~CGameMenuPanel()
{
}

void CGameMenuPanel::SetParent(CGame* g)
{
	m_pParent = g;
}

void CGameMenuPanel::Draw(Gdiplus::Graphics& gh)
{
	// 画图片
	gh.DrawImage(m_imgBackground, m_rect);
	// 画子菜单
	for (auto menuItem : m_menuItems )
	{
		auto img = get<0>(menuItem.vImgInfoPtr[menuItem.index]);
		auto rect = get<1>(menuItem.vImgInfoPtr[menuItem.index]);
		gh.DrawImage(img, rect);
	}
}

void CGameMenuPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	PointF pt(static_cast<float>(point.x), static_cast<float>(point.y));
	// 画子菜单
	for (auto & menuItem : m_menuItems)
	{
		auto img = get<0>(menuItem.vImgInfoPtr[menuItem.index]);
		auto rect = get<1>(menuItem.vImgInfoPtr[menuItem.index]);
		// 判断是否包含当前鼠标位置
		if (rect.Contains(pt))
		{
			menuItem.index = 1;
		}
		else
		{
			menuItem.index = 0;
		}

	}
}

void CGameMenuPanel::OnButtonUp(UINT nFlags, CPoint point)
{
	PointF pt(static_cast<float>(point.x), static_cast<float>(point.y));
	{
		auto& menuItem = m_menuItems[0];
		auto img = get<0>(menuItem.vImgInfoPtr[menuItem.index]);
		auto rect = get<1>(menuItem.vImgInfoPtr[menuItem.index]);
		if (rect.Contains(pt)) 
		{
			// 人机大战
			m_pParent->SetStep(CGame::EGameTypeOne2BotMenu);
			return;
		}
	}
	{
		auto& menuItem = m_menuItems[1];
		auto img = get<0>(menuItem.vImgInfoPtr[menuItem.index]);
		auto rect = get<1>(menuItem.vImgInfoPtr[menuItem.index]);
		if (rect.Contains(pt))
		{
			// 双人对战
			m_pParent->SetStep(CGame::EGameTypeOne2OneMenu);
			return;
		}
	}
}
