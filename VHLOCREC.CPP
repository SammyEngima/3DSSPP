// VHLOCREC.CPP

//============================ MEMBER FUNCTIONS: ===== Vehicle_Locals_Record
#include "stdafx.h"
#include "vhlocrec.hpp"
#include "vhgeorec.hpp"

#include "units.hpp"
#include "string.hpp"
#include "graph_db.hpp"
#include "comp_rec.hpp"


//------------------------------------- default constructor ------------------
Vehicle_Locals_Record ::                             Vehicle_Locals_Record
( void )
   :   Local_Group_Record( "Default", *(new Graphic_Database) )
   {
   Object::No_Default_Constructor( "Vehicle_Locals_Record" );
   return;
   }

//------------------------------------- copy constructor ---------------------
Vehicle_Locals_Record ::			       Vehicle_Locals_Record
(
const Vehicle_Locals_Record&   CopyRef
)
   :   Local_Group_Record( "Copy", *(new Graphic_Database) ) 
   {
   Object::No_Copy_Constructor( "Vehicle_Locals_Record" );
   return;
   }
//------------------------------------- destructor ---------------------------
Vehicle_Locals_Record ::			      ~Vehicle_Locals_Record
( void )    
   //NOTE: Any World_Group records (in the Graphics_Database object) that 
   //reference this macro record should be removed first so that they will
   //call the Remove_Graphics (from the Graphics_Database object) function 
   //for this macro object. It is important that this be done before this 
   //destructor is called since the mentioned graphics records reference the 
   //Local and Shape records which are about to be deleted. Assertion failures
   //will occur if there are any remaining graphics records 'attached' 
   //(referencing) the records deleted here-in.
   {                      
   String&   Name = this->Record::Key; //This group name

                                  //Remove locals (must be removed before
                                  //shapes)
   GDB.Local_Remove( Name + " Dashboard" );                                  
   GDB.Local_Remove( Name + " Steering" );                                  
   GDB.Local_Remove( Name + " Vehicle Seat" );
   GDB.Local_Remove( Name + " Vehicle Floor" );                                 

                                  //Remove shapes (must be removed after any
                                  //graphics that reference this record as
                                  //noted above)
   GDB.Shape_Remove( Name + " Dashboard" );                                  
   GDB.Shape_Remove( Name + " Vehicle Seat" );                                  
   
// this->Vehicle_Geometry_Ptr->Release( this ); //Disabled. See constructor note.

   return;
   }

//------------------------------------- constructor --------------------------
Vehicle_Locals_Record ::			       Vehicle_Locals_Record
(
      Graphic_Database&           GDBRef ,
const String&                     NameRef ,
      Vehicle_Geometry_Record&    VehicleGeometryRecordRef ,
const String&                     SurfaceNameRef 
)
   :   Local_Group_Record( NameRef, GDBRef ) ,
   Vehicle_Geometry_Ptr( &VehicleGeometryRecordRef ) ,
   Dashboard_Offset( 0.0F ) ,
   Steering_Offset( 0.0F ) ,
   Seat_Offset( 0.0F ) ,
   Floor_Offset( 0.0F )
   {
   assert(  Is_Object_Ref( NameRef  )  );
   assert(  Is_Object_Ref( VehicleGeometryRecordRef  )  );
   assert(  Is_Object_Ref( SurfaceNameRef  )  );
   
                                  //NOTE: Typically, objects derived from
                                  //Local_Group_Record attach themselves to
                                  //their corresponding geometry object in the
                                  //event a change in the geometry object requires
                                  //corresponding changes in the local group
                                  //object. This is not the case for this 
                                  //local group object and geometry object pair.                                  
// VehicleGeometryRecordRef.Attach( this );

   this->Create_Shapes();   
   this->Create_Locals( SurfaceNameRef );

   this->Update();

   return;
   }

