CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -pedantic -DMEMTRACE
TARGET   = rental
SRCS     = main.cpp utils.cpp eszkoz.cpp rendeles.cpp rendszer.cpp memtrace.cpp
OBJS     = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o:     main.cpp     rendszer.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

utils.o:    utils.cpp    utils.hpp
	$(CXX) $(CXXFLAGS) -c utils.cpp

eszkoz.o:   eszkoz.cpp   eszkoz.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -c eszkoz.cpp

rendeles.o: rendeles.cpp rendeles.hpp eszkoz.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -c rendeles.cpp

rendszer.o: rendszer.cpp rendszer.hpp rendeles.hpp eszkoz.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -c rendszer.cpp

memtrace.o: memtrace.cpp memtrace.h
	$(CXX) $(CXXFLAGS) -c memtrace.cpp

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
