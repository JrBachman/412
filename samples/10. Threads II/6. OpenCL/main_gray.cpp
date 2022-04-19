//====================================================================================
//	First attempt at simple image processing on the GPU, using OpenCL
//
//	This code is based on Apple's "Hello World" OpenCL code sample,
//	mixed with the "Lighthouse" library for loading/writing TGA image files
//
//	Authors:	Brett Saguid & Jean-Yves Hervé
//				©2015	3D Group for Interactive Visualization
//						Department of Computer Science and Statistics
//						University of Rhode Island
//	Revised 2020-03-30, 2022-04-05
//====================================================================================


//#include <fcntl.h>
#include <string>
#include <iostream>
//
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <OpenCL/opencl.h>
//
#include "ImageIO.h"

using namespace std;


//-------------------------------------------------------------------------------------
//	Definition of the various paths to source files and data
//	[since Xcode does not like paths relative to project anymore]
//-------------------------------------------------------------------------------------

#define XCODE_HELL	0

#if XCODE_HELL
	const string ROOT_PATH = "../../../9. Threads II/";
#else
	const string ROOT_PATH = "./";
#endif

const string KERNEL_FOLDER_PATH = ROOT_PATH + "Kernels/";
const string IMAGE_FOLDER_PATH = ROOT_PATH + "Images/";
const string OUTPUT_FOLDER_PATH = ROOT_PATH + "Output/";

const string KERNEL_FILE_PATH = KERNEL_FOLDER_PATH + "gray_kernel.cl";

const string INPUT_FILE_PATH = IMAGE_FOLDER_PATH + "claypots.tga";
const string OUTPUT_FILE_PATH = OUTPUT_FOLDER_PATH + "out_img.tga";

/* Create program from a file and compile it */
cl_program build_program(cl_context ctx, cl_device_id dev, const char* filename) {
    
    cl_program program;
    FILE *program_handle;
    char *program_buffer, *program_log;
    size_t program_size, log_size;
    int err;
    
    /* Read program file and place content into buffer */
    program_handle = fopen(filename, "r");
    if(program_handle == nullptr) {
        perror("Couldn't find the program file");
        exit(100);
    }
    fseek(program_handle, 0, SEEK_END);
    program_size = ftell(program_handle);
    rewind(program_handle);
    program_buffer = (char*)malloc(program_size + 1);
    program_buffer[program_size] = '\0';
    fread(program_buffer, sizeof(char), program_size, program_handle);
    fclose(program_handle);
    
    /* Create program from file */
    program = clCreateProgramWithSource(ctx, 1,
                                        (const char**)&program_buffer, &program_size, &err);
    if(err < 0) {
        perror("Couldn't create the program");
        exit(101);
    }
    free(program_buffer);
    
    /* Build program */
    err = clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);
    if(err < 0) {
        
        /* Find size of log and print to std output */
        clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
                              0, nullptr, &log_size);
        program_log = (char*) malloc(log_size + 1);
        program_log[log_size] = '\0';
        clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
                              log_size + 1, program_log, nullptr);
        printf("%s\n", program_log);
        free(program_log);
        exit(102);
    }
    
    return program;
}


void verifyFolders(void)
{
	//	Verify that the image folder exists
    DIR* imgDir = opendir(IMAGE_FOLDER_PATH.c_str());
    if (imgDir == nullptr)
	{
		cout << "Image folder not found" << endl;
		std::exit(-1);
    }
    else
	{
		if (closedir(imgDir) == -1L)
			cout << "could not close Image folder" << endl;

    }

    DIR* outDir = opendir(OUTPUT_FOLDER_PATH.c_str());
	//	If the directory does not exist, try to create it
    if (outDir == nullptr)
	{
		if (mkdir(OUTPUT_FOLDER_PATH.c_str(), static_cast<mode_t>(UF_SETTABLE | SF_SETTABLE)) != 0)
		{
			cout << "could not create Output folder" << endl;
			exit(4);
		}
    }
}


