/*******************************************************************************
Vendor: Xilinx 
Associated Filename: test-cl.c
Purpose: OpenCL Host Code for Matrix Multiply Example
Revision History: July 1, 2013 - initial release
                                                
*******************************************************************************
Copyright (C) 2013 XILINX, Inc.

This file contains confidential and proprietary information of Xilinx, Inc. and 
is protected under U.S. and international copyright and other intellectual 
property laws.

DISCLAIMER
This disclaimer is not a license and does not grant any rights to the materials 
distributed herewith. Except as otherwise provided in a valid license issued to 
you by Xilinx, and to the maximum extent permitted by applicable law: 
(1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX 
HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, 
INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR 
FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether 
in contract or tort, including negligence, or under any other theory of 
liability) for any loss or damage of any kind or nature related to, arising under 
or in connection with these materials, including for any direct, or any indirect, 
special, incidental, or consequential loss or damage (including loss of data, 
profits, goodwill, or any type of loss or damage suffered as a result of any 
action brought by a third party) even if such damage or loss was reasonably 
foreseeable or Xilinx had been advised of the possibility of the same.

CRITICAL APPLICATIONS
Xilinx products are not designed or intended to be fail-safe, or for use in any 
application requiring fail-safe performance, such as life-support or safety 
devices or systems, Class III medical devices, nuclear facilities, applications 
related to the deployment of airbags, or any other applications that could lead 
to death, personal injury, or severe property or environmental damage 
(individually and collectively, "Critical Applications"). Customer assumes the 
sole risk and liability of any use of Xilinx products in Critical Applications, 
subject only to applicable laws and regulations governing limitations on product 
liability. 

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT 
ALL TIMES.

*******************************************************************************/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <CL/opencl.h>

#include <iostream>
#include <chrono>
#include "defines.h"
#include "signals.h"

using namespace std;
using namespace std::chrono;

#define NUM_OF_SUPPORTED_DEVICES 3

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

int
load_file_to_memory(const char *filename, char **result)
{ 
    size_t size = 0;
    FILE *f = fopen(filename, "rb");
    if (f == NULL) 
        { 
            *result = NULL;
            return -1; // -1 means file opening fail 
        } 
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    *result = (char *)malloc(size+1);
    if (size != fread(*result, sizeof(char), size, f)) 
        { 
            free(*result);
            return -2; // -2 means file reading fail 
        } 
    fclose(f);
    (*result)[size] = 0;
    return size;
}

