#pragma once

#include <stdint.h>
#include <cstdlib>
#include <ostream>

namespace il2cpp
{
namespace debugger
{

struct CommandLineOptions
{
	char* host;
	int32_t port;
	bool print_help;
};

static const char* debugger_host_option = "--debugger-host";
static const char* debugger_port_option = "--debugger-port";
static const char* debugger_help_option = "--help";

class CommandLineParser
{
public:
	static bool Parse(int argc, char* argv[], CommandLineOptions* options);
	static void PrintUsage(std::ostream& out);
};

} /* namespace debugger */
} /* namespace il2cpp */
