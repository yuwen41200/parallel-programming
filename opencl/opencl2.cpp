__kernel void histogram(unsigned int *rst, unsigned int *img, unsigned int sz) {
	unsigned int idx;
	for (idx = 0; idx < sz; idx += 3)
		rst[img[idx]]++;

	rst += 256;
	for (idx = 1; idx < sz; idx += 3)
		rst[img[idx]]++;

	rst += 256;
	for (idx = 2; idx < sz; idx += 3)
		rst[img[idx]]++;
}