int main(int argc, char** argv)
{
    //TARGET_DEVICE macro needs to be passed from gcc command line
#if defined(SDA_PLATFORM) && !defined(TARGET_DEVICE)
  #define STR_VALUE(arg)      #arg
  #define GET_STRING(name) STR_VALUE(name)
  #define TARGET_DEVICE GET_STRING(SDA_PLATFORM)
#endif

    char *TARGET_DEVICES[] = {"xilinx:adm-pcie-ku3:2ddr-xpr:4.0"};
    char *XCLBIN_FILES[] = {"kernel_ku3.xclbin"};
    int NUM_SUPPORTED_DEVICES = sizeof(TARGET_DEVICES)/sizeof(char *);
    char *xclbin;

    int err;                            // error code returned from api calls
     
    int *data = data_test;
    int rng_state[]= {7,456,99};
    int *leaf_array = leaf_array_test;

    int *result_hw_index =(int*) malloc(N_POINTS * K_NEIGHBORS * sizeof(int));
    int *result_hw_dist = (int*) malloc(N_POINTS * K_NEIGHBORS * sizeof(int));

    int i;
    int j;

    cl_platform_id platforms[16];       // platform id
    cl_platform_id platform_id;         // platform id
    cl_uint platform_count;
    cl_device_id device_id;             // compute device id 
    cl_context context;                 // compute context
    cl_command_queue commands;          // compute command queue
    cl_program program;                 // compute program
    cl_kernel kernel;                   // compute kernel
   
    char cl_platform_vendor[1001];
   
    cl_mem data_in;                     // device memory used for the input array
    cl_mem rng_state_in;                // device memory used for the input array
#if RP_TREE_INIT
    cl_mem leaf_array_in;               // device memory used for the input array
#endif
    cl_mem result_out_index;            // device memory used for the output array
    cl_mem result_out_dist;             // device memory used for the output array
    
    std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();

    // 
    // Get all platforms and then select Xilinx platform
    err = clGetPlatformIDs(16, platforms, &platform_count);
    if (err != CL_SUCCESS)
        {
            printf("Error: Failed to find an OpenCL platform!\n");
            printf("Test failed with err: %d\n", err);
            return EXIT_FAILURE;
        }
    printf("INFO: Found %d platforms\n", platform_count);

    // Find Xilinx Plaftorm
    int platform_found = 0;
    for (unsigned int iplat=0; iplat<platform_count; iplat++) {
        err = clGetPlatformInfo(platforms[iplat], CL_PLATFORM_VENDOR, 1000, (void *)cl_platform_vendor,NULL);
        if (err != CL_SUCCESS) {
            printf("Error: clGetPlatformInfo(CL_PLATFORM_VENDOR) failed!\n");
            printf("Test failed\n");
            return EXIT_FAILURE;
        }
        if (strcmp(cl_platform_vendor, "Xilinx") == 0) {
            printf("INFO: Selected platform %d from %s\n", iplat, cl_platform_vendor);
            platform_id = platforms[iplat];
            platform_found = 1;
        }
    }
    if (!platform_found) {
        printf("ERROR: Platform Xilinx not found. Exit.\n");
        return EXIT_FAILURE;
    }
  
    // Connect to a compute device
    // find all devices and then select the target device
    cl_device_id devices[16];  // compute device id 
    cl_uint device_count;
    unsigned int device_found = 0;
    char cl_device_name[1001];
    err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ACCELERATOR,
                         16, devices, &device_count);
    if (err != CL_SUCCESS) {
        printf("Error: Failed to create a device group!\n");
        printf("Test failed\n");
        return EXIT_FAILURE;
    }

    //iterate all devices to select the target device. 
    for (i=0; (i<device_count) && (!device_found); i++) {
        err = clGetDeviceInfo(devices[i], CL_DEVICE_NAME, 1024, cl_device_name, 0);
        if (err != CL_SUCCESS) {
            printf("Error: Failed to get device name for device %d!\n", i);
            printf("Test failed\n");
            return EXIT_FAILURE;
        }
        printf("INFO: Analyzing device: %s\n", cl_device_name);
        for(int t=0; t < NUM_SUPPORTED_DEVICES; t++){
            if(strcmp(cl_device_name, TARGET_DEVICES[t]) == 0) {
                device_id = devices[i];
                device_found = 1;
                xclbin = XCLBIN_FILES[t];
                printf("INFO: Selected %s as the target device\n", cl_device_name);
            }
        }
    }
    
    if (!device_found) {
        printf("ERROR: Target device not found. Exit.\n");
        return EXIT_FAILURE;
    }


    err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ACCELERATOR,
                         1, &device_id, NULL);
    if (err != CL_SUCCESS)
        {
            printf("Error: Failed to create a device group!\n");
            printf("Test failed\n");
            return EXIT_FAILURE;
        }
  
    // Create a compute context 
    //
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    if (!context)
        {
            printf("Error: Failed to create a compute context!\n");
            printf("Test failed\n");
            return EXIT_FAILURE;
        }

    // Create a command commands
    //
    commands = clCreateCommandQueue(context, device_id, 0, &err);
    if (!commands)
        {
            printf("Error: Failed to create a command commands!\n");
            printf("Error: code %i\n",err);
            printf("Test failed\n");
            return EXIT_FAILURE;
        }

    int status;

    // Create Program Objects
    //
  
    // Load binary from disk
    unsigned char *kernelbinary;
    printf("INFO: Loading %s\n", xclbin);
    int n_i = load_file_to_memory(xclbin, (char **) &kernelbinary);
    if (n_i < 0) {
        printf("failed to load kernel from xclbin: %s\n", xclbin);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
    size_t n = n_i;
    // Create the compute program from offline
    program = clCreateProgramWithBinary(context, 1, &device_id, &n,
                                        (const unsigned char **) &kernelbinary, &status, &err);
    if ((!program) || (err!=CL_SUCCESS)) {
        printf("Error: Failed to create compute program from binary %d!\n", err);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }

    // Build the program executable
    //
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
        {
            size_t len;
            char buffer[2048];

            printf("Error: Failed to build program executable!\n");
            clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
            printf("%s\n", buffer);
            printf("Test failed\n");
            return EXIT_FAILURE;
        }

    // Create the compute kernel in the program we wish to run
    //
    kernel = clCreateKernel(program, "nn_descent", &err);
    if (!kernel || err != CL_SUCCESS)
        {
            printf("Error: Failed to create compute kernel!\n");
            printf("Test failed\n");
            return EXIT_FAILURE;
        }
    


    // Create the input and output arrays in device memory for our calculation
    //
    data_in = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(int) * N_POINTS * N_FEATURES , NULL, NULL);
    rng_state_in = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(int) * N_STATES , NULL, NULL);
