#pragma once

#include <cstdint>

#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "GPUMemory.cuh"

#include "../../NativeGeoPoint.h"
#include "../Context.h"

namespace PolygonFunctions
{
struct polyIntersect
{
    __device__ __host__ void operator()() const
    {
    }
};

struct polyUnion
{
    __device__ __host__ void operator()() const
    {
    }
};
} // namespace PolygonFunctions

// Struct for the polygon Doubly Linked List construction on the GPU
__device__ struct PolygonNodeDLL
{
    int32_t poly_group;

    NativeGeoPoint point;

    float linear_distance;
    int32_t is_intersect;

    int32_t next;
    int32_t prev;
    int32_t cross_link;
};

// Data buffers for doubly linked lists of polygons during clipping
__device__ PolygonNodeDLL* poly1DLList;
__device__ PolygonNodeDLL* poly2DLList;

// Doubly linked list starts offset
__device__ int32_t* poly1DLListOffset;
__device__ int32_t* poly2DLListOffset;

// A kernel for counting the number of vertices that a complex polygon has
inline __global__ void kernel_calculate_points_in_complex_polygon_count(int32_t* pointCounts,
                                                             GPUMemory::GPUPolygon complexPolygon,
                                                             int32_t dataElementCount)
{
    const int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    const int32_t stride = blockDim.x * gridDim.x;

    for (int32_t i = idx; i < dataElementCount; i += stride)
    {
        // Sum the number of points of a polygon and store it in a buffer
        int32_t vertexCountSum = 0;
        for (int32_t j = 0; j < complexPolygon.polyCount[i]; j++)
        {
            vertexCountSum += complexPolygon.pointCount[complexPolygon.polyIdx[i] + j];
        }
        pointCounts[i] = vertexCountSum;
    }
}

template <typename OP>
__global__ void kernel_polygon_clipping(GPUMemory::GPUPolygon complexPolygonOut,
                                        GPUMemory::GPUPolygon complexPolygon1,
                                        GPUMemory::GPUPolygon complexPolygon2,
                                        int32_t dataElementCount)
{
}

class GPUPolygonClip
{
public:
    template <typename OP>
    static void
    ColCol(GPUMemory::GPUPolygon polygonOut, GPUMemory::GPUPolygon polygon1, GPUMemory::GPUPolygon polygon2, int32_t dataElementCount)
    {
        // Get the context instance
        Context& context = Context::getInstance();

        // Precalcualte the maximal needed size for a doubly linked list as
        // n*k + n + k where n is the number of vertices of polygon 1 and k is the number of
        // vertices of polygon 2 This is a case for one row - for all rows this has to be done
        // dataElementCount times Offsets for the linked list start indexes are needed too - these
        // can be calculated as the prefix sum of doubly linked list sizes The result size of the
        // doubly linked list buffer is a sum of all dll buffers

        // The offset buffers - fill them with the prefix sum of input polygon vertices
        // Alloc the offset buffs
        GPUMemory::allocAndSet(&poly1DLListOffset, 0, dataElementCount);
        GPUMemory::allocAndSet(&poly2DLListOffset, 0, dataElementCount);

        // Calculate the number of vertices in the complexpolygons
        kernel_calculate_points_in_complex_polygon_count<<<context.calcGridDim(dataElementCount),
                                                       context.getBlockDim()>>>(poly1DLListOffset, polygon1,
                                                                                dataElementCount);
        kernel_calculate_points_in_complex_polygon_count<<<context.calcGridDim(dataElementCount),
                                                       context.getBlockDim()>>>(poly2DLListOffset, polygon2,
                                                                                dataElementCount);
        // Transform the offset buffers using the prefix sum

        // Debug code - copy back the buffers
        int32_t result[1];
        GPUMemory::copyDeviceToHost(result, poly1DLListOffset, 1);


        // The data sandbox for linked lists - the max count of the vertices is the result of the prefix sum
        poly1DLList;
        poly2DLList;


        // Alloc space for the doubly linked lists for both polygons in both collumns
        GPUMemory::alloc(&poly1DLList, dataElementCount);
        GPUMemory::alloc(&poly1DLList, dataElementCount);


        kernel_polygon_clipping<OP>
            <<<context.calcGridDim(dataElementCount), context.getBlockDim()>>>(polygonOut, polygon1,
                                                                               polygon2, dataElementCount);
        QueryEngineError::setCudaError(cudaGetLastError());
    }
};
