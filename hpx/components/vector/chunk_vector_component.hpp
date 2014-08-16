//  Copyright (c) 2014 Anuj R. Sharma
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/*! \file Chunk_vector.hpp */
#ifndef CHUNK_VECTOR_COMPONENT_HPP
#define CHUNK_VECTOR_COMPONENT_HPP

/** @file hpx/components/vector/chunk_vector_component.hpp
 *
 *  @brief The chunk_vector as the hpx component is defined here.
 *
 *   The chunk_vector is the wrapper to the stl vector class except all API's
 *    are defined as component action. All the API's in stubs classes are
 *    asynchronous API which return the futures.
 *
 */
//#include <hpx/hpx_fwd.hpp>
#include <hpx/runtime/components/server/managed_component_base.hpp>
#include <hpx/runtime/components/server/locking_hook.hpp>
#include <hpx/runtime/actions/component_action.hpp>
#include <hpx/util/function.hpp>
#include <hpx/include/lcos.hpp>
#include <hpx/include/util.hpp>

#include <vector>

#include <boost/assign/std.hpp>

/** @brief Defines the type of value stored by the chunk_vector.*/
#define VALUE_TYPE double

/**
 *  @namespace hpx
 *  @brief Main namespace of the HPX.
 *
 *  This holds all hpx API such as some distributed data structure eg. vector,
 *   components, dataflow, parallel Algorithms implemented in hpx etc.
 *
 */
namespace hpx
{

/**
 *  @namespace hpx::server
 *  @brief This contain the implementation of the component functionality.
 *
 */
    namespace server
    {

