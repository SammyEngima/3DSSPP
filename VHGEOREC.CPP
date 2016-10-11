//============================ MEMBER FUNCTIONS: ===== Vehicle_Geometry_Record
#include "stdafx.h"
#include "vhgeorec.hpp"
#include "string.hpp"
//#include "message.hpp"
//#include "flt3dpnt.hpp"
//#include "positrec.hpp"
//#include "scalerec.hpp"
#include "graph_db.hpp"

//------------------------------------- default constructor ------------------
Vehicle_Geometry_Record ::			       Vehicle_Geometry_Record
( void )
   :   Record( "Default" ) ,
   GDB( *( new Graphic_Database )  )
   {
   Object::No_Default_Constructor( "Vehicle_Geometry_Record" );
   return;
   }

//------------------------------------- copy constructor ---------------------
Vehicle_Geometry_Record ::			       Vehicle_Geometry_Record
(
const Vehicle_Geometry_Record&   CopyRef
)
   :   Record( CopyRef ) ,
   GDB( *( new Graphic_Database )  ) 
   {
   Object::No_Copy_Constructor( "Vehicle_Geometry_Record" );
   return;
   }

//------------------------------------- destructor ---------------------------
Vehicle_Geometry_Record ::		              ~Vehicle_Geometry_Record
( void )
   {
   assert(  Is_Object_Ref( this->GDB )  );

                                  //Remove scales   
   GDB.Scale_Remove( "Unit Scale" );
   GDB.Scale_Remove( "Steering" );
   GDB.Scale_Remove( "Vehicle Floor" );
   
                                  //Remove positions (origins)
   GDB.Position_Remove( "VehicleOrigin" );                                     
   GDB.Position_Remove( "Dashboard" );                                     
   GDB.Position_Remove( "Steering" );                                     
   GDB.Position_Remove( "Vehicle Seat" );                                     
   GDB.Position_Remove( "Vehicle Floor" );                                     

   return;
   }

//------------------------------------- constructor --------------------------
Vehicle_Geometry_Record ::			       Vehicle_Geometry_Record
(
      Graphic_Database&   GDBRef ,
const String&             NameRef ,
      int                 IniPercentSeatForward
)
   :   Record( NameRef, SPARE_DEPENDENTS ) ,
   GDB( GDBRef ) ,
   Percent_Seat_Forward( IniPercentSeatForward ) ,
   Seat_Rail_Origin() 

//For the vehicle interior geometry, only the percent seat forward value is
//adjustable.

   {
   assert(  Is_Object_Ref( NameRef  )  );
   assert(  Is_Object_Ref( GDBRef )  );
   assert(  Percent_Seat_Forward >=   0 );
   assert(  Percent_Seat_Forward <= 100 );

// NOTE: Vehicle_Geometry_Records are at the bottom of the dependency hierarchy and
// have no dependencies (Attach) on other record objects.   
   
   this->Initialize();
   
   this->Update();  
   
   return;                                                                  
   }

