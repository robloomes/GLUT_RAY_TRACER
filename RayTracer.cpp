/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The ray tracer class
*  the main kernel for to organise and draw other objects
-------------------------------------------------------------*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include "Tetrahedron.h"
#include "Plane.h"
#include "TextureBMP.h"
#include <GL/glut.h>
using namespace std;

const float WIDTH = 20.0;
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 500;                 
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;

vector<SceneObject*> sceneObjects; //A global list containing pointers to objects in the scene

//textures to display
TextureBMP txId[3];
//plane coords
float y = 20, z = -150, x = 25; 

//---The most important function in a ray tracer! ----------------------------------                                                                                                                            
//   Computes the colour value obtained by tracing a ray and finding its                                                                                                                                        
//     closest point of intersection with objects in the scene.                                                                                                                                                 
//----------------------------------------------------------------------------------  
glm::vec3 trace(Ray ray, int step) {
	const float shader_phong = 10;
	
	//inits
	glm::vec3 backgroundCol(0);
	glm::vec3 light_1 (24, -19, -49);
	glm::vec3 light_2 (-24, -19, -49);
	glm::vec3 spec(1, 1, 1);
	glm::vec3 ambi(0.2); 
	glm::vec3 col, vect_one, vect_two, col_sum, normalized_vect, col_one, col_two, reflect_one, reflect_two;
	float light_val_one, light_val_two;
	
	float pt_t = 0;
	float pt_s = 0;	
	
	float pt_dot_one, pt_dot_two, dot_reflect_one, dot_reflect_two;
	float pt_spec_one, pt_spec_two;
	float val;

	ray.closestPt(sceneObjects); //Compute the closest point of intersetion of objects with the ray
	
    if (ray.xindex == -1) return backgroundCol;
	else if (ray.xindex == 3) {
		pt_t = (ray.xpt.x-(-x))/(x-(-x));
		pt_s = (ray.xpt.z-(-x))/(z-(-x));
		col = ((txId[2].getColorAt(pt_s, pt_t)) * 1.0f) + col;
	} else if (ray.xindex == 5) {
		pt_s = (ray.xpt.x-(-x))/(z-(-x));
		pt_t = (ray.xpt.z-(-y))/(y-(-y));
		col = ((txId[1].getColorAt(pt_s, pt_t)) * 0.5f) + col;
	} else if (ray.xindex == 6) {
		pt_s = (ray.xpt.z-(-x))/(z-(-x));
		pt_t = (ray.xpt.y-(-y+10))/(y-(-y+10));
		col = ((txId[1].getColorAt(pt_s, pt_t)) * 0.5f) + col;
	} else if (ray.xindex == 7) {
		pt_s = (ray.xpt.z-z)/((-x)-z);
		pt_t = (ray.xpt.y-(-y))/(y-(-y));
		col = ((txId[1].getColorAt(pt_s, pt_t)) * 0.5f) + col;
	}
	
	normalized_vect = sceneObjects[ray.xindex]->normal(ray.xpt);
	
	col_one = sceneObjects[ray.xindex]->getColor();
    col_one = sceneObjects[ray.xindex]->getColor();
	
	if (ray.xindex == 2) {
		pt_s = (asin(normalized_vect.x)/M_PI)+0.5;
		pt_t = (asin(normalized_vect.y)/M_PI)+0.5;
		col = col + txId[0].getColorAt(pt_s, pt_t);	
	}
	
	
	//vector inits
	vect_one = light_1-ray.xpt;
	vect_two = light_2-ray.xpt;
	light_val_one = glm::length(vect_one);
	light_val_two = glm::length(vect_two);
	
	vect_one = glm::normalize(vect_one);
	vect_two = glm::normalize(vect_two);
	
	reflect_one = glm::reflect(-vect_one, normalized_vect);
	reflect_two = glm::reflect(-vect_two, normalized_vect);
	
	dot_reflect_one = glm::dot(reflect_one, normalized_vect);
	dot_reflect_two = glm::dot(reflect_two, normalized_vect);
	
	pt_dot_one = glm::dot(vect_one, normalized_vect);
	pt_dot_two = glm::dot(vect_two, normalized_vect);
	
	Ray x_shadow(ray.xpt, vect_one);
	Ray y_shadow(ray.xpt, vect_two);
	
	x_shadow.closestPt(sceneObjects);
	y_shadow.closestPt(sceneObjects);
	
	glm::vec3 val_relfect_dir;
	glm::vec3 col_reflection;
	glm::vec3 x;
	glm::vec3 y;
	glm::vec3 z;
	glm::vec3 col_refracted;
	
	if ((0 >= pt_dot_one) || (-1 < x_shadow.xindex && x_shadow.xdist < light_val_one)) {
		col_one = col_one*ambi;
	} else if (0 > dot_reflect_one) {
		    pt_spec_one = 0;
		    col_one = ((ambi + pt_dot_one) * col_one) + col + spec * pt_spec_one;
	} else {
		pt_spec_one = pow(dot_reflect_one, shader_phong);
		col_one = ((ambi + pt_dot_one) * col_one) + col + spec * pt_spec_one;
	}	
	
	if (0 >= pt_dot_two || (-1 < y_shadow.xindex && y_shadow.xdist < light_val_two)) {
		col_two = col_two*ambi;
	}else if (dot_reflect_two < 0) {
		    pt_spec_two = 0;
		    col_two = ((ambi + pt_dot_two) * col_two) + col + spec * pt_spec_two;
	} else {
			pt_spec_two = pow(dot_reflect_two, shader_phong);
			col_two = ((ambi + pt_dot_two) * col_two) + col + spec * pt_spec_two;
	}
		
	col_sum += 0.5f*(col_one + col_two);
	
	
	if ((MAX_STEPS > step) && (ray.xindex == 0 || (4 < ray.xindex && 8 > ray.xindex))) {
		
		val_relfect_dir = glm::reflect(ray.dir, normalized_vect);
		
		Ray reflected(ray.xpt,val_relfect_dir);
		col_reflection = trace(reflected, step+1);
		
		col_sum += (col_reflection * 0.8f);
	}else if ((MAX_STEPS > step) &&  (ray.xindex == 1)) {
		val = 1.003;
		x = glm::refract(ray.dir, normalized_vect, 1.0f/val);
		
		Ray reflected_1(ray.xpt, x);
		reflected_1.closestPt(sceneObjects);
		
		y = sceneObjects[reflected_1.xindex]->normal(reflected_1.xpt);
		z = glm::refract(x, -y, val);	
		Ray reflected_2(reflected_1.xpt, z);
		
		col_refracted = trace(reflected_2, step + 1);
		col_sum += col_refracted * (glm::vec3(0.99));
	}	
	
	return col_sum;
}


