#pragma once

#include<glew.h>
#include<freeglut.h>
#include<iostream>
#include<cmath>

#include "MyDataType.h"
using namespace std;
class Object;
class Oval;
class Box;
class Triangle;
class ObjectWithComponent;

class Object {
public:
	enum Shape { OVAL, BOX, TRIANGLE };
	Shape shape;
	Object() {

	}
	Object(int _width, int _height, GLdouble _rotation, Shape shape, GLdouble x = 0, GLdouble y = 0, GLdouble vecx = 0, GLdouble vecy = 0) : width(_width), height(_height), rotation(_rotation) {
		position = Vector2(x, y);
		velocity = Vector2(vecx, vecy);

	}

	int width;
	int height;

	string name;

	Vector2 position;
	Vector2 velocity;
	GLdouble rotation;
	bool isFixed = false;
	inline const Vector2& GetCurrentPosition() const { return position; }
	inline const GLdouble& GetCurrentRotation() const { return rotation; }
	inline const Vector2& GetCurrentVelocity() const {
		return velocity;
	}
	inline const Vector2& SetPosition(Vector2 _position) {
		position = _position;
		return position;
	}
	inline const Vector2& SetPosition(GLdouble x, GLdouble y) {
		position.x = x;
		position.y = y;
		return position;
	}
	inline const GLdouble& SetRotation(GLdouble _rotation) {
		rotation = _rotation;
		return rotation;
	}
	inline const Vector2& SetVelocity(Vector2 _velocity) {
		velocity = _velocity;
		return velocity;
	}
	inline const Vector2& SetVelocity(GLdouble x, GLdouble y) {
		velocity.x = x;
		velocity.y = y;
		return velocity;
	}
	inline const Vector2 GetSize() const {
		return Vector2(width, height);
	}
};
typedef Object Transform;

class Oval : public Object {
public:
	Oval() {
		shape = OVAL;
	}
	Oval(string name, int _width, int _height, GLdouble _rotation = 0) : Object(_width, _height, _rotation, OVAL) {
		shape = OVAL;
		this->name = name;
	}
};
class Box : public Object { //플레이어와 적의 Object
public:
	Box() {
		shape = BOX;
	}
	Box(string name, int _width, int _height, GLdouble _rotation = 0) : Object(_width, _height, _rotation, BOX) {
		shape = BOX;
		this->name = name;
	}
};
class Triangle : public Object {
public:
	Triangle() {
		shape = TRIANGLE;
	}
	Triangle(string _name, int _width, int _height, GLdouble _rotation = 0) : Object(_width, _height, _rotation, TRIANGLE) {
		shape = TRIANGLE;
		name = _name;
	}
};


class ObjectWithComponent {
public:	
	class CollisionComponent_ {
	public:
		Transform* collisionObject; //Collision object를 의미.
		Node<ObjectWithComponent, string>* gameObjectNode; //이 Component를 가진 GameObjectNode를 가르킴.
		CollisionComponent_(Transform* collisionObject, Object* parentObject) {
			this->collisionObject = collisionObject;
			this->collisionObject = collisionObject;
			if (f1 != NULL)
				f1(*this);
		}
		const Vector2 GetWorldPos() const {
			Node<ObjectWithComponent, string>* temp = gameObjectNode;
			Vector2 worldpos = collisionObject->position;
			while (temp != NULL)
			{
				worldpos += temp->data.object->position;
				temp = temp->precessor;
			}
			return worldpos;
		}
		const int GetWidth() const {
			return collisionObject->width;
		}
		const int GetHeight() const {
			return collisionObject->height;
		}
		const Object::Shape GetShape() const {
			return collisionObject->shape;
		}
		static void ConnectCollisionManagerAddFunction(void(*g) (CollisionComponent_ collisionComponent)) {
			f1 = g;
		}
	private:
		static void(*f1) (CollisionComponent_ collisionComponent);
	};
	Transform* object; //Object의 Transform Component
	CollisionComponent_* collisionComponent; // object의 collision Component
	ObjectWithComponent(Transform* object=NULL, CollisionComponent_* collisionComponent = NULL) {
		this->object = object;
		this->collisionComponent = collisionComponent;
	}
	void AddCollisionComponent(Object::Shape shape, GLdouble x, GLdouble y, int width, int height, GLdouble rotation);
	void AddCollisionComponentAsItself();
};
typedef ObjectWithComponent::CollisionComponent_ CollisionComponent;
typedef ObjectWithComponent GameObject;
typedef Node<ObjectWithComponent, string> GameObjectNode;
typedef BinaryTree<GameObject, string> GameObjectTree;


void ObjectWithComponent::AddCollisionComponent(Object::Shape shape, GLdouble x, GLdouble y, int width, int height, GLdouble rotation) {
	collisionComponent = new CollisionComponent_(new Transform(width, height, rotation, shape, x, y), object);
}
void ObjectWithComponent::AddCollisionComponentAsItself() {
	collisionComponent = new CollisionComponent_(new Transform(object->width, object->height, object->rotation, object->shape, 0, 0), object);
}



