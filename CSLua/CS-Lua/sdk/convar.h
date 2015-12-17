#pragma once

typedef int(*FnCommandCompletionCallback)(const char *partial, char commands[64][64]);
//extends ConCommandBase and IConVar (??)
class ConVar {
public:
	void* m_pNext;
	bool m_bRegistered;
	const char* m_pszName;
	const char* m_pszHelpString;
	int m_nFlags;

	void* m_pParent;
	const char *m_pszDefaultValue;
	char *m_pszString;
	int m_StringLength;
	float m_fValue;
	int m_nValue;
	bool m_bHasMin;
	float m_fMinVal;
	bool m_bHasMax;
	float m_fMaxVal;

	FnCommandCompletionCallback callback;
};