/*
** A* Pathfinding
** Alen Smailovic
** 17/01/2018
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

   std::ifstream oFile("../../Kernels/AStarPathfinding.cl");
   std::string oSrc(std::istreambuf_iterator<char>(oFile), (std::istreambuf_iterator<char>()));

   cl::Program::Sources oSources(1, std::make_pair(oSrc.c_str(), oSrc.length() + 1));

   cl::Context oContext(oDevice);
   cl::Program oProgram(oContext, oSources);

   oProgram.build("-cl-std=CL1.2");

   return 0;
}

