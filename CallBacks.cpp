#include "CallBacks.h"
#include "FakeLib.h"

#include <iostream>
#include <iterator>

void state_cb(pa_context *c, void *userdata) {
	pa_context_state_t state;
	int *ready = static_cast<int*>(userdata);
	state = pa_context_get_state(c);
	switch (state) {
		// There are just here for reference
		case PA_CONTEXT_UNCONNECTED:
		case PA_CONTEXT_CONNECTING:
		case PA_CONTEXT_AUTHORIZING:
		case PA_CONTEXT_SETTING_NAME:
		default:
			break;
		case PA_CONTEXT_FAILED:
		case PA_CONTEXT_TERMINATED:
			*ready = 2;
			break;
		case PA_CONTEXT_READY:
			*ready = 1;
			break;
	}
}

template<typename pa_type, typename user_type>
void get_infos_list_cb(__attribute__((unused)) pa_context *c,
		const pa_type* l,
		int eol,
		void* userdata,
		std::function<void(user_type&, const pa_type*)> implementation
		)
{
	info_list<user_type>* infos = static_cast<info_list<user_type>*>(userdata);
	if (eol > 0) {
		return;
	}
	for (int ctr = 0; ctr < info_list_size; ++ctr) {
		if (!infos->at(ctr).initialized) {
			infos->at(ctr).initialized = true;
			implementation(infos->at(ctr), l);
			break;
		}
	}
}

template<typename pa_type, typename user_type>
void get_infos_cb(__attribute__((unused)) pa_context *c,
		const pa_type* l,
		int eol,
		void* userdata,
		std::function<void(user_type&, const pa_type*)> implementation
		)
{
	user_type* infos = static_cast<user_type*>(userdata);
	if (eol > 0) {
		return;
	}
	implementation(*infos, l);
}

template<typename user_type>
void success_callback(pa_context *c, int success, void *userdata) {
	user_type* user = static_cast<user_type*>(userdata);
	user->success = success;
}

void move_source_output_port_cb(pa_context *c, int success,
		void *userdata) {
	success_callback<success_info_t>(c, success, userdata);
}

void move_sink_input_port_cb(pa_context *c, int success,
		void *userdata) {
	success_callback<success_info_t>(c, success, userdata);
}

void load_module_infos_cb(__attribute__((unused)) pa_context *c, uint32_t index,
		void *userdata) {
	load_module_t *load_module = static_cast<load_module_t*>(userdata);
	load_module->success = 1;
}

void unload_module_cb(pa_context *c, int success,
		void *userdata) {
	success_callback<unload_module_t>(c, success, userdata);
}

void set_sink_volume_cb(pa_context *c, int success,
		void *userdata) {
	success_callback<set_sink_volume_t>(c, success, userdata);
}

void set_sink_input_volume_cb(pa_context *c, int success,
		void *userdata) {
	success_callback<set_sink_input_volume_t>(c, success, userdata);
}

void module_infos_list_cb(pa_context *c, const pa_module_info *l, int eol, void *userdata) {
	get_infos_list_cb<pa_module_info, module_infos_t>(
		c, l, eol, userdata,
		[](module_infos_t& info, const pa_module_info* l){
		info.name = l->name;
		info.index = l->index;
		info.initialized = true;
		}
	);
}

void sink_infos_list_cb(pa_context *c, const pa_sink_info *l, int eol,
		void *userdata) {
	get_infos_list_cb<pa_sink_info, sink_infos_t>(
		c, l, eol, userdata,
		[](sink_infos_t& info, const pa_sink_info* l){
		info.name = l->name;
		info.description = l->description;
		info.index = l->index;
		info.initialized = true;
		}
	);
}

