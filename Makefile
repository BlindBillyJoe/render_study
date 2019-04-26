#Need to make more flexible in future, but working for now
CXX = clang++
AR = llvm-ar

OBJS = obj/main.o 
SWR_OBJS = obj/model.o obj/objparser.o obj/renderer.o obj/tgadevice.o obj/tgaimage.o obj/color.o
OUT = -o bin/render.exe
LIBS = lib/libswr.a
SWR_LIB = lib/libswr.a

all: swr.a

swr.a: model.o objparser.o renderer.o tgadevice.o tgaimage.o color.o
	$(AR) rcs $(SWR_LIB) $(SWR_OBJS)

model.o: swr/model/model.cpp
	$(CXX) -c swr/model/model.cpp -o obj/model.o

objparser.o: swr/parser/objparser.cpp
	$(CXX) -c swr/parser/objparser.cpp -o obj/objparser.o

renderer.o: swr/render/renderer.cpp
	$(CXX) -c swr/render/renderer.cpp -o obj/renderer.o

tgadevice.o: swr/render/iodevice/tgadevice.cpp
	$(CXX) -c swr/render/iodevice/tgadevice.cpp -o obj/tgadevice.o

tgaimage.o: swr/render/iodevice/tgaimage.cpp
	$(CXX) -c swr/render/iodevice/tgaimage.cpp -o obj/tgaimage.o

color.o: swr/utils/color.cpp
	$(CXX) -c swr/utils/color.cpp -o obj/color.o

libs: swr.a

clean:
	Del obj/*.o