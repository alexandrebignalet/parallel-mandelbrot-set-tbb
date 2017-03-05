.IGNORE:
MAIN=mandelbrot-benchmark

COMPILER=/usr/bin/g++
OPTIONS=-m64 -ltbb -std=c++11

FILES = src/Mandelbrot.cpp src/include/Mandelbrot.h src/Point.cpp src/include/Point.h
TESTS_LIB = ./tests/lib/MiniCUnit.c ./tests/lib/MiniCUnit.h

default: mandelbrot
	./mandel 1200 1200 200

mandelbrot:
	$(COMPILER) ./benchmarks/mandelbrot-benchmark.cpp $(FILES) -o mandel $(OPTIONS)


bm: data_bm

graphes_acc:
	./benchmarks/scripts/plot-acc.sh 640x360 200 70
	./benchmarks/scripts/plot-acc.sh 1280x720 200 70
	./benchmarks/scripts/plot-acc.sh 1920x1080 200 70
	./benchmarks/scripts/plot-acc.sh 3840x2160 200 70

graphes_temps:
	./benchmarks/scripts/plot-temps.sh 640x360 200 0.3
	./benchmarks/scripts/plot-temps.sh 1280x720 200 1
	./benchmarks/scripts/plot-temps.sh 1920x1080 200 10
	./benchmarks/scripts/plot-temps.sh 3840x2160 200 20

data_bm:
	./mandel 640 360 200 > ./benchmarks/data-files/temps-640x360-200.txt
	./mandel 1280 720 200 > ./benchmarks/data-files/temps-1280x720-200.txt
	./mandel 1920 1080 200 > ./benchmarks/data-files/temps-1920x1080-200.txt
	./mandel 3840 2160 200 > ./benchmarks/data-files/temps-3840x2160-200.txt


test:
	$(COMPILER) $(FILES) $(TESTS_LIB) ./tests/PointTest.cpp -o mandel_points_tests $(OPTIONS) -w
	$(COMPILER) $(FILES) $(TESTS_LIB) ./tests/MandelbrotTest.cpp -o mandel_mandelbrot_tests $(OPTIONS) -w
	./mandel_points_tests
	./mandel_mandelbrot_tests

clean:
	\rm -f mandel
	\rm -f mandel_mandelbrot_tests
	\rm -f mandel_points_tests
	\rm -f *~
	\rm -f ./benchmarks/plots/*.png
	\rm -f ./benchmarks/data-files/*.txt
