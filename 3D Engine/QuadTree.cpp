#include "GameObject.h"
#include "QuadTree.h"

#include "Devil/include/il.h"
#include "Devil/include/ilut.h"

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
	this->box = box;
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
	std::list<GameObject*>::iterator it = std::find(objectsInNode.begin(), objectsInNode.end(), go);
	if (it != objectsInNode.end())
		objectsInNode.erase(it);

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
	// Now redistribute ALL objects
	for (std::list<GameObject*>::iterator it = objectsInNode.begin(); it != objectsInNode.end();)
	{
		GameObject* go = *it;

		OBB b = go->GetAABB();
		AABB new_box(b.MinimalEnclosingAABB());

		// Now distribute this new gameobject onto the childs
		bool intersects[4];
		for (int i = 0; i < 4; ++i)
			intersects[i] = childs[i]->box.Intersects(new_box);

		if (intersects[0] && intersects[1] && intersects[2] && intersects[3])
			++it; // if it hits all childs, better to just keep it here
		else
		{
			it = objectsInNode.erase(it);
			for (int i = 0; i < 4; ++i)
				if (intersects[i]) childs[i]->Insert(go);
		}
	}
}

void QuadtreeNode::DebugDraw(){

	if (this != nullptr) {


		//float3 vertices[8];
		//box.GetCornerPoints(vertices);
		//glBegin(GL_QUADS);

		//glVertex3fv((GLfloat*)&vertices[1]);
		//glVertex3fv((GLfloat*)&vertices[5]);
		//glVertex3fv((GLfloat*)&vertices[7]);
		//glVertex3fv((GLfloat*)&vertices[3]);

		//glVertex3fv((GLfloat*)&vertices[4]);
		//glVertex3fv((GLfloat*)&vertices[0]);
		//glVertex3fv((GLfloat*)&vertices[2]);
		//glVertex3fv((GLfloat*)&vertices[6]);

		//glVertex3fv((GLfloat*)&vertices[5]);
		//glVertex3fv((GLfloat*)&vertices[4]);
		//glVertex3fv((GLfloat*)&vertices[6]);
		//glVertex3fv((GLfloat*)&vertices[7]);

		//glVertex3fv((GLfloat*)&vertices[0]);
		//glVertex3fv((GLfloat*)&vertices[1]);
		//glVertex3fv((GLfloat*)&vertices[3]);
		//glVertex3fv((GLfloat*)&vertices[2]);

		//glVertex3fv((GLfloat*)&vertices[3]);
		//glVertex3fv((GLfloat*)&vertices[7]);
		//glVertex3fv((GLfloat*)&vertices[6]);
		//glVertex3fv((GLfloat*)&vertices[2]);

		//glVertex3fv((GLfloat*)&vertices[0]);
		//glVertex3fv((GLfloat*)&vertices[4]);
		//glVertex3fv((GLfloat*)&vertices[5]);
		//glVertex3fv((GLfloat*)&vertices[1]);

		//
		//glEnd();

		for (int i = 0; i < 12; ++i) {
			glVertex3f(box.Edge(i).a.x, box.Edge(i).a.y, box.Edge(i).a.z);
			glVertex3f(box.Edge(i).b.x, box.Edge(i).b.y, box.Edge(i).b.z);
		}

	}

	for (int i = 0; i < 4;++i) {
		if (childs[i] != nullptr) {
			childs[i]->DebugDraw();
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
		if (go->GetAABB().Intersects(root->box)) {
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

void Quadtree::DebugDraw() {
	if (root!=nullptr) {
		glBegin(GL_LINES);
		glLineWidth(5.0f);
		root->DebugDraw();
		glEnd();
	}
}