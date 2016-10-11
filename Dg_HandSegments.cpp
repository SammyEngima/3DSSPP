#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include "UndoPosture.h"

#include "Dg_HandSegments.h"

Dg_HandSegments::Dg_HandSegments(C_Hom_Doc *aDocPtr, CWnd *pParent)
	:	CDialog(Dg_HandSegments::IDD, pParent),
		mDocPtr(aDocPtr)
{
	L1CA_V=0; L1CA_H=0; L1MC_V=0; L1MC_H=0; L1PP_V=0; L1PP_H=0; L1DP_V=0; L1DP_H=0;
	L2CM_V=0; L2CM_H=0; L2PP_V=0; L2PP_H=0; L2MP_V=0; L2MP_H=0; L2DP_V=0; L2DP_H=0;
	L3CM_V=0; L3CM_H=0; L3PP_V=0; L3PP_H=0; L3MP_V=0; L3MP_H=0; L3DP_V=0; L3DP_H=0;
	L4CM_V=0; L4CM_H=0; L4PP_V=0; L4PP_H=0; L4MP_V=0; L4MP_H=0; L4DP_V=0; L4DP_H=0;
	L5CM_V=0; L5CM_H=0; L5PP_V=0; L5PP_H=0; L5MP_V=0; L5MP_H=0; L5DP_V=0; L5DP_H=0;

	R1CA_V=0; R1CA_H=0; R1MC_V=0; R1MC_H=0; R1PP_V=0; R1PP_H=0; R1DP_V=0; R1DP_H=0;
	R2CM_V=0; R2CM_H=0; R2PP_V=0; R2PP_H=0; R2MP_V=0; R2MP_H=0; R2DP_V=0; R2DP_H=0;
	R3CM_V=0; R3CM_H=0; R3PP_V=0; R3PP_H=0; R3MP_V=0; R3MP_H=0; R3DP_V=0; R3DP_H=0;
	R4CM_V=0; R4CM_H=0; R4PP_V=0; R4PP_H=0; R4MP_V=0; R4MP_H=0; R4DP_V=0; R4DP_H=0;
	R5CM_V=0; R5CM_H=0; R5PP_V=0; R5PP_H=0; R5MP_V=0; R5MP_H=0; R5DP_V=0; R5DP_H=0;
	
	this->GetCurrentJointAnglesFromDocument();
}

