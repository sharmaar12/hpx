//  Copyright (c) 2014 Anuj R. Sharma
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include <hpx/hpx_main.hpp>
#include <hpx/components/vector/vector.hpp>
#include <hpx/util/lightweight_test.hpp>

#define VAL_TYPE double
#define INITIAL_VALUE 124
void test_constructor()
{
    //Vector created with default constructor
    hpx::vector v;
    HPX_TEST_EQ(v.size(), (std::size_t) 0);

    //Vector created with 100 chunks and each chunk is having 10005 elements
    std::size_t num_chunks = 100, chunk_size = 10005;
    std::size_t expected_total_elements;
    expected_total_elements = num_chunks * chunk_size;
    hpx::vector v_with_size(num_chunks, chunk_size);
    std::size_t count = 0;

    //Segmented iterator work for std:for_each as lamda is taking value not
    // reference and all other iterator API is implemented.
    std::for_each(v_with_size.begin(), v_with_size.end(),
                  [&count] (VAL_TYPE val)
                  {
                    HPX_TEST_EQ(val, (VAL_TYPE)0);
                    ++count;
                  });

    HPX_TEST_EQ(v_with_size.size(), count);
    HPX_TEST_EQ(v_with_size.size(), expected_total_elements );



    //Vector Created with 100 chunks and each chunk is having 10005 elements
    //All elements of vector are initialized to INITIAL_VALUE
    hpx::vector v_with_val(num_chunks, chunk_size, INITIAL_VALUE);
    count = 0;

    //Segmented iterator work for std:for_each as lamda is taking value not
    // reference and all other iterator API is implemented.
    std::for_each(v_with_val.begin(), v_with_val.end(),
                  [&count] (VAL_TYPE val)
                  {
                    //Check all element is initialized to the zero
                    HPX_TEST_EQ(val, (VAL_TYPE)INITIAL_VALUE);
                    ++count;
                  });

    HPX_TEST_EQ(v_with_val.size(), count);
    HPX_TEST_EQ(v_with_val.size(), expected_total_elements );


    //COPY CONSTRUCTOR TEST
    hpx::vector v_copy(v_with_val);
    count = 0;
    //Segmented iterator work for std:for_each as lamda is taking value not
    // reference and all other iterator API is implemented.
    std::for_each(v_copy.begin(), v_copy.end(),
                  [&count] (VAL_TYPE val)
                  {
                    //Check all element is initialized to the zero
                    HPX_TEST_EQ(val, (VAL_TYPE)INITIAL_VALUE);
                    ++count;
                  });

    HPX_TEST_EQ(v_copy.size(), count);
    HPX_TEST_EQ(v_copy.size(), v_with_val.size());
    HPX_TEST_EQ(v_copy.size(), expected_total_elements );

}// End of test_constructor()

