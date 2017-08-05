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

Spectrum TabulatedBSSRDF::Rd(Float d) const
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

ORION_NAMESPACE_END
