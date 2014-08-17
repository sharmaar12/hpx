//  Copyright (c) 2014 Anuj R. Sharma
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include <hpx/hpx_main.hpp>
#include <hpx/components/vector/vector.hpp>
#include <hpx/util/lightweight_test.hpp>

#define INITIAL_NUM_CHUNKS 100
#define INITIAL_CHUNK_SIZE 10005

typedef std::size_t		size_type;

void test_reserve_increase()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having 
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    size_type n = 100;
    hpx::vector v(num_chunks, chunk_size);
    size_type initial_capacity = v.capacity();

    v.reserve(v.capacity() + n);
    HPX_TEST(initial_capacity < v.capacity());

}// End of test_reserve_increase()

void test_reserve_async_increase()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having 
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    size_type n = 100;
    hpx::vector v(num_chunks, chunk_size);
    size_type initial_capacity = v.capacity();

    v.reserve_async(v.capacity() + n).get();
    HPX_TEST(initial_capacity < v.capacity());

}// End of test_reserve_async_increase()

void test_reserve_decrease()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having 
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    hpx::vector v(num_chunks, chunk_size);
    size_type initial_capacity = v.capacity();

    v.reserve(100);
    HPX_TEST(initial_capacity == v.capacity());

}// End of test_reserve_increase()

void test_reserve_async_decrease()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having 
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    hpx::vector v(num_chunks, chunk_size);
    size_type initial_capacity = v.capacity();

    v.reserve_async(100).get();
    HPX_TEST(initial_capacity == v.capacity());

}// End of test_reserve_async_increase()


int main()
{
    try
    {
        test_reserve_increase();
        test_reserve_async_increase();
        test_reserve_decrease();
        test_reserve_async_decrease();
    }
    catch(...)
    {
        //  Something went wrong in the program
        HPX_TEST(false);
    }

    return hpx::util::report_errors();
}