void test_constructor_exception()
{
    bool caught_exception = false;

    try
    {
        hpx::vector v(0, 100); //Invalid_vector
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

    //Resetting the variable to use for other condition
    caught_exception = false;
    try
    {
        hpx::vector v(100, 0); //Invalid_vector
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

}//End of test_constructor_exception()

void test_copy_assignment_operator()
{
    std::size_t num_chunks = 100, chunk_size = 10005;
    std::size_t expected_total_elements = num_chunks * chunk_size;
    hpx::vector v_test(num_chunks, chunk_size, INITIAL_VALUE);

    //CASE 1: size(lvalue_vector) < size(rvalue_vector)
    hpx::vector v_with_size_less;
    //Assign the vector to the empty vector
    v_with_size_less = v_test;

    std::size_t count = 0;
    //Segmented iterator work for std:for_each as lamda is taking value not
    // reference and all other iterator API is implemented.
    std::for_each(v_with_size_less.begin(), v_with_size_less.end(),
                  [&count] (VAL_TYPE val)
                  {
                    //Check all element is initialized to the zero
                    HPX_TEST_EQ(val, (VAL_TYPE)INITIAL_VALUE);
                    ++count;
                  });

    HPX_TEST_EQ(v_with_size_less.size(), count);
    HPX_TEST_EQ(v_with_size_less.size(), v_test.size());
    HPX_TEST_EQ(v_with_size_less.size(), expected_total_elements);
    //To check there is no change in the rvalue_vector of size
    HPX_TEST_EQ(v_test.size(), expected_total_elements);

    // CASE 2: size(lvalue_vector) > size(rvalue_vector)
    hpx::vector v_with_size_more(num_chunks, chunk_size + 100);
    //Test to check both vector sizes are different
    HPX_TEST_NEQ(v_with_size_more.size(), v_test.size());
    HPX_TEST_EQ(v_with_size_more.size(),
                (std::size_t) (num_chunks * (chunk_size + 100) ) );
    //Assign the vector smaller to the bigger vector
    v_with_size_more = v_test;

    count = 0;
    //Segmented iterator work for std:for_each as lamda is taking value not
    // reference and all other iterator API is implemented.
    std::for_each(v_with_size_more.begin(), v_with_size_more.end(),
                  [&count] (VAL_TYPE val)
                  {
                    //Check all element is initialized to the zero
                    HPX_TEST_EQ(val, (VAL_TYPE)INITIAL_VALUE);
                    ++count;
                  });

    HPX_TEST_EQ(v_with_size_more.size(), count);
    HPX_TEST_EQ(v_with_size_more.size(), v_test.size());
    HPX_TEST_EQ(v_with_size_more.size(), expected_total_elements);
    //To check there is no change in the rvalue_vector of size
    HPX_TEST_EQ(v_test.size(), expected_total_elements);

    // CASE 3: size(lvalue_vector) = size(rvalue_vector)
    hpx::vector v_with_size_equal(num_chunks, chunk_size, 1241991);
    //Test to check both vector sizes are different
    HPX_TEST_EQ(v_with_size_equal.size(), v_test.size());
    //Assign the vector smaller to the bigger vector
    v_with_size_equal = v_test;

    count = 0;
    //Segmented iterator work for std:for_each as lamda is taking value not
    // reference and all other iterator API is implemented.
    std::for_each(v_with_size_equal.begin(), v_with_size_equal.end(),
                  [&count] (VAL_TYPE val)
                  {
                    //Check all element is initialized to the zero
                    HPX_TEST_EQ(val, (VAL_TYPE)INITIAL_VALUE);
                    ++count;
                  });

    HPX_TEST_EQ(v_with_size_equal.size(), count);
    HPX_TEST_EQ(v_with_size_equal.size(), v_test.size());
    HPX_TEST_EQ(v_with_size_equal.size(), expected_total_elements);
    //To check there is no change in the rvalue_vector of size
    HPX_TEST_EQ(v_test.size(), expected_total_elements);

}//end of test_operator

void test_array_subscript_operator()
{
    //Vector created with 100 chunks and each chunk is having 10005 elements
    std::size_t num_chunks = 100, chunk_size = 10005;
    std::size_t expected_total_elements;
    expected_total_elements = num_chunks * chunk_size;
    hpx::vector v(num_chunks, chunk_size);

    std::size_t count = 0;

    //Assigning value from 0,1,...,vec.size() - 1
    for(std::size_t i = 0; i < v.size(); ++i)
    {
        try{
            v.set_value(i, (VAL_TYPE)i);
            ++count;
        }
        catch(...){
            HPX_TEST_MSG(false,
                "Something went Wrong in test_array_subscript_operator()");
        }
    }

    //To check for loop executed v.size() times
    HPX_TEST_EQ(v.size(), count);

    count = 0;
    //checking the values are correctly inserted and accessed by [] operator
    for(std::size_t i = 0; i < v.size(); ++i)
    {
        //Test the array subscript correctly assigning the value in correct order
        HPX_TEST_EQ(v[i], (VAL_TYPE)i );
        ++count;
    }
    //To check for loop executed v.size() times
    HPX_TEST_EQ(v.size(), count);
}//End of test_array_subscript_operator()

void test_size_and_size_async()
{
    //Vector created with 100 chunks and each chunk is having 10005 elements
    std::size_t num_chunks = 100, chunk_size = 10005;
    std::size_t expected_total_elements;
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
    //Vector created with 100 chunks and each chunk is having 10005 elements
    std::size_t num_chunks = 100, chunk_size = 10005;
    std::size_t expected_total_elements;
    expected_total_elements = num_chunks * chunk_size;
    hpx::vector v(num_chunks, chunk_size);

    //Both calculate the same value
    HPX_TEST_EQ(v.max_size(), v.max_size_async().get());
}//End of test_max_size_and_max_size_async()

//  This is when the new size is greater than
void test_resize_helper_increase(std::size_t num_chunks,
                                 std::size_t initial_chunk_size,
                                 std::size_t new_chunk_size,
                                 VAL_TYPE val = (double)0)
{
    std::size_t initial_size = num_chunks * initial_chunk_size;
    hpx::vector v(num_chunks, initial_chunk_size, INITIAL_VALUE);

    if(val == (VAL_TYPE)0)
    {
        //Resize without default value
        v.resize(new_chunk_size);
    }
    else
        v.resize(new_chunk_size, val);

    std::size_t new_size = num_chunks * new_chunk_size;
    std::size_t count_old_ele = 0, count_new_ele = 0;

    for(std::size_t chunks = 0; chunks < num_chunks; ++chunks)
    {
        for(std::size_t i = 0; i < initial_chunk_size; ++i)
        {
            HPX_TEST_EQ(v[ (chunks * new_chunk_size) + i],
                        (VAL_TYPE) INITIAL_VALUE);
            ++count_old_ele;
        }
        for(std::size_t i = initial_chunk_size;
                i < new_chunk_size;
                ++i)
        {
            //NOTE: I have not resized using val if val == 0 (I.e. the value for
            // val is not provided) hence in case where val is not specified
            // this checks the newly created elements are initialized to zero.
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
void test_resize_helper_decrease_equal(std::size_t num_chunks,
                                       std::size_t initial_chunk_size,
                                       std::size_t new_chunk_size,
                                       VAL_TYPE val = (double)0)
{
    std::size_t initial_size = num_chunks * initial_chunk_size;
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

    std::size_t new_size = num_chunks * new_chunk_size;
    std::size_t count_old_ele = 0;

    for(std::size_t chunks = 0; chunks < num_chunks; ++chunks)
    {
        for(std::size_t i = 0; i < new_chunk_size; ++i)
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
    // When Size is going to increase
    test_resize_helper_increase(100, 1000, 10005);
    test_resize_helper_increase(100, 1000, 10005, 1241991);

    // When Size is going to decrease
    test_resize_helper_decrease_equal(100, 10005, 1000);
    test_resize_helper_decrease_equal(100, 10005, 1000, 1241991);

    // When Size is going to remain the same
    test_resize_helper_decrease_equal(100, 10005, 1000);
    test_resize_helper_decrease_equal(100, 10005, 1000, 1241991);
}//End of test_resize()

void test_capacity_and_capacity_async()
{
    //Vector created with 100 chunks and each chunk is having 10005 elements
    std::size_t num_chunks = 100, chunk_size = 10005;
    std::size_t expected_total_elements;
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

    //Vector created with 100 chunks and each chunk is having 10005 elements
    std::size_t num_chunks = 100, chunk_size = 10005;
    hpx::vector v(num_chunks, chunk_size);
    HPX_TEST_EQ(false, v.empty());
    HPX_TEST_EQ(false, v.empty_async().get());
}//End of test_empty_and_empty_async()

int main()
{
    try
    {
        test_constructor();
        test_constructor_exception();
        test_copy_assignment_operator();
        test_array_subscript_operator();
        test_size_and_size_async();
        test_max_size_and_max_size_async();
        test_resize();
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
