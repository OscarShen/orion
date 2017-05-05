#include "merl.h"
#include <common/paramset.h>
#include <util/envvariable.h>
#include <math/linalg.h>
#include <core/transform.h>
namespace orion {

	// Lookup theta_half index
	// This is a non-linear mapping!
	// In:  [0 .. pi/2]
	// Out: [0 .. 89]
	inline int theta_half_index(Float theta_half)
	{
		if (theta_half <= 0.0)
			return 0;
		Float theta_half_deg = theta_half * 2 * invpi * MERL_SAMPLING_RES_THETA_H;
		Float temp = theta_half_deg * MERL_SAMPLING_RES_THETA_H;
		temp = std::sqrt(temp);
		int ret_val = (int)temp;
		if (ret_val < 0) ret_val = 0;
		if (ret_val >= MERL_SAMPLING_RES_THETA_H)
			ret_val = MERL_SAMPLING_RES_THETA_H - 1;
		return ret_val;
	}

	// Lookup theta_diff index
	// In:  [0 .. pi/2]
	// Out: [0 .. 89]
	inline int theta_diff_index(Float theta_diff)
	{
		int tmp = int(theta_diff * 2 * invpi * MERL_SAMPLING_RES_THETA_D);
		if (tmp < 0)
			return 0;
		else if (tmp < MERL_SAMPLING_RES_THETA_D - 1)
			return tmp;
		else
			return MERL_SAMPLING_RES_THETA_D - 1;
	}

	// Lookup phi_diff index
	inline int phi_diff_index(Float phi_diff)
	{
		// Because of reciprocity, the BRDF is unchanged under
		// phi_diff -> phi_diff + M_PI
		if (phi_diff < 0.0)
			phi_diff += pi;

		// In: phi_diff in [0 .. pi]
		// Out: tmp in [0 .. 179]
		int tmp = int(phi_diff * invpi * MERL_SAMPLING_RES_PHI_D);
		if (tmp < 0)
			return 0;
		else if (tmp < MERL_SAMPLING_RES_PHI_D - 1)
			return tmp;
		else
			return MERL_SAMPLING_RES_PHI_D - 1;
	}

	std::map<std::string, std::shared_ptr<Merl>> Merl::loadedMerl;

	std::shared_ptr<Merl> loadMerl(const std::string & filename)
	{
		std::string str = getResPath() + filename;

		std::ifstream ifs(str.c_str(), std::ios::binary);
		if (!ifs.is_open()) {
			CHECK_INFO(false, "Failed to open file : " + str);
			return nullptr;
		}
		int dims[3];
		ifs.read((char*)dims, sizeof(int) * 3);

		if (dims[0] != MERL_SAMPLING_RES_THETA_H ||
			dims[1] != MERL_SAMPLING_RES_THETA_D ||
			dims[2] != MERL_SAMPLING_RES_PHI_D) {
			ifs.close();
			CHECK_INFO(false, "load failed, parameters does not match!");
			return nullptr;
		}

		int trunksize = dims[0] * dims[1] * dims[2];
		int size = 3 * trunksize;

		Merl *merl = new Merl();
		auto &v = merl->data;
		v.resize(size);
		double *tmp = new double[size];
		ifs.read((char*)tmp, sizeof(double) * size);
		int offset = 0;
		for (int i = 0; i < trunksize; ++i, ++offset)
			v[offset] = (Float)(tmp[offset] * MERL_RED_SCALE);
		for (int i = 0; i < trunksize; ++i, ++offset)
			v[offset] = (Float)(tmp[offset] * MERL_GREEN_SCALE);
		for (int i = 0; i < trunksize; ++i, ++offset)
			v[offset] = (Float)(tmp[offset] * MERL_BLUE_SCALE);
		delete[] tmp;
		ifs.close();
		return std::shared_ptr<Merl>(merl);
	}
	std::shared_ptr<Merl> createMerl(const ParamSet & param)
	{
		std::string filename = param.getParam("filename");
		auto &loaded = Merl::loadedMerl;
		auto it = loaded.find(filename);
		if (it != loaded.end()) {
			return it->second;
		}
		auto merl = loadMerl(filename);
		Merl::loadedMerl.insert(std::make_pair(filename, merl));
		return merl;
	}
	Spectrum Merl::f(const Vector3f & Wi, const Vector3f & Wo) const
	{
		Vector3f wo = Wo, wi = Wi;
		if (wo.y < 0)
			wo = -wo;
		if (wi.y < 0)
			wi = -wi;

		Float theta_in = sphericalTheta(wi), theta_out = sphericalTheta(wo);
		Float phi_in = sphericalPhi(wi), phi_out = sphericalPhi(wo);
		Float theta_half, phi_half, theta_diff, phi_diff;

		// NOTE£¡£¡¡¡y is up, x right, z back, right hand coordination, phi from x axis right hand rule of y axis (0 - 2pi),
		// theta from y axis to plane xz (0 - pi/2)
		// compute in vector
		Float in_vec_y = cos(theta_in);
		Float proj_in_vec = sin(theta_in);
		Float in_vec_x = proj_in_vec * cos(phi_in);
		Float in_vec_z = -proj_in_vec * sin(phi_in);
		Vector3f in = normalize(Vector3f(in_vec_x, in_vec_y, in_vec_z));

		// compute out vector
		Float out_vec_y = cos(theta_out);
		Float proj_out_vec = sin(theta_out);
		Float out_vec_x = proj_out_vec * cos(phi_out);
		Float out_vec_z = -proj_out_vec * sin(phi_out);
		Vector3f out = normalize(Vector3f(out_vec_x, out_vec_y, out_vec_z));

		// compute halfway vector
		Vector3f half = normalize((in + out) * 0.5f);

		// compute  theta_half, fi_half
		theta_half = sphericalTheta(half);
		phi_half = sphericalPhi(half);

		// compute diff vector
		// note the order of rotate, because of Gimble lock
		Vector3f diff = rotate(degrees(-phi_half), Vector3f(0.0f, 1.0f, 0.0f))(in);
		diff = rotate(degrees(-theta_half), Vector3f(0.0f, 0.0f, -1.0f))(diff);
		diff = normalize(diff);
		theta_diff = sphericalTheta(diff);
		// here should not use _spericalTheta_, because MERL binary file need 0 to pi
		phi_diff = atan2(-diff[2], diff[0]);

		// Find index.
		// Note that phi_half is ignored, since isotropic BRDFs are assumed
		int ind = phi_diff_index(phi_diff) +
			theta_diff_index(theta_diff) * MERL_SAMPLING_RES_PHI_D +
			theta_half_index(theta_half) * MERL_SAMPLING_RES_PHI_D *
			MERL_SAMPLING_RES_THETA_D;

		Float red = data[ind];
		Float green = data[ind + MERL_SAMPLING_COUNT];
		Float blue = data[ind + MERL_SAMPLING_COUNT * 2];

		CHECK_INFO(red >= 0.0f && green >= 0.0f && blue >= 0.0f, "Below horizon.");
		return Spectrum((Float)red, (Float)green, (Float)blue);
	}
	Spectrum Merl::sample_f(Vector3f & wi, const Vector3f & wo, Float * pdf) const
	{
		return Spectrum();
	}
	std::shared_ptr<Merl> Merl::clone() const
	{
		auto m = new Merl();
		m->data = data;
		return std::shared_ptr<Merl>(m);
	}
}