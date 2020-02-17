build:
	g++ -w -std=c++17 -Wfatal-errors -m32 -static-libstdc++ \
	./src/*.cpp \
	-o sdlrts.exe \
	-I"C:\libsdl\include" \
	-L"C:\libsdl\lib" \
	-I".\lib\lua" \
	-L".\lib\lua" \
	-llua53 \
	-lmingw32 \
	-lSDL2main \
	-lSDL2_image \
	-lSDL2_ttf \
	-lSDL2_mixer \
	-lSDL2

clean:
	del sdlrts.exe

run:
	sdlrts.exe