    /** @brief This is the basic wrapper class for stl vector.
    *
    * This contain the implementation of the chunk_vector's component
    *  functionality.
    */
        class HPX_COMPONENT_EXPORT chunk_vector
            : public hpx::components::locking_hook<
                hpx::components::managed_component_base<chunk_vector> >
        {
            typedef std::vector<VALUE_TYPE>::const_iterator const_iterator_type;
            typedef std::vector<VALUE_TYPE>::iterator iterator_type;

        private:
            /** @brief It it the std::vector of VALUE_TYPE. */
            std::vector<VALUE_TYPE> chunk_vector_;

        public:
            //
            //Constructors
            //

            /** @brief Default Constructor which create chunk_vector with size 0.*/
            explicit chunk_vector(): chunk_vector_(0, VALUE_TYPE()) {}

//            /** @brief Constructor which create and initialize chunk_vector with
//             *          all elements as 0.
//             *
//             *  @param chunk_size The size of vector
//             */
//            explicit chunk_vector(std::size_t chunk_size)
//                : chunk_vector_(chunk_size, VALUE_TYPE()) {}

            /** @brief Constructor which create ans initialize chunk_vector with
             *          all elements as val.
             *
             *  @param chunk_size The size of vector
             *  @param val Default value for the element in chunk_vector
             */
            explicit chunk_vector(std::size_t chunk_size, VALUE_TYPE val)
                : chunk_vector_(chunk_size, val) {}

            //
            //Destructor (Non Virtual)
            //
            /** @brief chunk_vector destructor*/
            ~chunk_vector()
            {
            }

            //
            //Capacity Related API's in the server class
            //

            /** @brief Compute the size as the number of elements the
             *          chunk_vector it contains.
             *
             *  @return Return the number of element in the chunk_vector
             */
            std::size_t size() const
            {
                return chunk_vector_.size();
            }

            /** @brief Compute the max_size of chunk_vector.
             *
             *  @return Return maximum number of element the chunk_vector can
             *           hold
             */
            std::size_t max_size() const
            {
                return chunk_vector_.max_size();
            }

            /** @brief Resize the chunk_vector so that it contain n elements.
             *
             *  @param n    new size of the chunk_vector
             *  @param val  value to be copied if n is greater than the current
             *               size
             */
            void resize(std::size_t n, VALUE_TYPE const& val)
            {
                chunk_vector_.resize(n, val);
            }

            /** @brief Compute the size of currently allocated storage capacity
             *          for chunk_vector.
             *
             *  @return Returns capacity of chunk_vector, expressed in terms of
             *           elements
             */
            std::size_t capacity() const
            {
                return chunk_vector_.capacity();
            }

            /** @brief Return whether the chunk_vector is empty.
             *
             *  @return Return true if chunk_vector size is 0, false otherwise
             */
            bool empty() const
            {
                return chunk_vector_.empty();
            }

            /** @brief Request the change in chunk_vector capacity so that it
             *          can hold n elements. Throws the hpx::length_error
             *          exception.
             *
             *  This function request chunk_vector capacity should be at least
             *   enough to contain n elements. If n is greater than current
             *   chunk_vector capacity, the function causes the chunk_vector to
             *   reallocate its storage increasing its capacity to n (or greater).
             *  In other cases the chunk_vector capacity does not got affected. It
             *   does not change the chunk_vector size.
             *
             * @param n minimum capacity of chunk_vector
             *
             * @exception hpx::length_error If n is greater than max_size then
             *             function throw hpx::length_error exception.
             */
            void reserve(std::size_t n)
            {
                try
                {
                    chunk_vector_.reserve(n);
                }
                catch(std::length_error const& /*le*/)
                {
                    HPX_THROW_EXCEPTION(
                        hpx::length_error,
                        "reserve" ,
                        "Can not reserve the space for vector of given size");
                }
            }

            //
            // Element access API's
            //

            /** @brief Returns the element at the position pos in the chunk_vector
             *          container. It does not throw the exception
             *
             *  @param pos Position of the element in the chunk_vector [Note the
             *              first position in the chunk_vector is 0]
             *  @return Return the value of the element at position represented
             *           by pos [Note that this is not the reference to the
             *           element]
             */
            VALUE_TYPE get_value_noexpt(std::size_t pos) const
            {
                return chunk_vector_[pos];
            }

            /** @brief Returns the element at position pos in the chunk_vector
             *          container. It throws the hpx::out_of_bound exception.
             *
             *  @param pos Position of the element in the chunk_vector [Note the
             *              first position in the chunk_vector is 0]
             *  @return Return the value of the element at position represented
             *           by pos [Note that this is not the reference to the
             *           element]
             *
             *  @exception hpx::out_of_range The pos is bound checked and if pos
             *              is out of bound then it throws the hpx::out_of_bound
             *              exception.
             */
            VALUE_TYPE get_value(std::size_t pos) const
            {
                try
                {
                    return chunk_vector_.at(pos);
                }
                catch(const std::out_of_range& /*e*/)
                {
                    HPX_THROW_EXCEPTION(
                        hpx::out_of_range,
                        "get_value",
                        "Value of 'pos' is out of range");
                }
            }

            /** @brief Access the value of first element in the chunk_vector.
             *
             *  Calling the function on empty container cause undefined behavior.
             *
             * @return Return the value of the first element in the chunk_vector
             */
            VALUE_TYPE front() const
            {
                return chunk_vector_.front();
            }


            /** @brief Access the value of last element in the chunk_vector.
             *
             *  Calling the function on empty container cause undefined behavior.
             *
             * @return Return the value of the last element in the chunk_vector
             */
            VALUE_TYPE back() const
            {
                return chunk_vector_.back();
            }

            //
            // Modifiers API's in server class
            //

            /** @brief Assigns new contents to the chunk_vector, replacing its
             *          current contents, and modifying its size accordingly.
             *
             * @param n     new size of chunk_vector
             * @param val   Value to fill the container with
             */
            void assign(std::size_t n, VALUE_TYPE const& val)
            {
                chunk_vector_.assign(n, val);
            }

            /** @brief Add new element at the end of chunk_vector. The added
             *          element contain the val as value.
             *
             * @param val Value to be copied to new element
             */
            void push_back(VALUE_TYPE const& val)
            {
                chunk_vector_.push_back(val);
            }

            /** @brief Add new element at the end of chunk_vector. The added
             *          element contain the val as value.
             *
             * @param val Value to be moved to new element
             */
            void push_back_rval(VALUE_TYPE&& val)
            {
                chunk_vector_.push_back(std::move(val));
            }

            /** @brief Remove the last element from chunk_vector.
             *
             *  Removes the last element in the vector, effectively reducing the
             *   container size by one. The removed element is destroyed.
             */
            void pop_back()
            {
                chunk_vector_.pop_back();
            }

            //  This API is required as we do not returning the reference to the
            //  element in Any API.

            /** @brief Copy the value of val in the element at position pos in
             *          the chunk_vector container. It throws the
             *          hpx::out_of_bound exception.
             *
             *  @param pos   Position of the element in the chunk_vector [Note
             *                the first position in the chunk_vector is 0]
             *  @param val   The value to be copied
             *
             *  @exception hpx::out_of_range The pos is bound checked and if pos
             *              is out of bound then it throws the hpx::out_of_bound
             *              exception.
             */
            void set_value(std::size_t pos, VALUE_TYPE const& val)
            {
                try
                {
                    chunk_vector_.at(pos) = val;
                }
                catch(const std::out_of_range& /*e*/)
                {
                    HPX_THROW_EXCEPTION(
                        hpx::out_of_range,
                        "set_value",
                        "Value of 'pos' is out of range");
                }
            }

            /** @brief Move the val in the element at position pos in the
             *          chunk_vector container. It throws the
             *          hpx::out_of_bound exception.
             *
             *  @param pos   Position of the element in the chunk_vector
             *                [Note the first position in the chunk_vector is 0]
             *  @param val   The value to be moved
             *
             *  @exception hpx::out_of_range The pos is bound checked and if pos
             *              is out of bound then it throws the hpx::out_of_bound
             *              exception.
             */
            void set_value_rval(std::size_t pos, VALUE_TYPE const&& val)
            {
                try
                {
                    chunk_vector_.at(pos) = std::move(val);
                }
                catch(const std::out_of_range& /*e*/)
                {
                    HPX_THROW_EXCEPTION(
                        hpx::out_of_range,
                        "set_value_rval",
                        "Value of 'pos' is out of range");
                }
            }

            //TODO deprecate it
            void clear()
            {
                chunk_vector_.clear();
            }


            //
            // Algorithm API's
            //

            /** @brief Apply the function fn to each element in the range
             *          [first, last).
             *
             *  @param first    Initial position of the element in the sequence
             *                   [Note the first position in the chunk_vector
             *                   is 0]
             *  @param last     Final position of the element in the sequence
             *                   [Note the last element is not inclusive in the
             *                   range[first, last)]
             *  @param fn       Unary function (either function pointer or move
             *                   constructible function object) that accept an
             *                   element in the range as argument.
             *                  The return value of the fn is ignored.
             */
            void chunk_for_each(std::size_t first,
                                std::size_t last,
                                hpx::util::function<void(VALUE_TYPE &)> fn)
            {
                std::for_each( chunk_vector_.begin() + first,
                              chunk_vector_.begin() + last,
                              fn);
            }

            /** @brief Apply the function fn to each element in the range
             *          [first, last).
             *
             *  @param first    Initial position of the element in the sequence
             *                   [Note the first position in the chunk_vector
             *                   is 0]
             *  @param last     Final position of the element in the sequence
             *                   [Note the last element is not inclusive in the
             *                   range[first, last)]
             *  @param fn       Unary function (either function pointer or move
             *                   constructible function object) that accept an
             *                   const element in the range as argument.
             */
            void chunk_for_each_const(std::size_t first,
                                std::size_t last,
                                hpx::util::function<void(VALUE_TYPE const&)> fn) const
            {
                std::for_each( chunk_vector_.begin() + first,
                              chunk_vector_.begin() + last,
                              fn);
            }

            //
            //Define the component action here
            //

            //capacity related component action
            /** @brief Macro to define size function as HPX component action
            *           type.
            */
            HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, size);
            /** @brief Macro to define max_size function as HPX component action
             *          type.
             */
            HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, max_size);
            /** @brief Macro to define resize function as HPX component action
             *          type.
             */
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, resize);
            /** @brief Macro to define capacity function as HPX component action
             *          type.
             */
            HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, capacity);
            /** @brief Macro to define empty function as HPX component action
             *          type.
             */
            HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, empty);
            /** @brief Macro to define reserve function as HPX component action
             *          type.
             */
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, reserve);

            //Element access component action
            /** @brief Macro to define get_value_noexpt function as HPX component
             *          action type.
             */
            HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, get_value_noexpt);
            /** @brief Macro to define get_value function as HPX component action
             *          type.
             */
            HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, get_value);
            /** @brief Macro to define front function as HPX component action
             *          type.
             */
            HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, front);
            /** @brief Macro to define back function as HPX component action
             *          type.
             */
            HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, back);

            //Modifiers component action
            /** @brief Macro to define assign function as HPX component action
             *          type.
             */
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, assign);
            /** @brief Macro to define push_back function as HPX component action
             *          type.
             */
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, push_back);
            /** @brief Macro to define push_back_rval function as HPX component
             *          action type.
             */
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, push_back_rval);
            /** @brief Macro to define pop_back function as HPX component action
             *          type.
             */
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, pop_back);
            /** @brief Macro to define set_value function as HPX component action
             *          type.
             */
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, set_value);
            /** @brief Macro to define set_value_rval function as HPX component
             *          action type.
             */
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, set_value_rval);

            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, clear);

            //Algorithm API action
            /** @brief Macro to define chunk_for_each function as HPX component
             *          action type.
             */
            HPX_DEFINE_COMPONENT_ACTION(chunk_vector, chunk_for_each);

            /** @brief Macro to define chunk_for_each_const_iterator function as
             *          HPX component action type.
             */
            HPX_DEFINE_COMPONENT_CONST_ACTION(chunk_vector, chunk_for_each_const);
        };//end of class chunk_vector

    }//end of server namespace

