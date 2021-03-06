//  Copyright (c) 2013 Hartmut Kaiser
//  Copyright (c) 2013 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Bidirectional network bandwidth test

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <hpx/include/iostreams.hpp>
#include <hpx/util/serialize_buffer.hpp>

#include <boost/assert.hpp>
#include <boost/shared_ptr.hpp>

///////////////////////////////////////////////////////////////////////////////
#define MESSAGE_ALIGNMENT 64
#define MAX_ALIGNMENT 65536
#define MAX_MSG_SIZE 8 // (1<<22)
#define SEND_BUFSIZE (MAX_MSG_SIZE + MAX_ALIGNMENT)

#define LOOP_LARGE  100
#define SKIP_LARGE  10


char *send_buffer_orig;

///////////////////////////////////////////////////////////////////////////////
char* align_buffer (char* ptr, unsigned long align_size)
{
    return (char*)(((std::size_t)ptr + (align_size - 1)) / align_size * align_size);
}

#if defined(BOOST_MSVC)
unsigned long getpagesize()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return si.dwPageSize;
}
#endif

///////////////////////////////////////////////////////////////////////////////
hpx::util::serialize_buffer<char>
message(hpx::util::serialize_buffer<char> const& receive_buffer)
{
    return receive_buffer;
}
HPX_PLAIN_ACTION(message);

HPX_REGISTER_BASE_LCO_WITH_VALUE_DECLARATION(hpx::util::serialize_buffer<char>, serialization_buffer_char);
HPX_REGISTER_BASE_LCO_WITH_VALUE(hpx::util::serialize_buffer<char>, serialization_buffer_char);

///////////////////////////////////////////////////////////////////////////////
double receive(
    hpx::naming::id_type dest,
    char * send_buffer,
    std::size_t size,
    std::size_t loop,
    std::size_t window_size)
{
    int skip = SKIP_LARGE;

    typedef hpx::util::serialize_buffer<char> buffer_type;
    buffer_type recv_buffer;

    hpx::util::high_resolution_timer t;

    std::vector<hpx::future<buffer_type> > recv_buffers;
    recv_buffers.reserve(window_size);

    message_action msg;
    for (std::size_t i = 0; i != loop + skip; ++i) {
        // do not measure warm up phase
        if (i == skip)
            t.restart();

        for(std::size_t j = 0; j < window_size; ++j)
        {
            recv_buffers.push_back(hpx::async(msg, dest, buffer_type(send_buffer, size,
                buffer_type::reference)));
        }

        hpx::wait_all(recv_buffers);
        recv_buffers.clear();
    }

    double elapsed = t.elapsed();
    return (elapsed * 1e6) / (2 * loop * window_size);
}

///////////////////////////////////////////////////////////////////////////////
void print_header ()
{
    hpx::cout << "# OSU HPX Latency Test\n"
              << "# Size    Latency (microsec)\n"
              << hpx::flush;
}

///////////////////////////////////////////////////////////////////////////////
void run_benchmark(boost::program_options::variables_map & vm)
{
    // use the first remote locality to bounce messages, if possible
    hpx::id_type here = hpx::find_here();

    hpx::id_type there = here;
    std::vector<hpx::id_type> localities = hpx::find_remote_localities();
    if (!localities.empty())
        there = localities[0];

    std::size_t window_size = vm["window-size"].as<std::size_t>();
    std::size_t loop = vm["loop"].as<std::size_t>();
    std::size_t min_size = vm["min-size"].as<std::size_t>();
    std::size_t max_size = vm["max-size"].as<std::size_t>();

    if(max_size < min_size) std::swap(max_size, min_size);
    
    // align used buffers on page boundaries
    unsigned long align_size = getpagesize();
    //BOOST_ASSERT(align_size <= MAX_ALIGNMENT);
    send_buffer_orig = new char[max_size + align_size];
    char* send_buffer = align_buffer(send_buffer_orig, align_size);

    hpx::util::high_resolution_timer timer;
    // perform actual measurements
    for (std::size_t size = min_size; size <= max_size; size *= 2)
    {
        double latency = receive(there, send_buffer, size, loop, window_size);
        hpx::cout << std::left << std::setw(10) << size
                  << latency << hpx::endl << hpx::flush;
    }
    hpx::cout << "Total time: " << timer.elapsed_nanoseconds() << "\n" << hpx::flush;
    delete[] send_buffer_orig;

}
