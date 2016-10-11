#ifndef ARCHIVER_H
#define ARCHIVER_H

// File created by Luke Williams
//	7/26/04

// The "Archiver" class is used as a function object.
//
// The constructor takes an archive, and a bool to determine whether we're
// storing or retrieving.
//
// See Hom_srl_440.cpp for usage example
//
// The purpose of the object is to allow all serialization (storing and
// retrieving) to be accomplished in one function, so there's never a
// discrepancy between the order or presence of variables in the store/retrieve
// functions.  This is a danger that has caused problems in the past.

class Archiver
{
public:
   Archiver(CArchive& in_arch) : mArch(in_arch)
   { }

   template <class T1>
   void operator () (T1 & data)
   {
      if(mArch.IsStoring())
         mArch << data;
      else  // we're loading
         mArch >> data;
   }

   // Second operator provided for operations on enumerated
   //    data, which CArchive doesn't know how to handle.  To
   //    write enumerated data, pass in the data itself and
   //    a temporary variable of an appropriate type.
   template <class T1, class T2>
   void operator () (T1 & data, T2 & tempVar)
   {
      if (mArch.IsStoring())
      {
         tempVar = data;
         mArch << tempVar;
      }
      else  // we're loading
      {
         mArch >> tempVar;
         data = T1(tempVar);  // Convert tempVar to a T1 and assign to "data"
      }
   }
      
private:
   // Reference to the archive in use
   CArchive& mArch;
};

#endif