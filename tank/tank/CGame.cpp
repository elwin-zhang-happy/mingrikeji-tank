#include "pch.h"
#include "CGame.h"
#define KEYDOWN(vk) (GetAsyncKeyState(vk) & 0x8000) 

CGame::CGame()
{
	m_menuSelect.m_pParent = this;
	m_menuBackup.m_pParent = this;
}

CGame::~CGame()
{
}

void CGame::SetHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}

bool CGame::EnterFrame(DWORD dwTime)
{
	GameRunDraw();
	GameRunLogic();
	return false;
}

void CGame::OnMouseMove(UINT nFlags, CPoint point)
{
	// 选择阶段
	if (m_eStep == EGameTypeMenu)
	{
		m_menuSelect.OnMouseMove(nFlags, point);
	}
	else
	{
		m_menuBackup.OnMouseMove(nFlags, point);
	}
}

void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
	// 选择阶段
	if (m_eStep == EGameTypeMenu)
	{
		m_menuSelect.OnButtonUp(nFlags, point);
	}
	else
	{
		m_menuBackup.OnLButtonUp(nFlags, point);
	}
}

void CGame::SetStep(CGame::EGameType step)
{
	m_eStep = step;
	GameInit();
}

void CGame::GameRunDraw()
{
	// 窗口图形设备
	HDC hdc = ::GetDC(m_hWnd);
	// 长方形
	CRect rc;
	GetClientRect(m_hWnd, &rc);
	// 获取对象设备
	CDC * dc = CClientDC::FromHandle(hdc);
	// 双缓冲绘图
	CDC m_dcMemory;
	CBitmap bmp; // 位图，上下文图像
	bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
	m_dcMemory.CreateCompatibleDC(dc);
	// 获取上一步位图
	CBitmap* pOldBitmap = m_dcMemory.SelectObject(&bmp);
	// 构造对象
	Gdiplus::Graphics gh(m_dcMemory.GetSafeHdc());
	gh.Clear(Gdiplus::Color::White);
	gh.ResetClip();

	// 画入内存
	(this->*m_drawFunc[m_eStep])(gh);

	// 复制到屏幕
	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	// 释放
	//dc->DeleteDC();
	::ReleaseDC(m_hWnd, hdc);
}

void CGame::GameRunDrawOnMenu(Graphics& gh)
{
	m_menuSelect.Draw(gh);
}

void CGame::GameRunDrawOnOne2BotMenu(Graphics& gh)
{
	// 画背景
	m_menu.Draw(gh);
	// 画菜单
	m_keymenu01.Draw(gh);
}

void CGame::GameRunDrawOnOne2Bot(Graphics& gh)
{
	// 画菜单
	m_menuBackup.Draw(gh);
	// 画墙
	m_map.Draw(gh);
	// 画玩家一
	m_player01.Draw(gh);
	// 画机器人
	m_bot.Draw(gh);
	// 已发射的子弹
	for (auto &blt : m_lstBullets)
	{
		blt.Draw(gh);
	}
	// 输出fps
	DrawFps(gh);
}

void CGame::GameRunDrawOnOne2BotEnd(Graphics& gh)
{
	// 画菜单
	m_menuBackup.Draw(gh);
	// 画墙
	m_map.Draw(gh);
	// 画玩家一
	m_player01.Draw(gh);
	// 画机器人
	m_bot.Draw(gh);

	if (m_player01.IsBombEnd() || m_bot.IsBombEnd())
	{
		m_eStep = EGameTypeMenu;
	}
}

void CGame::GameRunDrawOnOne2OneMenu(Graphics& gh)
{
	// 画菜单
	m_menuBackup.Draw(gh);
	// 画菜单项一
	m_keymenu01.Draw(gh);
	// 画菜单项二
	m_keymenu02.Draw(gh);
}

void CGame::GameRunDrawOnOne2One(Graphics& gh)
{
	// 画菜单
	m_menuBackup.Draw(gh);
	// 画墙
	m_map.Draw(gh);
	// 画玩家一
	m_player01.Draw(gh);
	// 画玩家二
	m_player02.Draw(gh);
	// 已发射的子弹
	for (auto& blt : m_lstBullets)
	{
		blt.Draw(gh);
	}
	DrawFps(gh);

	if (m_player01.IsBombEnd() || m_player02.IsBombEnd())
	{
		m_eStep = EGameTypeMenu;
	}
}