/**
 *  @namespace hpx::stubs
 *
 *  @brief This contain the implementation of the lower-level interface to
 *          instances of the component.
 *
 */
    namespace stubs
    {

        /** @brief This is the low level interface to the chunk_vector.
         *
         * This class contain the implementation of the low level interface to
         *  the chunk_vector. All the function provides asynchronous interface
         *  which mean every function does not block the caller and returns the
         *  future as return value.
         */
        struct chunk_vector : hpx::components::stub_base <server::chunk_vector>
        {
            //
            //  Capacity related API's in stubs class
            //

            //SIZE

            /** @brief Calculate the size of the chunk_vector component.
             *
             *  @param gid  The global id of the chunk_vector component register
             *               with HPX
             *
             *  @return This return size as the hpx::future of type std::size_t
             */
            static hpx::lcos::future<std::size_t>
                size_async(hpx::naming::id_type const& gid)
            {
                return hpx::async<hpx::server::chunk_vector::size_action>(gid);
            }

//            static std::size_t size_sync(hpx::naming::id_type const& gid)
//            {
//                return hpx::async<hpx::server::chunk_vector::size_action>(gid).get();
//            }

            //MAX_SIZE
            /** @brief Calculate the max_size of the chunk_vector component.
             *
             *  @param gid  The global id of the chunk_vector component register
             *               with HPX
             *
             *  @return This return max_size the hpx::future of type std::size_t
             */
            static hpx::lcos::future<std::size_t>
                max_size_async(hpx::naming::id_type const& gid)
            {
                return hpx::async<hpx::server::chunk_vector::max_size_action>(gid);
            }

//            static std::size_t max_size_sync(hpx::naming::id_type const& gid)
//            {
//                return hpx::async<hpx::server::chunk_vector::max_size_action>(gid).get();
//            }

            //RESIZE
            /** @brief Resize the chunk_vector component.
             *
             *  If the val is not specified then it take as 0.
             *
             *  @param gid  The global id of the chunk_vector component register
             *               with HPX
             *  @param n    new size of the chunk_vector
             *  @param val  value to be copied if n is greater than the current
             *               size
             *
             *  @return This return the hpx::future of type void [The void return
             *           type can help to check whether the action is completed or
             *           not]
             */
            static hpx::lcos::future<void>
                resize_async(hpx::naming::id_type const& gid,
                             std::size_t n,
                             VALUE_TYPE const& val = VALUE_TYPE())
            {
                return hpx::async<hpx::server::chunk_vector::resize_action>(gid,
                                                                             n,
                                                                            val);
            }
//            static void resize_with_val_non_blocking(hpx::naming::id_type const& gid, std::size_t n, VALUE_TYPE const& val)
//            {
//                hpx::apply<hpx::server::chunk_vector::resize_with_val_action>(gid, n, val);
//            }


            //CAPACITY
            /** @brief Calculate the capacity of the chunk_vector component.
             *
             *  @param gid  The global id of the chunk_vector component register
             *               with HPX
             *
             *  @return This return capacity the hpx::future of type std::size_t
             */
            static hpx::lcos::future<std::size_t>
                capacity_async(hpx::naming::id_type const& gid)
            {
                return hpx::async<hpx::server::chunk_vector::capacity_action>(gid);
            }
//
//            static std::size_t capacity_sync(hpx::naming::id_type const& gid)
//            {
//                return hpx::async<hpx::server::chunk_vector::capacity_action>(gid).get();
//            }

            //EMPTY
            /** @brief Check whether chunk_vector component is empty.
             *
             *  @param gid  The global id of the chunk_vector component register
             *               with HPX
             *
             *  @return This function return result as the hpx::future of type
             *           bool
             */
            static hpx::lcos::future<bool>
                empty_async(hpx::naming::id_type const& gid)
            {
                return hpx::async<hpx::server::chunk_vector::empty_action>(gid);
            }

//            static bool empty_sync(hpx::naming::id_type const& gid)
//            {
//                return hpx::async<hpx::server::chunk_vector::empty_action>(gid).get();
//            }

            //RESERVE

            /** @brief Reserve the storage space for chunk_vector component.
             *          Throws the hpx::length_error exception.
             *
             *  @param gid  The global id of the chunk_vector component register
             *               with HPX
             *  @param n    minimum size of the chunk_vector
             *
             *  @exception hpx::length_error If n is greater than max_size then
             *              function throw hpx::length_error exception.
             *
             *  @return This return the hpx::future of type void [The void return
             *           type can help to check whether the action is completed
             *           or not]
             */
            static hpx::lcos::future<void>
                reserve_async(hpx::naming::id_type const& gid, std::size_t n)
            {
                return hpx::async<hpx::server::chunk_vector::reserve_action>(gid,
                                                                             n);
            }
//            static void reserve_non_blocking(hpx::naming::id_type const& gid, std::size_t n)
//            {
//                hpx::apply<hpx::server::chunk_vector::reserve_action>(gid, n);
//            }


            //
            //  Element Access API's in stubs class
            //

            //GET_VALUE_NOEXPT
            /** @brief Return the value at position pos in the chunk_vector
             *          component. It does not throw any exception exception.
             *
             *  @param gid  The global id of the chunk_vector component register
             *               with HPX
             *  @param pos  Position of the element in the chunk_vector [Note
             *               the first position in the chunk_vector is 0]
             *
             *  @return This return value as the hpx::future
             */
            static hpx::lcos::future<VALUE_TYPE>
                get_value_noexpt_async(hpx::naming::id_type const& gid,
                                        std::size_t pos)
            {
                return hpx::async<hpx::server::chunk_vector::
                                        get_value_noexpt_action>(gid,
                                                                 pos);
            }

            //GET_VALUE
            /** @brief Return the value at position pos in the chunk_vector
             *          component. It throws the hpx::out_of_bound exception.
             *
             *  @param gid  The global id of the chunk_vector component register
             *               with HPX
             *  @param pos  Position of the element in the chunk_vector [Note
             *               the first position in the chunk_vector is 0]
             *
             * @exception hpx::out_of_bound The pos is bound checked and if pos
             *             is out of bound then it throws the hpx::out_of_bound
             *             exception.
             *
             *  @return This return value as the hpx::future
             */
            static hpx::lcos::future<VALUE_TYPE>
                get_value_async(hpx::naming::id_type const& gid, std::size_t pos)
            {
                return hpx::async<hpx::server::chunk_vector::get_value_action>(gid,
                                                                               pos);
            }

//            static VALUE_TYPE get_value_sync(hpx::naming::id_type const& gid, std::size_t pos)
//            {
//                return hpx::async<hpx::server::chunk_vector::get_value_action>(gid, pos).get();
//            }

            //FRONT

            /** @brief Access the value of first element in chunk_vector
             *          component.
             *
             *  @param gid  The global id of the chunk_vector component
             *               register with HPX
             *
             *  @return This return Value as the hpx::future
             */
            static hpx::lcos::future<VALUE_TYPE>
                front_async(hpx::naming::id_type const& gid)
            {
                return hpx::async<hpx::server::chunk_vector::front_action>(gid);
            }

//            static VALUE_TYPE front_sync(hpx::naming::id_type const& gid)
//            {
//                return hpx::async<hpx::server::chunk_vector::front_action>(gid).get();
//            }

            //BACK
            /** @brief Access the value of last element in chunk_vector
             *          component.
             *
             *  @param gid  The global id of the chunk_vector component register
             *               with HPX
             *
             *  @return This return Value as the hpx::future
             */
            static hpx::lcos::future<VALUE_TYPE>
                back_async(hpx::naming::id_type const& gid)
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

            /** @brief Assign the new content to the elements in chunk_vector
             *          component, replacing the current content and modifying
             *          the size accordingly.
             *
             *  @param gid  The global id of the chunk_vector component register
             *               with HPX
             *  @param n    new size of the chunk_vector
             *  @param val  value to fill the container with
             *
             *  @return This return the hpx::future of type void [The void
             *           return type can help to check whether the action is
             *           completed or not]
             */
            static hpx::lcos::future<void>
                assign_async(hpx::naming::id_type const& gid,
                             std::size_t n,
                             VALUE_TYPE const& val)
            {
                return hpx::async<hpx::server::chunk_vector::assign_action>(gid,
                                                                            n,
                                                                            val);
            }
//            static void assign_non_blocking(hpx::naming::id_type const& gid, std::size_t n, VALUE_TYPE const& val)
//            {
//                hpx::apply<hpx::server::chunk_vector::assign_action>(gid, n, val);
//            }

            //PUSH_BACK
            /** @brief Add the new element at the end of chunk_vector component.
             *
             *  @param gid  The global id of the chunk_vector component register
             *               with HPX
             *  @param val  value to be copied to new element
             *
             *  @return This return the hpx::future of type void [The void
             *           return type can help to check whether the action is
             *           completed or not]
             */
            static hpx::lcos::future<void>
                push_back_async(hpx::naming::id_type const& gid,
                                VALUE_TYPE const& val)
            {
                return hpx::async<hpx::server::chunk_vector::push_back_action>(gid,
                                                                               val);
            }
//            static void push_back_non_blocking(hpx::naming::id_type const& gid, VALUE_TYPE const& val)
//            {
//                hpx::apply<hpx::server::chunk_vector::push_back_action>(gid, val);
//            }


            //PUSH_BACK_RVAL
            /** @brief Add the new element at the end of chunk_vector component.
             *
             *  @param gid  The global id of the chunk_vector component register
             *               with HPX
             *  @param val  value to be moved to new element
             *
             *  @return This return the hpx::future of type void [The void
             *           return type can help to check whether the action is
             *           completed or not].
             */
            static hpx::lcos::future<void>
                push_back_rval_async(hpx::naming::id_type const& gid,
                                     VALUE_TYPE const&& val)
            {
                return hpx::async<hpx::server::chunk_vector::push_back_rval_action>(gid,
                                                                                    std::move(val));
            }
//            static void push_back_rval_non_blocking(hpx::naming::id_type const& gid, VALUE_TYPE const&& val)
//            {
//                hpx::apply<hpx::server::chunk_vector::push_back_rval_action>(gid, std::move(val));
//            }

            //POP_BACK
            static hpx::lcos::future<void>
                pop_back_async(hpx::naming::id_type const& gid)
            {
                return hpx::async<hpx::server::chunk_vector::pop_back_action>(gid);
            }
//            static void pop_back_non_blocking(hpx::naming::id_type const& gid)
//            {
//                hpx::apply<hpx::server::chunk_vector::pop_back_action>(gid);
//            }

            //SET_VALUE
            /** @brief Copy the value of val in the element at position pos in
             *          the chunk_vector component. It throws the
             *          hpx::out_of_bound exception.
             *
             *  @param gid  The global id of the chunk_vector component register
             *               with HPX
             *  @param pos  Position of the element in the chunk_vector [Note
             *               the first position in the chunk_vector is 0].
             *  @param val  The value to be copied
             *
             * @exception hpx::out_of_bound The pos is bound checked and if pos
             *             is out of bound then it throws the hpx::out_of_bound
             *             exception.
             *
             *  @return This return the hpx::future of type void [The void
             *           return type can help to check whether the action is
             *           completed or not].
             */
            static hpx::lcos::future<void>
                set_value_async(hpx::naming::id_type const& gid,
                                std::size_t pos,
                                VALUE_TYPE const& val)
            {
                return hpx::async<hpx::server::chunk_vector::set_value_action>(gid,
                                                                               pos,
                                                                               val);
            }
//            static void set_value_non_blocking(hpx::naming::id_type const& gid, std::size_t pos, VALUE_TYPE const& val)
//            {
//                hpx::apply<hpx::server::chunk_vector::set_value_action>(gid, pos, val);
//            }

            //SET_VALUE_RVAL
            /** @brief Move the val in the element at position pos in the
             *          chunk_vector component. It throws the
             *          hpx::out_of_bound exception.
             *
             *  @param gid  The global id of the chunk_vector component register
             *               with HPX
             *  @param pos  Position of the element in the chunk_vector [Note
             *               the first position in the chunk_vector is 0].
             *  @param val  The value to be moved
             *
             * @exception hpx::out_of_bound The pos is bound checked and if pos
             *             is out of bound then it throws the hpx::out_of_bound
             *             exception.
             *
             *  @return This return the hpx::future of type void [The void
             *           return type can help to check whether the action is
             *           completed or not].
             */
            static hpx::lcos::future<void>
                set_value_rval_async(hpx::naming::id_type const& gid,
                                     std::size_t pos,
                                     VALUE_TYPE const&& val)
            {
                return hpx::async<hpx::server::chunk_vector::set_value_rval_action>(gid,
                                                                                    pos,
                                                                                    std::move(val));
            }
//            static void set_value_rval_non_blocking(hpx::naming::id_type const& gid, std::size_t pos,  VALUE_TYPE const&& val)
//            {
//                hpx::apply<hpx::server::chunk_vector::set_value_rval_action>(gid, pos, std::move(val));
//            }

            //CLEAR
            static hpx::lcos::future<void> clear_async(hpx::naming::id_type const& gid)
            {
                return hpx::async<hpx::server::chunk_vector::clear_action>(gid);
            }
//            static void clear_non_blocking(hpx::naming::id_type const& gid)
//            {
//                hpx::apply<server::chunk_vector::clear_action>(gid);
//            }

        //
        // Algorithm API's in Stubs class
        //

        /** @brief Apply the function fn to each element in the range
         *          [first, last) in chunk_vector component.
         *
         *  @param gid      The global id of the chunk_vector component register
         *                   with HPX
         *  @param first    Initial position of the element in the sequence
         *                   [Note the first position in the chunk_vector is 0].
         *  @param last     final position of the element in the sequence [Note
         *                   the last element is not inclusive in the range
         *                   [first, last)].
         *  @param fn       Unary function (either function pointer or move
         *                   constructible function object) that accept an
         *                   element in the range as argument.
         *                  The return value of the fn is ignored.
         *
         * @return This return the hpx::future of type void [The void return
         *          type can help to check whether the action is completed or
         *          not]
         */
        static hpx::lcos::future<void>
            chunk_for_each_async(hpx::naming::id_type const& gid,
                                 std::size_t first,
                                 std::size_t last,
                                 hpx::util::function<void(VALUE_TYPE &)> fn)
        {
            return hpx::async<hpx::server::chunk_vector::chunk_for_each_action>(gid,
                                                                                first,
                                                                                last,
                                                                                fn);

        }//end of chunk_vector_for_each_async


        /** @brief Apply the function fn to each element in the range
         *          [first, last) in chunk_vector component.
         *
         *  @param gid      The global id of the chunk_vector component register
         *                   with HPX
         *  @param first    Initial position of the element in the sequence
         *                   [Note the first position in the chunk_vector is 0].
         *  @param last     final position of the element in the sequence [Note
         *                   the last element is not inclusive in the range
         *                   [first, last)].
         *  @param fn       Unary function (either function pointer or move
         *                   constructible function object) that accept an
         *                   const element in the range as argument
         *
         * @return This return the hpx::future of type void [The void return
         *          type can help to check whether the action is completed or
         *          not]
         */
        static hpx::lcos::future<void>
            chunk_for_each_const_async(hpx::naming::id_type const& gid,
                                 std::size_t first,
                                 std::size_t last,
                                 hpx::util::function<void(VALUE_TYPE const &)> fn)
        {
            return hpx::async<hpx::server::chunk_vector::
                                chunk_for_each_const_action>(gid,
                                                             first,
                                                             last,
                                                             fn);

        }//end of chunk_vector_for_each_const_async



        };//end of chunk_vector(stubs)
    }//end of the namespace stubs


    class chunk_vector
        : public hpx::components::client_base<chunk_vector, stubs::chunk_vector>
    {
        typedef hpx::components::client_base<chunk_vector,
                                            stubs::chunk_vector> base_type;
    public:

        chunk_vector() {}
        chunk_vector(hpx::naming::id_type const& gid): base_type(gid) {}
        chunk_vector(hpx::shared_future<hpx::naming::id_type> const& gid)
            : base_type(gid) {}

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

        //RESIZE
        void resize(std::size_t n, VALUE_TYPE const& val = 0)
        {
            HPX_ASSERT(this->get_gid());
            (this->base_type::resize_async(this->get_gid(), n, val)).get();
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
            (this->base_type::set_value_rval_async(this->get_gid(),
                                                   pos,
                                                   std::move(val))).get();
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
/** @brief Macro to register size component action type with HPX AGAS.*/
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::size_action,
    chunk_vector_size_action);
/** @brief Macro to register max_size component action type with HPX AGAS.*/
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::max_size_action,
    chunk_vector_max_size_action);