void anti_alias(glm::vec3 i, float cell_xy, float x, float y)
{
	glm::vec3 col, col_1, col_2, col_3, col_4;
	Ray ray_1, ray_2, ray_3, ray_4;
	
	glm::vec3 dir_1(x + 0.25 * cell_xy,
	               y + 0.25 * cell_xy, -EDIST);
	
	col_1 = trace(ray_1, 1); 
	glm::vec3 dir_2(x + 0.25 * cell_xy, 
	               y + 0.75 * cell_xy, -EDIST);

	col_1 = trace(ray_2, 1); 
	glm::vec3 dir_3(x + 0.75 * cell_xy, 
	               y + 0.25 * cell_xy, -EDIST);

	col_3 = trace(ray_3, 1); 
	glm::vec3 dir_4(x + 0.75 * cell_xy, 
	               y + 0.75 * cell_xy, -EDIST);

	ray_1 = Ray(i, dir_1);
	ray_2 = Ray(i, dir_2);
	ray_3 = Ray(i, dir_3);
	ray_4 = Ray(i, dir_4);
	
	ray_1.normalize();
	ray_2.normalize();
	ray_3.normalize();
	ray_4.normalize();
	
	col_4 = trace(ray_4, 1); 
	col.r = (col_1.r + col_2.r + col_3.r + col_4.r)/4;
	col.g = (col_1.g + col_2.g + col_3.g + col_4.g)/4;
	col.b = (col_1.b + col_2.b + col_3.b + col_4.b)/4;
	
	glColor3f(col.r, col.g, col.b);
}