void Dg_HandSegments::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_L1CA_V, L1CA_V);	DDX_Text(pDX, IDC_L1CA_H, L1CA_H);
	DDX_Text(pDX, IDC_L1MC_V, L1MC_V);	DDX_Text(pDX, IDC_L1MC_H, L1MC_H);
	DDX_Text(pDX, IDC_L1PP_V, L1PP_V);	DDX_Text(pDX, IDC_L1PP_H, L1PP_H);
	DDX_Text(pDX, IDC_L1DP_V, L1DP_V);	DDX_Text(pDX, IDC_L1DP_H, L1DP_H);
	DDX_Text(pDX, IDC_L2CM_V, L2CM_V);	DDX_Text(pDX, IDC_L2CM_H, L2CM_H);
	DDX_Text(pDX, IDC_L2PP_V, L2PP_V);	DDX_Text(pDX, IDC_L2PP_H, L2PP_H);
	DDX_Text(pDX, IDC_L2MP_V, L2MP_V);	DDX_Text(pDX, IDC_L2MP_H, L2MP_H);
	DDX_Text(pDX, IDC_L2DP_V, L2DP_V);	DDX_Text(pDX, IDC_L2DP_H, L2DP_H);
	DDX_Text(pDX, IDC_L3CM_V, L3CM_V);	DDX_Text(pDX, IDC_L3CM_H, L3CM_H);
	DDX_Text(pDX, IDC_L3PP_V, L3PP_V);	DDX_Text(pDX, IDC_L3PP_H, L3PP_H);
	DDX_Text(pDX, IDC_L3MP_V, L3MP_V);	DDX_Text(pDX, IDC_L3MP_H, L3MP_H);
	DDX_Text(pDX, IDC_L3DP_V, L3DP_V);	DDX_Text(pDX, IDC_L3DP_H, L3DP_H);
	DDX_Text(pDX, IDC_L4CM_V, L4CM_V);	DDX_Text(pDX, IDC_L4CM_H, L4CM_H);
	DDX_Text(pDX, IDC_L4PP_V, L4PP_V);	DDX_Text(pDX, IDC_L4PP_H, L4PP_H);
	DDX_Text(pDX, IDC_L4MP_V, L4MP_V);	DDX_Text(pDX, IDC_L4MP_H, L4MP_H);
	DDX_Text(pDX, IDC_L4DP_V, L4DP_V);	DDX_Text(pDX, IDC_L4DP_H, L4DP_H);
	DDX_Text(pDX, IDC_L5CM_V, L5CM_V);	DDX_Text(pDX, IDC_L5CM_H, L5CM_H);
	DDX_Text(pDX, IDC_L5PP_V, L5PP_V);	DDX_Text(pDX, IDC_L5PP_H, L5PP_H);
	DDX_Text(pDX, IDC_L5MP_V, L5MP_V);	DDX_Text(pDX, IDC_L5MP_H, L5MP_H);
	DDX_Text(pDX, IDC_L5DP_V, L5DP_V);	DDX_Text(pDX, IDC_L5DP_H, L5DP_H);
	DDX_Text(pDX, IDC_R1CA_V, R1CA_V);	DDX_Text(pDX, IDC_R1CA_H, R1CA_H);
	DDX_Text(pDX, IDC_R1MC_V, R1MC_V);	DDX_Text(pDX, IDC_R1MC_H, R1MC_H);
	DDX_Text(pDX, IDC_R1PP_V, R1PP_V);	DDX_Text(pDX, IDC_R1PP_H, R1PP_H);
	DDX_Text(pDX, IDC_R1DP_V, R1DP_V);	DDX_Text(pDX, IDC_R1DP_H, R1DP_H);
	DDX_Text(pDX, IDC_R2CM_V, R2CM_V);	DDX_Text(pDX, IDC_R2CM_H, R2CM_H);
	DDX_Text(pDX, IDC_R2PP_V, R2PP_V);	DDX_Text(pDX, IDC_R2PP_H, R2PP_H);
	DDX_Text(pDX, IDC_R2MP_V, R2MP_V);	DDX_Text(pDX, IDC_R2MP_H, R2MP_H);
	DDX_Text(pDX, IDC_R2DP_V, R2DP_V);	DDX_Text(pDX, IDC_R2DP_H, R2DP_H);
	DDX_Text(pDX, IDC_R3CM_V, R3CM_V);	DDX_Text(pDX, IDC_R3CM_H, R3CM_H);
	DDX_Text(pDX, IDC_R3PP_V, R3PP_V);	DDX_Text(pDX, IDC_R3PP_H, R3PP_H);
	DDX_Text(pDX, IDC_R3MP_V, R3MP_V);	DDX_Text(pDX, IDC_R3MP_H, R3MP_H);
	DDX_Text(pDX, IDC_R3DP_V, R3DP_V);	DDX_Text(pDX, IDC_R3DP_H, R3DP_H);
	DDX_Text(pDX, IDC_R4CM_V, R4CM_V);	DDX_Text(pDX, IDC_R4CM_H, R4CM_H);
	DDX_Text(pDX, IDC_R4PP_V, R4PP_V);	DDX_Text(pDX, IDC_R4PP_H, R4PP_H);
	DDX_Text(pDX, IDC_R4MP_V, R4MP_V);	DDX_Text(pDX, IDC_R4MP_H, R4MP_H);
	DDX_Text(pDX, IDC_R4DP_V, R4DP_V);	DDX_Text(pDX, IDC_R4DP_H, R4DP_H);
	DDX_Text(pDX, IDC_R5CM_V, R5CM_V);	DDX_Text(pDX, IDC_R5CM_H, R5CM_H);
	DDX_Text(pDX, IDC_R5PP_V, R5PP_V);	DDX_Text(pDX, IDC_R5PP_H, R5PP_H);
	DDX_Text(pDX, IDC_R5MP_V, R5MP_V);	DDX_Text(pDX, IDC_R5MP_H, R5MP_H);
	DDX_Text(pDX, IDC_R5DP_V, R5DP_V);	DDX_Text(pDX, IDC_R5DP_H, R5DP_H);
}

