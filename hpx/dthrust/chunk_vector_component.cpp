#include "chunk_vector_component.hpp"
HPX_REGISTER_COMPONENT_MODULE();

typedef hpx::components::managed_component<hpx::dthrust::server::chunk_vector> chunk_vector_type;

HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(chunk_vector_type, chunk_vector);

HPX_REGISTER_ACTION(
    hpx::dthrust::server::chunk_vector::display_action,
    chunk_vector_display_action);
HPX_REGISTER_ACTION(
    hpx::dthrust::server::chunk_vector::size_action,
    chunk_vector_size_action);
HPX_REGISTER_ACTION(
    hpx::dthrust::server::chunk_vector::push_back_action,
    chunk_vector_push_back_action);
HPX_REGISTER_ACTION(
    hpx::dthrust::server::chunk_vector::push_back_rval_action,
    chunk_vector_push_back_rval_action);
HPX_REGISTER_ACTION(
    hpx::dthrust::server::chunk_vector::at_action,
    chunk_vector_at_action);
HPX_REGISTER_ACTION(
    hpx::dthrust::server::chunk_vector::set_value_action,
    chunk_vector_set_value_action);
HPX_REGISTER_ACTION(
    hpx::dthrust::server::chunk_vector::set_value_rval_action,
    chunk_vector_set_value_rval_action);
HPX_REGISTER_ACTION(
    hpx::dthrust::server::chunk_vector::empty_action,
    chunk_vector_empty_action);
HPX_REGISTER_ACTION(
    hpx::dthrust::server::chunk_vector::clear_action,
    chunk_vector_clear_action);
