__kernel void Start10(__global int* dataIn, __global int* dataOut)
{
   dataOut[get_global_id(0)] = dataIn[get_global_id(0)] * 2;
}