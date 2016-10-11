// InsertFramesBox.cpp : implementation file
//

#include "stdafx.h"
#include "InsertFramesBox.h"
#include "hom.h"
#include "hom_doc.h"
#include "AnimationBar.h"

// InsertFramesBox dialog

IMPLEMENT_DYNAMIC(InsertFramesBox, CDialog)

InsertFramesBox::InsertFramesBox(CWnd* pParent /*=NULL*/)
	: CDialog(InsertFramesBox::IDD, pParent)
	, mNumFrames(0)
	, mNumSeconds(_T(""))
{
	animationBar = (AnimationBar*)pParent;
}

InsertFramesBox::~InsertFramesBox()
{
}

void InsertFramesBox::setNumFrames(int numFrames) {
	mNumFrames = numFrames;
	// TODO get frame rate from task
	mNumSeconds.Format("%.2f", ((double)numFrames) / FRAMES_PER_SECOND);
	UpdateData(false);
}
void InsertFramesBox::setNumSeconds(double numSeconds) {
	setNumFrames(FRAMES_PER_SECOND * numSeconds);
}

void InsertFramesBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INSERTNUMFRAMES, mNumFrames);
	DDX_Text(pDX, IDC_INSERTNUMSECONDS, mNumSeconds);
}


BEGIN_MESSAGE_MAP(InsertFramesBox, CDialog)
	ON_EN_CHANGE(IDC_INSERTNUMFRAMES, &InsertFramesBox::OnEnChangeInsertnumframes)
	ON_EN_CHANGE(IDC_INSERTNUMSECONDS, &InsertFramesBox::OnEnChangeInsertnumseconds)
	ON_BN_CLICKED(IDC_BUTTON1, &InsertFramesBox::OnBnClickedButton1)
END_MESSAGE_MAP()


// InsertFramesBox message handlers

void InsertFramesBox::OnEnChangeInsertnumframes() {
	UpdateData(true);
	setNumFrames(mNumFrames);
}

void InsertFramesBox::OnEnChangeInsertnumseconds() {
	// TODO validation
	UpdateData(true);
	mNumFrames = FRAMES_PER_SECOND * atof(mNumSeconds);
	UpdateData(false);
}

void InsertFramesBox::OnBnClickedButton1()
{
	this->OnOK();
}

void InsertFramesBox::OnOK()
{
	animationBar->insertFrames(mNumFrames, false);
	CDialog::OnOK();
}
