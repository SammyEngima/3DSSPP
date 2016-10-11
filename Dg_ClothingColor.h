#ifndef DG_CLOTHINGCOLOR_H
#define DG_CLOTHINGCOLOR_H

#pragma once


// Dg_ClothingColor dialog

class Dg_ClothingColor : public CDialog
{
   DECLARE_DYNAMIC(Dg_ClothingColor)

public:
   Dg_ClothingColor(CWnd* pParent = NULL);   // standard constructor
   virtual ~Dg_ClothingColor();

   // Dialog Data
   enum { IDD = IDD_CLOTHING_COLOR };
   afx_msg void OnBnClickedButton2();
   afx_msg void OnBnClickedPantcolor();
   afx_msg void OnBnClickedShirtcolor();
   afx_msg void OnPaint();
   virtual void OnOK();
   afx_msg void OnBnClickedClothapply();
   afx_msg void OnStnClickedShirtprev();
   afx_msg void OnBnClickedOk();


protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   
   // Sayan 07/18/07
   //
   // UpdateDocument now needs an int passed to it as argument. This is the "gender"
   // parameter, needed because the gender of the human figure will now determine 
   // clothing color.

   bool UpdateDocument();
   DECLARE_MESSAGE_MAP()

private:
   C_Hom_Doc* pDoc;
 

   
};

#endif