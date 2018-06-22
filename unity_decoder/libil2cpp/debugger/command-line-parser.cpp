#include <cstring>
#include <cstring>
#include <stdlib.h>
#include <string.h>
#include "command-line-parser.h"

using namespace il2cpp::debugger;

bool CommandLineParser::Parse(int argc, char* argv[], CommandLineOptions* options)
{
    options->print_help = false;

    bool hostFound = false;
    bool portFound = false;

    int i = 1;
    while (i < argc)
    {
        if (strcmp(argv[i], debugger_host_option) == 0)
        {
            options->host = argv[i + 1];
            hostFound = true;
            i += 2;
        }
        else if (strcmp(argv[i], debugger_port_option) == 0)
        {
            options->port = atoi(argv[i + 1]);
            portFound = true;
            i += 2;
        }
        else if (strcmp(argv[i], debugger_help_option) == 0)
        {
            options->print_help = true;
            i += 1;
        }
        else
        {
            i += 1;
        }
    }

    return !options->print_help && hostFound && portFound;
}

void CommandLineParser::PrintUsage(std::ostream& out)
{
    out << "Usage: --debugger-host <host> (optional) --debugger-port <port> (optional) --help (optional)";
}
