/**
 * Image Histogram
 * Compilation Command: g++ -Wall -Wextra -lOpenCL opencl1.cpp -o opencl1
 * This program was originally written in serial method by the teacher.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#ifdef __APPLE__
	#include <OpenCL/opencl.h>
#else
	#include <CL/cl.h>
#endif

const char *clGetErrorString(cl_int);

int main() {
	std::fstream source("opencl2.cl", std::ios_base::in);
	std::fstream input("opencl3.in", std::ios_base::in);
	std::fstream output("opencl4.out", std::ios_base::out);

	unsigned int index = 0, temp, size, *image, *result;
	input >> size;
	image = new unsigned int[size];
	result = new unsigned int[768];
	while (input >> temp)
		image[index++] = temp;

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
	cl_build_status  status       = 0;
	char             *log_char    = 0;
	size_t           log_len      = 0;
	size_t           work_size    = 1;

	ret = clGetPlatformIDs(1, &platform_id, &platform_num);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &devices_num);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	context = clCreateContext(0, 1, &device_id, 0, 0, &ret);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	cmd_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	rst_mem = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 768 * sizeof(unsigned int), 0, &ret);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	img_mem = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, size * sizeof(unsigned int), image, &ret);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	sz_mem = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(unsigned int), &size, &ret);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	program = clCreateProgramWithSource(context, 1, &code_char, &code_len, &ret);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	ret = clBuildProgram(program, 1, &device_id, "-Werror", 0, 0);
	if (ret != CL_SUCCESS) {
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &status, 0);
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, 0, &log_len);
		log_char = new char[log_len + 1];
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_len + 1, log_char, 0);
		std::cerr << clGetErrorString(ret) << ", status = " << status << ", message: " << std::endl << log_char;
		delete log_char;
	}
	kernel = clCreateKernel(program, "histogram", &ret);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &rst_mem);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &img_mem);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), &sz_mem);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	ret = clEnqueueNDRangeKernel(cmd_queue, kernel, 1, 0, &work_size, 0, 0, 0, 0);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	ret = clEnqueueReadBuffer(cmd_queue, rst_mem, CL_TRUE, 0, 768 * sizeof(unsigned int), result, 0, 0, 0);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	ret = clFlush(cmd_queue);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	ret = clFinish(cmd_queue);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	ret = clReleaseKernel(kernel);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	ret = clReleaseProgram(program);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	ret = clReleaseMemObject(rst_mem);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	ret = clReleaseMemObject(img_mem);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	ret = clReleaseMemObject(sz_mem);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	ret = clReleaseCommandQueue(cmd_queue);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;
	ret = clReleaseContext(context);
	if (ret != CL_SUCCESS) std::cerr << clGetErrorString(ret) << std::endl;

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

const char *clGetErrorString(cl_int code) {
	switch(code) {
		case 0: return "CL_SUCCESS";
		case -1: return "CL_DEVICE_NOT_FOUND";
		case -2: return "CL_DEVICE_NOT_AVAILABLE";
		case -3: return "CL_COMPILER_NOT_AVAILABLE";
		case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
		case -5: return "CL_OUT_OF_RESOURCES";
		case -6: return "CL_OUT_OF_HOST_MEMORY";
		case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
		case -8: return "CL_MEM_COPY_OVERLAP";
		case -9: return "CL_IMAGE_FORMAT_MISMATCH";
		case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
		case -11: return "CL_BUILD_PROGRAM_FAILURE";
		case -12: return "CL_MAP_FAILURE";
		case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
		case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
		case -15: return "CL_COMPILE_PROGRAM_FAILURE";
		case -16: return "CL_LINKER_NOT_AVAILABLE";
		case -17: return "CL_LINK_PROGRAM_FAILURE";
		case -18: return "CL_DEVICE_PARTITION_FAILED";
		case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";
		case -30: return "CL_INVALID_VALUE";
		case -31: return "CL_INVALID_DEVICE_TYPE";
		case -32: return "CL_INVALID_PLATFORM";
		case -33: return "CL_INVALID_DEVICE";
		case -34: return "CL_INVALID_CONTEXT";
		case -35: return "CL_INVALID_QUEUE_PROPERTIES";
		case -36: return "CL_INVALID_COMMAND_QUEUE";
		case -37: return "CL_INVALID_HOST_PTR";
		case -38: return "CL_INVALID_MEM_OBJECT";
		case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
		case -40: return "CL_INVALID_IMAGE_SIZE";
		case -41: return "CL_INVALID_SAMPLER";
		case -42: return "CL_INVALID_BINARY";
		case -43: return "CL_INVALID_BUILD_OPTIONS";
		case -44: return "CL_INVALID_PROGRAM";
		case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
		case -46: return "CL_INVALID_KERNEL_NAME";
		case -47: return "CL_INVALID_KERNEL_DEFINITION";
		case -48: return "CL_INVALID_KERNEL";
		case -49: return "CL_INVALID_ARG_INDEX";
		case -50: return "CL_INVALID_ARG_VALUE";
		case -51: return "CL_INVALID_ARG_SIZE";
		case -52: return "CL_INVALID_KERNEL_ARGS";
		case -53: return "CL_INVALID_WORK_DIMENSION";
		case -54: return "CL_INVALID_WORK_GROUP_SIZE";
		case -55: return "CL_INVALID_WORK_ITEM_SIZE";
		case -56: return "CL_INVALID_GLOBAL_OFFSET";
		case -57: return "CL_INVALID_EVENT_WAIT_LIST";
		case -58: return "CL_INVALID_EVENT";
		case -59: return "CL_INVALID_OPERATION";
		case -60: return "CL_INVALID_GL_OBJECT";
		case -61: return "CL_INVALID_BUFFER_SIZE";
		case -62: return "CL_INVALID_MIP_LEVEL";
		case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
		case -64: return "CL_INVALID_PROPERTY";
		case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
		case -66: return "CL_INVALID_COMPILER_OPTIONS";
		case -67: return "CL_INVALID_LINKER_OPTIONS";
		case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";
		case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
		case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
		case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
		case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
		case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
		case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
		default: return "CL_UNKNOWN_ERROR";
	}
}
