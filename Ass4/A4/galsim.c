#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "graphics.h"

//remove tat we ave w and  in struct quad
//remove vectors for ods sake

typedef struct vector {
	double x;
	double y;
} vector_t;

typedef struct star {
	vector_t pos;
	vector_t vel;
	vector_t F;
	double mass;
	double brightness;
} star_t;

typedef struct quad {
	double x;
	double y;
	double w;
	double h;
	double mass;
	vector_t center;

	struct quad* quadOne;
	struct quad* quadTwo;
	struct quad* quadThree;
	struct quad* quadFour;
	
	star_t* star;
} quad_type;

int N;
double dt;
double theta_max;
double epsilon = 0.001;

int fsize(FILE *fp){
    int prev=ftell(fp);
    fseek(fp, 0L, SEEK_END);
    int sz=ftell(fp);
    fseek(fp,prev,SEEK_SET); //go back to where we were
    return sz;
}

void initStar (star_t** star, double xPos, double yPos, double xVel, double yVel, double mass,double brightness) {
	(*star) = (star_t*)malloc(sizeof(star_t));
	(*star)->pos.x = xPos;
	(*star)->pos.y = yPos;
	(*star)->vel.x = xVel;
	(*star)->vel.y = yVel;
	(*star)->mass = mass;
	(*star)->brightness = brightness;
	(*star)->F.x = 0;
	(*star)->F.y = 0;
}

void initQuad (quad_type* quad, double x, double y, double w, double h){

	quad->x = x;
	quad->y = y;
	quad->w = w;
	quad->h = h;

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
	double yplusmargin = quad->y+(quad->h/4); 
	double yminusmargin = quad->y-(quad->h/4);

	//ndra detta så att vi inte mallocas för de kvadranterna som inte ar stjrnor
	quad->quadOne = (quad_type*)malloc(sizeof(quad_type));
	quad->quadTwo = (quad_type*)malloc(sizeof(quad_type));
	quad->quadThree = (quad_type*)malloc(sizeof(quad_type));
	quad->quadFour = (quad_type*)malloc(sizeof(quad_type));

	initQuad(quad->quadOne, xplusmargin, yplusmargin , (quad->w/2), (quad->h/2));
	initQuad(quad->quadTwo, xminusmargin, yplusmargin , (quad->w/2), (quad->h/2));
	initQuad(quad->quadThree, xminusmargin, yminusmargin, (quad->w/2), (quad->h/2));
	initQuad(quad->quadFour, xplusmargin, yminusmargin, (quad->w/2), (quad->h/2));

	//What if star is on the border, dun dun dun, done without thinking!
	if(quad->star->pos.x >= quad->x) {
		if (quad->star->pos.y >= quad->y) {
			quad->quadOne->star = quad->star;
		}
		else if (quad->star->pos.y < quad->y) {
			quad->quadFour->star = quad->star;
		}
	}
	else if (quad->star->pos.x < quad->x) {
		if (quad->star->pos.y >= quad->y){
			quad->quadTwo->star = quad->star;
		}	
		else if (quad->star->pos.y < quad->y) {
			quad->quadThree->star = quad->star;
		}
	}
	quad->star = NULL;

}