BEGIN_MESSAGE_MAP(Dg_HandSegments, CDialog)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_COMMAND(ID_PRINT_PRINTWINDOW, &Dg_HandSegments::OnPrintPrintwindow)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

void Dg_HandSegments::GetCurrentJointAnglesFromDocument() 
{
	this->SetJointAngles(mDocPtr->GetSkeleton()->GetAngles());
}

//---------------------------------------------------------------------------
void Dg_HandSegments::SetJointAngles(const JointAngles& aJointAnglesRef )
{
	this->L1CA_V = aJointAnglesRef[A_L1CA_V];	this->L1CA_H = aJointAnglesRef[A_L1CA_H];
	this->L1MC_V = aJointAnglesRef[A_L1MC_V];	this->L1MC_H = aJointAnglesRef[A_L1MC_H];
	this->L1PP_V = aJointAnglesRef[A_L1PP_V];	this->L1PP_H = aJointAnglesRef[A_L1PP_H];
	this->L1DP_V = aJointAnglesRef[A_L1DP_V];	this->L1DP_H = aJointAnglesRef[A_L1DP_H];
	this->L2CM_V = aJointAnglesRef[A_L2CM_V];	this->L2CM_H = aJointAnglesRef[A_L2CM_H];
	this->L2PP_V = aJointAnglesRef[A_L2PP_V];	this->L2PP_H = aJointAnglesRef[A_L2PP_H];
	this->L2MP_V = aJointAnglesRef[A_L2MP_V];	this->L2MP_H = aJointAnglesRef[A_L2MP_H];
	this->L2DP_V = aJointAnglesRef[A_L2DP_V];	this->L2DP_H = aJointAnglesRef[A_L2DP_H];
	this->L3CM_V = aJointAnglesRef[A_L3CM_V];	this->L3CM_H = aJointAnglesRef[A_L3CM_H];
	this->L3PP_V = aJointAnglesRef[A_L3PP_V];	this->L3PP_H = aJointAnglesRef[A_L3PP_H];
	this->L3MP_V = aJointAnglesRef[A_L3MP_V];	this->L3MP_H = aJointAnglesRef[A_L3MP_H];
	this->L3DP_V = aJointAnglesRef[A_L3DP_V];	this->L3DP_H = aJointAnglesRef[A_L3DP_H];
	this->L4CM_V = aJointAnglesRef[A_L4CM_V];	this->L4CM_H = aJointAnglesRef[A_L4CM_H];
	this->L4PP_V = aJointAnglesRef[A_L4PP_V];	this->L4PP_H = aJointAnglesRef[A_L4PP_H];
	this->L4MP_V = aJointAnglesRef[A_L4MP_V];	this->L4MP_H = aJointAnglesRef[A_L4MP_H];
	this->L4DP_V = aJointAnglesRef[A_L4DP_V];	this->L4DP_H = aJointAnglesRef[A_L4DP_H];
	this->L5CM_V = aJointAnglesRef[A_L5CM_V];	this->L5CM_H = aJointAnglesRef[A_L5CM_H];
	this->L5PP_V = aJointAnglesRef[A_L5PP_V];	this->L5PP_H = aJointAnglesRef[A_L5PP_H];
	this->L5MP_V = aJointAnglesRef[A_L5MP_V];	this->L5MP_H = aJointAnglesRef[A_L5MP_H];
	this->L5DP_V = aJointAnglesRef[A_L5DP_V];	this->L5DP_H = aJointAnglesRef[A_L5DP_H];
	this->R1CA_V = aJointAnglesRef[A_R1CA_V];	this->R1CA_H = aJointAnglesRef[A_R1CA_H];
	this->R1MC_V = aJointAnglesRef[A_R1MC_V];	this->R1MC_H = aJointAnglesRef[A_R1MC_H];
	this->R1PP_V = aJointAnglesRef[A_R1PP_V];	this->R1PP_H = aJointAnglesRef[A_R1PP_H];
	this->R1DP_V = aJointAnglesRef[A_R1DP_V];	this->R1DP_H = aJointAnglesRef[A_R1DP_H];
	this->R2CM_V = aJointAnglesRef[A_R2CM_V];	this->R2CM_H = aJointAnglesRef[A_R2CM_H];
	this->R2PP_V = aJointAnglesRef[A_R2PP_V];	this->R2PP_H = aJointAnglesRef[A_R2PP_H];
	this->R2MP_V = aJointAnglesRef[A_R2MP_V];	this->R2MP_H = aJointAnglesRef[A_R2MP_H];
	this->R2DP_V = aJointAnglesRef[A_R2DP_V];	this->R2DP_H = aJointAnglesRef[A_R2DP_H];
	this->R3CM_V = aJointAnglesRef[A_R3CM_V];	this->R3CM_H = aJointAnglesRef[A_R3CM_H];
	this->R3PP_V = aJointAnglesRef[A_R3PP_V];	this->R3PP_H = aJointAnglesRef[A_R3PP_H];
	this->R3MP_V = aJointAnglesRef[A_R3MP_V];	this->R3MP_H = aJointAnglesRef[A_R3MP_H];
	this->R3DP_V = aJointAnglesRef[A_R3DP_V];	this->R3DP_H = aJointAnglesRef[A_R3DP_H];
	this->R4CM_V = aJointAnglesRef[A_R4CM_V];	this->R4CM_H = aJointAnglesRef[A_R4CM_H];
	this->R4PP_V = aJointAnglesRef[A_R4PP_V];	this->R4PP_H = aJointAnglesRef[A_R4PP_H];
	this->R4MP_V = aJointAnglesRef[A_R4MP_V];	this->R4MP_H = aJointAnglesRef[A_R4MP_H];
	this->R4DP_V = aJointAnglesRef[A_R4DP_V];	this->R4DP_H = aJointAnglesRef[A_R4DP_H];
	this->R5CM_V = aJointAnglesRef[A_R5CM_V];	this->R5CM_H = aJointAnglesRef[A_R5CM_H];
	this->R5PP_V = aJointAnglesRef[A_R5PP_V];	this->R5PP_H = aJointAnglesRef[A_R5PP_H];
	this->R5MP_V = aJointAnglesRef[A_R5MP_V];	this->R5MP_H = aJointAnglesRef[A_R5MP_H];
	this->R5DP_V = aJointAnglesRef[A_R5DP_V];	this->R5DP_H = aJointAnglesRef[A_R5DP_H];
}

