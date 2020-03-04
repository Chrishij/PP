#!/bin/bash
echo "Run!"

#sed -i 's/\r$//' <filename>
#sed -i 's/\r$//' run.sh

#valgrind --leak-check=full


# cd A4
# make
# time ./galsim 4 ../input_data/ellipse_N_00500.gal 200 0.00001 0.0 0
# cd ..
# cd compare_gal_files
# gcc -o comp compare_gal_files.c -lm
# ./comp 500 ../ref_output_data/ellipse_N_00500_after200steps.gal ../A4/result.gal

cd A4
make
time ./galsim 500 ../input_data/ellipse_N_00500.gal 200 0.00001 0.02 0
cd ..
cd compare_gal_files
gcc -o comp compare_gal_files.c -lm
./comp 500 ../ref_output_data/ellipse_N_00500_after200steps.gal ../A4/result.gal

# cd ..
# cd A4
# time  ./galsim 4 ../input_data/ellipse_N_02000.gal 200 0.00001 0.2 0
# cd ..
# cd compare_gal_files
# gcc -o comp compare_gal_files.c -lm
# ./comp 2000 ../ref_output_data/ellipse_N_02000_after200steps.gal ../A4/result.gal

# cd ..
# cd A4
# time ./galsim 4 ../input_data/ellipse_N_02000.gal 200 0.00001 0.5 0
# cd ..
# cd compare_gal_files
# gcc -o comp compare_gal_files.c -lm
# ./comp 2000 ../ref_output_data/ellipse_N_02000_after200steps.gal ../A4/result.gal

# cd ..
# cd A4
# time ./galsim 4 ../input_data/ellipse_N_02000.gal 200 0.00001 0.9 0
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