.IGNORE:
MAIN=benchmark-mandelbrot

COMPILER=/usr/bin/g++
OPTIONS=-m64 -ltbb -std=c++11

FILES = src/Mandelbrot.cpp src/include/Mandelbrot.h src/Point.cpp src/include/Point.h
TESTS_LIB = ./tests/lib/MiniCUnit.c ./tests/lib/MiniCUnit.h

default: test

mandelbrot:
	$(COMPILER) mandelbrot-benchmark.cpp $(FILES) -o mandel $(OPTIONS)


bm: data_bm

graphes_acc:
	./plot-acc.sh 640x360 50 70
	./plot-acc.sh 1280x720 50 70
	./plot-acc.sh 1920x1080 50 70
	./plot-acc.sh 3840x2160 50 70

graphes_temps:
	./plot-temps.sh 640x360 50 0,3
	./plot-temps.sh 1280x720 50 1
	./plot-temps.sh 1920x1080 50 10
	./plot-temps.sh 3840x2160 50 20

data_bm:
	./mandel 640 360 100 > ./benchmarks/temps-640x360-50.txt
	./mandel 1280 720 200 > ./benchmarks/temps-1280x720-50.txt
	./mandel 1920 1080 250 > ./benchmarks/temps-1920x1080-50.txt
	./mandel 3840 2160 400 > ./benchmarks/temps-3840x2160-50.txt


test:
	$(COMPILER) $(FILES) $(TESTS_LIB) ./tests/PointTest.cpp -o mandel_points_tests $(OPTIONS) -w
	$(COMPILER) $(FILES) $(TESTS_LIB) ./tests/MandelbrotTest.cpp -o mandel_mandelbrot_tests $(OPTIONS) -w
	./mandel_points_tests
	./mandel_mandelbrot_tests

clean:
	\rm -f mandel
	\rm -f *~