void centerOfMass(quad_type* quad){
	if(!(quad->quadOne)){
		if(quad->star){
	 		quad->center = quad->star->pos;
		} else {
			quad->center.x = quad->x;
			quad->center.y = quad->y;
		}
	}
	else if(quad->quadOne){
		centerOfMass(quad->quadOne);
		centerOfMass(quad->quadTwo);
		centerOfMass(quad->quadThree);
		centerOfMass(quad->quadFour);

		//divide by total mass?
		quad->center.x = ((quad->quadOne->mass*quad->quadOne->center.x + quad->quadTwo->mass*quad->quadTwo->center.x + quad->quadThree->mass*quad->quadThree->center.x + quad->quadFour->mass*quad->quadFour->center.x)/(quad->mass));

		quad->center.y = ((quad->quadOne->mass*quad->quadOne->center.y + quad->quadTwo->mass*quad->quadTwo->center.y + quad->quadThree->mass*quad->quadThree->center.y + quad->quadFour->mass*quad->quadFour->center.y)/(quad->mass));

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
	
	//distance to center of box
	//accordin to wiki it is center of mass not of box
	// vector_t distance;
	// distance.x = star->pos.x - quad->x;
	// distance.y = star->pos.y - quad->y;

	// double norm = sqrt((distance.x)*(distance.x) + (distance.y)*(distance.y)) + epsilon;

	//distance to center of mass of quad
	vector_t distance_center;
	distance_center.x = star->pos.x - quad->center.x;
	distance_center.y = star->pos.y - quad->center.y;

	double norm2 = sqrt((distance_center.x)*(distance_center.x) + (distance_center.y)*(distance_center.y)) + epsilon;
	
	theta = quad->w/norm2;

	if(quad->star){
		star->F.x += distance_center.x*(-G*(quad->mass))/((norm2)*(norm2)*(norm2));
		star->F.y += distance_center.y*(-G*(quad->mass))/((norm2)*(norm2)*(norm2));
	}
	else if(theta <= theta_max){
		star->F.x += distance_center.x*(-G*(quad->mass))/((norm2)*(norm2)*(norm2));
		star->F.y += distance_center.y*(-G*(quad->mass))/((norm2)*(norm2)*(norm2));
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
		if(star->pos.x >= quad->x) {
			if (star->pos.y >= quad->y) {
				addStar(quad->quadOne, star);
			}
			else if (star->pos.y < quad->y) {
				addStar(quad->quadFour, star);
			}
		}
		else if (star->pos.x < quad->x) {
			if (star->pos.y >= quad->y){
				addStar(quad->quadTwo, star);
			}	
			else if (star->pos.y < quad->y) {
				addStar(quad->quadThree, star);
			}
		}
	}
}

const float circleRadius=0.0025, circleColor=0;
const int windowWidth=800;

void drawQuad(quad_type* quad){
	DrawRectangle((float)((quad->x)-(quad->w/2)), ((quad->y)-(quad->h/2)), 1, 1, (float)(quad->w), (float)(quad->h), 0.5);
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


//-----------------
//-----------------
//-----------------
//-----------------
//-----------------
//-----------------
//-----------------
//-----------------
//-----------------
//-----------------
//-----------------
//-----------------
//-----------------
//-----------------
//-----------------
//-----------------
//-----------------
//-----------------
//-----------------
//-----------------
//-----------------



int main(int argc, char *argv[]) {
	if (argc != 7)
	{
		printf("Not six input arguments!\n");
	}
	else {

		char* filename = argv[2];
		int nsteps = atoi(argv[3]);
		double delta_t = atof(argv[4]);
		theta_max = atof(argv[5]);
		int graphics = atoi(argv[6]);

		FILE* file1;
		file1 = fopen(filename, "r");
		N = fsize(file1)/6/sizeof(double);
		star_t* starArray[N];

		dt = delta_t;

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


		//Calculate forces
		//Time loop
		for (int k = 0; k < nsteps; ++k)
		{
			quad_type* rootitoot = (quad_type*)malloc(sizeof(quad_type));
			initQuad(rootitoot, 0.5, 0.5, 1, 1);


		 	for (int i = 0; i < N; ++i)
		 	{
		 		addStar(rootitoot,starArray[i]);
		 	}
		 	quadMass(rootitoot);
		 	centerOfMass(rootitoot);

		    for (int i = 0; i < N; ++i){
				// vector_t distance;
				// double G = 100.0/N;
				// double norm;
				// double con;
				// double epsilon = 0.001;

				forceCal(rootitoot, starArray[i]);

				// for (int j = i; j < N; ++j)
				// {
				// 	distance.x = starArray[i]->pos.x - starArray[j]->pos.x;
				// 	distance.y = starArray[i]->pos.y - starArray[j]->pos.y;
				// 	norm = sqrt((distance.x)*(distance.x) + (distance.y)*(distance.y)) + epsilon;
				// 	con = (-G*(starArray[i]->mass)*(starArray[j]->mass))/((norm)*(norm)*(norm));

				// 	starArray[i]->F.x += distance.x*con;
				// 	starArray[i]->F.y += distance.y*con;

				// 	starArray[j]->F.x += -distance.x*con;
				// 	starArray[j]->F.y += -distance.y*con;


				// }
		    }

		    for (int i = 0; i < N; ++i)
		    {
				vector_t acc;

				//printf("%lf\n", starArray[i]->F.x);

				// acc.x = starArray[i]->F.x/starArray[i]->mass;
				// acc.y = starArray[i]->F.y/starArray[i]->mass;

				acc.x = starArray[i]->F.x;
				acc.y = starArray[i]->F.y;				

				starArray[i]->vel.x += acc.x*dt;
				starArray[i]->vel.y += acc.y*dt;

				starArray[i]->pos.x += starArray[i]->vel.x *dt;
				starArray[i]->pos.y += starArray[i]->vel.y *dt;

				starArray[i]->F.x = 0;
				starArray[i]->F.y = 0;
		    }

		    if (graphics == 1)
		    {
			    ClearScreen();
			    drawQuad(rootitoot);

			    for (int i = 0; i < N; ++i){
			    	DrawCircle((*starArray[i]).pos.x, (*starArray[i]).pos.y, L, W, circleRadius, circleColor);
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
			fwrite(&(starArray[i]->pos.x), 1, sizeof(double), file2);
			fwrite(&(starArray[i]->pos.y), 1, sizeof(double), file2);
			fwrite(&(starArray[i]->mass), 1, sizeof(double), file2);
			fwrite(&(starArray[i]->vel.x), 1, sizeof(double), file2);
			fwrite(&(starArray[i]->vel.y), 1, sizeof(double), file2);
			fwrite(&(starArray[i]->brightness), 1, sizeof(double), file2);
		}

		fclose(file2);

		for (int i = 0; i < N; ++i)
		{
			free(starArray[i]);
		}
			
	}
}