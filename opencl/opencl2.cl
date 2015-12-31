__kernel void histogram(__global unsigned int *rst, __constant unsigned int *img, unsigned int sz) {
	unsigned int idx;

	for (idx = 0; idx < 768; ++idx)
		rst[idx] = 0;

	for (idx = 0; idx < sz; idx += 3)
		rst[img[idx]]++;

	rst += 256;
	for (idx = 1; idx < sz; idx += 3)
		rst[img[idx]]++;

	rst += 256;
	for (idx = 2; idx < sz; idx += 3)
		rst[img[idx]]++;
}
