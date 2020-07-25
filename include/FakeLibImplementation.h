#ifndef RUN_COMMAND_H
#define RUN_COMMAND_H

#include "FakeLib.h"

#include <variant>
#include <pulse/pulseaudio.h>

#include <exception>
#include <vector>
#include <atomic>

struct ConnectionToServerFailure : public std::exception
{
	const char * what () const noexcept override
	{ 
		return "Failed to connect to pulse audio server.";
	}
};

struct UnknownObjectError : public std::exception
{
	const char * what () const noexcept override
	{ 
		return "Object command is unknown.";
	}
};

constexpr pa_context_flags_t F_0 = static_cast<pa_context_flags_t>(0);
constexpr pa_context_flags_t F_1 = static_cast<pa_context_flags_t>(1);

class FakeLibImplementation
{
public:
	static std::vector<ObjectVariant> run_pa_commands(std::vector<ObjectVariant>& objects);
	static void init_subscribtion(pa_subscription_mask_t mask, user_subscribe_callback_t callback);
	static void iterate_subscribtion_loop();
	static void clean_subscribtion_loop();
	static pa_mainloop *ml;
	static pa_context *ctx;
	static pa_operation *op;
	static set_subscribe_callback_info_t callback_info;
};

#endif //RUN_COMMAND_H
