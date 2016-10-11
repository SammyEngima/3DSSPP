#include "stdafx.h"

#include "Mass.hpp"
#include "hom.h"
#include "../c4eLibNew/units.hpp"
//#include "hom_doc.h"	// Unnecessary!
//--- Class Attributes ---

SystemOfUnitsEnm  Mass::cSystemOfUnits( MetricCns );
double             Mass::cPutConversion( 0.0F );
double             Mass::cGetConversion( 0.0F );
String            Mass::cUnitsText( "" );
double			  Mass::Dg_GravityVal( 9.81F );
//--- Constants ---

const double   fKgToKgCns = 1.0F;

const double   fLbToKgCns = U_LBM_KG; 
const double   fKgToLbCns = U_KG_LBM;  

const double   fSlugToKgCns = U_SLUG_KG;
const double   fKgToSlugCns = U_KG_SLUG;


//--- Interface ---

//---------------------------------------------------------------------------
Mass::
Mass
( void )
:  Dimension() 
{   return; }

//---------------------------------------------------------------------------
Mass::
Mass
( 
 double   aValue ,
 int     aDecimals 
 )
 :  Dimension( aValue, aDecimals ) 
{
	return; 
}

//---------------------------------------------------------------------------
Mass::
Mass
( const Mass &   aCopyRef )
:  Dimension( aCopyRef ) 
{   return; }

//---------------------------------------------------------------------------
Mass::
~Mass
( void )
{ return; }

//---------------------------------------------------------------------------
Mass &
Mass::
operator =
( const Mass &   aAssignRef )
{
	this->Dimension::operator=( aAssignRef );
	return   * this;
}

//---------------------------------------------------------------------------
Mass &
Mass::
operator =
( double   aFloat )
{
	this->Dimension::operator=( aFloat );
	return   * this;
}

//---------------------------------------------------------------------------
void Mass::GravitySet( double gravity )
{
	Dg_GravityVal = gravity;
}
void
Mass::
SystemOfUnits
( SystemOfUnitsEnm   aSetUnits )
{ 
	cSystemOfUnits = aSetUnits;
	
	if( cSystemOfUnits == EnglishCns ) 
	{
		if (Dg_GravityVal >= 9.80 && Dg_GravityVal < 9.82) {
			cPutConversion = fLbToKgCns;
			cGetConversion = fKgToLbCns;
			cUnitsText     = "lb";
		}
		else {
			cPutConversion = fSlugToKgCns;
			cGetConversion = fKgToSlugCns;
			cUnitsText     = "slug";
		}
	}
	
	else
	{
		cPutConversion = fKgToKgCns;
		cGetConversion = fKgToKgCns;
		cUnitsText     = "Kg";
	}
	
	return; 
}

//---------------------------------------------------------------------------
double
Mass::
GetConversion
( void )
const
{ 
	return   cGetConversion;
}

//---------------------------------------------------------------------------
double
Mass::
GetConversion
( 
 SystemOfUnitsEnm   aSystemOfUnits
 )
 const
{ 
	double   lGetConversion;
	if( aSystemOfUnits == EnglishCns ) 
	{
		lGetConversion = fKgToLbCns;
	}
	
	else
	{
		lGetConversion = fKgToKgCns;
	}
	
	return   lGetConversion;
}

//---------------------------------------------------------------------------
double
Mass::
PutConversion
( void )
const
{ 
	return   cPutConversion;
}

//---------------------------------------------------------------------------
double
Mass::
PutConversion
( 
 SystemOfUnitsEnm   aSystemOfUnits 
 )
 const
{ 

	double  lPutConversion;

	if( aSystemOfUnits == EnglishCns ) 
	{
		lPutConversion = fLbToKgCns;
	}
	
	else
	{
		lPutConversion = fKgToKgCns;
	}
	
	return   lPutConversion;
}

//---------------------------------------------------------------------------
String
Mass::
UnitsText
( void )
const
{
	return   this->cUnitsText;
}
