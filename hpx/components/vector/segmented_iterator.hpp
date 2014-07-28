//  Copyright (c) 2014 Anuj R. Sharma
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SEGMENTED_ITERATOR_HPP
#define SEGMENTED_ITERATOR_HPP

/** @file hpx/components/vector/segmented_iterator.hpp
 *
 *  @brief This file contain the implementation of segmented vector iterator for the hpx::vector.
 *
 */

#include <hpx/include/util.hpp>

// headers for checking the ranges of the Datatypes
#include <cstdint>
#include <boost/integer.hpp>

#include <hpx/components/vector/chunk_vector_component.hpp>

#define VALUE_TYPE double

namespace hpx
{
    //For checking the state of the iterator

    //INVALID STATE:  represent the iterator goes below the 0'th position on the first gid in the vector
    //which actually mean that -ve in the overall index

    /**  @brief This enum define the iterator state. */
    enum iter_state{
        invalid = 0,   /**< This represent the iterator goes into invalid state */
        valid = 1      /**< This represent the iterator is in valid state */
        };

//    class const_segmented_vector_iterator
//    {
//        typedef const_segmented_vector_iterator self_type;
//        typedef std::pair<std::size_t, hpx::lcos::shared_future<hpx::naming::id_type>> bfg_pair;
//        typedef std::vector< bfg_pair > vector_type;
//
//    private:
//        std::vector<VALUE_TYPE>::const_iterator chunk_vec_iterator_;
//        vector_type::const_iterator vec_iterator_;
//
//    public:
//
//    };//End of const_segmented_vector_iterator


//TODO while calculating the size we called get there, I can delay that..

    /** @brief This is the segmented_vector_iterator class which implement iterator functionality for hpx::vector.
    *
    *   This contain the implementation of the all random access iterator API need.
    *   This Class also contain some additional API which is needed to iterate over segmented data structure.
    */
    class segmented_vector_iterator
    {
        // This typedef helps to call object of same class.
        typedef segmented_vector_iterator self_type;

        //PROGRAMMER DOCUMENTATION: For the following two typedefs refer to hpx::vector class in hpx/components/vector/vector.hpp
        typedef std::pair<std::size_t, hpx::lcos::shared_future<hpx::naming::id_type>> bfg_pair;
        typedef std::vector< bfg_pair > vector_type;

        //PROGRAMMER DOCUMENTATION: This represent the return type of the local(), begin() and end() API which are important for segmented_vector iterator
        typedef std::pair<hpx::lcos::shared_future<hpx::naming::id_type>, std::size_t> local_return_type;

    private:

//        // PROGRAMMER DOCUMENTATION: This is the helper function for
//        std::size_t diff_helper(vector_type::const_iterator src, vector_type::const_iterator dest) const
//        {
//            std::size_t diff = 0;
//            //Calculating the total number of element in chunk starting from src to (dest - 1)
//            //the dest - 1 reduce one calculation of size_of_chunk (as that size is given by LAST_OBJECT.local_index)
//            while(src != dest)
//            {
//                diff += hpx::stubs::chunk_vector::size_async((src->second).get()).get();
//                src++;
//            }
//            return diff;
//        }
    protected:
        //  PROGRAMMER DOCUMENTATION: This is the iterator pointing the the vector which stores the (base_index, gid) pair.
        // (For base_index and gid refer to hpx::vector class in hpx/components/vector/vector.hpp).
        //  This actually represent to which gid pair our current iterator position is pointing.
        vector_type::const_iterator curr_bfg_pair_; //iterator to the base/gid pair vector

        // This represent the local position in the current base_gid pair pointed by the curr_bfg_pair_ (defined above) iterator.
        std::size_t local_index_;

        //This represent the state of the iterator
        hpx::iter_state state_;

    public:
        //
        // constructors
        //

        segmented_vector_iterator(){}
        segmented_vector_iterator(vector_type::const_iterator curr_bfg_pair, std::size_t local_index, iter_state state)
        : curr_bfg_pair_(curr_bfg_pair), local_index_(local_index), state_(state) {}


        /** @brief Copy Constructor for segmented_vector_iterator.
         *
         *  @param other    This the hpx::segmented_vector_iterator object which is to be copied
         */
        segmented_vector_iterator(self_type const& other)
        {
            this->curr_bfg_pair_ = other.curr_bfg_pair_;
            this->local_index_ = other.local_index_;
            this->state_ = other.state_;
        }

