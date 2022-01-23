#pragma once
#include "Object3D.h"
#include "vectors.h"


class Group : public Object3D {
private:
	int objectsNumber;
	Object3D** objs;
public:
	Group() :objectsNumber(0), objs(nullptr) {};
	Group(int n) :objectsNumber(n) {
		objs = new Object3D* [n];
	}
	~Group() {
		for (int i = 0; i < objectsNumber; i++) {
			delete objs[i];
		}
		delete[] objs;
	}
	void addObject(int index, Object3D* obj) {
		assert(index < objectsNumber);
		objs[index] = obj;
	}
};
