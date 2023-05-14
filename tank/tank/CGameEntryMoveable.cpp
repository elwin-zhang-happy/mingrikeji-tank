#include "pch.h"
#include "CGameEntryMoveable.h"

CGameEntryMoveable::CGameEntryMoveable()
{
}

CGameEntryMoveable::~CGameEntryMoveable()
{
}

CGameEntryMoveable::CGameEntryMoveable(const CGameEntryMoveable & cgameEntryMoveable)
{
	if (this != &cgameEntryMoveable)
	{
		this->m_bActive = cgameEntryMoveable.m_bActive;
		this->m_rect = cgameEntryMoveable.m_rect;
		this->m_fSpeed = cgameEntryMoveable.m_fSpeed;
		this->m_direction = cgameEntryMoveable.m_direction;
		this->m_directionTurn = cgameEntryMoveable.m_directionTurn;
	}
}

CGameEntryMoveable& CGameEntryMoveable::operator=(const CGameEntryMoveable& rhs)
{
	if (this != &rhs)
	{
		this->m_bActive = rhs.m_bActive;
		this->m_rect = rhs.m_rect;
		this->m_fSpeed = rhs.m_fSpeed;
		this->m_direction = rhs.m_direction;
		this->m_directionTurn = rhs.m_directionTurn;
	}
	return *this;
}

void CGameEntryMoveable::RotateRight()
{
	m_direction += m_directionTurn;
	if (m_direction >= 360)
	{
		m_direction -= 360;
	}
}

void CGameEntryMoveable::RotateLeft()
{
	m_direction -= m_directionTurn;
	if (m_direction < 0)
	{
		m_direction = static_cast<float>(360 - m_directionTurn);
	}
}

void CGameEntryMoveable::Forward()
{
	m_rect.X += static_cast<Gdiplus::REAL>((m_fSpeed) * sin(GetDirectionArc()));
	m_rect.Y -= static_cast<Gdiplus::REAL>((m_fSpeed) * cos(GetDirectionArc()));
}

void CGameEntryMoveable::Backward()
{
	m_rect.X -= static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
	m_rect.Y += static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
}


float CGameEntryMoveable::GetXSpeed() const
{
	return static_cast<float>(m_fSpeed * sin(GetDirectionArc()));
}

float CGameEntryMoveable::GetYSpeed() const
{
	return static_cast<float>(m_fSpeed * cos(GetDirectionArc()));
}

RectF CGameEntryMoveable::ForwardNextRect() const
{
	RectF rc = m_rect;
	rc.X += static_cast<Gdiplus::REAL>(m_fSpeed * sin(GetDirectionArc()));
	rc.Y -= static_cast<Gdiplus::REAL>(m_fSpeed * cos(GetDirectionArc()));
	return rc;
}

RectF CGameEntryMoveable::BackwardNextRect() const
{
	RectF rc = m_rect;
	rc.X -= static_cast<Gdiplus::REAL>(m_fSpeed * sin(GetDirectionArc()));
	rc.Y += static_cast<Gdiplus::REAL>(m_fSpeed * cos(GetDirectionArc()));
	return rc;
}

PointF CGameEntryMoveable::GetHeadPos() const
{
	PointF ptCenter = GetCenterPoint();
	PointF ptHead = ptCenter;
	// 计算头部所在位置
	float fRadius = std::sqrt(std::pow(m_rect.Width / 2 ,2) + std::pow(m_rect.Height / 2, 2));

	ptHead.X += fRadius * sin(GetDirectionArc());
	ptHead.Y -= fRadius * cos(GetDirectionArc());
	return ptHead;
}


void CGameEntryMoveable::SetDirectionArc(float dir)
{
	m_direction = dir * 180.0f / PI;
}

float CGameEntryMoveable::GetDirectionArc() const
{
	return PI * m_direction / 180.9f;
}


PointF CGameEntryMoveable::GetCenterPoint() const
{
	PointF  center = PointF(m_rect.X + m_rect.Width / 2, m_rect.Y + m_rect.Height / 2);
	return center;
}

void CGameEntryMoveable::SetCenterPoint(const PointF& ptCenter)
{
	PointF center = GetCenterPoint();
	center.X = ptCenter.X - center.X;
	center.Y = ptCenter.Y - center.Y;
	// 平移
	m_rect.Offset(center);
}

void CGameEntryMoveable::SetDirection(const float dir)
{
	m_direction = dir;
}

float CGameEntryMoveable::GetDirection() const
{
	return m_direction;
}

void CGameEntryMoveable::SetDirectionTurnArc(const float rh)
{
	m_directionTurn = PI * rh / 180.0f;
}

float CGameEntryMoveable::GetDirectionTurnArc() const
{
	return PI * m_directionTurn;
}

float CGameEntryMoveable::SetDirectionTurn(const float rh)
{
	return m_directionTurn = rh;
}

float CGameEntryMoveable::GetDirectionTurn() const
{
	return m_directionTurn;
}

bool CGameEntryMoveable::IsActive() const
{
	return m_bActive;
}

void CGameEntryMoveable::SetActive(bool bActive)
{
	m_bActive = bActive;
}

void CGameEntryMoveable::SetRect(const RectF rect)
{
	m_rect = rect;
}

RectF CGameEntryMoveable::GetRect() const
{
	return m_rect;;
}

void CGameEntryMoveable::SetSpeed(const float speed)
{
	m_fSpeed = speed;
}

float CGameEntryMoveable::GetSpeed() const
{
	return m_fSpeed;
}
