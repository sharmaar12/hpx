//  Copyright (c) 2014 Anuj R. Sharma
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <hpx/include/lcos.hpp>
#include <hpx/include/util.hpp>
#include <hpx/components/vector/segmented_iterator.hpp>
#include <hpx/components/vector/chunk_vector_component.hpp>

// headers for checking the ranges of the Datatypes
#include <cstdint>
#include <boost/integer.hpp>
#include <boost/foreach.hpp>

//TODO Remove all unnecessary comments from file

#define VALUE_TYPE double

namespace hpx{

    class vector
    {
        typedef hpx::server::chunk_vector chunk_vector_type;

        /**< This is the pair representing the (base_index, shared_future of gid)*/
        typedef std::pair<std::size_t, hpx::lcos::shared_future<hpx::naming::id_type>> bfg_pair;
        typedef std::vector< bfg_pair > vector_type;
    private:
            //It is the vector representing the base_index and corresponding gid's
            //Taken as future of hpx_id's as it delay the .get() as far as possible
            // shared future is mandatory as .get() can be called any number of time
            vector_type base_sf_of_gid_pair_;/**< Represent the Global ID's of each chunk */

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

                //Pushing the last Junk pair which indicate the LAST of the vector
                //This helps in preventing the crash of the system due to invalid state of the iterator
                hpx::naming::id_type invalid_id;
                base_sf_of_gid_pair_.push_back(
                        std::make_pair(std::numeric_limits<std::size_t>::max(), //this is maximum possible value for size_t
                                      hpx::make_ready_future(invalid_id)
                                       )
                                               );

                HPX_ASSERT(base_sf_of_gid_pair_.size()); //As this function changes the size we should have LAST always.
            } // End of create function


