//  Copyright (c) 2014 Anuj R. Sharma
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include <hpx/hpx_main.hpp>
#include <hpx/components/vector/vector.hpp>
#include <hpx/components/vector/algorithm.hpp>
#include <hpx/util/lightweight_test.hpp>

#include <math.h>

#define VAL_TYPE double

#define INITIAL_NUM_CHUNKS 100
#define INITIAL_CHUNK_SIZE 10005

typedef std::size_t		size_type;


//Functor declaration
struct square
{
    void operator () (double& i)
    {
        i = i * i;
    }
}sqr;

struct sqroot
{
    void operator () (double& i)
    {
        i = sqrt(i);
    }
}sqt;

void test_for_each_and_for_each_async()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    hpx::vector v(num_chunks, chunk_size);

    //Setting the different values for each elements
    for(size_type i = 0; i < v.size(); ++i)
    {
        v.set_value(i, (VAL_TYPE)i);
    }

//
// Using normal iterator
//
    //Applying square to each element using for each
    hpx::for_each(v.begin(), v.end(), sqr);

    //Checking the correct operator is applied
    for(size_type i = 0; i < v.size(); ++i)
    {
        HPX_TEST_EQ(v.get_value(i), (VAL_TYPE) i * i );
    }

    //Applying square to each element using for each
    hpx::for_each_async(v.begin(), v.end(), sqt).get();

    //Checking the correct operator is applied
    for(size_type i = 0; i < v.size(); ++i)
    {
        HPX_TEST_EQ(v.get_value(i), (VAL_TYPE) i );
    }

}// End of test_for_each()



void test_for_each_n_and_for_each_n_async()
{
    //  Vector created with INITIAL_NUM_CHUNKS chunks and each chunk is having
    //  INITIAL_CHUNK_SIZE elements
    size_type num_chunks = INITIAL_NUM_CHUNKS, chunk_size = INITIAL_CHUNK_SIZE;
    hpx::vector v(num_chunks, chunk_size);

    //Setting the different values for each elements
    for(size_type i = 0; i < v.size(); ++i)
    {
        v.set_value(i, (VAL_TYPE)i);
    }

//
// Using normal iterator
//
    //Applying square to each element using for each
    hpx::for_each_n(v.begin(), (v.size() / 2 ) , sqr);

    //Checking the correct operator is applied
    for(size_type i = 0; i < (v.size() / 2 ) ; ++i)
    {
        HPX_TEST_EQ(v.get_value(i), (VAL_TYPE) i * i );
    }

    //Applying square to each element using for each
    hpx::for_each_n_async(v.begin(), (v.size() / 2 ), sqt).get();

    //Checking the correct operator is applied
    for(size_type i = 0; i < v.size(); ++i)
    {
        HPX_TEST_EQ(v.get_value(i), (VAL_TYPE) i );
    }

}// End of test_for_each_n_and_for_each_n_async()


int main()
{
    try
    {
        test_for_each_and_for_each_async();
        test_for_each_n_and_for_each_n_async();
    }
    catch(...)
    {
        //  Something went wrong in the program
        HPX_TEST(false);
    }

    return hpx::util::report_errors();
}