int main(int argc, char** argv)
{
cout << KERNEL_FOLDER_PATH << endl;

	//	Verifies that the required Images and Output folders are there.
	//	 Bails out in case of error
	verifyFolders();

    size_t global;                      // global domain size for our calculation
    size_t local;                       // local domain size for our calculation
    
    cl_device_id device_id;             // compute device id
    cl_context context;                 // compute context
    cl_command_queue commands;          // compute command queue
    cl_program program;                 // compute program
    
    int err;                            // Error code returned from api calls

	//	Read the overexposed, underexposed, correctly exposed images
	RasterImage inputImg = readImage(INPUT_FILE_PATH.c_str());

	//	allocate an output image at the same dimensions
    RasterImage resultImg = newImage(inputImg.width, inputImg.height,inputImg.type, 0);
    
    // if images have dfferent dimensions --> error and quit
	const unsigned int DATA_SIZE = inputImg.height * inputImg.width;
    
    cout << "image dimensions: " << inputImg.width << " x " << inputImg.height << endl;
    
    // Connect to a compute device
    //
    int gpu = 1;
    err = clGetDeviceIDs(nullptr, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, nullptr);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to create a device group!\n");
        return 11;
    }
    
    // Create a compute context
    //
    context = clCreateContext(0, 1, &device_id, nullptr, nullptr, &err);
    if (!context)
    {
        printf("Error: Failed to create a compute context!\n");
        return 12;
    }
    
    // Create a command commands
    //
    commands = clCreateCommandQueue(context, device_id, 0, &err);
    if (!commands)
    {
        printf("Error: Failed to create a command commands!\n");
        return 13;
    }
    
    // Build the program executable
    //
    program = build_program(context, device_id, KERNEL_FILE_PATH.c_str());

    // Create the compute kernel in the program we wish to run
    //
    cl_kernel kernel = clCreateKernel(program, "gray_conversion", &err);
    if (!kernel || err != CL_SUCCESS)
    {
        printf("Error: Failed to create compute kernel!\n");
        exit(14);
    }
    
    // Create the input and output arrays in device memory for our calculation
    //
    cl_mem inputGPUraster = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(int) * DATA_SIZE, nullptr, nullptr);
    cl_mem outputGPUraster = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * DATA_SIZE, nullptr, nullptr);
    if (!inputGPUraster || !outputGPUraster)
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(15);
    }
    
    // Write our data set into the input array in device memory
    //
    err = clEnqueueWriteBuffer(commands, inputGPUraster, CL_TRUE, 0, sizeof(int) * DATA_SIZE, inputImg.raster, 0, nullptr, nullptr);
    if (err != CL_SUCCESS)
    {
        cout << "Error: Failed to write input image to source array!" << endl;
        exit(16);
    }
    
    // Set the arguments to our compute kernel
    err = 0;
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &inputGPUraster);
    if (err != CL_SUCCESS)
    {
        cout << "Error: Failed to set kernel argument 0! Error code " << err << endl;
        exit(19);
    }
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &outputGPUraster);
    if (err != CL_SUCCESS)
    {
        cout << "Error: Failed to set kernel argument 1! Error code " << err << endl;
        exit(22);
    }
    err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &DATA_SIZE);
    if (err != CL_SUCCESS)
    {
        cout << "Error: Failed to set kernel argument 2! Error code " << err << endl;
        exit(23);
    }
    
    // Get the maximum work group size for executing the kernel on the device
    //
    err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, nullptr);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve kernel work group info! %d\n", err);
        exit(24);
    }

    // Execute the kernel over the entire range of our 1d input data set
    // using the maximum number of work group items for this device
    //
    global = DATA_SIZE;
    err = clEnqueueNDRangeKernel(commands, kernel, 1, nullptr, &global, &local, 0, nullptr, nullptr);
    if (err)
    {
        cout << "Error: Failed to execute kernel!" << endl;
        exit(25);
    }
    
    // Wait for the command commands to get serviced before reading back results
    //
    clFinish(commands);
    
    // Read back the results from the device to verify the output
    //
    err = clEnqueueReadBuffer(  commands, outputGPUraster, CL_TRUE, 0, sizeof(float) * DATA_SIZE,
								static_cast<int*>(resultImg.raster),
								0, nullptr, nullptr );
    if (err != CL_SUCCESS)
    {
        cout << "Error: Failed to read output array! error " << err << endl;
        exit(26);
    }
    
    // Validate our results
    //

    writeImage(OUTPUT_FILE_PATH.c_str(), &resultImg);
    
    // Print a brief summary detailing the results
    //
    cout << "Done!" << endl;
    
    // Shutdown and cleanup
    //
    clReleaseMemObject(inputGPUraster);
    clReleaseMemObject(outputGPUraster);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);
    
    return 0;
}


