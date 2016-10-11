#ifndef ENUM_H
#define ENUM_H

enum E_Model_Type
{
   MT_NONE,
   //MT_STICKPERSON,    // Removed in 4.4.0
   //MT_ROD,            // Removed in 4.4.0
   MT_FLESH,
   //MT_SUPERIMPOSED,   // Removed in 4.4.0
   MT_WIREFRAME
};

enum E_Units   // Must match order in dialog box
{
   U_ENGLISH,
   U_METRIC
};

// enum gender // Hominid gender
// {
// 	MALE,
// 	FEMALE
// };

// Used to convert shade numbers from old files into colors
enum E_Shade
{
   SH_PINK,
   SH_BLUE,
   SH_VIOLET,
   SH_CYAN,
   SH_GRAY,
   SH_PRINT
};

enum E_Light_Type
{
   LT_TR,
   LT_TL,
   LT_R,
   LT_L,
   LT_BR,
   LT_BL
};

enum E_Barrier_Type
{
   BT_NOBARRIER,
   BT_WALL,
   BT_TABLE
};

enum E_HHO_Type
{
   HHO_NONE,
   HHO_BOX,
   HHO_CYLINDER,
   HHO_SPHERE
};

enum
{
   STANCE_BOTH_FEET  = 0,
   STANCE_LEFT_FOOT  = 1,
   STANCE_RIGHT_FOOT = 2,
   STANCE_NO_FEET    = 3
};    

enum
{
   SEATED = 1,
   HAS_BACK = 2,
   HAS_FRONTSUPPORT = 4,
   HAS_FRICTION = 8
};

#endif