#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include "matrix.h"
#include "image.h"

class IFS {
public:
	int n;
	vector<Matrix> transformation;
	vector<float> probability;
	IFS();
	~IFS();
	void readFile(char* filename);
	void render(Image& image, float num_points, float num_iters);
	float readFloat(FILE* file);
};