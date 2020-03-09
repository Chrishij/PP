#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "graphics.h"

//Constants
int N=500;
double dt=0.00001;

const float circleRadius=0.0025, circleColor=0;
const int windowWidth=800;


// typedef struct vector {
// 	double x;
// 	double y;
// } vector_t;

typedef struct star {
	double pos_x;
	double pos_y;
	double vel_x;
	double vel_y;
	double F_x;
	double F_y;
	double mass;
	double brightness;
} star_t;

int fsize(FILE *fp){
    int prev=ftell(fp);
    fseek(fp, 0L, SEEK_END);
    int sz=ftell(fp);
    fseek(fp,prev,SEEK_SET); //go back to where we were
    return sz;
}

void initStar (star_t** star, double xPos, double yPos, double xVel, double yVel, double mass,double brightness) {
	(*star) = (star_t*)malloc(sizeof(star_t));

	(*star)->pos_x = xPos;
	(*star)->pos_y = yPos;
	(*star)->vel_x = xVel;
	(*star)->vel_y = yVel;
	(*star)->mass = mass;
	(*star)->brightness = brightness;
}

int main(int argc, char *argv[]) {
	if (argc != 6)
	{
		printf("Not five input arguments!\n");
	}
	else {
		char* filename = argv[2];
		int nsteps = atoi(argv[3]);
		double delta_t = atof(argv[4]);
		int graphics = atoi(argv[5]);

		FILE* file1;
		file1 = fopen(filename, "r");
		N = fsize(file1)/6/sizeof(double);
		star_t* starArray[N];

		dt = delta_t;

		size_t mem_block_size = 1;
		double pos_x=0;
		double pos_y=0;
		double vel_x=0;
		double vel_y=0;
		double mass=0;
		double brightness=0;

		double temp =0;


		for (int i = 0; i < N; ++i)								
		{
			temp = fread(&pos_x, sizeof(double), mem_block_size, file1);
			temp = fread(&pos_y, sizeof(double), mem_block_size, file1);
			temp = fread(&mass, sizeof(double), mem_block_size, file1);
			temp = fread(&vel_x, sizeof(double), mem_block_size, file1);
			temp = fread(&vel_y, sizeof(double), mem_block_size, file1);
			temp = fread(&brightness, sizeof(double), mem_block_size, file1);
			temp = temp+1;
			initStar(&(starArray[i]), pos_x, pos_y, vel_x, vel_y, mass, brightness);
		}

		fclose(file1);

	 	float L=1;
	 	float W=1;

	    if (graphics == 1)
	    {
			InitializeGraphics(argv[0],windowWidth,windowWidth);
			SetCAxes(0,1);
		}

		for (int k = 0; k < nsteps; ++k)
		{
				
		    for (int i = 0; i < N; ++i){
				
				//vector_t distance;
				double distance_x=0;
				double distance_y=0;

				double G = 100.0/N;
				double norm;
				double con;
				double epsilon = 0.001;

				for (int j = i; j < N; ++j)
				{
					distance_x = starArray[i]->pos_x - starArray[j]->pos_x;
					distance_y = starArray[i]->pos_y - starArray[j]->pos_y;
					norm = sqrt((distance_x)*(distance_x) + (distance_y)*(distance_y)) + epsilon;
					con = (-G*(starArray[i]->mass)*(starArray[j]->mass))/((norm)*(norm)*(norm));

					starArray[i]->F_x += distance_x*con;
					starArray[i]->F_y += distance_y*con;

					starArray[j]->F_x += -distance_x*con;
					starArray[j]->F_y += -distance_y*con;
				}
		    }

		    for (int i = 0; i < N; ++i)
		    {
		    	//vector_t acc;
				double acc_x=0;
				double acc_y=0;

				acc_x = starArray[i]->F_x/starArray[i]->mass;
				acc_y = starArray[i]->F_y/starArray[i]->mass;

				starArray[i]->vel_x += acc_x*dt;
				starArray[i]->vel_y += acc_y*dt;

				starArray[i]->pos_x += starArray[i]->vel_x *dt;
				starArray[i]->pos_y += starArray[i]->vel_y *dt;

				starArray[i]->F_x = 0;
				starArray[i]->F_y = 0;
		    }

		    if (graphics == 1)
		    {
			    ClearScreen();
			    for (int i = 0; i < N; ++i){

			    	DrawCircle((*starArray[i]).pos_x, (*starArray[i]).pos_y, L, W, circleRadius, circleColor);
			    }
			    Refresh();
			    usleep(3000);
		    }
		}

		FILE* file2;
		file2 = fopen("result.gal", "w");


		for (int i = 0; i < N; ++i)
		{
		
			// fwrite(&(starArray[i]->pos_x), 1, sizeof(double), file2);
			// fwrite(&(starArray[i]->pos_y), 1, sizeof(double), file2);
			// fwrite(&(starArray[i]->mass), 1, sizeof(double), file2);
			// fwrite(&(starArray[i]->vel_x), 1, sizeof(double), file2);
			// fwrite(&(starArray[i]->vel_y), 1, sizeof(double), file2);
			// fwrite(&(starArray[i]->brightness), 1, sizeof(double), file2);
			
			pos_x=starArray[i]->pos_x;
			pos_y=starArray[i]->pos_y;
			vel_x=starArray[i]->vel_x;
			vel_y=starArray[i]->vel_y;
			mass= starArray[i]->mass;
			brightness= starArray[i]->brightness;

			// fwrite(&pos_x, 1, sizeof(double), file2);
			// fwrite(&pos_y, 1, sizeof(double), file2);
			// fwrite(&mass, 1, sizeof(double), file2);
			// fwrite(&vel_x, 1, sizeof(double), file2);
			// fwrite(&vel_y, 1, sizeof(double), file2);
			// fwrite(&brightness, 1, sizeof(double), file2);

			fwrite(&pos_x, sizeof(double), mem_block_size, file2);
			fwrite(&pos_y, sizeof(double), mem_block_size, file2);
			fwrite(&mass,sizeof(double), mem_block_size, file2);
			fwrite(&vel_x, sizeof(double), mem_block_size, file2);
			fwrite(&vel_y, sizeof(double), mem_block_size, file2);
			fwrite(&brightness, sizeof(double), mem_block_size, file2);
		}

		fclose(file2);

		for (int i = 0; i < N; ++i)
		{
			free(starArray[i]);
		}		
	}
}