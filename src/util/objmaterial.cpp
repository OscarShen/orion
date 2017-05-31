#ifndef TINY_OBJ_LOADER_H_
#define TINY_OBJ_LOADER_H_

#include <orion.h>
#include <parser.h>

namespace orion {

	typedef enum
	{
		triangulation = 1,        // used whether triangulate polygon face in .obj
		calculate_normals = 2,    // used whether calculate the normals if the .obj normals are empty
								  // Some nice stuff here
	} load_flags_t;

	class float3
	{
	public:
		float3()
			: x(0.0f)
			, y(0.0f)
			, z(0.0f)
		{
		}

		float3(float coord_x, float coord_y, float coord_z)
			: x(coord_x)
			, y(coord_y)
			, z(coord_z)
		{
		}

		float3(const float3& from, const float3& to)
		{
			coord[0] = to.coord[0] - from.coord[0];
			coord[1] = to.coord[1] - from.coord[1];
			coord[2] = to.coord[2] - from.coord[2];
		}

		float3 crossproduct(const float3 & vec)
		{
			float a = y * vec.z - z * vec.y;
			float b = z * vec.x - x * vec.z;
			float c = x * vec.y - y * vec.x;
			return float3(a, b, c);
		}

		void normalize()
		{
			const float length = std::sqrt((coord[0] * coord[0]) +
				(coord[1] * coord[1]) +
				(coord[2] * coord[2]));
			if (length != 1)
			{
				coord[0] = (coord[0] / length);
				coord[1] = (coord[1] / length);
				coord[2] = (coord[2] / length);
			}
		}

	private:
		union
		{
			float coord[3];
			struct
			{
				float x, y, z;
			};
		};
	};
}

#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cctype>

#include <fstream>
#include <sstream>

namespace orion {

#define TINYOBJ_SSCANF_BUFFER_SIZE (4096)

	struct vertex_index {
		int v_idx, vt_idx, vn_idx;
		vertex_index() : v_idx(-1), vt_idx(-1), vn_idx(-1) {}
		explicit vertex_index(int idx) : v_idx(idx), vt_idx(idx), vn_idx(idx) {}
		vertex_index(int vidx, int vtidx, int vnidx)
			: v_idx(vidx), vt_idx(vtidx), vn_idx(vnidx) {}
	};

	struct tag_sizes {
		tag_sizes() : num_ints(0), num_floats(0), num_strings(0) {}
		int num_ints;
		int num_floats;
		int num_strings;
	};

	// for std::map
	static inline bool operator<(const vertex_index &a, const vertex_index &b) {
		if (a.v_idx != b.v_idx)
			return (a.v_idx < b.v_idx);
		if (a.vn_idx != b.vn_idx)
			return (a.vn_idx < b.vn_idx);
		if (a.vt_idx != b.vt_idx)
			return (a.vt_idx < b.vt_idx);

		return false;
	}

	struct obj_shape {
		std::vector<float> v;
		std::vector<float> vn;
		std::vector<float> vt;
	};

	//See http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
	std::istream& safeGetline(std::istream& is, std::string& t)
	{
		t.clear();

		// The characters in the stream are read one-by-one using a std::streambuf.
		// That is faster than reading them one-by-one using the std::istream.
		// Code that uses streambuf this way must be guarded by a sentry object.
		// The sentry object performs various tasks,
		// such as thread synchronization and updating the stream state.

		std::istream::sentry se(is, true);
		std::streambuf* sb = is.rdbuf();

		for (;;) {
			int c = sb->sbumpc();
			switch (c) {
			case '\n':
				return is;
			case '\r':
				if (sb->sgetc() == '\n')
					sb->sbumpc();
				return is;
			case EOF:
				// Also handle the case when the last line has no line ending
				if (t.empty())
					is.setstate(std::ios::eofbit);
				return is;
			default:
				t += (char)c;
			}
		}
	}

#define IS_SPACE( x ) ( ( (x) == ' ') || ( (x) == '\t') )
#define IS_DIGIT( x ) ( (unsigned int)( (x) - '0' ) < (unsigned int)10 )
#define IS_NEW_LINE( x ) ( ( (x) == '\r') || ( (x) == '\n') || ( (x) == '\0') )

