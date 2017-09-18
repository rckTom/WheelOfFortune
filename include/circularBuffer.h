#pragma once

#include <array>

template<class T,std::size_t bufferSize>
class circularBuffer
{
	typedef std::array<T,bufferSize> StorageClass;

	StorageClass buffer{};
	typename StorageClass::iterator head;
	typename StorageClass::iterator tail;

public:
	circularBuffer()
	{
		head = buffer.begin();
		tail = buffer.begin();
	}

	T pop()
	{
		T element;
		if(tail == buffer.end())
			tail = buffer.begin();
		element = *tail;
		tail++;
		return element;
	}

	std::size_t size()
	{
		if(head >= tail)
			return head-tail;
		else
			return head+buffer.end()-tail;
	}

	void push(T element)
	{
		if(head == buffer.end())
			head = buffer.begin();
		*head = element;
		head++;
		if(head > tail)
		{
			tail++;
			if(tail == buffer.end())
				tail = buffer.begin();
		}
	}
};