//---The main display module -----------------------------------------------------------                                                                                                                        
// In a ray tracing application, it just displays the ray traced image by drawing                                                                                                                               
// each cell as a quad.                                                                                                                                                                                         
//--------------------------------------------------------------------------------------- 
void display()
{
	float xp, yp; //grid point
	float cellX = (XMAX-XMIN)/NUMDIV; //cell width
	float cellY = (YMAX-YMIN)/NUMDIV; //cell height
	
	glm::vec3 eye(0., 0., 0.); //The eye position (source of primary rays) is the origin
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glBegin(GL_QUADS);
    
    for(int i = 0; i < NUMDIV; i++) //Each cell is a quad
	{
		xp = XMIN + i*cellX;
		for(int j = 0; j < NUMDIV; j++) //For each grid point xp, yp  
		{
			yp = YMIN + j*cellY;
			
			anti_alias(eye, cellX, xp, yp);			
			glVertex2f(xp, yp);
			glVertex2f(xp+cellX, yp);
			glVertex2f(xp+cellX, yp+cellY);
			glVertex2f(xp, yp+cellY);
        }
    }
    
    glEnd();
    glFlush();
}

//---The cube display module -----------------------------------------------------------                                                                                                                        
// Draws various cubes in the display window                                                                                                                                                                                       
//--------------------------------------------------------------------------------------- - 
void cube() {
	//inits
	Plane *side1;
	Plane *side2;
	Plane *side3;
	Plane *side4;
	Plane *side5;
	Plane *side6;
	
	float min_x = -20.0;
	float max_x = -12.5;
	float min_y = -17.5;
	float max_y = -10.0;
	float min_z = -90.0;
	float max_z = -97.5;
	
	glm::vec3 col = glm::vec3(0.1, 0.8, 0.1); 
	
	side1 = new Plane(glm::vec3(min_x, max_y, max_z), glm::vec3(min_x, max_y, min_z), glm::vec3(max_x, max_y, min_z), glm::vec3(max_x, max_y, max_z), col);
	sceneObjects.push_back(side1);
	
	side2 = new Plane(glm::vec3(min_x, min_y, min_z), glm::vec3(min_x, min_y, max_z), glm::vec3(max_x, min_y, max_z), glm::vec3(max_x, min_y, min_z), col);
	sceneObjects.push_back(side2);
	
	side3 = new Plane(glm::vec3(max_x, max_y, min_z), glm::vec3(max_x, min_y, min_z), glm::vec3(max_x, min_y, max_z), glm::vec3(max_x, max_y, max_z), col);
	sceneObjects.push_back(side3);
	
	side4 = new Plane(glm::vec3(min_x, max_y, max_z), glm::vec3(min_x, min_y, max_z), glm::vec3(min_x, min_y, min_z), glm::vec3(min_x, max_y, min_z), col);
	sceneObjects.push_back(side4);
	
	side5 = new Plane(glm::vec3(min_x, min_y, min_z), glm::vec3(max_x, min_y, min_z), glm::vec3(max_x, max_y, min_z), glm::vec3(min_x, max_y, min_z), col);
	sceneObjects.push_back(side5);
	
	side6 = new Plane(glm::vec3(min_x, max_y, max_z), glm::vec3(max_x, max_y, max_z), glm::vec3(max_x, min_y, max_z), glm::vec3(min_x, min_y, max_z), col);
	sceneObjects.push_back(side6);
}

