#include "image.h"
#include "matrix.h"
#include "vectors.h"
#include <vector>

class IFS {
public:
	int n; //the number of transformations
	vector<Matrix> trans;
	vector<float> probs;
	IFS() : n(0), trans(0), probs(0) {}
	void input(char *filename);
	void render(Image &img, int points, int iterations);
};