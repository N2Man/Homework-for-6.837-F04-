#include "image.h"
#include "matrix.h"
#include "vectors.h"
#include "IFS.h"
#include <vector>


void IFS::input(char* filename) {
	FILE* inputfile;
	fopen_s(&inputfile, filename, "r");
	//assert(inputfile != nullptr);
	// input n and matrices
	int num_transforms;
	fscanf_s(inputfile, "%d", &num_transforms);
	n = num_transforms;
	for (int i = 0; i < n; i++) {
		fscanf_s(inputfile, "%f", &probs[i]);
		trans[i].Read3x3(inputfile);
	}
	fclose(inputfile);
	inputfile = nullptr;
}

void IFS::render(Image& img, int points, int iterations) {
	int width = img.Width();
	int height = img.Height();
	for (int i = 0; i < points; i++) {
		// random point
		Vec2f v = Vec2f(rand() * 1.0f / RAND_MAX, rand() * 1.0f / RAND_MAX);
		for (int j = 0; j < iterations; j++) {
			float sample = rand() * 1.0f / RAND_MAX;
			int k = 0;
			for (k = 0; k < n; k++) {
				if (sample > probs[i])
					continue;
				break;
			}
			trans[k].Transform(v);
		}
		if (v.x() >= 0 && v.x() <= 1 && v.y() >= 0 && v.y() <= 1) {
			img.SetPixel(v.x() * width, v.y() * height, Vec3f(0.0, 0.0, 0.0));
		}
	}
}