/*-----------------------------------*/ void                   //-------------
Vehicle_Geometry_Record ::                                          Initialize
( void )
   {
                                  //--- Initialize Data ----------------------
                                  //From data supplied in VH_DATA.TXT
   int   Dashboard_X =           325;  //Point B
   int   Dashboard_Y_Left =     -300;  //"      
   int   Dashboard_Y_Right =    1060;  //"
   int   Dashboard_Z =           650;  //"
   
   int   Steering_X =            490;  //Point E
   int   Steering_Y =              0;  //"
   int   Steering_Z =            630;  //"
   int   Steering_Tilt_Angle =    23;  //Angle F
   
   int   Steering_Dia =          390;  //Diameter G
   int   Steering_Thk =           38;  //Thickness H 
   
   int   Seat_Rail_X =           985;  //Point J
   int   Seat_Rail_Y =             0;  //"
   int   Seat_Rail_Z =           120;  //"
   int   Seat_Rail_Angle =        10;  //Angle N 
   
   int   Floor_X_Max =          1000;  //Point R
   int   Floor_X_Min =             0;  //Point Q2
   int   Floor_Y_Max =          1160;  //Point Q2
   int   Floor_Y_Min =          -400;  //Point Q1
   int   Floor_Z     =             0;  //Point Q1
   float Floor_Thk   =            50;  //Set here
   
                                  //--- Create Origins -----------------------
                                  
                                  //--- Vehicle Graphics
                                  //NOTE: This is the base position for all vehicle
                                  //graphics. The given values represent moving
                                  //the vehicle graphics into the animation study
                                  //coordinate system. It is presumed that the
                                  //vehicle origin and the subject's heel point 
                                  //are superimposed. The vehicle graphic data
                                  //must also be rotated -90 degrees with respect
                                  //to the study coordinate system.
   Position   Vehicle_Origin
                 ( -200.0F, 600.0F, -550.0F, 0.0F, 0.0F, -90.0F );

                                  //--- Dashboard
   Position   Dashboard_Position
                 ( 
                 Dashboard_X ,
                 0.5F * ( Dashboard_Y_Right + Dashboard_Y_Left ) ,
                 Dashboard_Z ,
                 0, 0, 0
                 );
                                  //--- Steering
   Position   Steering_Position
                 ( 
                 Steering_X , Steering_Y , Steering_Z ,
                 0, 90 - Steering_Tilt_Angle, 0
                 );
                                  //--- Seat Rail
   Position   Temp_Seat_Rail_Position
                 ( 
                 Seat_Rail_X , Seat_Rail_Y , Seat_Rail_Z ,
                 0, Seat_Rail_Angle, 0
                 );
   this->Seat_Rail_Origin = Temp_Seat_Rail_Position;
   //Note: Not added to database. Used later in Update to compute 
   //      seat origin and add to database.                     
   
                                  //--- Floor
   float   Floor_X      = 0.5F * ( Floor_X_Max + Floor_X_Min );
   float   Floor_Y      = 0.5F * ( Floor_Y_Max + Floor_Y_Min );
   
   Position   Vehicle_Floor_Position
                 ( 
                 Floor_X , Floor_Y , Floor_Z ,
                 0, 0, 0
                 );
                                  
                                  //--- Add origins to database --------------
                                  //Adjust for display origin
   
   this->GDB.Position_Add( "Vehicle_Origin", Vehicle_Origin );
   this->GDB.Dependent_Position_Add( "Dashboard", Dashboard_Position, "Vehicle_Origin" );
   this->GDB.Dependent_Position_Add( "Steering", Steering_Position, "Vehicle_Origin" );
 //Seat position added to database later in update function  
   this->GDB.Dependent_Position_Add( "Vehicle Floor", Vehicle_Floor_Position, "Vehicle_Origin" );
                                  
                                  //--- Create Scales ------------------------
                                  //--- Dashboard
   this->GDB.Scale_Add( "Unit Scale", 1.0F, 1.0F, 1.0F );
   
                                  //--- Steering
   this->GDB.Scale_Add( "Steering", Steering_Dia, Steering_Dia, Steering_Thk );

                                  //--- Seat
   //Seat uses unit scale above

                                  //--- Floor
   double Floor_Lth  =  Floor_X_Max - Floor_X_Min;
   double Floor_Wth  =  Floor_Y_Max - Floor_Y_Min;
   this->GDB.Scale_Add( "Vehicle Floor", Floor_Lth, Floor_Wth, Floor_Thk );
   
   return;
   }

/*-----------------------------------*/ void       //-------------------------
Vehicle_Geometry_Record ::					        Update
( void )
//Currently just computes the location of the seat origin along the seat rail
//and adds (or overwrites) it to the graphic database.
   {
   float   Seat_Rear_Stop  =   30.0F;
   float   Seat_Fore_Stop  = -170.0F;
   
   float   Seat_X = this->Percent_Seat_Forward * 0.01F * 
                       ( Seat_Fore_Stop - Seat_Rear_Stop ) 
                  + Seat_Rear_Stop;
   
   Position   Local_Displacement( Seat_X, 0, 0,  0, 0, 0 );
   
   Position   Global_Position;
   Global_Position = this->Seat_Rail_Origin + Local_Displacement;
   
   this->GDB.Dependent_Position_Add( "Vehicle Seat", Global_Position, "Vehicle_Origin" );
   
   this->Record::Update(); //To notify dependent records

   return;
   }

/*-----------------------------------*/ void                 //---------------
Vehicle_Geometry_Record ::		              Put_Percent_Seat_Forward
(
int   PutPercentSeatForward
)
   {   
   assert(  PutPercentSeatForward >=   0 );
   assert(  PutPercentSeatForward <= 100 );

   this->Percent_Seat_Forward = PutPercentSeatForward; 
   
   this->Update(); 
   
   return;
   }

/*-----------------------------------*/ int                  //---------------
Vehicle_Geometry_Record ::		              Get_Percent_Seat_Forward
( void )
   const 
   {   
   return    this->Percent_Seat_Forward;
   }
