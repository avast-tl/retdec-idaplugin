/**
 * @file idaplugin/defs.cpp
 * @brief Plugin-global definitions and includes.
 * @copyright (c) 2017 Avast Software, licensed under the MIT license
 */

#include "defs.h"
#include "plugin_config.h"

namespace idaplugin {

RdGlobalInfo::RdGlobalInfo() :
		pluginConfigFile(get_user_idadir())
{
	pluginInfo.id = pluginID.data();
	pluginInfo.name = pluginName.data();
	pluginInfo.producer = pluginProducer.data();
	pluginInfo.version = pluginVersion.data();
	pluginInfo.url = pluginContact.data();
	pluginInfo.freeform = pluginCopyright.data();

	navigationActual = navigationList.end();

	pluginConfigFile.append(pluginConfigFileName);
}

bool RdGlobalInfo::isAllDecompilation()
{
	return !outputFile.empty();
}

bool RdGlobalInfo::isSelectiveDecompilation()
{
	return !isAllDecompilation();
}

bool RdGlobalInfo::isDecompilerInSpecifiedPath() const
{
	std::string cmd = "python '" + decompilerPyPath + "'" + " --help";
	return std::system(cmd.c_str()) == 0;
}

bool RdGlobalInfo::isUseThreads() const
{
	return useThreads;
}

void RdGlobalInfo::setIsUseThreads(bool f)
{
	useThreads = f;
}

/**
 * @return @c True if canceled, @c false otherwise.
 */
bool RdGlobalInfo::configureDecompilation()
{
	if (isDecompilerInSpecifiedPath())
	{
		INFO_MSG("Found retdec-decompiler.py at %s -> plugin is properly configured.\n",
				decompilerPyPath.c_str());
		decompilationCmd = decompilerPyPath;
		return false;
	}
	else
	{
		warning("Decompilation is not properly configured.\n"
				"The path to retdec-decompiler.py must be provided in the configuration menu.");
		auto canceled = pluginConfigurationMenu(*this);
		if (canceled)
		{
			return canceled;
		}
		else
		{
			return configureDecompilation();
		}
	}
}

} // namespace idaplugin
