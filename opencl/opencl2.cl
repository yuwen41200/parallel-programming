__kernel void histogram(__global unsigned int *rst, __constant unsigned int *img, unsigned int sz) {
	unsigned int idx, no, tmp[256];
	no = get_global_id(0);
	for (idx = 0; idx < 256; ++idx)
		tmp[idx] = 0;
	for (idx = no; idx < sz; idx += 3)
		tmp[img[idx]]++;
	rst += 256 * no;
	for (idx = 0; idx < 256; ++idx)
		rst[idx] = tmp[idx];
}