void CGame::GameRunDrawOnOne2OneEnd(Graphics& gh)
{
	// 画菜单
	m_menuBackup.Draw(gh);
	// 画墙
	m_map.Draw(gh);
	// 画玩家一
	m_player01.Draw(gh);
	// 画玩家二
	m_player02.Draw(gh);
	// 已发射的子弹
	for (auto& blt : m_lstBullets)
	{
		blt.Draw(gh);
	}
	DrawFps(gh);
}

void CGame::RemoveTimeoutBullets()
{
	// 定义查找方法
	auto itRemove = remove_if(m_lstBullets.begin(), m_lstBullets.end(), [](CBullet& blt)->bool {return blt.IsTimeout(); });
	// 把子弹移除，并给对应坦克增加子弹
	for(auto it = itRemove; it != m_lstBullets.end(); ++it)
	{
		it->SetActive(false);
		it->GetOwner()->AddBullet(*it);
	}
	// 从本地删除子弹
	m_lstBullets.erase(itRemove, m_lstBullets.end());
}

void CGame::ProcessHitBullets()
{
	for (auto &blt : m_lstBullets)
	{
		m_map.HitWallProcess(blt);
		blt.Move();
	}
}

void CGame::AI()
{
	// 电脑运动状态维护
	static CGameTimer acTimer(-1, 150);
	
	if (acTimer.IsTimeval())
	{
		int startX, startY, targetX, targetY;
		if (!m_map.FindObjPosition(m_bot, startX, startY) || !m_map.FindObjPosition(m_player01, targetX, targetY))
		{
			return;
		}
		float fDirNext = 0;
		if (!m_map.FindNextDirection(&fDirNext, startX, startY, targetX, targetY))
		{
			return;
		}

		// 获取机器人
		PointF ptTankCenter = m_bot.GetCenterPoint();
		PointF ptAreaCenter = m_map.GetElementAreaCenter(startX, startY);
		RectF rc(ptAreaCenter.X - 5, ptAreaCenter.Y - 5, 10,10);

		if (rc.Contains(ptTankCenter))
		{
			m_bot.Forward();
			return;
		}
		else
		{
			m_bot.SetDirection(fDirNext);
			float dir;
			if (m_map.IsCanKillTarget(m_bot, m_player01, &dir))
			{
				CBullet blt;
				if (m_bot.Fire(blt))
				{
					m_lstBullets.push_back(blt);
				}
				return;
			}
			m_bot.Forward();
		}
	}
}

void CGame::DrawFps(Gdiplus::Graphics& gh)
{
	static int fps = 0;
	m_fps++;
	static DWORD dwLast = GetTickCount();
	// 没秒钟刷新一次，展示m_fps
	if (GetTickCount() - dwLast >= 1000) 
	{
		fps = m_fps;
		m_fps = 0;
		dwLast = GetTickCount();
	}

	// 输出fps
	CString s;
	s.Format(_T("FPS:%d"), fps);
	// 创建红色的画刷
	SolidBrush brush(Color(0x00, 0x00, 0xFF));
	Gdiplus::Font font(_T("宋体"), 10.0);
	// 获取当前窗口
	CRect rc;
	::GetClientRect(m_hWnd,rc);
	// 位置,右上角
	PointF origin(static_cast<float>(rc.right - 100), static_cast<float>(rc.top + 10));
	
	gh.DrawString(s.GetString(), -1, &font, origin, &brush);
}

bool CGame::GameInit()
{
	// 初始化随机数生成器
	srand(GetTickCount());
	// 根据不同阶段调用不同的处理方法
	return (this->*m_initFunc[m_eStep])();
}

bool CGame::GameInitMenu()
{
	return true;
}

// 游戏初始化： 人机大战菜单阶段
bool CGame::GameInitOne2BotMenu()
{
	RECT rc;
	GetWindowRect(m_hWnd, &rc);
	PointF pt;
	pt.X = rc.left + (rc.right - rc.left) / 2.0f;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	// 设置人机大战keyMenu位置为屏幕正中
	m_keymenu01.SetCenterPoint(pt);
	m_keymenu01.SetStop(false);
	return true;
}

bool CGame::GameInitOne2Bot()
{
	for ( ; ;)
	{
		// 载入地图
		m_map.LoadMap();
		// 玩家一
		{
			m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("调整player01位置失败"));
			}
			else
			{
				m_player01.SetCenterPoint(ptCenter);
			}
		}
		// 机器人（电脑控制的坦克）
		{
			m_bot = CBot(0, 0, _T("tank_bot.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("调整Bot位置失败"));
			}
			else
			{
				// 放置机器人到空地正中
				m_bot.SetCenterPoint(ptCenter);
			}
		}
		// 清空子弹数组
		m_lstBullets.clear();

		// 判断是否合法
		{
			// 获取机器人，玩家一所在的位置
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_bot, startX, startY) || m_map.FindObjPosition(m_player01, targetX, targetY))
			{
				AfxMessageBox(_T("获取坦克位置发生错误"));
				goto ___Init_End;
			}
			// 判断玩家一和机器人是否联通，如果无法联通，无法进入游戏
			VPath path;
			m_map.FindPath(startX, startY, targetX, targetY, path);
			if (!path.empty())
			{
				goto ___Init_End;
			}
		}
	}
