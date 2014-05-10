//  Copyright (c) 2007-2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_init.hpp>
#include <hpx/include/lcos.hpp>
#include <hpx/include/components.hpp>
#include <hpx/include/async.hpp>
#include <hpx/util/lightweight_test.hpp>

///////////////////////////////////////////////////////////////////////////////
boost::int32_t increment(boost::int32_t i)
{
    return i + 1;
}
HPX_PLAIN_ACTION(increment);

boost::int32_t increment_with_future(hpx::shared_future<boost::int32_t> fi)
{
    return fi.get() + 1;
}
HPX_PLAIN_ACTION(increment_with_future);

///////////////////////////////////////////////////////////////////////////////
struct decrement_server
  : hpx::components::managed_component_base<decrement_server>
{
    boost::int32_t call(boost::int32_t i) const
    {
        return i - 1;
    }

    HPX_DEFINE_COMPONENT_CONST_ACTION(decrement_server, call);
};

typedef hpx::components::managed_component<decrement_server> server_type;
HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(server_type, decrement_server);

typedef decrement_server::call_action call_action;
HPX_REGISTER_ACTION_DECLARATION(call_action);
HPX_REGISTER_ACTION(call_action);

///////////////////////////////////////////////////////////////////////////////
struct movable_only
{
    HPX_MOVABLE_BUT_NOT_COPYABLE(movable_only);

public:
    movable_only(boost::int32_t i = 0) : i(i) {}
    movable_only(movable_only&& other) : i(other.i) { other.i = 0; }

    template <typename Archive>
    void serialize(Archive& ar, unsigned) { ar & i; }

    boost::int32_t i;
};

boost::int32_t movable_only_increment(movable_only m)
{
    return m.i + 1;
}
HPX_PLAIN_ACTION(movable_only_increment);

movable_only movable_only_decrement(boost::int32_t i)
{
    return i - 1;
}
HPX_PLAIN_ACTION(movable_only_decrement);

///////////////////////////////////////////////////////////////////////////////
void test_remote_async(hpx::id_type const& target)
{
    {
        increment_action inc;

        hpx::future<boost::int32_t> f1 = hpx::async(inc, target, 42);
        HPX_TEST_EQ(f1.get(), 43);

        hpx::future<boost::int32_t> f2 =
            hpx::async(hpx::launch::all, inc, target, 42);
        HPX_TEST_EQ(f2.get(), 43);
    }

    {
        increment_with_future_action inc;
        hpx::promise<boost::int32_t> p;
        hpx::shared_future<boost::int32_t> f = p.get_future();

        hpx::future<boost::int32_t> f1 = hpx::async(inc, target, f);
        hpx::future<boost::int32_t> f2 =
            hpx::async(hpx::launch::all, inc, target, f);

        p.set_value(42);
        HPX_TEST_EQ(f1.get(), 43);
        HPX_TEST_EQ(f2.get(), 43);
    }

    {
        increment_action inc;

        hpx::future<boost::int32_t> f1 =
            hpx::async(hpx::util::bind(inc, target, 42));
        HPX_TEST_EQ(f1.get(), 43);
    }

    {
        hpx::future<boost::int32_t> f1 =
            hpx::async<increment_action>(target, 42);
        HPX_TEST_EQ(f1.get(), 43);

        hpx::future<boost::int32_t> f2 =
            hpx::async<increment_action>(hpx::launch::all, target, 42);
        HPX_TEST_EQ(f2.get(), 43);
    }

    {
        hpx::future<hpx::id_type> dec_f =
            hpx::components::new_<decrement_server>(target);
        hpx::id_type dec = dec_f.get();

        call_action call;

        hpx::future<boost::int32_t> f1 = hpx::async(call, dec, 42);
        HPX_TEST_EQ(f1.get(), 41);

        hpx::future<boost::int32_t> f2 =
            hpx::async(hpx::launch::all, call, dec, 42);
        HPX_TEST_EQ(f2.get(), 41);
    }

    {
        hpx::future<hpx::id_type> dec_f =
            hpx::components::new_<decrement_server>(target);
        hpx::id_type dec = dec_f.get();

        call_action call;

        hpx::future<boost::int32_t> f1 =
            hpx::async(hpx::util::bind(call, dec, 42));
        HPX_TEST_EQ(f1.get(), 41);

        using hpx::util::placeholders::_1;
        using hpx::util::placeholders::_2;

        hpx::future<boost::int32_t> f2 =
            hpx::async(hpx::util::bind(call, _1, 42), dec);
        HPX_TEST_EQ(f2.get(), 41);

        hpx::future<boost::int32_t> f3 =
            hpx::async(hpx::util::bind(call, _1, _2), dec, 42);
        HPX_TEST_EQ(f3.get(), 41);
    }

    {
        hpx::future<hpx::id_type> dec_f =
            hpx::components::new_<decrement_server>(target);
        hpx::id_type dec = dec_f.get();

        hpx::future<boost::int32_t> f1 =
            hpx::async<call_action>(dec, 42);
        HPX_TEST_EQ(f1.get(), 41);

        hpx::future<boost::int32_t> f2 =
            hpx::async<call_action>(hpx::launch::all, dec, 42);
        HPX_TEST_EQ(f2.get(), 41);
    }
}

