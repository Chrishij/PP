#!/bin/bash
echo "Run!"

#sed -i 's/\r$//' <filename>
#sed -i 's/\r$//' run.sh

#To fix display
#DISPLAY=:0.0; export DISPLAY
#xhost +

#valgrind --leak-check=full

#----

# cd A5
# gcc -Wall -Ofast -I/opt/X11/include -c graphics.c
# #gcc -Wall -Ofast -I/opt/X11/include -c galsim.c
# gcc -Wall -O1 -g -pg -I/opt/X11/include -c galsim.c
# #gcc -O1 -g -pg --disable-gold galsim /lib/gcrt0.o galsim.o graphics.o -lc_p -L/opt/X11/lib -lX11 -lm 
# gcc -O1 -g -pg galsim galsim.o graphics.o -L/opt/X11/lib -lX11 -lm 

# ./galsim 500 ./galsim 500 ../input_data/ellipse_N_00500.gal 200 0.00001 0.0 0
# gprof galsim 500 ../input_data/ellipse_N_00500.gal 200 0.00001 0.0 0 gmon.out > analysis.txt
# #cd ..
# #cd compare_gal_files
# #gcc -o comp compare_gal_files.c -lm
# #./comp 500 ../ref_output_data/ellipse_N_00500_after200steps.gal ../A5/result.gal
# cd ..


# # ---
# cd A5
# make
# pprof --pdf ./galsim 500 ./galsim 500 ../input_data/ellipse_N_00500.gal 200 0.00001 0.0 0
#  galsim.prof > out.pdf

# # #----

cd A5
make
time ./galsim 2000 ../input_data/ellipse_N_02000.gal 200 0.00001 0.21 0
#valgrind --tool=memcheck ./galsim 2000 ../input_data/ellipse_N_00500.gal 200 0.00001 0.21 0
cd ..
cd compare_gal_files
gcc -o comp compare_gal_files.c -lm
./comp 2000 ../ref_output_data/ellipse_N_02000_after200steps.gal ../A5/result.gal
cd ..

# cd A4
# make
# time ./galsim 4 ../input_data/ellipse_N_00500.gal 200 0.00001 0.0 0
# cd ..
# cd compare_gal_files
# gcc -o comp compare_gal_files.c -lm
# ./comp 500 ../ref_output_data/ellipse_N_00500_after200steps.gal ../A4/result.gal
# cd ..