#if RP_TREE_INIT
    leaf_array_in = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(int) * N_LEAF * LEAF_SIZE , NULL, NULL);
#endif
    result_out_index = clCreateBuffer(context,  CL_MEM_WRITE_ONLY,  sizeof(int) * N_POINTS * K_NEIGHBORS , NULL, NULL);
    result_out_dist = clCreateBuffer(context,  CL_MEM_WRITE_ONLY,  sizeof(int) * N_POINTS * K_NEIGHBORS , NULL, NULL);

    if (!data_in || !rng_state_in || !result_out_index || !result_out_dist
#if RP_TREE_INIT 
    || !leaf_array_in
#endif
    )
        {
            printf("Error: Failed to allocate device memory!\n");
            printf("Test failed\n");
            return EXIT_FAILURE;
        } 

    std::chrono::high_resolution_clock::time_point begin_write_inputs = std::chrono::high_resolution_clock::now();

    // Write our data set into the input array in device memory 
    //
    err = clEnqueueWriteBuffer(commands, data_in, CL_TRUE, 0, sizeof(int) * N_POINTS * N_FEATURES, data, 0, NULL, NULL);
    if (err != CL_SUCCESS)
        {
            printf("Error: Failed to write to source array data!\n");
            printf("Test failed\n");
            return EXIT_FAILURE;
        }

    // Write our data set into the input array in device memory 
    //
    err = clEnqueueWriteBuffer(commands, rng_state_in, CL_TRUE, 0, sizeof(int) * N_STATES, rng_state, 0, NULL, NULL);
    if (err != CL_SUCCESS)
        {
            printf("Error: Failed to write to source array rng_state!\n");
            printf("Test failed\n");
            return EXIT_FAILURE;
        }
    
#if RP_TREE_INIT
    // Write our data set into the input array in device memory 
    //
    err = clEnqueueWriteBuffer(commands, leaf_array_in, CL_TRUE, 0, sizeof(int) * N_LEAF * LEAF_SIZE, leaf_array, 0, NULL, NULL);
    if (err != CL_SUCCESS)
        {
            printf("Error: Failed to write to source array leaf_array_in!\n");
            printf("Test failed\n");
            return EXIT_FAILURE;
        }
