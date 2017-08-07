#include "interpolation.h"

ORION_NAMESPACE_BEGIN
bool catmullRomWeights(int size, const Float * nodes, Float x, int * offset, Float * weights)
{
	// Return _false_ if _x_ is out of bounds
	if (!(x >= nodes[0] && x <= nodes[size - 1])) return false;

	// Search for the interval _idx_ containing _x_
	int idx = findInterval(size, [&](int i) { return nodes[i] <= x; });
	*offset = idx - 1;
	Float x0 = nodes[idx], x1 = nodes[idx + 1];

	// Compute the $t$ parameter and powers
	Float t = (x - x0) / (x1 - x0), t2 = t * t, t3 = t2 * t;

	// Compute initial node weights $w_1$ and $w_2$
	weights[1] = 2 * t3 - 3 * t2 + 1;
	weights[2] = -2 * t3 + 3 * t2;

	// Compute first node weight $w_0$
	if (idx > 0) {
		Float w0 = (t3 - 2 * t2 + t) * (x1 - x0) / (x1 - nodes[idx - 1]);
		weights[0] = -w0;
		weights[2] += w0;
	}
	else {
		Float w0 = t3 - 2 * t2 + t;
		weights[0] = 0;
		weights[1] -= w0;
		weights[2] += w0;
	}

	// Compute last node weight $w_3$
	if (idx + 2 < size) {
		Float w3 = (t3 - t2) * (x1 - x0) / (nodes[idx + 2] - x0);
		weights[1] -= w3;
		weights[3] = w3;
	}
	else {
		Float w3 = t3 - t2;
		weights[1] -= w3;
		weights[2] += w3;
		weights[3] = 0;
	}
	return true;
}

Float sampleCatmullRom2D(int size1, int size2, const Float *nodes1,
	const Float *nodes2, const Float *values,
	const Float *cdf, Float alpha, Float u, Float *fval,
	Float *pdf) {
	// Determine offset and coefficients for the _alpha_ parameter
	int offset;
	Float weights[4];
	if (!catmullRomWeights(size1, nodes1, alpha, &offset, weights)) return 0;

	// Define a lambda function to interpolate table entries
	auto interpolate = [&](const Float *array, int idx) {
		Float value = 0;
		for (int i = 0; i < 4; ++i)
			if (weights[i] != 0)
				value += array[(offset + i) * size2 + idx] * weights[i];
		return value;
	};

	// Map _u_ to a spline interval by inverting the interpolated _cdf_
	Float maximum = interpolate(cdf, size2 - 1);
	u *= maximum;
	int idx =
		findInterval(size2, [&](int i) { return interpolate(cdf, i) <= u; });

	// Look up node positions and interpolated function values
	Float f0 = interpolate(values, idx), f1 = interpolate(values, idx + 1);
	Float x0 = nodes2[idx], x1 = nodes2[idx + 1];
	Float width = x1 - x0;
	Float d0, d1;

	// Re-scale _u_ using the interpolated _cdf_
	u = (u - interpolate(cdf, idx)) / width;

	// Approximate derivatives using finite differences of the interpolant
	if (idx > 0)
		d0 = width * (f1 - interpolate(values, idx - 1)) /
		(x1 - nodes2[idx - 1]);
	else
		d0 = f1 - f0;
	if (idx + 2 < size2)
		d1 = width * (interpolate(values, idx + 2) - f0) /
		(nodes2[idx + 2] - x0);
	else
		d1 = f1 - f0;

	// Invert definite integral over spline segment and return solution

	// Set initial guess for $t$ by importance sampling a linear interpolant
	Float t;
	if (f0 != f1)
		t = (f0 - std::sqrt(std::max((Float)0, f0 * f0 + 2 * u * (f1 - f0)))) /
		(f0 - f1);
	else
		t = u / f0;
	Float a = 0, b = 1, Fhat, fhat;
	while (true) {
		// Fall back to a bisection step when _t_ is out of bounds
		if (!(t >= a && t <= b)) t = 0.5f * (a + b);

		// Evaluate target function and its derivative in Horner form
		Fhat = t * (f0 +
			t * (.5f * d0 +
				t * ((1.f / 3.f) * (-2 * d0 - d1) + f1 - f0 +
					t * (.25f * (d0 + d1) + .5f * (f0 - f1)))));
		fhat = f0 +
			t * (d0 +
				t * (-2 * d0 - d1 + 3 * (f1 - f0) +
					t * (d0 + d1 + 2 * (f0 - f1))));

		// Stop the iteration if converged
		if (std::abs(Fhat - u) < 1e-6f || b - a < 1e-6f) break;

		// Update bisection bounds using updated _t_
		if (Fhat - u < 0)
			a = t;
		else
			b = t;

		// Perform a Newton step
		t -= (Fhat - u) / fhat;
	}

	// Return the sample position and function value
	if (fval) *fval = fhat;
	if (pdf) *pdf = fhat / maximum;
	return x0 + width * t;
}
ORION_NAMESPACE_END
