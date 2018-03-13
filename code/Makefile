CXX = g++-5
CXXFLAGS = -std=c++14 -Wall -MMD
EXEC = quadris
OBJECTS = main.o board.o block.o blocktypes.o \
		  subject.o textdisplay.o graphicsdisplay.o window.o \
		  blockgenerator.o level.o level0.o level1.o level2.o level3.o level4.o

DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
			${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
