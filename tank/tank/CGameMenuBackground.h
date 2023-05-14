#pragma once
#include "CGameEntryMoveable.h"


class CGame;

class CGameMenuBackground : public CGameEntryMoveable
{
public:
    CGameMenuBackground();
    ~CGameMenuBackground();

    CGame* m_pParent{ nullptr };
    RectF m_rect{0, 0, 800, 600};
    Image* m_imgBackground;

    void SetParent(CGame * g);
    virtual void Draw(Gdiplus::Graphics& gh);

};

