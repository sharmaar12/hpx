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
                    //Check all element is initialized to the INITIAL_VALUE
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
                    //Check all element is initialized to the INITIAL_VALUE
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

int main()
{
    try
    {
        test_constructor();
        test_constructor_exception();
    }
    catch(...)
    {
        //  Something went wrong in the program
        HPX_TEST(false);
    }

    return hpx::util::report_errors();
}

