// ======================================================================
//
// InterlockedVoidPointer.h
// Acy Stapp
//
// Copyright 6/19/2001 Sony Online Entertainment
//
// ======================================================================

#ifndef INCLUDED_InterlockedVoidPointer_h
#define INCLUDED_InterlockedVoidPointer_h

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class InterlockedVoidPointer
{
public:
	explicit InterlockedVoidPointer(void * initialValue=0);
	void * compareExchange(void * compare, void * exchange); // compare and exchange if same
	operator void * () const { return reinterpret_cast<void *>(value); }
private:
	InterlockedVoidPointer(const InterlockedVoidPointer &o);
	InterlockedVoidPointer &operator =(const InterlockedVoidPointer &o);
	void * volatile value;
};

template <class T>
class InterlockedPointer: public InterlockedVoidPointer
{
public:
	explicit InterlockedPointer(T * initialValue): InterlockedVoidPointer(initialValue) {}
	operator T * () const { return static_cast<const T *>(value); }
};

inline InterlockedVoidPointer::InterlockedVoidPointer(void * initialValue)
{
	value = initialValue;
}

inline void * InterlockedVoidPointer::compareExchange(void * compare, void * exchange)
{
	return (void *)InterlockedCompareExchange((void **)&value, exchange, compare);
}

#endif