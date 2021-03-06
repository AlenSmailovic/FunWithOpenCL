/*
** Start1.0
** Alen Smailovic
** 13/01/2018

*****************************************************
* Lenovo Z51 - i7 5500U 2.4GHz - AMD Radeon R9 M375 *
*****************************************************
Config Table | Device[0] | Device[1]
-------------------------------------
Platform [0] | Intel CPU | Intel GPU
Platform [1] | AMD GPU   | Intel CPU
Platform [2] | AMD GPU   | Intel CPU
-------------------------------------
*/

#include "stdafx.h"

int main()
{
   /******************************************************/

   std::vector<cl::Platform> oVecPlatforms;
   cl::Platform::get(&oVecPlatforms);

   // choose Platform : above explained
   auto oPlatform = oVecPlatforms[1];
   std::vector<cl::Device> oVecDevices;
   oPlatform.getDevices(CL_DEVICE_TYPE_GPU, &oVecDevices);

   // choose Device : above explained
   auto oDevice = oVecDevices[0];
   auto oVendor = oDevice.getInfo<CL_DEVICE_VENDOR>();
   auto oVersion = oDevice.getInfo<CL_DEVICE_VERSION>();

   /******************************************************/

   std::ifstream oFile("../../Kernels/Start10.cl");
   std::string oSrc(std::istreambuf_iterator<char>(oFile), (std::istreambuf_iterator<char>()));

   cl::Program::Sources oSources(1, std::make_pair(oSrc.c_str(), oSrc.length() + 1));

   cl::Context oContext(oDevice);
   cl::Program oProgram(oContext, oSources);

   oProgram.build("-cl-std=CL1.2");

   std::vector<int> oVecNaturals(ELEMENTS);

   cl_int err = 0;
   cl::Buffer bufIn(oContext, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int) * oVecNaturals.size(), oVecNaturals.data(), &err);
   cl::Buffer bufOut(oContext, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int) * oVecNaturals.size(), nullptr, &err);
   cl::Kernel oKernel(oProgram, "Start10");

   err = oKernel.setArg(0, bufIn);
   err = oKernel.setArg(1, bufOut);

   cl::CommandQueue oQueue(oContext, oDevice);

   clock_t time = clock();

   err = oQueue.enqueueFillBuffer(bufIn, 1, 0, sizeof(int) * oVecNaturals.size());

   err = oQueue.enqueueNDRangeKernel(oKernel, cl::NullRange, cl::NDRange(oVecNaturals.size()));
   err = oQueue.enqueueReadBuffer(bufOut, CL_FALSE, 0, sizeof(int) * oVecNaturals.size(), oVecNaturals.data());

   cl::finish();

   time = clock() - time;

   std::ofstream fout;
   fout.open("../../Results/Results_Start10.txt", std::ofstream::out | std::ofstream::app);
   fout << "Processing time for " << ELEMENTS << " elements is: " << ((double)time) / CLOCKS_PER_SEC * 1000 << " miliseconds" << std::endl;
   fout.close();

   return 0;
}