/** @brief Macro to register resize component action type with HPX AGAS.*/
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::resize_action,
    chunk_vector_resize_action);
/** @brief Macro to register capacity component action type with HPX AGAS.*/
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::capacity_action,
    chunk_vector_capacity_action);
/** @brief Macro to register empty component action type with HPX AGAS.*/
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::empty_action,
    chunk_vector_empty_action);
/** @brief Macro to register reserve component action type with HPX AGAS.*/
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::reserve_action,
    chunk_vector_reserve_action);

//Element access component action declaration
/** @brief Macro to register get_value_noexpt component action type with HPX AGAS.*/
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::get_value_noexpt_action,
    chunk_vector_get_value_noexpt_action);
/** @brief Macro to register get_value component action type with HPX AGAS.*/
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::get_value_action,
    chunk_vector_get_value_action);
/** @brief Macro to register front component action type with HPX AGAS.*/
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::front_action,
    chunk_vector_front_action);
/** @brief Macro to register back component action type with HPX AGAS.*/
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::back_action,
    chunk_vector_back_action);

//Modifiers component action declaration
/** @brief Macro to register assign component action type with HPX AGAS.*/
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::assign_action,
    chunk_vector_assign_action);
/** @brief Macro to register push_back component action type with HPX AGAS.*/
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::push_back_action,
    chunk_vector_push_back_action);
/** @brief Macro to register push_back_rval component action type with HPX AGAS.*/
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::push_back_rval_action,
    chunk_vector_push_back_rval_action);
/** @brief Macro to register pop_back component action type with HPX AGAS.*/
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::pop_back_action,
    chunk_vector_pop_back_action);
/** @brief Macro to register set_value component action type with HPX AGAS.*/
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::set_value_action,
    chunk_vector_set_value_action);
/** @brief Macro to register set_value_rval component action type with HPX AGAS.*/
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::set_value_rval_action,
    chunk_vector_set_value_rval_action);
/** @brief Macro to register clear component action type with HPX AGAS.*/
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::clear_action,
    chunk_vector_clear_action);

//Algorithm API's component action declaration
/** @brief Macro to register chunk_for_each component action type with HPX AGAS.*/
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::chunk_for_each_action,
    chunk_vector_chunk_for_each_action);
/** @brief Macro to register chunk_for_each_const component action type with
 *          HPX AGAS.
 */
HPX_REGISTER_ACTION_DECLARATION(
    hpx::server::chunk_vector::chunk_for_each_const_action,
    chunk_vector_chunk_for_each_const_action);
		
#endif // CHUNK_VECTOR_COMPONENT_HPP
