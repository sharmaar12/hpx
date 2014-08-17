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

typedef std::size_t		size_type;

void test_get_value_and_set_value()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having 
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    size_type expected_total_elements;
    expected_total_elements = num_chunks * chunk_size;
    hpx::vector v(num_chunks, chunk_size);

    size_type count = 0;

    //Assigning value from 0,1,...,vec.size() - 1
    for(size_type i = 0; i < v.size(); ++i)
    {
        try{
            v.set_value(i, (VAL_TYPE)i);
            ++count;
        }
        catch(...){
            HPX_TEST_MSG(false,
                "Something went Wrong in test_get_value_and_set_value()");
        }
    }

    //To check for loop executed v.size() times
    HPX_TEST_EQ(v.size(), count);

    count = 0;
    //checking the values are correctly inserted and accessed by [] operator
    for(size_type i = 0; i < v.size(); ++i)
    {
        try{
            //Test the array subscript correctly assigning the value in correct order
            HPX_TEST_EQ(v.get_value(i), (VAL_TYPE)i );
            ++count;
        }
        catch(...)
        {
            HPX_TEST_MSG(false,
                "Something went Wrong in test_get_value_and_set_value()");
        }

    }
    //To check for loop executed v.size() times
    HPX_TEST_EQ(v.size(), count);
}//End of test_get_value_and_set_value()


void test_get_value_async_and_set_value_async()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having 
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    size_type expected_total_elements;
    expected_total_elements = num_chunks * chunk_size;
    hpx::vector v(num_chunks, chunk_size);

    size_type count = 0;

    //Assigning value from 0,1,...,vec.size() - 1
    for(size_type i = 0; i < v.size(); ++i)
    {
        try{
            v.set_value_async(i, (VAL_TYPE)i).get();
            ++count;
        }
        catch(...){
            HPX_TEST_MSG(false,
                "Something went Wrong in test_get_value_and_set_value()");
        }
    }

    //To check for loop executed v.size() times
    HPX_TEST_EQ(v.size(), count);

    count = 0;
    //checking the values are correctly inserted and accessed by [] operator
    for(size_type i = 0; i < v.size(); ++i)
    {
        try{
            //Test the array subscript correctly assigning the value in correct order
            HPX_TEST_EQ(v.get_value_async(i).get(), (VAL_TYPE)i );
            ++count;
        }
        catch(...)
        {
            HPX_TEST_MSG(false,
                "Something went Wrong in test_get_value_and_set_value()");
        }

    }
    //To check for loop executed v.size() times
    HPX_TEST_EQ(v.size(), count);
}//End of test_get_value_async_and_set_value_async()


void test_get_value_exception()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having 
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    size_type expected_total_elements;
    expected_total_elements = num_chunks * chunk_size;
    hpx::vector v(num_chunks, chunk_size, INITIAL_VALUE);

    //
    //Checking the get_value Exceptions
    //
    bool caught_exception = false;
    //Boundary Case 1
    try{
        //This should throw the exception
        v.get_value(-1);
        HPX_TEST(false);
    }
    catch(hpx::exception const& e)
    {
        caught_exception = true;
    }
    catch(...)
    {
        HPX_TEST(false);
    }
    HPX_TEST(caught_exception);

    //Boundary case 2
    try{
        //This should not throw the exception
        v.get_value(0);
        HPX_TEST(true);
    }
    catch(hpx::exception const& e)
    {
        HPX_TEST(false);
    }
    catch(...)
    {
        HPX_TEST(false);
    }

    //Reseting to the variable to use further
    caught_exception = false;

    //Boundary case 3
    try{
        //This should throw the exception
        // NOTE the pos starts from zero hence this throws
        v.get_value(v.size());
        HPX_TEST(false);
    }
    catch(hpx::exception const& e)
    {
        caught_exception = true;
    }
    catch(...)
    {
        HPX_TEST(false);
    }
    HPX_TEST(caught_exception);

    //Boundary case 4
    try{
        //This should not throw the exception
        // NOTE the pos starts from zero
        v.get_value(v.size() - 1);
        HPX_TEST(true);
    }
    catch(hpx::exception const& e)
    {
        HPX_TEST(false);
    }
    catch(...)
    {
        HPX_TEST(false);
    }

    //Check exception for random position outside the range
    try{
        //This should throw the exception
        v.get_value(v.size() + 1991);
        HPX_TEST(false);
    }
    catch(hpx::exception const& e)
    {
        caught_exception = true;
    }
    catch(...)
    {
        HPX_TEST(false);
    }
    HPX_TEST(caught_exception);

}// End of test_get_value_exception()

void test_get_value_async_exception()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having 
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    size_type expected_total_elements;
    expected_total_elements = num_chunks * chunk_size;
    hpx::vector v(num_chunks, chunk_size, INITIAL_VALUE);

    //
    //Checking the get_value Exceptions
    //
    bool caught_exception = false;
    //Boundary Case 1
    try{
        //This should throw the exception
        v.get_value_async(-1).get();
        HPX_TEST(false);
    }
    catch(hpx::exception const& e)
    {
        caught_exception = true;
    }
    catch(...)
    {
        HPX_TEST(false);
    }
    HPX_TEST(caught_exception);

    //Boundary case 2
    try{
        //This should not throw the exception
        v.get_value_async(0).get();
        HPX_TEST(true);
    }
    catch(hpx::exception const& e)
    {
        HPX_TEST(false);
    }
    catch(...)
    {
        HPX_TEST(false);
    }

    //Reseting to the variable to use further
    caught_exception = false;

    //Boundary case 3
    try{
        //This should throw the exception
        // NOTE the pos starts from zero hence this throws
        v.get_value_async(v.size()).get();
        HPX_TEST(false);
    }
    catch(hpx::exception const& e)
    {
        caught_exception = true;
    }
    catch(...)
    {
        HPX_TEST(false);
    }
    HPX_TEST(caught_exception);

    //Boundary case 4
    try{
        //This should not throw the exception
        // NOTE the pos starts from zero
        v.get_value_async(v.size() - 1).get();
        HPX_TEST(true);
    }
    catch(hpx::exception const& e)
    {
        HPX_TEST(false);
    }
    catch(...)
    {
        HPX_TEST(false);
    }

    //Check exception for random position outside the range
    try{
        //This should throw the exception
        v.get_value_async(v.size() + 1991).get();
        HPX_TEST(false);
    }
    catch(hpx::exception const& e)
    {
        caught_exception = true;
    }
    catch(...)
    {
        HPX_TEST(false);
    }
    HPX_TEST(caught_exception);

}// End of test_get_value_async_exception()

