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
	// ѡ��׶�
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
	// ѡ��׶�
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
	// ����ͼ���豸
	HDC hdc = ::GetDC(m_hWnd);
	// ������
	CRect rc;
	GetClientRect(m_hWnd, &rc);
	// ��ȡ�����豸
	CDC * dc = CClientDC::FromHandle(hdc);
	// ˫�����ͼ
	CDC m_dcMemory;
	CBitmap bmp; // λͼ��������ͼ��
	bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
	m_dcMemory.CreateCompatibleDC(dc);
	// ��ȡ��һ��λͼ
	CBitmap* pOldBitmap = m_dcMemory.SelectObject(&bmp);
	// �������
	Gdiplus::Graphics gh(m_dcMemory.GetSafeHdc());
	gh.Clear(Gdiplus::Color::White);
	gh.ResetClip();

	// �����ڴ�
	(this->*m_drawFunc[m_eStep])(gh);

	// ���Ƶ���Ļ
	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	// �ͷ�
	//dc->DeleteDC();
	::ReleaseDC(m_hWnd, hdc);
}

void CGame::GameRunDrawOnMenu(Graphics& gh)
{
	m_menuSelect.Draw(gh);
}

void CGame::GameRunDrawOnOne2BotMenu(Graphics& gh)
{
	// ������
	m_menu.Draw(gh);
	// ���˵�
	m_keymenu01.Draw(gh);
}

void CGame::GameRunDrawOnOne2Bot(Graphics& gh)
{
	// ���˵�
	m_menuBackup.Draw(gh);
	// ��ǽ
	m_map.Draw(gh);
	// �����һ
	m_player01.Draw(gh);
	// ��������
	m_bot.Draw(gh);
	// �ѷ�����ӵ�
	for (auto &blt : m_lstBullets)
	{
		blt.Draw(gh);
	}
	// ���fps
	DrawFps(gh);
}

void CGame::GameRunDrawOnOne2BotEnd(Graphics& gh)
{
	// ���˵�
	m_menuBackup.Draw(gh);
	// ��ǽ
	m_map.Draw(gh);
	// �����һ
	m_player01.Draw(gh);
	// ��������
	m_bot.Draw(gh);

	if (m_player01.IsBombEnd() || m_bot.IsBombEnd())
	{
		m_eStep = EGameTypeMenu;
	}
}

void CGame::GameRunDrawOnOne2OneMenu(Graphics& gh)
{
	// ���˵�
	m_menuBackup.Draw(gh);
	// ���˵���һ
	m_keymenu01.Draw(gh);
	// ���˵����
	m_keymenu02.Draw(gh);
}

void CGame::GameRunDrawOnOne2One(Graphics& gh)
{
	// ���˵�
	m_menuBackup.Draw(gh);
	// ��ǽ
	m_map.Draw(gh);
	// �����һ
	m_player01.Draw(gh);
	// ����Ҷ�
	m_player02.Draw(gh);
	// �ѷ�����ӵ�
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
	// ���˵�
	m_menuBackup.Draw(gh);
	// ��ǽ
	m_map.Draw(gh);
	// �����һ
	m_player01.Draw(gh);
	// ����Ҷ�
	m_player02.Draw(gh);
	// �ѷ�����ӵ�
	for (auto& blt : m_lstBullets)
	{
		blt.Draw(gh);
	}
	DrawFps(gh);
}

void CGame::RemoveTimeoutBullets()
{
	// ������ҷ���
	auto itRemove = remove_if(m_lstBullets.begin(), m_lstBullets.end(), [](CBullet& blt)->bool {return blt.IsTimeout(); });
	// ���ӵ��Ƴ���������Ӧ̹�������ӵ�
	for(auto it = itRemove; it != m_lstBullets.end(); ++it)
	{
		it->SetActive(false);
		it->GetOwner()->AddBullet(*it);
	}
	// �ӱ���ɾ���ӵ�
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
	// �����˶�״̬ά��
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

		// ��ȡ������
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
	// û����ˢ��һ�Σ�չʾm_fps
	if (GetTickCount() - dwLast >= 1000) 
	{
		fps = m_fps;
		m_fps = 0;
		dwLast = GetTickCount();
	}

	// ���fps
	CString s;
	s.Format(_T("FPS:%d"), fps);
	// ������ɫ�Ļ�ˢ
	SolidBrush brush(Color(0x00, 0x00, 0xFF));
	Gdiplus::Font font(_T("����"), 10.0);
	// ��ȡ��ǰ����
	CRect rc;
	::GetClientRect(m_hWnd,rc);
	// λ��,���Ͻ�
	PointF origin(static_cast<float>(rc.right - 100), static_cast<float>(rc.top + 10));
	
	gh.DrawString(s.GetString(), -1, &font, origin, &brush);
}

bool CGame::GameInit()
{
	// ��ʼ�������������
	srand(GetTickCount());
	// ���ݲ�ͬ�׶ε��ò�ͬ�Ĵ�����
	return (this->*m_initFunc[m_eStep])();
}

bool CGame::GameInitMenu()
{
	return true;
}

