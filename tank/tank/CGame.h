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
		EGameTypeMenu = 0, // 选择阶段
		EGameTypeOne2BotMenu, // 人机大战菜单阶段
 		EGameTypeOne2Bot, // 人机大战
		EGameTypeOne2BotEnd, // 人机大战结束阶段
		EGameTypeOne2OneMenu, // 双人大战菜单阶段
		EGameTypeOne2One, // 双人大战
		EGameTypeOne2OneEnd, // 双人大战结束
		EGameTypeCount, // 有7个阶段
	};

	void SetStep(CGame::EGameType step);

public:
	
	/*
	游戏初始化
	生成游戏对象，初始化地图，对象位置
	*/
	bool GameInit();
	// 游戏初始化：选择阶段
	bool GameInitMenu();
	// 游戏初始化：人机大战菜单阶段
	bool GameInitOne2BotMenu();
	// 游戏初始化：人机大战
	bool GameInitOne2Bot();
	// 游戏初始化：人机大战结束
	bool GameInitOne2BotEnd();
	// 游戏初始化：双人大战菜单阶段
	bool GameInitOne2OneMenu();
	// 游戏初始化：双人大战
	bool GameInitOne2One();
	// 游戏初始化：双人大战结束
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
	* 游戏逻辑处理
	* 1. 维护子弹状态
	* 2. 维护机器人自动移动
	* 3. 维护玩家坦克状态
	* 碰撞检测
	*/
	void GameRunLogic();
	// 游戏逻辑处理： 选择阶段
	void GameRunLogicOnMenu();
	// 游戏逻辑处理： 人机大战菜单阶段
	void GameRunLogicOnOne2BotMenu();
	// 游戏逻辑处理： 人机大战阶段
	void GameRunLogicOnOne2Bot();
	// 游戏逻辑处理： 人机大战结束阶段
	void GameRunLogicOnOne2BotEnd();
	// 游戏逻辑处理： 双人大战菜单阶段
	void GameRunLogicOnOne2OneMenu();
	// 游戏逻辑处理： 双人大战
	void GameRunLogicOnOne2One();
	// 游戏逻辑处理： 双人大战结束阶段
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
	* 游戏绘图处理
	* 负责绘画游戏中的对象
	*/
	void GameRunDraw();
	// 游戏绘图处理： 选择阶段
	void GameRunDrawOnMenu(Graphics &gh);
	// 游戏绘图处理： 人机大战菜单阶段
	void GameRunDrawOnOne2BotMenu(Graphics& gh);
	// 游戏绘图处理： 人机大战阶段
	void GameRunDrawOnOne2Bot(Graphics& gh);
	// 游戏绘图处理： 人机大战结束阶段
	void GameRunDrawOnOne2BotEnd(Graphics& gh);
	// 游戏绘图处理： 双人大战菜单阶段
	void GameRunDrawOnOne2OneMenu(Graphics& gh);
	// 游戏绘图处理： 双人大战
	void GameRunDrawOnOne2One(Graphics& gh);
	// 游戏绘图处理： 双人大战结束阶段
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

	// 窗口
	HWND m_hWnd;
	// 移除超时子弹并给对应的坦克装弹
	void RemoveTimeoutBullets();
	// 维护子弹的运行及撞墙处理
	void ProcessHitBullets();
	// 维护电脑的自动寻路攻击
	void AI();
	// 输出fps
	void DrawFps(Graphics &gh);


public:
	// 记录游戏每秒多少帧
	int m_fps{ 0 };
	// 初始阶段： 菜单选择阶段
	EGameType m_eStep{ EGameTypeMenu };
	// 两个玩家
	CPlayer m_player01{ 100, 100, _T("tank_player1.png") };
	// 玩家对象2：双人大战时才会用到
	CPlayer m_player02{ 200, 100, _T("tank_player2.png") };
	// 机器人：人机大战
	CBot m_bot{ 300, 200, _T("tank_bot.png") };
	// 存在地图场景中的子弹对象数组
	list<CBullet> m_lstBullets;
	// 地图对象
	CGameMap m_map{ 10, 10, 790, 580 };
	// 开始菜单
	CGameMenuPanel m_menuSelect;
	// 开始菜单背景图
	CGameMenuBackground m_menu;
	// 返回菜单
	CGameMenuPanel2 m_menuBackup;
	// 提示按键的菜单
	CKeyMenuPlayer01 m_keymenu01;
	CKeyMenuPlayer02 m_keymenu02;

};
