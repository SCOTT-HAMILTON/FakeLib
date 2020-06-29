#ifndef RUN_COMMAND_H
#define RUN_COMMAND_H

#include "FakeLib.h"

#include <variant>
#include <pulse/pulseaudio.h>

#include <exception>
#include <vector>

namespace FakeLibImplementation
{

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



std::vector<ObjectVariant> run_pa_commands(std::vector<ObjectVariant>& objects);
}

#endif //RUN_COMMAND_H
