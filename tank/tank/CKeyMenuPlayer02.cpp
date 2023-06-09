#include "pch.h"
#include "CKeyMenuPlayer02.h"
#include "CGame.h"

CKeyMenuPlayer02::CKeyMenuPlayer02()
{
    m_img[0] = Image::FromFile(_T("menu_key_player02_0.png"));
    m_img[1] = Image::FromFile(_T("menu_key_player02_1.png"));
    if(m_img[0]) {
        RectF rc = {800 - 100, 600 - 40
                    , static_cast<float>(m_img[0]->GetWidth())
                    , static_cast<float>(m_img[0]->GetHeight())
                   };
        SetRect(rc);
    }
}

CKeyMenuPlayer02::~CKeyMenuPlayer02()
{

}

void CKeyMenuPlayer02::Draw(Graphics &gh)
{
    if(m_bStop) {
        gh.DrawImage(m_img[0], GetRect());
        return;
    }
    static bool bFlag{ false };
    if(bFlag) {
        gh.DrawImage(m_img[0], GetRect());
    }
    else {
        gh.DrawImage(m_img[1], GetRect());
    }
    if(m_time.IsTimeval()) {
        bFlag = !bFlag;
    }
}