	// Make index zero-base, and also support relative index.
	static inline int fixIndex(int idx, int n) {
		if (idx > 0)
			return idx - 1;
		if (idx == 0)
			return 0;
		return n + idx; // negative value = relative
	}

	static inline std::string parseString(const char *&token) {
		std::string s;
		token += strspn(token, " \t");
		size_t e = strcspn(token, " \t\r");
		s = std::string(token, &token[e]);
		token += e;
		return s;
	}

	static inline int parseInt(const char *&token) {
		token += strspn(token, " \t");
		int i = atoi(token);
		token += strcspn(token, " \t\r");
		return i;
	}

	// Tries to parse a floating point number located at s.
	//
	// s_end should be a location in the string where reading should absolutely
	// stop. For example at the end of the string, to prevent buffer overflows.
	//
	// Parses the following EBNF grammar:
	//   sign    = "+" | "-" ;
	//   END     = ? anything not in digit ?
	//   digit   = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
	//   integer = [sign] , digit , {digit} ;
	//   decimal = integer , ["." , integer] ;
	//   float   = ( decimal , END ) | ( decimal , ("E" | "e") , integer , END ) ;
	//
	//  Valid strings are for example:
	//   -0	 +3.1417e+2  -0.0E-3  1.0324  -1.41   11e2
	//
	// If the parsing is a success, result is set to the parsed value and true
	// is returned.
	//
	// The function is greedy and will parse until any of the following happens:
	//  - a non-conforming character is encountered.
	//  - s_end is reached.
	//
	// The following situations triggers a failure:
	//  - s >= s_end.
	//  - parse failure.
	//
	static bool tryParseDouble(const char *s, const char *s_end, double *result) {
		if (s >= s_end) {
			return false;
		}

		double mantissa = 0.0;
		// This exponent is base 2 rather than 10.
		// However the exponent we parse is supposed to be one of ten,
		// thus we must take care to convert the exponent/and or the
		// mantissa to a * 2^E, where a is the mantissa and E is the
		// exponent.
		// To get the final double we will use ldexp, it requires the
		// exponent to be in base 2.
		int exponent = 0;

		// NOTE: THESE MUST BE DECLARED HERE SINCE WE ARE NOT ALLOWED
		// TO JUMP OVER DEFINITIONS.
		char sign = '+';
		char exp_sign = '+';
		char const *curr = s;

		// How many characters were read in a loop.
		int read = 0;
		// Tells whether a loop terminated due to reaching s_end.
		bool end_not_reached = false;

		/*
		BEGIN PARSING.
		*/

		// Find out what sign we've got.
		if (*curr == '+' || *curr == '-') {
			sign = *curr;
			curr++;
		}
		else if (IS_DIGIT(*curr)) { /* Pass through. */
		}
		else {
			goto fail;
		}

		// Read the integer part.
		while ((end_not_reached = (curr != s_end)) && IS_DIGIT(*curr)) {
			mantissa *= 10;
			mantissa += static_cast<int>(*curr - 0x30);
			curr++;
			read++;
		}

		// We must make sure we actually got something.
		if (read == 0)
			goto fail;
		// We allow numbers of form "#", "###" etc.
		if (!end_not_reached)
			goto assemble;

		// Read the decimal part.
		if (*curr == '.') {
			curr++;
			read = 1;
			while ((end_not_reached = (curr != s_end)) && IS_DIGIT(*curr)) {
				// NOTE: Don't use powf here, it will absolutely murder precision.
				mantissa += static_cast<int>(*curr - 0x30) * pow(10.0, -read);
				read++;
				curr++;
			}
		}
		else if (*curr == 'e' || *curr == 'E') {
		}
		else {
			goto assemble;
		}

		if (!end_not_reached)
			goto assemble;

		// Read the exponent part.
		if (*curr == 'e' || *curr == 'E') {
			curr++;
			// Figure out if a sign is present and if it is.
			if ((end_not_reached = (curr != s_end)) && (*curr == '+' || *curr == '-')) {
				exp_sign = *curr;
				curr++;
			}
			else if (IS_DIGIT(*curr)) { /* Pass through. */
			}
			else {
				// Empty E is not allowed.
				goto fail;
			}

			read = 0;
			while ((end_not_reached = (curr != s_end)) && IS_DIGIT(*curr)) {
				exponent *= 10;
				exponent += static_cast<int>(*curr - 0x30);
				curr++;
				read++;
			}
			exponent *= (exp_sign == '+' ? 1 : -1);
			if (read == 0)
				goto fail;
		}

	assemble:
		*result =
			(sign == '+' ? 1 : -1) * ldexp(mantissa * pow(5.0, exponent), exponent);
		return true;
	fail:
		return false;
	}
	static inline float parseFloat(const char *&token) {
		token += strspn(token, " \t");
#ifdef TINY_OBJ_LOADER_OLD_FLOAT_PARSER
		float f = (float)atof(token);
		token += strcspn(token, " \t\r");
#else
		const char *end = token + strcspn(token, " \t\r");
		double val = 0.0;
		tryParseDouble(token, end, &val);
		float f = static_cast<float>(val);
		token = end;
#endif
		return f;
	}

