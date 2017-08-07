#include "bssrdf.h"
#include <core/bsdf.h>
#include <sampler/sampling.h>
#include "interpolation.h"
ORION_NAMESPACE_BEGIN

Float FresnelMoment1(Float eta) {
	Float eta2 = eta * eta, eta3 = eta2 * eta, eta4 = eta3 * eta,
		eta5 = eta4 * eta;
	if (eta < 1)
		return 0.45966f - 1.73965f * eta + 3.37668f * eta2 - 3.904945 * eta3 +
		2.49277f * eta4 - 0.68441f * eta5;
	else
		return -4.61686f + 11.1136f * eta - 10.4646f * eta2 + 5.11455f * eta3 -
		1.27198f * eta4 + 0.12746f * eta5;
}

Float FresnelMoment2(Float eta) {
	Float eta2 = eta * eta, eta3 = eta2 * eta, eta4 = eta3 * eta,
		eta5 = eta4 * eta;
	if (eta < 1) {
		return 0.27614f - 0.87350f * eta + 1.12077f * eta2 - 0.65095f * eta3 +
			0.07883f * eta4 + 0.04860f * eta5;
	}
	else {
		Float r_eta = 1 / eta, r_eta2 = r_eta * r_eta, r_eta3 = r_eta2 * r_eta;
		return -547.033f + 45.3087f * r_eta3 - 218.725f * r_eta2 +
			458.843f * r_eta + 404.557f * eta - 189.519f * eta2 +
			54.9327f * eta3 - 9.00603f * eta4 + 0.63942f * eta5;
	}
}

BSSRDFTable::BSSRDFTable(int nRhoSamples, int nRadiusSamples)
	: nRhoSamples(nRhoSamples),
	nRadiusSamples(nRadiusSamples),
	rhoSamples(new Float[nRhoSamples]),
	radiusSamples(new Float[nRadiusSamples]),
	profile(new Float[nRadiusSamples * nRhoSamples]),
	rhoEff(new Float[nRhoSamples]),
	profileCDF(new Float[nRadiusSamples * nRhoSamples]) {}

Spectrum BSSRDF::sample_Rd(const Scene & scene, Float rand1, Point2f rand2, Intersection * pi, Float * pdf) const
{
	Vector3f vx, vy, vz;
	if (rand1 < .5f) {
		vx = u;
		vy = v;
		vz = Vector3f(ns);
		rand1 *= 2;
	}
	else if (rand1 < 0.75f) {
		vx = v;
		vy = Vector3f(ns);
		vz = u;
		rand1 = (rand1 - .5f) * 4;
	}
	else {
		vx = Vector3f(ns);
		vy = u;
		vz = v;
		rand1 = (rand1 - .75f) * 4;
	}

	int ch = clamp((int)(rand1 * 3), 0, 2);
	rand1 = rand1 * 3 - ch;

	Float r = sample_R(ch, rand2[0]);
	if (r < 0)
		return 0;
	Float phi = 2 * orion::pi * rand2[1];

	Float rMax = sample_R(ch, 0.999f);
	if (r >= rMax)
		return 0; // all energy has been exhausted
	Float l = 2 * std::sqrt(rMax * rMax - r * r);

	Intersection base;
	base.p = po.p + r * (vx * std::cos(phi) + vy * std::sin(phi)) - l * vz * 0.5f;
	Point3f pTarget = base.p + l * vz;

	struct IntersectionChain {
		Intersection isec;
		IntersectionChain *next = nullptr;
	};
	IntersectionChain *chain = new IntersectionChain();

	IntersectionChain *ptr = chain;
	int nFound = 0;
	while (scene.intersect(base.spawnRay(pTarget), &ptr->isec)) {
		base = ptr->isec;
		if (ptr->isec.primitive->getMaterial() == this->material) {
			IntersectionChain *next = new IntersectionChain();
			ptr->next = next;
			ptr = next;
			nFound++;
		}
	}

	if (nFound == 0)
		return 0;
	int selected = clamp((int)(rand1 * nFound), 0, nFound - 1);
	while (selected-- > 0)
		chain = chain->next;
	*pi = chain->isec;

	*pdf = pdf_Rd(*pi) / nFound;
	return Rd(*pi);
}

