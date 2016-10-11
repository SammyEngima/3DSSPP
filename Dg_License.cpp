// Dg_License.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "Dg_License.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_License dialog

char* licenseTXT[] = 
{
    {"End-User License Agreement\r\n"},
    { "\r\n" }, 
    {"PROGRAM:  3D Static Strength Prediction Program (TM) Software\r\n"},
    { "\r\n" }, 
    {"IMPORTANT - READ CAREFULLY: This Agreement is a legal agreement between LICENSEE   and The Regents of The University of Michigan, a constitutional corporation of the State of Michigan, (\"MICHIGAN\").  The term \"LICENSEE\" shall mean the person installing the PROGRAM (defined below in Paragraph 1) if it is solely for personal use by that person on the personal equipment of that person.  Alternatively, if the PROGRAM is being installed on equipment for use by another legal entity, such as a corporation, limited liability company or partnership, then the person installing the PROGRAM by proceeding with the installation certifies that he or she has authority to bind that legal entity to this Agreement; and that legal entity shall be considered to be the LICENSEE.  By installing, copying, downloading, accessing or otherwise using the PROGRAM, you and LICENSEE agree to be bound by the terms of this Agreement. If you or LICENSEE (if a separate legal entity) do not agree with the terms of this Agreement, do not install, access or use the PROGRAM; instead you should return it to the University of Michigan for a full refund.\r\n"},
    { "\r\n" }, 
    {"BACKGROUND\r\n"},
    { "\r\n" }, 
    {"1.	The University of Michigan through its Center for Ergonomics has developed a proprietary computer program and related documentation, known as the 3D Static Strength Prediction Program, for use by trained individuals as a job physical stress analysis model in the field of ergonomics, (hereinafter referred to as \"PROGRAM\"); and\r\n"},
    { "\r\n" }, 
    {"2.	LICENSEE desires to obtain and MICHIGAN, consistent with its mission of education and research, desires to grant a license to use the PROGRAM subject to the terms and conditions set forth below.\r\n"},
    { "\r\n" }, 
    {"The parties therefore agree as follows:\r\n"},
    { "\r\n" }, 
    {"I.  	LICENSE\r\n"},
	{"    MICHIGAN hereby grants to LICENSEE a non-exclusive, non-transferable right to install and use the PROGRAM on one CPU at a time, in executable form only and subject to the terms and conditions of this Agreement. If the PROGRAM is being installed and used on behalf of an educational entity, then LICENSEE shall have the right to install and use multiple copies of the PROGRAM for academic, training or educational purposes.\r\n"},
    { "\r\n" }, 
    {"II.	LIMITATION OF LICENSE AND RESTRICTIONS\r\n"},
    {"A.	LICENSEE shall not use, print, copy, translate, reverse engineer, decompile, disassemble, modify, create derivative works of or publicly display the PROGRAM, in whole or in part, unless expressly authorized by this Agreement.\r\n"},
    { "\r\n" }, 
    {"B.	LICENSEE agrees that it shall use the PROGRAM only for LICENSEE'S sole and exclusive use, and shall not disclose, sell, license, or otherwise distribute the PROGRAM to any third party without the prior written consent of MICHIGAN.  LICENSEE shall not assign this Agreement, and any attempt by LICENSEE to assign it shall be void from the beginning.  LICENSEE agrees to secure and protect the PROGRAM and any copies in a manner consistent with the maintenance of MICHIGAN'S rights in the PROGRAM and to take appropriate action by instruction or agreement with its employees who are permitted access to the PROGRAM in order to satisfy LICENSEE'S obligations under this Agreement.  LICENSEE is authorized to copy the PROGRAM for backup purposes only.\r\n"},
    { "\r\n" }, 
    {"III.	TITLE AND OWNERSHIP\r\n"},
    {"A.	No ownership rights of MICHIGAN in the PROGRAM are conferred upon LICENSEE by this Agreement.\r\n"},
    { "\r\n" }, 
    {"B.	LICENSEE acknowledges MICHIGAN'S proprietary rights in the PROGRAM and agrees to reproduce all copyright notices supplied by MICHIGAN on all copies of the PROGRAM, and on all PROGRAM outputs and copies of PROGRAM outputs.\r\n"},
    { "\r\n" }, 
    {"IV.	DISCLAIMER OF WARRANTY AND LIMITATION OF LIABILITY\r\n"},
    {"A.	THE PROGRAM IS PROVIDED \"AS IS\" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  MICHIGAN DOES NOT WARRANT THAT THE FUNCTIONS CONTAINED IN THE PROGRAM WILL MEET LICENSEE'S REQUIREMENTS OR THAT OPERATION WILL BE UNINTERRUPTED OR ERROR FREE.  MICHIGAN shall not be liable for special, indirect, incidental, or consequential damages with respect to any claim on account of or arising from this Agreement or use of the PROGRAM, even if MICHIGAN has been or is hereafter advised of the possibility of such damages.  Because some states do not allow certain exclusions or limitations on implied warranties or of liability for consequential or incidental damages, the above exclusions may not apply to LICENSEE.  In no event, however, will MICHIGAN be liable to LICENSEE, under any theory of recovery, in an amount in excess of the license royalty paid by LICENSEE under this Agreement.\r\n"},
    { "\r\n" }, 
    {"B.	LICENSEE agrees that MICHIGAN has no obligation to provide to LICENSEE any maintenance, support, or update services. Should MICHIGAN provide any revised versions of the PROGRAM to LICENSEE, LICENSEE agrees that this license agreement shall apply to such revised versions.\r\n"},
    { "\r\n" }, 
    {"C.	MICHIGAN does not warrant, endorse or certify any individual or organization as a trainer for the PROGRAM, or any special strength testing, data input or other equipment to be used with the PROGRAM.\r\n"},
    { "\r\n" }, 
    {"D.	If the physical media provided hereunder contains any defects upon receipt by LICENSEE, LICENSEE shall have thirty (30) days in which to return said defective media and MICHIGAN shall provide a replacement free of charge.\r\n"},
    { "\r\n" }, 
    {"V.	WARRANTY OF LICENSEE\r\n"},
	{"    LICENSEE warrants and represents that it will carefully review the User's Manual and that it is aware of the necessity for training in the field of ergonomics and the underlying biomechanical model of the PROGRAM and of the necessity for having an expert in the field of ergonomics review and interpret data provided in or by the PROGRAM, including the User's Manual.  LICENSEE further warrants and represents that it either has such training or will incorporate the services of individuals with such training in any use or interpretation of data provided in or by the PROGRAM.\r\n"},
    { "\r\n" }, 
    {"VI. TERMINATION\r\n"},
	{"    If LICENSEE at any time fails to abide by the terms of this Agreement, MICHIGAN shall have the right to immediately terminate the license granted herein, require the return or destruction of all copies of the PROGRAM from LICENSEE and certification in writing as to such return or destruction, and pursue any other legal or equitable remedies available.\r\n"},
    { "\r\n" }, 
    {"VII.  MISCELLANEOUS\r\n"},
    {"A.	This Agreement shall be construed in accordance with the laws of the State of Michigan.  Should LICENSEE for any reason bring a claim, demand, or other action against MICHIGAN, its agents or employees, arising out of this Agreement or the PROGRAM licensed herein, LICENSEE agrees to bring said claim only in the Michigan Court of Claims.\r\n"},
    { "\r\n" }, 
    {"B.	THIS AGREEMENT REPRESENTS THE COMPLETE AND EXCLUSIVE STATEMENT OF THE AGREEMENT BETWEEN MICHIGAN AND LICENSEE AND SUPERSEDES ALL PRIOR AGREEMENTS, PROPOSALS, REPRESENTATIONS AND OTHER COMMUNICATIONS, VERBAL OR WRITTEN, BETWEEN THEM WITH RESPECT TO USE OF THE PROGRAM.  THIS AGREEMENT MAY BE MODIFIED ONLY WITH THE MUTUAL WRITTEN APPROVAL OF AUTHORIZED REPRESENTATIVES OF THE PARTIES.\r\n"},
    { "\r\n" }, 
    {"C.	The terms and conditions of this Agreement shall prevail notwithstanding any different, conflicting, or additional terms or conditions which may appear in any purchase order or other document submitted by LICENSEE.  LICENSEE agrees that such additional or inconsistent terms are deemed rejected by MICHIGAN.\r\n"},
    { "\r\n" }, 
    {"D.	Unless otherwise exempt therefrom, LICENSEE agrees that it will be responsible for any sales, use or excise taxes imposed by any governmental unit in this transaction except income taxes.\r\n"},
    { "\r\n" }, 
    {"E.	LICENSEE acknowledges that the PROGRAM is of United States origin. Licensee agrees to comply with all applicable international and national laws that apply to the PROGRAM, including the United States Export Administration Regulations, as well as end-user, end-use, and destination restrictions issued by the United States.\r\n"},
	{ "\r\n" }, 
	NULL
};


Dg_License::Dg_License(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_License::IDD, pParent)
{
	//{{AFX_DATA_INIT(Dg_License)
	mLicense = _T("");
	//}}AFX_DATA_INIT

    int i=0;
    while(licenseTXT[i])
    {
        mLicense += CString(licenseTXT[i++]);
    }
    
    
}


void Dg_License::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_License)
	DDX_Text(pDX, IDC_EDIT1, mLicense);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Dg_License, CDialog)
	//{{AFX_MSG_MAP(Dg_License)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_License message handlers

void Dg_License::OnOK() 
{
	// TO DO: Add extra validation here
	
	CDialog::OnOK();
}
