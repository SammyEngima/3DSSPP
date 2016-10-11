// DDESPPRT.CPP

#include "stdafx.h"

#include "ddespprt.h"

#include <stdlib.h>
#include "string.hpp" 

HSZ   DDE_System_StrHdl   ;
HSZ   DDE_Topic_StrHdl    ;
HSZ   DDE_Item_StrHdl[ Number_Of_String_Handles ];


/*------------------------------*/ void     //--------------------------------
/*module ::*/                                      DDE_Initialize_Item_Strings
(
DWORD   DDEIdentifier
)
      {
      DDE_System_StrHdl = 
         DdeCreateStringHandle( DDEIdentifier, "Test_Server",CP_WINANSI );
      
      DDE_Topic_StrHdl = 
         DdeCreateStringHandle( DDEIdentifier,  SZDDESYS_TOPIC, CP_WINANSI );
                                        
                                        //--- Array handles ---
      DDE_Item_StrHdl[ISH_Wireframes_Initialize] = 
         DdeCreateStringHandle( DDEIdentifier, "Wireframes Initialize",  CP_WINANSI );
      
      DDE_Item_StrHdl[ISH_Wireframe_Prepare_Next] = 
         DdeCreateStringHandle( DDEIdentifier, "Wireframe Prepare Next",  CP_WINANSI );
      
      DDE_Item_StrHdl[ISH_Wireframe_Get] = 
         DdeCreateStringHandle( DDEIdentifier, "Wireframe Get",  CP_WINANSI );
      
      DDE_Item_StrHdl[ISH_Wireframe_Prepare_Next_Cap] = 
         DdeCreateStringHandle( DDEIdentifier, "Wireframe Prepare Next Cap",  CP_WINANSI );
      
      DDE_Item_StrHdl[ISH_Wireframe_Cap_Get] = 
         DdeCreateStringHandle( DDEIdentifier, "Wireframe Get Cap",  CP_WINANSI );
      
      DDE_Item_StrHdl[ISH_UCS_Name_List_Clear] = 
         DdeCreateStringHandle( DDEIdentifier, "UCS Name List Clear",  CP_WINANSI );
      
      DDE_Item_StrHdl[ISH_UCS_Name_List_Add_String] = 
         DdeCreateStringHandle( DDEIdentifier, "UCS Name List Add String",  CP_WINANSI );
      
      DDE_Item_StrHdl[ISH_UCS_Purpose_Put] = 
         DdeCreateStringHandle( DDEIdentifier, "UCS Purpose Put",  CP_WINANSI );
      
      DDE_Item_StrHdl[ISH_UCS_Name_Get] = 
         DdeCreateStringHandle( DDEIdentifier, "UCS Name Get",  CP_WINANSI );
      
      DDE_Item_StrHdl[ISH_UCS_Position_Put] = 
         DdeCreateStringHandle( DDEIdentifier, "UCS Position Put",  CP_WINANSI );
                                           
                                           //--- Command string handles ---
      DDE_Item_StrHdl[CSH_Foot_Left] = 
         DdeCreateStringHandle( DDEIdentifier, "Foot Left",  CP_WINANSI );
      
      DDE_Item_StrHdl[CSH_Foot_Right] = 
         DdeCreateStringHandle( DDEIdentifier, "Foot Right",  CP_WINANSI );
      
      DDE_Item_StrHdl[CSH_Hand_Left] = 
         DdeCreateStringHandle( DDEIdentifier, "Hand Left",  CP_WINANSI );
      
      DDE_Item_StrHdl[CSH_Hand_Right] = 
         DdeCreateStringHandle( DDEIdentifier, "Hand Right",  CP_WINANSI );
      
      DDE_Item_StrHdl[CSH_World] = 
         DdeCreateStringHandle( DDEIdentifier, "World",  CP_WINANSI );
      
      DDE_Item_StrHdl[CSH_EndWaitCursor] = 
         DdeCreateStringHandle( DDEIdentifier, "StopWaitCursor",  CP_WINANSI );

      return;
      }

