#ifndef _UNDOREDO_H_
#define _UNDOREDO_H_

#include <deque>
#include <string>
#include <vector>
#include "jointangles.hpp"

class UndoableEvent;

class UndoHistory
{
    public:
        UndoHistory(void);
        ~UndoHistory(void) { clearHistory(); }

        bool undo(void);
        bool redo(void);

        std::string nextUndoString(void) const;
        std::string nextRedoString(void) const;

        bool canUndo(void) const;
        bool canRedo(void) const;

        void addUndoableEvent(UndoableEvent *event);
        void clearHistory(void);
		// removes events after current position
		// call this when breaking the undo/redo state, but not adding an event
		// e.g. Cutting frames. Either killRedo or make it an event
		void killRedo(void);

    private:
        std::deque<UndoableEvent*> _queue;
        int _index;
};

// individual UndoableEvent classes; these should all derive from the pure virtual UndoableEvent class
class UndoableEvent
{
	//friend class UndoHistory;
public:
	UndoableEvent(std::string str = "", int frame = 0) : _string(str), _frame(frame) { }
	virtual ~UndoableEvent(void) { }

public:
	virtual void undo(void) = 0;
	virtual void redo(void) = 0;
	virtual std::string getString() { return _string; }

protected:
	std::string _string;
	int _frame;
};

class GroupEvent : public UndoableEvent
{
public:
	GroupEvent(int firstFrame, int lastFrame);
	virtual ~GroupEvent();
	void addEvent(UndoableEvent* newEvent);
	virtual void undo(); // undo all events in group
	virtual void redo(); // redo all events in group
	virtual std::string getString(); // get representative string of group
private:
	std::vector<UndoableEvent*> events;
	int _lastFrame;
};

#endif