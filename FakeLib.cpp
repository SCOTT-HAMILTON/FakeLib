#include "FakeLib.h"
#include "CallBacks.h"
#include "FakeLibUtils.hpp"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <utility>
#include <variant>
#include <vector>

FakeLib FakeLib::move_source_output_port(uint32_t index, uint32_t sourceIndex) {
	move_source_output_port_t infos {
		.success = 0,
		.index = index,
		.sourceIndex = sourceIndex
	};
	commandObjects.emplace_back(infos);
	return *this;
}
FakeLib FakeLib::move_sink_input_port(uint32_t index, uint32_t sinkIndex) {
	move_sink_input_port_t infos {
		.success = 0,
		.index = index,
		.sinkIndex = sinkIndex
	};
	commandObjects.emplace_back(infos);
	return *this;
}
FakeLib FakeLib::load_module(const std::string& name, 
		 const std::string& arguments,
		 const std::string& description){
	load_module_t infos = {
		.success = 0,
		.name = name,
		.arguments = arguments,
		.description = description
	};
	commandObjects.emplace_back(infos);
	return *this;
}
FakeLib FakeLib::unload_module(uint32_t index) {
	unload_module_t infos = {
		.success = 0,
		.index = index
	};
	commandObjects.emplace_back(infos);
	return *this;
}
FakeLib FakeLib::set_sink_volume(uint32_t index, double volume) {
	set_sink_volume_t infos = {
		.success = 0,
		.index = index,
		.volume = volume
	};
	commandObjects.emplace_back(infos);
	return *this;
}
FakeLib FakeLib::set_sink_input_volume(uint32_t index, double volume) {
	set_sink_input_volume_t infos = {
		.success = 0,
		.index = index,
		.volume = volume
	};
	commandObjects.emplace_back(infos);
	return *this;
}
FakeLib FakeLib::get_module_list() {
	info_list<module_infos_t> infos;
	FakeLibUtils::reset_info_list(infos);
	commandObjects.emplace_back(infos);
	return *this;
}
FakeLib FakeLib::get_sink_list() {
	info_list<sink_infos_t> infos;
	FakeLibUtils::reset_info_list(infos);
	commandObjects.emplace_back(infos);
	return *this;
}
FakeLib FakeLib::get_sink_input_list() {
	info_list<sink_input_infos_t> infos;
	FakeLibUtils::reset_info_list(infos);
	commandObjects.emplace_back(infos);
	return *this;
}
FakeLib FakeLib::get_source_list() {
	info_list<source_infos_t> infos;
	FakeLibUtils::reset_info_list(infos);
	commandObjects.emplace_back(infos);
	return *this;
}
FakeLib FakeLib::get_source_output_list() {
	info_list<source_output_infos_t> infos;
	FakeLibUtils::reset_info_list(infos);
	commandObjects.emplace_back(infos);
	return *this;
}
FakeLib FakeLib::get_module(uint32_t index) {
	module_infos_t infos = {
		.initialized = false,
		.index  = index
	};
	commandObjects.emplace_back(infos);
	return *this;
}
FakeLib FakeLib::get_sink(uint32_t index) {
	sink_infos_t infos = {
		.initialized = false,
		.index  = index
	};
	commandObjects.emplace_back(infos);
	return *this;
}
FakeLib FakeLib::get_sink_input(uint32_t index) {
	sink_input_infos_t infos = {
		.initialized = false,
		.index  = index
	};
	commandObjects.emplace_back(infos);
	return *this;
}
FakeLib FakeLib::get_source(uint32_t index) {
	source_infos_t infos = {
		.initialized = false,
		.index  = index
	};
	commandObjects.emplace_back(infos);
	return *this;
}
FakeLib FakeLib::get_source_output(uint32_t index) {
	source_output_infos_t infos = {
		.initialized = false,
		.index  = index
	};
	commandObjects.emplace_back(infos);
	return *this;
}
FakeLib FakeLib::enable_subscription(pa_subscription_mask_t mask) {
	{
		set_subscribe_callback_info_t info = {};
		commandObjects.push_back(info);
	}
	{
		subscribe_info_t info = {
			.mask = mask
		};
		commandObjects.push_back(info);
	}
	return *this;
}
std::vector<ObjectVariant> FakeLib::run_commands() {
	return FakeLibImplementation::run_pa_commands(commandObjects);
}
FakeLib FakeLib::clear_commands() {
	commandObjects.clear();
	return *this;
}
void FakeLib::init_subscribtion(pa_subscription_mask_t mask, user_subscribe_callback_t user_callback) {
	user_callback(PA_SUBSCRIPTION_EVENT_SINK);
	FakeLibImplementation::init_subscribtion(mask, user_callback);
}
void FakeLib::iterate_subscribtion_loop() {
	FakeLibImplementation::iterate_subscribtion_loop();
}
void FakeLib::clean_subscribtion_loop() {
	FakeLibImplementation::clean_subscribtion_loop();
}
