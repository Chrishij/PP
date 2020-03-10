#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include "graphics.h"

// const int* N;
// int steps;
double dt;
double theta_max;
double G;

const double epsilon = 0.001;
const float circleRadius=0.0025, circleColor=0;
const int windowWidth=800;

const float L=1;
const float W=1;

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

typedef struct quad {
	struct quad* quadOne;
	struct quad* quadTwo;
	struct quad* quadThree;
	struct quad* quadFour;
	star_t* star;	
	double x;
	double y;
	double w;
	double mass;
	double center_x;
	double center_y;
} quad_type;

typedef struct thread_data {
	int startIndex;
	int numOfIter;
	quad_type* quad;
	star_t* array;
} thread_d;

void initQuad (quad_type* quad, double x, double y, double w){
	quad->x = x;
	quad->y = y;
	quad->w = w;
	quad->quadOne = NULL;
	quad->quadTwo = NULL;
	quad->quadThree = NULL;
	quad->quadFour = NULL;
	quad->star = NULL;
}

void addStar (quad_type* quad, star_t* star);

void subDivide (quad_type* quad) {
	double xplusmargin = (quad->x+(quad->w/4));
	double xminusmargin = (quad->x-(quad->w/4));
	double yplusmargin = quad->y+(quad->w/4); 
	double yminusmargin = quad->y-(quad->w/4);

	quad->quadOne = (quad_type*)malloc(sizeof(quad_type));
	quad->quadTwo = (quad_type*)malloc(sizeof(quad_type));
	quad->quadThree = (quad_type*)malloc(sizeof(quad_type));
	quad->quadFour = (quad_type*)malloc(sizeof(quad_type));

	initQuad(quad->quadOne, xplusmargin, yplusmargin , (quad->w/2));
	initQuad(quad->quadTwo, xminusmargin, yplusmargin , (quad->w/2));
	initQuad(quad->quadThree, xminusmargin, yminusmargin, (quad->w/2));
	initQuad(quad->quadFour, xplusmargin, yminusmargin, (quad->w/2));

	if(quad->star->pos_x >= quad->x) {
		if (quad->star->pos_y >= quad->y) {
			quad->quadOne->star = quad->star;
		}
		else if (quad->star->pos_y < quad->y) {
			quad->quadFour->star = quad->star;
		}
	}
	else if (quad->star->pos_x < quad->x) {
		if (quad->star->pos_y >= quad->y){
			quad->quadTwo->star = quad->star;
		}	
		else if (quad->star->pos_y < quad->y) {
			quad->quadThree->star = quad->star;
		}
	}
	quad->star = NULL;

}

void centerOfMass(quad_type* quad){
	if(!(quad->quadOne)){
		if(quad->star){
	 		quad->center_x = quad->star->pos_x;
	 		quad->center_y = quad->star->pos_y;
		} else {
			quad->center_x = quad->x;
			quad->center_y = quad->y;
		}
	}
	else if(quad->quadOne){
		centerOfMass(quad->quadOne);
		centerOfMass(quad->quadTwo);
		centerOfMass(quad->quadThree);
		centerOfMass(quad->quadFour);

		quad->center_x = ((quad->quadOne->mass*quad->quadOne->center_x + quad->quadTwo->mass*quad->quadTwo->center_x + quad->quadThree->mass*quad->quadThree->center_x + quad->quadFour->mass*quad->quadFour->center_x)/(quad->mass));

		quad->center_y = ((quad->quadOne->mass*quad->quadOne->center_y + quad->quadTwo->mass*quad->quadTwo->center_y + quad->quadThree->mass*quad->quadThree->center_y + quad->quadFour->mass*quad->quadFour->center_y)/(quad->mass));

	}
}

double quadMass(quad_type* quad){
	double mass = 0;
	if(quad->quadOne){
		mass=quadMass(quad->quadOne)+quadMass(quad->quadTwo)+quadMass(quad->quadThree)+quadMass(quad->quadFour);
	}
	if(quad->star){
		mass=quad->star->mass;
	}
	quad->mass = mass;
	return mass;
}