Float BSSRDF::pdf_Rd(const Intersection & pi) const
{
	Vector3f d = po.p - pi.p;
	Vector3f dLocal(dot(u, d), dot(v, d), dot(ns, d));
	Normal3f nLocal(dot(u, pi.ng), dot(v, pi.ng), dot(ns, pi.ng));

	Float rProj[3] = { std::sqrt(dLocal.y * dLocal.y + dLocal.z * dLocal.z),
		std::sqrt(dLocal.z * dLocal.z + dLocal.x * dLocal.x),
		std::sqrt(dLocal.x * dLocal.x + dLocal.y * dLocal.y) };

	Float pdf = 0, axisProb[3] = { .25f, .25f, .5f };
	Float chProb = 1 / (Float)3;
	for (int axis = 0; axis < 3; ++axis)
		for (int ch = 0; ch < 3; ++ch)
			pdf += pdf_R(ch, rProj[axis]) * std::abs(nLocal[axis]) * chProb *
			axisProb[axis];
	return pdf;
}

Spectrum TabulatedBSSRDF::R(Float d) const
{
	Spectrum R;
	for (int ch = 0; ch < 3; ++ch) {
		Float rOptical = d * sigma_t[ch];

		int rhoOffset, radiusOffset;
		Float rhoWeights[4], radiusWeights[4];
		if (!catmullRomWeights(table->nRhoSamples, table->rhoSamples.get(),
			rho[ch], &rhoOffset, rhoWeights) ||
			!catmullRomWeights(table->nRadiusSamples, table->radiusSamples.get(),
				rOptical, &radiusOffset, radiusWeights))
			continue;

		Float rr = 0;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				Float weight = rhoWeights[i] * radiusWeights[j];
				if (weight != 0)
					rr += weight *
					table->evalProfile(rhoOffset + i, radiusOffset + j);
			}
		}
		if (rOptical != 0) rr /= 2 * pi * rOptical;
		R[ch] = rr;
	}
	R *= sigma_t * sigma_t;
	return R.clamp();
}

Float TabulatedBSSRDF::sample_R(int ch, Float rand) const
{
	if (sigma_t[ch] == 0)
		return -1;
	return sampleCatmullRom2D(table->nRhoSamples, table->nRadiusSamples,
		table->rhoSamples.get(), table->radiusSamples.get(),
		table->profile.get(), table->profileCDF.get(),
		rho[ch], rand) / sigma_t[ch];
}

Float TabulatedBSSRDF::pdf_R(int ch, Float rand) const
{
	Float rOptical = rand * sigma_t[ch];

	int rhoOffset, radiusOffset;
	Float rhoWeights[4], radiusWeights[4];
	if (!catmullRomWeights(table->nRhoSamples, table->rhoSamples.get(), rho[ch],
			&rhoOffset, radiusWeights) ||
		!catmullRomWeights(table->nRadiusSamples, table->radiusSamples.get(), rOptical,
			&radiusOffset, radiusWeights))
		return 0;

	Float sr = 0, rhoEff = 0;
	for (int i = 0; i < 4; ++i) {
		if (rhoWeights[i] == 0) continue;
		rhoEff += table->rhoEff[rhoOffset + i] * rhoWeights[i];
		for (int j = 0; j < 4; ++j) {
			if (radiusWeights[j] == 0) continue;
			sr += table->evalProfile(rhoOffset + i, radiusOffset + j) *
				rhoWeights[i] * radiusWeights[j];
		}
	}

	if (rOptical != 0)
		sr /= 2 * orion::pi * rOptical;
	return std::max((Float)0, sr * sigma_t[ch] * sigma_t[ch]);
}

ORION_NAMESPACE_END