___Init_End:
	return true;
}

bool CGame::GameInitOne2BotEnd()
{
	return true;
}

bool CGame::GameInitOne2OneMenu()
{
	// 设置两个玩家的keymenu位置，屏幕正中间
	RECT rc;
	GetWindowRect(m_hWnd, &rc);
	PointF pt;
	pt.X = rc.left + m_keymenu01.GetRect().Width / 2.0f + 100;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	// 设置菜单项的位置
	m_keymenu01.SetCenterPoint(pt);
	// 设置不停止播放动画
	m_keymenu01.SetStop(false);

	return true;
}

bool CGame::GameInitOne2One()
{
	for (;;) 
	{
		// 载入地图
		m_map.LoadMap();
		{	
			// 构造玩家一对象
			m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
			PointF ptCenter;
			// 查找随机的空地位置
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("调整player01位置失败"));
			}
			else {
				// 设置玩家一到这块空地中心
				m_player01.SetCenterPoint(ptCenter);
			}
		}
		{
			// 构造玩家二对象
			m_player02 = CPlayer(0, 0, _T("tank_player2.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("调整player02位置失败"));
			}
			else {
				m_player02.SetCenterPoint(ptCenter);
			}
		}
		m_lstBullets.clear();
		// 判断是否合法
		{
			// 查找机器人，玩家所在的位置
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_player01, startX, startY) || 
				!m_map.FindObjPosition(m_player02, targetX, targetY))
			{
				AfxMessageBox(_T("获取坦克位置发生错误"));
			}
			// 判断两个玩家是否可以联通
			VPath path;
			m_map.FindPath(startX, startY, targetX, targetY, path);
			if (!path.empty())
			{
				break;
			}
		}
	}
	return true;
}

bool CGame::GameInitOne2OneEnd()
{
	return true;
}

void CGame::GameRunLogic()
{
	(this->*m_logicFunc[m_eStep])();
}

void CGame::GameRunLogicOnMenu()
{
	
}

void CGame::GameRunLogicOnOne2BotMenu()
{
	if (KEYDOWN('M'))
	{
		// 按下M停止动画
		m_keymenu01.SetStop();
	}
	if (m_keymenu01.GetStop())
	{
		SetStep(EGameTypeOne2Bot);
	}
}

void CGame::GameRunLogicOnOne2Bot()
{
	// 状态维护
	// 移除表中无效子弹，并给相应的坦克增加子弹
	RemoveTimeoutBullets();

	// 检查子弹是否击中坦克，击中爆炸
	for (auto& blt : m_lstBullets)
	{
		if (m_bot.IsHitted(blt))
		{
			// 机器人爆炸
			m_bot.Bomb();
			m_eStep = EGameTypeOne2BotEnd;
			blt.SetActive(false);
		}
		if (m_player01.IsHitted(blt))
		{
			// 玩家爆炸
			m_player01.Bomb();
			m_eStep = EGameTypeOne2BotEnd;
			blt.SetActive(false);
		}
		break;
	}
	// 子弹运动维护
	ProcessHitBullets();
	// 使机器人自动攻击玩家
	AI();
	// 按键处理
	{
		if (KEYDOWN(VK_LEFT))
		{
			m_player01.RotateLeft();
		}
		if (KEYDOWN(VK_RIGHT))
		{
			m_player01.RotateRight();
		}
		if (KEYDOWN(VK_UP))
		{
			// 坦克撞墙检测
			{
				if (m_map.IsHitTheWall(m_player01, true))
				{
					m_player01.ChangeDirection(true);
				}
				else
				{
					m_player01.Forward();
				}
			}
		}
		if (KEYDOWN(VK_DOWN))
		{
			{
				if (m_map.IsHitTheWall(m_player01, true))
				{
					m_player01.ChangeDirection(false);
				}
				else
				{
					m_player01.Backward();
				}
			}
		}
		if (KEYDOWN('M'))
		{
			CBullet blt;
			if (m_player01.Fire(blt))
			{
				m_lstBullets.push_back(blt);
			}
		}
		if (KEYDOWN('I'))
		{
			// 按下键盘上I键，机器人步进
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_bot, startX, startY)||!m_map.FindObjPosition(m_player01, targetX, targetY))
			{
				return;
			}
			float fDirNext = 0; // 机器人下一步的方向
			if (!m_map.FindNextDirection(&fDirNext, startX, startY, targetX, targetY))
			{
				return;
			}
			
			// 获取机器人
			PointF ptTankCenter = m_bot.GetCenterPoint();
			PointF ptAreaCenter = m_map.GetElementAreaCenter(startX, startY);
			RectF rc(ptAreaCenter.X - 5, ptAreaCenter.Y - 5, 10, 10);

			if (rc.Contains(ptTankCenter))
			{
				m_bot.Forward();
			}
			else
			{
				m_bot.SetDirection(fDirNext);
				m_bot.Forward();
			}
		}
		
	}
}

