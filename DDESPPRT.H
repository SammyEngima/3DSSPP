// DDESPPRT.H

#ifndef   DDESPPRT_INCLUDE
#define   DDESPPRT_INCLUDE

#include <windows.h>
#include <ddeml.h>

class String;

#ifndef WIN32 //Is Windows 3.x 16 bit
   #define VOID_PTR    void*
   #define DllExport   _export

#else //is Win32
   #define VOID_PTR    unsigned char*
   #define DllExport   __declspec( dllexport )
#endif

struct   UCS_Position
            {
            double   Origin[ 3 ];
            double   X_Axis[ 3 ];
            double   Y_Axis[ 3 ];
            UCS_Position( void )
               { 
               for( int Idx = 0; Idx < 3; Idx++ )
                  {
                  Origin[Idx] = 0.0F;
                  X_Axis[Idx] = 0.0F;
                  Y_Axis[Idx] = 0.0F;
                  }
               return;
               };
            };


#define  Number_Of_String_Handles  16
enum   Item_String_Handles
          {
          ISH_Wireframes_Initialize ,
          ISH_Wireframe_Prepare_Next ,
          ISH_Wireframe_Get ,
          ISH_Wireframe_Prepare_Next_Cap ,
          ISH_Wireframe_Cap_Get ,
          ISH_UCS_Name_List_Clear ,
          ISH_UCS_Name_List_Add_String ,
          ISH_UCS_Purpose_Put , //!!!
          ISH_UCS_Name_Get ,
          ISH_UCS_Position_Put ,
          CSH_Foot_Left ,
          CSH_Foot_Right ,
          CSH_Hand_Left ,
          CSH_Hand_Right ,
          CSH_World ,
			 CSH_EndWaitCursor
          };
          
enum   UCS_Purpose
          {
          UP_FOOT_LEFT ,
          UP_FOOT_RIGHT ,
          UP_HAND_LEFT ,
          UP_HAND_RIGHT ,
          UP_WORLD 
          };

extern "C" 
   {
   extern HSZ   DDE_System_StrHdl   ;
   extern HSZ   DDE_Topic_StrHdl    ;
   extern HSZ   DDE_Item_StrHdl[ Number_Of_String_Handles ];

   void    DDE_Initialize_Item_Strings
                  ( DWORD   DDEIdentifier ); 

   void    DDE_Free_Item_Strings
                  ( DWORD   DDEIdentifier ); 
   
   void    DDE_Data_Handle_To_String
                  ( 
                  DWORD      DDEIdentifier ,
                  HDDEDATA   DataHdl ,
                  String&    TargetString 
                  );

   BOOL    DDE_Error
                  ( 
                  DWORD           DDEIdentifier ,
                  const String&   FunctionNameRef ,
                  const String&   MessageRef
                  );
   }
   
#endif
