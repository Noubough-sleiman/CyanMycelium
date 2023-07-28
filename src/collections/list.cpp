//                     DotVision BluePanda - CONFIDENTIAL
//                           All Rights Reserved
// ____________________________________________________________________________
// 
// All information contained herein is, and remains the property of DotVision. 
// The intellectual and technical concepts contained herein remains the 
// property of DotVision and are protected by trade secret or copyright law.
// Dissemination of this information or reproduction of this material is 
// strictly forbidden unless prior written permission is obtained from 
// DotVision.
// 
// Usage of this content, in source, executable or binary format is strictly 
// reserved to authorized DotVision clients and partners
//
// Please refer to the latest DotVision BluePanda license for exhaustive 
// information and limitations.
//
// This file is part of the BluePanda real-time operating system.
// ____________________________________________________________________________
#include <stdlib.h>
#include "./collections/list.hpp"

using namespace CyanMycelium;

List::List (int32_t(*comparer)(void * a, void * b), int initialCount, int maxCount)
{
	this->comparer = comparer;
	this->count = 0;
	this->capacity = initialCount;
	this->maxCapacity = maxCount;
	this->buffer = (void**)malloc(this->capacity * sizeof(void*));
}

List::List (int initialCount, int maxCount)
{
	this->comparer = NULL;
	this->count = 0;
	this->capacity = initialCount;
	this->maxCapacity = maxCount;
	this->buffer = (void**)malloc(this->capacity * sizeof(void*));
}

bool List::EnsureEnoughCapacity(int requested)
{
	if (this->capacity < requested)
	{
		if (this->maxCapacity > 0)
		{
			if (this->capacity == this->maxCapacity) return false;
			if (requested > this->maxCapacity) return false;
		}
		if ((this->maxCapacity > 0) && (this->capacity == this->maxCapacity)) return false;
		if ((this->maxCapacity > 0) && (requested > this->maxCapacity)) return false;

		/* Grow the list */
		int newCapacity = this->capacity;
		do
		{
			newCapacity = (newCapacity * 3) / 2;
			if ((this->maxCapacity > 0) && (newCapacity > this->maxCapacity)) newCapacity = this->maxCapacity;
		} while (newCapacity < requested);
		
		void** newbuf = (void**)realloc(this->buffer, newCapacity * sizeof(void*));
		if (newbuf == NULL) return false;
		this->buffer = newbuf;
		return true;
	}
	else
	{
		/* We have enough space */
		return true;
	}
}

bool List::Add(void * element, bool replace)
{
	if (!EnsureEnoughCapacity(this->count + 1)) return false;

	if (this->comparer == NULL)
	{
		/* Standard list - replace is ignored */
		this->buffer[this->count++] = element;
	}
	else
	{
		/* Sorted list */
		int i, j;
		for (i = 0; i < this->count; i++)
		{
			if (this->comparer(this->buffer[i], element) >= 0) break;
		}

		if (replace && (this->comparer(this->buffer[i], element) == 0))
		{
			/* Replace at i */
			this->buffer[i] = element;
		}
		else
		{
			/* Insert at i */
			for (j = this->count; j > i; j--)
			{
				this->buffer[j] = this->buffer[j - 1];
			}

			this->buffer[i] = element;
			this->count++;
		}
	}
	return true;
}

bool List::Remove(void * element)
{
	int index = IndexOf(element);
	if (index < 0) return false;
	return RemoveAt(index);
}

bool List::RemoveAt(int idx)
{
	if ((idx < 0) || (idx >= this->count)) return false;
	int i;
	for (i = idx + 1; i < this->count; i++)
	{
		this->buffer[i - 1] = this->buffer[i];
	}
	this->count--;
	return true;
}

int List::IndexOf(void * element)
{
	if (this->comparer == NULL)
	{
		/* Standard list - Compare addresses */
		int i;
		for (i = 0; i < this->count; i++)
		{
			if (this->buffer[i] == element) return i;
		}
		return -1;
	}
	else
	{
		/* Sorted list - Use comparer */
		int i;
		for (i = 0; i < this->count; i++)
		{
			int32_t compare = this->comparer(this->buffer[i], element);
			if (compare == 0) return i;
			else if (compare > 0) return -1; /* Cannot find requested element */
		}
		return -1;
	}
}

void * List::Get(int idx) const
{
	if ((idx < 0) || (idx >= this->count)) return NULL;
	return this->buffer[idx];
}

void * List::operator[](int idx) const
{
	if ((idx < 0) || (idx >= this->count)) return NULL;
	return this->buffer[idx];
}

void List::Clear()
{
	this->count = 0;
}

void List::Trim()
{
	void** newbuf = (void**)realloc(this->buffer, this->count * sizeof(void*));
	if (newbuf != NULL)
	{
		this->buffer = newbuf;
		this->capacity = this->count;
	}
}

int List::GetCount() const
{
	return this->count;
}

List::~List()
{
	if (this->buffer != NULL) free(this->buffer);
}

void List::ForEach(forEachFunction forEach)
{
	int i;
	for (i = 0; i < this->count; i++)
	{
		forEach(this->buffer[i]);
	}
}
