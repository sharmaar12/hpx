//  Copyright (c) 2014 Anuj R. Sharma
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <hpx/hpx_fwd.hpp>
#include <hpx/runtime/actions/component_action.hpp>
#include <hpx/include/lcos.hpp>
#include <hpx/include/util.hpp>
#include <hpx/include/iostreams.hpp>

#include <boost/foreach.hpp>

#include <hpx/components/vector/chunk_vector_component.hpp>

#define VALUE_TYPE double


namespace hpx{
    class vector
    {
        typedef hpx::server::chunk_vector chunk_vector_type;

        /**< This is the pair representing the (base_index, shared_future of gid)*/
        typedef std::pair<std::size_t, hpx::lcos::shared_future<hpx::naming::id_type>> bfg_pair;
        typedef std::vector< bfg_pair > vector_type;

        private:
            //std::size_t num_chunks_;  /**< Represent number of chunks the vector is divided into */
            //std::size_t chunk_size_;        /**< Represent the size of the single chunk */
            //VALUE_TYPE val_;                /**< Default value assigned to the elements of vector */
            //std::vector< std::size_t > base_index_;/**< Represent the base index of each chunk */

            //It is the vector representing the base_index and corresponding gid's
            //Taken as future of hpx_id's as it delay the .get() as far as possible
            // shared future is mandatory as .get() can be called any number of time
            vector_type base_sf_of_gid_pair_;/**< Represent the Global ID's of each chunk */

        public:
            //
            // Constructors
            //
            explicit vector()
                //: num_chunks_(0)
                 {
                    create(0, 0, 0);
                 }

              //This constructor complicates the push_back operation as on which gid we have to push back and create function as all base are same
//            explicit vector(std::size_t num_chunks)
//            {
//                create(num_chunks, 0, 0);
//            }

            explicit vector(std::size_t num_chunks, std::size_t chunk_size)
                //: num_chunks_(num_chunks)
                 {
                    create(num_chunks, chunk_size, 0);
                 }

            explicit vector(std::size_t num_chunks, std::size_t chunk_size, VALUE_TYPE val)
                //: num_chunks_(num_chunks)
                 {
                    create(num_chunks, chunk_size, val);
                 }

            //
            // Capacity related API's in vector class
            //

            //SIZE
            std::size_t size() const
            {
                return size_helper(base_sf_of_gid_pair_.size() - 1).get();
            }
            hpx::lcos::future<std::size_t> size_async() const
            {
                return size_helper(base_sf_of_gid_pair_.size() - 1);
            }

           //MAX_SIZE
            std::size_t max_size() const
            {
                return max_size_helper(base_sf_of_gid_pair_.size() - 1).get();
            }
            hpx::lcos::future<std::size_t> max_size_async() const
            {
                return max_size_helper(base_sf_of_gid_pair_.size() - 1);
            }

            //RESIZE (without value) (SEMANTIC DIFFERENCE: It is resize with respective chunk not whole vector)
            void resize(std::size_t n)
            {
                std::vector<hpx::lcos::future<void>> resize_lazy_sync;
                BOOST_FOREACH(bfg_pair const& p, base_sf_of_gid_pair_)
                {
                    resize_lazy_sync.push_back(hpx::stubs::chunk_vector::resize_only_async((p.second).get(), n));
                }
                hpx::wait_all(resize_lazy_sync);
            }
            hpx::lcos::future<void> resize_async(std::size_t n)
            {
                //static_cast to resolve ambiguity of the overloaded function
                return hpx::async(launch::async, hpx::util::bind(static_cast<void(vector::*)(std::size_t)>(&vector::resize), this, n));
            }

            //RESIZE (with value)
            void resize(std::size_t n, VALUE_TYPE const& val)
            {
                std::vector<hpx::lcos::future<void>> resize_lazy_sync;
                BOOST_FOREACH(bfg_pair const& p, base_sf_of_gid_pair_)
                {
                    resize_lazy_sync.push_back(hpx::stubs::chunk_vector::resize_with_val_async((p.second).get(), n, val));
                }
                hpx::wait_all(resize_lazy_sync);
            }
            hpx::lcos::future<void> resize_async(std::size_t n, VALUE_TYPE const& val)
            {
                //static_cast to resolve ambiguity of the overloaded function
                return hpx::async(launch::async, hpx::util::bind(static_cast<void(vector::*)(std::size_t, VALUE_TYPE const&)>(&vector::resize), this, n, val));
            }