        //COPY ASSIGNMENT
        //Return allow a=b=c;
        /** @brief Copy one iterator into other.
         *
         *  @param other    This the hpx::segmented_vector_iterator objects which is to be copied
         *
         *  @return This return the reference to the newly created segmented_vector_iterator
         */
        self_type & operator = (self_type const & other)
        {
            this->curr_bfg_pair_ = other.curr_bfg_pair_;
            this->local_index_ = other.local_index_;
            this->state_ = other.state_;
            return *this;
        }

        //COMPARISON API
        /** @brief Compare the two hpx::segmented_vector_iterator for equality.
         *
         *  @param other    This the hpx::segmented_vector_iterator objects which is to be compared
         *
         *  @return Return true if both are equal, false otherwise
         */
        bool operator == (self_type const & other) const
        {
            return (this->curr_bfg_pair_ == other.curr_bfg_pair_ && this->state_ == other.state_ && this->local_index_ == other.local_index_);
        }

        /** @brief Compare the two hpx::segmented_vector_iterator for inequality.
         *
         *  @param other    This the hpx::segmented_vector_iterator objects which is to be compared
         *
         *  @return Return false if both are equal, false otherwise
         */
        bool operator != (self_type const & other) const
        {
            return !(*this == other);
        }

        //DEREFERENCE
        /** @brief Dereferences the iterator and returns the value of the element.
         *
         *  @return Value in the element pointed by the iterator
         */
        VALUE_TYPE operator * () const
        {
            return (hpx::stubs::chunk_vector::get_value_async((curr_bfg_pair_->second).get(), local_index_)).get();
        }

        //INCREMENT
        /** @brief Increment the iterator position by one unit.
         *
         *  @return Return the incremented hpx::segmented_vector_iterator object
         */
        self_type operator ++ ()  //prefix behavior
        {
            if(this->state_ == hpx::iter_state::invalid)
            {
                ++local_index_;
                if(local_index_ == 0)
                {
                    this->state_ = hpx::iter_state::valid;
                }
            }
            else
            {
                ++local_index_;
                hpx::naming::id_type invalid_id;
                if( ((curr_bfg_pair_ + 1)->second).get() != invalid_id //this condition does not cause function call hence it must be first
                   && local_index_ >= hpx::stubs::chunk_vector::size_async((curr_bfg_pair_->second).get()).get())
                {
                    ++curr_bfg_pair_;
                    local_index_ = 0;
                }
            }
            return *this;
        }

        /** @brief Increment the iterator position by one unit.
         *
         *  @return Return the original hpx::segmented_vector_iterator object
         */
        self_type operator ++ (int) //postfix behavior
        {
            self_type temp = *this; //temp object should be return to simulate the postfix behavior
            ++(*this);
            return temp;
        }

        //DECREMENT
        /** @brief Decrement the iterator position by one unit.
         *
         *  @return Return the decremented hpx::segmented_vector_iterator object
         */
        self_type operator -- () //prefix behavior
        {
           if( local_index_ == 0) //If it is just first gid just decrement the local index
           {
               if(curr_bfg_pair_->first != 0)
               {
                    --curr_bfg_pair_;
                    local_index_ = (hpx::stubs::chunk_vector::size_async((curr_bfg_pair_->second).get()).get() - 1);
               }
               else
               {
                   --local_index_;
                   this->state_ = hpx::iter_state::invalid;
               }
           }
           else
           {
                --local_index_;
           }
            return *this;
        }

        /** @brief Decrement the iterator position by one unit.
         *
         *  @return Return the original hpx::segmented_vector_iterator object
         */
        self_type operator -- (int) //postfix behavior
        {
            self_type temp = *this; //temp object should be return to simulate the postfix behavior
            --(*this);
            return temp;
        }

