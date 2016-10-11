//############################## HEADER FILE ################################
#ifndef      Palette_Include
   #define   Palette_Include
//--------------------- Include Files and Declarations

#include "object.hpp"
#include "afxwin.h"

class   CDC;

#ifndef NO_SHADES
class   Shade_Record;
#endif

struct   Log_Palette //Our dummy logical palette
            {
            WORD            Palette_Version;
            WORD            Palette_Number_Of_Entries;
            PALETTEENTRY*   Palette_Entries[ 256 ];
            };

union    Logical_Palette //Overlay of our and Windows palettes 
            { 
            Log_Palette     EC_Type; 
            LOGPALETTE      Win_Type; 
            };

//============================ Class Definition =============================
class                                                                 Palette
   : public  Object
   {
//-------------------------- Interface Functions -----------------------------
                                    //--- Constructors ---
   public:
                        Palette      
			   (
			   int          BaseIdx ,
			   const CDC&   DeviceContextRef
			   );
                                     //--- Accessors ---
      LOGPALETTE&       Get_Logical_Palette_Ref
                           ( void );

      LOGPALETTE*       Get_Logical_Palette_Ptr
                           ( void );

		//--- Concrete ---
   public:
                        Palette
			   ( void );

                        Palette
			   (
			   const Palette&   CopyRef
			   );

//    Palette&          operator = (Not permitted)
//                         (
//                         const Palette&   AssignRef
//                         );

      virtual          ~Palette
			   ( void );
			
//-------------------------- Internal Functions ------------------------------
//------------------------- Class Attributes ---------------------------------
   private:
      enum           {Max_Index = 245};
      enum           {Min_Index = 10 };
                      //--- Object Attributes ---
                                     //--- Contained State ---
   protected:
      Logical_Palette LP;
      int             Base_Index;  //Starting index in memory block
      int             Current_Index;
                                     //--- Shared State ---
                                     //--- Derived ---
                      //--- Associated Classes ---
   };

//Role:

#endif //------------------------------

