#include <string>
#include <vector>
#include "UndoRedo.h"
#include "Hom.h"
#include "Hom_Doc.h"

using std::string;
using std::vector;

#define maxFrameNum 10

UndoHistory::UndoHistory(void) : _index(-1) { _queue.clear(); }

bool UndoHistory::canUndo(void) const { return _index >= 0; }
bool UndoHistory::canRedo(void) const { return (int)_index < (int)_queue.size() - 1; }

string UndoHistory::nextUndoString(void) const
{
    if(!canUndo()) return NULL;
    //return _queue[_index]->_string;
	return _queue[_index]->getString();
}

string UndoHistory::nextRedoString(void) const
{
    if(!canRedo()) return NULL;
    //return _queue[_index + 1]->_string;
	return _queue[_index + 1]->getString();
}

bool UndoHistory::undo(void)
{
    if(!canUndo()) return false;

    _queue[_index]->undo();
    --_index;

    return true;
}

bool UndoHistory::redo(void)
{
    if(!canRedo()) return false;

    ++_index;
    _queue[_index]->redo();

    return true;
}

void UndoHistory::addUndoableEvent(UndoableEvent *event)
{	
	while(_queue.size() > _index + 1)
    {
        UndoableEvent *e = _queue.back();
		_queue.pop_back();

        delete e;
		e = NULL;
    }

	// Limit _queue size to be 10
	while (_queue.size() >= maxFrameNum) {
        UndoableEvent *e = _queue.front();
		_queue.pop_front();
		--_index;

        delete e;
		e = NULL;
	}

    _queue.push_back(event);
    ++_index;
}

void UndoHistory::clearHistory(void)
{
    for(int i = 0; i < _queue.size(); ++i) {
        delete _queue[i];
		_queue[i] = NULL;
	}
    _queue.clear();

    _index = -1;
}

void UndoHistory::killRedo(void) {
	while(_queue.size() > _index + 1)
	{
		UndoableEvent *e = _queue.back();
		_queue.pop_back();
		
		delete e;
		e = NULL;
	}
}

GroupEvent::GroupEvent(int firstFrame, int lastFrame) : UndoableEvent("", firstFrame), _lastFrame(lastFrame) {}
GroupEvent::~GroupEvent() {
	for(int i = 0; i < events.size(); i++) {
		delete events[i];
		events[i] = NULL;
	}
}
void GroupEvent::addEvent(UndoableEvent* newEvent) {
	events.push_back(newEvent);
}
// undo all events in group
void GroupEvent::undo() {
	for(int i = 0; i < events.size(); i++) {
		events[i]->undo();
	}
	// set selection to be frames undo applied to
	int currentFrame = Get_App_Ptr()->Get_Document_Ptr()->getCurrentFrame();
	if(currentFrame < _frame || currentFrame > _lastFrame) {
		Get_App_Ptr()->Get_Document_Ptr()->setCurrentFrame(_frame);
	}
	Get_App_Ptr()->Get_Document_Ptr()->setSelection(_frame, _lastFrame);
	
}
// redo all events in group
void GroupEvent::redo() {
	for(int i = 0; i < events.size(); i++) {
		events[i]->redo();
	}
	// set selection to be frames redo applied to
	int currentFrame = Get_App_Ptr()->Get_Document_Ptr()->getCurrentFrame();
	if(currentFrame < _frame || currentFrame > _lastFrame) {
		Get_App_Ptr()->Get_Document_Ptr()->setCurrentFrame(_frame);
	}
	Get_App_Ptr()->Get_Document_Ptr()->setSelection(_frame, _lastFrame);
}
// get representative string of group
string GroupEvent::getString() {
	if(events.size()) {
		return events[0]->getString();
	} else {
		return "Empty group operation";
	}
}