        //ARITHMATIC OPERATOR
        /** @brief Return the iterator pointing to the position which is n units ahead of the current position.*/
        self_type operator + (std::size_t n) const
        {
            //copying the current states of the iterator
            vector_type::const_iterator temp_curr_bfg_pair = this->curr_bfg_pair_;
            std::size_t temp_local_index = this->local_index_;
            hpx::iter_state temp_state = this->state_;

            //temp variables
            hpx::naming::id_type invalid_id;
            bool same_chunk = true;
            std::size_t size = 0;

            if(temp_state == hpx::iter_state::invalid)
            {
                std::size_t diff = (std::numeric_limits<std::size_t>::max() - temp_local_index + 1);
                //calculate the interval through which it is invalid
                if(n < diff )
                {
                    return hpx::segmented_vector_iterator(temp_curr_bfg_pair,
                                                          (temp_local_index + n),
                                                          temp_state);
                }
                else
                {
                    n = n - diff;
                    temp_local_index = 0;
                    temp_state = hpx::iter_state::valid;
                }
            }

            //Calculating the size of the first chunk
            size = hpx::stubs::chunk_vector::size_async((temp_curr_bfg_pair->second).get()).get() - temp_local_index;

            while( n >= size)
            {
                if(((temp_curr_bfg_pair + 1)->second).get() == invalid_id ) //Break this loop if this is previous to LAST gid
                    break;

                same_chunk = false;
                n = n - size;
                ++temp_curr_bfg_pair;
                size = hpx::stubs::chunk_vector::size_async((temp_curr_bfg_pair->second).get()).get();
            }
            if(same_chunk)
            {
                temp_local_index += n;
            }
            else
            {
                temp_local_index = n;
            }
            return segmented_vector_iterator(temp_curr_bfg_pair, temp_local_index, temp_state);
        }//End of a + n

        /** @brief Return the iterator pointing to the position which is n units behind the current position.*/
        self_type operator - (std::size_t n) const
        {
            //copying the current states of the iterator
            vector_type::const_iterator temp_curr_bfg_pair = this->curr_bfg_pair_;
            std::size_t temp_local_index = this->local_index_;
            hpx::iter_state temp_state = this->state_;

            //Temp variables
            bool same_chunk = true;
            std::size_t size = 0;
            if(temp_state == hpx::iter_state::invalid)
            {
                return hpx::segmented_vector_iterator(temp_curr_bfg_pair,
                                                      (temp_local_index - n),
                                                      temp_state );
            }
            else
            {
                //this calculate remaining elements in current chunk
                size = temp_local_index + 1; //This size tells how many need to go out side of current gid
                while (n >= size)
                {
                    //NOTE This if condition is only met when iterator is going invalid
                    if(temp_curr_bfg_pair->first == 0)
                    {
                        temp_state = hpx::iter_state::invalid;
                        break;
                    }
                    same_chunk = false;
                    n = n - size;
                    --temp_curr_bfg_pair;
                    size = hpx::stubs::chunk_vector::size_async((temp_curr_bfg_pair->second).get()).get();
                }//end of while
            }//end of

            if(same_chunk)
            {
                temp_local_index -= n;
            }
            else
            {
                temp_local_index = size - (n + 1);
            }

            return segmented_vector_iterator(temp_curr_bfg_pair, temp_local_index, temp_state);
        }//end of a - n

//        //TODO this returning int64_t which has half range with size_t
//        boost::int64_t operator - (self_type const& other) const
//        {
//            if(this->curr_bfg_pair_ == other.curr_bfg_pair_)
//            {
//                return static_cast<boost::int64_t>(this->local_index_ - other.local_index_);
//            }
//            else if(this->curr_bfg_pair_ > other.curr_bfg_pair_) //Answer is positive
//            {
//                std::size_t diff = diff_helper(other.curr_bfg_pair_, this->curr_bfg_pair_);
//                diff = diff + (this->local_index_ + 1); //Adding the part from (*this) chunk
//                diff = diff - (other.local_index_ + 1); //Subtracting extra part from from the other chunk
//                //TODO this should be the exception not the assert
//                HPX_ASSERT( diff <= std::numeric_limits<boost::int64_t>::max());
//                return static_cast<boost::int64_t>(diff);
//            }
//            else if(this->curr_bfg_pair_ < other.curr_bfg_pair_) //Answer is negative
//            {
//                std::size_t diff = diff_helper(this->curr_bfg_pair_, other.curr_bfg_pair_);
//                diff = diff - (this->local_index_ + 1); //Subtracting extra part from (*this) chunk
//                diff = diff + (other.local_index_ + 1); //Adding the part from from the other chunk
//                //TODO this should be the exception not the assert
//                HPX_ASSERT( diff <= std::numeric_limits<boost::int64_t>::max());
//                return static_cast<boost::int64_t>(diff);
//            }
//            else{HPX_ASSERT(0);}
//        }//end of a - b