            //CAPACITY
            std::size_t capactiy() const
            {
                return capacity_helper(base_sf_of_gid_pair_.size() - 1).get();
            }

            hpx::lcos::future<std::size_t> capacity_async() const
            {
                return capacity_helper(base_sf_of_gid_pair_.size() - 1);
            }

            //EMPTY
            bool empty() const
            {
                if(base_sf_of_gid_pair_.empty())
                    return true;
                else
                    return !(this->size());
            }

            hpx::lcos::future<bool> empty_async() const
            {
                return hpx::async(launch::async, hpx::util::bind(&vector::empty, this));
            }

            //RESERVE
            void reserve(std::size_t n)
            {
                std::vector<hpx::lcos::future<void>> reserve_lazy_sync;
                BOOST_FOREACH(bfg_pair const& p, base_sf_of_gid_pair_)
                {
                    reserve_lazy_sync.push_back(hpx::stubs::chunk_vector::reserve_async((p.second).get(), n));
                }
                hpx::wait_all(reserve_lazy_sync);
            }
            hpx::lcos::future<void> reserve_async(std::size_t n)
            {
                return hpx::async(launch::async, hpx::util::bind(&vector::reserve, this, n));
            }


            //
            //  Element access API's in vector class
            //

            //GET_VALUE
            VALUE_TYPE get_value(std::size_t pos) const
            {
                try{
                    vector_type::const_iterator it = get_base_gid_pair(pos);
                    return hpx::stubs::chunk_vector::get_value_async((it->second).get(), (pos - (it->first))).get();
                }
                catch(hpx::exception const& e){
                    HPX_THROW_EXCEPTION(hpx::out_of_range, "get_value", "Value of 'pos' is out of range");
                }
            }//end of get_value

            hpx::future< VALUE_TYPE > get_value_async(std::size_t pos) const
            {
                try{ //Here you can call the get_val_sync API but you have already an API to do that which reduce one function call
                    vector_type::const_iterator it = get_base_gid_pair(pos);
                    return hpx::stubs::chunk_vector::get_value_async((it->second).get(), (pos - (it->first)));
                }
                catch(hpx::exception const& e){
                    HPX_THROW_EXCEPTION(hpx::out_of_range, "get_value_async", "Value of 'pos' is out of range");
                }
            }//end of get_value_async

            //FRONT (never throws exception)
            VALUE_TYPE front() const
            {
                return hpx::stubs::chunk_vector::front_async((base_sf_of_gid_pair_.front().second).get()).get();
            }//end of front_value

            hpx::future< VALUE_TYPE > front_async(std::size_t pos) const
            {
                return hpx::stubs::chunk_vector::front_async((base_sf_of_gid_pair_.front().second).get());
            }//end of front_async

            //BACK (never throws exception)
            VALUE_TYPE back() const
            {
                return hpx::stubs::chunk_vector::back_async((base_sf_of_gid_pair_.back().second).get()).get();
            }//end of back_value

            hpx::future< VALUE_TYPE > back_async(std::size_t pos) const
            {
                return hpx::stubs::chunk_vector::front_async((base_sf_of_gid_pair_.back().second).get());
            }//end of back_async

            //
            // Modifier component action
            //

            //ASSIGN
            void assign(std::size_t n, VALUE_TYPE const& val)
            {
                std::vector<hpx::lcos::future<void>> assign_lazy_sync;
                BOOST_FOREACH(bfg_pair const& p, base_sf_of_gid_pair_)
                {
                    assign_lazy_sync.push_back(hpx::stubs::chunk_vector::assign_async((p.second).get(), n, val));
                }
                hpx::wait_all(assign_lazy_sync);
            }
            hpx::lcos::future<void> assign_async(std::size_t n, VALUE_TYPE const& val)
            {
                return hpx::async(launch::async, hpx::util::bind(&vector::assign, this, n, val));
            }

            //PUSH_BACK
            void push_back(VALUE_TYPE const& val)
            {
                hpx::stubs::chunk_vector::push_back_async((base_sf_of_gid_pair_.back().second).get(), val).get();
            }

