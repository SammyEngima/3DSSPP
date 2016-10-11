//============================ MEMBER FUNCTIONS: ======================= Palette
#include "stdafx.h"

#include "Palette.hpp"
#include "shaderec.hpp"
#include "string.hpp"


//------------------------------------- default constructor ------------------
Palette ::					                       Palette
( void )
   :   Object()
   {
   this->Object::No_Default_Constructor( "Palette" );
   return;
   }

//------------------------------------- copy constructor ---------------------
Palette ::					                       Palette
(
const Palette&   CopyRef
)
   :   Object( CopyRef ) 
   { 
   this->Object::No_Copy_Constructor( "Palette" );
   return;
   }

//------------------------------------- destructor ---------------------------
Palette ::					                      ~Palette
( void )
   { }

//---------------------------------- additional constructor ------------------
Palette ::					                       Palette
(
int          BaseIdx ,
const CDC&   DeviceContextRef
)
   :   Object() ,
   LP() ,
   Base_Index( BaseIdx ) ,
   Current_Index( BaseIdx - 1 )
   {   
   assert(  BaseIdx >= Palette::Min_Index  );
   assert(  BaseIdx <= Palette::Max_Index  );
   //!!!Check validity of device context object?

   this->LP.EC_Type.Palette_Version = 0x300;
   this->LP.EC_Type.Palette_Number_Of_Entries = 256;

   GetSystemPaletteEntries
      ( DeviceContextRef.GetSafeHdc(), 0, 256, this->LP.Win_Type.palPalEntry );

   return;
   }

/*-----------------------------------*/ LOGPALETTE&   //------------------------
Palette ::					         Get_Logical_Palette_Ref
( void )
   {
   return   this->LP.Win_Type;
   }

/*-----------------------------------*/ LOGPALETTE*   //------------------------
Palette ::					         Get_Logical_Palette_Ptr
( void )
   {
   return   &( this->LP.Win_Type );
   }