echo off

set Name=template_lib
REM TODO: arguments
set OutPath="../../bin/windows/Debug/"

set CommonCompilerFlags=/MT /nologo /Gm- /GR- /EHsc /Od /Oi /WX /W4 /Fo%OutPath%%Name%.obj /Fm%OutPath%%Name%.map /wd4201
set Includes=/I "../glfw-3.2.1/include/" /I "../glew-2.0.0/include/" /I "../glm/"
set LinkerOptions=/LIBPATH:"..\glfw-3.2.1\src\Debug" /LIBPATH:"..\glew-2.0.0\lib\Debug\Win32" /DLL /EXPORT:InitGame /EXPORT:RenderAndUpdate /LIBPATH:"..\glfw-3.2.1\src\Debug" /LIBPATH:"..\glew-2.0.0\lib\Debug\Win32"

cl %CommonCompilerFlags% %Includes% ..\..\src\lib.cpp "glew32d.lib" "glfw3dll.lib" "opengl32.lib" /LD /link %LinkerOptions% /OUT:%OutPath%%Name%.dll