            hpx::lcos::future<void> push_back_async(VALUE_TYPE const& val)
            {
                return hpx::stubs::chunk_vector::push_back_async((base_sf_of_gid_pair_.back().second).get(), val);
            }

            //PUSH_BACK (With rval)
            void push_back(VALUE_TYPE const&& val)
            {
                hpx::stubs::chunk_vector::push_back_rval_async((base_sf_of_gid_pair_.back().second).get(), std::move(val)).get();
            }

            hpx::lcos::future<void> push_back_async(VALUE_TYPE const&& val)
            {
                return hpx::stubs::chunk_vector::push_back_rval_async((base_sf_of_gid_pair_.back().second).get(), std::move(val));
            }

            //POP_BACK
            void pop_back()
            {
                //TODO Either throw exception or prevent crash by just returning
                hpx::stubs::chunk_vector::pop_back_async((base_sf_of_gid_pair_.back().second).get()).get();
                //TODO if following change the affect back() and further pop_back function
                //checking if last element from the particular gid is popped up then delete that..
                if(hpx::stubs::chunk_vector::empty_async((base_sf_of_gid_pair_.back().second).get()).get())
                    base_sf_of_gid_pair_.pop_back();
            }

            //
            //  set_value API's in vector class
            //
            void set_value(std::size_t pos, VALUE_TYPE const& val)
            {
                try{
                    vector_type::const_iterator it = get_base_gid_pair(pos);
                    return hpx::stubs::chunk_vector::set_value_async((it->second).get(), (pos - (it->first)), val).get();
                }
                catch(hpx::exception const& e){
                    HPX_THROW_EXCEPTION(hpx::out_of_range, "set_value", "Value of 'pos' is out of range");
                }
            }//end of set_value

            hpx::lcos::future<void> set_value_async(std::size_t pos, VALUE_TYPE const& val)
            {
                   try{
                       //This reduce one function call as we are directly calling chunk vector API
                    vector_type::const_iterator it = get_base_gid_pair(pos);
                    return hpx::stubs::chunk_vector::set_value_async((it->second).get(), (pos - (it->first)), val);
                }
                catch(hpx::exception const& e){
                    HPX_THROW_EXCEPTION(hpx::out_of_range, "set_value_async", "Value of 'pos' is out of range");
                }
            }//end of set_value_async

            //SET_VALUE (with rval)
            void set_value(std::size_t pos, VALUE_TYPE const&& val)
            {
                try{
                    vector_type::const_iterator it = get_base_gid_pair(pos);
                    return hpx::stubs::chunk_vector::set_value_rval_async((it->second).get(), (pos - (it->first)), std::move(val)).get();
                }
                catch(hpx::exception const& e){
                    HPX_THROW_EXCEPTION(hpx::out_of_range, "set_value", "Value of 'pos' is out of range");
                }
            }//end of set_value

            hpx::lcos::future<void> set_value_async(std::size_t pos, VALUE_TYPE const&& val)
            {
                   try{
                    vector_type::const_iterator it = get_base_gid_pair(pos);
                    return hpx::stubs::chunk_vector::set_value_rval_async((it->second).get(), (pos - (it->first)), std::move(val));
                }
                catch(hpx::exception const& e){
                    HPX_THROW_EXCEPTION(hpx::out_of_range, "set_value_async", "Value of 'pos' is out of range");
                }
            }//end of set_value_async



            //CLEAR
            //TODO if number of chunks is kept constant every time then clear should modified (clear each chunk_vector one by one).
            void clear()
            {
                base_sf_of_gid_pair_.clear();
            }

            //
            // HPX CUSTOM API's
            //

//            //CREATE_CHUNK
//            //TODO This statement can create Data Inconsistency :
//             //If size of base_sf_of_gid_pair_ calculated and added to the base_index but not whole creation is completed and in betwen this som push_back on hpx::vector is done then that operation is losted
//            void create_chunk(hpx::naming::id locality, std::size_t chunk_size = 0, VALUE_TYPE val = 0.0)
//            {
//                base_sf_of_gid_pair_.push_back(
//                        std::make_pair(
//                            base_sf_of_gid_pair_.size(),
//                             hpx::components::new_<chunk_vector_type>(locality, chunk_size, val)
//                                      )
//                                                    );
//            }//end of create chunk


