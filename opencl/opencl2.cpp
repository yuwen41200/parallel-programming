__kernel void histogram(unsigned int *rst, unsigned int *img, unsigned int sz) {
	unsigned int idx, *tmp;
	tmp = rst;

	for (idx = 0; idx < sz; idx += 3)
		tmp[img[idx]]++;

	tmp += 256;
	for (idx = 1; idx < sz; idx += 3)
		tmp[img[idx]]++;

	tmp += 256;
	for (idx = 2; idx < sz; idx += 3)
		tmp[img[idx]]++;
}
