#include "stdafx.h"
#include <strstream>
#include <string>

#include "hom.h"
#include "Dg_Intro.h"
//#include "dg_logon.h"
#include "AppVersion.hpp"

using std::string;
using std::ends;
using std::strstream;

//This file contains C_Hom_App helper member functions for processing logons.

//----------------------------------------------------------------------------
BOOL C_Hom_App::Log_On()
{
   int Permitted_Attempts = 3;
   
   BOOL Permission_Granted = FALSE;
   

   Dg_Intro Diag;
                                  //--- Intro 1
   Diag.Intro_Message = this->Intro_Text_1();
   if (Diag.DoModal() == IDOK)
   {
                                  //--- Intro 2
      // Diag.Intro_Message = this->Intro_Text_2();
      // if (Diag.DoModal() == IDOK)
      // {
                                  //--- Intro 3
         // Diag.Intro_Message = this->Intro_Text_3();
         // if( Diag.DoModal() == IDOK )
         // {
               Permission_Granted = TRUE;
         // }         
         // else  AfxMessageBox( "Exiting program at user's request." ); //Intro 3 
      // }      
      // else  AfxMessageBox( "Exiting program at user's request." ); //Intro 2
   }
   else  AfxMessageBox( "Exiting program at user's request." ); //Intro 1 

   return   Permission_Granted;
}                                   

#include <strstream>
//----------------------------------------------------------------------------
CString C_Hom_App::Intro_Text_1()
{
   strstream   Text;
   Text << "                                                           TM\n";

   Text << "                      3D STATIC STRENGTH PREDICTION PROGRAM\n";

   Text << "\n";
   CString VersionText;
   VersionText.Format("Version " szVERSION);
   int WhiteLength = (78 - VersionText.GetLength()) / 2;
   CString MYString(' ', WhiteLength);
   MYString += VersionText + "\n";
   Text << (LPCSTR)MYString;  
   //Text << "                                 Version " << this->Version << "\n";
   Text << "\n";
   Text << "                             Copyright 1986-" << szCOPYRIGHT_YEAR << "\n";
   Text << "\n";
   Text << "          By The Regents of the University of Michigan, ALL RIGHTS RESERVED\n";
   Text << "\n";
   Text << "\n";

   Text << "             The  program is useful as a job design  and  evaluation tool\n";
   Text << "         and for instructional purposes.   It  is  applicable  to  worker\n";
   Text << "         exertions  in three dimensions.   The effects of acceleration  and\n";
   Text << "         momentum  must  be  negligible.  The  static biomechanical model\n"; 
   Text << "         is most  applicable to the analysis of 'slow' movements used  in\n"; 
   Text << "         heavy materials handling tasks. Such tasks can best be  analyzed\n";
   Text << "         by describing the activity as a  sequence of  static  postures.\n";
 
   Text << "\n";
   Text << "             THE  PROGRAM  SHOULD  NOT BE USED AS THE SOLE DETERMINANT OF\n"; 
   Text << "         WORKER  STRENGTH  PERFORMANCE  OR  JOB  DESIGNS  BASED  ON  THAT\n";
   Text << "         PERFORMANCE.  OTHER  CRITERIA  AND  PROFESSIONAL  JUDGEMENT  ARE\n";  
   Text << "         REQUIRED TO  PROPERLY DESIGN A SAFE AND PRODUCTIVE JOB.\n";
 

   Text << "\n"; 

   Text << "             Instructions on the proper use of the program are recommended\n";
   Text << "         through  short  courses  at the  University  of  Michigan.   New\n";
   Text << "         versions will become  available  as additional research provides\n";
   Text << "         the basis for revisions.";
   Text << "\n";
   Text << "\n";
   Text << "\n";
   Text << "         The  program  and any   associated documentation are provided with\n"; 
   Text << "         RESTRICTED RIGHTS. Use, duplication or disclosure by the Government\n"; 
   Text << "         is subject to restrictions as set forth in subdivisions (c) (1)(ii)\n"; 
   Text << "         of  the  Rights  in  Technical  Data  and  Computer Software clause\n";
   Text << "         at 252.227-7013.\n";
   Text << "\n";
   Text << "         Contractor/Manufacturer  is  The  University of Michigan, Ann Arbor,\n";
   Text << "         MI 48109\n";
   Text << ends;

   // Text << "          
   // Text << " This  text  was  written  by  D.B. Chaffin and G.B.J.\n"; Andersson, and  was\n";
   // Text << "        Andersson, and  was published  by  J. Wiley and Sons, Inc., N.Y.;/London  in  1984.\n";
   // Text << "        Worker strength data used for the  prediction  model are from  a\n";
   // Text << "        collection of strength studies described  or  referenced  in the\n";
   // Text << "        same text.   The model results correlate with average population\n";
   // Text << "        static strengths (r=0.8).\n";
   Text << "\n";
   /* Text << "              The inverse kinematics algorithm was developed from research\n";
   Text << "         on the preferred postures of individuals  manipulating loads with\n";
   Text << "         known hand positions.  The posture generated by this algorithm is\n";
   Text << "         intended to be  a  first approximation, and is based on the posi-\n";
   Text << "         tion of and load at the hands, and is not necessarily representa-\n";
   Text << "         tive of the actual posture of the task being modeled.  The actual\n";
   Text << "         observed posture can be affected by other individual factors.\n";
   // Text << " 	   such\n";
   // Text << "        as body type, postural preference, and training, as well as envi-\n";
   // Text << "        ronmental factors such as the nature of the object being handled,\n";
   // Text << "        traction at the foot-floor interface, workplace obstructions, and\n";
   // Text << "        worker apparel.";*/

   /* Text << "             The  program is useful as a job design  and  evaluation tool\n";
   Text << "         and for instructional purposes.   It  is  applicable  to  worker\n";
   Text << "         exertions  in three dimensions.   The effects of acceleration  and\n";
   Text << "         momentum  must  be  negligible.  The  static biomechanical model\n"; 
   Text << "         is most  applicable to the analysis of 'slow' movements used  in\n"; 
   Text << "         heavy materials handling tasks. Such tasks can best be  analyzed\n";
   Text << "         by describing the activity as a  sequence of  static  postures.\n";
   // Text << "                                                                         ";  
   Text << "\n";*/

   /* Text << "             THE  PROGRAM  SHOULD  NOT BE USED AS THE SOLE DETERMINANT OF\n"; 
   Text << "         WORKER STRENGTH PERFORMANCE OR JOB DESIGNS BASED   ON THAT\n";
   Text << "         PERFORMANCE OR JOB DESIGNS BASED   ON THAT  PERFORMANCE.   OTHER\n";
   Text << "         CRITERIA  AND PROFESSIONAL  JUDGEMENT ARE  REQUIRED TO  PROPERLY\n";
   Text << "         DESIGN A SAFE AND PRODUCTIVE JOB.\n";
   Text << "\n";
   Text << "             Instructions on the proper use of the program are recommended\n";
   Text << "         through  short  courses  at the  University  of  Michigan.   New\n";
   Text << "         versions will become  available  as additional research provides\n";
   Text << "         the basis for revisions.";*/

   // Text << ends;




   // Text << ends;
   

   string Text_Ptr = Text.str();
	assert(Text_Ptr.length()); 


	CString Intro_Text = Text_Ptr.c_str();
   return Intro_Text; 

}

