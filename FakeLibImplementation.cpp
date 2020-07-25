#include "FakeLibImplementation.h"
#include "CallBacks.h"

#include <iostream>
#include <array>
#include <exception>
#include <iostream>

pa_mainloop* FakeLibImplementation::ml;
pa_context* FakeLibImplementation::ctx;
pa_operation* FakeLibImplementation::op = NULL;
set_subscribe_callback_info_t FakeLibImplementation::callback_info;

std::vector<ObjectVariant> FakeLibImplementation::run_pa_commands(std::vector<ObjectVariant>& objects) {
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
			auto& objectRef = std::get<move_source_output_port_t>(object);
			std::cerr << "Moving Source Ouput " << objectRef.index << " to source " << objectRef.sourceIndex << '\n';
			op = pa_context_move_source_output_by_index(
					ctx,
					objectRef.index,
					objectRef.sourceIndex,
					move_source_output_port_cb,
					&objectRef);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// move sink input port 
		try {
			auto& objectRef = std::get<move_sink_input_port_t>(object);
			std::cerr << "Moving Sink Input " << objectRef.index << " to sink " << objectRef.sinkIndex << '\n';
			op = pa_context_move_sink_input_by_index(
					ctx,
					objectRef.index,
					objectRef.sinkIndex,
					move_sink_input_port_cb,
					&objectRef);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// load module
		try {
			auto& objectRef = std::get<load_module_t>(object);
			op = pa_context_load_module(
					ctx,
					objectRef.name,
					objectRef.arguments,
					load_module_infos_cb,
					&objectRef);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// unload module
		try {
			auto& objectRef = std::get<unload_module_t>(object);
			op = pa_context_unload_module(
					ctx,
					objectRef.index,
					unload_module_cb,
					&objectRef);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// set source volume
		try {
			auto& objectRef = std::get<set_sink_volume_t>(object);
			double v = objectRef.volume/100.0;
			auto volume = pa_sw_volume_from_linear(v);
			pa_cvolume_set(&(objectRef.cvolume), 2, volume);
			op = pa_context_set_sink_volume_by_index(
					ctx,
					objectRef.index,
					&(objectRef.cvolume),
					set_sink_volume_cb,
					&objectRef);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// set sink input volume
		try {
			auto& objectRef = std::get<set_sink_input_volume_t>(object);
			double v = objectRef.volume/100.0;
			auto volume = pa_sw_volume_from_linear(v);
			pa_cvolume_set(&(objectRef.cvolume), 2, volume);
			op = pa_context_set_sink_input_volume(
					ctx,
					objectRef.index,
					&(objectRef.cvolume),
					set_sink_input_volume_cb,
					&objectRef);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// get modules list 
		try {
			auto& objectRef = std::get<info_list<module_infos_t>>(object);
			op = pa_context_get_module_info_list(
					ctx,
					module_infos_list_cb,
					&objectRef);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// get sinks list 
		try {
			auto& objectRef = std::get<info_list<sink_infos_t>>(object);
			op = pa_context_get_sink_info_list(
					ctx,
					sink_infos_list_cb,
					&objectRef);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// get sink inputs list 
		try {
			auto& objectRef = std::get<info_list<sink_input_infos_t>>(object);
			op = pa_context_get_sink_input_info_list(
					ctx,
					sink_input_infos_list_cb,
					&objectRef);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// get source list 
		try {
			auto& objectRef = std::get<info_list<source_infos_t>>(object);
			op = pa_context_get_source_info_list(
					ctx,
					source_infos_list_cb,
					&objectRef);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {
		}
		// get source output list 
		try {
			auto& objectRef = std::get<info_list<source_output_infos_t>>(object);
			op = pa_context_get_source_output_info_list(
					ctx,
					source_output_infos_list_cb,
					&objectRef);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {
		}
		// get module infos
		try {
			auto& objectRef = std::get<module_infos_t>(object);
			op = pa_context_get_module_info(
					ctx,
					objectRef.index,
					module_infos_cb,
					&objectRef);
			++objectIndex;
			throw std::exception();
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// get sink infos
		try {
			auto& objectRef = std::get<sink_infos_t>(object);
			op = pa_context_get_sink_info_by_index(
					ctx,
					objectRef.index,
					sink_infos_cb,
					&objectRef);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// get sink input infos
		try {
			auto& objectRef = std::get<sink_input_infos_t>(object);
			op = pa_context_get_sink_input_info(
					ctx,
					objectRef.index,
					sink_input_infos_cb,
					&objectRef);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// get source infos
		try {
			auto& objectRef = std::get<source_infos_t>(object);
			op = pa_context_get_source_info_by_index(
					ctx,
					objectRef.index,
					source_infos_cb,
					&objectRef);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// get source output infos
		try {
			auto& objectRef = std::get<source_output_infos_t>(object);
			op = pa_context_get_source_output_info(
					ctx,
					objectRef.index,
					source_output_infos_cb,
					&objectRef);
			++objectIndex;
			goto iterate;
		} catch(const std::bad_variant_access&) {}
		// set subscribe callback
		try {
			auto& objectRef = std::get<set_subscribe_callback_info_t>(object);
			pa_context_set_subscribe_callback(
					ctx,
					subscribe_cb,
					&objectRef);
			++objectIndex;
			// Do not try getting the state of the op
			continue;
		} catch(const std::bad_variant_access&) {}
		// enable subscribtion
		try {
			auto& objectRef = std::get<subscribe_info_t>(object);
			std::cerr << "enabling subscribe with mask mask : " << objectRef.mask << '\n';
			op = pa_context_subscribe(
					ctx,
					objectRef.mask,
					enable_subscribtion_cb,
					&objectRef);
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

void FakeLibImplementation::init_subscribtion(pa_subscription_mask_t mask, user_subscribe_callback_t user_callback) {
	// Define our pulse audio loop and connection variables
	pa_mainloop_api *mlapi;

	// We'll need these state variables to keep track of our requests
	int ready = 0;

	// Create a mainloop API and connection to the default server
	ml = pa_mainloop_new();
	mlapi = pa_mainloop_get_api(ml);
	ctx = pa_context_new(mlapi, "test");
	op = NULL;


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

	callback_info.user_callback = user_callback;
	
	pa_context_set_subscribe_callback(
			ctx,
			subscribe_cb, &callback_info);
	subscribe_info_t info_subscribe = {
		.success = 0,
		.mask = mask
	};
	std::cerr << "enabling subscribe with mask mask : " << mask << '\n';
	op = pa_context_subscribe(
			ctx,
			mask,
			enable_subscribtion_cb,
			&info_subscribe);
	while (pa_operation_get_state(op) != PA_OPERATION_DONE)
		 pa_mainloop_iterate(ml, 1, NULL);
}

void FakeLibImplementation::iterate_subscribtion_loop() {
	pa_mainloop_iterate(ml, 0, NULL);
}
void FakeLibImplementation::clean_subscribtion_loop() {
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
}

