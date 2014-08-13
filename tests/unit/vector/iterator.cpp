//  Copyright (c) 2014 Anuj R. Sharma
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_main.hpp>
#include <hpx/components/vector/vector.hpp>
#include <hpx/util/lightweight_test.hpp>

#define VAL_TYPE double
#define INITIAL_VALUE 124

typedef hpx::vector::iterator iter;

void test_dereference_and_offset_dereference()
{
    //Vector created with 100 chunks and each chunk is having 10005 elements
    std::size_t num_chunks = 100, chunk_size = 10005;
    hpx::vector v(num_chunks, chunk_size, INITIAL_VALUE);

    iter it_begin = v.begin();
    iter it_end = v.end();

    std::size_t count = 0;

    //Checking default value through dereference
    for(iter it = it_begin; it != it_end; ++it)
    {
        HPX_TEST_EQ(*it, (VAL_TYPE) INITIAL_VALUE);
        ++count;
    }
    HPX_TEST_EQ(count, v.size());

    count = 0;
    //Checking default value through offset dereference
    iter it_offset_deref = it_begin;
    for(std::size_t i = 0; i < v.size(); i++)
    {
        HPX_TEST_EQ(it_offset_deref[i], (VAL_TYPE) INITIAL_VALUE);
        ++count;
    }
    HPX_TEST_EQ(count, v.size());

    count = 0;
    //Setting the different values for each elements
    for(std::size_t i = 0; i < v.size(); ++i)
    {
        v.set_value(i, (VAL_TYPE)i);
        ++count;
    }
    //Check value is assigned for each element in vector
    HPX_TEST_EQ(count, v.size());

    count = 0;
    std::size_t j = 0;
    //Checking the values using the dereference operator
    for(iter it = it_begin; it != it_end; ++it, ++j)
    {
        HPX_TEST_EQ(*it, (VAL_TYPE) j);
        ++count;
    }
    // Check all values are tested
    HPX_TEST_EQ(count, v.size());

    count = 0;
    //Checking default value through offset dereference
    for(std::size_t i = 0; i < v.size(); i++)
    {
        HPX_TEST_EQ(it_offset_deref[i], (VAL_TYPE) i);
        ++count;
    }
    HPX_TEST_EQ(count, v.size());

    //Checking the dereference does not throw the exception
    try
    {
        iter it = it_end + 100;
        *it;
        it++;
        *it;

        it = it_begin - 100;
        *it;
    }
    catch(...)
    {
        HPX_TEST_MSG(false, "Dereference is throwing the exception");
    }

    //Checking the offset dereference does not throw the exception
    try
    {
        iter it = it_end;
        it[100];

        it = it_begin - 100;
        it[35];
    }
    catch(...)
    {
        HPX_TEST_MSG(false, "Offset dereference is throwing the exception");
    }

}// End of test_dereference_and_offset_dereference()

void test_copy_constructor_and_assignment_operator()
{
    //Vector created with 100 chunks and each chunk is having 10005 elements
    std::size_t num_chunks = 100, chunk_size = 10005;
    hpx::vector v(num_chunks, chunk_size);

    iter it_begin = v.begin();

    //Setting the different values for each elements
    for(std::size_t i = 0; i < v.size(); ++i)
    {
        v.set_value(i, (VAL_TYPE)i);
    }

    //Copy constructor
    iter it(it_begin);

    //Checking copy constructor work properly
    HPX_TEST_EQ(*it, *it_begin);

    //Test assignment operator
    iter it_assign = it_begin;

    //Checking assignment work properly
    HPX_TEST_EQ(*it_assign, *it_begin);

}// End of test_copy_constructor_and_assignment_operator()

