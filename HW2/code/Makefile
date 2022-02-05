all: *.cpp
	g++ *.cpp -o rasterizer -std=c++11 -O3

run:
	./rasterizer ../inputs_outputs/culling_$(culling)_inputs/$(xml).xml

clean:
	rm -rf *.ppm