///////////////////////////////////////////////////////////////////////////////
void test_movable_only_arguments(hpx::id_type const& target)
{
    typedef movable_only_increment_action increment_action;

    {
        increment_action inc;

        hpx::future<boost::int32_t> f1 = hpx::async(inc, target, movable_only(42));
        HPX_TEST_EQ(f1.get(), 43);

        hpx::future<boost::int32_t> f2 =
            hpx::async(hpx::launch::all, inc, target, movable_only(42));
        HPX_TEST_EQ(f2.get(), 43);
    }

    {
        increment_action inc;

        hpx::future<boost::int32_t> f1 =
            hpx::async(hpx::util::bind(hpx::util::one_shot(inc), target, movable_only(42)));
        HPX_TEST_EQ(f1.get(), 43);
    }

    {
        hpx::future<boost::int32_t> f1 =
            hpx::async<increment_action>(target, movable_only(42));
        HPX_TEST_EQ(f1.get(), 43);

        hpx::future<boost::int32_t> f2 =
            hpx::async<increment_action>(hpx::launch::all, target, movable_only(42));
        HPX_TEST_EQ(f2.get(), 43);
    }
}

///////////////////////////////////////////////////////////////////////////////
void test_movable_only_returns(hpx::id_type const& target)
{
    typedef movable_only_decrement_action decrement_action;

    {
        decrement_action inc;

        hpx::future<movable_only> f1 = hpx::async(inc, target, 42);
        HPX_TEST_EQ(f1.get().i, 41);

        hpx::future<movable_only> f2 =
            hpx::async(hpx::launch::all, inc, target, 42);
        HPX_TEST_EQ(f2.get().i, 41);
    }

    {
        decrement_action inc;

        hpx::future<movable_only> f1 =
            hpx::async(hpx::util::bind(hpx::util::one_shot(inc), target, 42));
        HPX_TEST_EQ(f1.get().i, 41);
    }

    {
        hpx::future<movable_only> f1 =
            hpx::async<decrement_action>(target, 42);
        HPX_TEST_EQ(f1.get().i, 41);

        hpx::future<movable_only> f2 =
            hpx::async<decrement_action>(hpx::launch::all, target, 42);
        HPX_TEST_EQ(f2.get().i, 41);
    }
}

int hpx_main()
{
    std::vector<hpx::id_type> localities = hpx::find_all_localities();
    BOOST_FOREACH(hpx::id_type const& id, localities)
    {
        test_remote_async(id);
        test_movable_only_arguments(id);
        test_movable_only_returns(id);
    }
    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    // Initialize and run HPX
    HPX_TEST_EQ_MSG(hpx::init(argc, argv), 0,
        "HPX main exited with non-zero status");

    return hpx::util::report_errors();
}