void sink_input_infos_list_cb(pa_context *c, const pa_sink_input_info *l, int eol,
		void *userdata) {
	get_infos_list_cb<pa_sink_input_info, sink_input_infos_t>(
		c, l, eol, userdata,
		[](sink_input_infos_t& info, const pa_sink_input_info* l){
		info.name = l->name;
		info.index = l->index;
		info.owner_module = l->owner_module;
		info.client = l->client;
		info.sink = l->sink;
		auto data = 
		pa_proplist_gets(
				l->proplist,
				PA_PROP_APPLICATION_PROCESS_BINARY);
		if (data == NULL) {
		} else {
		info.process_binary = std::string(data);
		}
		info.initialized = true;
		}
	);
}

void source_infos_list_cb(pa_context *c, const pa_source_info *l, int eol,
		void *userdata) {
	get_infos_list_cb<pa_source_info, source_infos_t>(
		c, l, eol, userdata,
		[](source_infos_t& info, const pa_source_info* l){
		info.name = l->name;
		info.description = l->description;
		info.index = l->index;
		info.initialized = true;
		}
	);
}

void source_output_infos_list_cb(pa_context *c, const pa_source_output_info *l,
		int eol, void *userdata) {
	get_infos_list_cb<pa_source_output_info, source_output_infos_t>(
		c, l, eol, userdata,
		[](source_output_infos_t& info, const pa_source_output_info* l){
		info.initialized = true;
		info.name = l->name;
		info.source = l->source;
		info.index = l->index;
		auto data = 
		pa_proplist_gets(
				l->proplist,
				PA_PROP_APPLICATION_PROCESS_BINARY);
		if (data == NULL) {
		} else {
		info.process_binary = std::string(data);
		}
		}
	);
}

void unload_module_infos_cb(pa_context *c, int success, void *userdata) {
	success_callback<unload_module_t>(c, success, userdata);
}


void module_infos_cb(pa_context *c, const pa_module_info *l, int eol, void *userdata) {
	get_infos_cb<pa_module_info, module_infos_t>(
		c, l, eol, userdata,
		[](module_infos_t& info, const pa_module_info* l){
		info.name = l->name;
		info.index = l->index;
		info.initialized = true;
		}
	);
}

void sink_infos_cb(pa_context *c, const pa_sink_info *l, int eol,
		void *userdata) {
	get_infos_cb<pa_sink_info, sink_infos_t>(
		c, l, eol, userdata,
		[](sink_infos_t& info, const pa_sink_info* l){
		info.name = l->name;
		info.description = l->description;
		info.index = l->index;
		info.initialized = true;
		}
	);
}

void sink_input_infos_cb(pa_context *c, const pa_sink_input_info *l, int eol,
		void *userdata) {
	get_infos_cb<pa_sink_input_info, sink_input_infos_t>(
		c, l, eol, userdata,
		[](sink_input_infos_t& info, const pa_sink_input_info* l){
		info.name = l->name;
		info.index = l->index;
		info.owner_module = l->owner_module;
		info.client = l->client;
		info.sink = l->sink;
		auto data = 
		pa_proplist_gets(
				l->proplist,
				PA_PROP_APPLICATION_PROCESS_BINARY);
		if (data == NULL) {
		std::cerr << "Source Output " << info.name << " doesn't have a process binary property\n";
		} else {
		info.process_binary = std::string(data);
		}
		info.initialized = true;
		}
	);
}

void source_infos_cb(pa_context *c, const pa_source_info *l, int eol,
		void *userdata) {
	get_infos_cb<pa_source_info, source_infos_t>(
		c, l, eol, userdata,
		[](source_infos_t& info, const pa_source_info* l){
		info.name = l->name;
		info.description = l->description;
		info.index = l->index;
		info.initialized = true;
		}
	);
}

void source_output_infos_cb(pa_context *c, const pa_source_output_info *l,
		int eol, void *userdata) {
	get_infos_cb<pa_source_output_info, source_output_infos_t>(
		c, l, eol, userdata,
		[](source_output_infos_t& info, const pa_source_output_info* l){
		info.initialized = true;
		info.name = l->name;
		info.source = l->source;
		info.index = l->index;
		auto data = 
		pa_proplist_gets(
				l->proplist,
				PA_PROP_APPLICATION_PROCESS_BINARY);
		if (data == NULL) {
		std::cerr << "Source Output " << info.name << " doesn't have a process binary property\n";
		} else {
		info.process_binary = std::string(data);
		}
		}
	);
}

