all: RTIN.o
	g++ -Iinclude -I$(BOOST_ROOT) src/roam.cpp src/RTIN.o

RTIN.o:
	g++ -c -Iinclude -I$(BOOST_ROOT) src/RTIN.cpp

HeightMap.o:
	g++ -c -Iinclude src/HeightMap.cpp