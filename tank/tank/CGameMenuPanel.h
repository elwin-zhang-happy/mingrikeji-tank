#pragma once
#include "CGameEntryMoveable.h"

using namespace std;

class CGame;

class CGameMenuPanel : public CGameEntryMoveable
{
public: 
    CGameMenuPanel();
    ~CGameMenuPanel();
    // 存放CGame的指针
    CGame * m_pParent{ nullptr };
    // 记录窗口范围
    RectF m_rect{ 0, 0, 800, 600 };
    // 背景图片
    Image* m_imgBackground{ nullptr };
    // 菜单项
    struct 
    {
        vector<tuple<Image*, RectF>> vImgInfoPtr;
        int index;
    } m_menuItems[2];

    // 存放CGame指针文本中
    virtual void SetParent(CGame * g);
    // 画菜单
    virtual void Draw(Gdiplus::Graphics& gh);
    // 处理鼠标移动事件
    void OnMouseMove(UINT nFlags, CPoint point);
    // 处理鼠标左键抬起事件
    void OnButtonUp(UINT nFlags, CPoint point);
};

