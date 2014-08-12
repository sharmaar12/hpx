//  Copyright (c) 2014 Anuj R. Sharma
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include <hpx/hpx_main.hpp>
#include <hpx/components/vector/vector.hpp>
#include <hpx/util/lightweight_test.hpp>

void test_reserve_increase()
{
    //Vector created with 100 chunks and each chunk is having 1000 elements
    std::size_t num_chunks = 100, chunk_size = 1000, n = 100;
    hpx::vector v(num_chunks, chunk_size);
    std::size_t initial_capacity = v.capacity();

    v.reserve(v.capacity() + n);
    HPX_TEST(initial_capacity < v.capacity());

}// End of test_reserve_increase()

void test_reserve_async_increase()
{
    //Vector created with 100 chunks and each chunk is having 1000 elements
    std::size_t num_chunks = 100, chunk_size = 1000, n = 100;
    hpx::vector v(num_chunks, chunk_size);
    std::size_t initial_capacity = v.capacity();

    v.reserve_async(v.capacity() + n).get();
    HPX_TEST(initial_capacity < v.capacity());

}// End of test_reserve_async_increase()

void test_reserve_decrease()
{
    //Vector created with 100 chunks and each chunk is having 1000 elements
    std::size_t num_chunks = 100, chunk_size = 1000;
    hpx::vector v(num_chunks, chunk_size);
    std::size_t initial_capacity = v.capacity();

    v.reserve(100);
    HPX_TEST(initial_capacity == v.capacity());

}// End of test_reserve_increase()

void test_reserve_async_decrease()
{
    //Vector created with 100 chunks and each chunk is having 1000 elements
    std::size_t num_chunks = 100, chunk_size = 1000;
    hpx::vector v(num_chunks, chunk_size);
    std::size_t initial_capacity = v.capacity();

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
