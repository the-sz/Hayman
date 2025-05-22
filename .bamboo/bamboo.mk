#!make

export SHELL=cmd

PYTHON_3=py.exe -3
ifdef bamboo_capability_system_builder_command_Python_3_8
	PYTHON_3="$(bamboo_capability_system_builder_command_Python_3_8)"
endif

#
# checkout additional repositories
#
checkout:
	$(PYTHON_3) ..\BambooUtilityBuildScripts\GIT\checkout.py "*defaults*"
	$(PYTHON_3) ..\BambooUtilityBuildScripts\GIT\checkout.py "*defaults_app*"
	$(PYTHON_3) ..\BambooUtilityBuildScripts\GIT\checkout.py "*defaults_kyle*"
	$(PYTHON_3) ..\BambooUtilityBuildScripts\GIT\checkout.py "modbus"

#
# build project
#
build:
	$(PYTHON_3) ..\BambooUtilityBuildScripts\VC6\prepare.py ".\\Hayman.dsw"

	call ..\BambooUtilityBuildScripts\VC6\build.bat ".\\Hayman.dsw" "Hayman - Win32 Debug Unicode"
	call ..\BambooUtilityBuildScripts\VC6\build.bat ".\\Hayman.dsw" "Hayman - Win32 Release Unicode"

	$(PYTHON_3) ..\BambooUtilityBuildScripts\VC6\log_to_junit.py ".\Log.log"
