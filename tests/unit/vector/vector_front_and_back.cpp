//  Copyright (c) 2014 Anuj R. Sharma
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_main.hpp>
#include <hpx/components/vector/vector.hpp>
#include <hpx/util/lightweight_test.hpp>

#define VAL_TYPE double
#define INITIAL_VALUE 124

void test_front_and_front_async()
{
    {
        //Vector created with 100 chunks and each chunk is having 10005 elements
        std::size_t num_chunks = 100, chunk_size = 10005;
        hpx::vector v(num_chunks, chunk_size);

        HPX_TEST_EQ(v.front(), (VAL_TYPE) 0 );
        HPX_TEST_EQ(v.front_async().get(), (VAL_TYPE) 0 );
    }
    {
        //Vector created with 100 chunks and each chunk is having 10005 elements
        std::size_t num_chunks = 100, chunk_size = 10005;
        hpx::vector v(num_chunks, chunk_size, INITIAL_VALUE);

        HPX_TEST_EQ(v.front(), (VAL_TYPE) INITIAL_VALUE );
        HPX_TEST_EQ(v.front_async().get(), (VAL_TYPE) INITIAL_VALUE );

        v.set_value(0, 1991);

        HPX_TEST_EQ(v.front(), (VAL_TYPE) 1991 );
        HPX_TEST_EQ(v.front_async().get(), (VAL_TYPE) 1991 );
    }
}//End of test_front_and_front_async()

void test_back_and_back_async()
{
    {
        //Vector created with 100 chunks and each chunk is having 10005 elements
        std::size_t num_chunks = 100, chunk_size = 10005;
        hpx::vector v(num_chunks, chunk_size);

        HPX_TEST_EQ(v.back(), (VAL_TYPE) 0 );
        HPX_TEST_EQ(v.back_async().get(), (VAL_TYPE) 0 );
    }
    {
        //Vector created with 100 chunks and each chunk is having 10005 elements
        std::size_t num_chunks = 100, chunk_size = 10005;
        hpx::vector v(num_chunks, chunk_size, INITIAL_VALUE);

        HPX_TEST_EQ(v.back(), (VAL_TYPE) INITIAL_VALUE );
        HPX_TEST_EQ(v.back_async().get(), (VAL_TYPE) INITIAL_VALUE );

        //NOTE the pos is starts from zero
        v.set_value(v.size() - 1, 1991);

        HPX_TEST_EQ(v.back(), (VAL_TYPE) 1991 );
        HPX_TEST_EQ(v.back_async().get(), (VAL_TYPE) 1991 );

        v.push_back((VAL_TYPE)1241991);

        HPX_TEST_EQ(v.back(), (VAL_TYPE) 1241991 );
        HPX_TEST_EQ(v.back_async().get(), (VAL_TYPE) 1241991 );
    }
}//End of test_back_and_back_async()

int main()
{
    try
    {
        test_front_and_front_async();
        test_back_and_back_async();
    }
    catch(...)
    {
        //  Something went wrong in the program
        HPX_TEST(false);
    }

    return hpx::util::report_errors();
}

