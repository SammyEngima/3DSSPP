#ifndef SHOW_WARNINGS
// ARTHUR::PRAGMA [05/07/01] - Disable browse file information truncation
#pragma warning(disable: 4786)
// ARTHUR::PRAGMA [01/17/03] - Disable bool->int performance warning
#pragma warning(disable: 4800)
#endif

#include "stdAfx.h"
#include "resource.h"

#include "BatchFile.hpp"

#include "hom.h"
#include "hom_doc.h"	// necessary!
#include "Skeleton.hpp"

#include "Anthropometry.hpp"
#include "JointAngles.hpp"
#include "HandLoads.hpp"
#include "TaskDescription.hpp"
#include "InputAuxiliary.h"
#include "Dg_Jointloads.h"

#include <strstream>
#include <string>

//Head file pack from Dg_JointLoads.cpp
#include "force.hpp"
#include "torque.hpp"
#include "../c4eLibNew/Units.hpp"
#include "UndoApplied.h"
#include "GLStickView.h"

// Remember, don't use "using namespace std;"
using std::ofstream;
using std::ifstream;
using std::fstream;
using std::strstream;
using std::ostrstream;
using std::endl;
using std::ios;
using std::ends;
using std::string;

//-------------------------------------
   BatchFile::
BatchFile
(
   const string &       aFilePath ,
   const string &       aFileRoot ,
         C_Hom_Doc *    aDocPtr  
)
:
mAutoExport( false ) ,
mInFileName( aFilePath ) ,
mExportFileName( aFileRoot + ".exp" ) ,
mLogFileName( aFileRoot + ".log" ) ,
mDocPtr( aDocPtr ) ,
mProgressDialog()
{
   mCommands[ "3DSSPPBATCHFILE" ] = bch3DSSPPBatchFile;
   mCommands[ "DESCRIPTION" ] = bchDescription;
   mCommands[ "ANTHROPOMETRY" ] = bchAnthropomentry;
   mCommands[ "JOINTANGLES" ] = bchJointAngles;
   mCommands[ "JOINTANGLES6" ] = bchJointAnglesWithLegs;
   mCommands[ "JOINTANGLESALL" ] = bchJointAnglesAll;
   mCommands[ "HANDLOADS" ] = bchHandLoads;
   mCommands[ "COMMENT" ] = bchComment;
   mCommands[ "EXPORT" ] = bchExport;
//newly add by Yijun May.8
   mCommands[ "OUTPUT" ] = bchExport;
//end
   mCommands[ "AUTOEXPORT" ] = bchAutoExport;
   mCommands[ "PAUSE" ] = bchPause;
   mCommands[ "FRAME" ] = bchFrameChange;
   mCommands[ "POSTUREPREDICTION" ] = bchPosturePrediction;
   mCommands[ "LOCATIONS" ] = bchLocations;
   mCommands[ "EXTERNALFORCE&MOMENTA" ] = bchexternala; //check details in BatchFile.hpp
   mCommands[ "EXTERNALFORCE&MOMENTB" ] = bchexternalb;
   mCommands[ "EXTERNALFORCE&MOMENTC" ] = bchexternalc;
   mCommands[ "EXTERNALFORCE&MOMENTD" ] = bchexternald;
   mCommands[ "EXTERNALFORCE&MOMENTE" ] = bchexternale;
   mCommands[ "EXTERNALFORCE&MOMENTF" ] = bchexternalf;
   mCommands[ "EXTERNALFORCE&MOMENTG" ] = bchexternalg;
   mCommands[ "EXTERNALFORCE&MOMENTH" ] = bchexternalh;
   mCommands[ "EXTERNALFORCE&MOMENTI" ] = bchexternali;
   mCommands[ "EXTERNALFORCE&MOMENTJ" ] = bchexternalj;
   mCommands[ "EXTERNALFORCE&MOMENTK" ] = bchexternalk;
   mCommands[ "EXTERNALFORCE&MOMENTL" ] = bchexternall;
   mCommands[ "EXTERNALFORCE&MOMENTM" ] = bchexternalm;
   mCommands[ "EXTERNALFORCE&MOMENTN" ] = bchexternaln;
   mCommands[ "HCLEFTHANDCOMPONENTLOAD" ] = bchLeftHandComponentLoad;
   mCommands[ "HCRIGHTHANDCOMPONENTLOAD" ] = bchRightHandComponentLoad;
//newly add by Kevin
   mCommands[ "SUP" ] = bchSupport;
//end

   mCommands[ "DES" ] = bchDescription;
   mCommands[ "ANT" ] = bchAnthropomentry;
   mCommands[ "JOI" ] = bchJointAngles;
   mCommands[ "JO6" ] = bchJointAnglesWithLegs;
   mCommands[ "JOA" ] = bchJointAnglesAll;
   mCommands[ "HAN" ] = bchHandLoads;
   mCommands[ "COM" ] = bchComment;
   mCommands[ "EXP" ] = bchExport;
//newly add by Yijun May.8
   mCommands[ "OUT" ] = bchExport;
//end
   mCommands[ "AUT" ] = bchAutoExport;
   mCommands[ "PAU" ] = bchPause;
   mCommands[ "FRM" ] = bchFrameChange;
   mCommands[ "PPR" ] = bchPosturePrediction;
   mCommands[ "LOC" ] = bchLocations;
   mCommands[ "EXA" ] = bchexternala;
   mCommands[ "EXB" ] = bchexternalb;
   mCommands[ "EXC" ] = bchexternalc;
   mCommands[ "EXD" ] = bchexternald;
   mCommands[ "EXE" ] = bchexternale;
   mCommands[ "EXF" ] = bchexternalf;
   mCommands[ "EXG" ] = bchexternalg;
   mCommands[ "EXH" ] = bchexternalh;
   mCommands[ "EXI" ] = bchexternali;
   mCommands[ "EXJ" ] = bchexternalj;
   mCommands[ "EXK" ] = bchexternalk;
   mCommands[ "EXL" ] = bchexternall;
   mCommands[ "EXM" ] = bchexternalm;
   mCommands[ "EXN" ] = bchexternaln;
   mCommands[ "HCL" ] = bchLeftHandComponentLoad;
   mCommands[ "HCR" ] = bchRightHandComponentLoad;
   return;
}

