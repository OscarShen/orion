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
	int parseInt(const std::string & s)
	{
		return atoi(s.c_str());
	}
	Float parseFloat(const std::string & s)
	{
		return static_cast<Float>(atof(s.c_str()));
	}
	Vector2i parseVector2i(const std::string & s)
	{
		auto v = split(s, ",");
		CHECK_INFO(v.size() >= 2, "Error, number of elements not enough to parse!");
		return Vector2i(
			atoi(v[0].c_str()),
			atoi(v[1].c_str())
		);
	}

	Vector3f parseVector3f(const std::string & s)
	{
		auto v = split(s, ",");
		CHECK_INFO(v.size() >= 3, "Error, number of elements not enough to parse!");
		return Vector3f(
			static_cast<Float>(atof(v[0].c_str())),
			static_cast<Float>(atof(v[1].c_str())),
			static_cast<Float>(atof(v[2].c_str())));
	}

	Vector3f parseRotate(const std::string & s, Float &degree)
	{
		auto v = split(s, ",");
		CHECK_INFO(v.size() > 3, "Error, number of elements not enough to parse!");
		degree = static_cast<Float>(atof(v[3].c_str()));
		return Vector3f(
			static_cast<Float>(atof(v[0].c_str())),
			static_cast<Float>(atof(v[1].c_str())),
			static_cast<Float>(atof(v[2].c_str())));
	}

	Point3f parsePoint3f(const std::string & s)
	{
		auto v = split(s, ",");
		CHECK_INFO(v.size() >= 3, "Error, number of elements not enough to parse!");
		return Point3f(
			static_cast<Float>(atof(v[0].c_str())),
			static_cast<Float>(atof(v[1].c_str())),
			static_cast<Float>(atof(v[2].c_str())));
	}
	Spectrum parseSpectrum(const std::string & s)
	{
		auto v = split(s, ",");
		if (v.size() < 3)
			return Spectrum(static_cast<Float>(atof(v[0].c_str())));
		return Spectrum(
			static_cast<Float>(atof(v[0].c_str())),
			static_cast<Float>(atof(v[1].c_str())),
			static_cast<Float>(atof(v[2].c_str())));
	}
	Transform parseLookAt(const std::string & s)
	{
		auto v = split(s, ",");
		if (v.size() < 9)
			CHECK_INFO(false, "do not have enough parameters for lookat!");

		Point3f origin(static_cast<Float>(atof(v[0].c_str())),
					   static_cast<Float>(atof(v[1].c_str())),
					   static_cast<Float>(atof(v[2].c_str())));
		Point3f lookat(static_cast<Float>(atof(v[3].c_str())),
					   static_cast<Float>(atof(v[4].c_str())),
					   static_cast<Float>(atof(v[5].c_str())));
		Vector3f up(   static_cast<Float>(atof(v[6].c_str())),
					   static_cast<Float>(atof(v[7].c_str())),
					   static_cast<Float>(atof(v[8].c_str())));
		return lookAt(origin, lookat, up);
	}
}