#include "sgmntrec.hpp" //For Segment_Source structure
/*-----------------------------------*/ void       //-------------------------
Vehicle_Locals_Record ::				         Create_Shapes
( void )
//Create the group of shapes to be used for representing the vehicle interior.
//These shapes are just shape, having no surface properties, scaling, or 
//placement in 3D space. These attributes are defined later when the shape is
//used in combination with other objects (e.g., surface, scale, and position)
//to create higher level objects (e.g., local graphic and world graphic)
   {
   String&   Name = this->Record::Key; //This group name
                           
                           //--- Data points and dimensions from drawing ---
   Flt_3D_Point   Pnt_A1 (    25,  -300,   650 );                                                      
   Flt_3D_Point   Pnt_A2 (    25,  1060,   650 );                                                      
   Flt_3D_Point   Pnt_B  (   325,  -300,   650 );                                                      
   Flt_3D_Point   Pnt_C  (   325,  -300,   420 );                                                      
   Flt_3D_Point   Pnt_D  (    85,  -300,   270 );                                                      
   Flt_3D_Point   Pnt_E  (   490,     0,   630 );                                                      
   double          Ang_F  (    23               );                                                      
   double          Dia_G  (   390               );                                                      
   double          Thk_H  (    30               );                                                      
// Flt_3D_Point   Pnt_I  (                     ); //Unused point in drawing
   Flt_3D_Point   Pnt_J  (   985,     0,   120 );                                                      
   double          Lth_K  (   480               );                                                      
   double          Lth_L  (   550               );                                                      
   double          Ang_M  (    74               );                                                      
   double          Ang_N  (    10               );                                                      
   double          Lth_O  (   200               );                                                      
   double          Wth_P  (   540               );                                                      
   Flt_3D_Point   Pnt_Q1 (     0,  -400,     0 );                                                      
   Flt_3D_Point   Pnt_Q2 (     0,  1160,     0 );                                                      
   Flt_3D_Point   Pnt_R  (  1000,  -400,     0 );                                                      

                                  //--- Dashboard ----------------------------
   int Dashboard_Rows = 2;
   int Dashboard_Columns = 7;
   Flt_3D_Mesh   Dashboard_Mesh( Dashboard_Rows, Dashboard_Columns, NO_CAPS );
   Flt_3D_Mesh&  DMRef = Dashboard_Mesh;
   
   DMRef.Reset_Index();
   
   (DMRef++).Put(   Pnt_A1( C_X ), Pnt_A1( C_Y ), Pnt_A1( C_Z )  ); //Row 0
   (DMRef++).Put(   Pnt_B ( C_X ), Pnt_A1( C_Y ), Pnt_B ( C_Z )  );
   (DMRef++).Put(   Pnt_C ( C_X ), Pnt_A1( C_Y ), Pnt_C ( C_Z )  );
   (DMRef++).Put(   Pnt_D ( C_X ), Pnt_A1( C_Y ), Pnt_D ( C_Z )  );
   (DMRef++).Put(   Pnt_C ( C_X ), Pnt_A1( C_Y ), Pnt_C ( C_Z )  );
   (DMRef++).Put(   Pnt_B ( C_X ), Pnt_A1( C_Y ), Pnt_B ( C_Z )  );
   (DMRef++).Put(   Pnt_A1( C_X ), Pnt_A1( C_Y ), Pnt_A1( C_Z )  );
       
   (DMRef++).Put(   Pnt_A2( C_X ), Pnt_A2( C_Y ), Pnt_A2( C_Z )  ); //Row 1
   (DMRef++).Put(   Pnt_B ( C_X ), Pnt_A2( C_Y ), Pnt_B ( C_Z )  );
   (DMRef++).Put(   Pnt_C ( C_X ), Pnt_A2( C_Y ), Pnt_C ( C_Z )  );
   (DMRef++).Put(   Pnt_D ( C_X ), Pnt_A2( C_Y ), Pnt_D ( C_Z )  );
   (DMRef++).Put(   Pnt_C ( C_X ), Pnt_A2( C_Y ), Pnt_C ( C_Z )  );
   (DMRef++).Put(   Pnt_B ( C_X ), Pnt_A2( C_Y ), Pnt_B ( C_Z )  );
   (DMRef++).Put(   Pnt_A2( C_X ), Pnt_A2( C_Y ), Pnt_A2( C_Z )  );
   
   Position   Dashboard_Origin
                 ( 
                 Pnt_B( C_X ) ,
                 0.5F * ( Pnt_A1( C_Y ) + Pnt_A2( C_Y ) ) ,
                 Pnt_B( C_Z ) ,
                 0, 0, 0
                 );
   
   Dashboard_Mesh.Flt_3D_Polyline::Transform( - Dashboard_Origin );

   GDB.Segment_Shape_Add( Name + " Dashboard", Dashboard_Mesh, OPEN );

                                  //--- Vehicle Seat -------------------------
   int Seat_Rows    = 2;
   int Seat_Columns = 5;
   Flt_3D_Mesh   Seat_Mesh( Seat_Rows, Seat_Columns, NO_CAPS );
   Flt_3D_Mesh&  SMRef = Seat_Mesh;
   
   double   Seat_Back_Z = (  Lth_L * sin( U_DEGREES_RADIANS * Ang_M )  );
   double   Seat_Back_X = (  Lth_L * cos( U_DEGREES_RADIANS * Ang_M )  );
   double   Half_Width  = 0.5F * Wth_P;
   
   SMRef.Reset_Index();
   
   (SMRef++).Put(        - Lth_K,   - Half_Width,             0 );
   (SMRef++).Put(              0,   - Half_Width,             0 );
   (SMRef++).Put(    Seat_Back_X,   - Half_Width,   Seat_Back_Z );
   (SMRef++).Put(              0,   - Half_Width,             0 );
   (SMRef++).Put(        - Lth_K,   - Half_Width,             0 );
   
   (SMRef++).Put(        - Lth_K,     Half_Width,             0 );
   (SMRef++).Put(              0,     Half_Width,             0 );
   (SMRef++).Put(    Seat_Back_X,     Half_Width,   Seat_Back_Z );
   (SMRef++).Put(              0,     Half_Width,             0 );
   (SMRef++).Put(        - Lth_K,     Half_Width,             0 );
   
   GDB.Segment_Shape_Add( Name + " Vehicle Seat", Seat_Mesh, OPEN );

                                  //--- Steering Wheel -----------------------
   //A primitive cylinder shape is already provided in the database. A scale
   //is provided by the corresponding geometry object (i.e., Vehicle_Geometry_Record)
   //for scaling the cylinder primitive to the proper dimensions for representing 
   //a steering wheel.

                                  //--- Floor Wheel --------------------------
   //A primitive box shape is already provided in the database. A scale
   //is provided by the corresponding geometry object (i.e., Vehicle_Geometry_Record)
   //for scaling the box primitive to the proper dimensions for representing 
   //a vehicle wheel.
   
   return;
   }