//---The tetrahedron display module -----------------------------------------------------------                                                                                                                        
// Draws various tets in the display window                                                                                                                                                                                       
//--------------------------------------------------------------------------------------- 
void tetrahedron()
{
	Tetrahedron *tet1;
	Tetrahedron *tet2;
	Tetrahedron *tet3;
	Tetrahedron *tet4;
	
	tet1 = new Tetrahedron(glm::vec3(10, -15, -75), glm::vec3(0, -15, -75), glm::vec3(5, -5, -75), glm::vec3(0.0, 0.0, 255.0));
	sceneObjects.push_back(tet1);
	
	tet2 = new Tetrahedron(glm::vec3(5, -15, -65), glm::vec3(0, -15, -75), glm::vec3(5, -5, -75), glm::vec3(0.0, 0.0, 255.0));
	sceneObjects.push_back(tet2);
	
	tet3 = new Tetrahedron(glm::vec3(10, -15, -75), glm::vec3(5, -15, -65), glm::vec3(5, -5, -75), glm::vec3(0.0, 255.0, 0.0));
	sceneObjects.push_back(tet3);
	
	tet4 = new Tetrahedron(glm::vec3(10, -15, -75), glm::vec3(0, -15, -75), glm::vec3(5, -15, -65), glm::vec3(0.0, 255.0, 0.0));
	sceneObjects.push_back(tet4);
}

//---The sphere display module -----------------------------------------------------------                                                                                                                        
// Draws various sphere in the display window                                                                                                                                                                                       
//---------------------------------------------------------------------------------------   
void spheres() {
	Sphere *main_sphere;
	Sphere *planet;
	Sphere *transparent;
	
	//spheres
	main_sphere = new Sphere(glm::vec3(-5, -2, -115.0), 18.0, glm::vec3(0.350, 0.250, 0.550));
	sceneObjects.push_back(main_sphere); 
	
	planet = new Sphere(glm::vec3(2, -9.5, -60.0), 6.0, glm::vec3(0.5, 100.0, 0.5));
	sceneObjects.push_back(planet);
	
	transparent = new Sphere(glm::vec3(-5.0, 10.0, -60.0), 4.0, glm::vec3(0.6, 0.8, 1.));
	sceneObjects.push_back(transparent);	
}

//---The planes display module -----------------------------------------------------------                                                                                                                        
// Draws various planes in the display window                                                                                                                                                                                       
//---------------------------------------------------------------------------------------   
void planes() {
	//neutral colour
	glm::vec3 col = glm::vec3(0.5, 0.5, 0.5);
	//inits
	Plane *plane1;
	Plane *plane2;
	Plane *plane3;
	Plane *plane4;
	Plane *plane5;
	
	plane1 = new Plane(glm::vec3(-x, -y, -x), glm::vec3(x, -y, -x), glm::vec3(x, -y, z), glm::vec3(-x, -y, z), col);
	sceneObjects.push_back(plane1);
	
	plane2 = new Plane(glm::vec3(-x, -y, z), glm::vec3(x, -y, z), glm::vec3(x, y, z), glm::vec3(-x, y, z), col);
	sceneObjects.push_back(plane2);
	
	plane3 = new Plane(glm::vec3(-x, y, z), glm::vec3(x, y, z), glm::vec3(x, y, -x), glm::vec3(-x, y, -x), col);
	sceneObjects.push_back(plane3);
	
	plane4 = new Plane(glm::vec3(-x, -y, -x), glm::vec3(-x, -y, z), glm::vec3(-x, y, z), glm::vec3(-x, y, -x), col);
	sceneObjects.push_back(plane4);
	
	plane5 = new Plane(glm::vec3(x, -y, z), glm::vec3(x, y, z), glm::vec3(x, y, -x), glm::vec3(x, -y, -x), col);
	sceneObjects.push_back(plane5);
}

//---This function initializes the scene -------------------------------------------                                                                                                                            
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)                                                                                                                             
//     and add them to the list of scene objects.                                                                                                                                                               
//   It also initializes the OpenGL orthographc projection matrix for drawing the                                                                                                                               
//     the ray traced image.                                                                                                                                                                                    
//----------------------------------------------------------------------------------    
void initialize() {
	txId[0] = TextureBMP((char* const) "planet.bmp");
	txId[1] = TextureBMP((char* const) "wall.bmp");
	txId[2] = TextureBMP((char* const) "twin_peaks.bmp");
	glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
    glClearColor(0, 0, 0, 1);
    
    //objects init
	spheres();
	planes();
	cube();
	tetrahedron();
}

//main
int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Ray Tracer");  
    initialize();  
    glutDisplayFunc(display);
    
    
    glutMainLoop();
	return 0;
}
