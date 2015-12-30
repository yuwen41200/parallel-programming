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

	cl_platform_id   platform_id  = NULL ;
	cl_uint          platform_num = 0    ;
	cl_device_id     device_id    = NULL ;
	cl_uint          devices_num  = 0    ;
	cl_context       context      = NULL ;
	cl_command_queue cmd_queue    = NULL ;
	cl_mem           rst_mem      = NULL ;
	cl_mem           img_mem      = NULL ;
	cl_mem           sz_mem       = NULL ;
	cl_program       program      = NULL ;
	cl_kernel        kernel       = NULL ;
	cl_int           value        = 0    ;

	value = clGetPlatformIDs(1, &platform_id, &platform_num);
	value = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &devices_num);
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &value);
	cmd_queue = clCreateCommandQueue(context, device_id, 0, &value);
	rst_mem = clCreateBuffer(context, CL_MEM_READ_WRITE, 768 * sizeof(unsigned int), NULL, &value);
	img_mem = clCreateBuffer(context, CL_MEM_READ_WRITE, size * sizeof(unsigned int), NULL, &value);
	sz_mem = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(unsigned int), NULL, &value);
	value = clEnqueueWriteBuffer(cmd_queue, rst_mem, CL_TRUE, 0, 768 * sizeof(unsigned int), result, 0, NULL, NULL);
	value = clEnqueueWriteBuffer(cmd_queue, img_mem, CL_TRUE, 0, size * sizeof(unsigned int), image, 0, NULL, NULL);
	value = clEnqueueWriteBuffer(cmd_queue, sz_mem, CL_TRUE, 0, sizeof(unsigned int), size, 0, NULL, NULL);
	program = clCreateProgramWithSource(context, 1, &code_char, &code_len, &value);
	value = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	kernel = clCreateKernel(program, "histogram", &value);
	value = clSetKernelArg(kernel, 0, sizeof(cl_mem), &rst_mem);
	value = clSetKernelArg(kernel, 1, sizeof(cl_mem), &img_mem);
	value = clSetKernelArg(kernel, 2, sizeof(cl_mem), &sz_mem);
	value = clEnqueueTask(cmd_queue, kernel, 0, NULL, NULL);
	value = clEnqueueReadBuffer(cmd_queue, rst_mem, CL_TRUE, 0, 768 * sizeof(unsigned int), result, 0, NULL, NULL);
	value = clFlush(cmd_queue);
	value = clFinish(cmd_queue);
	value = clReleaseKernel(kernel);
	value = clReleaseProgram(program);
	value = clReleaseMemObject(rst_mem);
	value = clReleaseMemObject(img_mem);
	value = clReleaseMemObject(sz_mem);
	value = clReleaseCommandQueue(cmd_queue);
	value = clReleaseContext(context);

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
