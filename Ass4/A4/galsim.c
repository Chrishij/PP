#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "graphics.h"

int N;
int steps;
double dt;
double theta_max;
const double epsilon = 0.001;

const float circleRadius=0.0025, circleColor=0;
const int windowWidth=800;


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
	double x;
	double y;
	double w;
	double mass;
	double center_x;
	double center_y;

	struct quad* quadOne;
	struct quad* quadTwo;
	struct quad* quadThree;
	struct quad* quadFour;
	
	star_t* star;
} quad_type;

void initStar (star_t** star, double xPos, double yPos, double xVel, double yVel, double mass,double brightness) {
	(*star) = (star_t*)malloc(sizeof(star_t));
	(*star)->pos_x = xPos;
	(*star)->pos_y = yPos;
	(*star)->vel_x = xVel;
	(*star)->vel_y = yVel;
	(*star)->mass = mass;
	(*star)->brightness = brightness;
	(*star)->F_x = 0;
	(*star)->F_y = 0;
}

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
	double G = 100.0/N;

	double distance_center_x=0;
	double distance_center_y=0;

	distance_center_x = star->pos_x - quad->center_x;
	distance_center_y = star->pos_y - quad->center_y;

	double norm2 = sqrt((distance_center_x)*(distance_center_x) + (distance_center_y)*(distance_center_y)) + epsilon;
	
	theta = quad->w/norm2;

	if(quad->star){
		star->F_x += distance_center_x*(-G*(quad->mass))/((norm2)*(norm2)*(norm2));
		star->F_y += distance_center_y*(-G*(quad->mass))/((norm2)*(norm2)*(norm2));
	}
	else if(theta <= theta_max){
		star->F_x += distance_center_x*(-G*(quad->mass))/((norm2)*(norm2)*(norm2));
		star->F_y += distance_center_y*(-G*(quad->mass))/((norm2)*(norm2)*(norm2));
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

int main(int argc, char *argv[]) {
	if (argc != 7)
	{
		printf("Not six input arguments!\n");
	}
	else {
		N = atoi(argv[1]);
		char* filename = argv[2];
		steps = atoi(argv[3]);
		dt = atof(argv[4]);
		theta_max = atof(argv[5]);
		int graphics = atoi(argv[6]);

		FILE* file1;
		file1 = fopen(filename, "r");

		star_t* starArray[N];

		size_t mem_block_size = 1;
		double pos_x;
		double pos_y;
		double vel_x;
		double vel_y;
		double mass;
		double brightness;

		double temp;

		//Read input file
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

		    for (int i = 0; i < N; ++i){
				forceCal(rootitoot, starArray[i]);
		    }

		    for (int i = 0; i < N; ++i)
		    {
				double acc_x=0;
				double acc_y=0;

				acc_x = starArray[i]->F_x;
				acc_y = starArray[i]->F_y;				

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