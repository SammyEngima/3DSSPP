#include "stdafx.h"
#include <assert.h>
#include <math.h>
#include <string>

#include "difcount.hpp"
#include "units.hpp"
#include "intrinsc.hpp"
#include "dimension.hpp" //for Near global function.

using std::string;

//--------------------------------/ default constructor /---------------------
Difference_Count ::                                           Difference_Count
( void )
   : 
   Count( 0 ) ,
   System_To_Dialog_Conversion( 0.0F ) ,
   Dialog_To_System_Conversion( 0.0F )
   {};

//---------------------------------------------------------------------------
bool Difference_Count::Different(float aSource, float aTarget)
{
//   if( ! Near( aTarget, aSource, 0.001F )  )
   if(aTarget != aSource)
   {
      this->Count ++;
      return   true;
   }
   else
   {
      return  false;
   }
} 

//---------------------------------------------------------------------------
bool Difference_Count::Different(double aSource, double aTarget)
{
//   if( ! Near( aTarget, aSource, 0.001F )  )
   if(aTarget != aSource)
   {
      this->Count ++;
      return   true;
   }
   else
   {
      return  false;
   }
} 

//---------------------------------------------------------------------------
   bool
   Difference_Count::
Different
( 
   int   aSource ,
   int   aTarget
)
{
   if( aSource != aTarget  )
   {
      this->Count ++;
      return   true;
   }
   else
   {
      return  false;
   }
} 

//---------------------------------------------------------------------------
   bool
   Difference_Count::
Different
( 
 const std::string &   aSource ,
 const std::string &   aTarget
)
{
   if( aSource != aTarget  )
   {
      this->Count ++;
      return   true;
   }
   else
   {
      return  false;
   }
} 
//---------------------------------------------------------------------------
   bool
   Difference_Count::
Different
( 
 JointAngles &   aSource ,
 JointAngles &   aTarget
)
{
   if( aSource != aTarget  )
   {
      this->Count ++;
      return   true;
   }
   else
   {
      return  false;
   }
} 
/*-------------------------------*/ int    //---------------------------------
Difference_Count ::                                                     Update
( 
int&   SourceRef ,
int&   TargetRef
)
  {
  if( TargetRef != SourceRef )
     {
     TargetRef = SourceRef;
     this->Count ++;
     };
  return  this->Count;
  }
 /*-------------------------------*/ int    //---------------------------------
Difference_Count ::Update
( 
JointAngles&   SourceRef ,
JointAngles&   TargetRef
)
  {
  if( TargetRef != SourceRef )
     {
     TargetRef = SourceRef;
     this->Count ++;
     };
  return  this->Count;
  } 
/*-------------------------------*/ int    //---------------------------------
Difference_Count ::                                                     Update
( 
BYTE&   SourceRef ,
BYTE&   TargetRef
)
  {
  if( TargetRef != SourceRef )
     {
     TargetRef = SourceRef;
     this->Count ++;
     };
  return  this->Count;
  } 
/*-------------------------------*/ int    //---------------------------------
Difference_Count ::                                                     Update
( 
float&   SourceRef ,
float&   TargetRef
)
  {
  if( TargetRef != SourceRef )
     {
     TargetRef = SourceRef;
     this->Count ++;
     };
  return  this->Count;
  } 

/*-------------------------------*/ int    //---------------------------------
Difference_Count ::                                                     Update
( 
const CString&   SourceRef ,
      CString&   TargetRef
)
  {
  if( TargetRef != SourceRef )
     {
     TargetRef = SourceRef;
     this->Count ++;
     };
  return  this->Count;
  } 

//---------------------------------------------------------------------------
   int
   Difference_Count::
Update
( 
const float &    SourceRef ,
      Dimension &   TargetRef
)
{
   if( TargetRef != SourceRef )
   {
      TargetRef = SourceRef;
      this->Count ++;
   };

   return  this->Count;
} 

/*-------------------------------*/ void   //---------------------------------
Difference_Count ::                            Set_System_To_Dialog_Conversion
(
float   SetConversion
)
   {
   assert( SetConversion != 0 );
   System_To_Dialog_Conversion = SetConversion;
   Dialog_To_System_Conversion = 1.0F / System_To_Dialog_Conversion;   
   return;
   }
      
/*-------------------------------*/ void   //---------------------------------
Difference_Count ::                                                Load_Dialog
( 
      float&	DialogTargetRef, 
		float		SystemSourceRef, 
      float&	TemporaryRef,
      int		Decimals 
)
{
   assert( System_To_Dialog_Conversion != 0 );
   float   To_Convert = System_To_Dialog_Conversion * SystemSourceRef;
   TemporaryRef    = Round_Float( To_Convert, Decimals );
   DialogTargetRef = TemporaryRef;

	return;
}