void subscribe_cb(pa_context *c, pa_subscription_event_type_t t, uint32_t idx, void *userdata) {
	/* switch (t) { */
	/* case PA_SUBSCRIPTION_EVENT_SINK: { */
	/* 	std::cerr << "event is PA_SUBSCRIPTION_EVENT_SINK\n"; */
	/* 	break; */
	/* } */

	/* case PA_SUBSCRIPTION_EVENT_SOURCE: { */
	/* 	std::cerr << "event is PA_SUBSCRIPTION_EVENT_SOURCE\n"; */
	/* 	break; */
	/* } */

	/* case PA_SUBSCRIPTION_EVENT_SINK_INPUT: { */
	/* 	std::cerr << "event is PA_SUBSCRIPTION_EVENT_SINK_INPUT\n"; */
	/* 	break; */
	/* } */

	/* case PA_SUBSCRIPTION_EVENT_SOURCE_OUTPUT: { */
	/* 	std::cerr << "event is PA_SUBSCRIPTION_EVENT_SOURCE_OUTPUT\n"; */
	/* 	break; */
	/* } */

	/* case PA_SUBSCRIPTION_EVENT_MODULE: { */
	/* 	std::cerr << "event is PA_SUBSCRIPTION_EVENT_MODULE\n"; */
	/* 	break; */
	/* } */

	/* case PA_SUBSCRIPTION_EVENT_CLIENT: { */
	/* 	std::cerr << "event is PA_SUBSCRIPTION_EVENT_CLIENT\n"; */
	/* 	break; */
	/* } */

	/* case PA_SUBSCRIPTION_EVENT_SAMPLE_CACHE: { */
	/* 	std::cerr << "event is PA_SUBSCRIPTION_EVENT_SAMPLE_CACHE\n"; */
	/* 	break; */
	/* } */

	/* case PA_SUBSCRIPTION_EVENT_SERVER: { */
	/* 	std::cerr << "event is PA_SUBSCRIPTION_EVENT_SERVER\n"; */
	/* 	break; */
	/* } */

	/* case PA_SUBSCRIPTION_EVENT_CARD: { */
	/* 	std::cerr << "event is PA_SUBSCRIPTION_EVENT_CARD\n"; */
	/* 	break; */
	/* } */

	/* case PA_SUBSCRIPTION_EVENT_FACILITY_MASK: { */
	/* 	std::cerr << "event is PA_SUBSCRIPTION_EVENT_FACILITY_MASK\n"; */
	/* 	break; */
	/* } */

	/* case PA_SUBSCRIPTION_EVENT_CHANGE: { */
	/* 	std::cerr << "event is PA_SUBSCRIPTION_EVENT_CHANGE\n"; */
	/* 	break; */
	/* } */

	/* case PA_SUBSCRIPTION_EVENT_REMOVE: { */
	/* 	std::cerr << "event is PA_SUBSCRIPTION_EVENT_REMOVE\n"; */
	/* 	break; */
	/* } */

	/* case PA_SUBSCRIPTION_EVENT_TYPE_MASK: { */
	/* 	std::cerr << "event is PA_SUBSCRIPTION_EVENT_TYPE_MASK\n"; */
	/* 	break; */
	/* } */
	/* case PA_SUBSCRIPTION_EVENT_AUTOLOAD: { */
	/* 	std::cerr << "event is PA_SUBSCRIPTION_EVENT_AUTOLOAD\n"; */
	/* 	break; */
	/* } */
	/* } */
	auto data = static_cast<set_subscribe_callback_info_t*>(userdata);
	data->user_callback(t);
}

void enable_subscribtion_cb(pa_context *c, int success,
		void *userdata) {
	success_callback<success_info_t>(c, success, userdata);
}
