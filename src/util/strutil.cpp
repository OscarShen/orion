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
	std::shared_ptr<Mesh> parseMeshData(const std::string & pStr, const std::string & uvStr, const std::string & nStr)
	{
		auto pStrs = split(pStr, ",");
		CHECK_INFO(pStrs.size() % 3 == 0, "number of figures can't make complete point!");
		int verticesNum = (int)pStrs.size() / 3;
		int triNum = verticesNum / 3;

		auto mesh = std::make_shared<Mesh>();

		// p
		std::vector<Point3f> p;
		p.reserve(verticesNum);
		for (int i = 0; i < verticesNum; ++i) {
			p.push_back(Point3f(
				static_cast<Float>(atof(pStrs[3 * i].c_str())),
				static_cast<Float>(atof(pStrs[3 * i + 1].c_str())),
				static_cast<Float>(atof(pStrs[3 * i + 2].c_str()))));
		}

		// uv
		std::vector<Point2f> uv;
		uv.reserve(verticesNum);
		if (!uvStr.empty()) {
			auto uvStrs = split(uvStr, ",");
			for (int i = 0; i < verticesNum; ++i) {
				uv.push_back(Point2f(
					static_cast<Float>(atof(uvStrs[2 * i].c_str())),
					static_cast<Float>(atof(uvStrs[2 * i + 1].c_str()))));
			}
		}

		// n
		std::vector<Normal3f> n;
		if (!nStr.empty()) {
			auto nStrs = split(nStr, ",");
			if (nStrs.size() == 3) {
				Normal3f normal(
					static_cast<Float>(atof(nStrs[0].c_str())),
					static_cast<Float>(atof(nStrs[1].c_str())),
					static_cast<Float>(atof(nStrs[2].c_str())));
				n = std::vector<Normal3f>(verticesNum, normal);
			}
			else {
				for (int i = 0; i < verticesNum; ++i) {
					n.push_back(Normal3f(
						static_cast<Float>(atof(nStrs[3 * i].c_str())),
						static_cast<Float>(atof(nStrs[3 * i + 1].c_str())),
						static_cast<Float>(atof(nStrs[3 * i + 2].c_str()))));
				}
			}
		}
		
		mesh->p = std::move(p);
		mesh->n = std::move(n);
		mesh->uv = std::move(uv);
		mesh->numTri = triNum;
		mesh->numVer = verticesNum;
		return mesh;
	}
}

