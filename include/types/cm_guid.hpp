#ifndef _CM_GUID_H
#define _CM_GUID_H

#include <stdint.h>
#include <stdbool.h>

namespace CyanMycelium
{
	class Guid
	{
	public:
		static Guid Empty;

		Guid(bool set = false);
		int ToString(char *dest, int length);
		bool operator=(Guid *b);
		bool IsEmpty();
		void Clear();
		void Set();

		static Guid *FromString(const char *src);
		static Guid *Create();
		static int CreateString(char *dest, int length);

		uint8_t operator[](int idx) const;
		uint8_t &operator[](int idx);

	private:
		union
		{
			uint8_t _raw[16];
			uint32_t _raw32[4];
		};
	};
}

#endif