            //
            // Destructor
            //

            ~vector()
            {
                //DEFAULT destructor
            }

        protected:
            void create(std::size_t num_chunks, std::size_t chunk_size, VALUE_TYPE val)
            {
                for (std::size_t chunk_index = 0; chunk_index < num_chunks; ++chunk_index)
                {
                    base_sf_of_gid_pair_.push_back(
                        std::make_pair(
                            chunk_index * chunk_size,
                             hpx::components::new_<chunk_vector_type>(hpx::find_here(), chunk_size, val)
                                      )
                                                    );
                }
            } // End of create function


            vector_type::const_iterator get_base_gid_pair(std::size_t pos) const
            {
                //TODO simplest logic only when chunk_size is constant (As it is user error I am putting the exception rather than assertion)
                    hpx::lcos::shared_future<hpx::naming::id_type> sf;
                    if(pos < 0)
                    {
                        HPX_THROW_EXCEPTION(hpx::out_of_range, "get_gid", "Value of 'pos' is out of range");
                    }
                    else
                    {
                        //return the iterator to the first element which does not comparable less than value (i.e equal or greater)
                        vector_type::const_iterator it = std::lower_bound(base_sf_of_gid_pair_.begin(),
                                                                     base_sf_of_gid_pair_.end(),
                                                                     std::make_pair(pos, sf),
                                                                     [](bfg_pair const& middle, bfg_pair const& val)-> bool
                                                                     {
                                                                       return middle.first < val.first;
                                                                     });
                        if(it->first == pos)
                        {
                            return it;
                        }
                        else //It takes care of the case if "it" is at the last
                        {
                            return (--it);
                        }
                    }
                        //return chunk_gid_.at( pos /chunk_size_ ).get();
            }//End of get_gid


            hpx::lcos::future <std::size_t> size_helper(std::size_t num_chunks) const
            {
                if(num_chunks < 1)
                {
                    HPX_ASSERT(num_chunks >= 0);
                    return hpx::stubs::chunk_vector::size_async(((base_sf_of_gid_pair_.at(num_chunks)).second).get());
                }
                else
                    return hpx::lcos::local::dataflow(
                [](hpx::lcos::future<std::size_t> s1, hpx::lcos::future<std::size_t> s2) -> std::size_t{
                                        return s1.get() + s2.get();
                                    },
                    hpx::stubs::chunk_vector::size_async(((base_sf_of_gid_pair_.at(num_chunks)).second).get()),
                    size_helper(num_chunks - 1)
                                    );
            }//end of size_helper

            hpx::lcos::future <std::size_t> max_size_helper(std::size_t num_chunks) const
            {
                if(num_chunks < 1)
                {
                    HPX_ASSERT(num_chunks >= 0);
                    return hpx::stubs::chunk_vector::max_size_async(((base_sf_of_gid_pair_.at(num_chunks)).second).get());
                }
                else
                    return hpx::lcos::local::dataflow(
                [](hpx::lcos::future<std::size_t> s1, hpx::lcos::future<std::size_t> s2) -> std::size_t{
                                        return s1.get() + s2.get();
                                    },
                    hpx::stubs::chunk_vector::max_size_async(((base_sf_of_gid_pair_.at(num_chunks)).second).get()),
                    max_size_helper(num_chunks - 1)
                                    );
            }//end of max_size_helper


            hpx::lcos::future <std::size_t> capacity_helper(std::size_t num_chunks) const
            {
                if(num_chunks < 1)
                {
                    HPX_ASSERT(num_chunks >= 0);
                    return hpx::stubs::chunk_vector::capacity_async(((base_sf_of_gid_pair_.at(num_chunks)).second).get());
                }
                else
                    return hpx::lcos::local::dataflow(
                [](hpx::lcos::future<std::size_t> s1, hpx::lcos::future<std::size_t> s2) -> std::size_t{
                                        return s1.get() + s2.get();
                                    },
                    hpx::stubs::chunk_vector::capacity_async(((base_sf_of_gid_pair_.at(num_chunks)).second).get()),
                    capacity_helper(num_chunks - 1)
                                    );
            }//end of capacity_helper

    };//end of class vector

} // end of hpx namespace

#endif // VECTOR_HPP
