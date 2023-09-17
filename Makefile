CXXFLAGS=-std=c++17 -g
OBJECTS=Window.o



render: main.cpp $(OBJECTS)
	$(CXX) $(CXXFLAGS)  -I ../glm main.cpp $(OBJECTS) -lGLESv2 -lglfw3 -ldl -lpthread -o render