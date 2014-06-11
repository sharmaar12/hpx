#include <hpx/hpx_main.hpp>
#include <hpx/include/iostreams.hpp>

#include <iostream>
#include "chunk_vector_component.hpp"


using namespace std;

int main()
{
    typedef hpx::dthrust::server::chunk_vector chunk_vector_type;
    hpx::cout<< "********Hi there, This is the program to check the Functionality of the chunk_vector_component********\n" << hpx::flush;
   //Block to check the functionality of the constructor with size as argument
    {
        hpx::cout << "\nCheck constructor (chunk_vector(size = 5)) " << hpx::flush;
        hpx::dthrust::chunk_vector vec(hpx::components::new_<chunk_vector_type>(hpx::find_here(), 5));
        hpx::cout << "\nSize of Vector : (size_async() function) " << (vec.size_async()).get() << hpx::flush;
    }


    //Block to check the functionality of the constructor with size and value as parameter as argument
    {
        hpx::cout << "\nCheck constructor (chunk_vector(size = 5, value = 11)) " << hpx::flush;
        hpx::dthrust::chunk_vector vec(hpx::components::new_<chunk_vector_type>(hpx::find_here(), 5, 11));
        hpx::cout << "\nSize of Vector (size_sync() function): " << vec.size_sync() << "\n" << hpx::flush;
    }

    //Block to check the functionality of push_back function
    {
        hpx::cout << "\nCheck Push_back (chunk_vector(size = 1, value = 11)) " << hpx::flush;
        hpx::dthrust::chunk_vector vec(hpx::components::new_<chunk_vector_type>(hpx::find_here(), 1, 11));
        hpx::cout << "\nSize of Vector before push_back: " << (vec.size_async()).get() << "\n" << hpx::flush;
        vec.push_back_sync(100);
        vec.push_back_non_blocking(200);
        hpx::cout << "\nSize of Vector after two push_back(with sync push_back and push_back_non_blocking): " << (vec.size_async()).get() << hpx::flush;
        hpx::cout << "\nThe Vector Contents: \n"  << hpx::flush;
        vec.display_sync();
        hpx::cout << "\nThe value at position 1 (at sync): "  << vec.at_sync(1) << hpx::flush;
        hpx::cout << "\nThe value at position 2 (at sync): "  << (vec.at_async(2)).get() << hpx::flush;
        //setting the value
        vec.set_value_sync(1, 12);
        hpx::cout << "\nThe value at position 1 after setting value to be 12 (set_value_sync) is: "  << vec.at_sync(1) << hpx::flush;

        hpx::cout << "\nChecking empty status of the chunk_vector : " << (vec.empty_async()).get() << hpx::flush;
        vec.clear_sync();
        hpx::cout << "\nChecking empty status after clear_sync() function of the chunk_vector : " << vec.empty_sync() << "\n" << hpx::flush;

    }

    return 0;
}

