/**
 * Image Histogram
 * Compilation Command: g++ -Wall -Wextra -Wpedantic -lOpenCL --std=c++11 opencl1.cpp -o opencl1
 * This program was originally written in serial method by the teacher.
 */

#include <fstream>

unsigned int *histogram(unsigned int *, unsigned int);

int main() {
	std::fstream input("opencl2.in", std::ios_base::in);
	std::fstream output("opencl3.out", std::ios_base::out);
	unsigned int index = 0, temp, size;

	input >> size;
	unsigned int *image = new unsigned int[size];

	while(input >> temp)
		image[index++] = temp;

	unsigned int *result = histogram(image, size);

	for (index = 0; index < 256*3; ++index) {
		if (index % 256 == 0 && index != 0)
			output << std::endl;
		output << result[index] << ' ';
	}

	delete result;
	delete image;

	input.close();
	output.close();
	return 0;
}

unsigned int *histogram(unsigned int *img, unsigned int sz) {
	unsigned int idx;
	unsigned int *rst = new unsigned int[256*3];

	for (idx = 0; idx < 256*3; ++idx)
		rst[idx] = 0;

	for (idx = 0; idx < sz; idx += 3)
		rst[img[idx]]++;

	rst += 256;
	for (idx = 1; idx < sz; idx += 3)
		rst[img[idx]]++;

	rst += 256;
	for (idx = 2; idx < sz; idx += 3)
		rst[img[idx]]++;

	rst -= 256*2;
	return rst;
}
