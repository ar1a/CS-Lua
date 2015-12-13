/*CREDITS: lilneo*/
#pragma once
#include <Windows.h>


class Address
{
public:
	Address()
	{
		Zero();
	}

	Address(const void *ptr)
	{
		Set(ptr);
	}

	Address(DWORD addr)
	{
		Set(reinterpret_cast<const void*>(addr));
	}

	~Address()
	{
		Zero();
	}

	void *Pointer() const
	{
		return m_ptr;
	}

	void Zero()
	{
		Set(nullptr);
	}
	
	void Free()
	{
		if(m_ptr)
			delete m_ptr;
	}

	void FreeArray()
	{
		if(m_ptr)
			delete[] m_ptr;
	}

	void Set(const void *ptr)
	{
		m_ptr = (void*)ptr;
	}

	template<typename T> T As() const
	{
		return (T)m_ptr;
	}

	template <typename T> T To() const
	{
		return *reinterpret_cast<T*>(m_ptr);
	}

	void AddOffset(DWORD dwOffset)
	{
		m_ptr = (void*)(((DWORD)m_ptr) + dwOffset);
	}

	//subtract an offset
	void AddDelta(DWORD dwOffset)
	{
		m_ptr = (void*)(((DWORD)m_ptr) - dwOffset);
	}

	// offset the pointer
	Address GetOffset(DWORD dwOffset) const
	{
		return Address(As<char*>() + dwOffset);
	}

	//subtract an offset
	Address GetDelta(DWORD dwOffset) const
	{
		return Address(As<char*>() - dwOffset);
	}

	bool IsNull() const
	{
		return m_ptr == nullptr;
	}

	operator unsigned long() const
	{
		return As<unsigned long>();
	}

	operator void*() const
	{
		return Pointer();
	}

	bool operator==(void *p) const
	{
		return m_ptr == p;
	}

	bool operator!=(void *p) const
	{
		return m_ptr != p;
	}

	int ByteCmp(char *signature, DWORD Siglen)
	{
		char s, c, b;

		if(m_ptr == nullptr || signature == nullptr)
			return 0;

		// save the first byte
		s = signature[0];

		for(DWORD i = 0, next = 0; i < Siglen; i++)
		{
			b = As<char*>()[i];
			c = signature[i];

			// wildcard
			if(c == '?')
				continue;

			// save the next starting point
			if(b == s)
				next = i;

			// now do the comparison
			if(b != c)
				return next;
		}

		return -1;
	}

protected:
	void *m_ptr;
};
