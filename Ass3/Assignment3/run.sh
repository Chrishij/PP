#!/bin/bash
echo "Run!"

#sed -i 's/\r$//' <filename>
#sed -i 's/\r$//' run.sh

cd A3
make
#valgrind -v --track-origins=yes ./galsim 500 ../input_data/ellipse_N_00500.gal 200 0.00001 0
valgrind -v --leak-check=full ./galsim 500 ../input_data/ellipse_N_00500.gal 200 0.00001 0
#./galsim 500 ../input_data/ellipse_N_00500.gal 200 0.00001 0
cd ..
cd compare_gal_files
gcc -o comp compare_gal_files.c -lm
./comp 500 ../ref_output_data/ellipse_N_00500_after200steps.gal ../A3/result.gal