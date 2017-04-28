#include "strutil.h"

namespace orion {
	void trim(std::string & s)
	{
		if (s.empty())
			return ;
		s.erase(0, s.find_first_not_of(' '));
		s.erase(s.find_last_not_of(' ') + 1);
	}
	std::vector<std::string> split(const std::string & s, const std::string & delim)
	{
		size_t last = 0;
		size_t index = s.find_first_of(delim, last);
		std::vector<std::string> res;
		while (index != std::string::npos) {
			res.push_back(s.substr(last, index - last));
			last = index + 1;
			index = s.find_first_of(delim, last);
		}
		if (index - last > 0) {
			res.push_back(s.substr(last, index - last));
		}
		return res;
	}
	Vector3f parseVector3f(std::string & s)
	{
		auto v = split(s, ",");
		return Vector3f(
			atof(v[0].c_str()),
			atof(v[1].c_str()),
			atof(v[2].c_str()));
	}
	Transform parseTransform(const std::string & s)
	{
		if (s.empty()) {
			ERROR("No value set in transform!");
			return Transform();
		}
		auto strs = split(s, ",");
		trim(strs[0]);
		if (strs[0] == "scale") {
			Float a = atof(strs[1].c_str()), b = atof(strs[2].c_str()), c = atof(strs[3].c_str());
			if (a != 1 || b!= 1 || c != 1)
				return scale(a, b, c);
		}
		else if (strs[0] == "rotate") {
			Float a = atof(strs[1].c_str()), b = atof(strs[2].c_str()), c = atof(strs[3].c_str());
			if (a != 0 || b != 0 || c != 0)
				return rotate(a, b, c);
		}
		else if (strs[1] == "translate") {
			Float a = atof(strs[1].c_str()), b = atof(strs[2].c_str()), c = atof(strs[3].c_str());
			if (a != 0 || b != 0 || c != 0)
				return translate(Vector3f(a, b, c));
		}


		return Transform();

	}
	std::string nextToken(std::string & str, char t)
	{
		int offset = str.find_first_not_of(' ');
		auto id = str.find_first_of(t, offset);
		return std::string();
	}

}

