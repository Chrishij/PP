echo "Run!"

cd A5
make
time ./galsim 2000 ../input_data/ellipse_N_02000.gal 200 0.00001 0.21 0 1
time ./galsim 2000 ../input_data/ellipse_N_02000.gal 200 0.00001 0.21 0 2
time ./galsim 2000 ../input_data/ellipse_N_02000.gal 200 0.00001 0.21 0 3
time ./galsim 2000 ../input_data/ellipse_N_02000.gal 200 0.00001 0.21 0 4
time ./galsim 2000 ../input_data/ellipse_N_02000.gal 200 0.00001 0.21 0 5
time ./galsim 2000 ../input_data/ellipse_N_02000.gal 200 0.00001 0.21 0 6
time ./galsim 2000 ../input_data/ellipse_N_02000.gal 200 0.00001 0.21 0 7
time ./galsim 2000 ../input_data/ellipse_N_02000.gal 200 0.00001 0.21 0 8
# cd ..
# cd compare_gal_files
# gcc -o comp compare_gal_files.c -lm
# ./comp 2000 ../ref_output_data/ellipse_N_02000_after200steps.gal ../A5/result.gal
cd ..
