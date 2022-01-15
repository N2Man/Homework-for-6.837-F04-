#include "image.h"
#include "matrix.h"
#include "vectors.h"
#include "IFS.h"
#include <vector>

int main(int argc, char *argv[]) {
	char *input_file = nullptr;
	int points = 100;
	int iters = 100;
	int size = 100;
	char *output_file = nullptr;
	
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-input")) {
			i++;
			input_file = argv[i];
		}
		else if (!strcmp(argv[i], "-points")) {
			i++;
			points = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-iters")) {
			i++;
			iters = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-size")) {
			i++;
			size = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-output")) {
			i++;
			output_file = argv[i];
		}
	}

	IFS ifs;
	Image img(size, size);
	ifs.input(input_file);
	ifs.render(img, points, iters);
	img.SavePPM(output_file);
	return 0;
}