bool Dg_HandSegments::SameAsDocument()
{
	JointAngles aJointAnglesRef = mDocPtr->GetSkeleton()->GetAngles();

	return (this->L1CA_V == aJointAnglesRef[A_L1CA_V] &&	this->L1CA_H == aJointAnglesRef[A_L1CA_H] &&
	this->L1MC_V == aJointAnglesRef[A_L1MC_V] &&	this->L1MC_H == aJointAnglesRef[A_L1MC_H] &&
	this->L1PP_V == aJointAnglesRef[A_L1PP_V] &&	this->L1PP_H == aJointAnglesRef[A_L1PP_H] &&
	this->L1DP_V == aJointAnglesRef[A_L1DP_V] &&	this->L1DP_H == aJointAnglesRef[A_L1DP_H] &&
	this->L2CM_V == aJointAnglesRef[A_L2CM_V] &&	this->L2CM_H == aJointAnglesRef[A_L2CM_H] &&
	this->L2PP_V == aJointAnglesRef[A_L2PP_V] &&	this->L2PP_H == aJointAnglesRef[A_L2PP_H] &&
	this->L2MP_V == aJointAnglesRef[A_L2MP_V] &&	this->L2MP_H == aJointAnglesRef[A_L2MP_H] &&
	this->L2DP_V == aJointAnglesRef[A_L2DP_V] &&	this->L2DP_H == aJointAnglesRef[A_L2DP_H] &&
	this->L3CM_V == aJointAnglesRef[A_L3CM_V] &&	this->L3CM_H == aJointAnglesRef[A_L3CM_H] &&
	this->L3PP_V == aJointAnglesRef[A_L3PP_V] &&	this->L3PP_H == aJointAnglesRef[A_L3PP_H] &&
	this->L3MP_V == aJointAnglesRef[A_L3MP_V] &&	this->L3MP_H == aJointAnglesRef[A_L3MP_H] &&
	this->L3DP_V == aJointAnglesRef[A_L3DP_V] &&	this->L3DP_H == aJointAnglesRef[A_L3DP_H] &&
	this->L4CM_V == aJointAnglesRef[A_L4CM_V] &&	this->L4CM_H == aJointAnglesRef[A_L4CM_H] &&
	this->L4PP_V == aJointAnglesRef[A_L4PP_V] &&	this->L4PP_H == aJointAnglesRef[A_L4PP_H] &&
	this->L4MP_V == aJointAnglesRef[A_L4MP_V] &&	this->L4MP_H == aJointAnglesRef[A_L4MP_H] &&
	this->L4DP_V == aJointAnglesRef[A_L4DP_V] &&	this->L4DP_H == aJointAnglesRef[A_L4DP_H] &&
	this->L5CM_V == aJointAnglesRef[A_L5CM_V] &&	this->L5CM_H == aJointAnglesRef[A_L5CM_H] &&
	this->L5PP_V == aJointAnglesRef[A_L5PP_V] &&	this->L5PP_H == aJointAnglesRef[A_L5PP_H] &&
	this->L5MP_V == aJointAnglesRef[A_L5MP_V] &&	this->L5MP_H == aJointAnglesRef[A_L5MP_H] &&
	this->L5DP_V == aJointAnglesRef[A_L5DP_V] &&	this->L5DP_H == aJointAnglesRef[A_L5DP_H] &&
	this->R1CA_V == aJointAnglesRef[A_R1CA_V] &&	this->R1CA_H == aJointAnglesRef[A_R1CA_H] &&
	this->R1MC_V == aJointAnglesRef[A_R1MC_V] &&	this->R1MC_H == aJointAnglesRef[A_R1MC_H] &&
	this->R1PP_V == aJointAnglesRef[A_R1PP_V] &&	this->R1PP_H == aJointAnglesRef[A_R1PP_H] &&
	this->R1DP_V == aJointAnglesRef[A_R1DP_V] &&	this->R1DP_H == aJointAnglesRef[A_R1DP_H] &&
	this->R2CM_V == aJointAnglesRef[A_R2CM_V] &&	this->R2CM_H == aJointAnglesRef[A_R2CM_H] &&
	this->R2PP_V == aJointAnglesRef[A_R2PP_V] &&	this->R2PP_H == aJointAnglesRef[A_R2PP_H] &&
	this->R2MP_V == aJointAnglesRef[A_R2MP_V] &&	this->R2MP_H == aJointAnglesRef[A_R2MP_H] &&
	this->R2DP_V == aJointAnglesRef[A_R2DP_V] &&	this->R2DP_H == aJointAnglesRef[A_R2DP_H] &&
	this->R3CM_V == aJointAnglesRef[A_R3CM_V] &&	this->R3CM_H == aJointAnglesRef[A_R3CM_H] &&
	this->R3PP_V == aJointAnglesRef[A_R3PP_V] &&	this->R3PP_H == aJointAnglesRef[A_R3PP_H] &&
	this->R3MP_V == aJointAnglesRef[A_R3MP_V] &&	this->R3MP_H == aJointAnglesRef[A_R3MP_H] &&
	this->R3DP_V == aJointAnglesRef[A_R3DP_V] &&	this->R3DP_H == aJointAnglesRef[A_R3DP_H] &&
	this->R4CM_V == aJointAnglesRef[A_R4CM_V] &&	this->R4CM_H == aJointAnglesRef[A_R4CM_H] &&
	this->R4PP_V == aJointAnglesRef[A_R4PP_V] &&	this->R4PP_H == aJointAnglesRef[A_R4PP_H] &&
	this->R4MP_V == aJointAnglesRef[A_R4MP_V] &&	this->R4MP_H == aJointAnglesRef[A_R4MP_H] &&
	this->R4DP_V == aJointAnglesRef[A_R4DP_V] &&	this->R4DP_H == aJointAnglesRef[A_R4DP_H] &&
	this->R5CM_V == aJointAnglesRef[A_R5CM_V] &&	this->R5CM_H == aJointAnglesRef[A_R5CM_H] &&
	this->R5PP_V == aJointAnglesRef[A_R5PP_V] &&	this->R5PP_H == aJointAnglesRef[A_R5PP_H] &&
	this->R5MP_V == aJointAnglesRef[A_R5MP_V] &&	this->R5MP_H == aJointAnglesRef[A_R5MP_H] &&
	this->R5DP_V == aJointAnglesRef[A_R5DP_V] &&	this->R5DP_H == aJointAnglesRef[A_R5DP_H] );
}

