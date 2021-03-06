#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <pulse/pulseaudio.h>

#include <functional>

void state_cb(pa_context *c, void *userdata);

template<typename pa_type, typename user_type>
void get_infos_list_cb(__attribute__((unused)) pa_context *c,
		const pa_type* l,
		int eol,
		void* userdata,
		std::function<void(user_type&, const pa_type*)> implementation
		);

template<typename pa_type, typename user_type>
void get_infos_cb(__attribute__((unused)) pa_context *c,
		const pa_type* l,
		int eol,
		void* userdata,
		std::function<void(user_type&, const pa_type*)> implementation
		);

template<typename user_type>
void success_callback(pa_context *c, int success, void *userdata);

// Actions
void move_source_output_port_cb(pa_context *c, int success,
				     void *userdata);
void move_sink_input_port_cb(pa_context *c, int success,
				     void *userdata);
void load_module_infos_cb(pa_context *c, uint32_t index, void *userdata);
void unload_module_cb(pa_context *c, int success,
				     void *userdata);
void set_sink_volume_cb(pa_context *c, int success, void *userdata);
void set_sink_input_volume_cb(pa_context *c, int success, void *userdata);


// Fetching list of modules, sinks, sink inputs, sources and source outputs
void module_infos_list_cb(pa_context *c, const pa_module_info *l, int eol,
		     void *userdata);
void sink_infos_list_cb(pa_context *c, const pa_sink_info *l, int eol,
		   void *userdata);
void sink_input_infos_list_cb(pa_context *c, const pa_sink_input_info *l, int eol,
		   void *userdata);
void source_infos_list_cb(pa_context *c, const pa_source_info *l, int eol,
		     void *userdata);
void source_output_infos_list_cb(pa_context *c, const pa_source_output_info *l,
			    int eol, void *userdata);

// Fetching modules, sinks, sources and source outputs
void module_infos_cb(pa_context *c, const pa_module_info *l, int eol,
		     void *userdata);
void sink_infos_cb(pa_context *c, const pa_sink_info *l, int eol,
		   void *userdata);
void sink_input_infos_cb(pa_context *c, const pa_sink_input_info *l, int eol,
		   void *userdata);
void source_infos_cb(pa_context *c, const pa_source_info *l, int eol,
		     void *userdata);
void source_output_infos_cb(pa_context *c, const pa_source_output_info *l,
			    int eol, void *userdata);

// Subscribe
void subscribe_cb(pa_context *c, pa_subscription_event_type_t t, uint32_t idx, void *userdata);
void enable_subscribtion_cb(pa_context *c, int success, void *userdata);

#endif //CALLBACKS_H
