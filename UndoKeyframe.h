#ifndef _UNDO_KEYFRAME_H_
#define _UNDO_KEYFRAME_H_

#include "UndoRedo.h"
#include "hom.h"
#include "hom_doc.h"
#include "skeleton.hpp"
#include <vector>

// UndoableKeyframeAdd is the action taken when a single frame is selected, it is unset, and the user clicks the Add Frame button.
class UndoableKeyframeAdd : public UndoableEvent
{
    public:
        UndoableKeyframeAdd(int frame) : UndoableEvent("Frame Add", frame) {
		}
        virtual ~UndoableKeyframeAdd(void) { }

		virtual void undo(void) {
			//Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton().removeKeyframe(_frame);

			// _frame may no longer be current, so if we set the frame to unset, we will not maintain a temporary skeleton
			// so we won't be able to redo. Therefore, we set _frame to be current before setting it to unset
			C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
			doc->setCurrentFrame(_frame);
			// set the frame (now current) to unset
			doc->setFrameType(FT_Unset);
		}
		virtual void redo(void) {
			//Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton().addKeyframe(_frame);

			// make sure _frame is current before redoing
			C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
			doc->setCurrentFrame(_frame);
			doc->setFrameType(FT_Keyframe);
		}

    private:
};

// UndoableKeyframeRemove is the action taken when 1 or more frames is deleted when the user clicks on the Remove Frame button
class UndoableKeyframeRemove : public UndoableEvent {
    public:
		UndoableKeyframeRemove(int frame, int numFrames, std::vector<Frame> frames) : UndoableEvent("Frame Remove", frame)
        {
            //Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton().GetAngles(_frame,_angles);
			_numFrames = numFrames;
			_frames = frames;
        }
        virtual ~UndoableKeyframeRemove(void) {
			if(deleteSkeletons) {
				for(int i = 0; i < _frames.size(); i++) {
					delete _frames[i].skeleton;
				}
			}
		}

        virtual void undo(void)
        {
            //Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton().addKeyframe(_frame);
            //Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton().SetAngles(_frame,_angles

			C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
			// insert spots to hold the restored frames
			doc->getTask().insertFrame(_numFrames, _frame - 1);
			TRACE("Insert %i frames at frame %i\n", _numFrames, _frame-1);
			// restore the contents of the frames
			for(int i = 0; i < _numFrames; i++) {
				doc->getTask().setFrame(_frame + i, _frames[i].skeleton, _frames[i].frameType);
			}
			// set selection to be the frames that were undeleted
			doc->setCurrentFrame(_frame);
			doc->setSelection(_frame, _frame + _numFrames - 1);

			// don't delete the skeletons if we destruct after undoing, because the skeletons are in use
			deleteSkeletons = false;
        }
		virtual void redo(void) {
			//Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton().removeKeyframe(_frame);

			// take frames out of frame sequence
			C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
			doc->getTask().extractFrames(_numFrames, _frame);

			// set frame to be just after removed frames
			doc->setCurrentFrame(_frame);
			doc->setSelection(_frame, _frame);

			doc->MakeDirtyAndUpdateViews();

			deleteSkeletons = true;
		}

    private:
        /*unsigned int _frame;
        JointAngles _angles;*/

		// UndoableEvent::_frame is the left frame of the selection
		// numFrames is the number of frames in the selection
		int _numFrames;
		// frames is the actual series of frames that was removed from the frame sequence
		// it contains the skeleton pointers and frame types
		std::vector<Frame> _frames;

		// true iff this event should delete the skeletons stored when it is destructed.
		// we should not delete the skeletons if we undo the event and then it is destroyed,
		// because the skeletons will be in use in the frame sequence
		bool deleteSkeletons;
};

// Inserting frames is inverse with removing. That is, the undo for insert is the redo for remove, and vice versa
class UndoableKeyframeInsert : public UndoableEvent {
public:
	UndoableKeyframeInsert(int frame, int numFrames, std::vector<Frame> frames) : UndoableEvent("Frame Insert") {
		removeEvent = new UndoableKeyframeRemove(frame, numFrames, frames);
	}
	virtual ~UndoableKeyframeInsert() {
	}

	virtual void undo() {
		removeEvent->redo();
	}
	virtual void redo() {
		removeEvent->undo();
	}
private:
	UndoableKeyframeRemove* removeEvent;
};

#endif