void forceCal(quad_type* quad, star_t* star){

	double theta;

	double distance_center_x=0;
	double distance_center_y=0;

	distance_center_x = star->pos_x - quad->center_x;
	distance_center_y = star->pos_y - quad->center_y;

	double norm2 = sqrt((distance_center_x)*(distance_center_x) + (distance_center_y)*(distance_center_y)) + epsilon;
	
	theta = quad->w/norm2;

	if(quad->star){
		star->F_x += distance_center_x*(-G*(quad->mass))/((norm2)*(norm2)*(norm2));
		star->F_y += distance_center_y*(-G*(quad->mass))/((norm2)*(norm2)*(norm2));
		printf("update F\n");
	}
	else if(theta <= theta_max){
		star->F_x += distance_center_x*(-G*(quad->mass))/((norm2)*(norm2)*(norm2));
		star->F_y += distance_center_y*(-G*(quad->mass))/((norm2)*(norm2)*(norm2));
		printf("update F 2\n");
	}
	else if(quad->quadOne){
		forceCal(quad->quadOne, star);
		forceCal(quad->quadTwo, star);
		forceCal(quad->quadThree, star);
		forceCal(quad->quadFour, star);	
	}
}

void printQuad (quad_type* quad, int depth) {
	if (quad->star)
	{
		for (int i = 0; i < depth; ++i)
		{
			printf(".");
		}
	}
	if (quad->quadOne){
		printQuad(quad->quadOne, depth+1);
		printQuad(quad->quadTwo, depth+1);
		printQuad(quad->quadThree, depth+1);
		printQuad(quad->quadFour, depth+1);
	}
}

void addStar (quad_type* quad, star_t* star) {
	if (!(quad->quadOne)) {
		if(!(quad->star)){
			quad->star = star;
		}
		else{
			subDivide(quad);
			addStar(quad, star);
		}
	}
	else {
		if(star->pos_x >= quad->x) {
			if (star->pos_y >= quad->y) {
				addStar(quad->quadOne, star);
			}
			else if (star->pos_y < quad->y) {
				addStar(quad->quadFour, star);
			}
		}
		else if (star->pos_x < quad->x) {
			if (star->pos_y >= quad->y){
				addStar(quad->quadTwo, star);
			}	
			else if (star->pos_y < quad->y) {
				addStar(quad->quadThree, star);
			}
		}
	}
}

void drawQuad(quad_type* quad){
	DrawRectangle((float)((quad->x)-(quad->w/2)), ((quad->y)-(quad->w/2)), 1, 1, (float)(quad->w), (float)(quad->w), 0.5);
	if ((quad->quadOne)){
		drawQuad((quad)->quadOne);
		drawQuad((quad)->quadTwo);
		drawQuad((quad)->quadThree);
		drawQuad((quad)->quadFour);
	}
}

void clearQuad(quad_type* quad){
	if (quad->quadOne)
	{
		clearQuad(quad->quadOne);
		clearQuad(quad->quadTwo);
		clearQuad(quad->quadThree);
		clearQuad(quad->quadFour);
	}
	free(quad);
	quad = NULL;
}

void *forceCaller(void* threadargs){
	struct thread_data *my_data;
	my_data = (struct thread_data*) threadargs;
	int startIndex = my_data->startIndex;
	int numOfIter = my_data->numOfIter;

	for (int i = startIndex; i < (startIndex+numOfIter); ++i)
	{
		forceCal((my_data->quad), my_data->array[i]);
	}
	pthread_exit(NULL); 
}

