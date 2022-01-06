all: *.cpp
	g++ *.cpp -o raytracer -pthread -std=c++11 -O3

output:
	./raytracer sample_scenes/inputs/$(name).xml

clear:
	rm -rf *.ppm
