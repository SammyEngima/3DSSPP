// Vhlocrec.hpp

//############################## HEADER FILE ########### Vehicle_Locals_Record
#ifndef      Vehicle_Locals_Record_Include
   #define   Vehicle_Locals_Record_Include
//--------------------- Include Files and Declarations -----------------------

#include "lcgrprec.hpp"
#include "table.hpp"
#include "skel_rec.hpp"

class   Graphic_Database;
class   String;
class   Vehicle_Geometry_Record;

//---------------------------- Class Definition ------------------------------
class                                                    Vehicle_Locals_Record
   : public   Local_Group_Record
   {
//-------------------------- Interface Functions -----------------------------
                                     //--- Additional Constructors ---
   public:
                      Vehicle_Locals_Record
                         (
                               Graphic_Database&          GDBRef ,
                         const String&                    NameRef ,
                               Vehicle_Geometry_Record&   GeometryRecordRef ,
                         const String&                    SurfaceNameRef                     
                         );
                                     //--- Accessors ---
                                     //--- Mutators ---
      virtual
      void            Create_Graphics 
                         (
                         Composition_Record&   TargetCompositionRef
                         );
      virtual
      void            Remove_Graphics
                         (
                         Composition_Record&   TargetCompositionRef
                         );
      virtual
      void            Update
                         ( void );                                           
      virtual
      void            Update_Graphics
                         (
                         Composition_Record&   TargetCompositionRef
                         );
                         
      void            Put_Offsets
                         (
                         double   PutDashboardOffset ,
                         double   PutSteeringOffset ,
                         double   PutSeatOffset ,
                         double   PutFloorOffset
                         );
                                     //--- Concrete ---                                     
   public:
                      Vehicle_Locals_Record
                         ( void );

                      Vehicle_Locals_Record
                         (
                         const Vehicle_Locals_Record&   CopyRef
                         );

      virtual        ~Vehicle_Locals_Record
                         ( void );

   private:
                      Vehicle_Locals_Record&   operator = //Disabled by private
                         (
	                 Vehicle_Locals_Record&   AssignRef
	                 );
//------------------------- Internal Functions -------------------------------
   private:
      virtual                   
      void            Create_Locals
                         (
                         const String&   SurfaceNameRef
                         );                         

      void            Create_Shapes
                         ( void );


//------------------------- Class Attributes ---------------------------------
                      //--- Object Attributes ---
                                     //--- Contained State ---
                                     //--- Shared State ---
      
      Vehicle_Geometry_Record*   Vehicle_Geometry_Ptr; //Underlying geometry
                                     
                                     //Graphic priority WRT camera
      double                      Dashboard_Offset;
      double                      Steering_Offset;   
      double                      Seat_Offset;
      double                      Floor_Offset;
                                     
                                     //--- Derived ---
                      //--- Associated Classes ---
   };
//Role:
//Provides the corresponding shape definitions for a simple vehicle interior.
//Also controls the placement of these shapes as scaled graphics in a composition 
//space when used in conjunction with the corresponding geometry object. Offsets
//control the overlapping of the indicated graphics with surrounding objects by
//artificially moving them (without actually moving them) farther away from or
//closer to the camers.
#endif   
