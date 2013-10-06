// Copyright (c) 2007-2013 Hartmut Kaiser
// Copyright (c) 2012-2013 Thomas Heller
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file has been automatically generated using the Boost.Wave tool.
// Do not edit manually.


namespace hpx { namespace lcos {
    namespace detail
    {
        template <
            typename Action
          , typename Futures
           
        >
        void
        broadcast_invoke(Action act, Futures& futures, hpx::id_type id
           
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                   
                )
            );
        }
        template <
            typename Action
          , typename Futures
           
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures, hpx::id_type id
           
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                   
                  , global_idx
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
           
        >
        void
        broadcast_invoke(Action act, Futures& futures, BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
           
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                   
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
           
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures
          , BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
           
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                   
                  , global_idx
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
           
        >
        
        void
        broadcast_impl0(
            Action const & act
          , std::vector<hpx::id_type> const & ids
           
          , std::size_t global_idx
          , boost::mpl::true_
        )
        {
            if(ids.empty()) return;
            std::vector<hpx::future<void> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , ids[0]
               
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                           
                          , global_idx + 1
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                           
                          , global_idx + half
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
            }
            
            hpx::when_all(broadcast_futures).then(&return_void).get();
        }
        template <
            typename Action
           
        >
        
        typename broadcast_result<Action>::type
        broadcast_impl0(
            Action const & act
          , std::vector<hpx::id_type> const & ids
           
          , std::size_t global_idx
          , boost::mpl::false_
        )
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef
                typename broadcast_result<Action>::type
                result_type;
            
            if(ids.empty()) return result_type();
            std::vector<hpx::future<result_type> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , &wrap_into_vector<action_result>
              , ids[0]
               
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                           
                          , global_idx + 1
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                           
                          , global_idx + half
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
            }
            return hpx::when_all(broadcast_futures).
                then(&return_result_type<action_result>).move();
        }
        
        template <
            typename Action
           
          , typename IsVoid
        >
        struct broadcast_invoker0
        {
            
            static typename broadcast_result<Action>::type
            call(
                Action const & act
              , std::vector<hpx::id_type> const & ids
               
              , std::size_t global_idx
              , IsVoid
            )
            {
                return
                    broadcast_impl0(
                        act
                      , ids
                       
                      , global_idx
                      , IsVoid()
                    );
            }
        };
        template <typename Action>
        struct make_broadcast_action_impl<Action, 0>
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef broadcast_invoker0<
                        Action
                      
                        
                      , typename boost::is_same<void, action_result>::type
                    >
                    broadcast_invoker_type;
            typedef
                typename HPX_MAKE_ACTION_TPL(broadcast_invoker_type::call)::type
                type;
        };
    }
    
    template <
        typename Action
       
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::id_type> const & ids
       )
    {
        hpx::id_type dest = hpx::get_colocation_id_sync(ids[0]);
        typedef
            typename detail::make_broadcast_action<Action>::type
            broadcast_impl_action;
        typedef
            typename detail::broadcast_result<Action>::action_result
            action_result;
        return
            hpx::async<broadcast_impl_action>(
                dest
              , Action()
              , ids
               
              , 0
              , typename boost::is_same<void, action_result>::type()
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
       
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
       )
    {
        return broadcast<Derived>(
                ids
               
            );
    }
    template <
        typename Action
       
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast_with_index(
        std::vector<hpx::id_type> const & ids
       )
    {
        return broadcast<detail::broadcast_with_index<Action> >(
                ids
               
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
       
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast_with_index(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
       )
    {
        return broadcast<detail::broadcast_with_index<Derived> >(
                ids
               
            );
    }
    
    namespace detail
    {
        template <
            typename Action
           
        >
        hpx::future<
            typename detail::broadcast_result<Action>::type
        >
        invoke_broadcast0(
            std::vector<hpx::future<hpx::id_type> > const & fids
           )
        {
            return broadcast<Action>(resolve_futures(fids)
               );
        }
    }
    template <
        typename Action
       
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::future<hpx::id_type> > const & ids
       )
    {
        typedef typename detail::broadcast_result<Action>::type result_type;
        typedef result_type (*invoke_broadcast_func)(
            std::vector<hpx::future<hpx::id_type> > const&
          
                );
        return when_all(ids).then(
            util::bind(
                (invoke_broadcast_func)
                    &detail::invoke_broadcast0<
                        Action
                       
                    >
              , util::placeholders::_1
               
            )
        );
    }
}}
namespace hpx { namespace lcos {
    namespace detail
    {
        template <
            typename Action
          , typename Futures
          , typename A0
        >
        void
        broadcast_invoke(Action act, Futures& futures, hpx::id_type id
          , A0 const & a0
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename A0
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures, hpx::id_type id
          , A0 const & a0
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0
                  , global_idx
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0
        >
        void
        broadcast_invoke(Action act, Futures& futures, BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures
          , BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0
                  , global_idx
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename A0
        >
        
        void
        broadcast_impl1(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0
          , std::size_t global_idx
          , boost::mpl::true_
        )
        {
            if(ids.empty()) return;
            std::vector<hpx::future<void> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , ids[0]
              , a0
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0
                          , global_idx + 1
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0
                          , global_idx + half
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
            }
            
            hpx::when_all(broadcast_futures).then(&return_void).get();
        }
        template <
            typename Action
          , typename A0
        >
        
        typename broadcast_result<Action>::type
        broadcast_impl1(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0
          , std::size_t global_idx
          , boost::mpl::false_
        )
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef
                typename broadcast_result<Action>::type
                result_type;
            
            if(ids.empty()) return result_type();
            std::vector<hpx::future<result_type> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , &wrap_into_vector<action_result>
              , ids[0]
              , a0
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0
                          , global_idx + 1
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0
                          , global_idx + half
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
            }
            return hpx::when_all(broadcast_futures).
                then(&return_result_type<action_result>).move();
        }
        
        template <
            typename Action
          , typename A0
          , typename IsVoid
        >
        struct broadcast_invoker1
        {
            
            static typename broadcast_result<Action>::type
            call(
                Action const & act
              , std::vector<hpx::id_type> const & ids
              , A0 const & a0
              , std::size_t global_idx
              , IsVoid
            )
            {
                return
                    broadcast_impl1(
                        act
                      , ids
                      , a0
                      , global_idx
                      , IsVoid()
                    );
            }
        };
        template <typename Action>
        struct make_broadcast_action_impl<Action, 1>
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef broadcast_invoker1<
                        Action
                      ,
                        typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 0 >::type
                      , typename boost::is_same<void, action_result>::type
                    >
                    broadcast_invoker_type;
            typedef
                typename HPX_MAKE_ACTION_TPL(broadcast_invoker_type::call)::type
                type;
        };
    }
    
    template <
        typename Action
      , typename A0
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0)
    {
        hpx::id_type dest = hpx::get_colocation_id_sync(ids[0]);
        typedef
            typename detail::make_broadcast_action<Action>::type
            broadcast_impl_action;
        typedef
            typename detail::broadcast_result<Action>::action_result
            action_result;
        return
            hpx::async<broadcast_impl_action>(
                dest
              , Action()
              , ids
              , a0
              , 0
              , typename boost::is_same<void, action_result>::type()
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0)
    {
        return broadcast<Derived>(
                ids
              , a0
            );
    }
    template <
        typename Action
      , typename A0
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast_with_index(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0)
    {
        return broadcast<detail::broadcast_with_index<Action> >(
                ids
              , a0
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast_with_index(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0)
    {
        return broadcast<detail::broadcast_with_index<Derived> >(
                ids
              , a0
            );
    }
    
    namespace detail
    {
        template <
            typename Action
          , typename A0
        >
        hpx::future<
            typename detail::broadcast_result<Action>::type
        >
        invoke_broadcast1(
            std::vector<hpx::future<hpx::id_type> > const & fids
          , A0 const & a0)
        {
            return broadcast<Action>(resolve_futures(fids)
              , a0);
        }
    }
    template <
        typename Action
      , typename A0
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::future<hpx::id_type> > const & ids
      , A0 const & a0)
    {
        typedef typename detail::broadcast_result<Action>::type result_type;
        typedef result_type (*invoke_broadcast_func)(
            std::vector<hpx::future<hpx::id_type> > const&
          ,
                A0 const&);
        return when_all(ids).then(
            util::bind(
                (invoke_broadcast_func)
                    &detail::invoke_broadcast1<
                        Action
                      , A0
                    >
              , util::placeholders::_1
              , a0
            )
        );
    }
}}
namespace hpx { namespace lcos {
    namespace detail
    {
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1
        >
        void
        broadcast_invoke(Action act, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1
                  , global_idx
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1
        >
        void
        broadcast_invoke(Action act, Futures& futures, BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures
          , BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1
                  , global_idx
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename A0 , typename A1
        >
        
        void
        broadcast_impl2(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1
          , std::size_t global_idx
          , boost::mpl::true_
        )
        {
            if(ids.empty()) return;
            std::vector<hpx::future<void> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , ids[0]
              , a0 , a1
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1
                          , global_idx + 1
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1
                          , global_idx + half
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
            }
            
            hpx::when_all(broadcast_futures).then(&return_void).get();
        }
        template <
            typename Action
          , typename A0 , typename A1
        >
        
        typename broadcast_result<Action>::type
        broadcast_impl2(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1
          , std::size_t global_idx
          , boost::mpl::false_
        )
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef
                typename broadcast_result<Action>::type
                result_type;
            
            if(ids.empty()) return result_type();
            std::vector<hpx::future<result_type> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , &wrap_into_vector<action_result>
              , ids[0]
              , a0 , a1
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1
                          , global_idx + 1
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1
                          , global_idx + half
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
            }
            return hpx::when_all(broadcast_futures).
                then(&return_result_type<action_result>).move();
        }
        
