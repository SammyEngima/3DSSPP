//############################## HEADER FILE ############################ Difference_Count
#ifndef      Difference_Count_Include
   #define   Difference_Count_Include

//--------------------- Include Files and Declarations
#include "Dimension.hpp"
#include "HighLowAttribute.hpp"
#include "JointAngles.hpp"

//---------------------------- Class Definition ------------------------------
class                                                         Difference_Count
{
   //--- Interface Functions ----
public:
   //--- Constructors ---
   //--- Accessors ---
   int Present() const {
      return this->Count;
   };
                        
   bool Different(float Source, float Target);
   bool Different(double aSource, double aTarget);
   bool Different(int Source, int Target);
   bool Different(const std::string& Source, const std::string& Target);
   bool Different(JointAngles& Source, JointAngles& Target);                                
   //--- Mutators ---
   void Reset() {
      this->Count = 0;
   };
      
   void Set_System_To_Dialog_Conversion(float SetConversion);
   void Load_Dialog(float& DialogTargetRef,
                    float SystemSourceRef,
                    float& TemporaryRef,
                    int Decimals = 0);

   // Added by LCW 8/27/04
   void Load_Dialog(double& DialogTargetRef,
                    double SystemSourceRef,
                    double& TemporaryRef,
                    int Decimals = 0);

   void Load_Dialog(int& DialogTargetRef,
                    float SystemSourceRef,
                    int& TemporaryRef);

   int Update_System(const float& DialogSourceRef,
                     float& SystemTargetRef,
                     const float& TemporaryRef,
                     int Decimals);

   // Added by LCW 8/27/04
   int Update_System(const double& DialogSourceRef,
                     double& SystemTargetRef,
                     const double& TemporaryRef,
                     int Decimals);

   int Update_System(const int& DialogSourceRef,
                     float& SystemTargetRef,
                     const int& TemporaryRef);
                           
   int Update_System(const int& DialogSourceRef,
                     int& SystemTargetRef,
                     const int& TemporaryRef);

   int Update(int& SourceRef, int& TargetRef);
   int Update(BYTE& SourceRef, BYTE& TargetRef);
   int Update(float& SourceRef, float& TargetRef);
   int Update(const CString& SourceRef, CString& TargetRef);
   int Update(const float& SourceRef, Dimension& TargetRef);
   int Update(JointAngles& SourceRef, JointAngles& TargetRef);

//      bool            Update
//                         ( 
//                         const float &              SourceRef ,
//                               HighLowAttribute &   TargetRef
//                         );

   Difference_Count operator + (const Difference_Count& RightOpRef) const
   {
      Difference_Count Temp;
      Temp.Count = this->Count + RightOpRef.Count;
      return Temp;
   };
                                                     
   //--- Concrete ---
public:
   Difference_Count();

//                      Difference_Count
//                         (
//                         const Difference_Count&   CopyRef
//                         );

    Difference_Count& operator = (const Difference_Count& RightOpRef)
    {   
       this->Count = RightOpRef.Count;
       return  *this;
    };

//      virtual        ~Difference_Count
//                         ( void );

//------------------------- Class Attributes ---------------------------------
                      //--- Object Attributes ---
                                     //--- Contained State ---
protected:
   int Count;
   float System_To_Dialog_Conversion;
   float Dialog_To_System_Conversion;
                                     //--- Shared State ---
                                     //--- Derived ---
                      //--- Associated Classes ---

//Role:
//   Helper class for carrying on transactions between a document and a dialog
//   box. This implementation assumes all system length units are millimeters. 

//Design Notes:
//   Future designs may more appropriately incorporate the functionality of
//   this class into a class derived from CDialog.
};
#endif
