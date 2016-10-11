// Dg_Parameters.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
//#include "hom_doc.h"	// Unnecessary!
#include "Dg_Parameters.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int NUM_PARAMETERS = 20;

/////////////////////////////////////////////////////////////////////////////
// Dg_Parameters dialog


Dg_Parameters::Dg_Parameters(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_Parameters::IDD, pParent)
{
	//{{AFX_DATA_INIT(Dg_Parameters)
	mParameterSetName = _T("");
	//}}AFX_DATA_INIT
}


void Dg_Parameters::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_Parameters)
	DDX_Text(pDX, IDC_ParameterSetName, mParameterSetName);
	//}}AFX_DATA_MAP
	for(int i = 0; i < NUM_PARAMETERS; ++i)
	{
		mParameters[i] = Round_Double(mParameters[i],1);
	}
	DDX_Text(pDX, IDC_P0,  mParameters[0] );
	DDX_Text(pDX, IDC_P1,  mParameters[1] );
	DDX_Text(pDX, IDC_P2,  mParameters[2] );
	DDX_Text(pDX, IDC_P3,  mParameters[3] );
	DDX_Text(pDX, IDC_P4,  mParameters[4] );
	DDX_Text(pDX, IDC_P5,  mParameters[5] );
	DDX_Text(pDX, IDC_P6,  mParameters[6] );
	DDX_Text(pDX, IDC_P7,  mParameters[7] );
	DDX_Text(pDX, IDC_P8,  mParameters[8] );
	DDX_Text(pDX, IDC_P9,  mParameters[9] );
	DDX_Text(pDX, IDC_P10, mParameters[10] );
	DDX_Text(pDX, IDC_P11, mParameters[11] );
	DDX_Text(pDX, IDC_P12, mParameters[12] );
	DDX_Text(pDX, IDC_P13, mParameters[13] );
	DDX_Text(pDX, IDC_P14, mParameters[14] );
	DDX_Text(pDX, IDC_P15, mParameters[15] );
	DDX_Text(pDX, IDC_P16, mParameters[16] );
	DDX_Text(pDX, IDC_P17, mParameters[17] );
	DDX_Text(pDX, IDC_P18, mParameters[18] );
	DDX_Text(pDX, IDC_P19, mParameters[19] );

	return;
	
}


BEGIN_MESSAGE_MAP(Dg_Parameters, CDialog)
	//{{AFX_MSG_MAP(Dg_Parameters)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_Parameters message handlers