	static inline void parseFloat2(float &x, float &y, const char *&token) {
		x = parseFloat(token);
		y = parseFloat(token);
	}

	static inline void parseFloat3(float &x, float &y, float &z,
		const char *&token) {
		x = parseFloat(token);
		y = parseFloat(token);
		z = parseFloat(token);
	}

	static tag_sizes parseTagTriple(const char *&token) {
		tag_sizes ts;

		ts.num_ints = atoi(token);
		token += strcspn(token, "/ \t\r");
		if (token[0] != '/') {
			return ts;
		}
		token++;

		ts.num_floats = atoi(token);
		token += strcspn(token, "/ \t\r");
		if (token[0] != '/') {
			return ts;
		}
		token++;

		ts.num_strings = atoi(token);
		token += strcspn(token, "/ \t\r") + 1;

		return ts;
	}

	// Parse triples: i, i/j/k, i//k, i/j
	static vertex_index parseTriple(const char *&token, int vsize, int vnsize,
		int vtsize) {
		vertex_index vi(-1);

		vi.v_idx = fixIndex(atoi(token), vsize);
		token += strcspn(token, "/ \t\r");
		if (token[0] != '/') {
			return vi;
		}
		token++;

		// i//k
		if (token[0] == '/') {
			token++;
			vi.vn_idx = fixIndex(atoi(token), vnsize);
			token += strcspn(token, "/ \t\r");
			return vi;
		}

		// i/j/k or i/j
		vi.vt_idx = fixIndex(atoi(token), vtsize);
		token += strcspn(token, "/ \t\r");
		if (token[0] != '/') {
			return vi;
		}

		// i/j/k
		token++; // skip '/'
		vi.vn_idx = fixIndex(atoi(token), vnsize);
		token += strcspn(token, "/ \t\r");
		return vi;
	}

