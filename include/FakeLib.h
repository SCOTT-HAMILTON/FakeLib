#ifndef FAKE_LIB_H
#define FAKE_LIB_H

#include "FakeLib/common.h"

#include <pulse/operation.h>
#include <pulse/pulseaudio.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <array>
#include <exception>
#include <functional>
#include <string>
#include <variant>
#include <vector>

struct ObjectNotFoundError : public std::exception
{
	const char* what () const throw ()
	{ 
		return "Failed to found object in results";
	}
};

template<typename info_type>
using info_list = std::array<info_type, info_list_size>;

/* int move_source_output_port(uint32_t sourceIndex, uint32_t index); */
/* int move_sink_input_port(uint32_t sinkIndex, uint32_t index); */
int load_module(load_module_t *load_module_infos);
int unload_module(load_module_t *load_module_infos);

using ObjectVariant = std::variant<
	move_source_output_port_t,
	move_sink_input_port_t,
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
	source_output_infos_t,
	set_subscribe_callback_info_t,
	subscribe_info_t
>;

class FakeLib
{
public:
	FakeLib move_source_output_port(uint32_t index, uint32_t sourceIndex);
	FakeLib move_sink_input_port(uint32_t index, uint32_t sinkIndex);
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
	FakeLib enable_subscription(pa_subscription_mask_t mask);
	std::vector<ObjectVariant> run_commands();
	FakeLib clear_commands();
	void init_subscribtion(pa_subscription_mask_t mask, user_subscribe_callback_t callback);
	void iterate_subscribtion_loop();
	void clean_subscribtion_loop();

private:
	std::vector<ObjectVariant> commandObjects;

};

#endif // FAKE_LIB_H
