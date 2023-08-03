#include <stdio.h>
#include <string.h>
#include "types/cm_guid.hpp"
#include "cm.h"

using namespace CyanMycelium;

Guid::Guid(bool set)
{
	if (set)
	{
		this->Set();
	}
	else
	{
		this->Clear();
	}
}

bool Guid::IsEmpty()
{
	int i;
	for (i = 0; i < 4; i++)
		if (this->_raw32[i] != 0)
			return false;
	return true;
}

void Guid ::Clear()
{
	cm_memset(this->_raw, 0, sizeof(this->_raw));
}

void Guid ::Set()
{
	int i;
	for (i = 0; i < 4; i++)
		this->_raw32[i] = cm_rand();
}

#define UI(c) (unsigned int)(c)
int Guid::ToString(char *dest, int length)
{
	return snprintf(dest, length, "%2hhX%2hhX%2hhX%2hhX-%2hhX%2hhX-%2hhX%2hhX-%2hhX%2hhX-%2hhX%2hhX%2hhX%2hhX%2hhX%2hhX",
					this->_raw[0],
					this->_raw[1],
					this->_raw[2],
					this->_raw[3],
					this->_raw[4],
					this->_raw[5],
					this->_raw[6],
					this->_raw[7],
					this->_raw[8],
					this->_raw[9],
					this->_raw[10],
					this->_raw[11],
					this->_raw[12],
					this->_raw[13],
					this->_raw[14],
					this->_raw[15]);
}

int Guid::CreateString(char *dest, int length)
{
	int i;
	union
	{
		uint8_t _raw[16];
		uint32_t _raw32[4];
	};
	for (i = 0; i < 4; i++)
		_raw32[i] = cm_rand();

	return snprintf(dest, length, "%2hhX%2hhX%2hhX%2hhX-%2hhX%2hhX-%2hhX%2hhX-%2hhX%2hhX-%2hhX%2hhX%2hhX%2hhX%2hhX%2hhX",
					_raw[0],
					_raw[1],
					_raw[2],
					_raw[3],
					_raw[4],
					_raw[5],
					_raw[6],
					_raw[7],
					_raw[8],
					_raw[9],
					_raw[10],
					_raw[11],
					_raw[12],
					_raw[13],
					_raw[14],
					_raw[15]);
}

bool Guid::operator=(Guid *b)
{
	int i;
	for (i = 0; i < 4; i++)
		if (this->_raw32[i] != b->_raw32[i])
			return false;
	return true;
}

Guid *Guid::FromString(const char *src)
{
	Guid *res = new Guid();
	if (sscanf(src, "%2hhX%2hhX%2hhX%2hhX-%2hhX%2hhX-%2hhX%2hhX-%2hhX%2hhX-%2hhX%2hhX%2hhX%2hhX%2hhX%2hhX",
			   &(res->_raw[0]),
			   &(res->_raw[1]),
			   &(res->_raw[2]),
			   &(res->_raw[3]),
			   &(res->_raw[4]),
			   &(res->_raw[5]),
			   &(res->_raw[6]),
			   &(res->_raw[7]),
			   &(res->_raw[8]),
			   &(res->_raw[9]),
			   &(res->_raw[10]),
			   &(res->_raw[11]),
			   &(res->_raw[12]),
			   &(res->_raw[13]),
			   &(res->_raw[14]),
			   &(res->_raw[15])) != 16)
	{
		delete res;
		return NULL;
	}

	return res;
}

Guid *Guid::Create()
{
	return new Guid(true);
}

uint8_t Guid::operator[](int idx) const
{
	if ((idx < 0) || (idx > (int)sizeof(_raw)))
		return 0;
	return this->_raw[idx];
}

uint8_t &Guid::operator[](int idx)
{
	return this->_raw[idx];
}
