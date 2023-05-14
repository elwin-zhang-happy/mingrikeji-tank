#pragma once
#include "CGameEntry.h"


class CGameEntryMoveable : public CGameEntry
{
private:
	bool m_bActive {false}; // �Ƿ���Ч
	RectF m_rect{ 0, 0, 100, 100 }; // ռ�õ�����
	float m_fSpeed{ 10 }; // �ƶ��ٶ�
	float m_direction{ 0 }; // ��ǰ�ĽǶ�
	float m_directionTurn{ 5 }; // ÿ����ת�ĽǶ�

public:
	CGameEntryMoveable();
	virtual ~CGameEntryMoveable();
	CGameEntryMoveable(const CGameEntryMoveable& cgameEntryMoveable);
	CGameEntryMoveable& operator=(const CGameEntryMoveable & rhs);

public:
	virtual void Draw(Gdiplus::Graphics& gh) = 0;
/***********************************���ƶ����干��***********************************/
public: 
	// ��ת
	virtual void RotateRight();
	// ��ת
	virtual void RotateLeft();
	// ǰ��
	virtual void Forward();
	// ����
	virtual void Backward();
	// ��ȡˮƽ�����ϵ��ٶȷ���
	virtual float GetXSpeed() const;
	// ��ȡ��ֱ�����ϵ��ٶȷ���
	virtual float GetYSpeed() const;
	// ��һ����ǰ������λ��
	virtual RectF ForwardNextRect() const;
	// ��һ�������ˣ���λ��
	virtual RectF BackwardNextRect() const;
	// ��ȡͷ��λ�ã�ͼƬ���Σ�ͷ��λ��Ϊ�Ƕ�Ϊ0
	virtual PointF GetHeadPos() const;

	// ���ýǶ�
	virtual void SetDirectionArc(float dir);
	// ��ȡ�Ƕ�
	virtual float GetDirectionArc() const;
	// ��ȡ����λ��
	virtual PointF GetCenterPoint() const;
	// ��������λ��
	virtual void SetCenterPoint(const PointF & ptCenter);
	// ���ýǶ�
	virtual void SetDirection(const float dir);
	// ��ȡ�Ƕ�
	virtual float GetDirection() const;
	// ����ÿ����ת�ĽǶ�
	virtual void SetDirectionTurnArc(const float rh);
	// ��ȡ��ǰ�ĽǶȣ���λPI��
	virtual float GetDirectionTurnArc() const;
	// ����ÿ����ת�Ƕ�
	virtual float SetDirectionTurn(const float rh);
	// ��ȡ��ǰ��ת�Ƕ�
	virtual float GetDirectionTurn() const;
	// �Ƿ���Ч
	virtual bool IsActive() const;
	// ������Ч
	virtual void SetActive(bool bActive);
	// ռ�÷�Χ
	virtual void SetRect(const RectF rect);
	// ��ȡռ�÷�Χ
	virtual RectF GetRect() const;
	// �����ٶ�
	virtual void SetSpeed(const float speed);
	// ��ȡ�ٶ�
	virtual float GetSpeed() const;
};

