#pragma once
#include "motiontabctrl.h"

class CMotionTabCtrlSit : public CMotionTabCtrl {

public:
	CMotionTabCtrlSit(int initialFrame, Dg_MotionPrediction* parent);
	virtual ~CMotionTabCtrlSit(void);
};
