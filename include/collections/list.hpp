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
#ifndef __BP_LIST_HPP__
#define __BP_LIST_HPP__
#include <stdint.h>

namespace CyanMycelium
{
	class List
	{
	public:
		List (int32_t(*comparer)(void * a, void * b), int initialCount = 16, int maxCount = 0);
		List (int initialCount = 16, int maxCount = 0);

		bool Add(void * element, bool replace = false);
		bool Remove(void * element);
		bool RemoveAt(int idx);
		int IndexOf(void * element);
		void * Get(int idx) const;
		void * operator[](int idx) const;
		void Clear();
		void Trim();
		int GetCount() const;
		~List();

		typedef void(*forEachFunction)(void * element);
		void ForEach(forEachFunction forEach);

		static int32_t StringComparer(void * a, void * b);
		static List * StringList(int initialCount = 16, int maxCount = 0);

	private:
		bool EnsureEnoughCapacity(int requested);

		int32_t(*comparer)(void * a, void * b);
		int count;
		int capacity;
		int maxCapacity;
		void ** buffer;
	};
}

#endif
