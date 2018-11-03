#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "Math.h"
#include "MathGeoLib/MathGeoLib.h"
#include <list>
#include <map>

class GameObject;

class QuadtreeNode {
public:

	QuadtreeNode(const AABB& box);
	virtual ~QuadtreeNode();

	bool IsLeaf() const;

	void Insert(GameObject* go);
	void Remove(GameObject* go);
	void CreateChilds();
	void RedistributeChilds();

	void DebugDraw();

	//Intersections
	template<typename TYPE>
	void CollectIntersections(std::map<float, GameObject*>& objects, const TYPE& primitive) const;
	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& objects, const TYPE& primitive) const;

public:
	AABB box;
	std::list<GameObject*> objectsInNode;
	QuadtreeNode* parent;
	QuadtreeNode* childs[4];


};

class Quadtree {
public:

	Quadtree();
	virtual ~Quadtree();

	void SetBoundries(const AABB& box);

	void Insert(GameObject* go);
	void Remove(GameObject* go);
	void Clear();

	void DebugDraw();

	//Intersections
	template<typename TYPE>
	void CollectIntersections(std::map<float, GameObject*>& objects, const TYPE& primitive) const;
	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& objects, const TYPE& primitive) const;

public:
	QuadtreeNode* root = nullptr;

};

#endif // !_QUADTREE_H_