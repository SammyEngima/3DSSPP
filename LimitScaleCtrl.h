#pragma once

//A structure used to define the range of one of the limits. Start is the 
//starting value and end is the ending value of the range. These numbers
//are out of a maximum value set by the SetMaximum() function.
struct range
{
    int start;
    int end;
};

// CLimitScaleCtrl

class CLimitScaleCtrl : public CSliderCtrl
{
	DECLARE_DYNAMIC(CLimitScaleCtrl)

public:
	CLimitScaleCtrl();
	virtual ~CLimitScaleCtrl();
    afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);

    //Mutator functions
    void SetMaximum(int max);               //Set the maximum value
    void SetULRange(int lower, int upper);  //Set upper limit range
    void SetDLRange(int lower, int upper);  //Set design limit range
    void SetOKRange(int lower, int upper);  //Set acceptable range
    void SetAllRanges(int ULLower, int ULUpper, int DLLower, int DLUpper, int OKLower, int OKUpper); //Set all ranges
    void SetLimitsText(CString upper, CString design); //Set text to describe limits

protected:
	DECLARE_MESSAGE_MAP()

//Class Variables
private:
    void DrawChannel(LPNMCUSTOMDRAW pNMCD);     //Draw the channel
    void DrawThumb(LPNMCUSTOMDRAW pNMCD);       //Draw the thumb
    void Refresh();                             //A hack to force the window to update

    COLORREF mUpperLimitClr;    //Color for the upper limit (red)
    COLORREF mDesignLimitClr;   //Color for the design limit (yellow)
    COLORREF mAcceptableClr;    //Color for acceptable (green)

    int   maximum;              //The maximum value of a range
    range upperLimit;           //The range for the upper limit
    range designLimit;          //The range for the design limit
    range acceptable;           //The range for the acceptable area
    
    CString upperLimitText;     //Text values to label upper limit region
    CString designLimitText;    //Text values to label design limit region
};


