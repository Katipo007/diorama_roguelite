rem parameter 1 = full project dir
rem parameter 2 = full output dir

echo ##########################
echo Executing Prebuild Script
echo ##########################

call %1Common\BuildScripts\GenerateVersionNumberCpp.bat %1 %2

echo ###### End Prebuild ######