void LogError(ofstream & aOFStreamRef,int aRecordNumber,CString aClassNameRef,const string &aErrorMessageRef)
{
    CString lMsg;
	lMsg.Format("\nRecord No. %d: %s: %s",aRecordNumber,aClassNameRef,aErrorMessageRef.c_str());
    aOFStreamRef << lMsg;
}

//-------------------------------------
bool BatchFile::Process()
{
   static const char lEndOfRecord = '#';
   static const int lLineLength = 256;

  bool   lOK = true;

///  ARTHUR - commented due to string stream incompatability
//	 Jtk - uncommented to bring back batch file functionality.
//   type cast the strings to (char*) which appeared to solve 
//	 the string stream incompatibility
   //attempt to open input file
   
  ifstream   lInFile( mInFileName.c_str(), ios::in );
   if( ! lInFile.is_open() )
   {
      AfxMessageBox( "Unable to open batch input file." );
      return   false;
   }

   //make sure this is a batch file
   string   lLine;//( lLineLength, "", ' ' );
   lInFile >> lLine;
   //lInFile.getline( (char*)lLine, lLineLength, lEndOfRecord );
   
 // JTK - Removed simplying setting lCommandName = lLine appears to work
 /*  strstream   lCore;
   lCore << lLine << endl;*/
   string   lCommandName;//( 20, "", ' ' );
// JTK - Removed because strstream no longer exists   
//   lCore >> (char*)lCommandName;
   lCommandName = lLine;		// added because of the removal of strstream
   // allow no space so we don't break peoples' workarounds
   if(lCommandName != "3DSSPPBATCHFILE#") {
	if(mCommands.find(lCommandName) == mCommands.end() ||
		mCommands[lCommandName] != bch3DSSPPBatchFile) {
			AfxMessageBox("Batch file header missing. \nFile must begin with '3DSSPPBATCHFILE #'.");
			lInFile.close();
			return false;
	}
	
		// make sure there's a # next
		lInFile >> lLine;
		if(lLine != "#") {
			AfxMessageBox("Batch file header missing. \nFile must begin with '3DSSPPBATCHFILE #'.");
			lInFile.close();
			return false;
		}
   }
   //cMapType::iterator   lIt = mCommands.find( lCommandName );
/*
   if(lIt != mCommands.end()) //match found
   {
      switch((*lIt).second)
      {
         case bch3DSSPPBatchFile :
         {
            //Header found as first command. OK to proceed.
            break;
         }
         default:
         {
            AfxMessageBox( "Batch file header missing. \nFile must begin with '3DSSPPBATCHFILE #'." );
            lInFile.close();
            return false;
         }
      }
   }
   else //match not found
   {
      AfxMessageBox( "3DSSPP batch file header missing. This is not a valid 3DSSPP batch file." );
	  lInFile.close();     
	  return false;      
   }*/
   
   //attempt to open export file
   
   fstream lExportFile(mExportFileName.c_str(), ios::in | ios::out | ios::trunc | ios::ate ); 
   if(!lExportFile.is_open())
   {
      AfxMessageBox( "Unable to open batch export file." );
      lInFile.close();
      return false;
   }
   
   //attempt to open log file
   
   ofstream lLogFile(mLogFileName.c_str()); 
   if(!lLogFile.is_open())
   {
      AfxMessageBox("Unable to open batch log file.");
      lInFile.close();
      lExportFile.close();
      return false;
   }

   // prepare the progress dialog

   mProgressDialog.Create(IDD_BATCHFILE);
   mProgressDialog.ShowWindow(SW_SHOWNORMAL);
   
   // repeatedly get and decode lines

   int lRecordNumber = 1;
   bool lFirstCommand = true;
   
   while(lInFile.peek() != EOF)
   {
      //Get line

      string   lLine;//( lLineLength, "", ' ' );
      //lInFile.getline( (char*)lLine, lLineLength, lEndOfRecord );
	  lInFile >> lLine;
      
      //strstream   lCore;
      //lCore << lLine << ends;
      
      //Get the command name

      //String   lCommandName( 20, "", ' ' );
      //lCore >> (char*)lCommandName;
	  string lCommandName = lLine;
      
      cMapType::iterator   lIt = mCommands.find( lCommandName );
      
      if( lIt != mCommands.end() ) //match found
      {
         switch( (*lIt).second )
         {
            case bchDescription :
            {
               TaskDescription   lTaskDescription;
               lInFile >> lTaskDescription;
               if( lTaskDescription.Error() )
               {
                  LogError( lLogFile, lRecordNumber, "TaskDescription", (char*)lTaskDescription.LastErrorMessage() );
                  lOK = false;
               }
               else
               {
                  mDocPtr->SetDescription( lTaskDescription );
                  //NOTE: Task description does not trigger automatic export.
               }
               break;
            }
            
            case bchAnthropomentry :
			{
                Anthropometry   lAnthropometry;
			    lInFile >> lAnthropometry;
                if( lAnthropometry.Error() ) {
                    LogError( lLogFile, lRecordNumber, "Anthropometry", (char*)lAnthropometry.LastErrorMessage() );
                    lOK = false;
                } else {
                    mDocPtr->setAnthropometry( lAnthropometry );
					mDocPtr->MakeDirtyAndUpdateViews(true);
                    if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
                }
               break;
            }
            
            case bchJointAngles :
            {
     //          JointAngles lJointAngles;
     //          lInFile >> lJointAngles;
			  // if(! lJointAngles.Validate() ) 
     //          {
     //             LogError( lLogFile, lRecordNumber, "JointAngles", lJointAngles.getLastError().c_str() );
     //             lOK = false;
     //          }
     //          else
     //          {
					//// make the frame a keyframe if it is currently unset
					//// TODO make keyframe as long as it is not a keyframe?
					//if(mDocPtr->getFrameType() == FT_Unset) {
					//	mDocPtr->setFrameType(FT_Keyframe);
					//}

					//// Put the joint angles and clear the undoing queue
     //               mDocPtr->clearUndoHistory();
     //               mDocPtr->GetSkeleton()->SetAngles(lJointAngles);
					//mDocPtr->MakeDirtyAndUpdateViews(true);

					//if (mAutoExport)
					//	mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND);
     //          }
     //          break;
				AfxMessageBox(_T("JOI not supported in 7.00 and later version."), MB_OK | MB_ICONSTOP);
            }

			case bchJointAnglesWithLegs :
			{
				//no longer needed in 7.0.0

				//JointAngles lJointAngles;
				//// don't user operator when we're reading in with the leg angles
				//// we should actually get rid of the operator then, or have a better
				//// versioning system
				////lInFile >> lJointAngles;
				//lJointAngles.ReadBatchFileWithLegs(lInFile);
				//if(! lJointAngles.Validate() )
				//{
				//	LogError(lLogFile, lRecordNumber, "JointAngles", lJointAngles.getLastError().c_str());
				//	lOK = false;
				//}
				//else
				//{
				//	// make the frame a keyframe if it is currently unset
				//	// TODO make keyframe as long as it is not a keyframe?
				//	if(mDocPtr->getFrameType() == FT_Unset) {
				//		mDocPtr->setFrameType(FT_Keyframe);
				//	}

				//	// Put the joint angles and clear the undoing queue
				//	mDocPtr->clearUndoHistory();
				//	mDocPtr->GetSkeleton()->SetAngles(lJointAngles);
				//	mDocPtr->MakeDirtyAndUpdateViews(true);

				//	if(mAutoExport)
				//		mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND);
				//}
				//break;
				AfxMessageBox(_T("JO6 not supported in 7.00 and later version."), MB_OK | MB_ICONSTOP);
			}

			case bchJointAnglesAll :
			{
				JointAngles lJointAngles;
				// don't user operator when we're reading in with the leg angles
				// we should actually get rid of the operator then, or have a better
				// versioning system
				//lInFile >> lJointAngles;
				lJointAngles.ReadBatchFileAll(lInFile);
				/*if(! lJointAngles.Validate() )
				{
					LogError(lLogFile, lRecordNumber, "JointAngles", lJointAngles.getLastError().c_str());
					lOK = false;
				}
				else*/
				{
					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}

					// Put the joint angles and clear the undoing queue
					mDocPtr->clearUndoHistory();
					mDocPtr->GetSkeleton()->SetAngles(lJointAngles);
					mDocPtr->MakeDirtyAndUpdateViews(true);

					if(mAutoExport)
						mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND);
				}
				break;
			}
            
            case bchHandLoads :
            {
               HandLoads   lHandLoads;
               lInFile >> lHandLoads;
               if(lHandLoads.Error()) 
               {
                  LogError( lLogFile, lRecordNumber, "HandLoads", (char*)lHandLoads.LastErrorMessage() );
                  lOK = false;
               }
               else
               {

				   mDocPtr->GetSkeleton()->setExtForce(JT_LHAND, lHandLoads.GetLForceVector());
				   mDocPtr->GetSkeleton()->setExtForce(JT_RHAND, lHandLoads.GetRForceVector());

					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}
					mDocPtr->MakeDirtyAndUpdateViews(true);

				  if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
               }
               break;
            }
			// for hand component load
			case bchLeftHandComponentLoad :
            {
               HandLoads   lHandLoads;
               lInFile >> lHandLoads;
               if(lHandLoads.Error()) 
               {
                  LogError( lLogFile, lRecordNumber, "HandComponentLoads", (char*)lHandLoads.LastErrorMessage() );
                  lOK = false;
               }
               else
               {
				   Vector3 leftForceVec;
				   leftForceVec[0] = lHandLoads.LeftMagnitude();
				   leftForceVec[1] = lHandLoads.LeftVerticalAngle();
				   leftForceVec[2] = lHandLoads.LeftHorizontalAngle();
				   Vector3 leftTorqueVec;
				   leftTorqueVec[0] = lHandLoads.RightMagnitude();
				   leftTorqueVec[1] = lHandLoads.RightVerticalAngle();
				   leftTorqueVec[2] = lHandLoads.RightHorizontalAngle();
				   mDocPtr->GetSkeleton()->setExtForce(JT_LHAND, leftForceVec);
				   mDocPtr->GetSkeleton()->setExtTorque(JT_LHAND, leftTorqueVec);

					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}
					mDocPtr->MakeDirtyAndUpdateViews(true);

				  if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
               }
               break;
            }
			case bchRightHandComponentLoad :
            {
               HandLoads   lHandLoads;
               lInFile >> lHandLoads;
               if(lHandLoads.Error()) 
               {
                  LogError( lLogFile, lRecordNumber, "HandComponentLoads", (char*)lHandLoads.LastErrorMessage() );
                  lOK = false;
               }
               else
               {
				   Vector3 rightForceVec;
				   rightForceVec[0] = lHandLoads.LeftMagnitude();
				   rightForceVec[1] = lHandLoads.LeftVerticalAngle();
				   rightForceVec[2] = lHandLoads.LeftHorizontalAngle();
				   Vector3 rightTorqueVec;
				   rightTorqueVec[0] = lHandLoads.RightMagnitude();
				   rightTorqueVec[1] = lHandLoads.RightVerticalAngle();
				   rightTorqueVec[2] = lHandLoads.RightHorizontalAngle();
				   mDocPtr->GetSkeleton()->setExtForce(JT_RHAND, rightForceVec);
				   mDocPtr->GetSkeleton()->setExtTorque(JT_RHAND, rightTorqueVec);

					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}
					mDocPtr->MakeDirtyAndUpdateViews(true);

				  if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
               }
               break;
            }

			//for multi-joint load
			case bchexternala:
            {
               HandLoads   lJointLoads;
               lInFile >> lJointLoads;
               if(lJointLoads.Error()) 
               {
                  LogError( lLogFile, lRecordNumber, "JointLoads", (char*)lJointLoads.LastErrorMessage() );
                  lOK = false;
               }
               else
               {
				   const JointID order[11] = {JT_RELBOW,JT_RSHOULDER,JT_LELBOW,JT_LSHOULDER,JT_L5S1_TEMP,
                               JT_RHIP,JT_RKNEE,JT_RANKLE,JT_LHIP,JT_LKNEE,JT_LANKLE};

					// make group undo event
					int left = Get_App_Ptr()->Get_Document_Ptr()->LeftSelect();
					int right = Get_App_Ptr()->Get_Document_Ptr()->RightSelect();
					GroupEvent* groupEvent = new GroupEvent(left, right);

					for(int frame = left; frame <= right; frame++) {
						//Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
						Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(frame);
								 
						skel.setExtForce(order[0],lJointLoads.GetForceVector());
						skel.setExtTorque(order[0],lJointLoads.GetTorqueVector());
					}
					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}
					mDocPtr->MakeDirtyAndUpdateViews(true);

				  if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
               }
               break;
            }

			case bchexternalb:
            {
               HandLoads   lJointLoads;
               lInFile >> lJointLoads;
               if(lJointLoads.Error()) 
               {
                  LogError( lLogFile, lRecordNumber, "JointLoads", (char*)lJointLoads.LastErrorMessage() );
                  lOK = false;
               }
               else
               {
				   const JointID order[11] = {JT_RELBOW,JT_RSHOULDER,JT_LELBOW,JT_LSHOULDER,JT_L5S1_TEMP,
                               JT_RHIP,JT_RKNEE,JT_RANKLE,JT_LHIP,JT_LKNEE,JT_LANKLE};

					// make group undo event
					int left = Get_App_Ptr()->Get_Document_Ptr()->LeftSelect();
					int right = Get_App_Ptr()->Get_Document_Ptr()->RightSelect();
					GroupEvent* groupEvent = new GroupEvent(left, right);

					for(int frame = left; frame <= right; frame++) {
						//Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
						Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(frame);
								 
						skel.setExtForce(order[1],lJointLoads.GetForceVector());
						skel.setExtTorque(order[1],lJointLoads.GetTorqueVector());
					}
					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}
					mDocPtr->MakeDirtyAndUpdateViews(true);

				  if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
               }
               break;
            }

			case bchexternalc:
            {
               HandLoads   lJointLoads;
               lInFile >> lJointLoads;
               if(lJointLoads.Error()) 
               {
                  LogError( lLogFile, lRecordNumber, "JointLoads", (char*)lJointLoads.LastErrorMessage() );
                  lOK = false;
               }
               else
               {
				   const JointID order[11] = {JT_RELBOW,JT_RSHOULDER,JT_LELBOW,JT_LSHOULDER,JT_L5S1_TEMP,
                               JT_RHIP,JT_RKNEE,JT_RANKLE,JT_LHIP,JT_LKNEE,JT_LANKLE};

					// make group undo event
					int left = Get_App_Ptr()->Get_Document_Ptr()->LeftSelect();
					int right = Get_App_Ptr()->Get_Document_Ptr()->RightSelect();
					GroupEvent* groupEvent = new GroupEvent(left, right);

					for(int frame = left; frame <= right; frame++) {
						//Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
						Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(frame);
								 
						skel.setExtForce(order[2],lJointLoads.GetForceVector());
						skel.setExtTorque(order[2],lJointLoads.GetTorqueVector());
					}
					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}
					mDocPtr->MakeDirtyAndUpdateViews(true);

				  if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
               }
               break;
            }

			case bchexternald:
            {
               HandLoads   lJointLoads;
               lInFile >> lJointLoads;
               if(lJointLoads.Error()) 
               {
                  LogError( lLogFile, lRecordNumber, "JointLoads", (char*)lJointLoads.LastErrorMessage() );
                  lOK = false;
               }
               else
               {
				   const JointID order[11] = {JT_RELBOW,JT_RSHOULDER,JT_LELBOW,JT_LSHOULDER,JT_L5S1_TEMP,
                               JT_RHIP,JT_RKNEE,JT_RANKLE,JT_LHIP,JT_LKNEE,JT_LANKLE};

					// make group undo event
					int left = Get_App_Ptr()->Get_Document_Ptr()->LeftSelect();
					int right = Get_App_Ptr()->Get_Document_Ptr()->RightSelect();
					GroupEvent* groupEvent = new GroupEvent(left, right);

					for(int frame = left; frame <= right; frame++) {
						//Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
						Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(frame);
								 
						skel.setExtForce(order[3],lJointLoads.GetForceVector());
						skel.setExtTorque(order[3],lJointLoads.GetTorqueVector());
					}
					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}
					mDocPtr->MakeDirtyAndUpdateViews(true);

				  if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
               }
               break;
            }

			case bchexternale:
            {
               HandLoads   lJointLoads;
               lInFile >> lJointLoads;
               if(lJointLoads.Error()) 
               {
                  LogError( lLogFile, lRecordNumber, "JointLoads", (char*)lJointLoads.LastErrorMessage() );
                  lOK = false;
               }
               else
               {
				   const JointID order[11] = {JT_RELBOW,JT_RSHOULDER,JT_LELBOW,JT_LSHOULDER,JT_L5S1_TEMP,
                               JT_RHIP,JT_RKNEE,JT_RANKLE,JT_LHIP,JT_LKNEE,JT_LANKLE};

					// make group undo event
					int left = Get_App_Ptr()->Get_Document_Ptr()->LeftSelect();
					int right = Get_App_Ptr()->Get_Document_Ptr()->RightSelect();
					GroupEvent* groupEvent = new GroupEvent(left, right);

					for(int frame = left; frame <= right; frame++) {
						//Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
						Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(frame);
								 
						skel.setExtForce(order[4],lJointLoads.GetForceVector());
						skel.setExtTorque(order[4],lJointLoads.GetTorqueVector());
					}
					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}
					mDocPtr->MakeDirtyAndUpdateViews(true);

				  if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
               }
               break;
            }

			case bchexternalf:
            {
               HandLoads   lJointLoads;
               lInFile >> lJointLoads;
               if(lJointLoads.Error()) 
               {
                  LogError( lLogFile, lRecordNumber, "JointLoads", (char*)lJointLoads.LastErrorMessage() );
                  lOK = false;
               }
               else
               {
				   const JointID order[11] = {JT_RELBOW,JT_RSHOULDER,JT_LELBOW,JT_LSHOULDER,JT_L5S1_TEMP,
                               JT_RHIP,JT_RKNEE,JT_RANKLE,JT_LHIP,JT_LKNEE,JT_LANKLE};

					// make group undo event
					int left = Get_App_Ptr()->Get_Document_Ptr()->LeftSelect();
					int right = Get_App_Ptr()->Get_Document_Ptr()->RightSelect();
					GroupEvent* groupEvent = new GroupEvent(left, right);

					for(int frame = left; frame <= right; frame++) {
						//Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
						Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(frame);
								 
						skel.setExtForce(order[5],lJointLoads.GetForceVector());
						skel.setExtTorque(order[5],lJointLoads.GetTorqueVector());
					}
					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}
					mDocPtr->MakeDirtyAndUpdateViews(true);

				  if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
               }
               break;
            }

			case bchexternalg:
            {
               HandLoads   lJointLoads;
               lInFile >> lJointLoads;
               if(lJointLoads.Error()) 
               {
                  LogError( lLogFile, lRecordNumber, "JointLoads", (char*)lJointLoads.LastErrorMessage() );
                  lOK = false;
               }
               else
               {
				   const JointID order[11] = {JT_RELBOW,JT_RSHOULDER,JT_LELBOW,JT_LSHOULDER,JT_L5S1_TEMP,
                               JT_RHIP,JT_RKNEE,JT_RANKLE,JT_LHIP,JT_LKNEE,JT_LANKLE};

					// make group undo event
					int left = Get_App_Ptr()->Get_Document_Ptr()->LeftSelect();
					int right = Get_App_Ptr()->Get_Document_Ptr()->RightSelect();
					GroupEvent* groupEvent = new GroupEvent(left, right);

					for(int frame = left; frame <= right; frame++) {
						//Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
						Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(frame);
								 
						skel.setExtForce(order[6],lJointLoads.GetForceVector());
						skel.setExtTorque(order[6],lJointLoads.GetTorqueVector());
					}
					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}
					mDocPtr->MakeDirtyAndUpdateViews(true);

				  if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
               }
               break;
            }

			case bchexternalh:
            {
               HandLoads   lJointLoads;
               lInFile >> lJointLoads;
               if(lJointLoads.Error()) 
               {
                  LogError( lLogFile, lRecordNumber, "JointLoads", (char*)lJointLoads.LastErrorMessage() );
                  lOK = false;
               }
               else
               {
				   const JointID order[11] = {JT_RELBOW,JT_RSHOULDER,JT_LELBOW,JT_LSHOULDER,JT_L5S1_TEMP,
                               JT_RHIP,JT_RKNEE,JT_RANKLE,JT_LHIP,JT_LKNEE,JT_LANKLE};

					// make group undo event
					int left = Get_App_Ptr()->Get_Document_Ptr()->LeftSelect();
					int right = Get_App_Ptr()->Get_Document_Ptr()->RightSelect();
					GroupEvent* groupEvent = new GroupEvent(left, right);

					for(int frame = left; frame <= right; frame++) {
						//Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
						Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(frame);
								 
						skel.setExtForce(order[7],lJointLoads.GetForceVector());
						skel.setExtTorque(order[7],lJointLoads.GetTorqueVector());
					}
					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}
					mDocPtr->MakeDirtyAndUpdateViews(true);

				  if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
               }
               break;
            }

			case bchexternali:
            {
               HandLoads   lJointLoads;
               lInFile >> lJointLoads;
               if(lJointLoads.Error()) 
               {
                  LogError( lLogFile, lRecordNumber, "JointLoads", (char*)lJointLoads.LastErrorMessage() );
                  lOK = false;
               }
               else
               {
				   const JointID order[11] = {JT_RELBOW,JT_RSHOULDER,JT_LELBOW,JT_LSHOULDER,JT_L5S1_TEMP,
                               JT_RHIP,JT_RKNEE,JT_RANKLE,JT_LHIP,JT_LKNEE,JT_LANKLE};

					// make group undo event
					int left = Get_App_Ptr()->Get_Document_Ptr()->LeftSelect();
					int right = Get_App_Ptr()->Get_Document_Ptr()->RightSelect();
					GroupEvent* groupEvent = new GroupEvent(left, right);

					for(int frame = left; frame <= right; frame++) {
						//Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
						Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(frame);
								 
						skel.setExtForce(order[8],lJointLoads.GetForceVector());
						skel.setExtTorque(order[8],lJointLoads.GetTorqueVector());
					}
					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}
					mDocPtr->MakeDirtyAndUpdateViews(true);

				  if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
               }
               break;
            }

			case bchexternalj:
            {
               HandLoads   lJointLoads;
               lInFile >> lJointLoads;
               if(lJointLoads.Error()) 
               {
                  LogError( lLogFile, lRecordNumber, "JointLoads", (char*)lJointLoads.LastErrorMessage() );
                  lOK = false;
               }
               else
               {
				   const JointID order[11] = {JT_RELBOW,JT_RSHOULDER,JT_LELBOW,JT_LSHOULDER,JT_L5S1_TEMP,
                               JT_RHIP,JT_RKNEE,JT_RANKLE,JT_LHIP,JT_LKNEE,JT_LANKLE};

					// make group undo event
					int left = Get_App_Ptr()->Get_Document_Ptr()->LeftSelect();
					int right = Get_App_Ptr()->Get_Document_Ptr()->RightSelect();
					GroupEvent* groupEvent = new GroupEvent(left, right);

					for(int frame = left; frame <= right; frame++) {
						//Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
						Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(frame);
								 
						skel.setExtForce(order[9],lJointLoads.GetForceVector());
						skel.setExtTorque(order[9],lJointLoads.GetTorqueVector());
					}
					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}
					mDocPtr->MakeDirtyAndUpdateViews(true);

				  if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
               }
               break;
            }

			case bchexternalk:
            {
               HandLoads   lJointLoads;
               lInFile >> lJointLoads;
               if(lJointLoads.Error()) 
               {
                  LogError( lLogFile, lRecordNumber, "JointLoads", (char*)lJointLoads.LastErrorMessage() );
                  lOK = false;
               }
               else
               {
				   const JointID order[11] = {JT_RELBOW,JT_RSHOULDER,JT_LELBOW,JT_LSHOULDER,JT_L5S1_TEMP,
                               JT_RHIP,JT_RKNEE,JT_RANKLE,JT_LHIP,JT_LKNEE,JT_LANKLE};

					// make group undo event
					int left = Get_App_Ptr()->Get_Document_Ptr()->LeftSelect();
					int right = Get_App_Ptr()->Get_Document_Ptr()->RightSelect();
					GroupEvent* groupEvent = new GroupEvent(left, right);

					for(int frame = left; frame <= right; frame++) {
						//Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
						Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(frame);
								 
						skel.setExtForce(order[10],lJointLoads.GetForceVector());
						skel.setExtTorque(order[10],lJointLoads.GetTorqueVector());
					}
					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}
					mDocPtr->MakeDirtyAndUpdateViews(true);

				  if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
               }
               break;
            }

			case bchexternall:
            {
               HandLoads   lJointLoads;
               lInFile >> lJointLoads;
               if(lJointLoads.Error()) 
               {
                  LogError( lLogFile, lRecordNumber, "JointLoads", (char*)lJointLoads.LastErrorMessage() );
                  lOK = false;
               }
               else
               {
				   const JointID order[14] = {JT_RELBOW,JT_RSHOULDER,JT_LELBOW,JT_LSHOULDER,JT_L5S1_TEMP,
                               JT_RHIP,JT_RKNEE,JT_RANKLE,JT_LHIP,JT_LKNEE,JT_LANKLE, JT_RWRIST, JT_LWRIST,JT_TRAGION};

					// make group undo event
					int left = Get_App_Ptr()->Get_Document_Ptr()->LeftSelect();
					int right = Get_App_Ptr()->Get_Document_Ptr()->RightSelect();
					GroupEvent* groupEvent = new GroupEvent(left, right);

					for(int frame = left; frame <= right; frame++) {
						//Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
						Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(frame);
								 
						skel.setExtForce(order[11],lJointLoads.GetForceVector());
						skel.setExtTorque(order[11],lJointLoads.GetTorqueVector());
					}
					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}
					mDocPtr->MakeDirtyAndUpdateViews(true);

				  if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
               }
               break;
            }

			case bchexternalm:
            {
               HandLoads   lJointLoads;
               lInFile >> lJointLoads;
               if(lJointLoads.Error()) 
               {
                  LogError( lLogFile, lRecordNumber, "JointLoads", (char*)lJointLoads.LastErrorMessage() );
                  lOK = false;
               }
               else
               {
				   const JointID order[14] = {JT_RELBOW,JT_RSHOULDER,JT_LELBOW,JT_LSHOULDER,JT_L5S1_TEMP,
					   JT_RHIP,JT_RKNEE,JT_RANKLE,JT_LHIP,JT_LKNEE,JT_LANKLE, JT_RWRIST, JT_LWRIST, JT_TRAGION};

					// make group undo event
					int left = Get_App_Ptr()->Get_Document_Ptr()->LeftSelect();
					int right = Get_App_Ptr()->Get_Document_Ptr()->RightSelect();
					GroupEvent* groupEvent = new GroupEvent(left, right);

					for(int frame = left; frame <= right; frame++) {
						//Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
						Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(frame);
								 
						skel.setExtForce(order[12],lJointLoads.GetForceVector());
						skel.setExtTorque(order[12],lJointLoads.GetTorqueVector());
					}
					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}
					mDocPtr->MakeDirtyAndUpdateViews(true);

				  if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
               }
               break;
            }
			case bchexternaln:
            {
               HandLoads   lJointLoads;
               lInFile >> lJointLoads;
               if(lJointLoads.Error()) 
               {
                  LogError( lLogFile, lRecordNumber, "JointLoads", (char*)lJointLoads.LastErrorMessage() );
                  lOK = false;
               }
               else
               {
				   const JointID order[14] = {JT_RELBOW,JT_RSHOULDER,JT_LELBOW,JT_LSHOULDER,JT_L5S1_TEMP,
					   JT_RHIP,JT_RKNEE,JT_RANKLE,JT_LHIP,JT_LKNEE,JT_LANKLE, JT_RWRIST, JT_LWRIST, JT_TRAGION};

					// make group undo event
					int left = Get_App_Ptr()->Get_Document_Ptr()->LeftSelect();
					int right = Get_App_Ptr()->Get_Document_Ptr()->RightSelect();
					GroupEvent* groupEvent = new GroupEvent(left, right);

					for(int frame = left; frame <= right; frame++) {
						//Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
						Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(frame);
								 
						skel.setExtForce(order[13],lJointLoads.GetForceVector());
						skel.setExtTorque(order[13],lJointLoads.GetTorqueVector());
					}
					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}
					mDocPtr->MakeDirtyAndUpdateViews(true);

				  if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
               }
               break;
            }

			case bchSupport : 
			{
				Support_s tempsupport;
				InputAuxiliary inputcheck; //input checking class
				string temp; //temp string for the input
				int feetSupport, position; //temp int value for enum to support read
				lInFile >> temp; 
				if (!inputcheck.isInteger(temp) | !inputcheck.CheckRange(inputcheck.StringtoInt(temp), 0,3))
				{
					LogError(lLogFile, lRecordNumber, "Support Selection", string("The input feetsupport must be an integer from 0 to 3 \n"));
					lOK = false;
					break;
				}
				feetSupport = inputcheck.StringtoInt(temp);
				lInFile >> temp; 
				if (!inputcheck.isInteger(temp) | !inputcheck.CheckRange(inputcheck.StringtoInt(temp), 0,1))
				{
					LogError(lLogFile, lRecordNumber, "Support Selection", string("The input position must be an integer from 0 to 1 \n"));
					lOK = false;
					break;
				}
				position = (bool)inputcheck.StringtoInt(temp);
				lInFile >> temp; 
				if (!inputcheck.isInteger(temp) | !inputcheck.CheckRange(inputcheck.StringtoInt(temp), 0,1))
				{
					LogError(lLogFile, lRecordNumber, "Support Selection", string("The input frontSeatPanSuppor must be an integer from 0 to 1 \n"));
					lOK = false;
					break;
				}
				tempsupport.frontSeatPanSupport = (bool)inputcheck.StringtoInt(temp);
				lInFile >> temp; 
				if (!inputcheck.isInteger(temp) | !inputcheck.CheckRange(inputcheck.StringtoInt(temp), 0,1))
				{
					LogError(lLogFile, lRecordNumber, "Support Selection", string("The input seatHasBackRest must be an integer from 0 to 1 \n"));
					lOK = false;
					break;
				}
				tempsupport.seatHasBackRest = (bool)inputcheck.StringtoInt(temp);
				
				lInFile >> temp; 
				if (!inputcheck.isDouble(temp) | !inputcheck.CheckLowBound(inputcheck.StringtoDouble(temp), 19.05))
				{
					LogError(lLogFile, lRecordNumber, "Support Selection", string("The input backRestCenterHeight must be a number and it is bigger than 19.05 \n"));
					lOK = false;
					break;
				}
				tempsupport.backRestCenterHeight = inputcheck.StringtoDouble(temp);

				lInFile >> temp; 
				if (!inputcheck.isInteger(temp) | !inputcheck.CheckRange(inputcheck.StringtoInt(temp), -360,360))
				{
					LogError(lLogFile, lRecordNumber, "Support Selection", string("The input additionalPelvicTilt must be an integer from -360 to 360 \n"));
					lOK = false;
					break;
				}
				tempsupport.additionalPelvicTilt = inputcheck.StringtoInt(temp);

				tempsupport.feetSupport = FeetSupport_e(feetSupport);
				tempsupport.position = Position_e(position);

				//The following two line is due to the coodinate changing. 
				tempsupport.additionalPelvicTilt = tempsupport.additionalPelvicTilt;
				tempsupport.displayAdditionalPelvicTilt = tempsupport.additionalPelvicTilt;

				if(lInFile.fail()) {
					LogError(lLogFile, lRecordNumber, "Support Selection", string("Support Selection parameters must be FeetSupport, Position, FrontSeatPanSupport,seatHasBackRest, backRestCenterHeight. additionalPelvicTilt, displayAdditionalPelvicTilt \n"));
					lOK = false;
				}
				else if (position && (feetSupport ==1|feetSupport ==2)){
					LogError(lLogFile, lRecordNumber, "Support Selection", string("Two feet must be both on or not on the ground if seated \n"));
					lOK = false;
				}
				else if (!position && (feetSupport == 3)){
					LogError(lLogFile, lRecordNumber, "Support Selection", string("Either or both feet needs to be on the ground if standing \n"));
					lOK = false;
				}
				else if (tempsupport.frontSeatPanSupport && (feetSupport !=3)){
					LogError(lLogFile, lRecordNumber, "Support Selection", string("No frontPan is allowed \n"));
					lOK = false;
				}

				else {

					mDocPtr->setSupport(tempsupport);
					mDocPtr->MakeDirtyAndUpdateViews(true); 
				}
				break;
				
			}
            case bchComment :
            {
               //do nothing
               break;
            }
            
            case bchExport :
            {
               mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND);  
               break;
            }
            
            case bchAutoExport :
            {
               int   lAutoExport;
               lInFile >> lAutoExport;
               if( ! lInFile.fail() )
               {
				   if (lAutoExport)
					   mAutoExport = true;
				   else 
					   mAutoExport = false;
                  
               }
               else
               {
                  string   lMsg( "Value not 0 (false) or 1 (true)." );
                  LogError( lLogFile, lRecordNumber, "AutoExport", lMsg );
                  lOK = false;                  
               }
               break;
            }
			
			case bchPause :
			{
				int pauseTime;
				string temp; //temp string for the input
				lInFile >> temp;
				InputAuxiliary inputcheck; //input checking class
				if (!inputcheck.isInteger(temp))
				{	
					LogError(lLogFile, lRecordNumber, "Pause", string("Must enter a positive integer number of milliseconds to pause, or 0 to be prompted to resume"));
					lOK = false;
					break;
				}
				pauseTime = inputcheck.StringtoInt(temp);

				if( ! lInFile.fail() && pauseTime >= 0) {
					//mDocPtr->RedrawViews();
					mDocPtr->UpdateViews();
					if(pauseTime == 0) {
						AfxMessageBox("Batch file processing paused. Press OK to continue", MB_OK);
					} else {
						Sleep(pauseTime);
					}
				} else {
					LogError( lLogFile, lRecordNumber, "Pause", string("Must enter a positive integer number of milliseconds to pause, or 0 to be prompted to resume"));
					lOK = false;
				}
				break;
			}

			case bchFrameChange:
			{
				string firstParam;
				lInFile >> firstParam;
				if(lInFile.fail()) {
					LogError(lLogFile, lRecordNumber, "Frame", string("Frame command parameters must be + <num> | - <num> | FIRST | LAST | <num>"));
					lOK = false;
				} else {
					// Go to first frame
					if(firstParam == "FIRST") {
						mDocPtr->setCurrentFrame(0);
					} else
					// Go to last frame
					if(firstParam == "LAST") {
						mDocPtr->setCurrentFrame(mDocPtr->getLastFrame());
					} else
					// Advance a number of frames
					if(firstParam == "+") {
						// read number of frames
						int numFrames;
						lInFile >> numFrames;
						if(lInFile.fail()) {
							LogError(lLogFile, lRecordNumber, "Frame", string("Frame command parameters must be + <num> | - <num> | FIRST | LAST | <num>"));
							lOK = false;
						} else {
							mDocPtr->setCurrentFrame(mDocPtr->getCurrentFrame() + numFrames);
						}
					} else
					// Rewind a number of frames
					if(firstParam == "-") {
						// read number of frames
						int numFrames;
						lInFile >> numFrames;
						if(lInFile.fail()) {
							LogError(lLogFile, lRecordNumber, "Frame", string("Frame command parameters must be + <num> | - <num> | FIRST | LAST | <num>"));
							lOK = false;
						} else {
							mDocPtr->setCurrentFrame(mDocPtr->getCurrentFrame() - numFrames);
						}
					} else {
						// Go to numbered frame
						int frameNumber = atoi(firstParam.c_str());
						mDocPtr->setCurrentFrame(frameNumber);
					}
				}
				// update views after frame change
				mDocPtr->UpdateViews();
				break;
			}
			case bchPosturePrediction:
			{
				double LH, LV, LL, RH, RV, RL;
				int HO;
				InputAuxiliary inputcheck; //input checking class
				string temp; //temp string for the input
				lInFile >> temp;				
				if (!inputcheck.isDouble(temp))
				{	
					LogError(lLogFile, lRecordNumber, "Posture Prediction", string("Posture predition parameters must be an number"));
					lOK = false;
					break;
				}
				LH = inputcheck.StringtoDouble(temp);
				lInFile >> temp;				
				if (!inputcheck.isDouble(temp))
				{	
					LogError(lLogFile, lRecordNumber, "Posture Prediction", string("Posture predition parameters must be an number"));
					lOK = false;
					break;
				}
				LV = inputcheck.StringtoDouble(temp);

				lInFile >> temp;				
				if (!inputcheck.isDouble(temp))
				{	
					LogError(lLogFile, lRecordNumber, "Posture Prediction", string("Posture predition parameters must be an number"));
					lOK = false;
					break;
				}
				LL = inputcheck.StringtoDouble(temp);

				lInFile >> temp;				
				if (!inputcheck.isDouble(temp))
				{	
					LogError(lLogFile, lRecordNumber, "Posture Prediction", string("Posture predition parameters must be an number"));
					lOK = false;
					break;
				}
				RH = inputcheck.StringtoDouble(temp);


				lInFile >> temp;				
				if (!inputcheck.isDouble(temp))
				{	
					LogError(lLogFile, lRecordNumber, "Posture Prediction", string("Posture predition parameters must be an number"));
					lOK = false;
					break;
				}
				RV = inputcheck.StringtoDouble(temp);

				lInFile >> temp;				
				if (!inputcheck.isDouble(temp))
				{	
					LogError(lLogFile, lRecordNumber, "Posture Prediction", string("Posture predition parameters must be an number"));
					lOK = false;
					break;
				}
				RL = inputcheck.StringtoDouble(temp);


				lInFile >> temp;				
				if (!inputcheck.isInteger(temp))
				{	
					LogError(lLogFile, lRecordNumber, "Posture Prediction", string("Posture predition parameters must be an integer"));
					lOK = false;
					break;
				}
				HO = inputcheck.StringtoInt(temp);

				if(lInFile.fail()) {
					LogError(lLogFile, lRecordNumber, "Posture Prediction", string("Posture prediction parameters must be Left Horizontal, Vertical, Lateral; Right Horizontal, Vertical, Lateral; Hand Orientation"));
				} else {
					Vector3 LeftHand(LL, LH, LV);
					Vector3 RightHand(RL, RH, RV);
					// add offset because prediction doesn't use offset
					LeftHand += mDocPtr->getOffset();
					RightHand += mDocPtr->getOffset();
					// convert to metric because prediction assumes metric
					if(mDocPtr->Is_English()) {
						LeftHand *= IN_TO_CM;
						RightHand *= IN_TO_CM;
					}
					// perform posture prediction
					mDocPtr->PredictPosture(LeftHand, RightHand);
					// set hand orientation
					mDocPtr->getTask().Put_Hand_Orientation((E_Hand_Orientation)HO);
					// make the frame a keyframe if it is currently unset
					// TODO make keyframe as long as it is not a keyframe?
					if(mDocPtr->getFrameType() == FT_Unset) {
						mDocPtr->setFrameType(FT_Keyframe);
					}
				}
				mDocPtr->MakeDirtyAndUpdateViews(true); 
				if( mAutoExport ) mDocPtr->DoExportSummary(lExportFile, DM_COMMA, FM_APPEND); 
				break;
			}
			case bchLocations:
			{
				// get Skeleton* from loc data
				Skeleton* skel = mDocPtr->getTask().SkeletonFromLocString(lInFile);
				// set as current skeleton and mark as Loc
				mDocPtr->getTask().setFrame(mDocPtr->getTask().getCurrentFrame(), skel, FT_Loc);
				// analyze
				skel->setDirty();
				mDocPtr->getTask().Analyze();
				mDocPtr->MakeDirtyAndUpdateViews();
				break;
			}
		}
	}
      
      else //command not found
      {
         //lCommandName.Size_To_Fit();
         string   lMsg( "'" );
         lMsg += lCommandName += "' not found.";
         LogError( lLogFile, lRecordNumber, "Command", lMsg );
         lOK = false;
      }

	  // extract delimiter #
	  lInFile.ignore(lLineLength, lEndOfRecord);
      
      mProgressDialog.Update( lRecordNumber );
      lRecordNumber++;   
   }

   mProgressDialog.ShowWindow( SW_HIDE );
   AfxGetMainWnd()->SetFocus();

   if( lOK )
   {
	   ostrstream lMsg;
	   lMsg << lRecordNumber;
      lMsg << " commands processed with no errors." << ends;
      lLogFile << lMsg.str();
      AfxMessageBox( lMsg.str() );

   }
   else
   {
      string   lMsg( "Batch file completed with one or more errors. \nSee file '" );
      lMsg += mLogFileName += "'.";
	  AfxMessageBox( lMsg.c_str() );
   }

   lInFile.close();
   lExportFile.close();
   lLogFile.close();

   return   lOK;

}


