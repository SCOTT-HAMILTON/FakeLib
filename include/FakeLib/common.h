#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <pulse/volume.h>
#include <pulse/subscribe.h>

// declarations that are C compatible

// Info list size
// The bigger this value gets
// the more stack the library will use
static const int info_list_size = 50;

typedef struct move_source_output_port {
	int success;
	uint32_t index;
	uint32_t sourceIndex;
} move_source_output_port_t;

typedef struct move_sink_input_port {
	int success;
	uint32_t index;
	uint32_t sinkIndex;
} move_sink_input_port_t;

typedef struct load_module {
	int success;
	char name[256];
	char arguments[256];
	/* arbitrary string to know what this module was loaded for */
	char description[256];
} load_module_t;

typedef struct unload_module {
	int success;
	uint32_t index;
} unload_module_t;

typedef struct set_sink_volume {
	int success;
	uint32_t index;
	double volume;
	pa_cvolume cvolume;
} set_sink_volume_t;

typedef struct set_sink_input_volume {
	int success;
	uint32_t index;
	double volume;
	pa_cvolume cvolume;
} set_sink_input_volume_t;

typedef struct module_infos {
	int initialized;
	char name[256];
	uint32_t index;
} module_infos_t;

typedef struct sink_infos {
	int initialized;
	char name[256];
	uint32_t index;
	char description[256];
} sink_infos_t;

typedef struct sink_input_infos {
	int initialized;
	char name[256];
	uint32_t index;
	uint32_t owner_module;
	uint32_t client;
	uint32_t sink;
	char process_binary[256];
} sink_input_infos_t;

typedef struct source_infos {
	int initialized;
	char name[256];
	uint32_t index;
	char description[256];
} source_infos_t;

typedef struct source_output_infos {
	int initialized;
	char name[256];
	uint32_t index;
	uint32_t source;
	char process_binary[256];
} source_output_infos_t;

typedef struct success_info {
	int success;
} success_info_t;

typedef void(*user_subscribe_callback_t)(pa_subscription_event_type_t event);

typedef struct set_subscribe_callback_info {
	user_subscribe_callback_t user_callback;
} set_subscribe_callback_info_t;

typedef struct subscribe_info {
	int success;
	pa_subscription_mask_t mask;
} subscribe_info_t;

/* int move_source_output_port(uint32_t sourceIndex, uint32_t index); */
/* int move_sink_input_port(uint32_t sinkIndex, uint32_t index); */
int load_module(load_module_t *load_module_infos);
int unload_module(load_module_t *load_module_infos);

#endif //COMMON_H