/*-----------------------------------*/ void       //-------------------------
Vehicle_Locals_Record ::					 Create_Locals
( const String&   SurfaceNameRef )
//Create local graphic objects having scale and surface attributes. These 
//objects are local in that they have not been placed at a particular position
//in a particular composition's global space. A given local object may be
//placed at several different locations in the same or different compositions.
   {
   assert(  Is_Object_Ref( this->GDB )  );

   String&   Name = this->Record::Key; //This group name

                                  //--- Dashboard ---
   GDB.Local_Add
      (
         Name + " Dashboard",  //Name of local
         Name + " Dashboard",  //Shape
         "Unit Scale",         //Scale 
         SurfaceNameRef        //Surface
      );

                                  //--- Steering ---
   GDB.Local_Add
      (
         Name + " Steering", //Name of local
         "Unit Cylinder Shape",    //Shape (from application)
         "Steering",               //Scale
         SurfaceNameRef            //Surface
      );

                                  //--- Vehicle Seat ---
   GDB.Local_Add
      (
         Name + " Vehicle Seat",  //Name of local
         Name + " Vehicle Seat",  //Shape
         "Unit Scale",            //Scale
         SurfaceNameRef           //Surface
      );

                                  //--- Vehicle Floor ---
   GDB.Local_Add
      (
         Name + " Vehicle Floor",  //Name of local
         "Unit Box Shape",         //Shape (from application)
         "Vehicle Floor",          //Scale
         SurfaceNameRef            //Surface
      );

   return;
   }
   
