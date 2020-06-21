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

FakeLib FakeLib::move_source_output_port(uint32_t portIndex, uint32_t sourceIndex) {
	move_source_output_port_t infos {
		.success = 0,
		.portIndex = portIndex,
		.sourceIndex = sourceIndex
	};
	commandObjects.push_back(infos);
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
	commandObjects.push_back(infos);
	return *this;
}
FakeLib FakeLib::unload_module(uint32_t index) {
	unload_module_t infos = {
		.success = 0,
		.index = index
	};
	commandObjects.push_back(infos);
	return *this;
}
FakeLib FakeLib::set_sink_volume(uint32_t index, double volume) {
	set_sink_volume_t infos = {
		.success = 0,
		.index = index,
		.volume = volume
	};
	commandObjects.push_back(infos);
	return *this;
}
FakeLib FakeLib::get_module_list() {
	info_list<module_infos_t> infos;
	FakeLibUtils::reset_info_list(infos);
	commandObjects.push_back(infos);
	return *this;
}
FakeLib FakeLib::get_sink_list() {
	info_list<sink_infos_t> infos;
	FakeLibUtils::reset_info_list(infos);
	commandObjects.push_back(infos);
	return *this;
}
FakeLib FakeLib::get_source_list() {
	info_list<source_infos_t> infos;
	FakeLibUtils::reset_info_list(infos);
	commandObjects.push_back(infos);
	return *this;
}
FakeLib FakeLib::get_source_output_list() {
	info_list<source_output_infos_t> infos;
	FakeLibUtils::reset_info_list(infos);
	commandObjects.push_back(infos);
	return *this;
}
FakeLib FakeLib::get_module(uint32_t index) {
	module_infos_t infos = {
		.initialized = false,
		.index  = index
	};
	commandObjects.push_back(infos);
	return *this;
}
FakeLib FakeLib::get_sink(uint32_t index) {
	sink_infos_t infos = {
		.initialized = false,
		.index  = index
	};
	commandObjects.push_back(infos);
	return *this;
}
FakeLib FakeLib::get_source(uint32_t index) {
	source_infos_t infos = {
		.initialized = false,
		.index  = index
	};
	commandObjects.push_back(infos);
	return *this;
}
FakeLib FakeLib::get_source_output(uint32_t index) {
	source_output_infos_t infos = {
		.initialized = false,
		.index  = index
	};
	commandObjects.push_back(infos);
	return *this;
}
std::vector<ObjectVariant> FakeLib::run_commands() {
	return FakeLibImplementation::run_pa_commands(commandObjects);
}
FakeLib FakeLib::clear_commands() {
	commandObjects.clear();
	return *this;
}
