/* (MPT) This file isn't supposed to build, but it does anyway, so commented out */

////Make sure NO_VBX_SUPPORT is defined for app.cpp before include stdafx.h.
//#include <ddeml.h>
//#include <assert.h>
//#include <strstream>
//
//#include "ddespprt.h" //DDE support functions
//#include "hom_doc.h" // Necessary
//
//#include "Dg_AC_Conversation.h"
//
////=== DDE Module Scope Variables ============================================
//static HINSTANCE    App_Instance_Hdl;
//static CWnd*        Wnd_Instance_Ptr;
//static HWND         Main_Window_Hdl;
//static HWND         Client_Window_Hdl;
//static WORD         AutoCAD_WndMsg;
//
//static DWORD        DDE_Identifier;   
//static FARPROC      DDE_Callback_Ptr;
//
//static HCONV        DDE_Conversation_Hdl;
//static int          DDE_Conversation_Active;
//
//static UCS_Purpose  UCS_Context; //Currently active UCS purpose 
//
//static Dg_AC_Conversation*   AC_Conversation_Dialog_Ptr = NULL;
//
////=== DDE Global Scope Functions =============================================
//extern "C"
//   {
//   #ifndef WIN32
//      HDDEDATA EXPENTRY _export DdeCallback
//   #else
//      HDDEDATA CALLBACK DdeCallback
//   #endif
//	                     (
//	                     WORD        XType ,
//	                     WORD        ClipboardFormat ,
//	                     HCONV       ConversationHdl ,
//	                     HSZ         String1Hdl ,
//	                     HSZ         String2Hdl ,
//	                     HDDEDATA    DataHdl ,
//	                     DWORD       Data1 ,
//	                     DWORD       Data2
//	                     );
//
//   #ifdef  WIN32                  //For finding and setting the AutoCAD 
//                                  //version 13+ client window handle
//      BOOL CALLBACK EnumWindowsProc
//         (
//         HWND  hwnd,	// handle to parent window
//         LPARAM  lParam 	// application-defined value
//         );
//   #endif	  
//
//   void         DDE_Kick_AutoCAD( const char* Action );
//   } //end of extern "C" bracket
//
//
////=== DDE Module Scope Functions =============================================
//static int          DDE_Environment_Ready       ( void );
//static int          DDE_Initialize_DDEMgr       ( void );
//static int          DDE_Uninitialize_DDEMgr     ( void );
//static BOOL         DDE_Conversation_In_Progress( void );
//
//static HDDEDATA     DDE_Wireframes_Initialize   ( void );
//static HDDEDATA     DDE_Wireframe_Prepare_Next  ( void );
//static HDDEDATA     DDE_Wireframe_Get           ( void );
//static HDDEDATA     DDE_Wireframe_Prepare_Next_Cap  ( void );
//static HDDEDATA     DDE_Wireframe_Cap_Get           ( void );
//                    
//        
//static HDDEDATA     DDE_UCS_Name_List_Clear     ( void );
//static HDDEDATA     DDE_UCS_Name_List_Add_String( HDDEDATA  DataHdl );
//static HDDEDATA     DDE_UCS_Name_Get            ( void );
//static HDDEDATA     DDE_UCS_Position_Put        ( HDDEDATA  DataHdl );
//static HDDEDATA     DDE_UCS_Purpose_Put         ( HDDEDATA  DataHdl );
//                    
//static BOOL         DDE_Exit_Instance           ( void );       
//
//
///*------------------------------*/ int      //-------------------------------
///*module ::*/                                           DDE_Environment_Ready
//( void )
////Returns true if Windows is in protected mode. Always returns true if
////compiled as a 32 bit application. 
//   {
//   int   OK = 1;
//   #ifndef WIN32
//      DWORD   Win_Flags = GetWinFlags();
//      OK = (int)( Win_Flags & WF_PMODE );
//   #endif
//   return   OK;
//   }
//
///*------------------------------*/ int      //-------------------------------
///*module ::*/                                           DDE_Initialize_DDEMgr
//( void )
//   {
//   if( ! DDE_Environment_Ready() )
//      {
//      MessageBox
//         ( NULL, "Error: Invalid Windows mode.", 
//           "CServerApp::DDE_Initialize", MB_OK );
//      return   FALSE; //<------------------------------------------ Exit
//      }
//
//                                  //--- Initilialize module variables ---
//   DDE_Identifier           = 0;
//   DDE_Callback_Ptr         = 0;
//   DDE_Conversation_Hdl     = NULL;
//   DDE_Conversation_Active  = 0;
//   Client_Window_Hdl        = NULL;
//   AutoCAD_WndMsg           = NULL;
//
//   int   DDE_Status 
//              = DdeInitialize
//                  ( 
//                  &(DDE_Identifier), //receives instance identifier 
//		  (PFNCALLBACK)( DdeCallback ),
//                    CBF_SKIP_REGISTRATIONS 
//                  | CBF_FAIL_ADVISES ,
//                 0L
//                  );
//   int   OK;
//   switch( DDE_Status )
//      {
//      case DMLERR_DLL_USAGE:
//         { 
//         MessageBox( NULL, "DLL_USAGE", "CServerApp::DDE_Initialize", MB_OK); 
//         OK = 0;
//         break;
//         }
//      case DMLERR_INVALIDPARAMETER:
//         { 
//         MessageBox( NULL, "INVALIDPARAMETER", "CServerApp::DDE_Initialize", MB_OK); 
//         OK = 0;
//         break;
//         }
//      case DMLERR_SYS_ERROR:
//         { 
//         MessageBox( NULL, "SYS_ERROR", "CServerApp::DDE_Initialize", MB_OK); 
//         OK = 0;
//         break;
//         }
//      case DMLERR_NO_ERROR: OK = 1; break; 
//      }
//   
//   if( OK )
//      {
//      DDE_System_StrHdl = DdeCreateStringHandle
//                                    ( 
//                                    DDE_Identifier,
//                                    "Test_Server",
//                                    CP_WINANSI
//                                    );
//      int Register_OK = (int)DdeNameService
//                                ( 
//                                DDE_Identifier, 
//                                DDE_System_StrHdl,
//                                NULL,
//                                DNS_REGISTER
//                                );
//                                  
//                                  //--- Get handles for topic and items ---
//      DDE_Initialize_Item_Strings( DDE_Identifier );
//
//      if( Register_OK ) OK = 1;
//      else              OK = 0;         
//      }                      
//
//   return   OK;       
//   }   
//
///*------------------------------*/ int      //-------------------------------
///*module ::*/                                         DDE_Uninitialize_DDEMgr
//( void )
//   {
//   int Terminate_OK = int(DdeNameService //Remove service names
//                             ( 
//                             DDE_Identifier, 
//                             NULL,
//                             NULL,
//                             DNS_UNREGISTER
//                             ));
//   if( Terminate_OK ) 
//      {
//      Terminate_OK = DdeUninitialize( DDE_Identifier ); 
//      if( ! Terminate_OK )
//         MessageBox( NULL, "Error: Uninitialize", 
//                    "CServerApp::DDE_Uninitialize_DDEMgr", MB_OK );
//      }
//   else
//      {
//      MessageBox( NULL, "Error: DdeNameService", 
//                 "CServerApp::DDE_Uninitialize_DDEMgr", MB_OK );
//      }
//      
//   return   Terminate_OK;   
//   }
//
///*------------------------------*/ BOOL     //-------------------------------
///*global ::*/                                    DDE_Conversation_In_Progress
//( void )
//   {
//   return  DDE_Conversation_Active; 
//   }
//
///*------------------------------*/ void     //-------------------------------
///*global ::*/                                                DDE_Kick_AutoCAD
//( 
//const char*   Action
//)
////NOTE: Incoming action string must also contain "\n" at its end to cause
////AutoCAD to issue a carriage return.
////SendMessage per the example on "Using AutoCAD" p 201.                                  
//   {
//   if(  DDE_Conversation_Active  ) //Conversation in progress 
//      {
//      if(  Client_Window_Hdl != NULL ) //AutoCAD window exists
//         {
//         assert( Action != NULL );
//													//Create and open the autocad conversation
//			                              //in progress dialog. If the pointer is
//													//already not NULL, a dialog is already 
//													//open from a prior call to DDE_Kick.
//													//Do not open again.
//			if( AC_Conversation_Dialog_Ptr == NULL )
//			   {
//				AC_Conversation_Dialog_Ptr = new   Dg_AC_Conversation();
//				}
//
//	 #ifdef WIN32 //Is AutoCAD 13+
//	    
//	    String   Action_String( Action );
//	    COPYDATASTRUCT   Data;
//	    Data.dwData = 1;
//	    Data.cbData = Action_String.Get_Length() + 1;
//	    Data.lpData = (PVOID)Action;
//	    
//	    int Send_Success = 
//	    SendMessage
//	       ( Client_Window_Hdl, WM_COPYDATA, (WPARAM)Main_Window_Hdl, (LPARAM)(&Data) );
//	    if( ! Send_Success ) 
//	       MessageBox
//	          ( 
//		  NULL, 
//		  "Problem sending message to AutoCAD.", 
//		  "AutoCAD Refresh", 
//		  MB_OK 
//		  );
//	 	 
//	 #else //Is AutoCAD 12
//	    SendMessage
//	       ( Client_Window_Hdl, AutoCAD_WndMsg, 0, (long)(LPBYTE)Action );
//	 #endif
//         }
//      
//      else
//         {
//         MessageBox( NULL, "AutoCAD window handle is NULL.", "AutoCAD Refresh", MB_OK ); 
//         }  
//      }
//
//   else                           //Conversation NOT in progress
//      {
//      MessageBox( NULL, "AutoCAD not connected.", "AutoCAD", MB_OK );
//      }   
//	
//	return;
//   }
//
///*-------------------------------*/ HDDEDATA //-------------------------------
///*module :: */                                       DDE_Wireframes_Initialize
//(void )
////Called by AutoCAD through the Dde_Callback interface to access the current
////number of available mesh graphics for subsequent display in AutoCAD.
//   {
//   HDDEDATA   Data_Hdl;
//    
//   int   Count;
//   Count = DDE_GDB_Ptr->Graphics_Reset( DDE_Doc_Ptr->Get_Com_Name() );
//  
//   Data_Hdl = DdeCreateDataHandle
//      ( DDE_Identifier, 
//       (LPBYTE)( &Count ), sizeof( Count ),                  
//        0L, DDE_Item_StrHdl[ ISH_Wireframes_Initialize ], CF_TEXT, 0
//      );
//
//   return   Data_Hdl;
//   }
//
//static Flt_3D_Mesh*   Current_Mesh_Ptr;
//static int            DDE_Cap_Counter;
///*-------------------------------*/ HDDEDATA //-------------------------------
///*module :: */                                      DDE_Wireframe_Prepare_Next
//(void )
////Called by AutoCAD through the Dde_Callback interface to access the row and
////column dimensions and the number of caps of the next mesh to be displayed 
////in AutoCAD.
//   {
//   HDDEDATA   Data_Hdl;
//   
//   Current_Mesh_Ptr = &( DDE_GDB_Ptr->
//                            Graphics_Get_Next_Mesh_Ref
//                               (   DDE_Doc_Ptr->Get_Com_Name()  ) 
//                       );
//
//   int   Dimensions[3];
//   Dimensions[ 0 ] = Current_Mesh_Ptr->Get_Vertex_Rows()   ;
//   Dimensions[ 1 ] = Current_Mesh_Ptr->Get_Vertex_Columns();
//   Dimensions[ 2 ] = Current_Mesh_Ptr->Get_Number_Of_Caps();
//
//   DDE_Cap_Counter = 0;
//   
//   Data_Hdl = DdeCreateDataHandle
//      ( DDE_Identifier, 
//       (LPBYTE)( Dimensions ), sizeof( Dimensions ),                  
//        0L, DDE_Item_StrHdl[ ISH_Wireframe_Prepare_Next ], CF_TEXT, 0
//      );
//   
//   return   Data_Hdl;
//   }
//
///*-------------------------------*/ HDDEDATA //-------------------------------
///*module :: */                                               DDE_Wireframe_Get
//(void )
////Called by AutoCAD through the Dde_Callback interface to get the actual 
////mesh data for display in AutoCAD.
//   {
//   HDDEDATA   Data_Hdl;
//                  
//                                     //--- Transform the polyline ---
//   Flt_3D_Polyline   Scaled;
//   Scaled = *Current_Mesh_Ptr;
//   Scaled.Transform( DDE_Doc_Ptr->Get_AC_Transform() );              
//
//                                     //--- Scale the mesh to AutoCAD units ---
//   Scaled = Scaled * DDE_Doc_Ptr->Get_AC_Units_Conversion();
//
//                                     //--- Assemble the data for transfer ---
//   double&   Float_Ref = ( Scaled( 0 ) )( 0 );
//   char*    Data_Ptr = (char*)( &Float_Ref );
//
//   int     Bytes = Scaled.Array::Get_Memory_Size();
//
//   Data_Hdl = DdeCreateDataHandle
//      ( DDE_Identifier, 
//       (LPBYTE)( Data_Ptr ), Bytes ,                  
//        0L, DDE_Item_StrHdl[ ISH_Wireframe_Get ], CF_TEXT, 0
//      );
//
//   return   Data_Hdl;
//   }
//
///*-------------------------------*/ HDDEDATA //-------------------------------
///*module :: */                                  DDE_Wireframe_Prepare_Next_Cap
//(void )
////Called by AutoCAD through the Dde_Callback interface to access number of
////vertices for the next cap on the current mesh. 
//   {
//   HDDEDATA   Data_Hdl;
//   
//   int   Cap_Vertices = Current_Mesh_Ptr->Get_Vertex_Columns();
//   
//   Data_Hdl = DdeCreateDataHandle
//      ( DDE_Identifier, 
//       (LPBYTE)( &Cap_Vertices ), sizeof( Cap_Vertices ),                  
//        0L, DDE_Item_StrHdl[ ISH_Wireframe_Prepare_Next_Cap ], CF_TEXT, 0
//      );
//   
//   return   Data_Hdl;
//   }
//
///*-------------------------------*/ HDDEDATA //-------------------------------
///*module :: */                                           DDE_Wireframe_Cap_Get
//(void )
////Called by AutoCAD through the Dde_Callback interface to get the actual 
////mesh data for display in AutoCAD.  Also increments the DDE cap
////counter which is zeroed in DDE_Wireframe_Prepare_Next_Mesh.
//   {
//   HDDEDATA   Data_Hdl;
//                                     //--- Transform the polyline ---
//   Flt_3D_Polyline   Scaled;
//   Scaled = Current_Mesh_Ptr->Get_Cap_Polyline( DDE_Cap_Counter );
//   Scaled.Transform( DDE_Doc_Ptr->Get_AC_Transform() );              
//
//                                     //--- Scale the polyline ---
//   Scaled = Scaled * DDE_Doc_Ptr->Get_AC_Units_Conversion();
//
//                                     //--- Assemble the data for transfer ---
//   double&   Float_Ref = ( Scaled( 0 ) )( 0 );
//   char*    Data_Ptr = (char*)( &Float_Ref );
//
//   int     Bytes = Scaled.Array::Get_Memory_Size();
//
//   Data_Hdl = DdeCreateDataHandle
//      ( DDE_Identifier, 
//       (LPBYTE)( Data_Ptr ), Bytes ,                  
//        0L, DDE_Item_StrHdl[ ISH_Wireframe_Cap_Get ], CF_TEXT, 0
//      );
//
//   DDE_Cap_Counter ++;
//
//   return   Data_Hdl;
//   }
//
///*-------------------------------*/ HDDEDATA //-------------------------------
///*module :: */                                         DDE_UCS_Name_List_Clear
//( void )
//   {
//   assert(  DDE_App_Ptr != 0 );
//   //DDE_App_Ptr->UCS_Name_List.Delete_Objects();
//   DDE_App_Ptr->UCS_Name_List.clear();
//  
//   int   Dummy_Data = 1;
//   HDDEDATA   Data_Hdl;
//   Data_Hdl = DdeCreateDataHandle
//      ( DDE_Identifier, 
//       (LPBYTE)( &Dummy_Data ), sizeof( Dummy_Data ),                  
//        0L, DDE_Item_StrHdl[ ISH_UCS_Name_List_Clear ], CF_TEXT, 0
//      );
//
//   return   Data_Hdl;
//   }
//
///*-------------------------------*/ HDDEDATA //-------------------------------
///*module :: */                                    DDE_UCS_Name_List_Add_String
//(
//HDDEDATA   DataHdl
//)
////Response to client poking a name onto the application's list of UCS names.
//   {
//   String   UCS_Name;
//   DDE_Data_Handle_To_String(DDE_Identifier, DataHdl, UCS_Name);
//   //DDE_App_Ptr->UCS_Name_List + UCS_Name;
//   DDE_App_Ptr->UCS_Name_List.push_back((char*)UCS_Name);
//   return   (HDDEDATA)TRUE;
//   }
///*-------------------------------*/ HDDEDATA //-------------------------------
///*module :: */                                             DDE_UCS_Purpose_Put
//(
//HDDEDATA   DataHdl
//)
////Response to client poking the currently active UCS context. This context is
////used in subsequent transactions WRT UCS information to insure that the proper
////body of information is transacted. Transactions effected are 
////DDE_UCS_Name_Get and DDE_UCS_Position_Put.
//   {
//                                  //--- Get the data ---   
//#ifndef	WIN32   
//	DdeGetData( DataHdl, (void*)( &UCS_Context ), sizeof( UCS_Context ), 0 );
//#else
//	DdeGetData( DataHdl, (LPBYTE)( &UCS_Context ), sizeof( UCS_Context ), 0 );
//#endif
//	
//   if(
//     DDE_Error( DDE_Identifier, 
//               "DDE_UCS_Purpose_Put", 
//               "Retrieving UCS context data." ) 
//     ) return  (HDDEDATA)FALSE;
//
//                                  //--- Free the data handle
//   DdeFreeDataHandle( DataHdl );
//   if( 
//     DDE_Error( DDE_Identifier, 
//               "DDE_UCS_Purpose_Put", 
//               "Freeing the data handle." )
//     ) return   (HDDEDATA)FALSE;
//   
//   return   (HDDEDATA)TRUE;
//   }
//
///*-------------------------------*/ HDDEDATA //-------------------------------
///*module :: */                                            DDE_UCS_Position_Put
//(
//HDDEDATA   DataHdl
//)
////Response to client poking the position corresponding to the currently
////selected UCS name. Position is converted from AutoCAD units to internal
////system units (millimeters) at this time.
//   {
//   UCS_Position   Where;
//                                  //--- Get and validate data size ---
//   DWORD   Bytes  =  DdeGetData( DataHdl, NULL, 0, 0 );
//   if( 
//     DDE_Error( DDE_Identifier, 
//               "DDE_UCS_Position_Put", 
//               "Retrieving data size." ) 
//     ) return  (HDDEDATA)FALSE;
//   DWORD   Size_Of_Where = (DWORD)( sizeof( Where ) );  
////   assert(  Bytes == Size_Of_Where  );
//   
//                                  //--- Get the data ---   
//   DdeGetData( DataHdl, (VOID_PTR)( &Where ), Size_Of_Where, 0 );
//   if(
//     DDE_Error( DDE_Identifier, 
//               "DDE_UCS_Position_Put", 
//               "Retrieving data from handle." ) 
//     ) return  (HDDEDATA)FALSE;
//
//                                  //--- Free the data handle
//   DdeFreeDataHandle( DataHdl );
//   if( 
//     DDE_Error( DDE_Identifier, 
//               "DDE_UCS_Position_Put", 
//               "Freeing the data handle." )
//     ) return   (HDDEDATA)FALSE;
//   
////!!!Diagnostics
////   ostrstream   Where_Str;
////   Where_Str 
////          <<   "Origin X: " << Where.Origin[0] 
////                  << " Y: " << Where.Origin[1] 
////                  << " Z: " << Where.Origin[2] 
////          << "\nX_Axis X: " << Where.X_Axis[0] 
////                  << " Y: " << Where.X_Axis[1] 
////                  << " Z: " << Where.X_Axis[2] 
////          << "\nY_Axis X: " << Where.Y_Axis[0] 
////                  << " Y: " << Where.Y_Axis[1] 
////                  << " Z: " << Where.Y_Axis[2] << ends; 
////   char*   Where_Text = Where_Str.str();                
////   AfxMessageBox( Where_Text ); 
////   delete Where_Text;
//                                  
//                                  //--- Convert from AutoCAD definition to 
//                                  //    internal system position and assign 
//                                  //    to application UCS position selected.                                     
//   double System_To_AC  =  DDE_Doc_Ptr->Get_AC_Units_Conversion();
//   assert( System_To_AC != 0.0 );
//   double Convert_Units = 1.0 / DDE_Doc_Ptr->Get_AC_Units_Conversion();
//   Flt_3D_Point   Origin( //Create origin converted to system units
//                        Convert_Units * Where.Origin[0] , 
//                        Convert_Units * Where.Origin[1] , 
//                        Convert_Units * Where.Origin[2] 
//                        );
//   Flt_3D_Point   X_Axis( Where.X_Axis );
//   Flt_3D_Point   Y_Axis( Where.Y_Axis );
//   Flt_3D_Point   Z_Point; Z_Point = Origin + X_Axis.Cross( Y_Axis );
//   Flt_3D_Point   X_Point; X_Point = Origin + X_Axis;
//   
//   Position Temp( Origin, Z_Point, X_Point );   
//   DDE_Doc_Ptr->UCS_Position_Put( UCS_Context, Temp ); 
//   
//   return   (HDDEDATA)TRUE;
//   }
//
///*-------------------------------*/ HDDEDATA //-------------------------------
///*module :: */                                                DDE_UCS_Name_Get
//( void )
////Response to client's request for the name of the selected UCS. NOTE: The
////returned name is based on the currently active UCS_Context.
//   {
//   String   UCS_Name;
//   UCS_Name = DDE_Doc_Ptr->UCS_Name_Get( UCS_Context );
//   
//   HDDEDATA   Data_Hdl = DdeCreateDataHandle
//      ( 
//      DDE_Identifier, 
//      (LPBYTE)(  (char*)UCS_Name  ), UCS_Name.Array::Get_Memory_Size() ,                  
//      0L, DDE_Item_StrHdl[ ISH_UCS_Name_Get ], CF_TEXT, 0
//      );
//   
//   DDE_Error( DDE_Identifier, "DDE_UCS_Name_Get", "Creating UCS name handle." ); 
//   
//   return   Data_Hdl;
//   }
//
///*------------------------------*/ BOOL     //--------------------------------
///*module ::*/                                                DDE_Exit_Instance
//( void )
//   {                              //Force a termination of any conversation
//                                  //if a conversation is in progress.
//   if(  DDE_Conversation_In_Progress()  )
//      DdeDisconnect( DDE_Conversation_Hdl );
//                                  //Free the DDEMgr resources
//   DDE_Uninitialize_DDEMgr();
//   return   TRUE;   
//   }
//
/////*------------------------------*/ HDDEDATA EXPENTRY _export  //----------------
////*global*/                                                            DdeCallback
////NOTE: In this development system HDDEDATA EXPENTRY = DWORD far pascal.
//#ifndef WIN32
//   HDDEDATA EXPENTRY _export DdeCallback
//#else
//   HDDEDATA CALLBACK DdeCallback
//#endif
//(
//WORD        XType ,
//WORD        ClipboardFormat ,
//HCONV       ConversationHdl ,
//HSZ         String1Hdl ,
//HSZ         String2Hdl ,
//HDDEDATA    ObjectHdl ,
//DWORD       Data1 ,
//DWORD       Data2
//)
//   {    
//   HDDEDATA   Response;
//   switch( XType )
//      {
//      case /*-------------------------------------*/  XTYP_CONNECT:
//         {
//         Response = (HDDEDATA)TRUE;
//         if( DDE_Conversation_Active ) //Only one conversation allowed 
//            {
//            String   Msg( "Only one AutoCAD connection allowed. " );
//            Msg = Msg + "If a program error has caused an unintentional disconnect ";
//            Msg = Msg + "from AutoCAD, save your 3DSSPP work, exit 3DSSPP, and ";
//            Msg = Msg + "then attempt the connection from AutoCAD again.";
//            MessageBox( NULL, (char*)Msg, "Connect to AutoCAD", MB_OK );
//            Response = (HDDEDATA)FALSE;
//            }
//  
//         else if( DdeCmpStringHandles( String1Hdl, DDE_Topic_StrHdl ) != 0 )
//            {
//            Response = (HDDEDATA)FALSE;
//            }
//         break;
//         }   
//
//      case /*-------------------------------------*/  XTYP_CONNECT_CONFIRM:
//         //Store conversation handle. 
//         //Attempt to find client window handle.
//         //Attempt to register window message type.
//         {
//         DDE_Conversation_Active = 1;
//         DDE_Conversation_Hdl    = ConversationHdl;
//
//       	 #ifdef WIN32
//	    ::EnumWindows( EnumWindowsProc, 0L ); //Works with EnumWindowProc
//	                                           //below to get the AutoCAD
//					 	   //application window handle.
//	 #else
//	    Client_Window_Hdl = FindWindow( "AutoCAD.gfx", NULL );
//         #endif
//	 
//	 if(  Client_Window_Hdl != NULL )
//            {
//            AutoCAD_WndMsg = RegisterWindowMessage( "WM_ACAD");
//            if( AutoCAD_WndMsg == 0 )
//               MessageBox( NULL, "Error: Register AutoCAD message failed.",
//                                 "Server XTYP_CONNECT_CONFIRM", MB_OK );
//            }
//         else
//            {
//            MessageBox( NULL, "Error: Find AutoCAD window failed.",
//                              "Server XTYP_CONNECT_CONFIRM", MB_OK );
//            }   
//
//         AC_Conversation_Dialog_Ptr = NULL;
//			Response = (HDDEDATA)NULL;   
//         break;
//         }
//
//      case /*-------------------------------------*/  XTYP_DISCONNECT:
//         //Terminate this instance of the application. This application's only
//         //reason for living is to serve a single client at a time.
//         //CServerApp::ExitInstance() will disconnect any conversation and
//         //deinitialize the DDEMgr. However, we must release the conversation
//         //handle here so that the exit functions do not issue yet another
//         //DdeDisconnect to the client.
//         {
//         DDE_Conversation_Active = 0;
//         DDE_Conversation_Hdl = NULL;
//         PostMessage( Main_Window_Hdl, WM_CLOSE, 0, 0 );
//         Response = (HDDEDATA)NULL;
//         break;
//         }
//
//      case /*-------------------------------------*/  XTYP_POKE:
//         {
//         if( String1Hdl == DDE_Topic_StrHdl )
//            {
//            if( String2Hdl == DDE_Item_StrHdl[ ISH_UCS_Name_List_Add_String ] ) 
//               {
//               DDE_UCS_Name_List_Add_String( ObjectHdl );
//               Response = (HDDEDATA)DDE_FACK;
//               }
//            else if( String2Hdl == DDE_Item_StrHdl[ ISH_UCS_Purpose_Put ] ) 
//               {
//               DDE_UCS_Purpose_Put( ObjectHdl );
//               Response = (HDDEDATA)DDE_FACK;
//               }
//            else if( String2Hdl == DDE_Item_StrHdl[ ISH_UCS_Position_Put ] ) 
//               {
//               DDE_UCS_Position_Put( ObjectHdl );
//               Response = (HDDEDATA)DDE_FACK;
//               }
//
//														//--- Commands ---
//            else if( String2Hdl == DDE_Item_StrHdl[ CSH_Foot_Left ] ) 
//               {
//               (DDE_App_Ptr->Get_Document_Ptr())->Foot_Left_Place();
//               Response = (HDDEDATA)DDE_FACK;
//               DdeFreeDataHandle( ObjectHdl );
//               }
//            else if( String2Hdl == DDE_Item_StrHdl[ CSH_Foot_Right ] ) 
//               {
//               (DDE_App_Ptr->Get_Document_Ptr())->Foot_Right_Place();
//               Response = (HDDEDATA)DDE_FACK;
//               DdeFreeDataHandle( ObjectHdl );
//               }
//            else if( String2Hdl == DDE_Item_StrHdl[ CSH_Hand_Left ] ) 
//               {
//               (DDE_App_Ptr->Get_Document_Ptr())->Hand_Left_Place();
//               Response = (HDDEDATA)DDE_FACK;
//               DdeFreeDataHandle( ObjectHdl );
//               }
//            else if( String2Hdl == DDE_Item_StrHdl[ CSH_Hand_Right ] ) 
//               {
//               (DDE_App_Ptr->Get_Document_Ptr())->Hand_Right_Place();
//               Response = (HDDEDATA)DDE_FACK;
//               DdeFreeDataHandle( ObjectHdl );
//               }
//            else if( String2Hdl == DDE_Item_StrHdl[ CSH_World ] ) 
//               {
//               (DDE_App_Ptr->Get_Document_Ptr())->World_Place();
//               Response = (HDDEDATA)DDE_FACK;
//               DdeFreeDataHandle( ObjectHdl );
//               }
//            else if( String2Hdl == DDE_Item_StrHdl[ CSH_EndWaitCursor ] ) 
//               {
//               									//Turn off the ac conversation
//														//dialog
//					if( AC_Conversation_Dialog_Ptr != NULL )
//					   {
//						AC_Conversation_Dialog_Ptr->SendMessage( WM_CLOSE );
//						AC_Conversation_Dialog_Ptr = NULL;
//						}
//               
//					Response = (HDDEDATA)DDE_FACK;
//               DdeFreeDataHandle( ObjectHdl );
//               }
//            
//            else //Item not recognized
//               {
//               Response = (HDDEDATA)NULL;
//					AfxMessageBox( "Unknown item string handle." );
//				   }
//            }
//         
//         else //Topic not recognized
//            {
//            Response = (HDDEDATA)NULL;
//            }   
//
//         break;
//         }
//
//      case /*-------------------------------------*/  XTYP_REQUEST:
//         {
//         if( String1Hdl == DDE_Topic_StrHdl )
//            {
//            if( String2Hdl == DDE_Item_StrHdl[ISH_Wireframes_Initialize] ) 
//               {
//               Response = DDE_Wireframes_Initialize();
//               }
//            else if( String2Hdl == DDE_Item_StrHdl[ISH_Wireframe_Prepare_Next] ) 
//               {
//               Response = DDE_Wireframe_Prepare_Next();
//               }
//            else if( String2Hdl == DDE_Item_StrHdl[ISH_Wireframe_Get] ) 
//               {
//               Response = DDE_Wireframe_Get();
//               }
//            else if( String2Hdl == DDE_Item_StrHdl[ISH_Wireframe_Prepare_Next_Cap] ) 
//               {
//               Response = DDE_Wireframe_Prepare_Next_Cap();
//               }
//            else if( String2Hdl == DDE_Item_StrHdl[ISH_Wireframe_Cap_Get] ) 
//               {
//               Response = DDE_Wireframe_Cap_Get();
//               }
//            else if( String2Hdl == DDE_Item_StrHdl[ISH_UCS_Name_List_Clear] ) 
//               {
//               Response = DDE_UCS_Name_List_Clear();
//               }
//            else if( String2Hdl == DDE_Item_StrHdl[ISH_UCS_Name_Get] ) 
//               {
//               Response = DDE_UCS_Name_Get();
//               }
//            else //Item not recognized
//               {
//               Response = (HDDEDATA)NULL;
//               }
//            }
//         
//         else //Topic not recognized
//            {
//            Response = (HDDEDATA)NULL;
//            }   
//         
//         break;
//         }
//
//      default: //----------------------------------   default
//         {
//         Response = (HDDEDATA)NULL;
//         break;
//         }
//      }
//   return   Response;
//   }
//
////----------------------------------------------------------------------------
//#ifdef  WIN32                  //For finding and setting the AutoCAD 
//                               //version 13+ client window handle
//BOOL CALLBACK EnumWindowsProc
//(
//HWND    Hwnd,	// handle to parent window
//LPARAM  LParam 	// application-defined value
//)
//   {
//   const int   Text_Length = 20;
//   char  Window_Text[Text_Length + 1];
//   int   Window_Text_Status = GetWindowText( Hwnd, Window_Text, Text_Length );
//   
//   if( Window_Text_Status != 0 ) 
//      {
//      CString   Test_String( Window_Text );
//      //CString   Msg( "Test window string: " );
//      //Msg += Test_String;
//      //AfxMessageBox( Msg );
//      if( Test_String.Left( 7 ) == "AutoCAD" )   Client_Window_Hdl = Hwnd;
//      else					   Client_Window_Hdl = NULL;
//      }  
//   
//   else //window has no title bar or some other problem occured
//      {
//      //AfxMessageBox( "EnumWindowsProc: Problem encountered getting window text." );
//      Client_Window_Hdl = NULL;
//      }
//
//   if( Client_Window_Hdl != NULL ) return FALSE; //Stops EnumWindow
//   else                            return TRUE;
//   }
//
//#endif	  
//     