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
#include <hpx/components/vector/chunk_vector_component.hpp>

namespace hpx
{
 //   template<class input_iterator, class fun>
        void for_each(hpx::segmented_vector_iterator first, segmented_vector_iterator last, hpx::util::function<void(VALUE_TYPE&)> fn_)
        {
            auto sfirst_ = segmented_vector_iterator::segment(first);
            auto slast_ = segmented_vector_iterator::segment(last);

            if(sfirst_ == slast_)
                hpx::stubs::chunk_vector::chunk_for_each_async( (segmented_vector_iterator::local(first).first).get(), //gives gid
                                             segmented_vector_iterator::local(first).second, // gives first index
                                             segmented_vector_iterator::local(last).second, //gives last index
                                             fn_); //gives the functor
            else
            {
                hpx::stubs::chunk_vector::chunk_for_each_async( (segmented_vector_iterator::local(first).first).get(), //gives gid
                                             segmented_vector_iterator::local(first).second, // gives first index
                                             segmented_vector_iterator::end(sfirst_).second, //gives last index
                                             fn_); //gives the functor
                ++sfirst_;
                while(sfirst_ != slast_)
                {
                    hpx::stubs::chunk_vector::chunk_for_each_async( (segmented_vector_iterator::begin(sfirst_).first).get(), //gives gid
                                             segmented_vector_iterator::begin(sfirst_).second, // gives first index
                                             segmented_vector_iterator::end(sfirst_).second, //gives last index
                                             fn_); //gives the functor
                    ++sfirst_;
                }

                hpx::stubs::chunk_vector::chunk_for_each_async( (segmented_vector_iterator::begin(slast_).first).get(), //gives gid
                                             segmented_vector_iterator::begin(slast_).second, // gives first index
                                             segmented_vector_iterator::local(last).second, //gives last index
                                             fn_); //gives the functor
            }//end of else

        }//end of for_each
}//end of hpx namespace

#endif // ALGORITHM_HPP