int main(int argc, char *argv[]) {
	if (argc != 7)
	{
		printf("Not six input arguments!\n");
	}
	else {
		const int N = atoi(argv[1]);
		char* filename = argv[2];
		const int steps = atoi(argv[3]);
		dt = atof(argv[4]);
		theta_max = atof(argv[5]);
		int graphics = atoi(argv[6]);
		G = 100.0/(N);

		FILE* file1;
		file1 = fopen(filename, "r");

		star_t* starArray[N];

		size_t mem_block_size = 1;
		double temp;

		//Read input file
		for (int i = 0; i < N; ++i)								
		{
			(starArray[i]) = (star_t*)malloc(sizeof(star_t));
			temp = fread(&(starArray[i]->pos_x), sizeof(double), mem_block_size, file1);
			temp = fread(&(starArray[i]->pos_y), sizeof(double), mem_block_size, file1);
			temp = fread(&(starArray[i]->mass), sizeof(double), mem_block_size, file1);
			temp = fread(&(starArray[i]->vel_x), sizeof(double), mem_block_size, file1);
			temp = fread(&(starArray[i]->vel_y), sizeof(double), mem_block_size, file1);
			temp = fread(&(starArray[i]->brightness), sizeof(double), mem_block_size, file1);
			temp = temp+1;

			starArray[i]->F_x = 0;
			starArray[i]->F_y = 0;
		}
		
		fclose(file1);


	    if (graphics == 1)
	    {
			InitializeGraphics(argv[0],windowWidth,windowWidth);
			SetCAxes(0,1);
		}

		// Main time loop
		for (int k = 0; k < steps; ++k)
		{
			quad_type* rootitoot = (quad_type*)malloc(sizeof(quad_type));
			initQuad(rootitoot, 0.5, 0.5, 1);


		 	for (int i = 0; i < N; ++i)
		 	{
		 		addStar(rootitoot,starArray[i]);
		 	}
		 	quadMass(rootitoot);
		 	centerOfMass(rootitoot);

			pthread_t ptid;
			pthread_t ptid2;
			pthread_t ptid3;
			pthread_t ptid4;

			struct thread_data* threadargs = (thread_d*)malloc(sizeof(thread_d));
			threadargs->array = (star_t*) malloc(sizeof(star_t)*(N));
			threadargs->startIndex = 0;
			threadargs->numOfIter = N/4;
			threadargs->quad = rootitoot;

			struct thread_data* threadargs2 = (thread_d*)malloc(sizeof(thread_d));
			threadargs2->array = (star_t*) malloc(sizeof(star_t)*(N));
			threadargs2->startIndex = N/4;
			threadargs2->numOfIter = N/4;
			threadargs2->quad = rootitoot;

			struct thread_data* threadargs3 = (thread_d*)malloc(sizeof(thread_d));
			threadargs3->array = (star_t*) malloc(sizeof(star_t)*(N));
			threadargs3->startIndex = N/2;
			threadargs3->numOfIter = N/4;
			threadargs3->quad = rootitoot;

			struct thread_data* threadargs4 = (thread_d*)malloc(sizeof(thread_d)); 
			threadargs4->array = (star_t*) malloc(sizeof(star_t)*(N));
			threadargs4->startIndex = (N/4)*3;
			threadargs4->numOfIter = N/4;
			threadargs4->quad = rootitoot;

			for (int i = 0; i < N; ++i)
			{
				threadargs->array[i] = starArray[i];
				threadargs2->array[i] = starArray[i];
				threadargs3->array[i] = starArray[i];
				threadargs4->array[i] = starArray[i];
			}

			pthread_create(&ptid, NULL, &forceCaller, (void* ) threadargs);
			pthread_create(&ptid2, NULL, &forceCaller, (void* ) threadargs2);
			pthread_create(&ptid3, NULL, &forceCaller, (void* ) threadargs3);
			pthread_create(&ptid4, NULL, &forceCaller, (void* ) threadargs4);

			pthread_join(ptid, NULL); 
			pthread_join(ptid2, NULL); 
			pthread_join(ptid3, NULL); 
			pthread_join(ptid4, NULL);

		    for (int i = 0; i < N; ++i)
		    {
				starArray[i]->vel_x += starArray[i]->F_x*dt;
				starArray[i]->vel_y += starArray[i]->F_y*dt;

				starArray[i]->pos_x += starArray[i]->vel_x *dt;
				starArray[i]->pos_y += starArray[i]->vel_y *dt;

				starArray[i]->F_x = 0;
				starArray[i]->F_y = 0;
		    }

		    if (graphics == 1)
		    {
			    ClearScreen();
			    drawQuad(rootitoot);

			    for (int i = 0; i < N; ++i){
			    	DrawCircle((*starArray[i]).pos_x, (*starArray[i]).pos_y, L, W, circleRadius, circleColor);
			    }
			    Refresh();
			    usleep(3000);
		    }
			clearQuad(rootitoot);
		}

		FILE* file2;
		file2 = fopen("result.gal", "w");

		for (int i = 0; i < N; ++i)
		{
			fwrite(&(starArray[i]->pos_x), 1, sizeof(double), file2);
			fwrite(&(starArray[i]->pos_y), 1, sizeof(double), file2);
			fwrite(&(starArray[i]->mass), 1, sizeof(double), file2);
			fwrite(&(starArray[i]->vel_x), 1, sizeof(double), file2);
			fwrite(&(starArray[i]->vel_y), 1, sizeof(double), file2);
			fwrite(&(starArray[i]->brightness), 1, sizeof(double), file2);
		}

		fclose(file2);

		for (int i = 0; i < N; ++i)
		{
			free(starArray[i]);
		}
			
	}
}