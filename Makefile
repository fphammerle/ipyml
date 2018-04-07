ipyml : main.cpp link.cpp
	g++ -std=c++11 $^ -lmnl -o $@