	static unsigned int
		updateVertex(std::map<vertex_index, unsigned int> &vertexCache,
			std::vector<float> &positions, std::vector<float> &normals,
			std::vector<float> &texcoords,
			const std::vector<float> &in_positions,
			const std::vector<float> &in_normals,
			const std::vector<float> &in_texcoords, const vertex_index &i) {
		const std::map<vertex_index, unsigned int>::iterator it = vertexCache.find(i);

		if (it != vertexCache.end()) {
			// found cache
			return it->second;
		}

		assert(in_positions.size() > static_cast<unsigned int>(3 * i.v_idx + 2));

		positions.push_back(in_positions[3 * static_cast<size_t>(i.v_idx) + 0]);
		positions.push_back(in_positions[3 * static_cast<size_t>(i.v_idx) + 1]);
		positions.push_back(in_positions[3 * static_cast<size_t>(i.v_idx) + 2]);

		if ((i.vn_idx >= 0) &&
			(static_cast<size_t>(i.vn_idx * 3 + 2) < in_normals.size())) {
			normals.push_back(in_normals[3 * static_cast<size_t>(i.vn_idx) + 0]);
			normals.push_back(in_normals[3 * static_cast<size_t>(i.vn_idx) + 1]);
			normals.push_back(in_normals[3 * static_cast<size_t>(i.vn_idx) + 2]);
		}

		if ((i.vt_idx >= 0) &&
			(static_cast<size_t>(i.vt_idx * 2 + 1) < in_texcoords.size())) {
			texcoords.push_back(in_texcoords[2 * static_cast<size_t>(i.vt_idx) + 0]);
			texcoords.push_back(in_texcoords[2 * static_cast<size_t>(i.vt_idx) + 1]);
		}

		unsigned int idx = static_cast<unsigned int>(positions.size() / 3 - 1);
		vertexCache[i] = idx;

		return idx;
	}

	static void InitMaterial(material_t &material) {
		material.name = "";
		material.ambient_texname = "";
		material.diffuse_texname = "";
		material.specular_texname = "";
		material.specular_highlight_texname = "";
		material.bump_texname = "";
		material.displacement_texname = "";
		material.alpha_texname = "";
		for (int i = 0; i < 3; i++) {
			material.ambient[i] = 0.f;
			material.diffuse[i] = 0.f;
			material.specular[i] = 0.f;
			material.transmittance[i] = 0.f;
			material.emission[i] = 0.f;
		}
		material.illum = 0;
		material.dissolve = 1.f;
		material.shininess = 1.f;
		material.ior = 1.f;
		material.unknown_parameter.clear();
	}

