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
ORION_NAMESPACE_END
