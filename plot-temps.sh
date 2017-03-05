#!

taille=$1 iter_max=$2 max=$3

gnuplot -persist <<EOF
set terminal png
set output './benchmarks/graphe-temps-$taille.png'
set style line 1 lc rgb "orange" lt 1 lw 2 pt 7
set style line 2 lc rgb "black" lt 1 lw 2 pt 7
set style line 3 lc rgb "red" lw 2 pt 7
set style line 4 lc rgb "blue" lw 2 pt 7
set logscale x
set xlabel "Nombre de threads"
set ylabel "Temps d'exécution"
set title "Temps d'exécution en fonction du nombre de threads pour générer une image $taille"
set xtics (1, 2, 4, 8, 16, 32, 64, 128)
plot [0.9:150][0:$max] \
	 "./benchmarks/temps-$taille-$iter_max.txt" using 1:(\$2) title "seq" with linespoints ls 1,\
	 "./benchmarks/temps-$taille-$iter_max.txt" using 1:(\$3) title "par dyn" with linespoints ls 2,\
	 "./benchmarks/temps-$taille-$iter_max.txt" using 1:(\$4) title "par statique" with linespoints ls 3,\
	 "./benchmarks/temps-$taille-$iter_max.txt" using 1:(\$5) title "par" with linespoints ls 4
EOF