/*-------------------------------*/ void   //---------------------------------
Difference_Count ::                                                Load_Dialog
( 
      double&	DialogTargetRef, 
		double		SystemSourceRef, 
      double&	TemporaryRef,
      int		Decimals 
)
{
   assert( System_To_Dialog_Conversion != 0 );
   double   To_Convert = System_To_Dialog_Conversion * SystemSourceRef;
   TemporaryRef    = Round_Double( To_Convert, Decimals );
   DialogTargetRef = TemporaryRef;

	return;
}

/*-------------------------------*/ void   //---------------------------------
Difference_Count ::                                                Load_Dialog
( 
      int&	DialogTargetRef, 
		float	SystemSourceRef, 
      int&	TemporaryRef 
)
//For rounding system data to the nearest integer for dialog display.
{
   assert( System_To_Dialog_Conversion != 0 );
   float   To_Convert = System_To_Dialog_Conversion * SystemSourceRef;
   TemporaryRef    = Round_Int( To_Convert );
   DialogTargetRef = TemporaryRef;
   return;
}

/*-------------------------------*/ int    //---------------------------------
Difference_Count ::                                              Update_System
( 
const float&    DialogSourceRef, 
      float&    SystemTargetRef, 
const float&    TemporaryRef,
      int       Decimals 
)
//If value has changed in the dialog, round to the indicated decimals and then
//convert to internal system units.
{
   if( DialogSourceRef != TemporaryRef )
	{
      double   Delta = fabs( DialogSourceRef - TemporaryRef );
      double   Ratio;
      if( TemporaryRef != 0 )
      {
         Ratio = fabs( Delta / TemporaryRef );
      }
      else
      { 
         if( DialogSourceRef != 0 )
            {
            Ratio = fabs( Delta / DialogSourceRef );
         }
         else   
            Ratio = 0.0F;
      }

      double   Epsilon = 0.000001;

      if( Ratio > Epsilon )
      {     
         assert( Dialog_To_System_Conversion != 0 );
         SystemTargetRef = Dialog_To_System_Conversion * Round_Float( DialogSourceRef, Decimals );
         this->Count ++;
      }
   }

   return this->Count;
}

/*-------------------------------*/ int    //---------------------------------
Difference_Count ::                                              Update_System
( 
const double&    DialogSourceRef, 
      double&    SystemTargetRef, 
const double&    TemporaryRef,
      int       Decimals 
)
//If value has changed in the dialog, round to the indicated decimals and then
//convert to internal system units.
{
   if( DialogSourceRef != TemporaryRef )
	{
      double   Delta = fabs( DialogSourceRef - TemporaryRef );
      double   Ratio;
      if( TemporaryRef != 0 )
      {
         Ratio = fabs( Delta / TemporaryRef );
      }
      else
      { 
         if( DialogSourceRef != 0 )
            {
            Ratio = fabs( Delta / DialogSourceRef );
         }
         else   
            Ratio = 0.0F;
      }

      double   Epsilon = 0.000001;

      if( Ratio > Epsilon )
      {     
         assert( Dialog_To_System_Conversion != 0 );
         SystemTargetRef = Dialog_To_System_Conversion * Round_Double( DialogSourceRef, Decimals );
         this->Count ++;
      }
   }

   return this->Count;
}

/*-------------------------------*/ int    //---------------------------------
Difference_Count ::                                              Update_System
( 
const int&      DialogSourceRef, 
      float&    SystemTargetRef, 
const int&      TemporaryRef 
)
   {
   if( DialogSourceRef != TemporaryRef )
      {
      assert( Dialog_To_System_Conversion != 0 );
      SystemTargetRef = Dialog_To_System_Conversion * DialogSourceRef;
      this->Count ++;
      };
   return  this->Count;
   }

/*-------------------------------*/ int    //---------------------------------
Difference_Count ::                                              Update_System
( 
const int&      DialogSourceRef, 
      int&      SystemTargetRef, 
const int&      TemporaryRef 
)
   {
   if( DialogSourceRef != TemporaryRef )
      {
      assert( Dialog_To_System_Conversion != 0 );
      SystemTargetRef = Round_Int(  Dialog_To_System_Conversion * DialogSourceRef  );
      this->Count ++;
      };
   return  this->Count;
   }
   