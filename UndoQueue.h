///////////////////////////////////////////////////////////////////////
// UndoQueue.h
//

///////////////////////////////////////////////////////////////////////
//	Declaration of the UndoQueue class
//	Written by Luke Williams, 8/17/04
//
// The idea of this class is to have a queue of objects,
//		only one of which is the "current" object, while
//		the rest can only be accessed by "undoing" and
//		"redoing."  Since there must be a "current" object,
//		the queue is never allowed to be empty.
//

#ifndef UNDOQUEUE_H
#define UNDOQUEUE_H

#include <vector>

template < class T >
class UndoQueue
{
public:
	// Constructor
	//		(Note: If not passed a value, constructor will
	//		push the default value of T as the first member)
	UndoQueue(T initVal = T());

	const T& GetCurrent() const;
	void SetCurrent(const T& newVal, bool clearQueue = false);

	// Functions to move the index forward/backward
	bool Undo();
	bool Redo();

	// Functions to test whether undo/redo are currently possible
	bool CanUndo() const;
	bool CanRedo() const;

private:
	std::vector <T> mQueue;
	int mIndex;
};

///////////////////////////////////////////////////////////////////////
//	Implementation of the UndoQueue class
//	Written by Luke Williams, 8/17/04

//	Init the queue with a value
template <class T>
inline UndoQueue<T>::UndoQueue(T initVal) : mIndex(0)
{
	mQueue.push_back(initVal);
}

// Get the current value pointed at by mValue
//	Returns a reference because the queue
//		may contain objects for which needless
//		copying is wasteful.
//	Labelled "const" to disallow editing of
//		the current object - the only operations
//		allowed are to redo it or add another
//		as the current.

template <class T>
inline const T& UndoQueue<T>::GetCurrent() const
{
	return mQueue.at(mIndex);
}

// Set the queue's current value to newVal
template <class T>
void UndoQueue<T>::SetCurrent(const T& newVal, bool clearQueue)
{
	// Clear the queue if the user specifies;
	//		otherwise, just erase all elements that
	//		were in the redo side of the queue (they
	//		are now permanently gone)
	if (clearQueue)
	{
		mQueue.clear();
	}
	else
	{
		int i = mIndex + 1;
		while (i < int(mQueue.size()))
			mQueue.pop_back();
	}

	// Add the new value to the queue
	mQueue.push_back(newVal);
	
	// Set the index to point to the new value
	mIndex = mQueue.size() - 1;
}

// Decrement mIndex if possible, which will cause GetCurrent
//		to return a value further back in the history
template <class T>
bool UndoQueue<T>::Undo()
{
	if (!CanUndo())
		return false;

	--mIndex;
	return true;
}

// Increment mIndex if possible, which will cause GetCurrent
//		to return a value further forward in the history
template <class T>
bool UndoQueue<T>::Redo()
{
	if (!CanRedo())
		return false;

	++mIndex;
	return true;
}

// Returns true if and only if the queue is currently undoable
template <class T>
inline bool UndoQueue<T>::CanUndo() const
{
	return (mIndex > 0);
}

// Returns true if and only if the queue is currently redoable
template <class T>
inline bool UndoQueue<T>::CanRedo() const
{
	return (mIndex < int(mQueue.size() - 1));
}

#endif

