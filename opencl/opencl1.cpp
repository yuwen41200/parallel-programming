/**
 * Image Histogram
 * Compilation Command: g++ -Wall -Wextra -Wpedantic -lOpenCL --std=c++11 opencl1.cpp -o opencl1
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
	std::string codes;
	ss << source.rdbuf();
	codes = ss.str();

//----------------------------------------------------------------------------//
//               OpenCL is the most ugly API I have ever seen.                //
//----------------------------------------------------------------------------//

	cl_platform_id   platform_id  = nullptr ;
	cl_uint          platform_num = 0       ;
	cl_device_id     device_id    = nullptr ;
	cl_uint          devices_num  = 0       ;
	cl_context       context      = nullptr ;
	cl_command_queue cmd_queue    = nullptr ;
	cl_mem           rst_mem      = nullptr ;
	cl_mem           img_mem      = nullptr ;
	cl_program       program      = nullptr ;
	cl_kernel        kernel       = nullptr ;
	cl_int           value        = 0       ;

	value = clGetPlatformIDs(1, &platform_id, &platform_num);
	value = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &devices_num);
	context = clCreateContext(nullptr, 1, &device_id, nullptr, nullptr, &value);
	cmd_queue = clCreateCommandQueue(context, device_id, 0, &value);
	rst_mem = clCreateBuffer(context, CL_MEM_READ_WRITE, 768 * sizeof(unsigned int), nullptr, &value);
	img_mem = clCreateBuffer(context, CL_MEM_READ_WRITE, size * sizeof(unsigned int), nullptr, &value);
	value = clEnqueueWriteBuffer(cmd_queue, rst_mem, CL_TRUE, 0, 768 * sizeof(unsigned int), result, 0, nullptr, nullptr);
	value = clEnqueueWriteBuffer(cmd_queue, img_mem, CL_TRUE, 0, size * sizeof(unsigned int), image, 0, nullptr, nullptr);
	program = clCreateProgramWithSource(context, 1, &(codes.c_str()), &(codes.size()), &value);
	value = clBuildProgram(program, 1, &device_id, nullptr, nullptr, nullptr);
	kernel = clCreateKernel(program, "histogram", &value);
	value = clSetKernelArg(kernel, 0, sizeof(cl_mem), &rst_mem);
	value = clSetKernelArg(kernel, 1, sizeof(cl_mem), &img_mem);
	value = clSetKernelArg(kernel, 2, sizeof(cl_uint), &size);
	value = clEnqueueTask(cmd_queue, kernel, 0, nullptr, nullptr);
	value = clEnqueueReadBuffer(cmd_queue, rst_mem, CL_TRUE, 0, 768 * sizeof(unsigned int), result, 0, nullptr, nullptr);
	value = clFlush(cmd_queue);
	value = clFinish(cmd_queue);
	value = clReleaseKernel(kernel);
	value = clReleaseProgram(program);
	value = clReleaseMemObject(mem);
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
