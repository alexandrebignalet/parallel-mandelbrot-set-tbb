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
bm: data_bm

graphes_acc:
	./plot-acc.sh 1280x720 200 6
	./plot-acc.sh 1920x1080 250 6
	./plot-acc.sh 3840x2160 400 6

data_bm:
	./mandel 1280 720 200 > ./benchmarks/temps-1280x720-200.txt
	./mandel 1920 1080 250 > ./benchmarks/temps-1920x1080-250.txt
	./mandel 3840 2160 400 > ./benchmarks/temps-3840x2160-400.txt

# should run the tests
tests: $(default)

clean:
	\rm -f mandel
	\rm -f *~