	void LoadMtl(std::vector<material_t> &materials, std::istream &inStream) {
		// Create a default material anyway.
		material_t material;
		InitMaterial(material);

		while (inStream.peek() != -1) {
			std::string linebuf;
			safeGetline(inStream, linebuf);

			// Trim newline '\r\n' or '\n'
			if (linebuf.size() > 0) {
				if (linebuf[linebuf.size() - 1] == '\n')
					linebuf.erase(linebuf.size() - 1);
			}
			if (linebuf.size() > 0) {
				if (linebuf[linebuf.size() - 1] == '\r')
					linebuf.erase(linebuf.size() - 1);
			}

			// Skip if empty line.
			if (linebuf.empty()) {
				continue;
			}

			// Skip leading space.
			const char *token = linebuf.c_str();
			token += strspn(token, " \t");

			assert(token);
			if (token[0] == '\0')
				continue; // empty line

			if (token[0] == '#')
				continue; // comment line

						  // new mtl
			if ((0 == strncmp(token, "newmtl", 6)) && IS_SPACE((token[6]))) {
				// flush previous material.
				if (!material.name.empty()) {
					materials.push_back(material);
				}

				// initial temporary material
				InitMaterial(material);

				// set new mtl name
				char namebuf[TINYOBJ_SSCANF_BUFFER_SIZE];
				token += 7;
#ifdef _MSC_VER
				sscanf_s(token, "%s", namebuf, (unsigned)_countof(namebuf));
#else
				sscanf(token, "%s", namebuf);
#endif
				material.name = namebuf;
				continue;
			}

			// ambient
			if (token[0] == 'K' && token[1] == 'a' && IS_SPACE((token[2]))) {
				token += 2;
				float r, g, b;
				parseFloat3(r, g, b, token);
				material.ambient[0] = r;
				material.ambient[1] = g;
				material.ambient[2] = b;
				continue;
			}

			// diffuse
			if (token[0] == 'K' && token[1] == 'd' && IS_SPACE((token[2]))) {
				token += 2;
				float r, g, b;
				parseFloat3(r, g, b, token);
				material.diffuse[0] = r;
				material.diffuse[1] = g;
				material.diffuse[2] = b;
				continue;
			}

			// specular
			if (token[0] == 'K' && token[1] == 's' && IS_SPACE((token[2]))) {
				token += 2;
				float r, g, b;
				parseFloat3(r, g, b, token);
				material.specular[0] = r;
				material.specular[1] = g;
				material.specular[2] = b;
				continue;
			}

			// transmittance
			if (token[0] == 'K' && token[1] == 't' && IS_SPACE((token[2]))) {
				token += 2;
				float r, g, b;
				parseFloat3(r, g, b, token);
				material.transmittance[0] = r;
				material.transmittance[1] = g;
				material.transmittance[2] = b;
				continue;
			}

			// ior(index of refraction)
			if (token[0] == 'N' && token[1] == 'i' && IS_SPACE((token[2]))) {
				token += 2;
				material.ior = parseFloat(token);
				continue;
			}

			// emission
			if (token[0] == 'K' && token[1] == 'e' && IS_SPACE(token[2])) {
				token += 2;
				float r, g, b;
				parseFloat3(r, g, b, token);
				material.emission[0] = r;
				material.emission[1] = g;
				material.emission[2] = b;
				continue;
			}

			// shininess
			if (token[0] == 'N' && token[1] == 's' && IS_SPACE(token[2])) {
				token += 2;
				material.shininess = parseFloat(token);
				continue;
			}

			// illum model
			if (0 == strncmp(token, "illum", 5) && IS_SPACE(token[5])) {
				token += 6;
				material.illum = parseInt(token);
				continue;
			}

			// dissolve
			if ((token[0] == 'd' && IS_SPACE(token[1]))) {
				token += 1;
				material.dissolve = parseFloat(token);
				continue;
			}
			if (token[0] == 'T' && token[1] == 'r' && IS_SPACE(token[2])) {
				token += 2;
				// Invert value of Tr(assume Tr is in range [0, 1])
				material.dissolve = 1.0f - parseFloat(token);
				continue;
			}

			// ambient texture
			if ((0 == strncmp(token, "map_Ka", 6)) && IS_SPACE(token[6])) {
				token += 7;
				material.ambient_texname = token;
				continue;
			}

			// diffuse texture
			if ((0 == strncmp(token, "map_Kd", 6)) && IS_SPACE(token[6])) {
				token += 7;
				material.diffuse_texname = token;
				continue;
			}

			// specular texture
			if ((0 == strncmp(token, "map_Ks", 6)) && IS_SPACE(token[6])) {
				token += 7;
				material.specular_texname = token;
				continue;
			}

			// specular highlight texture
			if ((0 == strncmp(token, "map_Ns", 6)) && IS_SPACE(token[6])) {
				token += 7;
				material.specular_highlight_texname = token;
				continue;
			}

			// bump texture
			if ((0 == strncmp(token, "map_bump", 8)) && IS_SPACE(token[8])) {
				token += 9;
				material.bump_texname = token;
				continue;
			}

			// alpha texture
			if ((0 == strncmp(token, "map_d", 5)) && IS_SPACE(token[5])) {
				token += 6;
				material.alpha_texname = token;
				continue;
			}

			// bump texture
			if ((0 == strncmp(token, "bump", 4)) && IS_SPACE(token[4])) {
				token += 5;
				material.bump_texname = token;
				continue;
			}

			// displacement texture
			if ((0 == strncmp(token, "disp", 4)) && IS_SPACE(token[4])) {
				token += 5;
				material.displacement_texname = token;
				continue;
			}

			// unknown parameter
			const char *_space = strchr(token, ' ');
			if (!_space) {
				_space = strchr(token, '\t');
			}
			if (_space) {
				std::ptrdiff_t len = _space - token;
				std::string key(token, static_cast<size_t>(len));
				std::string value = _space + 1;
				material.unknown_parameter.insert(
					std::pair<std::string, std::string>(key, value));
			}
		}
		// flush last material.
		materials.push_back(material);
	}
} // namespace
#endif
