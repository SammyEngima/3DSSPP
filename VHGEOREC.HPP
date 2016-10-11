//############################## HEADER FILE ######### Vehicle_Geometry_Record
#ifndef      Vehicle_Geometry_Record_Include
   #define   Vehicle_Geometry_Record_Include
//--------------------- Include Files and Declarations -----------------------
#include "record.hpp"
#include "position.hpp"

class   String;
class   Graphic_Database;
         
//---------------------------- Class Definition ------------------------------
class                                                  Vehicle_Geometry_Record
   : public   Record
   {
//-------------------------- Interface Functions -----------------------------
                                     //--- Additional Constructors ---
   public:
                      Vehicle_Geometry_Record
                         (
		               Graphic_Database&   GDBRef ,
		         const String&             NameRef ,
		               int                 IniPercentSeatForward 
                         );
                                     //--- Accessors ---

      int             Get_Percent_Seat_Forward
                         ( void ) const;

                                     //--- Mutators ---
      virtual
      void            Update
                         ( void );

      void            Put_Percent_Seat_Forward
                         ( int   PutPercentSeatForward );
                                                  
                                     //--- Concrete ---
   public:
                      Vehicle_Geometry_Record
                         ( void );

                      Vehicle_Geometry_Record
                         (
                         const Vehicle_Geometry_Record&   CopyRef
                         );

      virtual        ~Vehicle_Geometry_Record
                         ( void );
   private:
                      Vehicle_Geometry_Record&   operator = //Disabled by private
                         (
	                 Vehicle_Geometry_Record&   AssignRef
 	                 );
//------------------------- Internal Functions -------------------------------
      void            Initialize
                         ( void );
//------------------------- Class Attributes ---------------------------------
                      //--- Object Attributes ---
                                     //--- Contained State ---
   protected:
      int             Percent_Seat_Forward;
      Position        Seat_Rail_Origin;
                                     //--- Shared State ---
   protected:
      Graphic_Database&   GDB;      //Reference to containing graphic DB                               
                                     //--- Derived ---
                      //--- Associated Classes ---
   };
//Role:
//   The underlying geometric aspect (basically everything except graphical
//presentation) of a simple vehicle interior.
#endif
