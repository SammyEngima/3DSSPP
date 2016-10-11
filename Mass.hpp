#if !defined( MassHppMcr )
#define   MassHppMcr

//Target saved units are mm. Setting the system of units changes the conversions for
//mutation and access, NOT the internal value.

#include "Dimension.hpp"

class   Mass
: public Dimension
{
//--- Interface ---
public:

Mass
( void );

Mass
( 
   double   aValue ,
   int     aDecimals
);

Mass
( const Mass &   aCopyRef );

~Mass
( void );

   Mass &
operator =
( const Mass &   aAssignRef );

   Mass &
operator =
( double   aFloatRef );

//--- Overrides and Particular --- 
static void GravitySet( double gravity );


   static
   void
SystemOfUnits
( SystemOfUnitsEnm   aSetUnits );

   virtual
   double
GetConversion
( void )
const;

   virtual
   double
GetConversion
( SystemOfUnitsEnm   aSystemOfUnits )
const;

   virtual
   double
PutConversion
( void )
const;

   virtual
   double
PutConversion
( SystemOfUnitsEnm   aSystemOfUnits )
const;

   String
UnitsText
( void )
const;

//--- Class Attributes ---
private:

static  SystemOfUnitsEnm  cSystemOfUnits;
static  double             cPutConversion;
static  double             cGetConversion;
static  String            cUnitsText;
static  double			  Dg_GravityVal;	// ACT - Gravity setting
//--- Object Attributes ---
};

#endif