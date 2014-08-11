//  Copyright (c) 2014 Anuj R. Sharma
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include <hpx/hpx_main.hpp>
#include <hpx/components/vector/vector.hpp>
#include <hpx/util/lightweight_test.hpp>

#define VAL_TYPE double
#define INITIAL_VALUE 124

void test_copy_assignment_operator()
{
    //Vector created with 100 chunks and each chunk is having 10005 elements
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
                    //Check all element is initialized to the INITIAL_VALUE
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
                    //Check all element is initialized to the INITIAL_VALUE
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
                    //Check all element is initialized to the INITIAL_VALUE
                    HPX_TEST_EQ(val, (VAL_TYPE)INITIAL_VALUE);
                    ++count;
                  });

    HPX_TEST_EQ(v_with_size_equal.size(), count);
    HPX_TEST_EQ(v_with_size_equal.size(), v_test.size());
    HPX_TEST_EQ(v_with_size_equal.size(), expected_total_elements);
    //To check there is no change in the rvalue_vector of size
    HPX_TEST_EQ(v_test.size(), expected_total_elements);

}//end of test_copy_assignment_operator()

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

int main()
{
    try
    {
        test_copy_assignment_operator();
        test_array_subscript_operator();
    }
    catch(...)
    {
        //  Something went wrong in the program
        HPX_TEST(false);
    }

    return hpx::util::report_errors();
}

