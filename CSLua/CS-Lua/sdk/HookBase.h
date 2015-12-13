#pragma once
class HookBase
{
public:
	HookBase() {};
	virtual ~HookBase() {};
	virtual bool init() = 0;
	virtual void shutdown() = 0;

	virtual void removeHooks() = 0;
};