//----------------------------------------------------------------------------
/*CString  C_Hom_App::Intro_Text_2()
{
   strstream Text;

   // Text << " This  text  was  written  by  D.B. Chaffin and G.B.J.\n"; Andersson, and  was\n";
   // Text << "        Andersson, and  was published  by  J. Wiley and Sons, Inc., N.Y.;/London  in  1984.\n";
   // Text << "        Worker strength data used for the  prediction  model are from  a\n";
   // Text << "        collection of strength studies described  or  referenced  in the\n";
   // Text << "        same text.   The model results correlate with average population\n";
   // Text << "        static strengths (r=0.8).\n";
   // Text << " 	   such\n";
   // Text << "        as body type, postural preference, and training, as well as envi-\n";
   // Text << "        ronmental factors such as the nature of the object being handled,\n";
   // Text << "        traction at the foot-floor interface, workplace obstructions, and\n";
   // Text << "        worker apparel.";

   Text << " ";  
   Text << "\n";
   Text << "             THE  PROGRAM  SHOULD  NOT BE USED AS THE SOLE DETERMINANT OF\n"; 
   Text << "         WORKER STRENGTH PERFORMANCE OR JOB DESIGNS BASED   ON THAT\n";
   Text << "         PERFORMANCE OR JOB DESIGNS BASED   ON THAT  PERFORMANCE.   OTHER\n";
   Text << "         CRITERIA  AND PROFESSIONAL  JUDGEMENT ARE  REQUIRED TO  PROPERLY\n";
   Text << "         DESIGN A SAFE AND PRODUCTIVE JOB.\n";
   Text << "\n";
   Text << "             Instructions on the proper use of the program are recommended\n";
   Text << "         through  short  courses  at the  University  of  Michigan.   New\n";
   Text << "         versions will become  available  as additional research provides\n";
   Text << "         the basis for revisions."; 

   Text << ends;

   




   string Text_Ptr = Text.str();
	assert(Text_Ptr.length());

	CString Intro_Text = Text_Ptr.c_str();
   return Intro_Text;
}

//----------------------------------------------------------------------------
CString C_Hom_App::Intro_Text_3()
{
   strstream   Text;
   Text << ends;

   string Text_Ptr = Text.str();
	assert(Text_Ptr.length());

	CString Intro_Text = Text_Ptr.c_str();
   return Intro_Text;
}
*/