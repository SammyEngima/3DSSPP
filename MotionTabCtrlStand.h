#pragma once
#include "motiontabctrl.h"

class CMotionTabCtrlStand :	public CMotionTabCtrl {

public:
	CMotionTabCtrlStand(int initialFrame, Dg_MotionPrediction* parent);
	virtual ~CMotionTabCtrlStand(void);
};