        template <
            typename Action
          , typename A0 , typename A1
          , typename IsVoid
        >
        struct broadcast_invoker2
        {
            
            static typename broadcast_result<Action>::type
            call(
                Action const & act
              , std::vector<hpx::id_type> const & ids
              , A0 const & a0 , A1 const & a1
              , std::size_t global_idx
              , IsVoid
            )
            {
                return
                    broadcast_impl2(
                        act
                      , ids
                      , a0 , a1
                      , global_idx
                      , IsVoid()
                    );
            }
        };
        template <typename Action>
        struct make_broadcast_action_impl<Action, 2>
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef broadcast_invoker2<
                        Action
                      ,
                        typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 0 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 1 >::type
                      , typename boost::is_same<void, action_result>::type
                    >
                    broadcast_invoker_type;
            typedef
                typename HPX_MAKE_ACTION_TPL(broadcast_invoker_type::call)::type
                type;
        };
    }
    
    template <
        typename Action
      , typename A0 , typename A1
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1)
    {
        hpx::id_type dest = hpx::get_colocation_id_sync(ids[0]);
        typedef
            typename detail::make_broadcast_action<Action>::type
            broadcast_impl_action;
        typedef
            typename detail::broadcast_result<Action>::action_result
            action_result;
        return
            hpx::async<broadcast_impl_action>(
                dest
              , Action()
              , ids
              , a0 , a1
              , 0
              , typename boost::is_same<void, action_result>::type()
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1)
    {
        return broadcast<Derived>(
                ids
              , a0 , a1
            );
    }
    template <
        typename Action
      , typename A0 , typename A1
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast_with_index(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1)
    {
        return broadcast<detail::broadcast_with_index<Action> >(
                ids
              , a0 , a1
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast_with_index(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1)
    {
        return broadcast<detail::broadcast_with_index<Derived> >(
                ids
              , a0 , a1
            );
    }
    
    namespace detail
    {
        template <
            typename Action
          , typename A0 , typename A1
        >
        hpx::future<
            typename detail::broadcast_result<Action>::type
        >
        invoke_broadcast2(
            std::vector<hpx::future<hpx::id_type> > const & fids
          , A0 const & a0 , A1 const & a1)
        {
            return broadcast<Action>(resolve_futures(fids)
              , a0 , a1);
        }
    }
    template <
        typename Action
      , typename A0 , typename A1
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::future<hpx::id_type> > const & ids
      , A0 const & a0 , A1 const & a1)
    {
        typedef typename detail::broadcast_result<Action>::type result_type;
        typedef result_type (*invoke_broadcast_func)(
            std::vector<hpx::future<hpx::id_type> > const&
          ,
                A0 const& , A1 const&);
        return when_all(ids).then(
            util::bind(
                (invoke_broadcast_func)
                    &detail::invoke_broadcast2<
                        Action
                      , A0 , A1
                    >
              , util::placeholders::_1
              , a0 , a1
            )
        );
    }
}}
namespace hpx { namespace lcos {
    namespace detail
    {
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2
        >
        void
        broadcast_invoke(Action act, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2
                  , global_idx
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2
        >
        void
        broadcast_invoke(Action act, Futures& futures, BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures
          , BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2
                  , global_idx
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2
        >
        
        void
        broadcast_impl3(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2
          , std::size_t global_idx
          , boost::mpl::true_
        )
        {
            if(ids.empty()) return;
            std::vector<hpx::future<void> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , ids[0]
              , a0 , a1 , a2
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2
                          , global_idx + 1
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2
                          , global_idx + half
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
            }
            
            hpx::when_all(broadcast_futures).then(&return_void).get();
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2
        >
        
        typename broadcast_result<Action>::type
        broadcast_impl3(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2
          , std::size_t global_idx
          , boost::mpl::false_
        )
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef
                typename broadcast_result<Action>::type
                result_type;
            
            if(ids.empty()) return result_type();
            std::vector<hpx::future<result_type> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , &wrap_into_vector<action_result>
              , ids[0]
              , a0 , a1 , a2
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2
                          , global_idx + 1
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2
                          , global_idx + half
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
            }
            return hpx::when_all(broadcast_futures).
                then(&return_result_type<action_result>).move();
        }
        
        template <
            typename Action
          , typename A0 , typename A1 , typename A2
          , typename IsVoid
        >
        struct broadcast_invoker3
        {
            
            static typename broadcast_result<Action>::type
            call(
                Action const & act
              , std::vector<hpx::id_type> const & ids
              , A0 const & a0 , A1 const & a1 , A2 const & a2
              , std::size_t global_idx
              , IsVoid
            )
            {
                return
                    broadcast_impl3(
                        act
                      , ids
                      , a0 , a1 , a2
                      , global_idx
                      , IsVoid()
                    );
            }
        };
        template <typename Action>
        struct make_broadcast_action_impl<Action, 3>
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef broadcast_invoker3<
                        Action
                      ,
                        typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 0 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 1 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 2 >::type
                      , typename boost::is_same<void, action_result>::type
                    >
                    broadcast_invoker_type;
            typedef
                typename HPX_MAKE_ACTION_TPL(broadcast_invoker_type::call)::type
                type;
        };
    }
    
