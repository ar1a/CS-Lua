#pragma once
#include <string>
//Files in here could be executed but we don't want the client to worry about these

std::string HOOK_CODE("local hooks = {}  \
hook = {}  \
 \
	function hook.Add(event, id, callback) \
	if (type(callback) ~=  \"function\") then \
		error( \"bad argument #3 to 'Add' (function expected, got \" ..type(callback) .. \")\", 2) \
		end \
 \
		hooks[event] = hooks[event] or {} \
hooks[event][id] = callback \
end \
 \
function hook.Remove(event, id) \
 \
if not hooks[event] then \
error(\"attempt to remove non-existent hook\", 2) \
end \
 \
if not hooks[event] then return end \
 \
hooks[event][id] = nil \
end \
 \
function hook.Call(event, ...) \
for k, v in pairs(hooks[event]) do \
local success, err = pcall(v, ...) \
 \
if not success then \
print(err) \
hooks[event][k] = nil \
end \
end \
end \
");