bool Dg_HandSegments::UpdateDocument()
{
	if(SameAsDocument())
		return true;	// Do not update; no changes

	JointAngles oldAngles,newJointAngles;
	//newJointAngles.ClearLastErrorMessage();

	newJointAngles[A_L1CA_V] = this->L1CA_V; newJointAngles[A_L1CA_H] = this->L1CA_H;
	newJointAngles[A_L1MC_V] = this->L1MC_V; newJointAngles[A_L1MC_H] = this->L1MC_H;
	newJointAngles[A_L1PP_V] = this->L1PP_V; newJointAngles[A_L1PP_H] = this->L1PP_H;
	newJointAngles[A_L1DP_V] = this->L1DP_V; newJointAngles[A_L1DP_H] = this->L1DP_H;
	newJointAngles[A_L2CM_V] = this->L2CM_V; newJointAngles[A_L2CM_H] = this->L2CM_H;
	newJointAngles[A_L2PP_V] = this->L2PP_V; newJointAngles[A_L2PP_H] = this->L2PP_H;
	newJointAngles[A_L2MP_V] = this->L2MP_V; newJointAngles[A_L2MP_H] = this->L2MP_H;
	newJointAngles[A_L2DP_V] = this->L2DP_V; newJointAngles[A_L2DP_H] = this->L2DP_H;
	newJointAngles[A_L3CM_V] = this->L3CM_V; newJointAngles[A_L3CM_H] = this->L3CM_H;
	newJointAngles[A_L3PP_V] = this->L3PP_V; newJointAngles[A_L3PP_H] = this->L3PP_H;
	newJointAngles[A_L3MP_V] = this->L3MP_V; newJointAngles[A_L3MP_H] = this->L3MP_H;
	newJointAngles[A_L3DP_V] = this->L3DP_V; newJointAngles[A_L3DP_H] = this->L3DP_H;
	newJointAngles[A_L4CM_V] = this->L4CM_V; newJointAngles[A_L4CM_H] = this->L4CM_H;
	newJointAngles[A_L4PP_V] = this->L4PP_V; newJointAngles[A_L4PP_H] = this->L4PP_H;
	newJointAngles[A_L4MP_V] = this->L4MP_V; newJointAngles[A_L4MP_H] = this->L4MP_H;
	newJointAngles[A_L4DP_V] = this->L4DP_V; newJointAngles[A_L4DP_H] = this->L4DP_H;
	newJointAngles[A_L5CM_V] = this->L5CM_V; newJointAngles[A_L5CM_H] = this->L5CM_H;
	newJointAngles[A_L5PP_V] = this->L5PP_V; newJointAngles[A_L5PP_H] = this->L5PP_H;
	newJointAngles[A_L5MP_V] = this->L5MP_V; newJointAngles[A_L5MP_H] = this->L5MP_H;
	newJointAngles[A_L5DP_V] = this->L5DP_V; newJointAngles[A_L5DP_H] = this->L5DP_H;
	newJointAngles[A_R1CA_V] = this->R1CA_V; newJointAngles[A_R1CA_H] = this->R1CA_H;
	newJointAngles[A_R1MC_V] = this->R1MC_V; newJointAngles[A_R1MC_H] = this->R1MC_H;
	newJointAngles[A_R1PP_V] = this->R1PP_V; newJointAngles[A_R1PP_H] = this->R1PP_H;
	newJointAngles[A_R1DP_V] = this->R1DP_V; newJointAngles[A_R1DP_H] = this->R1DP_H;
	newJointAngles[A_R2CM_V] = this->R2CM_V; newJointAngles[A_R2CM_H] = this->R2CM_H;
	newJointAngles[A_R2PP_V] = this->R2PP_V; newJointAngles[A_R2PP_H] = this->R2PP_H;
	newJointAngles[A_R2MP_V] = this->R2MP_V; newJointAngles[A_R2MP_H] = this->R2MP_H;
	newJointAngles[A_R2DP_V] = this->R2DP_V; newJointAngles[A_R2DP_H] = this->R2DP_H;
	newJointAngles[A_R3CM_V] = this->R3CM_V; newJointAngles[A_R3CM_H] = this->R3CM_H;
	newJointAngles[A_R3PP_V] = this->R3PP_V; newJointAngles[A_R3PP_H] = this->R3PP_H;
	newJointAngles[A_R3MP_V] = this->R3MP_V; newJointAngles[A_R3MP_H] = this->R3MP_H;
	newJointAngles[A_R3DP_V] = this->R3DP_V; newJointAngles[A_R3DP_H] = this->R3DP_H;
	newJointAngles[A_R4CM_V] = this->R4CM_V; newJointAngles[A_R4CM_H] = this->R4CM_H;
	newJointAngles[A_R4PP_V] = this->R4PP_V; newJointAngles[A_R4PP_H] = this->R4PP_H;
	newJointAngles[A_R4MP_V] = this->R4MP_V; newJointAngles[A_R4MP_H] = this->R4MP_H;
	newJointAngles[A_R4DP_V] = this->R4DP_V; newJointAngles[A_R4DP_H] = this->R4DP_H;
	newJointAngles[A_R5CM_V] = this->R5CM_V; newJointAngles[A_R5CM_H] = this->R5CM_H;
	newJointAngles[A_R5PP_V] = this->R5PP_V; newJointAngles[A_R5PP_H] = this->R5PP_H;
	newJointAngles[A_R5MP_V] = this->R5MP_V; newJointAngles[A_R5MP_H] = this->R5MP_H;
	newJointAngles[A_R5DP_V] = this->R5DP_V; newJointAngles[A_R5DP_H] = this->R5DP_H;

	Skeleton& skel = *mDocPtr->GetSkeleton();
	if(!skel.ValidateJointAngles(newJointAngles)) {
		// changed to just checking joint angle limits, not posture angles,
		// so we don't have to check for the hard limit here
		/*if(!skel.HardLimitError()) {
			ostringstream message;
			message << skel.getLastError() << endl << "Continue Anyway?" << endl;
			if(AfxMessageBox(message.str().c_str(), MB_OKCANCEL) == IDCANCEL) {
				return false;
			}
	   } else {*/
		   AfxMessageBox(skel.getLastError().c_str());
		   return false;
	   //}
	}
	/*int frame = mDocPtr->getCurrentFrame();*/
	oldAngles = mDocPtr->GetSkeleton()->GetAngles();
	mDocPtr->GetSkeleton()->SetAngles(newJointAngles);
	/*UndoablePostureChange *undoEvent = new UndoablePostureChange(frame,oldAngles,newJointAngles,mDocPtr->getFrameType(frame), FT_Keyframe);
	mDocPtr->addUndoEvent(undoEvent);*/

	mDocPtr->MakeDirtyAndUpdateViews();
	
	/*// Gray/ungray the Undo and Redo buttons if necessary	
	UndoButtonStatus();
    RedoButtonStatus();*/

	return true;
}