    template <
        typename Action
      , typename A0 , typename A1 , typename A2
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2)
    {
        hpx::id_type dest = hpx::get_colocation_id_sync(ids[0]);
        typedef
            typename detail::make_broadcast_action<Action>::type
            broadcast_impl_action;
        typedef
            typename detail::broadcast_result<Action>::action_result
            action_result;
        return
            hpx::async<broadcast_impl_action>(
                dest
              , Action()
              , ids
              , a0 , a1 , a2
              , 0
              , typename boost::is_same<void, action_result>::type()
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2)
    {
        return broadcast<Derived>(
                ids
              , a0 , a1 , a2
            );
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast_with_index(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2)
    {
        return broadcast<detail::broadcast_with_index<Action> >(
                ids
              , a0 , a1 , a2
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast_with_index(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2)
    {
        return broadcast<detail::broadcast_with_index<Derived> >(
                ids
              , a0 , a1 , a2
            );
    }
    
    namespace detail
    {
        template <
            typename Action
          , typename A0 , typename A1 , typename A2
        >
        hpx::future<
            typename detail::broadcast_result<Action>::type
        >
        invoke_broadcast3(
            std::vector<hpx::future<hpx::id_type> > const & fids
          , A0 const & a0 , A1 const & a1 , A2 const & a2)
        {
            return broadcast<Action>(resolve_futures(fids)
              , a0 , a1 , a2);
        }
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::future<hpx::id_type> > const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2)
    {
        typedef typename detail::broadcast_result<Action>::type result_type;
        typedef result_type (*invoke_broadcast_func)(
            std::vector<hpx::future<hpx::id_type> > const&
          ,
                A0 const& , A1 const& , A2 const&);
        return when_all(ids).then(
            util::bind(
                (invoke_broadcast_func)
                    &detail::invoke_broadcast3<
                        Action
                      , A0 , A1 , A2
                    >
              , util::placeholders::_1
              , a0 , a1 , a2
            )
        );
    }
}}
namespace hpx { namespace lcos {
    namespace detail
    {
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3
        >
        void
        broadcast_invoke(Action act, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3
                  , global_idx
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3
        >
        void
        broadcast_invoke(Action act, Futures& futures, BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures
          , BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3
                  , global_idx
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3
        >
        
        void
        broadcast_impl4(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3
          , std::size_t global_idx
          , boost::mpl::true_
        )
        {
            if(ids.empty()) return;
            std::vector<hpx::future<void> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , ids[0]
              , a0 , a1 , a2 , a3
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3
                          , global_idx + 1
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3
                          , global_idx + half
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
            }
            
            hpx::when_all(broadcast_futures).then(&return_void).get();
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3
        >
        
        typename broadcast_result<Action>::type
        broadcast_impl4(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3
          , std::size_t global_idx
          , boost::mpl::false_
        )
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef
                typename broadcast_result<Action>::type
                result_type;
            
            if(ids.empty()) return result_type();
            std::vector<hpx::future<result_type> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , &wrap_into_vector<action_result>
              , ids[0]
              , a0 , a1 , a2 , a3
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3
                          , global_idx + 1
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3
                          , global_idx + half
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
            }
            return hpx::when_all(broadcast_futures).
                then(&return_result_type<action_result>).move();
        }
        
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3
          , typename IsVoid
        >
        struct broadcast_invoker4
        {
            
            static typename broadcast_result<Action>::type
            call(
                Action const & act
              , std::vector<hpx::id_type> const & ids
              , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3
              , std::size_t global_idx
              , IsVoid
            )
            {
                return
                    broadcast_impl4(
                        act
                      , ids
                      , a0 , a1 , a2 , a3
                      , global_idx
                      , IsVoid()
                    );
            }
        };
        template <typename Action>
        struct make_broadcast_action_impl<Action, 4>
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef broadcast_invoker4<
                        Action
                      ,
                        typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 0 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 1 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 2 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 3 >::type
                      , typename boost::is_same<void, action_result>::type
                    >
                    broadcast_invoker_type;
            typedef
                typename HPX_MAKE_ACTION_TPL(broadcast_invoker_type::call)::type
                type;
        };
    }
    
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3)
    {
        hpx::id_type dest = hpx::get_colocation_id_sync(ids[0]);
        typedef
            typename detail::make_broadcast_action<Action>::type
            broadcast_impl_action;
        typedef
            typename detail::broadcast_result<Action>::action_result
            action_result;
        return
            hpx::async<broadcast_impl_action>(
                dest
              , Action()
              , ids
              , a0 , a1 , a2 , a3
              , 0
              , typename boost::is_same<void, action_result>::type()
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3)
    {
        return broadcast<Derived>(
                ids
              , a0 , a1 , a2 , a3
            );
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast_with_index(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3)
    {
        return broadcast<detail::broadcast_with_index<Action> >(
                ids
              , a0 , a1 , a2 , a3
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast_with_index(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3)
    {
        return broadcast<detail::broadcast_with_index<Derived> >(
                ids
              , a0 , a1 , a2 , a3
            );
    }
    
    namespace detail
    {
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3
        >
        hpx::future<
            typename detail::broadcast_result<Action>::type
        >
        invoke_broadcast4(
            std::vector<hpx::future<hpx::id_type> > const & fids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3)
        {
            return broadcast<Action>(resolve_futures(fids)
              , a0 , a1 , a2 , a3);
        }
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::future<hpx::id_type> > const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3)
    {
        typedef typename detail::broadcast_result<Action>::type result_type;
        typedef result_type (*invoke_broadcast_func)(
            std::vector<hpx::future<hpx::id_type> > const&
          ,
                A0 const& , A1 const& , A2 const& , A3 const&);
        return when_all(ids).then(
            util::bind(
                (invoke_broadcast_func)
                    &detail::invoke_broadcast4<
                        Action
                      , A0 , A1 , A2 , A3
                    >
              , util::placeholders::_1
              , a0 , a1 , a2 , a3
            )
        );
    }
}}
namespace hpx { namespace lcos {
    namespace detail
    {
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4
        >
        void
        broadcast_invoke(Action act, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4
                  , global_idx
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4
        >
        void
        broadcast_invoke(Action act, Futures& futures, BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures
          , BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4
                  , global_idx
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4
        >
        
        void
        broadcast_impl5(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4
          , std::size_t global_idx
          , boost::mpl::true_
        )
        {
            if(ids.empty()) return;
            std::vector<hpx::future<void> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , ids[0]
              , a0 , a1 , a2 , a3 , a4
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4
                          , global_idx + 1
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4
                          , global_idx + half
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
            }
            
            hpx::when_all(broadcast_futures).then(&return_void).get();
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4
        >
        
        typename broadcast_result<Action>::type
        broadcast_impl5(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4
          , std::size_t global_idx
          , boost::mpl::false_
        )
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef
                typename broadcast_result<Action>::type
                result_type;
            
            if(ids.empty()) return result_type();
            std::vector<hpx::future<result_type> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , &wrap_into_vector<action_result>
              , ids[0]
              , a0 , a1 , a2 , a3 , a4
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4
                          , global_idx + 1
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4
                          , global_idx + half
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
            }
            return hpx::when_all(broadcast_futures).
                then(&return_result_type<action_result>).move();
        }
        
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4
          , typename IsVoid
        >
        struct broadcast_invoker5
        {
            
            static typename broadcast_result<Action>::type
            call(
                Action const & act
              , std::vector<hpx::id_type> const & ids
              , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4
              , std::size_t global_idx
              , IsVoid
            )
            {
                return
                    broadcast_impl5(
                        act
                      , ids
                      , a0 , a1 , a2 , a3 , a4
                      , global_idx
                      , IsVoid()
                    );
            }
        };
        template <typename Action>
        struct make_broadcast_action_impl<Action, 5>
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef broadcast_invoker5<
                        Action
                      ,
                        typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 0 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 1 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 2 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 3 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 4 >::type
                      , typename boost::is_same<void, action_result>::type
                    >
                    broadcast_invoker_type;
            typedef
                typename HPX_MAKE_ACTION_TPL(broadcast_invoker_type::call)::type
                type;
        };
    }
    
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4)
    {
        hpx::id_type dest = hpx::get_colocation_id_sync(ids[0]);
        typedef
            typename detail::make_broadcast_action<Action>::type
            broadcast_impl_action;
        typedef
            typename detail::broadcast_result<Action>::action_result
            action_result;
        return
            hpx::async<broadcast_impl_action>(
                dest
              , Action()
              , ids
              , a0 , a1 , a2 , a3 , a4
              , 0
              , typename boost::is_same<void, action_result>::type()
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4)
    {
        return broadcast<Derived>(
                ids
              , a0 , a1 , a2 , a3 , a4
            );
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast_with_index(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4)
    {
        return broadcast<detail::broadcast_with_index<Action> >(
                ids
              , a0 , a1 , a2 , a3 , a4
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast_with_index(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4)
    {
        return broadcast<detail::broadcast_with_index<Derived> >(
                ids
              , a0 , a1 , a2 , a3 , a4
            );
    }
    
    namespace detail
    {
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4
        >
        hpx::future<
            typename detail::broadcast_result<Action>::type
        >
        invoke_broadcast5(
            std::vector<hpx::future<hpx::id_type> > const & fids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4)
        {
            return broadcast<Action>(resolve_futures(fids)
              , a0 , a1 , a2 , a3 , a4);
        }
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::future<hpx::id_type> > const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4)
    {
        typedef typename detail::broadcast_result<Action>::type result_type;
        typedef result_type (*invoke_broadcast_func)(
            std::vector<hpx::future<hpx::id_type> > const&
          ,
                A0 const& , A1 const& , A2 const& , A3 const& , A4 const&);
        return when_all(ids).then(
            util::bind(
                (invoke_broadcast_func)
                    &detail::invoke_broadcast5<
                        Action
                      , A0 , A1 , A2 , A3 , A4
                    >
              , util::placeholders::_1
              , a0 , a1 , a2 , a3 , a4
            )
        );
    }
}}
namespace hpx { namespace lcos {
    namespace detail
    {
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5
        >
        void
        broadcast_invoke(Action act, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5
                  , global_idx
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5
        >
        void
        broadcast_invoke(Action act, Futures& futures, BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures
          , BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5
                  , global_idx
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5
        >
        
        void
        broadcast_impl6(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5
          , std::size_t global_idx
          , boost::mpl::true_
        )
        {
            if(ids.empty()) return;
            std::vector<hpx::future<void> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5
                          , global_idx + 1
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5
                          , global_idx + half
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
            }
            
            hpx::when_all(broadcast_futures).then(&return_void).get();
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5
        >
        
        typename broadcast_result<Action>::type
        broadcast_impl6(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5
          , std::size_t global_idx
          , boost::mpl::false_
        )
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef
                typename broadcast_result<Action>::type
                result_type;
            
            if(ids.empty()) return result_type();
            std::vector<hpx::future<result_type> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , &wrap_into_vector<action_result>
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5
                          , global_idx + 1
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5
                          , global_idx + half
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
            }
            return hpx::when_all(broadcast_futures).
                then(&return_result_type<action_result>).move();
        }
        
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5
          , typename IsVoid
        >
        struct broadcast_invoker6
        {
            
            static typename broadcast_result<Action>::type
            call(
                Action const & act
              , std::vector<hpx::id_type> const & ids
              , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5
              , std::size_t global_idx
              , IsVoid
            )
            {
                return
                    broadcast_impl6(
                        act
                      , ids
                      , a0 , a1 , a2 , a3 , a4 , a5
                      , global_idx
                      , IsVoid()
                    );
            }
        };
        template <typename Action>
        struct make_broadcast_action_impl<Action, 6>
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef broadcast_invoker6<
                        Action
                      ,
                        typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 0 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 1 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 2 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 3 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 4 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 5 >::type
                      , typename boost::is_same<void, action_result>::type
                    >
                    broadcast_invoker_type;
            typedef
                typename HPX_MAKE_ACTION_TPL(broadcast_invoker_type::call)::type
                type;
        };
    }
    
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5)
    {
        hpx::id_type dest = hpx::get_colocation_id_sync(ids[0]);
        typedef
            typename detail::make_broadcast_action<Action>::type
            broadcast_impl_action;
        typedef
            typename detail::broadcast_result<Action>::action_result
            action_result;
        return
            hpx::async<broadcast_impl_action>(
                dest
              , Action()
              , ids
              , a0 , a1 , a2 , a3 , a4 , a5
              , 0
              , typename boost::is_same<void, action_result>::type()
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5)
    {
        return broadcast<Derived>(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5
            );
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast_with_index(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5)
    {
        return broadcast<detail::broadcast_with_index<Action> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast_with_index(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5)
    {
        return broadcast<detail::broadcast_with_index<Derived> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5
            );
    }
    
    namespace detail
    {
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5
        >
        hpx::future<
            typename detail::broadcast_result<Action>::type
        >
        invoke_broadcast6(
            std::vector<hpx::future<hpx::id_type> > const & fids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5)
        {
            return broadcast<Action>(resolve_futures(fids)
              , a0 , a1 , a2 , a3 , a4 , a5);
        }
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::future<hpx::id_type> > const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5)
    {
        typedef typename detail::broadcast_result<Action>::type result_type;
        typedef result_type (*invoke_broadcast_func)(
            std::vector<hpx::future<hpx::id_type> > const&
          ,
                A0 const& , A1 const& , A2 const& , A3 const& , A4 const& , A5 const&);
        return when_all(ids).then(
            util::bind(
                (invoke_broadcast_func)
                    &detail::invoke_broadcast6<
                        Action
                      , A0 , A1 , A2 , A3 , A4 , A5
                    >
              , util::placeholders::_1
              , a0 , a1 , a2 , a3 , a4 , a5
            )
        );
    }
}}
namespace hpx { namespace lcos {
    namespace detail
    {
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6
        >
        void
        broadcast_invoke(Action act, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6
                  , global_idx
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6
        >
        void
        broadcast_invoke(Action act, Futures& futures, BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures
          , BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6
                  , global_idx
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6
        >
        
        void
        broadcast_impl7(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6
          , std::size_t global_idx
          , boost::mpl::true_
        )
        {
            if(ids.empty()) return;
            std::vector<hpx::future<void> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5 , a6
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6
                          , global_idx + 1
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6
                          , global_idx + half
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
            }
            
            hpx::when_all(broadcast_futures).then(&return_void).get();
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6
        >
        
        typename broadcast_result<Action>::type
        broadcast_impl7(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6
          , std::size_t global_idx
          , boost::mpl::false_
        )
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef
                typename broadcast_result<Action>::type
                result_type;
            
            if(ids.empty()) return result_type();
            std::vector<hpx::future<result_type> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , &wrap_into_vector<action_result>
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5 , a6
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6
                          , global_idx + 1
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6
                          , global_idx + half
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
            }
            return hpx::when_all(broadcast_futures).
                then(&return_result_type<action_result>).move();
        }
        
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6
          , typename IsVoid
        >
        struct broadcast_invoker7
        {
            
            static typename broadcast_result<Action>::type
            call(
                Action const & act
              , std::vector<hpx::id_type> const & ids
              , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6
              , std::size_t global_idx
              , IsVoid
            )
            {
                return
                    broadcast_impl7(
                        act
                      , ids
                      , a0 , a1 , a2 , a3 , a4 , a5 , a6
                      , global_idx
                      , IsVoid()
                    );
            }
        };
        template <typename Action>
        struct make_broadcast_action_impl<Action, 7>
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef broadcast_invoker7<
                        Action
                      ,
                        typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 0 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 1 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 2 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 3 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 4 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 5 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 6 >::type
                      , typename boost::is_same<void, action_result>::type
                    >
                    broadcast_invoker_type;
            typedef
                typename HPX_MAKE_ACTION_TPL(broadcast_invoker_type::call)::type
                type;
        };
    }
    
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6)
    {
        hpx::id_type dest = hpx::get_colocation_id_sync(ids[0]);
        typedef
            typename detail::make_broadcast_action<Action>::type
            broadcast_impl_action;
        typedef
            typename detail::broadcast_result<Action>::action_result
            action_result;
        return
            hpx::async<broadcast_impl_action>(
                dest
              , Action()
              , ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6
              , 0
              , typename boost::is_same<void, action_result>::type()
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6)
    {
        return broadcast<Derived>(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6
            );
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast_with_index(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6)
    {
        return broadcast<detail::broadcast_with_index<Action> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast_with_index(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6)
    {
        return broadcast<detail::broadcast_with_index<Derived> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6
            );
    }
    
    namespace detail
    {
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6
        >
        hpx::future<
            typename detail::broadcast_result<Action>::type
        >
        invoke_broadcast7(
            std::vector<hpx::future<hpx::id_type> > const & fids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6)
        {
            return broadcast<Action>(resolve_futures(fids)
              , a0 , a1 , a2 , a3 , a4 , a5 , a6);
        }
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::future<hpx::id_type> > const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6)
    {
        typedef typename detail::broadcast_result<Action>::type result_type;
        typedef result_type (*invoke_broadcast_func)(
            std::vector<hpx::future<hpx::id_type> > const&
          ,
                A0 const& , A1 const& , A2 const& , A3 const& , A4 const& , A5 const& , A6 const&);
        return when_all(ids).then(
            util::bind(
                (invoke_broadcast_func)
                    &detail::invoke_broadcast7<
                        Action
                      , A0 , A1 , A2 , A3 , A4 , A5 , A6
                    >
              , util::placeholders::_1
              , a0 , a1 , a2 , a3 , a4 , a5 , a6
            )
        );
    }
}}
namespace hpx { namespace lcos {
    namespace detail
    {
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7
        >
        void
        broadcast_invoke(Action act, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7
                  , global_idx
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7
        >
        void
        broadcast_invoke(Action act, Futures& futures, BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures
          , BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7
                  , global_idx
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7
        >
        
        void
        broadcast_impl8(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7
          , std::size_t global_idx
          , boost::mpl::true_
        )
        {
            if(ids.empty()) return;
            std::vector<hpx::future<void> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7
                          , global_idx + 1
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7
                          , global_idx + half
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
            }
            
            hpx::when_all(broadcast_futures).then(&return_void).get();
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7
        >
        
        typename broadcast_result<Action>::type
        broadcast_impl8(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7
          , std::size_t global_idx
          , boost::mpl::false_
        )
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef
                typename broadcast_result<Action>::type
                result_type;
            
            if(ids.empty()) return result_type();
            std::vector<hpx::future<result_type> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , &wrap_into_vector<action_result>
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7
                          , global_idx + 1
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7
                          , global_idx + half
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
            }
            return hpx::when_all(broadcast_futures).
                then(&return_result_type<action_result>).move();
        }
        
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7
          , typename IsVoid
        >
        struct broadcast_invoker8
        {
            
            static typename broadcast_result<Action>::type
            call(
                Action const & act
              , std::vector<hpx::id_type> const & ids
              , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7
              , std::size_t global_idx
              , IsVoid
            )
            {
                return
                    broadcast_impl8(
                        act
                      , ids
                      , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7
                      , global_idx
                      , IsVoid()
                    );
            }
        };
        template <typename Action>
        struct make_broadcast_action_impl<Action, 8>
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef broadcast_invoker8<
                        Action
                      ,
                        typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 0 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 1 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 2 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 3 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 4 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 5 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 6 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 7 >::type
                      , typename boost::is_same<void, action_result>::type
                    >
                    broadcast_invoker_type;
            typedef
                typename HPX_MAKE_ACTION_TPL(broadcast_invoker_type::call)::type
                type;
        };
    }
    
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7)
    {
        hpx::id_type dest = hpx::get_colocation_id_sync(ids[0]);
        typedef
            typename detail::make_broadcast_action<Action>::type
            broadcast_impl_action;
        typedef
            typename detail::broadcast_result<Action>::action_result
            action_result;
        return
            hpx::async<broadcast_impl_action>(
                dest
              , Action()
              , ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7
              , 0
              , typename boost::is_same<void, action_result>::type()
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7)
    {
        return broadcast<Derived>(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7
            );
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast_with_index(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7)
    {
        return broadcast<detail::broadcast_with_index<Action> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast_with_index(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7)
    {
        return broadcast<detail::broadcast_with_index<Derived> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7
            );
    }
    
    namespace detail
    {
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7
        >
        hpx::future<
            typename detail::broadcast_result<Action>::type
        >
        invoke_broadcast8(
            std::vector<hpx::future<hpx::id_type> > const & fids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7)
        {
            return broadcast<Action>(resolve_futures(fids)
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7);
        }
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::future<hpx::id_type> > const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7)
    {
        typedef typename detail::broadcast_result<Action>::type result_type;
        typedef result_type (*invoke_broadcast_func)(
            std::vector<hpx::future<hpx::id_type> > const&
          ,
                A0 const& , A1 const& , A2 const& , A3 const& , A4 const& , A5 const& , A6 const& , A7 const&);
        return when_all(ids).then(
            util::bind(
                (invoke_broadcast_func)
                    &detail::invoke_broadcast8<
                        Action
                      , A0 , A1 , A2 , A3 , A4 , A5 , A6 , A7
                    >
              , util::placeholders::_1
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7
            )
        );
    }
}}
namespace hpx { namespace lcos {
    namespace detail
    {
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8
        >
        void
        broadcast_invoke(Action act, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8
                  , global_idx
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8
        >
        void
        broadcast_invoke(Action act, Futures& futures, BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures
          , BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8
                  , global_idx
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8
        >
        
        void
        broadcast_impl9(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8
          , std::size_t global_idx
          , boost::mpl::true_
        )
        {
            if(ids.empty()) return;
            std::vector<hpx::future<void> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8
                          , global_idx + 1
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8
                          , global_idx + half
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
            }
            
            hpx::when_all(broadcast_futures).then(&return_void).get();
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8
        >
        
        typename broadcast_result<Action>::type
        broadcast_impl9(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8
          , std::size_t global_idx
          , boost::mpl::false_
        )
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef
                typename broadcast_result<Action>::type
                result_type;
            
            if(ids.empty()) return result_type();
            std::vector<hpx::future<result_type> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , &wrap_into_vector<action_result>
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8
                          , global_idx + 1
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8
                          , global_idx + half
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
            }
            return hpx::when_all(broadcast_futures).
                then(&return_result_type<action_result>).move();
        }
        
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8
          , typename IsVoid
        >
        struct broadcast_invoker9
        {
            
            static typename broadcast_result<Action>::type
            call(
                Action const & act
              , std::vector<hpx::id_type> const & ids
              , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8
              , std::size_t global_idx
              , IsVoid
            )
            {
                return
                    broadcast_impl9(
                        act
                      , ids
                      , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8
                      , global_idx
                      , IsVoid()
                    );
            }
        };
        template <typename Action>
        struct make_broadcast_action_impl<Action, 9>
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef broadcast_invoker9<
                        Action
                      ,
                        typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 0 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 1 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 2 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 3 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 4 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 5 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 6 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 7 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 8 >::type
                      , typename boost::is_same<void, action_result>::type
                    >
                    broadcast_invoker_type;
            typedef
                typename HPX_MAKE_ACTION_TPL(broadcast_invoker_type::call)::type
                type;
        };
    }
    
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8)
    {
        hpx::id_type dest = hpx::get_colocation_id_sync(ids[0]);
        typedef
            typename detail::make_broadcast_action<Action>::type
            broadcast_impl_action;
        typedef
            typename detail::broadcast_result<Action>::action_result
            action_result;
        return
            hpx::async<broadcast_impl_action>(
                dest
              , Action()
              , ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8
              , 0
              , typename boost::is_same<void, action_result>::type()
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8)
    {
        return broadcast<Derived>(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8
            );
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast_with_index(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8)
    {
        return broadcast<detail::broadcast_with_index<Action> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast_with_index(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8)
    {
        return broadcast<detail::broadcast_with_index<Derived> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8
            );
    }
    
    namespace detail
    {
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8
        >
        hpx::future<
            typename detail::broadcast_result<Action>::type
        >
        invoke_broadcast9(
            std::vector<hpx::future<hpx::id_type> > const & fids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8)
        {
            return broadcast<Action>(resolve_futures(fids)
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8);
        }
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::future<hpx::id_type> > const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8)
    {
        typedef typename detail::broadcast_result<Action>::type result_type;
        typedef result_type (*invoke_broadcast_func)(
            std::vector<hpx::future<hpx::id_type> > const&
          ,
                A0 const& , A1 const& , A2 const& , A3 const& , A4 const& , A5 const& , A6 const& , A7 const& , A8 const&);
        return when_all(ids).then(
            util::bind(
                (invoke_broadcast_func)
                    &detail::invoke_broadcast9<
                        Action
                      , A0 , A1 , A2 , A3 , A4 , A5 , A6 , A7 , A8
                    >
              , util::placeholders::_1
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8
            )
        );
    }
}}
namespace hpx { namespace lcos {
    namespace detail
    {
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9
        >
        void
        broadcast_invoke(Action act, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9
                  , global_idx
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9
        >
        void
        broadcast_invoke(Action act, Futures& futures, BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures
          , BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9
                  , global_idx
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9
        >
        
        void
        broadcast_impl10(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9
          , std::size_t global_idx
          , boost::mpl::true_
        )
        {
            if(ids.empty()) return;
            std::vector<hpx::future<void> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9
                          , global_idx + 1
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9
                          , global_idx + half
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
            }
            
            hpx::when_all(broadcast_futures).then(&return_void).get();
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9
        >
        
        typename broadcast_result<Action>::type
        broadcast_impl10(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9
          , std::size_t global_idx
          , boost::mpl::false_
        )
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef
                typename broadcast_result<Action>::type
                result_type;
            
            if(ids.empty()) return result_type();
            std::vector<hpx::future<result_type> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , &wrap_into_vector<action_result>
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9
                          , global_idx + 1
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9
                          , global_idx + half
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
            }
            return hpx::when_all(broadcast_futures).
                then(&return_result_type<action_result>).move();
        }
        
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9
          , typename IsVoid
        >
        struct broadcast_invoker10
        {
            
            static typename broadcast_result<Action>::type
            call(
                Action const & act
              , std::vector<hpx::id_type> const & ids
              , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9
              , std::size_t global_idx
              , IsVoid
            )
            {
                return
                    broadcast_impl10(
                        act
                      , ids
                      , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9
                      , global_idx
                      , IsVoid()
                    );
            }
        };
        template <typename Action>
        struct make_broadcast_action_impl<Action, 10>
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef broadcast_invoker10<
                        Action
                      ,
                        typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 0 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 1 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 2 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 3 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 4 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 5 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 6 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 7 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 8 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 9 >::type
                      , typename boost::is_same<void, action_result>::type
                    >
                    broadcast_invoker_type;
            typedef
                typename HPX_MAKE_ACTION_TPL(broadcast_invoker_type::call)::type
                type;
        };
    }
    
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9)
    {
        hpx::id_type dest = hpx::get_colocation_id_sync(ids[0]);
        typedef
            typename detail::make_broadcast_action<Action>::type
            broadcast_impl_action;
        typedef
            typename detail::broadcast_result<Action>::action_result
            action_result;
        return
            hpx::async<broadcast_impl_action>(
                dest
              , Action()
              , ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9
              , 0
              , typename boost::is_same<void, action_result>::type()
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9)
    {
        return broadcast<Derived>(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9
            );
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast_with_index(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9)
    {
        return broadcast<detail::broadcast_with_index<Action> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast_with_index(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9)
    {
        return broadcast<detail::broadcast_with_index<Derived> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9
            );
    }
    
    namespace detail
    {
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9
        >
        hpx::future<
            typename detail::broadcast_result<Action>::type
        >
        invoke_broadcast10(
            std::vector<hpx::future<hpx::id_type> > const & fids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9)
        {
            return broadcast<Action>(resolve_futures(fids)
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9);
        }
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::future<hpx::id_type> > const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9)
    {
        typedef typename detail::broadcast_result<Action>::type result_type;
        typedef result_type (*invoke_broadcast_func)(
            std::vector<hpx::future<hpx::id_type> > const&
          ,
                A0 const& , A1 const& , A2 const& , A3 const& , A4 const& , A5 const& , A6 const& , A7 const& , A8 const& , A9 const&);
        return when_all(ids).then(
            util::bind(
                (invoke_broadcast_func)
                    &detail::invoke_broadcast10<
                        Action
                      , A0 , A1 , A2 , A3 , A4 , A5 , A6 , A7 , A8 , A9
                    >
              , util::placeholders::_1
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9
            )
        );
    }
}}
namespace hpx { namespace lcos {
    namespace detail
    {
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10
        >
        void
        broadcast_invoke(Action act, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10
                  , global_idx
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10
        >
        void
        broadcast_invoke(Action act, Futures& futures, BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures
          , BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10
                  , global_idx
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10
        >
        
        void
        broadcast_impl11(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10
          , std::size_t global_idx
          , boost::mpl::true_
        )
        {
            if(ids.empty()) return;
            std::vector<hpx::future<void> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10
                          , global_idx + 1
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10
                          , global_idx + half
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
            }
            
            hpx::when_all(broadcast_futures).then(&return_void).get();
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10
        >
        
        typename broadcast_result<Action>::type
        broadcast_impl11(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10
          , std::size_t global_idx
          , boost::mpl::false_
        )
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef
                typename broadcast_result<Action>::type
                result_type;
            
            if(ids.empty()) return result_type();
            std::vector<hpx::future<result_type> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , &wrap_into_vector<action_result>
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10
                          , global_idx + 1
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10
                          , global_idx + half
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
            }
            return hpx::when_all(broadcast_futures).
                then(&return_result_type<action_result>).move();
        }
        
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10
          , typename IsVoid
        >
        struct broadcast_invoker11
        {
            
            static typename broadcast_result<Action>::type
            call(
                Action const & act
              , std::vector<hpx::id_type> const & ids
              , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10
              , std::size_t global_idx
              , IsVoid
            )
            {
                return
                    broadcast_impl11(
                        act
                      , ids
                      , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10
                      , global_idx
                      , IsVoid()
                    );
            }
        };
        template <typename Action>
        struct make_broadcast_action_impl<Action, 11>
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef broadcast_invoker11<
                        Action
                      ,
                        typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 0 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 1 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 2 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 3 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 4 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 5 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 6 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 7 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 8 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 9 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 10 >::type
                      , typename boost::is_same<void, action_result>::type
                    >
                    broadcast_invoker_type;
            typedef
                typename HPX_MAKE_ACTION_TPL(broadcast_invoker_type::call)::type
                type;
        };
    }
    
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10)
    {
        hpx::id_type dest = hpx::get_colocation_id_sync(ids[0]);
        typedef
            typename detail::make_broadcast_action<Action>::type
            broadcast_impl_action;
        typedef
            typename detail::broadcast_result<Action>::action_result
            action_result;
        return
            hpx::async<broadcast_impl_action>(
                dest
              , Action()
              , ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10
              , 0
              , typename boost::is_same<void, action_result>::type()
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10)
    {
        return broadcast<Derived>(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10
            );
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast_with_index(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10)
    {
        return broadcast<detail::broadcast_with_index<Action> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast_with_index(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10)
    {
        return broadcast<detail::broadcast_with_index<Derived> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10
            );
    }
    
    namespace detail
    {
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10
        >
        hpx::future<
            typename detail::broadcast_result<Action>::type
        >
        invoke_broadcast11(
            std::vector<hpx::future<hpx::id_type> > const & fids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10)
        {
            return broadcast<Action>(resolve_futures(fids)
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10);
        }
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::future<hpx::id_type> > const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10)
    {
        typedef typename detail::broadcast_result<Action>::type result_type;
        typedef result_type (*invoke_broadcast_func)(
            std::vector<hpx::future<hpx::id_type> > const&
          ,
                A0 const& , A1 const& , A2 const& , A3 const& , A4 const& , A5 const& , A6 const& , A7 const& , A8 const& , A9 const& , A10 const&);
        return when_all(ids).then(
            util::bind(
                (invoke_broadcast_func)
                    &detail::invoke_broadcast11<
                        Action
                      , A0 , A1 , A2 , A3 , A4 , A5 , A6 , A7 , A8 , A9 , A10
                    >
              , util::placeholders::_1
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10
            )
        );
    }
}}
namespace hpx { namespace lcos {
    namespace detail
    {
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11
        >
        void
        broadcast_invoke(Action act, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11
                  , global_idx
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11
        >
        void
        broadcast_invoke(Action act, Futures& futures, BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures
          , BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11
                  , global_idx
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11
        >
        
        void
        broadcast_impl12(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11
          , std::size_t global_idx
          , boost::mpl::true_
        )
        {
            if(ids.empty()) return;
            std::vector<hpx::future<void> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11
                          , global_idx + 1
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11
                          , global_idx + half
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
            }
            
            hpx::when_all(broadcast_futures).then(&return_void).get();
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11
        >
        
        typename broadcast_result<Action>::type
        broadcast_impl12(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11
          , std::size_t global_idx
          , boost::mpl::false_
        )
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef
                typename broadcast_result<Action>::type
                result_type;
            
            if(ids.empty()) return result_type();
            std::vector<hpx::future<result_type> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , &wrap_into_vector<action_result>
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11
                          , global_idx + 1
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11
                          , global_idx + half
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
            }
            return hpx::when_all(broadcast_futures).
                then(&return_result_type<action_result>).move();
        }
        
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11
          , typename IsVoid
        >
        struct broadcast_invoker12
        {
            
            static typename broadcast_result<Action>::type
            call(
                Action const & act
              , std::vector<hpx::id_type> const & ids
              , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11
              , std::size_t global_idx
              , IsVoid
            )
            {
                return
                    broadcast_impl12(
                        act
                      , ids
                      , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11
                      , global_idx
                      , IsVoid()
                    );
            }
        };
        template <typename Action>
        struct make_broadcast_action_impl<Action, 12>
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef broadcast_invoker12<
                        Action
                      ,
                        typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 0 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 1 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 2 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 3 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 4 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 5 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 6 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 7 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 8 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 9 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 10 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 11 >::type
                      , typename boost::is_same<void, action_result>::type
                    >
                    broadcast_invoker_type;
            typedef
                typename HPX_MAKE_ACTION_TPL(broadcast_invoker_type::call)::type
                type;
        };
    }
    
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11)
    {
        hpx::id_type dest = hpx::get_colocation_id_sync(ids[0]);
        typedef
            typename detail::make_broadcast_action<Action>::type
            broadcast_impl_action;
        typedef
            typename detail::broadcast_result<Action>::action_result
            action_result;
        return
            hpx::async<broadcast_impl_action>(
                dest
              , Action()
              , ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11
              , 0
              , typename boost::is_same<void, action_result>::type()
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11)
    {
        return broadcast<Derived>(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11
            );
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast_with_index(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11)
    {
        return broadcast<detail::broadcast_with_index<Action> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast_with_index(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11)
    {
        return broadcast<detail::broadcast_with_index<Derived> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11
            );
    }
    
    namespace detail
    {
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11
        >
        hpx::future<
            typename detail::broadcast_result<Action>::type
        >
        invoke_broadcast12(
            std::vector<hpx::future<hpx::id_type> > const & fids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11)
        {
            return broadcast<Action>(resolve_futures(fids)
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11);
        }
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::future<hpx::id_type> > const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11)
    {
        typedef typename detail::broadcast_result<Action>::type result_type;
        typedef result_type (*invoke_broadcast_func)(
            std::vector<hpx::future<hpx::id_type> > const&
          ,
                A0 const& , A1 const& , A2 const& , A3 const& , A4 const& , A5 const& , A6 const& , A7 const& , A8 const& , A9 const& , A10 const& , A11 const&);
        return when_all(ids).then(
            util::bind(
                (invoke_broadcast_func)
                    &detail::invoke_broadcast12<
                        Action
                      , A0 , A1 , A2 , A3 , A4 , A5 , A6 , A7 , A8 , A9 , A10 , A11
                    >
              , util::placeholders::_1
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11
            )
        );
    }
}}
namespace hpx { namespace lcos {
    namespace detail
    {
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12
        >
        void
        broadcast_invoke(Action act, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12
                  , global_idx
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12
        >
        void
        broadcast_invoke(Action act, Futures& futures, BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures
          , BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12
                  , global_idx
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12
        >
        
        void
        broadcast_impl13(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12
          , std::size_t global_idx
          , boost::mpl::true_
        )
        {
            if(ids.empty()) return;
            std::vector<hpx::future<void> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12
                          , global_idx + 1
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12
                          , global_idx + half
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
            }
            
            hpx::when_all(broadcast_futures).then(&return_void).get();
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12
        >
        
        typename broadcast_result<Action>::type
        broadcast_impl13(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12
          , std::size_t global_idx
          , boost::mpl::false_
        )
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef
                typename broadcast_result<Action>::type
                result_type;
            
            if(ids.empty()) return result_type();
            std::vector<hpx::future<result_type> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , &wrap_into_vector<action_result>
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12
                          , global_idx + 1
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12
                          , global_idx + half
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
            }
            return hpx::when_all(broadcast_futures).
                then(&return_result_type<action_result>).move();
        }
        
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12
          , typename IsVoid
        >
        struct broadcast_invoker13
        {
            
            static typename broadcast_result<Action>::type
            call(
                Action const & act
              , std::vector<hpx::id_type> const & ids
              , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12
              , std::size_t global_idx
              , IsVoid
            )
            {
                return
                    broadcast_impl13(
                        act
                      , ids
                      , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12
                      , global_idx
                      , IsVoid()
                    );
            }
        };
        template <typename Action>
        struct make_broadcast_action_impl<Action, 13>
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef broadcast_invoker13<
                        Action
                      ,
                        typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 0 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 1 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 2 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 3 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 4 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 5 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 6 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 7 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 8 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 9 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 10 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 11 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 12 >::type
                      , typename boost::is_same<void, action_result>::type
                    >
                    broadcast_invoker_type;
            typedef
                typename HPX_MAKE_ACTION_TPL(broadcast_invoker_type::call)::type
                type;
        };
    }
    
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12)
    {
        hpx::id_type dest = hpx::get_colocation_id_sync(ids[0]);
        typedef
            typename detail::make_broadcast_action<Action>::type
            broadcast_impl_action;
        typedef
            typename detail::broadcast_result<Action>::action_result
            action_result;
        return
            hpx::async<broadcast_impl_action>(
                dest
              , Action()
              , ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12
              , 0
              , typename boost::is_same<void, action_result>::type()
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12)
    {
        return broadcast<Derived>(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12
            );
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast_with_index(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12)
    {
        return broadcast<detail::broadcast_with_index<Action> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast_with_index(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12)
    {
        return broadcast<detail::broadcast_with_index<Derived> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12
            );
    }
    
    namespace detail
    {
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12
        >
        hpx::future<
            typename detail::broadcast_result<Action>::type
        >
        invoke_broadcast13(
            std::vector<hpx::future<hpx::id_type> > const & fids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12)
        {
            return broadcast<Action>(resolve_futures(fids)
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12);
        }
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::future<hpx::id_type> > const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12)
    {
        typedef typename detail::broadcast_result<Action>::type result_type;
        typedef result_type (*invoke_broadcast_func)(
            std::vector<hpx::future<hpx::id_type> > const&
          ,
                A0 const& , A1 const& , A2 const& , A3 const& , A4 const& , A5 const& , A6 const& , A7 const& , A8 const& , A9 const& , A10 const& , A11 const& , A12 const&);
        return when_all(ids).then(
            util::bind(
                (invoke_broadcast_func)
                    &detail::invoke_broadcast13<
                        Action
                      , A0 , A1 , A2 , A3 , A4 , A5 , A6 , A7 , A8 , A9 , A10 , A11 , A12
                    >
              , util::placeholders::_1
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12
            )
        );
    }
}}
namespace hpx { namespace lcos {
    namespace detail
    {
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13
        >
        void
        broadcast_invoke(Action act, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13
                  , global_idx
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13
        >
        void
        broadcast_invoke(Action act, Futures& futures, BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures
          , BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13
                  , global_idx
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13
        >
        
        void
        broadcast_impl14(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13
          , std::size_t global_idx
          , boost::mpl::true_
        )
        {
            if(ids.empty()) return;
            std::vector<hpx::future<void> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13
                          , global_idx + 1
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13
                          , global_idx + half
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
            }
            
            hpx::when_all(broadcast_futures).then(&return_void).get();
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13
        >
        
        typename broadcast_result<Action>::type
        broadcast_impl14(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13
          , std::size_t global_idx
          , boost::mpl::false_
        )
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef
                typename broadcast_result<Action>::type
                result_type;
            
            if(ids.empty()) return result_type();
            std::vector<hpx::future<result_type> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , &wrap_into_vector<action_result>
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13
                          , global_idx + 1
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13
                          , global_idx + half
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
            }
            return hpx::when_all(broadcast_futures).
                then(&return_result_type<action_result>).move();
        }
        
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13
          , typename IsVoid
        >
        struct broadcast_invoker14
        {
            
            static typename broadcast_result<Action>::type
            call(
                Action const & act
              , std::vector<hpx::id_type> const & ids
              , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13
              , std::size_t global_idx
              , IsVoid
            )
            {
                return
                    broadcast_impl14(
                        act
                      , ids
                      , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13
                      , global_idx
                      , IsVoid()
                    );
            }
        };
        template <typename Action>
        struct make_broadcast_action_impl<Action, 14>
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef broadcast_invoker14<
                        Action
                      ,
                        typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 0 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 1 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 2 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 3 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 4 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 5 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 6 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 7 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 8 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 9 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 10 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 11 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 12 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 13 >::type
                      , typename boost::is_same<void, action_result>::type
                    >
                    broadcast_invoker_type;
            typedef
                typename HPX_MAKE_ACTION_TPL(broadcast_invoker_type::call)::type
                type;
        };
    }
    
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13)
    {
        hpx::id_type dest = hpx::get_colocation_id_sync(ids[0]);
        typedef
            typename detail::make_broadcast_action<Action>::type
            broadcast_impl_action;
        typedef
            typename detail::broadcast_result<Action>::action_result
            action_result;
        return
            hpx::async<broadcast_impl_action>(
                dest
              , Action()
              , ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13
              , 0
              , typename boost::is_same<void, action_result>::type()
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13)
    {
        return broadcast<Derived>(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13
            );
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast_with_index(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13)
    {
        return broadcast<detail::broadcast_with_index<Action> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast_with_index(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13)
    {
        return broadcast<detail::broadcast_with_index<Derived> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13
            );
    }
    
    namespace detail
    {
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13
        >
        hpx::future<
            typename detail::broadcast_result<Action>::type
        >
        invoke_broadcast14(
            std::vector<hpx::future<hpx::id_type> > const & fids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13)
        {
            return broadcast<Action>(resolve_futures(fids)
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13);
        }
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::future<hpx::id_type> > const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13)
    {
        typedef typename detail::broadcast_result<Action>::type result_type;
        typedef result_type (*invoke_broadcast_func)(
            std::vector<hpx::future<hpx::id_type> > const&
          ,
                A0 const& , A1 const& , A2 const& , A3 const& , A4 const& , A5 const& , A6 const& , A7 const& , A8 const& , A9 const& , A10 const& , A11 const& , A12 const& , A13 const&);
        return when_all(ids).then(
            util::bind(
                (invoke_broadcast_func)
                    &detail::invoke_broadcast14<
                        Action
                      , A0 , A1 , A2 , A3 , A4 , A5 , A6 , A7 , A8 , A9 , A10 , A11 , A12 , A13
                    >
              , util::placeholders::_1
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13
            )
        );
    }
}}
namespace hpx { namespace lcos {
    namespace detail
    {
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13 , typename A14
        >
        void
        broadcast_invoke(Action act, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13 , A14 const & a14
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13 , a14
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13 , typename A14
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures, hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13 , A14 const & a14
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13 , a14
                  , global_idx
                )
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13 , typename A14
        >
        void
        broadcast_invoke(Action act, Futures& futures, BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13 , A14 const & a14
          , std::size_t)
        {
            futures.push_back(
                hpx::async(
                    act
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13 , a14
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename Futures
          , typename Cont
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13 , typename A14
        >
        void
        broadcast_invoke(broadcast_with_index<Action>, Futures& futures
          , BOOST_FWD_REF(Cont) cont
          , hpx::id_type id
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13 , A14 const & a14
          , std::size_t global_idx)
        {
            futures.push_back(
                hpx::async(
                    Action()
                  , id
                  , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13 , a14
                  , global_idx
                ).then(boost::forward<Cont>(cont))
            );
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13 , typename A14
        >
        
        void
        broadcast_impl15(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13 , A14 const & a14
          , std::size_t global_idx
          , boost::mpl::true_
        )
        {
            if(ids.empty()) return;
            std::vector<hpx::future<void> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13 , a14
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13 , a14
                          , global_idx + 1
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13 , a14
                          , global_idx + half
                          , boost::integral_constant<bool, true>::type()
                        )
                    );
                }
            }
            
            hpx::when_all(broadcast_futures).then(&return_void).get();
        }
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13 , typename A14
        >
        
        typename broadcast_result<Action>::type
        broadcast_impl15(
            Action const & act
          , std::vector<hpx::id_type> const & ids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13 , A14 const & a14
          , std::size_t global_idx
          , boost::mpl::false_
        )
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef
                typename broadcast_result<Action>::type
                result_type;
            
            if(ids.empty()) return result_type();
            std::vector<hpx::future<result_type> > broadcast_futures;
            broadcast_futures.reserve(3);
            broadcast_invoke(
                act
              , broadcast_futures
              , &wrap_into_vector<action_result>
              , ids[0]
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13 , a14
              , global_idx
            );
            if(ids.size() > 1)
            {
                std::size_t half = (ids.size() / 2) + 1;
                std::vector<hpx::id_type>
                    ids_first(ids.begin() + 1, ids.begin() + half);
                std::vector<hpx::id_type>
                    ids_second(ids.begin() + half, ids.end());
                typedef
                    typename detail::make_broadcast_action<
                        Action
                    >::type
                    broadcast_impl_action;
                if(!ids_first.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_first[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_first)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13 , a14
                          , global_idx + 1
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
                if(!ids_second.empty())
                {
                    hpx::id_type id = hpx::get_colocation_id_sync(ids_second[0]);
                    broadcast_futures.push_back(
                        hpx::async<broadcast_impl_action>(
                            id
                          , act
                          , boost::move(ids_second)
                          , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13 , a14
                          , global_idx + half
                          , boost::integral_constant<bool, false>::type()
                        )
                    );
                }
            }
            return hpx::when_all(broadcast_futures).
                then(&return_result_type<action_result>).move();
        }
        
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13 , typename A14
          , typename IsVoid
        >
        struct broadcast_invoker15
        {
            
            static typename broadcast_result<Action>::type
            call(
                Action const & act
              , std::vector<hpx::id_type> const & ids
              , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13 , A14 const & a14
              , std::size_t global_idx
              , IsVoid
            )
            {
                return
                    broadcast_impl15(
                        act
                      , ids
                      , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13 , a14
                      , global_idx
                      , IsVoid()
                    );
            }
        };
        template <typename Action>
        struct make_broadcast_action_impl<Action, 15>
        {
            typedef
                typename broadcast_result<Action>::action_result
                action_result;
            typedef broadcast_invoker15<
                        Action
                      ,
                        typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 0 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 1 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 2 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 3 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 4 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 5 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 6 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 7 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 8 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 9 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 10 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 11 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 12 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 13 >::type , typename boost::fusion::result_of::value_at_c< typename Action::arguments_type, 14 >::type
                      , typename boost::is_same<void, action_result>::type
                    >
                    broadcast_invoker_type;
            typedef
                typename HPX_MAKE_ACTION_TPL(broadcast_invoker_type::call)::type
                type;
        };
    }
    
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13 , typename A14
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13 , A14 const & a14)
    {
        hpx::id_type dest = hpx::get_colocation_id_sync(ids[0]);
        typedef
            typename detail::make_broadcast_action<Action>::type
            broadcast_impl_action;
        typedef
            typename detail::broadcast_result<Action>::action_result
            action_result;
        return
            hpx::async<broadcast_impl_action>(
                dest
              , Action()
              , ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13 , a14
              , 0
              , typename boost::is_same<void, action_result>::type()
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13 , typename A14
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13 , A14 const & a14)
    {
        return broadcast<Derived>(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13 , a14
            );
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13 , typename A14
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast_with_index(
        std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13 , A14 const & a14)
    {
        return broadcast<detail::broadcast_with_index<Action> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13 , a14
            );
    }
    template <
        typename Component
      , typename Result
      , typename Arguments
      , typename Derived
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13 , typename A14
    >
    hpx::future<
        typename detail::broadcast_result<Derived>::type
    >
    broadcast_with_index(
        hpx::actions::action<
            Component, Result, Arguments, Derived
        > 
      , std::vector<hpx::id_type> const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13 , A14 const & a14)
    {
        return broadcast<detail::broadcast_with_index<Derived> >(
                ids
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13 , a14
            );
    }
    
    namespace detail
    {
        template <
            typename Action
          , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13 , typename A14
        >
        hpx::future<
            typename detail::broadcast_result<Action>::type
        >
        invoke_broadcast15(
            std::vector<hpx::future<hpx::id_type> > const & fids
          , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13 , A14 const & a14)
        {
            return broadcast<Action>(resolve_futures(fids)
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13 , a14);
        }
    }
    template <
        typename Action
      , typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9 , typename A10 , typename A11 , typename A12 , typename A13 , typename A14
    >
    hpx::future<
        typename detail::broadcast_result<Action>::type
    >
    broadcast(
        std::vector<hpx::future<hpx::id_type> > const & ids
      , A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5 , A6 const & a6 , A7 const & a7 , A8 const & a8 , A9 const & a9 , A10 const & a10 , A11 const & a11 , A12 const & a12 , A13 const & a13 , A14 const & a14)
    {
        typedef typename detail::broadcast_result<Action>::type result_type;
        typedef result_type (*invoke_broadcast_func)(
            std::vector<hpx::future<hpx::id_type> > const&
          ,
                A0 const& , A1 const& , A2 const& , A3 const& , A4 const& , A5 const& , A6 const& , A7 const& , A8 const& , A9 const& , A10 const& , A11 const& , A12 const& , A13 const& , A14 const&);
        return when_all(ids).then(
            util::bind(
                (invoke_broadcast_func)
                    &detail::invoke_broadcast15<
                        Action
                      , A0 , A1 , A2 , A3 , A4 , A5 , A6 , A7 , A8 , A9 , A10 , A11 , A12 , A13 , A14
                    >
              , util::placeholders::_1
              , a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9 , a10 , a11 , a12 , a13 , a14
            )
        );
    }
}}