/*------------------------------*/ void     //--------------------------------
/*module ::*/                                            DDE_Free_Item_Strings
(
DWORD   DDEIdentifier
)
      {
      String   Fct_Name( "DDE_Free_Item_Strings" );
      String   Msg( "Problem releasing DDE string handle: " );
                                        //--- System and topic handles ---
      if( ! 
         DdeFreeStringHandle( DDEIdentifier, DDE_System_StrHdl )
        ) { DDE_Error( DDEIdentifier, Fct_Name, Msg + "DDE_System_StrHdl" ); return; }       
      if( ! 
         DdeFreeStringHandle( DDEIdentifier,  DDE_Topic_StrHdl )
        ) { DDE_Error( DDEIdentifier, Fct_Name, Msg + "DDE_Topic_StrHdl" ); return; }                                        

                                        //--- Arrayed item handles ---
      if( ! 
         DdeFreeStringHandle( DDEIdentifier, DDE_Item_StrHdl[ISH_Wireframes_Initialize] )
        ) { DDE_Error( DDEIdentifier, Fct_Name, Msg + "ISH_Wireframes_Initialize" ); return; }      
      if( ! 
         DdeFreeStringHandle( DDEIdentifier, DDE_Item_StrHdl[ISH_Wireframe_Prepare_Next] )
        ) { DDE_Error( DDEIdentifier, Fct_Name, Msg + "ISH_Wireframe_Prepare_Next" ); return; }      
      if( ! 
         DdeFreeStringHandle( DDEIdentifier, DDE_Item_StrHdl[ISH_Wireframe_Get] )
        ) { DDE_Error( DDEIdentifier, Fct_Name, Msg + "ISH_Wireframe_Get" ); return; }     
      if( !       
         DdeFreeStringHandle( DDEIdentifier, DDE_Item_StrHdl[ISH_Wireframe_Prepare_Next_Cap] )
        ) { DDE_Error( DDEIdentifier, Fct_Name, Msg + "ISH_Wireframe_Prepare_Next_Cap" ); return; }      
      if( !       
         DdeFreeStringHandle( DDEIdentifier, DDE_Item_StrHdl[ISH_Wireframe_Cap_Get] )
        ) { DDE_Error( DDEIdentifier, Fct_Name, Msg + "ISH_Wireframe_Cap_Get" ); return; }      
      if( !       
         DdeFreeStringHandle( DDEIdentifier, DDE_Item_StrHdl[ISH_UCS_Name_List_Clear] )
        ) { DDE_Error( DDEIdentifier, Fct_Name, Msg + "ISH_UCS_Name_List_Clear" ); return; }      
      if( !       
         DdeFreeStringHandle( DDEIdentifier, DDE_Item_StrHdl[ISH_UCS_Name_List_Add_String] )
        ) { DDE_Error( DDEIdentifier, Fct_Name, Msg + "ISH_UCS_Name_List_Add_String" ); return; }      
      if( !       
         DdeFreeStringHandle( DDEIdentifier, DDE_Item_StrHdl[ISH_UCS_Purpose_Put] )
        ) { DDE_Error( DDEIdentifier, Fct_Name, Msg + "ISH_UCS_Purpose_Put" ); return; }      
      if( !       
         DdeFreeStringHandle( DDEIdentifier, DDE_Item_StrHdl[ISH_UCS_Name_Get] )
        ) { DDE_Error( DDEIdentifier, Fct_Name, Msg + "ISH_UCS_Name_Get" ); return; }      
      if( !       
         DdeFreeStringHandle( DDEIdentifier, DDE_Item_StrHdl[ISH_UCS_Position_Put] )
        ) { DDE_Error( DDEIdentifier, Fct_Name, Msg + "ISH_UCS_Position_Put" ); return; }                                           

                                           //--- Command string handles ---
      if( !       
         DdeFreeStringHandle( DDEIdentifier, DDE_Item_StrHdl[CSH_Foot_Left] )
        ) { DDE_Error( DDEIdentifier, Fct_Name, Msg + "CSH_Foot_Left" ); return; }      
      if( !       
         DdeFreeStringHandle( DDEIdentifier, DDE_Item_StrHdl[CSH_Foot_Right] )
        ) { DDE_Error( DDEIdentifier, Fct_Name, Msg + "CSH_Foot_Right" ); return; }     
      if( !       
         DdeFreeStringHandle( DDEIdentifier, DDE_Item_StrHdl[CSH_Hand_Left] )
        ) { DDE_Error( DDEIdentifier, Fct_Name, Msg + "CSH_Hand_Left" ); return; }      
      if( !       
         DdeFreeStringHandle( DDEIdentifier, DDE_Item_StrHdl[CSH_Hand_Right] )
        ) { DDE_Error( DDEIdentifier, Fct_Name, Msg + "CSH_Hand_Right" ); return; }      
      if( !       
         DdeFreeStringHandle( DDEIdentifier, DDE_Item_StrHdl[CSH_World] )
        ) { DDE_Error( DDEIdentifier, Fct_Name, Msg + "CSH_World" ); return; }
      if( !       
         DdeFreeStringHandle( DDEIdentifier, DDE_Item_StrHdl[CSH_EndWaitCursor] )
        ) { DDE_Error( DDEIdentifier, Fct_Name, Msg + "CSH_EndWaitCursor" ); return; }                                           

      return;
      }

