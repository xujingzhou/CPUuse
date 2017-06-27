
CPUuseps.dll: dlldata.obj CPUuse_p.obj CPUuse_i.obj
	link /dll /out:CPUuseps.dll /def:CPUuseps.def /entry:DllMain dlldata.obj CPUuse_p.obj CPUuse_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del CPUuseps.dll
	@del CPUuseps.lib
	@del CPUuseps.exp
	@del dlldata.obj
	@del CPUuse_p.obj
	@del CPUuse_i.obj
