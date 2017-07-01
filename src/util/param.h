/************************************************************************ 
 * @description :  Param
 * @author		:  Oscar Shen
 * @creat 		:  2017-6-29 15:19:16
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_UTIL_PARAM_H_
#define ORION_UTIL_PARAM_H_

#include <orion.h>

ORION_NAMESPACE_BEGIN

class ParamVec
{
private:
	std::vector<std::pair<std::string, std::string>> pairs;
	mutable size_t pos;

public:
	void addParam(const std::string &key, const std::string &value) {
		pairs.push_back(std::make_pair(key, value));
	}
	const std::pair<std::string, std::string> *getPair() const {
		if (pos < pairs.size())
			return &pairs[pos++];
		else
			return nullptr;
	}
	void reset() const { pos = 0; }
};

class ParamSet
{
private:
	std::map<std::string, std::string> pairs;

public:
	void setParam(const std::string &key, const std::string &value) {
		CHECK_INFO(pairs.find(key) == pairs.end(), "Already has same param in _ParamSet_ !");
		pairs.insert(std::make_pair(key, value));
	}
	std::string getParam(const std::string &key) const {
		auto it = pairs.find(key);
		if (it != pairs.end())
			return it->second;
		CHECK_INFO(false, "param not found, param key is : " + key);
		return std::string();
	}
	bool hasParam(const std::string &key) const {
		return pairs.find(key) != pairs.end();
	}
};

ORION_NAMESPACE_END

#endif // !ORION_UTIL_PARAM_H_
