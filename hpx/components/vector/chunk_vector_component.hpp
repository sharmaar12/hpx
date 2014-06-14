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


namespace hpx{
        namespace server{

          //  template <typename VALUE_TYPE>
            class HPX_COMPONENT_EXPORT chunk_vector : public hpx::components::locking_hook<
                hpx::components::managed_component_base<chunk_vector> >
            {
                typedef VALUE_TYPE chunk_value_type;
                public:
                    explicit chunk_vector(): chunk_size_(0), val_(0), chunk_vector_(0,0) {}

                    explicit chunk_vector(size_t chunk_size)
                    : chunk_size_(chunk_size), val_(0), chunk_vector_(chunk_size,0) {}

                    explicit chunk_vector(size_t chunk_size, VALUE_TYPE val)
                    : chunk_size_(chunk_size), val_(val), chunk_vector_(chunk_size, val) {}

                    void display() const
                    {
                        if(chunk_vector_.empty())
                            hpx::cout << "Chunk Vector is empty \n" << hpx::flush;
                        else{
                             std::vector<VALUE_TYPE>::const_iterator it = chunk_vector_.begin();
                            for(;it != chunk_vector_.end(); it++ )
                                hpx::cout << *it << "\n" << hpx::flush;
                        }
                    } //end of display

                    size_t size() const{
                        return chunk_vector_.size();
                    }//end of size

                    void push_back(VALUE_TYPE const& val){
                        chunk_vector_.push_back(val);
                    }
                    void push_back_rval(VALUE_TYPE&& val){
                        chunk_vector_.push_back(std::move(val));
                    }
                    VALUE_TYPE at(size_t pos){
                        return chunk_vector_.at(pos);
                    }
                    void set_value(size_t pos, VALUE_TYPE const& val)
                    {

                        try{
                            chunk_vector_.at(pos) = val;
                        }
                        catch(const std::out_of_range& e)
                        {
                            HPX_THROW_EXCEPTION(hpx::out_of_range, "set_value", "Out of range error");
                        }
                    }

                    void set_value_rval(size_t pos, VALUE_TYPE const&& val)
                    {
                         try{
                            chunk_vector_.at(pos) = std::move(val);
                        }
                        catch(const std::out_of_range& e)
                        {
                            HPX_THROW_EXCEPTION(hpx::out_of_range, "set_value_rval", "Out of range error");
                        }
                    }

                    bool empty() const{
                        return chunk_vector_.empty();
                    }

                    void clear(){
                        chunk_vector_.clear();
                    }


                    virtual ~chunk_vector(){

                        }
                //
                //Define the component action here
                //
                HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, display);
                HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, size);
                HPX_DEFINE_COMPONENT_ACTION(chunk_vector, push_back);
                HPX_DEFINE_COMPONENT_ACTION(chunk_vector, push_back_rval);
                HPX_DEFINE_COMPONENT_ACTION(chunk_vector, at);
                HPX_DEFINE_COMPONENT_ACTION(chunk_vector, set_value);
                HPX_DEFINE_COMPONENT_ACTION(chunk_vector, set_value_rval);
                HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, empty);
                HPX_DEFINE_COMPONENT_ACTION(chunk_vector, clear);

                private:
                    size_t chunk_size_;
                    VALUE_TYPE val_;
                    std::vector<VALUE_TYPE> chunk_vector_;
            };//end of class chunk_vector

        }//end of server namespace

        namespace stubs{
            struct chunk_vector : hpx::components::stub_base <server::chunk_vector>{
                //
                //  Display API's in stub class
                //

                static void display_non_blocking(hpx::naming::id_type const& gid)
                {
                    hpx::apply<server::chunk_vector::display_action>(gid);
                }

                static void display_sync(hpx::naming::id_type const& gid)
                {
                    hpx::async<server::chunk_vector::display_action>(gid).get();
                }

                //
                //  Size API's in stub class
                //

                static hpx::lcos::future<size_t> size_async(hpx::naming::id_type const& gid)
                {
                    return hpx::async<hpx::server::chunk_vector::size_action>(gid);
                }

                static size_t size_sync(hpx::naming::id_type const& gid)
                {
                    return hpx::async<hpx::server::chunk_vector::size_action>(gid).get();
                }

                //
                //Push_back API's in stub class
                //
                static void push_back_non_blocking(hpx::naming::id_type const& gid, VALUE_TYPE const& val)
                {
                    hpx::apply<hpx::server::chunk_vector::push_back_action>(gid, val);
                }

                static void push_back_sync(hpx::naming::id_type const& gid, VALUE_TYPE const& val)
                {
                    hpx::async<hpx::server::chunk_vector::push_back_action>(gid, val).get();
                }

                //
                //  Push_back_rval API's in stub class
                //
                static void push_back_rval_non_blocking(hpx::naming::id_type const& gid, VALUE_TYPE const&& val)
                {
                    hpx::apply<hpx::server::chunk_vector::push_back_rval_action>(gid, std::move(val));
                }

                static void push_back_rval_sync(hpx::naming::id_type const& gid, VALUE_TYPE const&& val)
                {
                    hpx::async<hpx::server::chunk_vector::push_back_rval_action>(gid, std::move(val)).get();
                }

                //
                // at API's in stub class
                //

                static hpx::lcos::future<VALUE_TYPE> at_async(hpx::naming::id_type const& gid, size_t pos)
                {
                    return hpx::async<hpx::server::chunk_vector::at_action>(gid, pos);
                }

                static VALUE_TYPE at_sync(hpx::naming::id_type const& gid, size_t pos)
                {
                    return hpx::async<hpx::server::chunk_vector::at_action>(gid, pos).get();
                }

                //
                //  set_value API's in stub class
                //
                static void set_value_non_blocking(hpx::naming::id_type const& gid, size_t pos, VALUE_TYPE const& val)
                {
                    hpx::apply<hpx::server::chunk_vector::set_value_action>(gid, pos, val);
                }

                static void set_value_sync(hpx::naming::id_type const& gid, size_t pos, VALUE_TYPE const& val)
                {
                    hpx::async<hpx::server::chunk_vector::set_value_action>(gid, pos, val).get();
                }

                //
                //  set_value_rval API's in stub class
                //
                static void set_value_rval_non_blocking(hpx::naming::id_type const& gid, size_t pos,  VALUE_TYPE const&& val)
                {
                    hpx::apply<hpx::server::chunk_vector::set_value_rval_action>(gid, pos, std::move(val));
                }

                static void set_value_rval_sync(hpx::naming::id_type const& gid, size_t pos, VALUE_TYPE const&& val)
                {
                    hpx::async<hpx::server::chunk_vector::set_value_rval_action>(gid, pos, std::move(val)).get();
                }

                //
                //  empty API's in stub class
                //

                static hpx::lcos::future<bool> empty_async(hpx::naming::id_type const& gid)
                {
                    return hpx::async<hpx::server::chunk_vector::empty_action>(gid);
                }

                static bool empty_sync(hpx::naming::id_type const& gid)
                {
                    return hpx::async<hpx::server::chunk_vector::empty_action>(gid).get();
                }

                //
                //  clear API's in client class
                //
                static void clear_non_blocking(hpx::naming::id_type const& gid)
                {
                    hpx::apply<server::chunk_vector::clear_action>(gid);
                }

                static void clear_sync(hpx::naming::id_type const& gid)
                {
                    hpx::async<server::chunk_vector::clear_action>(gid).get();
                }


            };//end of chunk_vector(stubs)
        }//end of the namespace stubs


            class chunk_vector : public hpx::components::client_base<chunk_vector, stubs::chunk_vector>
            {
                 typedef hpx::components::client_base<chunk_vector, stubs::chunk_vector> base_type;
            public:


                //TODO flag the error of invalid gid for default constructor
                chunk_vector(){}
                chunk_vector(hpx::naming::id_type const& gid): base_type(gid){}
                chunk_vector(hpx::shared_future<hpx::naming::id_type> const& gid): base_type(gid){}
                //
                // Display API's in client class
                //
                void display_non_blocking() const
                {
                    HPX_ASSERT(this->get_gid());
                    this->base_type::display_non_blocking(this->get_gid());
                }

                void display() const
                {
                    HPX_ASSERT(this->get_gid());
                    this->base_type::display_sync(this->get_gid());
                }

                //
                //  Size API's in client class
                //

                hpx::lcos::future<size_t> size_async() const
                {
                    HPX_ASSERT(this->get_gid());
                    return this->base_type::size_async(this->get_gid());
                }

                size_t size() const
                {
                    HPX_ASSERT(this->get_gid());
                    return this->base_type::size_sync(this->get_gid());
                }

                //
                //  Push_back API's in Client class
                //

                void push_back_non_blocking(VALUE_TYPE const& val)
                {
                    HPX_ASSERT(this->get_gid());
                    this->base_type::push_back_non_blocking(this->get_gid(), val);
                }

                void push_back(VALUE_TYPE const& val)
                {
                    HPX_ASSERT(this->get_gid());
                    this->base_type::push_back_sync(this->get_gid(), val);
                }

                //
                //  Push_back_rval API's in Client class [NOTE it is overloading of push_back]
                //
                void push_back_non_blocking(VALUE_TYPE const&& val)
                {
                    HPX_ASSERT(this->get_gid());
                    this->base_type::push_back_rval_non_blocking(this->get_gid(), std::move(val));
                }

                void push_back(VALUE_TYPE const&& val)
                {
                    HPX_ASSERT(this->get_gid());
                    this->base_type::push_back_rval_sync(this->get_gid(), std::move(val));
                }

                //
                // at API's in client class
                //

                hpx::lcos::future<VALUE_TYPE> get_value_async(size_t pos) const
                {
                    HPX_ASSERT(this->get_gid());
                    return this->base_type::at_async(this->get_gid(), pos);
                }

                VALUE_TYPE get_value(size_t pos) const
                {
                    HPX_ASSERT(this->get_gid());
                    return this->base_type::at_sync(this->get_gid(), pos);
                }

                //
                //  set_value API's in client class
                //
                void set_value_non_blocking(size_t pos, VALUE_TYPE const& val)
                {
                    HPX_ASSERT(this->get_gid());
                    this->base_type::set_value_non_blocking(this->get_gid(), pos, val);
                }

                void set_value(size_t pos, VALUE_TYPE const& val)
                {
                    HPX_ASSERT(this->get_gid());
                    this->base_type::set_value_sync(this->get_gid(), pos, val);
                }


                //
                //  set_value_rval API's in client class [NOTE it is overloading if the set_value function]
                //
                void set_value_non_blocking(size_t pos, VALUE_TYPE const&& val)
                {
                    HPX_ASSERT(this->get_gid());
                    this->base_type::set_value_rval_non_blocking(this->get_gid(), pos, std::move(val));
                }

                void set_value(size_t pos, VALUE_TYPE const&& val)
                {
                    HPX_ASSERT(this->get_gid());
                    this->base_type::set_value_rval_sync(this->get_gid(), pos, std::move(val));
                }

                //
                // empty API's in client class
                //

                hpx::lcos::future<bool> empty_async() const
                {
                    HPX_ASSERT(this->get_gid());
                    return this->base_type::empty_async(this->get_gid());
                }

                bool empty() const
                {
                    HPX_ASSERT(this->get_gid());
                    return this->base_type::empty_sync(this->get_gid());
                }

                //
                //  clear API's in client class
                //
                void clear_non_blocking()
                {
                    HPX_ASSERT(this->get_gid());
                    this->base_type::clear_non_blocking(this->get_gid());
                }

                void clear()
                {
                    HPX_ASSERT(this->get_gid());
                    this->base_type::clear_sync(this->get_gid());
                }

            };
}//end of hpx namespace

//Registering the component action to AGAS
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::display_action,
    chunk_vector_display_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::size_action,
    chunk_vector_size_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::push_back_action,
    chunk_vector_push_back_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::push_back_rval_action,
    chunk_vector_push_back_rval_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::at_action,
    chunk_vector_at_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::set_value_action,
    chunk_vector_set_value_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::set_value_rval_action,
    chunk_vector_set_value_rval_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::empty_action,
    chunk_vector_empty_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::clear_action,
    chunk_vector_clear_action);

#endif // CHUNK_VECTOR_COMPONENT_H