void test_remaining_operator()
{
    //Vector created with 100 chunks and each chunk is having 10005 elements
    std::size_t num_chunks = 100, chunk_size = 10005;
    hpx::vector v(num_chunks, chunk_size);

    iter it_begin = v.begin();
    iter it_end = v.end();

    //Setting the different values for each elements
    for(std::size_t i = 0; i < v.size(); ++i)
    {
        v.set_value(i, (VAL_TYPE)i);
    }


    //TEST == & !=
    {
        iter it1 = it_begin;
        iter it2 = it1; //We tested assignment so it is correct

        HPX_TEST_EQ(true, it1 == it2);
        //If both are equal then the value at that positions must be same
        HPX_TEST_EQ(*it1, *it2);

        // The following operation surely does not make it2 to go out of range
        it2++; //Assume this increment correctly

        HPX_TEST_EQ(true, it1 != it2);
        HPX_TEST_NEQ(*it1, *it2);
    }


    //TEST ++ & --
    {
        //BOUNDARY CASE 1
        iter it = it_begin;
        //Prefix increment and decrement
        --it;
        HPX_TEST(it != it_begin);
        ++it;
        HPX_TEST(it == it_begin);

        //Postfix increment and decrement
        it--;
        HPX_TEST(it != it_begin);
        it++;
        HPX_TEST(it == it_begin);

        //BOUNDARY CASE 2
        it = it_end;
        //Prefix increment and decrement
        --it;
        HPX_TEST(it != it_end);
        ++it;
        HPX_TEST(it == it_end);

        //Postfix increment and decrement
        it--;
        HPX_TEST(it != it_end);
        it++;
        HPX_TEST(it == it_end);

        //MIDDLE CASE
        iter it_test = it_begin;

        //increment it 5 times
        for(int i = 0; i < 5; i++)
            it_test++;

        it = it_test;
        //Prefix increment and decrement
        --it;
        HPX_TEST(it != it_test);
        ++it;
        HPX_TEST(it == it_test);

        //Postfix increment and decrement
        it--;
        HPX_TEST(it != it_test);
        it++;
        HPX_TEST(it == it_test);
    }


    //TEST + n & - n
    {
        //BOUNDARY CASE 1
        iter it = it_begin;
        iter it_test = it + 5;

        //As values are from 0,1 hence this value must be 5
        HPX_TEST_EQ(5, *it_test);

        // Increment it 5 times
        for(int i = 0; i < 5; i++)
            it++;

        HPX_TEST(it == it_test);

        it_test = it - 5;

        //As values are from 0,1 hence this value must be 0
        HPX_TEST_EQ(0, *it_test);

        //decrement it 5 times
        for(int i = 0; i < 5; i++)
            it--;

        HPX_TEST(it == it_test);

        //BOUNDARY CASE 2
        it = it_end;
        it_test = it - 5;
        //As values are from 0,1 hence this value must be v.size() - 5
        HPX_TEST_EQ(v.size() - 5, *it_test);

        // Decrement it 5 times
        for(int i = 0; i < 5; i++)
            it--;

        HPX_TEST(it == it_test);

        it_test = it + 5;

        //Increment it 5 times
        for(int i = 0; i < 5; i++)
            it++;

        HPX_TEST(it == it_test);

        //MIDDLE CASE
        it = it_begin + 5;
        it_test = it;

        //This works as we are having values assigned in ascending order
        HPX_TEST_EQ(*(it_test - 2), *it - 2);
        HPX_TEST_EQ(*(it_test + 2), *it + 2);
    }

    //TEST += & -=
    {
        //BOUNDARY CASE 1
        iter it = it_begin;
        iter it_test = it;

        it_test += 5;
        it = it + 5;
        HPX_TEST(it_test == it);

        it_test -= 5;
        it = it - 5;
        HPX_TEST(it_test == it);

        //BOUNDARY CASE 2
        it = it_end;
        it_test = it;

        it_test -= 5;
        it = it - 5;
        HPX_TEST(it_test == it);

        it_test += 5;
        it = it + 5;
        HPX_TEST(it_test == it);

        //MIDDLE CASE
        it = it_begin + 5;
        it_test = it;

        it_test -= 3;
        it = it - 3;
        HPX_TEST(it_test == it);

        it_test += 3;
        it = it + 3;
        HPX_TEST(it_test == it);
    }

    //TEST <, <=, > & >=
    {
        //BOUNDARY CASE 1
        iter it = it_begin;
        iter it_test = it;

        HPX_TEST_EQ(true, it <= it_test);
        HPX_TEST_EQ(false, it < it_test);

        HPX_TEST_EQ(true, it >= it_test);
        HPX_TEST_EQ(false, it > it_test);

        //Increment it_test
        it_test++;
        HPX_TEST_EQ(true, it <= it_test);
        HPX_TEST_EQ(true, it < it_test);

        HPX_TEST_EQ(false, it >= it_test);
        HPX_TEST_EQ(false, it > it_test);

        //BOUNDARY CASE 2
        it = it_end;
        it_test = it;

        HPX_TEST_EQ(true, it <= it_test);
        HPX_TEST_EQ(false, it < it_test);

        HPX_TEST_EQ(true, it >= it_test);
        HPX_TEST_EQ(false, it > it_test);

        //Decrement it_test
        it_test--;
        HPX_TEST_EQ(false, it <= it_test);
        HPX_TEST_EQ(false, it < it_test);

        HPX_TEST_EQ(true, it >= it_test);
        HPX_TEST_EQ(true, it > it_test);

        //MIDDLE CASE
        it = it_begin + 5;
        it_test = it;

        HPX_TEST_EQ(true, it <= it_test);
        HPX_TEST_EQ(false, it < it_test);

        HPX_TEST_EQ(true, it >= it_test);
        HPX_TEST_EQ(false, it > it_test);

        //Increment it_test
        it_test++;
        HPX_TEST_EQ(true, it <= it_test);
        HPX_TEST_EQ(true, it < it_test);

        HPX_TEST_EQ(false, it >= it_test);
        HPX_TEST_EQ(false, it > it_test);

        //NOTE we have decremented twice
        //Decrement it_test
        it_test--; it_test--;
        HPX_TEST_EQ(false, it <= it_test);
        HPX_TEST_EQ(false, it < it_test);

        HPX_TEST_EQ(true, it >= it_test);
        HPX_TEST_EQ(true, it > it_test);
    }
}// End of test_remaining_operator()

int main()
{
    try
    {
        test_dereference_and_offset_dereference();
        test_copy_constructor_and_assignment_operator();
        test_remaining_operator();
    }
    catch(...)
    {
        //  Something went wrong in the program
        HPX_TEST(false);
    }

    return hpx::util::report_errors();
}

