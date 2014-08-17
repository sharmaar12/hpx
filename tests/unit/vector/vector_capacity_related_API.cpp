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

typedef std::size_t         size_type;

void test_size_and_size_async()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    size_type expected_total_elements;
    expected_total_elements = num_chunks * chunk_size;
    hpx::vector v(num_chunks, chunk_size);

    //Test both function calculate size correctly
    HPX_TEST_EQ(v.size(), expected_total_elements);
    HPX_TEST_EQ(v.size_async().get(), expected_total_elements);
    //Both calculate the same value (Redundant)
    HPX_TEST_EQ(v.size(), v.size_async().get());
}//End of test_size_and_size_async()

void test_max_size_and_max_size_async()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    size_type expected_total_elements;
    expected_total_elements = num_chunks * chunk_size;
    hpx::vector v(num_chunks, chunk_size);

    //Both calculate the same value
    HPX_TEST_EQ(v.max_size(), v.max_size_async().get());
}//End of test_max_size_and_max_size_async()

//  This is when the new size is greater than
void test_resize_helper_increase(size_type num_chunks,
                                 size_type initial_chunk_size,
                                 size_type new_chunk_size,
                                 VAL_TYPE val = (double)0)
{
    size_type initial_size = num_chunks * initial_chunk_size;
    hpx::vector v(num_chunks, initial_chunk_size, INITIAL_VALUE);

    if(val == (VAL_TYPE)0)
    {
        //Resize without default value
        v.resize(new_chunk_size);
    }
    else
        v.resize(new_chunk_size, val);

    size_type new_size = num_chunks * new_chunk_size;
    size_type count_old_ele = 0, count_new_ele = 0;

    for(size_type chunks = 0; chunks < num_chunks; ++chunks)
    {
        for(size_type i = 0; i < initial_chunk_size; ++i)
        {
            HPX_TEST_EQ(v[ (chunks * new_chunk_size) + i],
                        (VAL_TYPE) INITIAL_VALUE);
            ++count_old_ele;
        }
        for(size_type i = initial_chunk_size;
                i < new_chunk_size;
                ++i)
        {
            //NOTE: I have not resized using val if val == 0 (I.e. the value for
            // val is not provided) hence in case where val is not specified
            // this checks the newly created elements are initialized to val.
            HPX_TEST_EQ(v[ (chunks * new_chunk_size) + i],
                        (VAL_TYPE) val);
            ++count_new_ele;
        }
    }

    HPX_TEST_EQ(count_old_ele, initial_size);
    HPX_TEST_EQ(count_old_ele + count_new_ele, new_size);
    HPX_TEST_EQ(count_old_ele + count_new_ele, v.size());
}//End of test_resize_helper_increase()

//  This is when the new size is less than or equal
void test_resize_helper_decrease_equal(size_type num_chunks,
                                       size_type initial_chunk_size,
                                       size_type new_chunk_size,
                                       VAL_TYPE val = (double)0)
{
    size_type initial_size = num_chunks * initial_chunk_size;
    hpx::vector v(num_chunks, initial_chunk_size, INITIAL_VALUE);

    if(val == (VAL_TYPE)0)
    {
        //Resize without default value
        v.resize(new_chunk_size);
    }
    else
    {
        //This part does not make sense but we are still making sure that
        //it work correctly
        v.resize(new_chunk_size, val);
    }

    size_type new_size = num_chunks * new_chunk_size;
    size_type count_old_ele = 0;

    for(size_type chunks = 0; chunks < num_chunks; ++chunks)
    {
        for(size_type i = 0; i < new_chunk_size; ++i)
        {
            HPX_TEST_EQ(v[ (chunks * new_chunk_size) + i],
                        (VAL_TYPE) INITIAL_VALUE);
            ++count_old_ele;
        }
    }

    HPX_TEST_EQ(count_old_ele, new_size);
    HPX_TEST_EQ(count_old_ele, v.size());

    //When both are equal we have to test following
    if(initial_chunk_size == new_chunk_size)
    {
        HPX_TEST_EQ(initial_size, v.size());
        HPX_TEST_EQ(initial_size, count_old_ele);
    }
}//End of test_resize_helper_decrease_equal()

void test_resize()
{
    //When Size is going to increase
    test_resize_helper_increase(100, 1000, 10005);
    test_resize_helper_increase(100, 1000, 10005, 1241991);

    //When Size is going to decrease
    test_resize_helper_decrease_equal(100, 10005, 1000);
    test_resize_helper_decrease_equal(100, 10005, 1000, 1241991);

    //When Size is going to remain the same
    test_resize_helper_decrease_equal(100, 10005, 1000);
    test_resize_helper_decrease_equal(100, 10005, 1000, 1241991);
}//End of test_resize

