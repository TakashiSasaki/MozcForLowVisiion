.PHONY: all clean sync gyp build_tools build uninstall install

all: uninstall clean sync gyp build_tools-release build-release uninstall install-release

clean:
	git clean -fdX	

build-release: build_tools-release
	-cmd.exe /C build_mozc.bat build -c Release package

build-debug: build_tools-debug
	-cmd.exe /C build_mozc.bat build -c Debug package

build_tools-release:
	-cmd.exe /C build_mozc.bat build_tools -c Release

build_tools-debug:
	-cmd.exe /C build_mozc.bat build_tools -c Debug

gyp:
	-cmd.exe /C build_mozc.bat gyp --msvs_version=2012 --noqt

sync:
	python depot_tool/gclient.py sync


target_dir_32=/cygdrive/c/Program\ Files\ \(x86\)/mozc
target_dir_64=/cygdrive/c/Program\ Files/mozc

install-release: 
	-mkdir $(target_dir_32)
	cp src/out/Release/mozc_broker32.exe $(target_dir_32)
	cp src/out/Release/mozc_cache_service.exe $(target_dir_32)
	cp src/out/Release/mozc_renderer.exe $(target_dir_32)
	cp src/out/Release/mozc_server.exe $(target_dir_32)
	cp src/out/Release/mozc_tool.exe $(target_dir_32)
	cp src/out/Release/mozc_ja.ime /cygdrive/c/Windows/System32/
	ls $(target_dir_32)
	$(target_dir_32)/mozc_broker32.exe --mode=register_ime

install-debug: 
	-mkdir $(target_dir_32)
	cp src/out/Debug/mozc_broker32.exe $(target_dir_32)
	cp src/out/Debug/mozc_cache_service.exe $(target_dir_32)
	cp src/out/Debug/mozc_renderer.exe $(target_dir_32)
	cp src/out/Debug/mozc_server.exe $(target_dir_32)
	cp src/out/Debug/mozc_tool.exe $(target_dir_32)
	cp src/out/Debug/mozc_ja.ime /cygdrive/c/Windows/System32/
	ls $(target_dir_32)
	$(target_dir_32)/mozc_broker32.exe --mode=register_ime

uninstall:
	-$(target_dir_32)/mozc_broker32.exe --mode=unregister_ime

