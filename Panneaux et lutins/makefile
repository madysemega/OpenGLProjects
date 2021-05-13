CONTEXT=sdl2
#CONTEXT=glfw3

CXXFLAGS += -DFENETRE_$(CONTEXT)
CXXFLAGS += -g -W -Wall -Wno-unused-parameter -Wvla -std=c++17 -pedantic # -Wno-deprecated-declarations
CXXFLAGS += $(shell pkg-config --cflags glew || echo -I/usr/local/include)
CXXFLAGS += $(shell pkg-config --cflags $(CONTEXT) || echo -I/usr/local/include/SDL2)

LDFLAGS += -g
LDFLAGS += $(shell pkg-config --libs glew || echo -I/usr/local/lib -lGLEW)
LDFLAGS += $(shell pkg-config --libs $(CONTEXT) || echo -I/usr/local/lib -lSDL2)

ifeq "$(shell uname)" "Darwin"
  LDFLAGS += -framework OpenGL
  ifeq "$(CONTEXT)" "glfw3"
    LDFLAGS += -lobjc -framework Foundation -framework Cocoa
  endif
endif

SRC=main
EXE=build/tp4_panneaux.exe

exe : $(EXE)
run : exe
	./$(EXE)
$(EXE) : *.cpp *.h
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o$@ *.cpp $(LDFLAGS)

# pour construire un projet en utilisant cmake (il faut ensuite aller dans 'build' et y faire make)
cmake :
	mkdir -p build
	cd ./build && cmake ..

# nettoyage
clean :
	rm -rf *.o *.exe *.exe.dSYM ${EXE}
realclean : clean
	rm -rf build/

# pour créer le fichier à remettre dans Moodle
remise zip :
	make clean
	rm -f INF2705_remise_tp4.zip
	zip -r INF2705_remise_tp4.zip *.cpp *.h *.glsl makefile *.txt textures
