#pragma once
#include "CGameEntryMoveable.h"

using namespace std;

class CGame;

class CGameMenuPanel : public CGameEntryMoveable
{
public: 
    CGameMenuPanel();
    ~CGameMenuPanel();
    // ���CGame��ָ��
    CGame * m_pParent{ nullptr };
    // ��¼���ڷ�Χ
    RectF m_rect{ 0, 0, 800, 600 };
    // ����ͼƬ
    Image* m_imgBackground{ nullptr };
    // �˵���
    struct 
    {
        vector<tuple<Image*, RectF>> vImgInfoPtr;
        int index;
    } m_menuItems[2];

    // ���CGameָ���ı���
    virtual void SetParent(CGame * g);
    // ���˵�
    virtual void Draw(Gdiplus::Graphics& gh);
    // ��������ƶ��¼�
    void OnMouseMove(UINT nFlags, CPoint point);
    // ����������̧���¼�
    void OnButtonUp(UINT nFlags, CPoint point);
};