        //RELATIONAL OPERATOR
        /** @brief Compare the two hpx::segmented_vector_iterator for less than relation.
         *
         *  @param other    This the hpx::segmented_vector_iterator objects which is to be compared
         *
         *  @return Return true if object with which it called is less than other, false otherwise
         */
        bool operator < (self_type const& other)
        {
            if (this->curr_bfg_pair_ < other.curr_bfg_pair_) //If both are from diff gid
            {
                return true;
            }
            else if (this->curr_bfg_pair_ == other.curr_bfg_pair_) //Now if bot are from same gid
            {
                if(this->state_ < other.state_) //as invalid state = 0 and valid = 1
                    return true;
                else if(this->state_ == other.state_ && this->local_index_ < other.local_index_)//if both are same then check local index
                    return true;
            }
                return false;
        }// End of <

        /** @brief Compare the two hpx::segmented_vector_iterator for greater than relation.
         *
         *  @param other    This the hpx::segmented_vector_iterator objects which is to be compared
         *
         *  @return Return true if object with which it called is greater than other, false otherwise
         */
        bool operator > (self_type const& other)
        {
            if (this->curr_bfg_pair_ > other.curr_bfg_pair_)
            {
                return true;
            }
            else if (this->curr_bfg_pair_ == other.curr_bfg_pair_)
            {
                if(this->state_ > other.state_)
                    return true;
                else if (this->state_ == other.state_ && this->local_index_ > other.local_index_)
                    return true;
            }
                return false;
        }// End of >

        /** @brief Compare the two hpx::segmented_vector_iterator for less than or equal to relation.
         *
         *  @param other    This the hpx::segmented_vector_iterator objects which is to be compared
         *
         *  @return Return true if object with which it called is less than or equal to the other, false otherwise
         */
        bool operator <= (self_type const& other)
        {
            if ( (*this) < other || (*this) == other )
            {
                return true;
            }
            else
                return false;
        }// End of <=

        /** @brief Compare the two hpx::segmented_vector_iterator for greater than or equal to relation.
         *
         *  @param other    This the hpx::segmented_vector_iterator objects which is to be compared
         *
         *  @return Return true if object with which it called is greater than or equal to the other, false otherwise
         */
        bool operator >= (self_type const& other)
        {
            if ( (*this) > other || (*this) == other )
            {
                return true;
            }
            else
                return false;
        }// End of >=

        //COMPOUND ASSIGNMENT
        /** @brief Increment the hpx::segmented_vector_iterator by n.
         *
         *  @return Returns the reference to the incremented object
         */
        self_type & operator +=(std::size_t n) //return self_type to make (a = (b += n)) work
        {
            *this = *this + n;
            return *this;
        }//End of +=

        /** @brief Decrement the hpx::segmented_vector_iterator by n.
         *
         *  @return Returns the reference to the decremented object
         */
        self_type & operator -=(std::size_t n) //return self_type to make (a = (b -= n)) work
        {
            *this = *this - n;
            return *this;
        }//End of +=

        //OFFSET DEREFERENCE
        /** @brief Dereferences the iterator which is at n position ahead of the current iterator position and returns the value of the element.
         *
         *  @return Value in the element which is at n position ahead of the current iterator
         */
        VALUE_TYPE operator[](std::size_t n)
        {
            self_type temp = *this;
            temp = temp + n;
            return *temp;
        }

        // API related to Segmented Iterators
        static vector_type::const_iterator segment(self_type const& seg_iter)
        {
            return seg_iter.curr_bfg_pair_;
        }

        static local_return_type local(self_type const& seg_iter)
        {
            return std::make_pair((seg_iter.curr_bfg_pair_)->second, seg_iter.local_index_);
        }

        static local_return_type begin(vector_type::const_iterator chunk_bfg_pair)
        {
            return std::make_pair(chunk_bfg_pair->second, 0);
        }

        static local_return_type end(vector_type::const_iterator chunk_bfg_pair)
        {
            return std::make_pair(chunk_bfg_pair->second,
                                  hpx::stubs::chunk_vector::size_async((chunk_bfg_pair->second).get()).get()
                                    );
        }


        //
        // Destructor
        //
        /** @brief Default destructor for hpx::segmented_vector_iterator.*/
        ~segmented_vector_iterator()
            {
                //DEFAULT destructor
            }
    };//end of segmented vector iterator
}//end of hpx namespace

#endif