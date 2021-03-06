//  Copyright (c) 2007-2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_TRAITS_IS_LAUNCH_POLICY_APR_8_2014_0436PM)
#define HPX_TRAITS_IS_LAUNCH_POLICY_APR_8_2014_0436PM

#include <hpx/hpx_fwd.hpp>
#include <hpx/runtime/threads/thread_executor.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/mpl/or.hpp>

namespace hpx { namespace traits
{
    template <typename Policy>
    struct is_launch_policy
      : boost::mpl::or_<
            boost::is_same<BOOST_SCOPED_ENUM(launch), Policy>
          , boost::is_base_and_derived<threads::executor, Policy>
        >
    {};
}}

#endif

