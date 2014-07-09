//  Copyright (c) 2014 Anuj R. Sharma
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//
// This file contains the hpx implementation of some algorithm from Standard Template Library
//

#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <hpx/include/util.hpp>
#include <hpx/include/lcos.hpp>
#include <hpx/components/vector/chunk_vector_component.hpp>

namespace hpx
{
 //   template<class input_iterator, class fun>
        void for_each(hpx::segmented_vector_iterator first, segmented_vector_iterator last, hpx::util::function<void(VALUE_TYPE&)> fn)
        {
            auto sfirst_ = segmented_vector_iterator::segment(first);
            auto slast_ = segmented_vector_iterator::segment(last);

            std::vector<hpx::lcos::future<void>> for_each_lazy_sync;

            if(sfirst_ == slast_)
            {
                for_each_lazy_sync.push_back(
                        hpx::stubs::chunk_vector::chunk_for_each_async( (segmented_vector_iterator::local(first).first).get(), //gives gid
                                             segmented_vector_iterator::local(first).second, // gives first index
                                             segmented_vector_iterator::local(last).second, //gives last index
                                             fn) //gives the functor
                                             );
            }
            else
            {
                for_each_lazy_sync.push_back(
                        hpx::stubs::chunk_vector::chunk_for_each_async( (segmented_vector_iterator::local(first).first).get(), //gives gid
                                             segmented_vector_iterator::local(first).second, // gives first index
                                             segmented_vector_iterator::end(sfirst_).second, //gives last index
                                             fn) //gives the functor
                                             );
                ++sfirst_;
                while(sfirst_ != slast_)
                {
                    for_each_lazy_sync.push_back(
                                hpx::stubs::chunk_vector::chunk_for_each_async( (segmented_vector_iterator::begin(sfirst_).first).get(), //gives gid
                                                 segmented_vector_iterator::begin(sfirst_).second, // gives first index
                                                 segmented_vector_iterator::end(sfirst_).second, //gives last index
                                                 fn) //gives the functor
                                                  );
                    ++sfirst_;
                }
                for_each_lazy_sync.push_back(
                            hpx::stubs::chunk_vector::chunk_for_each_async( (segmented_vector_iterator::begin(slast_).first).get(), //gives gid
                                             segmented_vector_iterator::begin(slast_).second, // gives first index
                                             segmented_vector_iterator::local(last).second, //gives last index
                                             fn) //gives the functor
                                             );
            }//end of else

            hpx::wait_all(for_each_lazy_sync);
        }//end of for_each

        hpx::lcos::future<void> for_each_async(hpx::segmented_vector_iterator first, segmented_vector_iterator last, hpx::util::function<void(VALUE_TYPE&)> fn)
        {
            return hpx::async(launch::async,
                              hpx::util::bind((&hpx::for_each),
                                               first,
                                               last,
                                               fn
                                              )
                              );
        }//end of for_each_async
        
        //
        // FOR_EACH_N API
        //
        hpx::util::function<void(VALUE_TYPE&)> for_each_n(hpx::segmented_vector_iterator first,
                                                        std::size_t n,
                                                        hpx::util::function<void(VALUE_TYPE&)> fn)
        {
            return hpx::for_each(first, first + n, fn);
        }//end of for_each_n

        hpx::lcos::future<hpx::util::function<void(VALUE_TYPE&)>>
         for_each_n_async(hpx::segmented_vector_iterator first,
                         std::size_t n,
                         hpx::util::function<void(VALUE_TYPE&)> fn)
        {
            return hpx::async(launch::async,
                              hpx::util::bind((&hpx::for_each),
                                               first,
                                               first + n,
                                               fn
                                              )
                              );
        }//end of for_each _n_async
        
        
}//end of hpx namespace

#endif // ALGORITHM_HPP
