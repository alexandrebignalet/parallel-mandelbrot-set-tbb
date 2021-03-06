.IGNORE:
MAIN=mandelbrot-benchmark

COMPILER=/usr/bin/g++
OPTIONS=-m64 -ltbb -std=c++11

FILES = src/Mandelbrot.cpp src/include/Mandelbrot.h src/Point.cpp src/include/Point.h
TESTS_LIB = ./tests/lib/MiniCUnit.c ./tests/lib/MiniCUnit.h

ITER_MAX = 200
WARMUP_NB = 10
REPETITION_NB = 10

default: mandelbrot
	./mandel 1200 1200 $(WARMUP_NB) $(REPETITION_NB) $(ITER_MAX)

mandelbrot:
	$(COMPILER) ./benchmarks/mandelbrot-benchmark.cpp $(FILES) -o mandel $(OPTIONS)


bm: mandelbrot data_bm

graphes_acc:
	./benchmarks/scripts/plot-acc.sh 640x360 $(ITER_MAX) 70
	./benchmarks/scripts/plot-acc.sh 1280x720 $(ITER_MAX) 70
	./benchmarks/scripts/plot-acc.sh 1920x1080 $(ITER_MAX) 70
	./benchmarks/scripts/plot-acc.sh 3840x2160 $(ITER_MAX) 70

graphes_temps:
	./benchmarks/scripts/plot-temps.sh 640x360 $(ITER_MAX) 0.1
	./benchmarks/scripts/plot-temps.sh 1280x720 $(ITER_MAX) 0.5
	./benchmarks/scripts/plot-temps.sh 1920x1080 $(ITER_MAX) 1
	./benchmarks/scripts/plot-temps.sh 3840x2160 $(ITER_MAX) 3

data_bm:
	./mandel 640 360 $(WARMUP_NB) $(REPETITION_NB) $(ITER_MAX) > ./benchmarks/data-files/temps-640x360-$(ITER_MAX).txt
	./mandel 1280 720 $(WARMUP_NB) $(REPETITION_NB) $(ITER_MAX) > ./benchmarks/data-files/temps-1280x720-$(ITER_MAX).txt
	./mandel 1920 1080 $(WARMUP_NB) $(REPETITION_NB) $(ITER_MAX) > ./benchmarks/data-files/temps-1920x1080-$(ITER_MAX).txt
	./mandel 3840 2160 $(WARMUP_NB) $(REPETITION_NB) $(ITER_MAX) > ./benchmarks/data-files/temps-3840x2160-$(ITER_MAX).txt


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
