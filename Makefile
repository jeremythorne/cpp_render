render: main.cpp
	g++ -std=c++17 -g -I ../glm main.cpp -lGLESv2 -lglfw3 -ldl -lpthread -o render