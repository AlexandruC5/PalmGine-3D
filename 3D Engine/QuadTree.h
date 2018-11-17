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

//Quadtree CollectIntersections using map
template<typename TYPE>
void Quadtree::CollectIntersections(std::map<float, GameObject*>& objects, const TYPE & primitive)const 
{
	if (root != nullptr) {
		root->CollectIntersections(objects, primitive);
	}
}

//Quadtree CollectIntersections using vector
template<typename TYPE>
void Quadtree::CollectIntersections(std::vector<GameObject*>& objects, const TYPE & primitive)const 
{
	if (root != nullptr) {
		root->CollectIntersections(objects, primitive);
	}
}

//QuadtreeNode CollectIntersections using map
template<typename TYPE>
void QuadtreeNode::CollectIntersections(std::map<float, GameObject*>& objects, const TYPE& primitive) const 
{
	if (primitive.Intersects(box) == true) {
		float hitNear;
		float hitFar;
		for (std::list<GameObject*>::const_iterator iterator = this->objectsInNode.begin(); iterator != this->objectsInNode.end(); ++iterator) {
			if (primitive.Intersects((*iterator)->GetAABB(), hitNear, hitFar)) {
				objects[hitNear] = *iterator;
			}
		}
		for (int i = 0; i < 4; ++i) {
			if (childs[i] != nullptr) {
				childs[i]->CollectIntersections(objects, primitive);
			}
		}
	}
}

//QuadtreeNode CollectIntersections using vector
template<typename TYPE>
void QuadtreeNode::CollectIntersections(std::vector<GameObject*>& objects, const TYPE& primitive) const 
{
	if (primitive.Intersects(box) == true) {
		for (std::list<GameObject*>::const_iterator iterator = this->objectsInNode.begin(); iterator != this->objectsInNode.end(); ++iterator) {
			if (primitive.Intersects((*iterator)->GetAABB())) {
				objects.push_back(*iterator);
			}
		}
		for (int i = 0; i < 4; ++i) {
			if (childs[i] != nullptr) {
				childs[i]->CollectIntersections(objects, primitive);
			}
		}
	}
}

#endif // !_QUADTREE_H_