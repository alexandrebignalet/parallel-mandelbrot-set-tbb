.IGNORE:
MAIN=benchmark-mandelbrot

COMPILER=/usr/bin/g++
OPTIONS=-m64 -ltbb -std=c++11

FILES = mandelbrot-benchmark.cpp src/Mandelbrot.cpp src/include/Mandelbrot.h src/Point.cpp src/include/Point.h

default: mandelbrot
	./mandel 1920 1080 250


mandelbrot:
	$(COMPILER) $(FILES) -o mandel $(OPTIONS)

# should generate files for gnuplot
bm: mandelbrot
	./mandel 640 480 150
	./mandel 1280 720 200
	./mandel 1920 1080 250
	./mandel 2560 2048 300
	./mandel 3840 2160 400
	./mandel 5120 4096 500

# should run the tests
tests: $(default)

clean:
	\rm -f mandel
	\rm -f *~