void CGame::GameRunLogicOnOne2BotEnd()
{
	//不接受按键

	// 状态维护
	// 移除列表中无效的子弹，并给相应坦克增加子弹
	RemoveTimeoutBullets();

	// 子弹撞墙改变方向发
	ProcessHitBullets();
}

void CGame::GameRunLogicOnOne2OneMenu()
{
	if (KEYDOWN('M'))
	{
		m_keymenu01.SetStop();
	}
	if (KEYDOWN('Q'))
	{
		m_keymenu02.SetStop();
	}

	if (m_keymenu01.GetStop() && m_keymenu02.GetStop())
	{
		SetStep(EGameTypeOne2One);
	}
}

void CGame::GameRunLogicOnOne2One()
{
	// 玩家一
	if (KEYDOWN(VK_LEFT))
	{
		m_player01.RotateLeft();
	}
	if (KEYDOWN(VK_RIGHT))
	{
		m_player01.RotateRight();
	}
	if (KEYDOWN(VK_UP))
	{
		// 坦克撞墙检测
		{
			if (m_map.IsHitTheWall(m_player01, true))
			{
				m_player01.ChangeDirection(true);
			}
			else
			{
				m_player01.Forward();
			}
		}
	}
	if (KEYDOWN(VK_DOWN))
	{
		{
			if (m_map.IsHitTheWall(m_player01, true))
			{
				m_player01.ChangeDirection(false);
			}
			else
			{
				m_player01.Backward();
			}
		}
	}
	if (KEYDOWN('M'))
	{
		CBullet blt;
		if (m_player01.Fire(blt))
		{
			m_lstBullets.push_back(blt);
		}
	}

	// 玩家二
	if (KEYDOWN('A'))
	{
		m_player02.RotateLeft();
	}
	if (KEYDOWN('D'))
	{
		m_player02.RotateRight();
	}
	if (KEYDOWN('W'))
	{
		// 坦克撞墙检测
		{
			if (m_map.IsHitTheWall(m_player02, true))
			{
				m_player02.ChangeDirection(true);
			}
			else {
				m_player02.Forward();
			}
		}
	}
	if (KEYDOWN('S'))
	{
		// 坦克撞墙检测
		{
			if (m_map.IsHitTheWall(m_player02, false))
			{
				m_player02.ChangeDirection(false);
			}
			else {
				m_player02.Backward();
			}
		}
	}
	if (KEYDOWN('Q'))
	{
		// 按下Q键开火
		CBullet blt;
		if (m_player02.Fire(blt))
		{
			m_lstBullets.push_back(blt);
		}
	}
	if (KEYDOWN('Z'))
	{
		if (m_map.IsCanKillTarget(m_player01, m_player02)) 
		{
			AfxMessageBox(_T("可以打到"));
		}
	}
	// 移除列表中无效子弹
	RemoveTimeoutBullets();

	for (auto & blt : m_lstBullets)
	{
		if (!blt.IsActive())
		{
			continue;
		}
		if (m_player01.IsHitted(blt))
		{
			m_player01.Bomb();
			m_eStep = EGameTypeOne2OneEnd;
			blt.SetActive(false);
		}
		if (m_player02.IsHitted(blt))
		{
			m_player02.Bomb();
			m_eStep = EGameTypeOne2OneEnd;
			blt.SetActive(false);
		}
	}

	ProcessHitBullets();
}

void CGame::GameRunLogicOnOne2OneEnd()
{
	// 不需要处理按键
	// 移除列表中无效的子弹，并给相应的坦克增加子弹
	RemoveTimeoutBullets();
	// 子弹撞墙
	ProcessHitBullets();
}
