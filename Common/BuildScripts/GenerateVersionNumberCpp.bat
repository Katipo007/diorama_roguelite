rem parameter 1 = full project dir
rem parameter 2 = full output dir

set output_file=%1\Common\Version\BuildVersion.cpp
for /f %%i in ('git describe --tags --long --dirty --always') do set VER=%%i

echo //> %output_file%
echo // THIS FILE IS GENERATED DURING THE PRE-BUILD STEP>> %output_file%
echo //>> %output_file%
echo #include "BuildVersion.hpp"%NEWLINE%>> %output_file%
echo.>> %output_file%
echo namespace BuildVersion>> %output_file%
echo {>> %output_file%
echo     const std::string_view FullVersionString = "%VER%";>> %output_file%
echo }>> %output_file%
