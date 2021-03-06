#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <random>

#include "../qikkDB/QueryEngine/Context.h"
#include "../qikkDB/QueryEngine/GPUCore/GPUBinary.cuh"
#include "../qikkDB/QueryEngine/GPUCore/GPUMemory.cuh"
#include "../qikkDB/QueryEngine/GPUCore/cuda_ptr.h"
#include "../qikkDB/QueryEngine/NullConstants.cuh"
#include "gtest/gtest.h"


// Count of the testing data elements:
const int32_t DATA_ELEMENT_COUNT = 1 << 18;

template <typename T>
void testColColArithmetic()
{
    // CPU data:
    std::unique_ptr<T[]> inputDataA = std::make_unique<T[]>(DATA_ELEMENT_COUNT);
    std::unique_ptr<T[]> inputDataB = std::make_unique<T[]>(DATA_ELEMENT_COUNT);
    std::unique_ptr<T[]> outputData = std::make_unique<T[]>(DATA_ELEMENT_COUNT);

    // Fill input data buffers:
    std::default_random_engine generator;
    if (std::is_integral<T>::value)
    {
        std::uniform_int_distribution<int32_t> distributionInt(-1024, 1024);
        for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
        {
            inputDataA[i] = distributionInt(generator);
            inputDataB[i] = distributionInt(generator);
            if (inputDataB[i] == 0)
            {
                inputDataB[i] = 1;
            }
        }
    }
    else
    {
        std::uniform_real_distribution<float> distributionFloat(-1024.0f, 1024.0f);
        for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
        {
            inputDataA[i] = distributionFloat(generator);
            inputDataB[i] = distributionFloat(generator);
            if (inputDataB[i] == 0)
            {
                inputDataB[i] = 1;
            }
        }
    }


    // Create CUDA buffers:
    T* inputBufferA;
    T* inputBufferB;
    T* outputBuffer;

    // Alloc buffers in GPU memory:
    GPUMemory::alloc(&inputBufferA, DATA_ELEMENT_COUNT);
    GPUMemory::alloc(&inputBufferB, DATA_ELEMENT_COUNT);
    GPUMemory::alloc(&outputBuffer, DATA_ELEMENT_COUNT);

    // Copy the contents of the buffers to the GPU
    GPUMemory::copyHostToDevice(inputBufferA, inputDataA.get(), DATA_ELEMENT_COUNT);
    GPUMemory::copyHostToDevice(inputBufferB, inputDataB.get(), DATA_ELEMENT_COUNT);

    //////////////////////////////////////////////////////////////////////////////////////
    // Run kernels, copy back values and compare them

    // Add
    GPUBinary<ArithmeticOperations::add, T, T*, T*>::Binary(outputBuffer, inputBufferA,
                                                            inputBufferB, DATA_ELEMENT_COUNT);
    GPUMemory::copyDeviceToHost(outputData.get(), outputBuffer, DATA_ELEMENT_COUNT);
    for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
    {
        if (std::is_integral<T>::value)
        {
            ASSERT_EQ(outputData[i], inputDataA[i] + inputDataB[i]);
        }
        else
        {
            ASSERT_FLOAT_EQ(outputData[i], inputDataA[i] + inputDataB[i]);
        }
    }

    // Sub
    GPUBinary<ArithmeticOperations::sub, T, T*, T*>::Binary(outputBuffer, inputBufferA,
                                                            inputBufferB, DATA_ELEMENT_COUNT);
    GPUMemory::copyDeviceToHost(outputData.get(), outputBuffer, DATA_ELEMENT_COUNT);
    for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
    {
        if (std::is_integral<T>::value)
        {
            ASSERT_EQ(outputData[i], inputDataA[i] - inputDataB[i]);
        }
        else
        {
            ASSERT_FLOAT_EQ(outputData[i], inputDataA[i] - inputDataB[i]);
        }
    }

    // Mul
    GPUBinary<ArithmeticOperations::mul, T, T*, T*>::Binary(outputBuffer, inputBufferA,
                                                            inputBufferB, DATA_ELEMENT_COUNT);
    GPUMemory::copyDeviceToHost(outputData.get(), outputBuffer, DATA_ELEMENT_COUNT);
    for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
    {
        if (std::is_integral<T>::value)
        {
            ASSERT_EQ(outputData[i], inputDataA[i] * inputDataB[i]);
        }
        else
        {
            ASSERT_FLOAT_EQ(outputData[i], inputDataA[i] * inputDataB[i]);
        }
    }

    // Div
    GPUBinary<ArithmeticOperations::div, T, T*, T*>::Binary(outputBuffer, inputBufferA,
                                                            inputBufferB, DATA_ELEMENT_COUNT);
    GPUMemory::copyDeviceToHost(outputData.get(), outputBuffer, DATA_ELEMENT_COUNT);
    for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
    {
        if (std::is_integral<T>::value)
        {
            if (inputDataB[i] != 0)
            {
                ASSERT_EQ(outputData[i], static_cast<T>(inputDataA[i] / inputDataB[i]));
            }
        }
        else
        {
            if (inputDataB[i] != 0)
            {
                ASSERT_FLOAT_EQ(outputData[i], static_cast<T>(inputDataA[i] / inputDataB[i]));
            }
        }
    }

    // Modulus
    if (std::is_integral<T>::value)
    {
        GPUBinary<ArithmeticOperations::mod, T, T*, T*>::Binary(outputBuffer, inputBufferA,
                                                                inputBufferB, DATA_ELEMENT_COUNT);
        GPUMemory::copyDeviceToHost(outputData.get(), outputBuffer, DATA_ELEMENT_COUNT);
        for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
        {
            if (inputDataB[i] != 0)
            {
                ASSERT_EQ(outputData[i], inputDataA[i] % inputDataB[i]);
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////

    // Free buffers in GPU memory:
    GPUMemory::free(inputBufferA);
    GPUMemory::free(inputBufferB);
    GPUMemory::free(outputBuffer);
}

template <>
void testColColArithmetic<float>()
{
    // CPU data:
    std::unique_ptr<float[]> inputDataA = std::make_unique<float[]>(DATA_ELEMENT_COUNT);
    std::unique_ptr<float[]> inputDataB = std::make_unique<float[]>(DATA_ELEMENT_COUNT);
    std::unique_ptr<float[]> outputData = std::make_unique<float[]>(DATA_ELEMENT_COUNT);

    // Fill input data buffers:
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distributionFloat(-1024.0f, 1024.0f);
    for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
    {
        inputDataA[i] = distributionFloat(generator);
        inputDataB[i] = distributionFloat(generator);
        if (inputDataB[i] == 0)
        {
            inputDataB[i] = 1;
        }
    }

    // Create CUDA buffers:
    float* inputBufferA;
    float* inputBufferB;
    float* outputBuffer;

    // Alloc buffers in GPU memory:
    GPUMemory::alloc(&inputBufferA, DATA_ELEMENT_COUNT);
    GPUMemory::alloc(&inputBufferB, DATA_ELEMENT_COUNT);
    GPUMemory::alloc(&outputBuffer, DATA_ELEMENT_COUNT);

    // Copy the contents of the buffers to the GPU
    GPUMemory::copyHostToDevice(inputBufferA, inputDataA.get(), DATA_ELEMENT_COUNT);
    GPUMemory::copyHostToDevice(inputBufferB, inputDataB.get(), DATA_ELEMENT_COUNT);

    //////////////////////////////////////////////////////////////////////////////////////
    // Run kernels, copy back values and compare them

    // Add
    GPUBinary<ArithmeticOperations::add, float, float*, float*>::Binary(outputBuffer, inputBufferA,
                                                                        inputBufferB, DATA_ELEMENT_COUNT);
    GPUMemory::copyDeviceToHost(outputData.get(), outputBuffer, DATA_ELEMENT_COUNT);
    for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
    {
        ASSERT_FLOAT_EQ(outputData[i], inputDataA[i] + inputDataB[i]);
    }

    // Sub
    GPUBinary<ArithmeticOperations::sub, float, float*, float*>::Binary(outputBuffer, inputBufferA,
                                                                        inputBufferB, DATA_ELEMENT_COUNT);
    GPUMemory::copyDeviceToHost(outputData.get(), outputBuffer, DATA_ELEMENT_COUNT);
    for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
    {
        ASSERT_FLOAT_EQ(outputData[i], inputDataA[i] - inputDataB[i]);
    }

    // Mul
    GPUBinary<ArithmeticOperations::mul, float, float*, float*>::Binary(outputBuffer, inputBufferA,
                                                                        inputBufferB, DATA_ELEMENT_COUNT);
    GPUMemory::copyDeviceToHost(outputData.get(), outputBuffer, DATA_ELEMENT_COUNT);
    for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
    {
        ASSERT_FLOAT_EQ(outputData[i], inputDataA[i] * inputDataB[i]);
    }

    // Div
    GPUBinary<ArithmeticOperations::div, float, float*, float*>::Binary(outputBuffer, inputBufferA,
                                                                        inputBufferB, DATA_ELEMENT_COUNT);
    GPUMemory::copyDeviceToHost(outputData.get(), outputBuffer, DATA_ELEMENT_COUNT);
    for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
    {
        if (inputDataB[i] != 0)
        {
            ASSERT_FLOAT_EQ(outputData[i], static_cast<float>(inputDataA[i] / inputDataB[i]));
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////

    // Free buffers in GPU memory:
    GPUMemory::free(inputBufferA);
    GPUMemory::free(inputBufferB);
    GPUMemory::free(outputBuffer);
}

TEST(GPUArithmeticTests, ArithmeticsColCol)
{
    // Initialize CUDA context:
    Context::getInstance();

    testColColArithmetic<int32_t>();
    testColColArithmetic<int64_t>();
    testColColArithmetic<float>();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
void testColConstArithmetic()
{
    // CPU data:
    std::unique_ptr<T[]> inputDataA = std::make_unique<T[]>(DATA_ELEMENT_COUNT);
    T inputDataBConst;
    std::unique_ptr<T[]> outputData = std::make_unique<T[]>(DATA_ELEMENT_COUNT);

    // Fill input data buffers:
    std::default_random_engine generator;
    if (std::is_integral<T>::value)
    {
        std::uniform_int_distribution<int32_t> distributionInt(-1024, 1024);
        for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
        {
            inputDataA[i] = distributionInt(generator);
        }
        inputDataBConst = distributionInt(generator);
        if (inputDataBConst == 0)
        {
            inputDataBConst = 1;
        }
    }
    else
    {
        std::uniform_real_distribution<float> distributionFloat(-1024.0f, 1024.0f);
        for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
        {
            inputDataA[i] = distributionFloat(generator);
        }
        inputDataBConst = distributionFloat(generator);
        if (inputDataBConst == 0)
        {
            inputDataBConst = 1;
        }
    }


    // Create CUDA buffers:
    T* inputBufferA;
    T* outputBuffer;

    // Alloc buffers in GPU memory:
    GPUMemory::alloc(&inputBufferA, DATA_ELEMENT_COUNT);
    GPUMemory::alloc(&outputBuffer, DATA_ELEMENT_COUNT);

    // Copy the contents of the buffers to the GPU
    GPUMemory::copyHostToDevice(inputBufferA, inputDataA.get(), DATA_ELEMENT_COUNT);

    //////////////////////////////////////////////////////////////////////////////////////
    // Run kernels, copy back values and compare them

    // Add
    GPUBinary<ArithmeticOperations::add, T, T*, T>::Binary(outputBuffer, inputBufferA,
                                                           inputDataBConst, DATA_ELEMENT_COUNT);
    GPUMemory::copyDeviceToHost(outputData.get(), outputBuffer, DATA_ELEMENT_COUNT);
    for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
    {
        if (std::is_integral<T>::value)
        {
            ASSERT_EQ(outputData[i], inputDataA[i] + inputDataBConst);
        }
        else
        {
            ASSERT_FLOAT_EQ(outputData[i], inputDataA[i] + inputDataBConst);
        }
    }

    // Sub
    GPUBinary<ArithmeticOperations::sub, T, T*, T>::Binary(outputBuffer, inputBufferA,
                                                           inputDataBConst, DATA_ELEMENT_COUNT);
    GPUMemory::copyDeviceToHost(outputData.get(), outputBuffer, DATA_ELEMENT_COUNT);
    for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
    {
        if (std::is_integral<T>::value)
        {
            ASSERT_EQ(outputData[i], inputDataA[i] - inputDataBConst);
        }
        else
        {
            ASSERT_FLOAT_EQ(outputData[i], inputDataA[i] - inputDataBConst);
        }
    }

    // Mul
    GPUBinary<ArithmeticOperations::mul, T, T*, T>::Binary(outputBuffer, inputBufferA,
                                                           inputDataBConst, DATA_ELEMENT_COUNT);
    GPUMemory::copyDeviceToHost(outputData.get(), outputBuffer, DATA_ELEMENT_COUNT);
    for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
    {
        if (std::is_integral<T>::value)
        {
            ASSERT_EQ(outputData[i], inputDataA[i] * inputDataBConst);
        }
        else
        {
            ASSERT_FLOAT_EQ(outputData[i], inputDataA[i] * inputDataBConst);
        }
    }

    // Div
    GPUBinary<ArithmeticOperations::div, T, T*, T>::Binary(outputBuffer, inputBufferA,
                                                           inputDataBConst, DATA_ELEMENT_COUNT);
    GPUMemory::copyDeviceToHost(outputData.get(), outputBuffer, DATA_ELEMENT_COUNT);
    for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
    {
        if (std::is_integral<T>::value)
        {
            if (inputDataBConst != 0)
            {
                ASSERT_EQ(outputData[i], static_cast<T>(inputDataA[i] / inputDataBConst));
            }
        }
        else
        {
            if (inputDataBConst != 0)
            {
                ASSERT_FLOAT_EQ(outputData[i], static_cast<T>(inputDataA[i] / inputDataBConst));
            }
        }
    }

    // Modulus
    if (std::is_integral<T>::value)
    {
        GPUBinary<ArithmeticOperations::mod, T, T*, T>::Binary(outputBuffer, inputBufferA,
                                                               inputDataBConst, DATA_ELEMENT_COUNT);
        GPUMemory::copyDeviceToHost(outputData.get(), outputBuffer, DATA_ELEMENT_COUNT);
        for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
        {
            if (inputDataBConst != 0)
            {
                ASSERT_EQ(outputData[i], inputDataA[i] % inputDataBConst);
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////

    // Free buffers in GPU memory:
    GPUMemory::free(inputBufferA);
    GPUMemory::free(outputBuffer);
}

template <>
void testColConstArithmetic<float>()
{
    // CPU data:
    std::unique_ptr<float[]> inputDataA = std::make_unique<float[]>(DATA_ELEMENT_COUNT);
    float inputDataBConst;
    std::unique_ptr<float[]> outputData = std::make_unique<float[]>(DATA_ELEMENT_COUNT);

    // Fill input data buffers:
    std::default_random_engine generator;
    if (std::is_integral<float>::value)
    {
        std::uniform_int_distribution<int32_t> distributionInt(-1024, 1024);
        for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
        {
            inputDataA[i] = distributionInt(generator);
        }
        inputDataBConst = distributionInt(generator);
    }
    else
    {
        std::uniform_real_distribution<float> distributionFloat(-1024.0f, 1024.0f);
        for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
        {
            inputDataA[i] = distributionFloat(generator);
        }
        inputDataBConst = distributionFloat(generator);
    }


    // Create CUDA buffers:
    float* inputBufferA;
    float* outputBuffer;

    // Alloc buffers in GPU memory:
    GPUMemory::alloc(&inputBufferA, DATA_ELEMENT_COUNT);
    GPUMemory::alloc(&outputBuffer, DATA_ELEMENT_COUNT);

    // Copy the contents of the buffers to the GPU
    GPUMemory::copyHostToDevice(inputBufferA, inputDataA.get(), DATA_ELEMENT_COUNT);

    //////////////////////////////////////////////////////////////////////////////////////
    // Run kernels, copy back values and compare them

    // Add
    GPUBinary<ArithmeticOperations::add, float, float*, float>::Binary(outputBuffer, inputBufferA,
                                                                       inputDataBConst, DATA_ELEMENT_COUNT);
    GPUMemory::copyDeviceToHost(outputData.get(), outputBuffer, DATA_ELEMENT_COUNT);
    for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
    {
        ASSERT_FLOAT_EQ(outputData[i], inputDataA[i] + inputDataBConst);
    }

    // Sub
    GPUBinary<ArithmeticOperations::sub, float, float*, float>::Binary(outputBuffer, inputBufferA,
                                                                       inputDataBConst, DATA_ELEMENT_COUNT);
    GPUMemory::copyDeviceToHost(outputData.get(), outputBuffer, DATA_ELEMENT_COUNT);
    for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
    {
        ASSERT_FLOAT_EQ(outputData[i], inputDataA[i] - inputDataBConst);
    }

    // Mul
    GPUBinary<ArithmeticOperations::mul, float, float*, float>::Binary(outputBuffer, inputBufferA,
                                                                       inputDataBConst, DATA_ELEMENT_COUNT);
    GPUMemory::copyDeviceToHost(outputData.get(), outputBuffer, DATA_ELEMENT_COUNT);
    for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
    {
        ASSERT_FLOAT_EQ(outputData[i], inputDataA[i] * inputDataBConst);
    }

    // Div
    GPUBinary<ArithmeticOperations::div, float, float*, float>::Binary(outputBuffer, inputBufferA,
                                                                       inputDataBConst, DATA_ELEMENT_COUNT);
    GPUMemory::copyDeviceToHost(outputData.get(), outputBuffer, DATA_ELEMENT_COUNT);
    for (int i = 0; i < DATA_ELEMENT_COUNT; i++)
    {
        if (inputDataBConst != 0)
        {
            ASSERT_FLOAT_EQ(outputData[i], static_cast<float>(inputDataA[i] / inputDataBConst));
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////

    // Free buffers in GPU memory:
    GPUMemory::free(inputBufferA);
    GPUMemory::free(outputBuffer);
}

TEST(GPUArithmeticTests, ArithmeticsColConst)
{
    // Initialize CUDA context:
    Context::getInstance();

    testColConstArithmetic<int32_t>();
    testColConstArithmetic<int64_t>();
    testColConstArithmetic<float>();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename OP, typename T>
void testErrors(std::vector<T> inputDataA, std::vector<T> inputDataB, std::vector<T> correctResult, QueryEngineErrorType errorType)
{
    int32_t size = std::min(inputDataA.size(), inputDataB.size());
    ASSERT_EQ(correctResult.size(), size) << "Input data and result sizes are not equal";
    std::unique_ptr<T[]> hostResult = std::make_unique<T[]>(size);
    cuda_ptr<T> deviceDataA(size);
    cuda_ptr<T> deviceDataB(size);
    cuda_ptr<T> deviceResult(size);
    GPUMemory::copyHostToDevice(deviceDataA.get(), inputDataA.data(), size);
    GPUMemory::copyHostToDevice(deviceDataB.get(), inputDataB.data(), size);

    auto lambda = [&]() {
        try
        {
            GPUBinary<OP, T, T*, T*>::Binary(deviceResult.get(), deviceDataA.get(), deviceDataB.get(), size);
        }
        catch (query_engine_error& err)
        {
            EXPECT_EQ(err.GetQueryEngineError(), errorType)
                << "Query Engine Error that shouldn't be thrown";
            throw;
        }
    };

    EXPECT_THROW({ lambda(); }, query_engine_error);

    GPUMemory::copyDeviceToHost(hostResult.get(), deviceResult.get(), size);
    for (int i = 0; i < size; i++)
    {
        ASSERT_EQ(hostResult[i], correctResult[i]) << "Value at [" << i << "] does not match";
    }
}

TEST(GPUArithmeticTests, DivisionByZero)
{
    // Initialize CUDA context:
    Context::getInstance();

    testErrors<ArithmeticOperations::div, int32_t>({1, 8, 1, -1, 0, -1}, {1, 4, 0, 0, 0, 1},
                                                   {1, 2, GetNullConstant<int32_t>(), GetNullConstant<int32_t>(),
                                                    GetNullConstant<int32_t>(), -1},
                                                   QueryEngineErrorType::GPU_DIVISION_BY_ZERO_ERROR);
}

TEST(GPUArithmeticTests, AddOverflow)
{
    // Initialize CUDA context:
    Context::getInstance();

    testErrors<ArithmeticOperations::add, int32_t>({1, 2147483646, -2147483646, -1, 0, -1}, {1, 10, -10, -1},
                                                   {2, GetNullConstant<int32_t>(),
                                                    GetNullConstant<int32_t>(), -2},
                                                   QueryEngineErrorType::GPU_INTEGER_OVERFLOW_ERROR);
}

TEST(GPUArithmeticTests, MulOverflow)
{
    // Initialize CUDA context:
    Context::getInstance();

    testErrors<ArithmeticOperations::mul, int32_t>({1, 65536, -65536, -1, 0, -1}, {1, 65536, 65536, -1},
                                                   {1, GetNullConstant<int32_t>(),
                                                    GetNullConstant<int32_t>(), 1},
                                                   QueryEngineErrorType::GPU_INTEGER_OVERFLOW_ERROR);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename OP, typename T>
void testCustomFunctions(std::vector<T> inputDataA, std::vector<T> inputDataB, std::vector<T> correctResult)
{
    int32_t size = std::min(inputDataA.size(), inputDataB.size());
    std::unique_ptr<T[]> hostResult = std::make_unique<T[]>(size);
    cuda_ptr<T> deviceDataA(size);
    cuda_ptr<T> deviceDataB(size);
    cuda_ptr<T> deviceResult(size);
    GPUMemory::copyHostToDevice(deviceDataA.get(), inputDataA.data(), size);
    GPUMemory::copyHostToDevice(deviceDataB.get(), inputDataB.data(), size);

    GPUBinary<OP, T, T*, T*>::Binary(deviceResult.get(), deviceDataA.get(), deviceDataB.get(), size);

    GPUMemory::copyDeviceToHost(hostResult.get(), deviceResult.get(), size);
    for (int i = 0; i < size; i++)
    {
        EXPECT_FLOAT_EQ(hostResult[i], correctResult[i]) << "Value at [" << i << "] does not match";
    }
}

TEST(GPUArithmeticTests, GeoCoordinatesArithmeticConversions)
{
    // Initialize CUDA context:
    Context::getInstance();

    testCustomFunctions<ArithmeticOperations::geoLongitudeToTileX, float>({18.1, -150.0, 0.0, 180.0, 10.0},
                                                                          {18, 18, 10, 1, -1},
                                                                          {144252, 21845, 512, 2, 0});

    testCustomFunctions<ArithmeticOperations::geoLatitudeToTileY, float>({18.1, -85.0, 0.0, 85.0, 10.0},
                                                                         {18, 18, 10, 1, -1},
                                                                         {117667, 261714, 512, 0, 0});

    testCustomFunctions<ArithmeticOperations::geoTileXToLongitude, float>({144252, 21845, 512, 2, 0},
                                                                          {18, 18, 10, 1, 1},
                                                                          {18.099997f, -150.000457f,
                                                                           0.0f, 180.0f, -180.0f});

    testCustomFunctions<ArithmeticOperations::geoTileYToLatitude, float>(
        {117667, 261714, 512, 0, 1}, {18, 18, 10, 1, 1}, {18.100161f, -84.999924f, 0, 85.051128f, 0.0f});
}
