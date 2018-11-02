#include "GameObject.h"
#include "CompMesh.h"
#include "QuadTree.h"

#define NE 0 //NorthEast UpRight
#define SE 1 //SouthEast DownRight
#define SW 2 //SouthWest DownLeft
#define NW 3 //NorthWest UpLeft
/*
-----------
| NW | NE |
|---------|
| SW | SE |
-----------
*/

#define QUADTREE_MAX_ITEMS 8
#define QUADTREE_MIN_SIZE 10.0f 

//QuadtreeNode

QuadtreeNode::QuadtreeNode(const AABB& box) {
	parent = nullptr;
	childs[NE] = nullptr;
	childs[SE] = nullptr;
	childs[SW] = nullptr;
	childs[NW] = nullptr;
}

QuadtreeNode::~QuadtreeNode() {
	for (int i = 0; i < 4; ++i) {
		if (childs[i] != nullptr) {
			RELEASE(childs[i]);
		}
	}
}

bool QuadtreeNode::IsLeaf() const{
	bool ret = false;
	if (childs[NE] == nullptr) {
		ret = true;
	}
	return ret;
}

void QuadtreeNode::Insert(GameObject* go) {
	if (IsLeaf()==true && (objectsInNode.size() < QUADTREE_MAX_ITEMS || (box.HalfSize().LengthSq() <= QUADTREE_MIN_SIZE * QUADTREE_MIN_SIZE))) {
		objectsInNode.push_back(go);
	}
	else {
		if (IsLeaf() == true) {
			CreateChilds();
		}
		objectsInNode.push_back(go);
		RedistributeChilds();
	}
}

void QuadtreeNode::Remove(GameObject* go) {
	std::list<GameObject*>::iterator iterator = objectsInNode.begin();
	while (iterator != objectsInNode.end()) {
		if ((*iterator) != go) {
			++iterator;
		}
		objectsInNode.erase(iterator);
	}

	if (IsLeaf() == false) {
		for (int i = 0; i < 4; ++i) {
			childs[i]->Remove(go);
		}
	}

}

//Subdivides actual node
void QuadtreeNode::CreateChilds() {
	float3 newSize(box.Size().x/2, box.Size().y, box.Size().z/2);
	AABB newBox;

	//NE
	float3 NECenter(box.CenterPoint().x + box.Size().x / 4, box.CenterPoint().y, box.CenterPoint().z + box.Size().z / 4);
	newBox.SetFromCenterAndSize(NECenter,newSize);
	childs[NE] = new QuadtreeNode(newBox);

	//SE
	float3 SECenter(box.CenterPoint().x + box.Size().x / 4, box.CenterPoint().y, box.CenterPoint().z - box.Size().z / 4);
	newBox.SetFromCenterAndSize(SECenter, newSize);
	childs[SE] = new QuadtreeNode(newBox);

	//SW
	float3 SWCenter(box.CenterPoint().x - box.Size().x / 4, box.CenterPoint().y, box.CenterPoint().z - box.Size().z / 4);
	newBox.SetFromCenterAndSize(SWCenter, newSize);
	childs[SW] = new QuadtreeNode(newBox);

	//NW
	float3 NWCenter(box.CenterPoint().x - box.Size().x / 4, box.CenterPoint().y, box.CenterPoint().z + box.Size().z / 4);
	newBox.SetFromCenterAndSize(NWCenter, newSize);
	childs[NW] = new QuadtreeNode(newBox);

}

//Distribute objects in actual node to new childs
void QuadtreeNode::RedistributeChilds() {
	for (std::list<GameObject*>::iterator iterator = objectsInNode.begin(); iterator != objectsInNode.end();  ++iterator) {
		AABB newBox = (*iterator)->GetCompMesh()->GetAABB();

		//Redistribute objects in childs
		bool isIntersecting[4]; //1 intersection for child
		//Check intersections between child's box and new box
		for (int i = 0; i < 4; ++i) {
			isIntersecting[i] = childs[i]->box.Intersects(newBox);
		}

		if (isIntersecting[NE] == true && isIntersecting[SE] == true && isIntersecting[SW] == true && isIntersecting[NW] == true) {
			++iterator;//it hits all so we don't move it and we pass to next gameobject
		}
		else {
			//move it
			GameObject* gameObjectToMove = *iterator;
			objectsInNode.erase(iterator);//Remove
			for (int i = 0; i < 4; ++i) {
				if (isIntersecting[i] == true) {
					childs[i]->Insert(gameObjectToMove);//Place it in the child's box it intersects first
				}
			}

		}

		

	}
}



//---------------------------

//Quadtree


Quadtree::Quadtree() {

}

Quadtree::~Quadtree() {
	Clear();
}

void Quadtree::SetBoundries(const AABB& box) {
	Clear();
	root = new QuadtreeNode(box);
}

void Quadtree::Insert(GameObject* go) {
	if (root != nullptr) {
		if (go->GetCompMesh()->GetAABB().Intersects(root->box)) {
			root->Insert(go);
		}
	}
}

void Quadtree::Remove(GameObject* go) {
	if (root!=nullptr) {
		root->Remove(go);
	}
}

void Quadtree::Clear() {
	RELEASE(root);
}


//---------------------------

//Quadtree CollectIntersections using map
template<typename TYPE>
void Quadtree::CollectIntersections(std::map<float, GameObject*>& objects, const TYPE & primitive)const {
	if (root!=nullptr) {
		root->CollectIntersections(objects, primitive);
	}
}

//Quadtree CollectIntersections using vector
template<typename TYPE>
void Quadtree::CollectIntersections(std::vector<GameObject*>& objects, const TYPE & primitive)const {
	if (root!=nullptr) {
		root->CollectIntersections(objects, primitive);
	}
}

//QuadtreeNode CollectIntersections using map
template<typename TYPE>
void QuadtreeNode::CollectIntersections(std::map<float, GameObject*>& objects, const TYPE& primitive) const {
	if (primitive.Intersects(box) == true) {
		float hitNear;
		float hitFar;
		for (std::list<GameObject*>::const_iterator iterator = this->objects.begin(); iterator != this->objects.end(); ++iterator) {
			if (primitive.Intersects((*iterator)->GetCompMesh()->GetAABB(), hitNear, hitFar)) {
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
void QuadtreeNode::CollectIntersections(std::vector<GameObject*>& objects, const TYPE& primitive) const {
	if (primitive.Intersects(box) == true) {
		for (std::list<GameObject*>::const_iterator iterator = this->objects.begin(); iterator != this->objects.end(); ++iterator) {
			if (primitive.Intersects((*iterator)->GetCompMesh()->GetAABB())) {
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