/*-------------------------------*/ void    //-------------------------------
/*module :: */                                      DDE_Data_Handle_To_String
( 
DWORD      DDEIdentifier ,
HDDEDATA   DataHdl ,
String&    TargetString 
)
   {
                                  //--- Get and validate data size ---
   DWORD   Bytes         = DdeGetData( DataHdl, NULL, 0, 0 );
   if( 
     DDE_Error( DDEIdentifier, 
               "DDE_Data_Handle_To_String", 
               "Retrieving data size." ) 
     ) return ;
   assert( Bytes > 1 );
                                  //--- Create string to hold data ---
   int     String_Length = Bytes - 1;
   assert( String_Length > 0 );
   TargetString.Redimension( String_Length );
//   String   String_Data( String_Length, "", ' ' );
                                  
                                  //--- Get the data ---   
   DdeGetData( DataHdl, (VOID_PTR)(  (char*)( TargetString )  ), Bytes, 0 );
   if(
     DDE_Error( DDEIdentifier, 
               "DDE_Data_Handle_To_String", 
               "Retrieving data from handle." ) 
     ) return ;

                                  //--- Free the data handle
   DdeFreeDataHandle( DataHdl );
   if( 
     DDE_Error( DDEIdentifier, 
               "DDE_Data_Handle_To_String", 
               "Freeing the data handle." )
     ) return  ;

   return;
   }


/*------------------------------*/ BOOL     //--------------------------------
/*module ::*/                                                        DDE_Error
(
DWORD   DDEIdentifier ,
const String&  FunctionNameRef ,
const String&  MessageRef
)
   {
   BOOL   Error_Occured  =  FALSE;
   
   int  DDE_Error_Number = DdeGetLastError( DDEIdentifier );
   if(  DDE_Error_Number !=  DMLERR_NO_ERROR )
      {
		String	Error_String;
		switch( DDE_Error_Number )
			{
			case DMLERR_ADVACKTIMEOUT : 
					{ Error_String = "ADVACKTIMEOUT";       break; } 
			case DMLERR_BUSY :              
					{ Error_String = "BUSY";                break; } 
			case DMLERR_DATAACKTIMEOUT : 
					{ Error_String = "DATAACKTIMEOUT";      break; } 
		   case DMLERR_DLL_NOT_INITIALIZED : 
					{ Error_String = "DLL_NOT_INITIALIZED"; break; } 
		   case DMLERR_DLL_USAGE : 
					{ Error_String = "DLL_USAGE";           break; } 
		   case DMLERR_EXECACKTIMEOUT : 
					{ Error_String = "EXECACKTIMEOUT";      break; } 
		   case DMLERR_INVALIDPARAMETER : 
					{ Error_String = "INVALIDPARAMETER";    break; } 
		   case DMLERR_LOW_MEMORY : 
					{ Error_String = "LOW_MEMORY";          break; } 
		   case DMLERR_MEMORY_ERROR : 
					{ Error_String = "MEMORY_ERROR";        break; } 
		   case DMLERR_NO_CONV_ESTABLISHED : 
					{ Error_String = "NO_CONV_ESTABLISHED"; break; } 
		   case DMLERR_NOTPROCESSED : 
					{ Error_String = "NOTPROCESSED";        break; } 
		   case DMLERR_POKEACKTIMEOUT : 
					{ Error_String = "POKEACKTIMEOUT";      break; } 
		   case DMLERR_POSTMSG_FAILED : 
					{ Error_String = "POSTMSG_FAILED";      break; } 
		   case DMLERR_REENTRANCY : 
					{ Error_String = "REENTRANCY";          break; } 
		   case DMLERR_SERVER_DIED : 
					{ Error_String = "SERVER_DIED";         break; } 
		   case DMLERR_SYS_ERROR : 
					{ Error_String = "SYS_ERROR";           break; } 
		   case DMLERR_UNADVACKTIMEOUT :
					{ Error_String = "UNADVACKTIMEOUT";     break; } 
		   case DMLERR_UNFOUND_QUEUE_ID :
					{ Error_String = "UNFOUND_QUEUE_ID";    break; } 
			default:
				{
				String   Error_Number( 10, "", ' ' );
				_itoa( DDE_Error_Number, (char*)Error_Number, 10 );
      		Error_String = Error_Number;
				}
			
			} //end switch on error number
      String   Error_Msg( "DDE error: " );
		Error_Msg = Error_Msg 
                + Error_String 
                + ": "
                + MessageRef;
		MessageBox( NULL, Error_Msg, FunctionNameRef, MB_OK );		
      Error_Occured = TRUE;      
		}//end if error occured
   
	return   Error_Occured;   
   }
