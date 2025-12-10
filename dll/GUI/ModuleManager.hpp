#pragma once
#include "util/Singleton.hpp"
#include "Module.hpp"
#include "CheatFunc.hpp"
#include <vector>
#include <map>

class KeyBind {
public:
	int key;
	bool * func;
	KeyBind(int key , bool * func) : key(key) , func(func) {}
	void process() const;
};

//一个作弊方法
class CheatFunction {
	bool* enable;
	std::string name;
	KeyBind * keybind;
public:

	~CheatFunction() {
		if (keybind)
			delete keybind;
	}

	CheatFunction(bool* enable , std::string name);
	bool isEnabled() const  {
		return *enable;
	}

	virtual const std::string & getName() const {
		return name;
	}

	KeyBind * getKeyBind() {
		return keybind;
	}

	void setKeyBind(int key) {
		keybind->key = key;
	}
};



class ModuleManager : public Singleton<ModuleManager> {
	friend class Singleton<ModuleManager>;
public:
	std::vector<Module*> modules; // virtual functions need pointers
	std::vector<CheatFunction*> cheatFunctions;
	std::map<std::string , CheatFunction *> mapping;
	ModuleManager();
	Module* getModuleByName(std::string name);
};