void Dg_HandSegments::OnOK() 
{
   if(this->CWnd::UpdateData(TRUE /*Retrieve*/)) //Data transfer successful 
   {
      if(this->UpdateDocument())
         CDialog::OnOK();
   }
}

void Dg_HandSegments::OnApply() 
{
   if(this->CWnd::UpdateData(TRUE /*Retrieve*/)) //Data transfer successful 
      this->UpdateDocument();
}

void Dg_HandSegments::OnPrintPrintwindow()
{
	// TODO: Add your command handler code here
 CRect m_rect;
 GetClientRect(&m_rect);
 CDC* pImageDC=GetDC();
 int formx,formy;
 formx=pImageDC->GetDeviceCaps(LOGPIXELSX);
 formy=pImageDC->GetDeviceCaps(LOGPIXELSY);
 ReleaseDC(pImageDC);
 DWORD dwflags=PD_PAGENUMS|PD_HIDEPRINTTOFILE|PD_SELECTION;  
 CPrintDialog m_printdlg(FALSE,dwflags,NULL);     
 if(m_printdlg.DoModal()==IDOK)
 {
  CDC pdc;
  pdc.Attach(m_printdlg.GetPrinterDC());
  int printerx,printery;
  printerx=pdc.GetDeviceCaps(LOGPIXELSX);
  printery=pdc.GetDeviceCaps(LOGPIXELSY);
  double ratex,ratey;
  ratex=(double)printerx/formx;
  ratey=(double)printery/formy;
  CClientDC dc(this);
  CBitmap bmp;
  int w=m_rect.Width();
  int h =m_rect.Height();
  bmp.CreateCompatibleBitmap(&dc,m_rect.Width(),m_rect.Height());
  CDC imagedc;
  imagedc.CreateCompatibleDC(&dc);
  imagedc.SelectObject(&bmp);
  imagedc.BitBlt(0,0,m_rect.Width(),m_rect.Height(),&dc,0,0,SRCCOPY);
  BITMAP bmap;
  bmp.GetBitmap(&bmap);
  int panelsize=0;
  if(bmap.bmBitsPixel<16)
  {
   panelsize=pow(2,bmap.bmBitsPixel*sizeof(RGBQUAD));
  }
  BITMAPINFO* bInfo=(BITMAPINFO*)LocalAlloc(LPTR,sizeof(BITMAPINFO)+panelsize);
  bInfo->bmiHeader.biClrImportant=0;
  bInfo->bmiHeader.biBitCount=bmap.bmBitsPixel;
  bInfo->bmiHeader.biCompression=0;
  bInfo->bmiHeader.biHeight=bmap.bmHeight;
  bInfo->bmiHeader.biPlanes=bmap.bmPlanes;
  bInfo->bmiHeader.biSize=sizeof(BITMAPINFO);
  bInfo->bmiHeader.biSizeImage=bmap.bmWidthBytes*bmap.bmHeight;
  bInfo->bmiHeader.biWidth=bmap.bmWidth;
  bInfo->bmiHeader.biXPelsPerMeter=0;
  bInfo->bmiHeader.biYPelsPerMeter=0;
  char *pData=new char[bmap.bmWidthBytes*bmap.bmHeight];
  ::GetDIBits(imagedc.m_hDC,bmp,0,bmap.bmHeight,pData,bInfo,DIB_RGB_COLORS);
  pdc.StartDoc("print");
  ::StretchDIBits(pdc.m_hDC,0,0,(int)((m_rect.Width()*ratex)),
   (int)((m_rect.Height())*ratey),0,0,m_rect.Width(),
   m_rect.Height(),pData,bInfo,DIB_RGB_COLORS,SRCCOPY);
  pdc.EndDoc();
  LocalFree(bInfo);
  delete[] pData;
  pData = NULL;
  bmp.DeleteObject();
 }
}


BOOL Dg_HandSegments::OnInitDialog()
{
	CDialog::OnInitDialog();
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->AppendMenu(MF_SEPARATOR);// add a separator
	pSysMenu->AppendMenu(MF_STRING,IDR_MENU1,"Print Window");// add print menu
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void Dg_HandSegments::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
		CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (nID ==IDR_MENU1)             
    {
		OnPrintPrintwindow();
    }
	  else
	CDialog::OnSysCommand(nID, lParam);
}
