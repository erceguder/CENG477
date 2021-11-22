all: *.cpp
	g++ *.cpp -o raytracer -pthread -std=c++11 -O3

output:
	./raytracer sample_scenes/inputs/$(name).xml

simple:
	./raytracer sample_scenes/inputs/simple.xml

bunny:
	./raytracer sample_scenes/inputs/bunny.xml

killeroo:
	./raytracer sample_scenes/inputs/killeroo.xml

cornellbox: 
	./raytracer sample_scenes/inputs/cornellbox.xml

car:
	./raytracer sample_scenes/inputs/car.xml

clear:
	rm -rf *.ppm
