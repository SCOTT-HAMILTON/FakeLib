#ifndef FAKE_LIB_H
#define FAKE_LIB_H

#include <pulse/pulseaudio.h>
#include <pulse/operation.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <string>
#include <array>
#include <exception>
#include <vector>
#include <variant>

// Info list size
// The bigger this value gets
// the more stack the library will use
static const int info_list_size = 50;

// Field list is here:
// http://0pointer.de/lennart/projects/pulseaudio/doxygen/structpa__sink__info.html

struct ObjectNotFoundError : public std::exception
{
	const char* what () const throw ()
	{ 
		return "Failed to found object in results";
	}
};

typedef struct move_source_output_port {
	int success;
	uint32_t portIndex;
	uint32_t sourceIndex;
} move_source_output_port_t;

typedef struct load_module {
	int success;
	std::string name;
	std::string arguments;
	/* arbitrary string to know what this module was loaded for */
	std::string description;
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
	bool initialized;
	std::string name;
	uint32_t index;
} module_infos_t;

typedef struct sink_infos {
	bool initialized;
	std::string name;
	uint32_t index;
	std::string description;
} sink_infos_t;

typedef struct sink_input_infos {
	bool initialized;
	std::string name;
	uint32_t index;
	uint32_t owner_module;
	uint32_t client;
	uint32_t sink;
} sink_input_infos_t;

typedef struct source_infos {
	bool initialized;
	std::string name;
	uint32_t index;
	std::string description;
} source_infos_t;

typedef struct source_output_infos {
	bool initialized;
	std::string name;
	uint32_t index;
	uint32_t source;
	std::string source_process_binary;
} source_output_infos_t;

typedef struct success_info {
	int success;
} success_info_t;

template<typename info_type>
using info_list = std::array<info_type, info_list_size>;

int move_source_output_port(uint32_t sourceIndex, uint32_t portIndex);
int load_module(load_module_t *load_module_infos);
int unload_module(load_module_t *load_module_infos);

using ObjectVariant = std::variant<
	move_source_output_port_t,
	load_module_t,
	unload_module_t,
	set_sink_volume_t,
	set_sink_input_volume_t,
	info_list<module_infos_t>,
	info_list<sink_infos_t>,
	info_list<sink_input_infos_t>,
	info_list<source_infos_t>,
	info_list<source_output_infos_t>,
	module_infos_t,
	sink_infos_t,
	sink_input_infos_t,
	source_infos_t,
	source_output_infos_t
>;

class FakeLib
{
public:
	FakeLib move_source_output_port(uint32_t portIndex, uint32_t sourceIndex);
	FakeLib load_module(const std::string& name, 
			 const std::string& arguments,
			 const std::string& description = "");
	FakeLib unload_module(uint32_t index);
	FakeLib set_sink_volume(uint32_t index, double volume); // volume in percentage
	FakeLib set_sink_input_volume(uint32_t index, double volume); // volume in percentage
	FakeLib get_module_list();
	FakeLib get_sink_list();
	FakeLib get_sink_input_list();
	FakeLib get_source_list();
	FakeLib get_source_output_list();
	FakeLib get_module(uint32_t index);
	FakeLib get_sink(uint32_t index);
	FakeLib get_sink_input(uint32_t index);
	FakeLib get_source(uint32_t index);
	FakeLib get_source_output(uint32_t index);
	std::vector<ObjectVariant> run_commands();
	FakeLib clear_commands();

private:
	std::vector<ObjectVariant> commandObjects;

};

#endif // FAKE_LIB_H
