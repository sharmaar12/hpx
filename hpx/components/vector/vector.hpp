//  Copyright (c) 2014 Anuj R. Sharma
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <hpx/hpx_fwd.hpp>
#include <hpx/runtime/components/server/managed_component_base.hpp>
#include <hpx/runtime/components/server/locking_hook.hpp>
#include <hpx/runtime/actions/component_action.hpp>
#include <hpx/include/lcos.hpp>
#include <hpx/include/util.hpp>
#include <hpx/include/iostreams.hpp>

#include <hpx/components/vector/chunk_vector_component.hpp>

#define VALUE_TYPE double

namespace hpx{
    class vector
    {
        typedef hpx::server::chunk_vector chunk_vector_type;
        public:
            //
            // Constructors
            //
            explicit vector()
                : num_chunks_(0), chunk_size_(0), val_(0)
                 {
                    create(0);
                 }
            explicit vector(std::size_t num_chunks, std::size_t chunk_size)
                : num_chunks_(num_chunks), chunk_size_(chunk_size), val_(0)
                 {
                    create(num_chunks, chunk_size);
                 }
            explicit vector(std::size_t num_chunks, std::size_t chunk_size, VALUE_TYPE val)
                : num_chunks_(num_chunks), chunk_size_(chunk_size), val_(val)
                 {
                    create(num_chunks, chunk_size, val);
                 }
            //
            // size API's in vector class
            //

            std::size_t size()
            {
                return size_helper(chunk_gid_.size() - 1).get();
            }

            hpx::lcos::future<std::size_t> size_async()
            {
                return size_helper(chunk_gid_.size() - 1);
            }

            //
            // push_back API's in vector class
            //
            void push_back(VALUE_TYPE const& val)
            {
                hpx::stubs::chunk_vector::push_back_sync((chunk_gid_.back()).get(), val);
            }

            //
            //  get_value API's in vector class
            //
            VALUE_TYPE get_value(std::size_t pos) const
            {
                try{
                    return hpx::stubs::chunk_vector::get_value_sync(get_gid(pos), (pos % chunk_size_));
                }
                catch(hpx::exception const& e){
                    HPX_THROW_EXCEPTION(hpx::out_of_range, "get_value", "Value of 'pos' is out of range");
                }
            }//end of get_value

            hpx::future< VALUE_TYPE > get_value_async(std::size_t pos) const
            {
                try{
                    return hpx::stubs::chunk_vector::get_value_async(get_gid(pos), (pos % chunk_size_));
                }
                catch(hpx::exception const& e){
                    HPX_THROW_EXCEPTION(hpx::out_of_range, "get_value_async", "Value of 'pos' is out of range");
                }
            }//end of get_value_async

            //
            //  set_value API's in vector class
            //
            void set_value(std::size_t pos, VALUE_TYPE val)
            {
                try{
                    hpx::stubs::chunk_vector::set_value_sync(get_gid(pos), (pos % chunk_size_), val);
                }
                catch(hpx::exception const& e){
                    HPX_THROW_EXCEPTION(hpx::out_of_range, "set_value", "Value of 'pos' is out of range");
                }
            }//end of get_value

            void set_value_non_blocking(std::size_t pos, VALUE_TYPE val)
            {
                try{
                    return hpx::stubs::chunk_vector::set_value_non_blocking(get_gid(pos), (pos % chunk_size_), val);
                }
                catch(hpx::exception const& e){
                    HPX_THROW_EXCEPTION(hpx::out_of_range, "set_value_non_blocking", "Value of 'pos' is out of range");
                }
            }//end of get_value_async

            //
            //  empty API in vector class
            //
            bool empty()
            {
                if(chunk_gid_.empty() || chunk_size_ == 0)
                    return true;
                else
                    return false;
            }

            hpx::lcos::future<bool> empty_async()
            {
                return hpx::make_ready_future(empty());
            }

            //
            // clear API in vector class
            //

            void clear()
            {
                chunk_gid_.clear();
                base_index_.clear();
                num_chunks_ = 0;
                chunk_size_ = 0;
            }

            //
            // Destructor
            //
            ~vector()
            {
                //DEFAULT destructor
            }
        protected:
        private:

            void create(std::size_t num_chunks, std::size_t chunk_size = 0, VALUE_TYPE val = 0)
            {
                for (std::size_t chunk_index = 0; chunk_index < num_chunks; ++chunk_index)
                {
                    base_index_.push_back( chunk_index * chunk_size );
                    chunk_gid_.push_back(hpx::components::new_<chunk_vector_type>(hpx::find_here(), chunk_size, val));
                }
            } // End of create function

            hpx::naming::id_type get_gid(std::size_t pos) const
            {
                //TODO simplest logic only when chunk_size is constant (As it is user error I am putting the exception rather than assertion)
                try{
                    return chunk_gid_.at( pos /chunk_size_ ).get();
                }
                catch(const std::out_of_range& e){
                    HPX_THROW_EXCEPTION(hpx::out_of_range, "get_gid", "Value of 'pos' is out of range");
                }
            }//End of gid

            hpx::lcos::future <std::size_t> size_helper(std::size_t num_chunks)
            {
                if(num_chunks < 1)
                {
                    HPX_ASSERT(num_chunks >= 0);
                    return hpx::stubs::chunk_vector::size_async((chunk_gid_.at(num_chunks)).get());
                }
                else
                    return hpx::lcos::local::dataflow(
                [](hpx::lcos::future<std::size_t> s1, hpx::lcos::future<std::size_t> s2) -> std::size_t{
                                        return s1.get() + s2.get();
                                    },
                    hpx::stubs::chunk_vector::size_async((chunk_gid_.at(num_chunks)).get()),
                    size_helper(num_chunks - 1)
                                    );
            }//end of size_t

            std::size_t num_chunks_;  /**< Represent number of chunks the vector is divided into */
            std::size_t chunk_size_;        /**< Represent the size of the single chunk */
            VALUE_TYPE val_;                /**< Default value assigned to the elements of vector */
            std::vector< std::size_t > base_index_;/**< Represent the base index of each chunk */
            //Taken as future of hpx_id's as it delay the .get() as far as possible // shared future is mandatory as .get() can be called any number of time
            std::vector< hpx::lcos::shared_future< hpx::naming::id_type > > chunk_gid_;/**< Represent the Global ID's of each chunk */

    };//end of class vector

} // end of hpx namespace

#endif // VECTOR_HPP
