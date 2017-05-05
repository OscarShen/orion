/************************************************************************ 
 * @description :  cache transform to save memory for each primitive's transform
 * @author		:  Oscar Shen
 * @creat 		:  2017Äê4ÔÂ28ÈÕ
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_UTIL_TRANSFORM_CACHE_H_
#define ORION_UTIL_TRANSFORM_CACHE_H_
#include <orion.h>
#include <core/transform.h>
#include <common/singleton.h>
namespace orion {

	class TransformCache : public Singleton<TransformCache>
	{
	private:
		std::map<Transform, std::pair<Transform *, Transform *>> cache;

	public:
		static void init() { new TransformCache(); }

		void lookup(const Transform &t, Transform **tCached, Transform **tCachedInv) {
			auto it = cache.find(t);
			if (it == cache.end()) { // not find
				Transform *transform = new Transform(); // TODO: use transform pool in place
				*transform = t;
				Transform *transformInv = new Transform(); // TODO: use transform pool in place
				*transformInv = Transform(inverse(t));
				cache[t] = std::make_pair(transform, transformInv);
				it = cache.find(t);
			}
			if (tCached) *tCached = it->second.first;
			if (tCachedInv) *tCachedInv = it->second.second;
		}
		void clear() {
			cache.erase(cache.begin(), cache.end());
			// TODO:delete transform pool
		}

	private:
		TransformCache() {}
	};
}

#endif // !ORION_UTIL_TRANSFORM_CACHE_H_
