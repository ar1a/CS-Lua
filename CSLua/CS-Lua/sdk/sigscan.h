#pragma once

#include "Address.h"

class SigScan
{
private:
	char* base_addr;
	size_t base_len;


public:
	
	SigScan(void *base_addr, size_t base_len);
	SigScan(const char *module_name);
	SigScan(void *insider_addr);

	Address Scan(const char* signature, int offset = 0);
	static Address CalcAbsAddress(Address address);
	bool IsValid();

	char *GetBase();
	size_t GetLength();

};