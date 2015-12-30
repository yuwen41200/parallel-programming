/**
 * Image Histogram
 * Compilation Command: g++ -Wall -Wextra -lOpenCL opencl1.cpp -o opencl1
 * This program was originally written in serial method by the teacher.
 */

#include <fstream>
#include <sstream>
#include <string>

#ifdef __APPLE__
	#include <OpenCL/opencl.h>
#else
	#include <CL/cl.h>
#endif

int main() {
	std::fstream source("opencl2.cpp", std::ios_base::in);
	std::fstream input("opencl3.in", std::ios_base::in);
	std::fstream output("opencl4.out", std::ios_base::out);

	unsigned int index = 0, temp, size, *image, *result;
	input >> size;
	image = new unsigned int[size];
	result = new unsigned int[768];
	while (input >> temp)
		image[index++] = temp;
	for (index = 0; index < 768; ++index)
		result[index] = 0;

	std::stringstream ss;
	std::string code_str;
	const char *code_char;
	size_t code_len;
	ss << source.rdbuf();
	code_str = ss.str();
	code_char = code_str.c_str();
	code_len = code_str.size();

//----------------------------------------------------------------------------//
//               OpenCL is the most ugly API I have ever seen.                //
//----------------------------------------------------------------------------//

	cl_platform_id   platform_id  = 0;
	cl_uint          platform_num = 0;
	cl_device_id     device_id    = 0;
	cl_uint          devices_num  = 0;
	cl_context       context      = 0;
	cl_command_queue cmd_queue    = 0;
	cl_mem           rst_mem      = 0;
	cl_mem           img_mem      = 0;
	cl_mem           sz_mem       = 0;
	cl_program       program      = 0;
	cl_kernel        kernel       = 0;
	cl_int           ret          = 0;

	ret = clGetPlatformIDs(1, &platform_id, &platform_num);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &devices_num);
	context = clCreateContext(0, 1, &device_id, 0, 0, &ret);
	cmd_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	rst_mem = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 768 * sizeof(unsigned int), result, &ret);
	img_mem = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, size * sizeof(unsigned int), image, &ret);
	sz_mem = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(unsigned int), &size, &ret);
	program = clCreateProgramWithSource(context, 1, &code_char, &code_len, &ret);
	ret = clBuildProgram(program, 1, &device_id, 0, 0, 0);
	kernel = clCreateKernel(program, "histogram", &ret);
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &rst_mem);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &img_mem);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), &sz_mem);
	ret = clEnqueueTask(cmd_queue, kernel, 0, 0, 0);
	ret = clEnqueueReadBuffer(cmd_queue, rst_mem, CL_TRUE, 0, 768 * sizeof(unsigned int), result, 0, 0, 0);
	ret = clFlush(cmd_queue);
	ret = clFinish(cmd_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(rst_mem);
	ret = clReleaseMemObject(img_mem);
	ret = clReleaseMemObject(sz_mem);
	ret = clReleaseCommandQueue(cmd_queue);
	ret = clReleaseContext(context);

//----------------------------------------------------------------------------//
//                          End of these ugly codes.                          //
//----------------------------------------------------------------------------//

	for (index = 0; index < 768; ++index) {
		if (index % 256 == 0 && index != 0)
			output << std::endl;
		output << result[index] << ' ';
	}

	delete image;
	delete result;

	source.close();
	input.close();
	output.close();
	return 0;
}
