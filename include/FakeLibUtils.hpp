#ifndef FAKE_LIB_UTILS_H
#define FAKE_LIB_UTILS_H

#include "FakeLibImplementation.h"

#include <functional>
#include <iostream>
#include <vector>
#include <variant>
#include <exception>

namespace FakeLibUtils
{

template<typename user_type>
static void __attribute__((unused)) print_list(info_list<user_type> list,
			std::function<void(const user_type&, int)> print){
	for (int ctr = 0; ctr < info_list_size; ++ctr) {
		if (list[ctr].initialized)
			print(list[ctr], ctr);
	}
}

static void __attribute__((unused)) print_module_list(info_list<module_infos_t> list){
	print_list<module_infos_t>(
		list,
		[](const auto& info, int ctr) {
			std::cerr << "=======[ Module info #" << ctr + 1 << " ]=======\n";
			std::cerr << "Name: " << 
					info.name << '\n';
			std::cerr << "Index: " << 
					info.index << '\n';
			std::cerr << '\n';
		}
	);
}

static void __attribute__((unused)) print_sink_list(info_list<sink_infos_t> list){
	print_list<sink_infos_t>(
		list,
		[](const auto& info, int ctr) {
			std::cerr << "=======[ Sink info #" << ctr + 1 << " ]=======\n";
			std::cerr << "Name: " << 
					info.name << '\n';
			std::cerr << "Index: " << 
					info.index << '\n';
			std::cerr << "Description: " << 
					info.description << '\n';
			std::cerr << '\n';
		}
	);
}

static void __attribute__((unused)) print_sink_input_list(info_list<sink_input_infos_t> list){
	print_list<sink_input_infos_t>(
		list,
		[](const auto& info, int ctr) {
			std::cerr << "=======[ Sink Input info #" << ctr + 1 << " ]=======\n";
			std::cerr << "Name: " << 
					info.name << '\n';
			std::cerr << "Index: " << 
					info.index << '\n';
			std::cerr << "Owner Module: " << 
					info.owner_module << '\n';
			std::cerr << "Client: " << 
					info.client << '\n';
			std::cerr << "Sink: " << 
					info.sink << '\n';
			std::cerr << "Process Binary: " << 
					info.process_binary << '\n';
			std::cerr << '\n';
		}
	);
}

static void __attribute__((unused)) print_source_list(info_list<source_infos_t> list){
	print_list<source_infos_t>(
		list,
		[](const auto& info, int ctr) {
			std::cerr << "=======[ Source info #" << ctr + 1 << " ]=======\n";
			std::cerr << "Name: " << 
					info.name << '\n';
			std::cerr << "Index: " << 
					info.index << '\n';
			std::cerr << "Description: " << 
					info.description << '\n';
			std::cerr << '\n';
		}
	);
}

static void __attribute__((unused)) print_source_output_list(info_list<source_output_infos_t> list){
	print_list<source_output_infos_t>(
		list,
		[](const auto& info, int ctr) {
			std::cerr << "=======[ Source Output info #" << ctr + 1 << " ]=======\n";
			std::cerr << "Name: " << 
					info.name << '\n';
			std::cerr << "Source: " << 
					info.source << '\n';
			std::cerr << "Index: " << 
					info.index << '\n';
			std::cerr << "Process Binary: " << 
					info.process_binary << '\n';
			std::cerr << '\n';
		}
	);
}

template<typename info_type>
static void __attribute__((unused)) reset_info_list(info_list<info_type>& list){
	for (auto& object : list) {
		object.initialized = false;
	}
}
template<typename user_type>
static auto extract(std::vector<ObjectVariant>& results) {
	for (auto& result : results) {
		try {
			return std::get<user_type>(result);
		} catch (const std::bad_variant_access&) {
		}
	}
	throw ObjectNotFoundError();
}

// Finds a source output in a source output list by its source binary name
template<typename user_type>
static auto __attribute__((unused)) find_by_process_binary(const info_list<user_type>& list,
			const std::string& processBinary) {
	for (const auto& info : list) {
		if (info.process_binary == processBinary)
			return info;
	}
	throw ObjectNotFoundError();
}

template <typename user_type>
static auto __attribute__((unused)) find_by_name(const info_list<user_type>& list,
				  const std::string& name)
{
	for (const auto& info : list) {
		if (info.name == name)
			return info;
	}
	throw ObjectNotFoundError();
}
template <typename user_type>
static auto __attribute__((unused)) find_by_index(const info_list<user_type>& list,
				   uint32_t index)
{
	for (const auto& info : list) {
		if (info.index == index)
			return info;
	}
	throw ObjectNotFoundError();
}

}

#endif //FAKE_LIB_UTILS_H
