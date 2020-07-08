#include "FakeLibImplementation.h"
#include "CallBacks.h"

#include <iostream>
#include <array>
#include <exception>
#include <iostream>

namespace FakeLibImplementation
{
std::vector<ObjectVariant> run_pa_commands(std::vector<ObjectVariant>& objects) {
	// Define our pulse audio loop and connection variables
	pa_mainloop *ml;
	pa_mainloop_api *mlapi;
	pa_operation *op = NULL;
	pa_context *ctx;

	// We'll need these state variables to keep track of our requests
	int ready = 0;

	// Create a mainloop API and connection to the default server
	ml = pa_mainloop_new();
	mlapi = pa_mainloop_get_api(ml);
	ctx = pa_context_new(mlapi, "test");

	// This function connects to the pulse server
	pa_context_connect(ctx, NULL, F_0, NULL); // This function defines a callback so the server will tell us it's
	// state. Our callback will wait for the state to be ready.  The
	// callback will modify the variable to 1 so we know when we have a
	// connection and it's ready. If there's an error, the callback will set
	// ready to 2
	pa_context_set_state_callback(ctx, state_cb, &ready);

	// Now we'll enter into an infinite loop until we get the data we
	// receive or if there's an error
	while (true) {
		// We can't do anything until PA is ready, so just iterate the
		// mainloop and continue
		if (ready == 0) {
			pa_mainloop_iterate(ml, 1, NULL);
			continue;
		}
		// We couldn't get a connection to the server, so exit out
		if (ready == 2) {
			pa_context_disconnect(ctx);
			pa_context_unref(ctx);
			pa_mainloop_free(ml);
			throw ConnectionToServerFailure();		
		}

		break;
		// Iterate the main loop and go again.  The second argument is
		// whether or not the iteration should block until something is
		// ready to be done.  Set it to zero for non-blocking.
	}
	uint32_t objectIndex = 0;
	while (true) {
		if (objectIndex >= objects.size()){
			break;
		}

		auto& object = objects[objectIndex];
		// move source output port 
		try {
			auto& objectPtr = std::get<move_source_output_port_t>(object);
			op = pa_context_move_source_output_by_index(
					ctx,
					objectPtr.sourceIndex,
					objectPtr.index,
					move_source_output_port_cb,
					&objectPtr);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// move sink input port 
		try {
			auto& objectPtr = std::get<move_sink_input_port_t>(object);
			op = pa_context_move_sink_input_by_index(
					ctx,
					objectPtr.sinkIndex,
					objectPtr.index,
					move_sink_input_port_cb,
					&objectPtr);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// load module
		try {
			auto& objectPtr = std::get<load_module_t>(object);
			op = pa_context_load_module(
					ctx,
					objectPtr.name.c_str(),
					objectPtr.arguments.c_str(),
					load_module_infos_cb,
					&objectPtr);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// unload module
		try {
			auto& objectPtr = std::get<unload_module_t>(object);
			op = pa_context_unload_module(
					ctx,
					objectPtr.index,
					unload_module_cb,
					&objectPtr);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// set source volume
		try {
			auto& objectPtr = std::get<set_sink_volume_t>(object);
			double v = objectPtr.volume/100.0;
			auto volume = pa_sw_volume_from_linear(v);
			pa_cvolume_set(&(objectPtr.cvolume), 2, volume);
			op = pa_context_set_sink_volume_by_index(
					ctx,
					objectPtr.index,
					&(objectPtr.cvolume),
					set_sink_volume_cb,
					&objectPtr);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// set sink input volume
		try {
			auto& objectPtr = std::get<set_sink_input_volume_t>(object);
			double v = objectPtr.volume/100.0;
			auto volume = pa_sw_volume_from_linear(v);
			pa_cvolume_set(&(objectPtr.cvolume), 2, volume);
			op = pa_context_set_sink_input_volume(
					ctx,
					objectPtr.index,
					&(objectPtr.cvolume),
					set_sink_input_volume_cb,
					&objectPtr);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// get modules list 
		try {
			auto& objectPtr = std::get<info_list<module_infos_t>>(object);
			op = pa_context_get_module_info_list(
					ctx,
					module_infos_list_cb,
					&objectPtr);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// get sinks list 
		try {
			auto& objectPtr = std::get<info_list<sink_infos_t>>(object);
			op = pa_context_get_sink_info_list(
					ctx,
					sink_infos_list_cb,
					&objectPtr);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// get sink inputs list 
		try {
			auto& objectPtr = std::get<info_list<sink_input_infos_t>>(object);
			op = pa_context_get_sink_input_info_list(
					ctx,
					sink_input_infos_list_cb,
					&objectPtr);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// get source list 
		try {
			auto& objectPtr = std::get<info_list<source_infos_t>>(object);
			op = pa_context_get_source_info_list(
					ctx,
					source_infos_list_cb,
					&objectPtr);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {
		}
		// get source output list 
		try {
			auto& objectPtr = std::get<info_list<source_output_infos_t>>(object);
			op = pa_context_get_source_output_info_list(
					ctx,
					source_output_infos_list_cb,
					&objectPtr);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {
		}
		// get module infos
		try {
			auto& objectPtr = std::get<module_infos_t>(object);
			op = pa_context_get_module_info(
					ctx,
					objectPtr.index,
					module_infos_cb,
					&objectPtr);
			++objectIndex;
			throw std::exception();
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// get sink infos
		try {
			auto& objectPtr = std::get<sink_infos_t>(object);
			op = pa_context_get_sink_info_by_index(
					ctx,
					objectPtr.index,
					sink_infos_cb,
					&objectPtr);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// get sink input infos
		try {
			auto& objectPtr = std::get<sink_input_infos_t>(object);
			op = pa_context_get_sink_input_info(
					ctx,
					objectPtr.index,
					sink_input_infos_cb,
					&objectPtr);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// get source infos
		try {
			auto& objectPtr = std::get<source_infos_t>(object);
			op = pa_context_get_source_info_by_index(
					ctx,
					objectPtr.index,
					source_infos_cb,
					&objectPtr);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// get source output infos
		try {
			auto& objectPtr = std::get<source_output_infos_t>(object);
			op = pa_context_get_source_output_info(
					ctx,
					objectPtr.index,
					source_output_infos_cb,
					&objectPtr);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}

		throw UnknownObjectError();
iterate:
		while (pa_operation_get_state(op) != PA_OPERATION_DONE)
			 pa_mainloop_iterate(ml, 1, NULL);
	}


	// Clean up time
	if (pa_operation_get_state(op) == PA_OPERATION_DONE) {
		// Now we're done, clean up and disconnect and
		// return
		pa_operation_unref(op);
		op = NULL;
		pa_context_disconnect(ctx);
		pa_context_unref(ctx);
		pa_mainloop_free(ml);
	}
	return objects;
}
}