#endif

    // Set the arguments to our compute kernel
    //
    err = 0;
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &data_in);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &rng_state_in);
#if RP_TREE_INIT
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &leaf_array_in);
#endif
    err |= clSetKernelArg(kernel, 2+RP_TREE_INIT, sizeof(cl_mem), &result_out_index);
    err |= clSetKernelArg(kernel, 3+RP_TREE_INIT, sizeof(cl_mem), &result_out_dist);

    if (err != CL_SUCCESS)
        {
            printf("Error: Failed to set kernel arguments! %d\n", err);
            printf("Test failed\n");
            return EXIT_FAILURE;
        }
    
    std::chrono::high_resolution_clock::time_point begin_computation = std::chrono::high_resolution_clock::now();
    
    // Execute the kernel
    //
    cl_event executeevent;
    err = clEnqueueTask(commands, kernel, 0, NULL, &executeevent);

    if (err)
        {
            printf("Error: Failed to execute kernel! %d\n", err);
            printf("Test failed\n");
            return EXIT_FAILURE;
        }
    clWaitForEvents(1, &executeevent);

    std::chrono::high_resolution_clock::time_point begin_read_outputs = std::chrono::high_resolution_clock::now();

    // Read back the results from the device to verify the output
    //
    cl_event readevent;
    err = clEnqueueReadBuffer( commands, result_out_index, CL_TRUE, 0, sizeof(int) * N_POINTS * K_NEIGHBORS, result_hw_index, 0, NULL, &readevent );  
    if (err != CL_SUCCESS)
        {
            printf("Error: Failed to read output array! %d\n", err);
            printf("Test failed\n");
            return EXIT_FAILURE;
        }
    clWaitForEvents(1, &readevent);

    // Read back the results from the device to verify the output
    //
    cl_event readevent2;
    err = clEnqueueReadBuffer( commands, result_out_dist, CL_TRUE, 0, sizeof(int) * N_POINTS * K_NEIGHBORS, result_hw_dist, 0, NULL, &readevent2 );  
    if (err != CL_SUCCESS)
        {
            printf("Error: Failed to read output array! %d\n", err);
            printf("Test failed\n");
            return EXIT_FAILURE;
        }
    clWaitForEvents(1, &readevent2);
    
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

    // Validate our results
    //
    printf("\nHW COMPUTATION ENDED!\n\n");

	/////////////////////////////////////////////////////////////////////////////

	int N_PRINT=0;
	for(i=N_POINTS-N_PRINT; i<N_POINTS; i++){
		printf("Considering the point: idx:%d ", i);
		for(int k=0; k<N_FEATURES; k++){
			printf("%c:%d ", 'A'+k, data[i*N_FEATURES + k]);
		}
		printf("\n");
		for(j=0; j<K_NEIGHBORS; j++){
			printf("idx:%d ",result_hw_index[i*K_NEIGHBORS + j]);
			printf("at distance %d", result_hw_dist[i*K_NEIGHBORS + j]);
			printf(" (from Python idx: %d, dist: %f)", knn_indices_test[i*K_NEIGHBORS + j], knn_dists_test[i*K_NEIGHBORS + j]*knn_dists_test[i*K_NEIGHBORS + j]);
			printf("\n");
		}
		printf("\n");
	}
	
	printf("Input transfer duration: %f | Comuptation duration: %f | Output transfer duration %f\n", double(duration_cast<microseconds> (begin_computation-begin_write_inputs).count())/1000000, double(duration_cast<microseconds> (begin_read_outputs-begin_computation).count())/1000000, double(duration_cast<microseconds> (end-begin_read_outputs).count())/1000000);
	printf("Fpga total time %f; Python total time = %f\n", double(duration_cast<microseconds> (end-begin).count())/1000000 , time_python);

	printf("\nPRINT ENDED!\n\n");
    
	/////////////////////////////////////////////////////////////////////////////
    
    // Shutdown and cleanup
    //
    clReleaseMemObject(data_in);
    clReleaseMemObject(rng_state_in);
#if RP_TREE_INIT
    clReleaseMemObject(leaf_array_in);
#endif
    clReleaseMemObject(result_out_index);
    clReleaseMemObject(result_out_dist);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);

    return EXIT_SUCCESS;
}

// XSIP watermark, do not delete 67d7842dbbe25473c3c32b93c0da8047785f30d78e8a024de1b57352245f9689