/*-----------------------------------*/ void       //-------------------------
Vehicle_Locals_Record ::				       Create_Graphics
( Composition_Record&   CompRef )
//Place the local objects into the provided composition space at locations
//created and added to the database by the corresponding geometry object
//Vehicle_Geometry_Record.
   {
   assert(  Is_Object_Ref( CompRef )  );

   String   Comp_Name;
   Comp_Name = CompRef.Get_Key(); //The provided composition's name

   String&   Name = this->Record::Key; //This group name

   GDB.Graphic_Add(  Name + " Dashboard",      //Graphic Name
                     Name + " Dashboard",      //Local
                    "Dashboard",               //Position 
                     Comp_Name,                //Composition
                     this->Dashboard_Offset
                     );
                     
   GDB.Graphic_Add(  Name + " Steering",       //Graphic Name
                     Name + " Steering",       //Local
                    "Steering",                //Position 
                     Comp_Name,                //Composition
                     this->Steering_Offset
                     );
                     
   GDB.Graphic_Add(  Name + " Vehicle Seat",   //Graphic Name
                     Name + " Vehicle Seat",   //Local
                    "Vehicle Seat",            //Position 
                     Comp_Name,                //Composition
                     this->Seat_Offset
                     );              

   GDB.Graphic_Add(  Name + " Vehicle Floor",  //Graphic Name
                     Name + " Vehicle Floor",  //Local
                    "Vehicle Floor",           //Position 
                     Comp_Name,                //Composition
                     this->Floor_Offset        
                     );                   
   return;
   }

   
/*-----------------------------------*/ void       //-------------------------
Vehicle_Locals_Record ::					        Update
( void )
//NOTE: Updating of graphics managed by this Local_Record is deferred to the
//associated World_Group_Record objects which call back to this object's 
//Update_Graphics member function with their particular Composition_Record
//reference. These World_Group_Records are activated by their dependency on
//this object and the following call to Local_Record::Update which eventually
//updates all related objects.
   { 
   this->Local_Group_Record::Update();
   return;
   }

/*-----------------------------------*/ void       //-------------------------
Vehicle_Locals_Record ::				       Update_Graphics
( 
Composition_Record&   CompRef
)
//The Z Depth Offsets for the various vehicle graphics may have changed. NOTE:
//There should be no need to check to see if the graphic records are still in 
//the database since this function is called only by the corresponding 
//World_Group record. If it is not present in the database, niether is the
//group of graphics modified below.
   {
   assert(  Is_Object_Ref( CompRef )  );

   String&   Name = this->Record::Key; //This group name

   CompRef.Get_World_Ref( Name + " Dashboard" ).Put_Z_Depth_Offset( this->Dashboard_Offset );
   CompRef.Get_World_Ref( Name + " Steering" ).Put_Z_Depth_Offset( this->Steering_Offset );
   CompRef.Get_World_Ref( Name + " Vehicle Seat" ).Put_Z_Depth_Offset( this->Seat_Offset );
   CompRef.Get_World_Ref( Name + " Vehicle Floor" ).Put_Z_Depth_Offset( this->Floor_Offset );

   return;
   } 

/*-----------------------------------*/ void       //-------------------------
Vehicle_Locals_Record ::				       Remove_Graphics
(
Composition_Record&   CompRef
)
//Removes all associated world graphics. This function is called from a 
//World_Group_Record in a Graphics_Database object when that object is removed
//from the database. As such, it is not used by the world outside of the graphics
//database.

//To be more true to the Macro metaphor, this function should received the name
//of a Composition object (as opposed to a reference) and call the GDB functions
//instead of the Composition functions directly.  
   {
   assert(  Is_Object_Ref( CompRef )  );

   String&   Name = this->Record::Key; //This group name

   CompRef.Remove_Graphic( Name + " Dashboard" );
   CompRef.Remove_Graphic( Name + " Steering" );
   CompRef.Remove_Graphic( Name + " Vehicle Seat" );
   CompRef.Remove_Graphic( Name + " Vehicle Floor" );
   
   return;
   }
   
/*-----------------------------------*/ void       //-------------------------
Vehicle_Locals_Record ::					   Put_Offsets
(
double   PutDashboardOffset ,
double   PutSteeringOffset ,
double   PutSeatOffset ,
double   PutFloorOffset
)
   {
   this->Dashboard_Offset = PutDashboardOffset;
   this->Steering_Offset  = PutSteeringOffset;
   this->Seat_Offset      = PutSeatOffset;
   this->Floor_Offset     = PutFloorOffset;

   this->Update();
   
   return;
   }
   