
refresh: refresh_release refresh_debug

refresh_debug:
	cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build/
build_debug:
	make -C build
debug: build_debug
	./build/OpenGLTempl

refresh_release:
	cmake -DCMAKE_BUILD_TYPE=Release -S . -B Release/
build_release:
	make -C Release
release: build_release
	./Release/OpenGLTempl

.PHONY: refresh refresh_release refresh_debug build_release build_debug debug release



