#ifndef _UNDO_APPLIED_H_
#define _UNDO_APPLIED_H_

#include "UndoRedo.h"
#include "hom.h"
#include "hom_doc.h"
#include "skeleton.hpp"
#include <set>

class UndoableHandLoads : public UndoableEvent
{
    public:
        /*UndoableHandLoads(Vector3 oldLF,Vector3 oldRF,Vector3 oldLT,Vector3 oldRT) : UndoableEvent("Applied Hand Loads"),
            _oldLF(oldLF),_oldRF(oldRF),_oldLT(oldLT),_oldRT(oldRT)
        {
			Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
            _newLF = skel.getExtForce(JT_LHAND);
            _newRF = skel.getExtForce(JT_RHAND);
            _newLT = skel.getExtTorque(JT_LHAND);
            _newRT = skel.getExtTorque(JT_RHAND);
        }*/
		UndoableHandLoads(Vector3 oldLF, Vector3 oldRF, Vector3 oldLT, Vector3 oldRT,
							Vector3 newLF, Vector3 newRF, Vector3 newLT, Vector3 newRT,
							int frame) : UndoableEvent("Applied Hand Loads"),
							_oldLF(oldLF), _oldRF(oldRF), _oldLT(oldLT), _oldRT(oldRT),
							_newLF(newLF), _newRF(newRF), _newLT(newLT), _newRT(newRT) {
			_frame = frame;
		}
        virtual ~UndoableHandLoads(void) { }

    protected:
        virtual void undo(void)
        {
			//Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
			Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(_frame);
            skel.setExtForce(JT_LHAND, _oldLF);
            skel.setExtForce(JT_RHAND, _oldRF);
            skel.setExtTorque(JT_LHAND, _oldLT);
            skel.setExtTorque(JT_RHAND, _oldRT);
			// TODO analyze?
        }
        virtual void redo(void)
        {
			//Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
			Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(_frame);
            skel.setExtForce(JT_LHAND, _newLF);
            skel.setExtForce(JT_RHAND, _newRF);
            skel.setExtTorque(JT_LHAND, _newLT);
            skel.setExtTorque(JT_RHAND, _newRT);
        }

    private:
        Vector3 _oldLF,_oldRF,_newLF,_newRF;
        Vector3 _oldLT,_oldRT,_newLT,_newRT;
};


class UndoableAppliedLoads : public UndoableEvent
{
	public:
		//UndoableAppliedLoads(Vector3 oldForces[NUM_JOINTS],Vector3 oldTorques[NUM_JOINTS]) : UndoableEvent("Applied Joint Loads")
		UndoableAppliedLoads(Vector3 oldForces[NUM_JOINTS], Vector3 oldTorques[NUM_JOINTS],
								Vector3 newForces[NUM_JOINTS], Vector3 newTorques[NUM_JOINTS],
								int frame) : UndoableEvent("Applied Joint Loads")
		{
			_frame = frame;
			memcpy(_newForces, newForces, sizeof(_newForces));
			memcpy(_newTorques, newTorques, sizeof(_newTorques));
			memcpy(_oldForces,oldForces,sizeof(_oldForces));
			memcpy(_oldTorques,oldTorques,sizeof(_oldTorques));
		}
		virtual ~UndoableAppliedLoads(void) { }

	protected:
		virtual void undo(void)
		{
			Skeleton* skel = Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(_frame);
			skel->setExtForces(_oldForces, true);
			skel->setExtTorques(_oldTorques, true);
		}
		virtual void redo(void)
		{
			Skeleton* skel = Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(_frame);
			skel->setExtForces(_newForces, true);
			skel->setExtTorques(_newTorques, true);
		}

	private:
		Vector3 _oldForces[NUM_JOINTS];
		Vector3 _newForces[NUM_JOINTS];

		Vector3 _oldTorques[NUM_JOINTS];
		Vector3 _newTorques[NUM_JOINTS];
};

#endif