void test_resize_exception()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    hpx::vector v(num_chunks, chunk_size);

    bool caught_exception = false;

    try
    {
        //resize without value
        v.resize(0);
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

    //Resetting the variable for further use
    caught_exception = false;
    try
    {
        //resize without value
        v.resize(0, 1991);
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

}// End of test_resize_exception()

//  This is when the new size is greater than
void test_resize_async_helper_increase(size_type num_chunks,
                                       size_type initial_chunk_size,
                                       size_type new_chunk_size,
                                       VAL_TYPE val = (double)0)
{
    size_type initial_size = num_chunks * initial_chunk_size;
    hpx::vector v(num_chunks, initial_chunk_size, INITIAL_VALUE);

    if(val == (VAL_TYPE)0)
    {
        //Resize without default value
        v.resize_async(new_chunk_size).get();
    }
    else
        v.resize_async(new_chunk_size, val).get();

    size_type new_size = num_chunks * new_chunk_size;
    size_type count_old_ele = 0, count_new_ele = 0;

    for(size_type chunks = 0; chunks < num_chunks; ++chunks)
    {
        for(size_type i = 0; i < initial_chunk_size; ++i)
        {
            HPX_TEST_EQ(v[ (chunks * new_chunk_size) + i],
                        (VAL_TYPE) INITIAL_VALUE);
            ++count_old_ele;
        }
        for(size_type i = initial_chunk_size;
                i < new_chunk_size;
                ++i)
        {
            //NOTE: I have not resized using val if val == 0 (I.e. the value for
            // val is not provided) hence in case where val is not specified
            // this checks the newly created elements are initialized to val.
            HPX_TEST_EQ(v[ (chunks * new_chunk_size) + i],
                        (VAL_TYPE) val);
            ++count_new_ele;
        }
    }

    HPX_TEST_EQ(count_old_ele, initial_size);
    HPX_TEST_EQ(count_old_ele + count_new_ele, new_size);
    HPX_TEST_EQ(count_old_ele + count_new_ele, v.size());
}//End of test_resize_async_helper_increase()

//  This is when the new size is less than or equal
void test_resize_async_helper_decrease_equal(size_type num_chunks,
                                             size_type initial_chunk_size,
                                             size_type new_chunk_size,
                                             VAL_TYPE val = (double)0)
{
    size_type initial_size = num_chunks * initial_chunk_size;
    hpx::vector v(num_chunks, initial_chunk_size, INITIAL_VALUE);

    if(val == (VAL_TYPE)0)
    {
        //Resize without default value
        v.resize_async(new_chunk_size).get();
    }
    else
    {
        //This part does not make sense but we are still making sure that
        //it work correctly
        v.resize_async(new_chunk_size, val).get();
    }

    size_type new_size = num_chunks * new_chunk_size;
    size_type count_old_ele = 0;

    for(size_type chunks = 0; chunks < num_chunks; ++chunks)
    {
        for(size_type i = 0; i < new_chunk_size; ++i)
        {
            HPX_TEST_EQ(v[ (chunks * new_chunk_size) + i],
                        (VAL_TYPE) INITIAL_VALUE);
            ++count_old_ele;
        }
    }

    HPX_TEST_EQ(count_old_ele, new_size);
    HPX_TEST_EQ(count_old_ele, v.size());

    //When both are equal we have to test following
    if(initial_chunk_size == new_chunk_size)
    {
        HPX_TEST_EQ(initial_size, v.size());
        HPX_TEST_EQ(initial_size, count_old_ele);
    }
}//End of test_resize_helper_decrease_equal()

void test_resize_async()
{
    //When Size is going to increase
    test_resize_async_helper_increase(100, 1000, 10005);
    test_resize_async_helper_increase(100, 1000, 10005, 1241991);

    //When Size is going to decrease
    test_resize_async_helper_decrease_equal(100, 10005, 1000);
    test_resize_async_helper_decrease_equal(100, 10005, 1000, 1241991);

    //When Size is going to remain the same
    test_resize_async_helper_decrease_equal(100, 10005, 1000);
    test_resize_async_helper_decrease_equal(100, 10005, 1000, 1241991);
}//End of test_resize_async()

void test_resize_async_exception()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    hpx::vector v(num_chunks, chunk_size);

    bool caught_exception = false;

    try
    {
        //resize without value
        v.resize_async(0).get();
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

    //Resetting the variable for further use
    caught_exception = false;
    try
    {
        //resize without value
        v.resize_async(0, 1991).get();
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

}// End of test_resize_async_exception()


void test_capacity_and_capacity_async()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    size_type expected_total_elements;
    expected_total_elements = num_chunks * chunk_size;
    hpx::vector v(num_chunks, chunk_size);

    //Both calculate the same value
    HPX_TEST_EQ(v.capacity(), v.capacity_async().get());
}//End of test_capacity_and_capacity_async()

void test_empty_and_empty_async()
{
    //Created Empty vector
    hpx::vector v_empty;
    HPX_TEST(v_empty.empty());
    HPX_TEST(v_empty.empty_async().get());

    //Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    hpx::vector v(num_chunks, chunk_size);
    HPX_TEST_EQ(false, v.empty());
    HPX_TEST_EQ(false, v.empty_async().get());
}//End of test_empty_and_empty_async()

int main()
{
    try
    {
        test_size_and_size_async();
        test_max_size_and_max_size_async();
        test_resize();
        test_resize_exception();
        test_resize_async();
        test_resize_async_exception();
        test_capacity_and_capacity_async();
        test_empty_and_empty_async();
    }
    catch(...)
    {
        //  Something went wrong in the program
        HPX_TEST(false);
    }

    return hpx::util::report_errors();
}

