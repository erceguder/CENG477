all:
	g++ *.cpp -o raytracer -std=c++11 -O3
run:
	./raytracer sample_scenes/inputs/simple.xml