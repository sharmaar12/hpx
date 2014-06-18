//  Copyright (c) 2014 Anuj R. Sharma
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/components/vector/chunk_vector_component.hpp>
HPX_REGISTER_COMPONENT_MODULE();

typedef hpx::components::managed_component<hpx::server::chunk_vector> chunk_vector_type;

HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(chunk_vector_type, chunk_vector);

//Capacity related action registration
HPX_REGISTER_ACTION(
    hpx::server::chunk_vector::size_action,
    chunk_vector_size_action);
HPX_REGISTER_ACTION(
    hpx::server::chunk_vector::max_size_action,
    chunk_vector_max_size_action);
HPX_REGISTER_ACTION(
    hpx::server::chunk_vector::resize_only_action,
    chunk_vector_resize_only_action);
HPX_REGISTER_ACTION(
    hpx::server::chunk_vector::resize_with_val_action,
    chunk_vector_resize_with_val_action);
HPX_REGISTER_ACTION(
    hpx::server::chunk_vector::capacity_action,
    chunk_vector_capacity_action);
HPX_REGISTER_ACTION(
    hpx::server::chunk_vector::empty_action,
    chunk_vector_empty_action);
HPX_REGISTER_ACTION(
    hpx::server::chunk_vector::reserve_action,
    chunk_vector_reserve_action);

//Element access component action registration
HPX_REGISTER_ACTION(
    hpx::server::chunk_vector::get_value_action,
    chunk_vector_get_value_action);
HPX_REGISTER_ACTION(
    hpx::server::chunk_vector::front_action,
    chunk_vector_front_action);
HPX_REGISTER_ACTION(
    hpx::server::chunk_vector::back_action,
    chunk_vector_back_action);

////Modifiers component action registration
HPX_REGISTER_ACTION(
    hpx::server::chunk_vector::assign_action,
    chunk_vector_assign_action);
HPX_REGISTER_ACTION(
    hpx::server::chunk_vector::push_back_action,
    chunk_vector_push_back_action);
HPX_REGISTER_ACTION(
    hpx::server::chunk_vector::push_back_rval_action,
    chunk_vector_push_back_rval_action);
HPX_REGISTER_ACTION(
    hpx::server::chunk_vector::pop_back_action,
    chunk_vector_pop_back_action);
HPX_REGISTER_ACTION(
    hpx::server::chunk_vector::set_value_action,
    chunk_vector_set_value_action);
HPX_REGISTER_ACTION(
    hpx::server::chunk_vector::set_value_rval_action,
    chunk_vector_set_value_rval_action);
//HPX_REGISTER_ACTION(
//    hpx::server::chunk_vector::swap_action,
//    chunk_vector_swap_action);
HPX_REGISTER_ACTION(
    hpx::server::chunk_vector::clear_action,
    chunk_vector_clear_action);

