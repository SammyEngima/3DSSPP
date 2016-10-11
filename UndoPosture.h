#ifndef _UNDO_POSTURE_H_
#define _UNDO_POSTURE_H_

#include "UndoRedo.h"
#include "hom.h"
#include "hom_doc.h"
#include "Skeleton.hpp"
#include "Analysis.h"
#include "Types.h"

// TODO redesign

class UndoablePostureChange : public UndoableEvent
{
    public:
        UndoablePostureChange(int frame, const JointAngles &oldAngles,const JointAngles &newAngles, FrameType_e oldFrameType, FrameType_e newFrameType)
            : UndoableEvent("Posture Change", frame), _oldAngles(oldAngles), _newAngles(newAngles), _oldFrameType(oldFrameType), _newFrameType(newFrameType) {
		}
        virtual ~UndoablePostureChange(void) { }

    protected:
        virtual void undo(void)
        {
			C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
			Skeleton &skel = *doc->getSkeletonAtFrame(_frame);
			skel.SetAngles(_oldAngles);
			doc->setFrameType(_oldFrameType, _frame);
			if(_frame != doc->getCurrentFrame()) {
				Analysis analysis;
				analysis.Analyze(&skel);
				doc->setCurrentFrame(_frame);
				doc->setSelection(_frame, _frame);
			}

            Get_App_Ptr()->Get_Document_Ptr()->MakeDirtyAndUpdateViews(true);
        }
        virtual void redo(void)
        {
            C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
			Skeleton& skel = *doc->getSkeletonAtFrame(_frame);
			skel.SetAngles(_newAngles);
			doc->setFrameType(_newFrameType, _frame);
			if(_frame != doc->getCurrentFrame()) {
				Analysis analysis;
				analysis.Analyze(&skel);
				doc->setCurrentFrame(_frame);
				doc->setSelection(_frame, _frame);
			}

            Get_App_Ptr()->Get_Document_Ptr()->MakeDirtyAndUpdateViews(true);
        }

    private:
        JointAngles _oldAngles;
        JointAngles _newAngles;
		FrameType_e _oldFrameType;
		FrameType_e _newFrameType;
};

class UndoablePresetPosture : public UndoableEvent
{
    public:
        UndoablePresetPosture(int frame,const JointAngles &oldAngles,const JointAngles &newAngles,Vector3 oldForces[NUM_JOINTS])
            : UndoableEvent("Preset Posture", frame), _oldAngles(oldAngles), _newAngles(newAngles)
        {
            for(int i = 0; i < NUM_JOINTS; ++i)
                _newForces[i] = Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton()->getExtForce((JointID)i);
            memcpy(_oldForces,oldForces,sizeof(_oldForces));
        }
        virtual ~UndoablePresetPosture(void) { }

    protected:
        virtual void undo(void)
        {
			C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
			Skeleton &skel = *doc->getSkeletonAtFrame(_frame);

			skel.SetAngles(_oldAngles);
            for(int i = 0; i < NUM_JOINTS; ++i)
				skel.setExtForce((JointID)i,_oldForces[i]);

			if(_frame != doc->getCurrentFrame()) {
				Analysis analysis;
				analysis.Analyze(&skel);
				doc->setCurrentFrame(_frame);
			}

			Get_App_Ptr()->Get_Document_Ptr()->MakeDirtyAndUpdateViews(true);

        }
        virtual void redo(void)
        {
			C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
			Skeleton &skel = *doc->getSkeletonAtFrame(_frame);

			skel.SetAngles(_newAngles);
			for(int i = 0; i < NUM_JOINTS; ++i)
				skel.setExtForce((JointID)i, _newForces[i]);

			if(_frame != doc->getCurrentFrame()) {
				Analysis analysis;
				analysis.Analyze(&skel);
				doc->setCurrentFrame(_frame);
			}

            Get_App_Ptr()->Get_Document_Ptr()->MakeDirtyAndUpdateViews(true);
        }

    private:
        JointAngles _oldAngles;
        JointAngles _newAngles;
        Vector3 _oldForces[NUM_JOINTS];
        Vector3 _newForces[NUM_JOINTS];
};

#endif