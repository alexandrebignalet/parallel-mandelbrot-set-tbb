.IGNORE:
MAIN=benchmark-mandelbrot

COMPILER=/usr/bin/g++
OPTIONS=-m64 -ltbb -std=c++11

FILES = benchmark-mandelbrot.cpp Mandelbrot.cpp Mandelbrot.h Point.cpp Point.h

default: benchmark-mandelbrot
	./mandel 3840 2160 400


benchmark-mandelbrot:
	$(COMPILER) $(FILES) -o mandel $(OPTIONS)

bm: benchmark-mandelbrot
	./mandel 640 480 150
	./mandel 1280 720 200
	./mandel 1920 1080 250
	./mandel 2560 2048 300
	./mandel 3840 2160 400
	./mandel 5120 4096 500

clean:
	\rm -f histogramme
	\rm -f *~