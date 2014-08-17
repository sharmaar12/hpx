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

typedef std::size_t	size_type;

void test_constructor()
{
    //Vector created with default constructor
    hpx::vector v;
    HPX_TEST_EQ(v.size(), (size_type) 0);
    //Tests iterator API are behaving correctly
    HPX_TEST(v.begin() == v.end());
    HPX_TEST(v.cbegin() == v.cend());

    //	Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having 
    //	INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    size_type expected_total_elements;
    expected_total_elements = num_chunks * chunk_size;
    hpx::vector v_with_size(num_chunks, chunk_size);

    //Tests iterator API are behaving correctly
    HPX_TEST(v_with_size.begin() != v_with_size.end());
    HPX_TEST(v_with_size.cbegin() != v_with_size.cend());

    size_type count = 0;

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

    //Vector Created with INITIAL_NUM_CHUNKS chunks and each chunk is having INITIAL_CHUNK_SIZE elements
    //All elements of vector are initialized to INITIAL_VALUE
    hpx::vector v_with_val(num_chunks, chunk_size, INITIAL_VALUE);


    //Tests iterator API are behaving correctly
    HPX_TEST(v_with_val.begin() != v_with_val.end());
    HPX_TEST(v_with_val.cbegin() != v_with_val.cend());

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

    //Tests iterator API are behaving correctly
    HPX_TEST(v_copy.begin() != v_copy.end());
    HPX_TEST(v_copy.cbegin() != v_copy.cend());

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

