#ifndef MATH_QUATERNION_H 
#define MATH_QUATERNION_H

#pragma once
#include "math_vector.h"

namespace perihelion
{

   // Type Definitions
   typedef pVector4f pQuat;
   typedef pVector4d pQuat_d;

   // Globals
   extern pVector4f g_quatIdent;
   extern pVector4d g_quatIdent_d;

   #ifndef __MATH_QUAT__
   #define __MATH_QUAT__
      void pQuatMultiply_d          ( const pVector4d a   , const pVector4d b     , pVector4d out  );
      void pQuatInverse_d           ( const pVector4d in  , pVector4d out                          );
      void mdpQuatFromAngleAxis_d   ( const GLdouble theta, const pVector3d axis  , pVector4d quat );

      void pQuatMultiply         ( const pVector4f a  , const pVector4f b         , pVector4f out  );
      void pQuatInverse          ( const pVector4f in , pVector4f out                              );
      void mdpQuatFromAngleAxis  ( const GLfloat theta, const pVector3f axis      , pVector4f quat );

      // (LCW) This statement not used anywhere, generates double-define warnings
      //#define pQuatConjugate(a,b) ((b)[0]=-(a)[0],(b)[1]=-(a)[1],(b)[2]=-(a)[2],(b)[3]=(a)[3],pVectorNormalize4f(b,b))   
   #endif

   void pQuatToMatrix      (pVector4f q, GLfloat m[4][4]);
   void pQuatRotateVector  (const pVector4f quat, const pVector3f vin, pVector3f vout );
   void pQuatRotateVector_d(const pQuat_d quat  , const pVector3d vin, pVector3d vout );
}

#endif
