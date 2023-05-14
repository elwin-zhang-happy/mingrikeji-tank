
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "tank.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define GAME_WIN_W  (800)
#define GAME_WIN_H (600)
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
#define MY_STYLE (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | FWS_ADDTOTITLE)
	// 创建窗口
	Create(nullptr, _T("明日科技.坦克大战"), MY_STYLE, CRect(0,0,GAME_WIN_W,GAME_WIN_H));
	// 设置客户区大小
	{
		CRect rcCli;
		// 获取客户区大小
		GetClientRect(rcCli);

		// 计算边框的大小，并设置
		RECT rcFrame = {
			0, 0 ,
			m_iWidth + m_iWidth - rcCli.right,
			m_iHeight + m_iHeight - rcCli.bottom
		};
		// 调用windowsapi设置窗口位置和大小
		MoveWindow(&rcFrame, TRUE);
	}
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 启动定时器每次都会进入游戏帧
	SetTimer(ETimerIdGameLoop, 0 , nullptr);
	m_game.SetHandle(GetSafeHwnd());
	return 0;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case ETimerIdGameLoop:
		// 本次时刻 long型
		static DWORD dwLastUpdate = GetTickCount();
		// 判断时间间隔
		if (GetTickCount() - dwLastUpdate >= 20) 
		{
			// 游戏帧处理
			m_game.EnterFrame(GetTickCount());
			// 记录时间间隔
			dwLastUpdate = GetTickCount();
		}	
		break;
	}

	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	m_game.OnMouseMove(nFlags, point);
	CFrameWnd::OnMouseMove(nFlags, point);
}

void CMainFrame::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_game.OnLButtonUp(nFlags, point);
	CFrameWnd::OnLButtonUp(nFlags, point);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG
