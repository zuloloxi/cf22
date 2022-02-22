ml /coff /c  /nologo /Fm /FR /Zi /Zd /Fl color.asm
if errorlevel 1 goto end
rc /v resource.rc
if errorlevel 1 goto end
cvtres /machine:ix86 resource.res 

link /SUBSYSTEM:WINDOWS /DYNAMICBASE:NO /FIXED /NXCOMPAT /MACHINE:X86 /opt:ref /STACK:2097152 /debug /entry:_start  /MAP /MAPINFO:EXPORTS /section:.text,ERW /section:_STACK,ERW color.obj resource.obj
if errorlevel 1 goto end
echo #### SUCCESS ####
:end