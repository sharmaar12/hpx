//  Copyright (c) 2014 Anuj R. Sharma
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include <hpx/hpx_main.hpp>
#include <hpx/components/vector/vector.hpp>
#include <hpx/util/lightweight_test.hpp>

#define VAL_TYPE double
#define INITIAL_VALUE 124
#define INITIAL_NUM_CHUNKS 100
#define INITIAL_CHUNK_SIZE 10005

typedef std::size_t     size_type;

void test_assign(size_type initial_chunk_size,
                 size_type new_chunk_size,
                 VAL_TYPE val)
{
    size_type num_chunks = INITIAL_NUM_CHUNKS;
    size_type initial_vector_size = num_chunks * initial_chunk_size;
    size_type new_vector_size = num_chunks * new_chunk_size;
    hpx::vector v(num_chunks, initial_chunk_size, INITIAL_VALUE);

    v.assign(new_chunk_size, val);

    size_type count = 0;
    std::for_each(v.begin(), v.end(),
                  [&count] (VAL_TYPE val)
                  {
                    //Check all element is initialized to the val
                    HPX_TEST_EQ(val, (VAL_TYPE)val);
                    ++count;
                  });

    //  This is to check appropriate size change has happened
    HPX_TEST_EQ(count, new_vector_size);
    HPX_TEST_EQ(count, v.size());

    if(initial_chunk_size < new_chunk_size)
        HPX_TEST(initial_vector_size < v.size());
    else if(initial_chunk_size > new_chunk_size)
        HPX_TEST(initial_vector_size > v.size());
    else if(initial_chunk_size == new_chunk_size)
        HPX_TEST(initial_vector_size == v.size());
}//end of test_assign()

void test_assign_async(size_type initial_chunk_size,
                       size_type new_chunk_size,
                       VAL_TYPE val)
{
    size_type num_chunks = INITIAL_NUM_CHUNKS;
    size_type initial_vector_size = num_chunks * initial_chunk_size;
    size_type new_vector_size = num_chunks * new_chunk_size;
    hpx::vector v(num_chunks, initial_chunk_size, INITIAL_VALUE);

    v.assign_async(new_chunk_size, val).get();

    size_type count = 0;
    std::for_each(v.begin(), v.end(),
                  [&count] (VAL_TYPE val)
                  {
                    //Check all element is initialized to the val
                    HPX_TEST_EQ(val, (VAL_TYPE)val);
                    ++count;
                  });

    //  This is to check appropriate size change has happened
    HPX_TEST_EQ(count, new_vector_size);
    HPX_TEST_EQ(count, v.size());

    if(initial_chunk_size < new_chunk_size)
        HPX_TEST(initial_vector_size < v.size());
    else if(initial_chunk_size > new_chunk_size)
        HPX_TEST(initial_vector_size > v.size());
    else if(initial_chunk_size == new_chunk_size)
        HPX_TEST(initial_vector_size == v.size());
}//end of test_assign_async()

void test_assign_exception()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    hpx::vector v(num_chunks, chunk_size, INITIAL_VALUE);

    bool caught_exception = false;
    try
    {
        v.assign(0, 1241991);
        HPX_TEST(false);
    }
    catch(hpx::exception const& /*e*/)
    {
        caught_exception = true;
    }
    catch(...)
    {
        HPX_TEST(false);
    }
    HPX_TEST(caught_exception);
}//End of test_assign_exception()

void test_assign_async_exception()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    hpx::vector v(num_chunks, chunk_size, INITIAL_VALUE);

    bool caught_exception = false;
    try
    {
        v.assign_async(0, 1241991).get();
        HPX_TEST(false);
    }
    catch(hpx::exception const& /*e*/)
    {
        caught_exception = true;
    }
    catch(...)
    {
        HPX_TEST(false);
    }
    HPX_TEST(caught_exception);
}//End of test_assign_exception()

void test_push_back_and_push_back_async()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    size_type initial_vector_size;
    initial_vector_size = num_chunks * chunk_size;
    hpx::vector v(num_chunks, chunk_size);

    v.push_back((VAL_TYPE)1241991);

    HPX_TEST_EQ(v.back(), 1241991);
    size_type size_after_one_push_back = v.size();
    HPX_TEST(size_after_one_push_back > initial_vector_size);

    v.push_back_async((VAL_TYPE)124).get();
    HPX_TEST_EQ(v.back(), 124);
    HPX_TEST(v.size() > initial_vector_size);
    HPX_TEST(v.size() > size_after_one_push_back);
}//end of test_push_back_and_push_back_async()

int main()
{
    try
    {
        //  When vector size is increased
        test_assign(1000, 10005, 1241991);
        test_assign_async(1000, 10005, 1241991);

        //  When vector size is decreased
        test_assign(10005, 1000, 1241991);
        test_assign_async(10005, 1000, 1241991);

        //  When vector size remains the same
        test_assign(10005, 10005, 1241991);
        test_assign_async(10005, 10005, 1241991);

        // Test the Exceptions
        test_assign_exception();
        test_assign_async_exception();

        test_push_back_and_push_back_async();
    }
    catch(...)
    {
        //  Something went wrong in the program
        HPX_TEST(false);
    }

    return hpx::util::report_errors();
}

