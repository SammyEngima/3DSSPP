#pragma once


// Dg_FactorsROM dialog

class Dg_FactorsROM : public CDialog
{
	DECLARE_DYNAMIC(Dg_FactorsROM)

public:
	Dg_FactorsROM(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dg_FactorsROM();

	void SetFactors(const Factors& in_Factors) {
		mFactors = in_Factors;
	}

	Factors GetFactors() const {
		return mFactors;
	}

// Dialog Data
	enum { IDD = IDD_FACTORS_ROM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	Factors mFactors;
	int ForZeroValues;

	DECLARE_MESSAGE_MAP()
};
