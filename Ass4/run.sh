#!/bin/bash
echo "Run!"

#sed -i 's/\r$//' <filename>
#sed -i 's/\r$//' run.sh

#To fix display
#DISPLAY=:0.0; export DISPLAY
#xhost +

#valgrind --leak-check=full

#----
# Profile
# 
# cd A4
# gcc -Wall -Ofast -I/opt/X11/include -c graphics.c
# gcc -Wall -Ofast -I/opt/X11/include -c galsim.c
# gcc -o -g -pg galsim galsim.o graphics.o -L/opt/X11/lib -lX11 -lm

# ./galsim 500 ./galsim 4 ../input_data/ellipse_N_00500.gal 200 0.00001 0.0 0
# gprof galsim gmon.out
# #cd ..
# #cd compare_gal_files
# #gcc -o comp compare_gal_files.c -lm
# #./comp 500 ../ref_output_data/ellipse_N_00500_after200steps.gal ../A4/result.gal
# cd ..

#----


# cd A4
# make
# time ./galsim 4 ../input_data/ellipse_N_00500.gal 200 0.00001 0.0 0
# cd ..
# cd compare_gal_files
# gcc -o comp compare_gal_files.c -lm
# ./comp 500 ../ref_output_data/ellipse_N_00500_after200steps.gal ../A4/result.gal
# cd ..

cd A4
make
#time ./galsim 5000 ../input_data/ellipse_N_05000.gal 100 0.00001 0.21 0
time ./galsim 500 ../input_data/ellipse_N_00500.gal 100 0.00001 0.21 1
cd ..
cd compare_gal_files
gcc -o comp compare_gal_files.c -lm
./comp 500 ../ref_output_data/ellipse_N_00500_after200steps.gal ../A4/result.gal
cd ..


# cd A4
# time  ./galsim 4 ../input_data/ellipse_N_02000.gal 200 0.00001 0.2 0
# cd ..
# cd compare_gal_files
# gcc -o comp compare_gal_files.c -lm
# ./comp 2000 ../ref_output_data/ellipse_N_02000_after200steps.gal ../A4/result.gal
# cd ..


# cd A4
# time ./galsim 4 ../input_data/ellipse_N_02000.gal 200 0.00001 1 0
# cd ..
# cd compare_gal_files
# gcc -o comp compare_gal_files.c -lm
# ./comp 2000 ../ref_output_data/ellipse_N_02000_after200steps.gal ../A4/result.gal