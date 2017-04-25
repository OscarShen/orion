/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_KDTREE_H_
#define ORION_KDTREE_H_
#include <orion.h>
#include "accelerator.h"
namespace orion {

	struct KdAccelNode;
	struct BoundEdge;
	class KdTreeAccel : public Accelerator {
	public:
		// KdTreeAccel Public Methods
		KdTreeAccel(const std::vector<std::shared_ptr<Shape>> &p,
			int isectCost = 80, int traversalCost = 1,
			Float emptyBonus = 0.5, int maxPrims = 1, int maxDepth = -1);
		Bounds3f worldBound() const { return bounds; }
		~KdTreeAccel();
		bool intersect(const Ray &ray, Intersection *isect) const;

	private:
		// KdTreeAccel Private Methods
		void buildTree(int nodeNum, const Bounds3f &bounds,
			const std::vector<Bounds3f> &primBounds, int *primNums,
			int nprims, int depth,
			const std::unique_ptr<BoundEdge[]> edges[3], int *prims0,
			int *prims1, int badRefines = 0);

		// KdTreeAccel Private Data
		const int isectCost, traversalCost, maxPrims;
		const Float emptyBonus;
		std::vector<std::shared_ptr<Shape>> primitives;
		std::vector<int> primitiveIndices;
		KdAccelNode *nodes;
		int nAllocedNodes, nextFreeNode;
		Bounds3f bounds;
	};

	struct KdToDo {
		const KdAccelNode *node;
		Float tMin, tMax;
	};

	std::shared_ptr<KdTreeAccel> createKdTreeAccelerator(
		const std::vector<std::shared_ptr<Shape>> &shapes);
}

#endif // !ORION_KDTREE_H_