            vector_type::const_iterator get_base_gid_pair(std::size_t pos) const
            {
                    hpx::lcos::shared_future<hpx::naming::id_type> sf;
                    //return the iterator to the first element which does not comparable less than value (i.e equal or greater)
                    vector_type::const_iterator it = std::lower_bound(base_sf_of_gid_pair_.begin(),
                                                                          //As we don't need to compare with LAST, if not matches
                                                                          //LowerBound return the last iterator so the return should be LAST not the .end()
                                                                          //hence --it make sense or else it doesn't
                                                                     base_sf_of_gid_pair_.end() - 1,
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
            }//End of get_gid

            //Note num_chunks == represent then chunk vector index
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

        public:
            typedef hpx::segmented_vector_iterator iterator;

            //
            // Constructors
            //
            //TODO default constructor has at least one empty chunk created
            explicit vector()
            {
                create(1, 0, 0);
            }

              //This constructor complicates the push_back operation as on which gid we have to push back and create function as all base are same
            explicit vector(std::size_t num_chunks)
            {
                create(num_chunks, 0, 0);
            }

            explicit vector(std::size_t num_chunks, std::size_t chunk_size)
            {
                create(num_chunks, chunk_size, 0);
            }

            explicit vector(std::size_t num_chunks, std::size_t chunk_size, VALUE_TYPE val)
            {
                create(num_chunks, chunk_size, val);
            }

            //
            // Capacity related API's in vector class
            //

            //SIZE
            std::size_t size() const
            {
                if (base_sf_of_gid_pair_.size() == 1)
                    return 0;
                else
                //- 2 is because we have extra pair which represent LAST
                return size_helper(base_sf_of_gid_pair_.size() - 2).get();
            }
            hpx::lcos::future<std::size_t> size_async() const
            {
                if (base_sf_of_gid_pair_.size() == 0)
                    return hpx::make_ready_future(static_cast<std::size_t>(0));
                else
                //- 2 is because we have extra pair which represent LAST
                return size_helper(base_sf_of_gid_pair_.size() - 2);
            }

           //MAX_SIZE
            std::size_t max_size() const
            {
                if (base_sf_of_gid_pair_.size() == 1) //If no chunk_created then we can not push_back hence maxsize=0 make sense
                    return 0;
                else
                //- 2 is because we have extra pair which represent LAST
                return max_size_helper(base_sf_of_gid_pair_.size() - 2).get();
            }
            hpx::lcos::future<std::size_t> max_size_async() const
            {
                if (base_sf_of_gid_pair_.size() == 1) //If no chunk_created then we can not push_back hence maxsize=0 make sense
                    return hpx::make_ready_future(static_cast<std::size_t>(0));
                else
                //- 2 is because we have extra pair which represent LAST
                return max_size_helper(base_sf_of_gid_pair_.size() - 2);
            }

            //RESIZE (without value) (SEMANTIC DIFFERENCE: It is resize with respective chunk not whole vector)
            void resize(std::size_t n)
            {
                std::vector<hpx::lcos::future<void>> resize_lazy_sync;
                //Resizing the vector chunks
                //AS we have to iterate until we hit LAST
                BOOST_FOREACH(bfg_pair const& p, std::make_pair(base_sf_of_gid_pair_.begin(), base_sf_of_gid_pair_.end() - 1) )
                {
                    resize_lazy_sync.push_back(hpx::stubs::chunk_vector::resize_only_async((p.second).get(), n));
                }
                HPX_ASSERT(base_sf_of_gid_pair_.size() > 1); //As this function changes the size we should have LAST always.
                //waiting for the resizing
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
                BOOST_FOREACH(bfg_pair const& p, std::make_pair(base_sf_of_gid_pair_.begin(), base_sf_of_gid_pair_.end() - 1))
                {
                    resize_lazy_sync.push_back(hpx::stubs::chunk_vector::resize_with_val_async((p.second).get(), n, val));
                }
                HPX_ASSERT(base_sf_of_gid_pair_.size() > 1); //As this function changes the size we should have LAST always.
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
                if (base_sf_of_gid_pair_.size() == 1) //If no chunk_created then we can not push_back hence capacity=0 make sense
                    return 0;
                else
                //- 2 is because we have extra pair which represent LAST
                return capacity_helper(base_sf_of_gid_pair_.size() - 2).get();
            }

            hpx::lcos::future<std::size_t> capacity_async() const
            {
                if (base_sf_of_gid_pair_.size() == 1) //If no chunk_created then we can not push_back hence capacity=0 make sense
                    return hpx::make_ready_future(static_cast<std::size_t>(0));
                else
                //- 2 is because we have extra pair which represent LAST
                return capacity_helper(base_sf_of_gid_pair_.size() - 2);
            }

            //EMPTY
            bool empty() const
            {
                if(base_sf_of_gid_pair_.size() <= 1)
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
                BOOST_FOREACH(bfg_pair const& p, std::make_pair(base_sf_of_gid_pair_.begin(), base_sf_of_gid_pair_.end() - 1))
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
            //TODO What Exception we should throw here..
            //Both Front and back has undefined behavior when vector is empty.
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
                //As the LAST pair is there
                return hpx::stubs::chunk_vector::back_async(((base_sf_of_gid_pair_.end() - 2)->second).get()).get();
            }//end of back_value

            hpx::future< VALUE_TYPE > back_async(std::size_t pos) const
            {
                //As the LAST pair is there
                return hpx::stubs::chunk_vector::front_async(((base_sf_of_gid_pair_.end() - 2)->second).get());
            }//end of back_async

            //
            // Modifier component action
            //

            //ASSIGN
            void assign(std::size_t n, VALUE_TYPE const& val)
            {
                std::vector<hpx::lcos::future<void>> assign_lazy_sync;
                BOOST_FOREACH(bfg_pair const& p, std::make_pair(base_sf_of_gid_pair_.begin(), base_sf_of_gid_pair_.end() - 1))
                {
                    assign_lazy_sync.push_back(hpx::stubs::chunk_vector::assign_async((p.second).get(), n, val));
                }
                hpx::wait_all(assign_lazy_sync);
                HPX_ASSERT(base_sf_of_gid_pair_.size() > 1); //As this function changes the size we should have LAST always.
            }
            hpx::lcos::future<void> assign_async(std::size_t n, VALUE_TYPE const& val)
            {
                return hpx::async(launch::async, hpx::util::bind(&vector::assign, this, n, val));
            }

            //PUSH_BACK
            void push_back(VALUE_TYPE const& val)
            {
                try
                {
                    hpx::stubs::chunk_vector::push_back_async(( (base_sf_of_gid_pair_.end() - 2 )->second).get(), val).get();
                }
                catch(hpx::exception const& e)
                {

                }
            }

            hpx::lcos::future<void> push_back_async(VALUE_TYPE const& val)
            {
                try
                {
                    return hpx::stubs::chunk_vector::push_back_async(( (base_sf_of_gid_pair_.end() - 2)->second).get(), val);
                }
                catch(hpx::exception const& e)
                {

                }
            }

            //PUSH_BACK (With rval)
            void push_back(VALUE_TYPE const&& val)
            {
                try
                {
                    hpx::stubs::chunk_vector::push_back_rval_async(( (base_sf_of_gid_pair_.end() - 2)->second).get(), std::move(val)).get();
                }
                catch(hpx::exception const& e)
                {

                }
            }

            hpx::lcos::future<void> push_back_async(VALUE_TYPE const&& val)
            {
                try
                {
                    return hpx::stubs::chunk_vector::push_back_rval_async(( (base_sf_of_gid_pair_.end() - 2)->second).get(), std::move(val));
                }
                catch(hpx::exception const& e)
                {

                }
            }

            //POP_BACK (Never throw exception)
//            void pop_back()
//            {
//                hpx::stubs::chunk_vector::pop_back_async(( (base_sf_of_gid_pair_.end() - 2)->second).get()).get();
//                //TODO if following change the affect back() and further pop_back function
//                //checking if last element from the particular gid is popped up then delete that..
//                // (-2)I am retaining one gid in vector as otherwise it goes to invalid state and it makes a compulsion that we need to keep at least one element that is not good
//                if(hpx::stubs::chunk_vector::empty_async(( (base_sf_of_gid_pair_.end() - 2)->second).get()).get() && base_sf_of_gid_pair_.size() > 2)
//                    base_sf_of_gid_pair_.pop_back();
//                HPX_ASSERT(base_sf_of_gid_pair_.size() > 1); //As this function changes the size we should have LAST always.
//            }

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
//            void clear()
//            {
//                //It is keeping one gid hence iterator does not go in an invalid state
//                base_sf_of_gid_pair_.erase(base_sf_of_gid_pair_.begin() + 1,
//                                           base_sf_of_gid_pair_.end()-1);
//                hpx::stubs::chunk_vector::clear_async((base_sf_of_gid_pair_[0].second).get()).get();
//                HPX_ASSERT(base_sf_of_gid_pair_.size() > 1); //As this function changes the size we should have LAST always.
//            }

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
            // Iteratro API's in vector class
            //
            hpx::vector::iterator begin()
            {
                return iterator(base_sf_of_gid_pair_.begin(), 0, valid);
            }//end of begin

            hpx::vector::iterator end()
            {
                return iterator((base_sf_of_gid_pair_.end() - 2),
                                hpx::stubs::chunk_vector::size_async( ((base_sf_of_gid_pair_.end() - 2)->second).get() ).get(),\
                                valid);
            }//end of begin

            //
            // Destructor
            //

            ~vector()
            {
                //DEFAULT destructor
            }


    };//end of class vector

} // end of hpx namespace

#endif // VECTOR_HPP
