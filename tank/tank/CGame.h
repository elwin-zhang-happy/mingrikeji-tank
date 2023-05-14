#pragma once
#include "CGameMenuPanel.h"
#include "CGameMenuPanel2.h"
#include "CGameMenuBackground.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CBot.h"
#include "CGameMap.h"
#include "CWall.h"
#include "CKeyMenuPlayer01.h"
#include "CKeyMenuPlayer02.h"


class CGame
{
public:
	CGame();
	~CGame();

public:
	void SetHandle(HWND hWnd);
	bool EnterFrame(DWORD dwTime);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);

	enum EGameType
	{
		EGameTypeMenu = 0, // ѡ��׶�
		EGameTypeOne2BotMenu, // �˻���ս�˵��׶�
 		EGameTypeOne2Bot, // �˻���ս
		EGameTypeOne2BotEnd, // �˻���ս�����׶�
		EGameTypeOne2OneMenu, // ˫�˴�ս�˵��׶�
		EGameTypeOne2One, // ˫�˴�ս
		EGameTypeOne2OneEnd, // ˫�˴�ս����
		EGameTypeCount, // ��7���׶�
	};

	void SetStep(CGame::EGameType step);

public:
	
	/*
	��Ϸ��ʼ��
	������Ϸ���󣬳�ʼ����ͼ������λ��
	*/
	bool GameInit();
	// ��Ϸ��ʼ����ѡ��׶�
	bool GameInitMenu();
	// ��Ϸ��ʼ�����˻���ս�˵��׶�
	bool GameInitOne2BotMenu();
	// ��Ϸ��ʼ�����˻���ս
	bool GameInitOne2Bot();
	// ��Ϸ��ʼ�����˻���ս����
	bool GameInitOne2BotEnd();
	// ��Ϸ��ʼ����˫�˴�ս�˵��׶�
	bool GameInitOne2OneMenu();
	// ��Ϸ��ʼ����˫�˴�ս
	bool GameInitOne2One();
	// ��Ϸ��ʼ����˫�˴�ս����
	bool GameInitOne2OneEnd();

	bool(CGame::*m_initFunc[EGameTypeCount])() = {
		&CGame::GameInitMenu,
		&CGame::GameInitOne2BotMenu,
		&CGame::GameInitOne2Bot,
		&CGame::GameInitOne2BotEnd,
		&CGame::GameInitOne2OneMenu,
		&CGame::GameInitOne2One,
		&CGame::GameInitOne2OneEnd,
	};

	
public: 
	/**
	* ��Ϸ�߼�����
	* 1. ά���ӵ�״̬
	* 2. ά���������Զ��ƶ�
	* 3. ά�����̹��״̬
	* ��ײ���
	*/
	void GameRunLogic();
	// ��Ϸ�߼����� ѡ��׶�
	void GameRunLogicOnMenu();
	// ��Ϸ�߼����� �˻���ս�˵��׶�
	void GameRunLogicOnOne2BotMenu();
	// ��Ϸ�߼����� �˻���ս�׶�
	void GameRunLogicOnOne2Bot();
	// ��Ϸ�߼����� �˻���ս�����׶�
	void GameRunLogicOnOne2BotEnd();
	// ��Ϸ�߼����� ˫�˴�ս�˵��׶�
	void GameRunLogicOnOne2OneMenu();
	// ��Ϸ�߼����� ˫�˴�ս
	void GameRunLogicOnOne2One();
	// ��Ϸ�߼����� ˫�˴�ս�����׶�
	void GameRunLogicOnOne2OneEnd();

	void (CGame::* m_logicFunc[EGameTypeCount])() = {
		& CGame::GameRunLogicOnMenu,
		& CGame::GameRunLogicOnOne2BotMenu,
		& CGame::GameRunLogicOnOne2Bot,
		& CGame::GameRunLogicOnOne2BotEnd,
		& CGame::GameRunLogicOnOne2OneMenu,
		& CGame::GameRunLogicOnOne2One,
		& CGame::GameRunLogicOnOne2OneEnd,
	};


public:
	/**
	* ��Ϸ��ͼ����
	* ����滭��Ϸ�еĶ���
	*/
	void GameRunDraw();
	// ��Ϸ��ͼ���� ѡ��׶�
	void GameRunDrawOnMenu(Graphics &gh);
	// ��Ϸ��ͼ���� �˻���ս�˵��׶�
	void GameRunDrawOnOne2BotMenu(Graphics& gh);
	// ��Ϸ��ͼ���� �˻���ս�׶�
	void GameRunDrawOnOne2Bot(Graphics& gh);
	// ��Ϸ��ͼ���� �˻���ս�����׶�
	void GameRunDrawOnOne2BotEnd(Graphics& gh);
	// ��Ϸ��ͼ���� ˫�˴�ս�˵��׶�
	void GameRunDrawOnOne2OneMenu(Graphics& gh);
	// ��Ϸ��ͼ���� ˫�˴�ս
	void GameRunDrawOnOne2One(Graphics& gh);
	// ��Ϸ��ͼ���� ˫�˴�ս�����׶�
	void GameRunDrawOnOne2OneEnd(Graphics& gh);

	void (CGame::* m_drawFunc[EGameTypeCount])(Graphics& gh) = {
		&CGame::GameRunDrawOnMenu,
		&CGame::GameRunDrawOnOne2BotMenu,
		&CGame::GameRunDrawOnOne2Bot,
		&CGame::GameRunDrawOnOne2BotEnd,
		&CGame::GameRunDrawOnOne2OneMenu,
		&CGame::GameRunDrawOnOne2One,
		&CGame::GameRunDrawOnOne2OneEnd,
	};

private:

	// ����
	HWND m_hWnd;
	// �Ƴ���ʱ�ӵ�������Ӧ��̹��װ��
	void RemoveTimeoutBullets();
	// ά���ӵ������м�ײǽ����
	void ProcessHitBullets();
	// ά�����Ե��Զ�Ѱ·����
	void AI();
	// ���fps
	void DrawFps(Graphics &gh);


public:
	// ��¼��Ϸÿ�����֡
	int m_fps{ 0 };
	// ��ʼ�׶Σ� �˵�ѡ��׶�
	EGameType m_eStep{ EGameTypeMenu };
	// �������
	CPlayer m_player01{ 100, 100, _T("tank_player1.png") };
	// ��Ҷ���2��˫�˴�սʱ�Ż��õ�
	CPlayer m_player02{ 200, 100, _T("tank_player2.png") };
	// �����ˣ��˻���ս
	CBot m_bot{ 300, 200, _T("tank_bot.png") };
	// ���ڵ�ͼ�����е��ӵ���������
	list<CBullet> m_lstBullets;
	// ��ͼ����
	CGameMap m_map{ 10, 10, 790, 580 };
	// ��ʼ�˵�
	CGameMenuPanel m_menuSelect;
	// ��ʼ�˵�����ͼ
	CGameMenuBackground m_menu;
	// ���ز˵�
	CGameMenuPanel2 m_menuBackup;
	// ��ʾ�����Ĳ˵�
	CKeyMenuPlayer01 m_keymenu01;
	CKeyMenuPlayer02 m_keymenu02;

};
