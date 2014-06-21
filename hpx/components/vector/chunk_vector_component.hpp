//  Copyright (c) 2014 Anuj R. Sharma
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/*! \file Chunk_vector.hpp */
#ifndef CHUNK_VECTOR_COMPONENT_HPP
#define CHUNK_VECTOR_COMPONENT_HPP

#include <hpx/hpx_fwd.hpp>
#include <hpx/runtime/components/server/managed_component_base.hpp>
#include <hpx/runtime/components/server/locking_hook.hpp>
#include <hpx/runtime/actions/component_action.hpp>
#include <hpx/include/lcos.hpp>
#include <hpx/include/util.hpp>
#include <hpx/include/iostreams.hpp>

#include <vector>
#include <iostream>
#include <boost/assign/std.hpp>

#define VALUE_TYPE double

/**
 *  @namespace hpx
 *  @brief This is the brief description
 *
 *  This is the hpx namespace
 *
 *
 */


namespace hpx
{
    namespace server
    {

        //  template <typename VALUE_TYPE>
        class HPX_COMPONENT_EXPORT chunk_vector : public hpx::components::locking_hook<
            hpx::components::managed_component_base<chunk_vector> >
        {
        public:
            //
            //Constructors
            //
            explicit chunk_vector(): chunk_vector_(0,0) {}

            explicit chunk_vector(std::size_t chunk_size)
                : chunk_vector_(chunk_size,0) {}

            explicit chunk_vector(std::size_t chunk_size, VALUE_TYPE val)
                : chunk_vector_(chunk_size, val) {}

            //
            //Destructor (Non Virtual)
            //
            ~chunk_vector()
            {
            }

            //
            //Capacity Related API's in the server class
            //
            std::size_t size() const
            {
                return chunk_vector_.size();
            }

            std::size_t max_size() const
            {
                return chunk_vector_.max_size();
            }

            void resize_only (std::size_t n)
            {
                chunk_vector_.resize(n);
            }
            void resize_with_val (std::size_t n, VALUE_TYPE const& val)
            {
                chunk_vector_.resize(n, val);
            }

            std::size_t capacity() const
            {
                return chunk_vector_.capacity();
            }

            bool empty() const
            {
                return chunk_vector_.empty();
            }

            void reserve(std::size_t n)
            {
                try
                {
                    chunk_vector_.reserve(n);
                }
                catch(std::length_error const& le)
                {
                    HPX_THROW_EXCEPTION(hpx::length_error, "reserve" , "Can not reserve the space for vector of given size");
                }
            }

            //
            // Element access API's
            //
            VALUE_TYPE get_value(std::size_t pos) const
            {
                try
                {
                    return chunk_vector_.at(pos);
                }
                catch(const std::out_of_range& e)
                {
                    HPX_THROW_EXCEPTION(hpx::out_of_range, "set_value", "Value of 'pos' is out of range");
                }
            }

            VALUE_TYPE front() const
            {
                return chunk_vector_.front();
            }

            VALUE_TYPE back() const
            {
                return chunk_vector_.back();
            }

            //
            // Modifiers API's in server class
            //
            void assign(std::size_t n, VALUE_TYPE const& val)
            {
                chunk_vector_.assign(n, val);
            }

            void push_back(VALUE_TYPE const& val)
            {
                chunk_vector_.push_back(val);
            }
            void push_back_rval(VALUE_TYPE&& val)
            {
                chunk_vector_.push_back(std::move(val));
            }

            void pop_back()
            {
                chunk_vector_.pop_back();
            }

            //this API is required as we do not returning the reference to the element in Any API.
            void set_value(std::size_t pos, VALUE_TYPE const& val)
            {
                try
                {
                    chunk_vector_.at(pos) = val;
                }
                catch(const std::out_of_range& e)
                {
                    HPX_THROW_EXCEPTION(hpx::out_of_range, "set_value", "Value of 'pos' is out of range");
                }
            }
            void set_value_rval(std::size_t pos, VALUE_TYPE const&& val)
            {
                try
                {
                    chunk_vector_.at(pos) = std::move(val);
                }
                catch(const std::out_of_range& e)
                {
                    HPX_THROW_EXCEPTION(hpx::out_of_range, "set_value_rval", "Value of 'pos' is out of range");
                }
            }

//            void swap(hpx::server::chunk_vector x){
//                this->chunk_vector_.swap(x.chunk_vector_);
//            }

            void clear()
            {
                chunk_vector_.clear();
            }

            //
            //Define the component action here
            //

            //capacity related component action
            HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, size);
            HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, max_size);
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, resize_only);
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, resize_with_val);
            HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, capacity);
            HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, empty);
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, reserve);

            //Element access component action
            HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, get_value);
            HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, front);
            HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, back);

            //Modifiers component action
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, assign);
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, push_back);
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, push_back_rval);
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, pop_back);
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, set_value);
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, set_value_rval);
//            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, swap);
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, clear);

        private:
            std::vector<VALUE_TYPE> chunk_vector_;
        };//end of class chunk_vector

    }//end of server namespace

    namespace stubs
    {
        struct chunk_vector : hpx::components::stub_base <server::chunk_vector>
        {
            //
            //  Capacity related API's in stubs class
            //

            //SIZE
            static hpx::lcos::future<std::size_t> size_async(hpx::naming::id_type const& gid)
            {
                return hpx::async<hpx::server::chunk_vector::size_action>(gid);
            }

//            static std::size_t size_sync(hpx::naming::id_type const& gid)
//            {
//                return hpx::async<hpx::server::chunk_vector::size_action>(gid).get();
//            }

            //MAX_SIZE
            static hpx::lcos::future<std::size_t> max_size_async(hpx::naming::id_type const& gid)
            {
                return hpx::async<hpx::server::chunk_vector::max_size_action>(gid);
            }

//            static std::size_t max_size_sync(hpx::naming::id_type const& gid)
//            {
//                return hpx::async<hpx::server::chunk_vector::max_size_action>(gid).get();
//            }

            //RESIZE_ONLY
            static hpx::lcos::future<void> resize_only_async(hpx::naming::id_type const& gid, std::size_t n)
            {
                return hpx::async<hpx::server::chunk_vector::resize_only_action>(gid, n);
            }
//            static void resize_only_non_blocking(hpx::naming::id_type const& gid, std::size_t n)
//            {
//                hpx::apply<hpx::server::chunk_vector::resize_only_action>(gid, n);
//            }

            //RESIZE_WITH_VAL
            static hpx::lcos::future<void> resize_with_val_async(hpx::naming::id_type const& gid, std::size_t n, VALUE_TYPE const& val)
            {
                return hpx::async<hpx::server::chunk_vector::resize_with_val_action>(gid, n, val);
            }
//            static void resize_with_val_non_blocking(hpx::naming::id_type const& gid, std::size_t n, VALUE_TYPE const& val)
//            {
//                hpx::apply<hpx::server::chunk_vector::resize_with_val_action>(gid, n, val);
//            }


            //CAPACITY
            static hpx::lcos::future<std::size_t> capacity_async(hpx::naming::id_type const& gid)
            {
                return hpx::async<hpx::server::chunk_vector::capacity_action>(gid);
            }
//
//            static std::size_t capacity_sync(hpx::naming::id_type const& gid)
//            {
//                return hpx::async<hpx::server::chunk_vector::capacity_action>(gid).get();
//            }

            //EMPTY
            static hpx::lcos::future<bool> empty_async(hpx::naming::id_type const& gid)
            {
                return hpx::async<hpx::server::chunk_vector::empty_action>(gid);
            }

//            static bool empty_sync(hpx::naming::id_type const& gid)
//            {
//                return hpx::async<hpx::server::chunk_vector::empty_action>(gid).get();
//            }

            //RESERVE
            static hpx::lcos::future<void> reserve_async(hpx::naming::id_type const& gid, std::size_t n)
            {
                return hpx::async<hpx::server::chunk_vector::reserve_action>(gid, n);
            }
//            static void reserve_non_blocking(hpx::naming::id_type const& gid, std::size_t n)
//            {
//                hpx::apply<hpx::server::chunk_vector::reserve_action>(gid, n);
//            }


            //
            //  Element Access API's in stubs class
            //

            //GET_VALUE
            static hpx::lcos::future<VALUE_TYPE> get_value_async(hpx::naming::id_type const& gid, std::size_t pos)
            {
                return hpx::async<hpx::server::chunk_vector::get_value_action>(gid, pos);
            }

//            static VALUE_TYPE get_value_sync(hpx::naming::id_type const& gid, std::size_t pos)
//            {
//                return hpx::async<hpx::server::chunk_vector::get_value_action>(gid, pos).get();
//            }

            //FRONT
            static hpx::lcos::future<VALUE_TYPE> front_async(hpx::naming::id_type const& gid)
            {
                return hpx::async<hpx::server::chunk_vector::front_action>(gid);
            }

//            static VALUE_TYPE front_sync(hpx::naming::id_type const& gid)
//            {
//                return hpx::async<hpx::server::chunk_vector::front_action>(gid).get();
//            }

            //BACK
            static hpx::lcos::future<VALUE_TYPE> back_async(hpx::naming::id_type const& gid)
            {
                return hpx::async<hpx::server::chunk_vector::back_action>(gid);
            }

//            static VALUE_TYPE back_sync(hpx::naming::id_type const& gid)
//            {
//                return hpx::async<hpx::server::chunk_vector::back_action>(gid).get();
//            }

            //
            //  Modifiers API's in stubs class
            //

            //ASSIGN
            static hpx::lcos::future<void> assign_async(hpx::naming::id_type const& gid, std::size_t n, VALUE_TYPE const& val)
            {
                return hpx::async<hpx::server::chunk_vector::assign_action>(gid, n, val);
            }
//            static void assign_non_blocking(hpx::naming::id_type const& gid, std::size_t n, VALUE_TYPE const& val)
//            {
//                hpx::apply<hpx::server::chunk_vector::assign_action>(gid, n, val);
//            }

            //PUSH_BACK
            static hpx::lcos::future<void> push_back_async(hpx::naming::id_type const& gid, VALUE_TYPE const& val)
            {
                return hpx::async<hpx::server::chunk_vector::push_back_action>(gid, val);
            }
//            static void push_back_non_blocking(hpx::naming::id_type const& gid, VALUE_TYPE const& val)
//            {
//                hpx::apply<hpx::server::chunk_vector::push_back_action>(gid, val);
//            }


            //PUSH_BACK_RVAL
            static hpx::lcos::future<void> push_back_rval_async(hpx::naming::id_type const& gid, VALUE_TYPE const&& val)
            {
                return hpx::async<hpx::server::chunk_vector::push_back_rval_action>(gid, std::move(val));
            }
//            static void push_back_rval_non_blocking(hpx::naming::id_type const& gid, VALUE_TYPE const&& val)
//            {
//                hpx::apply<hpx::server::chunk_vector::push_back_rval_action>(gid, std::move(val));
//            }

            //POP_BACK
            static hpx::lcos::future<void> pop_back_async(hpx::naming::id_type const& gid)
            {
                return hpx::async<hpx::server::chunk_vector::pop_back_action>(gid);
            }
//            static void pop_back_non_blocking(hpx::naming::id_type const& gid)
//            {
//                hpx::apply<hpx::server::chunk_vector::pop_back_action>(gid);
//            }

            //SET_VALUE
            static hpx::lcos::future<void> set_value_async(hpx::naming::id_type const& gid, std::size_t pos, VALUE_TYPE const& val)
            {
                return hpx::async<hpx::server::chunk_vector::set_value_action>(gid, pos, val);
            }
//            static void set_value_non_blocking(hpx::naming::id_type const& gid, std::size_t pos, VALUE_TYPE const& val)
//            {
//                hpx::apply<hpx::server::chunk_vector::set_value_action>(gid, pos, val);
//            }

            //SET_VALUE_RVAL
            static hpx::lcos::future<void> set_value_rval_async(hpx::naming::id_type const& gid, std::size_t pos, VALUE_TYPE const&& val)
            {
                return hpx::async<hpx::server::chunk_vector::set_value_rval_action>(gid, pos, std::move(val));
            }
//            static void set_value_rval_non_blocking(hpx::naming::id_type const& gid, std::size_t pos,  VALUE_TYPE const&& val)
//            {
//                hpx::apply<hpx::server::chunk_vector::set_value_rval_action>(gid, pos, std::move(val));
//            }

            //CLEAR
            static hpx::lcos::future<void> clear_async(hpx::naming::id_type const& gid)
            {
                return hpx::async<server::chunk_vector::clear_action>(gid);
            }
//            static void clear_non_blocking(hpx::naming::id_type const& gid)
//            {
//                hpx::apply<server::chunk_vector::clear_action>(gid);
//            }



        };//end of chunk_vector(stubs)
    }//end of the namespace stubs


    class chunk_vector : public hpx::components::client_base<chunk_vector, stubs::chunk_vector>
    {
        typedef hpx::components::client_base<chunk_vector, stubs::chunk_vector> base_type;
    public:


        //TODO flag the error of invalid gid for default constructor
        chunk_vector() {}
        chunk_vector(hpx::naming::id_type const& gid): base_type(gid) {}
        chunk_vector(hpx::shared_future<hpx::naming::id_type> const& gid): base_type(gid) {}

        //Destructor
        ~chunk_vector()
        {
        }

        //
        //  Capacity related API's in chunk_vector client class
        //

        //SIZE
        hpx::lcos::future<std::size_t> size_async() const
        {
            HPX_ASSERT(this->get_gid());
            return this->base_type::size_async(this->get_gid());
        }

        std::size_t size() const
        {
            HPX_ASSERT(this->get_gid());
            return (this->base_type::size_async(this->get_gid())).get();
        }

        //MAX_SIZE
        hpx::lcos::future<std::size_t> max_size_async() const
        {
            HPX_ASSERT(this->get_gid());
            return this->base_type::max_size_async(this->get_gid());
        }

        std::size_t max_size() const
        {
            HPX_ASSERT(this->get_gid());
            return (this->base_type::max_size_async(this->get_gid())).get();
        }

        //RESIZE (for resize_only)
        void resize(std::size_t n)
        {
            HPX_ASSERT(this->get_gid());
            (this->base_type::resize_only_async(this->get_gid(), n)).get();
        }
//        void resize_non_blocking(std::size_t n)
//        {
//            HPX_ASSERT(this->get_gid());
//            this->base_type::resize_only_non_blocking(this->get_gid(), n);
//        }

        //RESIZE (fro resize_with_val)
        void resize(std::size_t n, VALUE_TYPE const& val)
        {
            HPX_ASSERT(this->get_gid());
            (this->base_type::resize_with_val_async(this->get_gid(), n, val)).get();
        }
//        void resize_non_blocking(std::size_t n, VALUE_TYPE const& val)
//        {
//            HPX_ASSERT(this->get_gid());
//            this->base_type::resize_with_val_non_blocking(this->get_gid(), n, val);
//        }

        //CAPACITY
        hpx::lcos::future<std::size_t> capacity_async() const
        {
            HPX_ASSERT(this->get_gid());
            return this->base_type::capacity_async(this->get_gid());
        }

        std::size_t capacity() const
        {
            HPX_ASSERT(this->get_gid());
            return (this->base_type::capacity_async(this->get_gid())).get();
        }

        //EMPTY
        hpx::lcos::future<bool> empty_async() const
        {
            HPX_ASSERT(this->get_gid());
            return this->base_type::empty_async(this->get_gid());
        }

        bool empty() const
        {
            HPX_ASSERT(this->get_gid());
            return (this->base_type::empty_async(this->get_gid())).get();
        }

        //RESERVE
        void reserve(std::size_t n)
        {
            HPX_ASSERT(this->get_gid());
            (this->base_type::reserve_async(this->get_gid(), n)).get();
        }
//        void reserve_non_blocking(std::size_t n)
//        {
//            HPX_ASSERT(this->get_gid());
//            this->base_type::reserve_non_blocking(this->get_gid(), n);
//        }


        //
        //  Element Access API's in Client class
        //

        //GET_VALUE
        hpx::lcos::future<VALUE_TYPE> get_value_async(std::size_t pos) const
        {
            HPX_ASSERT(this->get_gid());
            return this->base_type::get_value_async(this->get_gid(), pos);
        }

        VALUE_TYPE get_value(std::size_t pos) const
        {
            HPX_ASSERT(this->get_gid());
            return (this->base_type::get_value_async(this->get_gid(), pos)).get();
        }

        //FRONT
        hpx::lcos::future<VALUE_TYPE> front_async() const
        {
            HPX_ASSERT(this->get_gid());
            return this->base_type::front_async(this->get_gid());
        }

        VALUE_TYPE front() const
        {
            HPX_ASSERT(this->get_gid());
            return (this->base_type::front_async(this->get_gid())).get();
        }

        //BACK
        hpx::lcos::future<VALUE_TYPE> back_async() const
        {
            HPX_ASSERT(this->get_gid());
            return this->base_type::back_async(this->get_gid());
        }

        VALUE_TYPE back() const
        {
            HPX_ASSERT(this->get_gid());
            return (this->base_type::back_async(this->get_gid())).get();
        }

        //
        //  Modifiers API's in client class
        //
        void assign(std::size_t n, VALUE_TYPE const& val)
        {
            HPX_ASSERT(this->get_gid());
            (this->base_type::assign_async(this->get_gid(), n, val)).get();
        }
//        void assign_non_blocking(std::size_t n, VALUE_TYPE const& val)
//        {
//            HPX_ASSERT(this->get_gid());
//            this->base_type::assign_non_blocking(this->get_gid(), n, val);
//        }

        //PUSH_BACK
        void push_back(VALUE_TYPE const& val)
        {
            HPX_ASSERT(this->get_gid());
            (this->base_type::push_back_async(this->get_gid(), val)).get();
        }
//        void push_back_non_blocking(VALUE_TYPE const& val)
//        {
//            HPX_ASSERT(this->get_gid());
//            this->base_type::push_back_non_blocking(this->get_gid(), val);
//        }

        //PUSH_BACK (for push_back_rval)
        void push_back(VALUE_TYPE const&& val)
        {
            HPX_ASSERT(this->get_gid());
            (this->base_type::push_back_rval_async(this->get_gid(), std::move(val))).get();
        }
//        void push_back_non_blocking(VALUE_TYPE const&& val)
//        {
//            HPX_ASSERT(this->get_gid());
//            this->base_type::push_back_rval_non_blocking(this->get_gid(), std::move(val));
//        }

        //POP_BACK
        void pop_back()
        {
            HPX_ASSERT(this->get_gid());
            (this->base_type::pop_back_async(this->get_gid())).get();
        }
//        void pop_back_non_blocking()
//        {
//            HPX_ASSERT(this->get_gid());
//            this->base_type::pop_back_non_blocking(this->get_gid());
//        }
//
        //SET_VALUE
        void set_value(std::size_t pos, VALUE_TYPE const& val)
        {
            HPX_ASSERT(this->get_gid());
            (this->base_type::set_value_async(this->get_gid(), pos, val)).get();
        }
//        void set_value_non_blocking(std::size_t pos, VALUE_TYPE const& val)
//        {
//            HPX_ASSERT(this->get_gid());
//            this->base_type::set_value_non_blocking(this->get_gid(), pos, val);
//        }

        //SET_VALUE (for set_value rval)
        void set_value(std::size_t pos, VALUE_TYPE const&& val)
        {
            HPX_ASSERT(this->get_gid());
            (this->base_type::set_value_rval_async(this->get_gid(), pos, std::move(val))).get();
        }
//        void set_value_non_blocking(std::size_t pos, VALUE_TYPE const&& val)
//        {
//            HPX_ASSERT(this->get_gid());
//            this->base_type::set_value_rval_non_blocking(this->get_gid(), pos, std::move(val));
//        }

        //CLEAR
        void clear()
        {
            HPX_ASSERT(this->get_gid());
            (this->base_type::clear_async(this->get_gid()));
        }
//        void clear_non_blocking()
//        {
//            HPX_ASSERT(this->get_gid());
//            this->base_type::clear_non_blocking(this->get_gid());
//        }

    };//end of chunk_vector (client)

}//end of hpx namespace

//Registering the component action to AGAS

//Capacity related action declaration
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::size_action,
    chunk_vector_size_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::max_size_action,
    chunk_vector_max_size_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::resize_only_action,
    chunk_vector_resize_only_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::resize_with_val_action,
    chunk_vector_resize_with_val_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::capacity_action,
    chunk_vector_capacity_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::empty_action,
    chunk_vector_empty_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::reserve_action,
    chunk_vector_reserve_action);

//Element access component action declaration
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::get_value_action,
    chunk_vector_get_value_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::front_action,
    chunk_vector_front_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::back_action,
    chunk_vector_back_action);

//Modifiers component action declaration
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::assign_action,
    chunk_vector_assign_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::push_back_action,
    chunk_vector_push_back_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::push_back_rval_action,
    chunk_vector_push_back_rval_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::pop_back_action,
    chunk_vector_pop_back_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::set_value_action,
    chunk_vector_set_value_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::set_value_rval_action,
    chunk_vector_set_value_rval_action);
//HPX_REGISTER_ACTION_DECLARATION(
//    hpx::server::chunk_vector::swap_action,
//    chunk_vector_swap_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::clear_action,
    chunk_vector_clear_action);

#endif // CHUNK_VECTOR_COMPONENT_HPP