void test_set_value_exception()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having 
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    size_type expected_total_elements;
    expected_total_elements = num_chunks * chunk_size;
    hpx::vector v(num_chunks, chunk_size, INITIAL_VALUE);

    //
    //Checking the set_value Exceptions
    //
    bool caught_exception = false;
    //Boundary Case 1
    try{
        //This should throw the exception
        v.set_value(-1, 1991);
        HPX_TEST(false);
    }
    catch(hpx::exception const& e)
    {
        caught_exception = true;
    }
    catch(...)
    {
        HPX_TEST(false);
    }
    HPX_TEST(caught_exception);

    //Boundary case 2
    try{
        //This should not throw the exception
        v.set_value(0, 1991);
        //To check value correctly inserted
        HPX_TEST_EQ(v.get_value(0), 1991);
        HPX_TEST(true);
    }
    catch(hpx::exception const& e)
    {
        HPX_TEST(false);
    }
    catch(...)
    {
        HPX_TEST(false);
    }

    //Reseting to the variable to use further
    caught_exception = false;

    //Boundary case 3
    try{
        //This should throw the exception
        // NOTE the pos starts from zero hence this throws
        v.set_value(v.size(), 1991);
        HPX_TEST(false);
    }
    catch(hpx::exception const& e)
    {
        caught_exception = true;
    }
    catch(...)
    {
        HPX_TEST(false);
    }
    HPX_TEST(caught_exception);

    //Boundary case 4
    try{
        //This should not throw the exception
        // NOTE the pos starts from zero
        v.set_value(v.size() - 1, 1991);
        //To check value correctly inserted
        HPX_TEST_EQ(v.get_value(v.size() - 1), 1991);
        HPX_TEST(true);
    }
    catch(hpx::exception const& e)
    {
        HPX_TEST(false);
    }
    catch(...)
    {
        HPX_TEST(false);
    }

    //Check exception for random position outside the range
    try{
        //This should throw the exception
        v.set_value(v.size() + 1991, 1991);
        HPX_TEST(false);
    }
    catch(hpx::exception const& e)
    {
        caught_exception = true;
    }
    catch(...)
    {
        HPX_TEST(false);
    }
    HPX_TEST(caught_exception);

}// End of test_set_value_exception()

void test_set_value_async_exception()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having 
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    size_type expected_total_elements;
    expected_total_elements = num_chunks * chunk_size;
    hpx::vector v(num_chunks, chunk_size, INITIAL_VALUE);

    //
    //Checking the set_value Exceptions
    //
    bool caught_exception = false;
    //Boundary Case 1
    try{
        //This should throw the exception
        v.set_value_async(-1, 1991).get();
        HPX_TEST(false);
    }
    catch(hpx::exception const& e)
    {
        caught_exception = true;
    }
    catch(...)
    {
        HPX_TEST(false);
    }
    HPX_TEST(caught_exception);

    //Boundary case 2
    try{
        //This should not throw the exception
        v.set_value_async(0, 1991).get();
        //To check value correctly inserted
        HPX_TEST_EQ(v.get_value(0), 1991);
        HPX_TEST(true);
    }
    catch(hpx::exception const& e)
    {
        HPX_TEST(false);
    }
    catch(...)
    {
        HPX_TEST(false);
    }

    //Reseting to the variable to use further
    caught_exception = false;

    //Boundary case 3
    try{
        //This should throw the exception
        // NOTE the pos starts from zero hence this throws
        v.set_value_async(v.size(), 1991).get();
        HPX_TEST(false);
    }
    catch(hpx::exception const& e)
    {
        caught_exception = true;
    }
    catch(...)
    {
        HPX_TEST(false);
    }
    HPX_TEST(caught_exception);

    //Boundary case 4
    try{
        //This should not throw the exception
        // NOTE the pos starts from zero
        v.set_value_async(v.size() - 1, 1991).get();
        //To check value correctly inserted
        HPX_TEST_EQ(v.get_value(v.size() - 1), 1991);
        HPX_TEST(true);
    }
    catch(hpx::exception const& e)
    {
        HPX_TEST(false);
    }
    catch(...)
    {
        HPX_TEST(false);
    }

    //Check exception for random position outside the range
    try{
        //This should throw the exception
        v.set_value_async(v.size() + 1991, 1991).get();
        HPX_TEST(false);
    }
    catch(hpx::exception const& e)
    {
        caught_exception = true;
    }
    catch(...)
    {
        HPX_TEST(false);
    }
    HPX_TEST(caught_exception);

}// End of test_set_value_async_exception()

int main()
{
    try
    {
        test_get_value_and_set_value();
        test_get_value_async_and_set_value_async();
        test_get_value_exception();
        test_get_value_async_exception();
        test_set_value_exception();
        test_set_value_async_exception();
    }
    catch(...)
    {
        //  Something went wrong in the program
        HPX_TEST(false);
    }

    return hpx::util::report_errors();
}

