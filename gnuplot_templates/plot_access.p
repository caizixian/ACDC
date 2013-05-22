unset title

set terminal postscript eps color
set output 'access.eps'

set xlabel "x value"
set ylabel "total access time per thread in milliseconds\n(lower is better)"

set multiplot

set key outside center bottom horizontal

plot "access.dat" using 1:($2/2100000):($3/2100000):xticlabel(1) with errorlines lt 1 lw 3 title 'jemalloc', \
"access.dat" using 1:(($4)/2100000):($5/2100000):xticlabel(1) with errorlines    lt 2 lw 3 title 'llalloc', \
"access.dat" using 1:(($6)/2100000):($7/2100000):xticlabel(1) with errorlines   lt 3 lw 3 title 'static', \
"access.dat" using 1:(($8)/2100000):($9/2100000):xticlabel(1) with errorlines   lt 4 lw 3 title 'ptmalloc2', \
"access.dat" using 1:(($10)/2100000):($11/2100000):xticlabel(1) with errorlines   lt 8 lw 3 title 'ptmalloc3', \
"access.dat" using 1:(($12)/2100000):($13/2100000):xticlabel(1) with errorlines   lt 9 lw 3 title 'tbb', \
"access.dat" using 1:(($14)/2100000):($15/2100000):xticlabel(1) with errorlines   lt 7 lw 3 title 'tcmalloc', \
"access.dat" using 1:(($16)/2100000):($17/2100000):xticlabel(1) with errorlines   lt 10 lw 3 title 'streamflow', \
"access.dat" using 1:(($18)/2100000):($19/2100000):xticlabel(1) with errorlines   lt 11 lw 3 title 'hoard', \
"access.dat" using 1:(($20)/2100000):($21/2100000):xticlabel(1) with errorlines   lt 12 lw 3 title 'scalloc