// ��Ϸ��ʼ���� �˻���ս�˵��׶�
bool CGame::GameInitOne2BotMenu()
{
	RECT rc;
	GetWindowRect(m_hWnd, &rc);
	PointF pt;
	pt.X = rc.left + (rc.right - rc.left) / 2.0f;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	// �����˻���սkeyMenuλ��Ϊ��Ļ����
	m_keymenu01.SetCenterPoint(pt);
	m_keymenu01.SetStop(false);
	return true;
}

bool CGame::GameInitOne2Bot()
{
	for ( ; ;)
	{
		// �����ͼ
		m_map.LoadMap();
		// ���һ
		{
			m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("����player01λ��ʧ��"));
			}
			else
			{
				m_player01.SetCenterPoint(ptCenter);
			}
		}
		// �����ˣ����Կ��Ƶ�̹�ˣ�
		{
			m_bot = CBot(0, 0, _T("tank_bot.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("����Botλ��ʧ��"));
			}
			else
			{
				// ���û����˵��յ�����
				m_bot.SetCenterPoint(ptCenter);
			}
		}
		// ����ӵ�����
		m_lstBullets.clear();

		// �ж��Ƿ�Ϸ�
		{
			// ��ȡ�����ˣ����һ���ڵ�λ��
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_bot, startX, startY) || m_map.FindObjPosition(m_player01, targetX, targetY))
			{
				AfxMessageBox(_T("��ȡ̹��λ�÷�������"));
				goto ___Init_End;
			}
			// �ж����һ�ͻ������Ƿ���ͨ������޷���ͨ���޷�������Ϸ
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
	// ����������ҵ�keymenuλ�ã���Ļ���м�
	RECT rc;
	GetWindowRect(m_hWnd, &rc);
	PointF pt;
	pt.X = rc.left + m_keymenu01.GetRect().Width / 2.0f + 100;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	// ���ò˵����λ��
	m_keymenu01.SetCenterPoint(pt);
	// ���ò�ֹͣ���Ŷ���
	m_keymenu01.SetStop(false);

	return true;
}

bool CGame::GameInitOne2One()
{
	for (;;) 
	{
		// �����ͼ
		m_map.LoadMap();
		{	
			// �������һ����
			m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
			PointF ptCenter;
			// ��������Ŀյ�λ��
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("����player01λ��ʧ��"));
			}
			else {
				// �������һ�����յ�����
				m_player01.SetCenterPoint(ptCenter);
			}
		}
		{
			// ������Ҷ�����
			m_player02 = CPlayer(0, 0, _T("tank_player2.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("����player02λ��ʧ��"));
			}
			else {
				m_player02.SetCenterPoint(ptCenter);
			}
		}
		m_lstBullets.clear();
		// �ж��Ƿ�Ϸ�
		{
			// ���һ����ˣ�������ڵ�λ��
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_player01, startX, startY) || 
				!m_map.FindObjPosition(m_player02, targetX, targetY))
			{
				AfxMessageBox(_T("��ȡ̹��λ�÷�������"));
			}
			// �ж���������Ƿ������ͨ
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
		// ����Mֹͣ����
		m_keymenu01.SetStop();
	}
	if (m_keymenu01.GetStop())
	{
		SetStep(EGameTypeOne2Bot);
	}
}

void CGame::GameRunLogicOnOne2Bot()
{
	// ״̬ά��
	// �Ƴ�������Ч�ӵ���������Ӧ��̹�������ӵ�
	RemoveTimeoutBullets();

	// ����ӵ��Ƿ����̹�ˣ����б�ը
	for (auto& blt : m_lstBullets)
	{
		if (m_bot.IsHitted(blt))
		{
			// �����˱�ը
			m_bot.Bomb();
			m_eStep = EGameTypeOne2BotEnd;
			blt.SetActive(false);
		}
		if (m_player01.IsHitted(blt))
		{
			// ��ұ�ը
			m_player01.Bomb();
			m_eStep = EGameTypeOne2BotEnd;
			blt.SetActive(false);
		}
		break;
	}
	// �ӵ��˶�ά��
	ProcessHitBullets();
	// ʹ�������Զ��������
	AI();
	// ��������
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
			// ̹��ײǽ���
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
			// ���¼�����I���������˲���
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_bot, startX, startY)||!m_map.FindObjPosition(m_player01, targetX, targetY))
			{
				return;
			}
			float fDirNext = 0; // ��������һ���ķ���
			if (!m_map.FindNextDirection(&fDirNext, startX, startY, targetX, targetY))
			{
				return;
			}
			
			// ��ȡ������
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
	//�����ܰ���

	// ״̬ά��
	// �Ƴ��б�����Ч���ӵ���������Ӧ̹�������ӵ�
	RemoveTimeoutBullets();

	// �ӵ�ײǽ�ı䷽��
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
	// ���һ
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
		// ̹��ײǽ���
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

	// ��Ҷ�
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
		// ̹��ײǽ���
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
		// ̹��ײǽ���
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
		// ����Q������
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
			AfxMessageBox(_T("���Դ�"));
		}
	}
	// �Ƴ��б�����Ч�ӵ�
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
	// ����Ҫ������
	// �Ƴ��б�����Ч���ӵ���������Ӧ��̹�������ӵ�
	RemoveTimeoutBullets();
	// �ӵ�ײǽ
	ProcessHitBullets();
}
