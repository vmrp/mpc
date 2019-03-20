@echo off
del ..\lib\mtk_vm.lib
del ..\lib\spr_vm.lib

del ..\lib\mtk176_run.lib
del ..\lib\mtk240.lib
del ..\lib\mtk400.lib
del ..\lib\spr176.lib
del ..\lib\spr240.lib
del ..\lib\spr400.lib

del ..\bin\mtk176.mrp
del ..\bin\mtk240.mrp
del ..\bin\mtk400.mrp
del ..\bin\spr176.mrp
del ..\bin\spr240.mrp
del ..\bin\spr400.mrp
@echo on

c:\SKYsdk\Compiler\mrpbuilder.net.exe ../mpr/lib_run.mpr
c:\SKYsdk\Compiler\mrpbuilder.net.exe ../mpr/vm.mpr
c:\SKYsdk\Compiler\mrpbuilder.net.exe ../mpr/Makefile_run.mpr
pause
