#include "il2cpp-config.h"
#include "vm/Environment.h"

namespace il2cpp
{
namespace vm
{
	int Environment::num_main_args = 0;
	char** Environment::main_args = NULL;

	void Environment::SetMainArgs(char** args, int num_args)
	{
		main_args = args;
		num_main_args = num_args;
	}

	char** Environment::GetMainArgs()
	{
		return main_args;
	}

	int Environment::GetNumMainArgs()
	{
		return num_main_args;
	}

	
} /* namespace vm */
} /* namespace il2cpp */