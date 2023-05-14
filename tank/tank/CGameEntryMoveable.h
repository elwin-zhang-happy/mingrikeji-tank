#pragma once
#include "CGameEntry.h"


class CGameEntryMoveable : public CGameEntry
{
private:
	bool m_bActive {false}; // 是否有效
	RectF m_rect{ 0, 0, 100, 100 }; // 占用的区域
	float m_fSpeed{ 10 }; // 移动速度
	float m_direction{ 0 }; // 当前的角度
	float m_directionTurn{ 5 }; // 每次旋转的角度

public:
	CGameEntryMoveable();
	virtual ~CGameEntryMoveable();
	CGameEntryMoveable(const CGameEntryMoveable& cgameEntryMoveable);
	CGameEntryMoveable& operator=(const CGameEntryMoveable & rhs);

public:
	virtual void Draw(Gdiplus::Graphics& gh) = 0;
/***********************************可移动物体共性***********************************/
public: 
	// 右转
	virtual void RotateRight();
	// 左转
	virtual void RotateLeft();
	// 前移
	virtual void Forward();
	// 后移
	virtual void Backward();
	// 获取水平方向上的速度分量
	virtual float GetXSpeed() const;
	// 获取竖直方向上的速度分量
	virtual float GetYSpeed() const;
	// 下一步（前进）的位置
	virtual RectF ForwardNextRect() const;
	// 下一步（后退）的位置
	virtual RectF BackwardNextRect() const;
	// 获取头部位置：图片矩形，头部位置为角度为0
	virtual PointF GetHeadPos() const;

	// 设置角度
	virtual void SetDirectionArc(float dir);
	// 获取角度
	virtual float GetDirectionArc() const;
	// 获取中心位置
	virtual PointF GetCenterPoint() const;
	// 设置中心位置
	virtual void SetCenterPoint(const PointF & ptCenter);
	// 设置角度
	virtual void SetDirection(const float dir);
	// 获取角度
	virtual float GetDirection() const;
	// 设置每次旋转的角度
	virtual void SetDirectionTurnArc(const float rh);
	// 获取当前的角度（单位PI）
	virtual float GetDirectionTurnArc() const;
	// 设置每次旋转角度
	virtual float SetDirectionTurn(const float rh);
	// 获取当前旋转角度
	virtual float GetDirectionTurn() const;
	// 是否有效
	virtual bool IsActive() const;
	// 设置有效
	virtual void SetActive(bool bActive);
	// 占用范围
	virtual void SetRect(const RectF rect);
	// 获取占用范围
	virtual RectF GetRect() const;
	// 设置速度
	virtual void SetSpeed(const float speed);
	// 获取速度
	virtual float GetSpeed() const;
};

