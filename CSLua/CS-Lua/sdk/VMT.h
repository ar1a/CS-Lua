#pragma once
#include <Windows.h>
#include "HookBase.h"
#pragma warning(disable: 4715)
#pragma warning(disable: 4018)

class VMT : public HookBase
{
public:
	VMT();
	VMT(void* object);
	virtual ~VMT();

	/*
	* Initializes the class using the designated object pointer
	*/
	virtual bool init();

	/*
	* Shutdown the class and clean everything up
	*/
	virtual void shutdown();

	/*
	* Removes all method hooks and _vftp hook
	*/
	virtual void removeHooks();

	/*
	* Sets the target object for hooking
	*/
	virtual void setTargetObject(void* object);

	/*
	* Returns the target object
	*/
	virtual void* getTargetObject();

	/*
	* Copies the table and replaces the vftp with a pointer to the copy
	* Returns true if successful
	*/
	virtual bool setTableHook(bool hooked = true);

	/*
	* Replaces the function designated at index with another function.
	* Call setTableHook before this if you want to hook the _Vftp
	* Returns pointer to the original function.
	*/
	virtual void* hookFunction(int index, void* hookedFunction);

	/*
	* Replaces the function designated at index with it's original function
	* Returns true if successful
	*/
	virtual bool unhookFunction(int index);

	/*
	* Is the object and table pointer valid
	*/
	virtual bool isValid();

	/*
	* Returns a vfunc from a vtable.
	*/
	template<typename T> static T getvfunc(void *base, unsigned int index)
	{
		DWORD** tablepointer = (DWORD**)base;
		DWORD* tablebase = *tablepointer;
		DWORD addr = tablebase[index];
		return (T)(addr);
	}
	/*
	* Returns the address from the original table of functions.
	*/
	virtual void* getOriginalFunction(int index);


private:
	virtual unsigned int countTable();

	//Whether we installed a vftp hook or not
	bool m_bVftpHooked;

	//Whether we have installed tablehooks or not
	bool m_bTableHooked;

	//Whether the object is a valid pointer
	bool m_bValidObject;

	//Whether the table is a valid pointer
	bool m_bValidTable;

	//List of bools designating whether a method is hooked or not
	bool *m_pbHookedMethods;

	//Number of hooks installed
	int m_iHookCount;

	//vtable count
	unsigned int m_iCount;

	//Allocated when _vftp hook is installed
	void* m_pCopyTable;

	//Pointer to class object to be hooked
	void* m_pObject;

	//A backup table
	void* m_pBackupTable;

	//A pointer to the original table
	void* m_pOriginalTable;
};