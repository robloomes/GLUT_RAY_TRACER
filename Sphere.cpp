/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The sphere class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Sphere.h"
#include <math.h>

/**
* Sphere's intersection method.  The input is a ray (pos, dir). 
*/
float Sphere::intersect(glm::vec3 posn, glm::vec3 dir)
{
    glm::vec3 vdif = posn-a;
    float length = glm::length(vdif);
    float i = (length*length) - b*b;
    float dot_x = glm::dot(dir, vdif);
    float d = (dot_x*dot_x) - i; //delta
	float t1 = -dot_x - sqrt(d);
    float t2 = -dot_x + sqrt(d);
	if(0.001 > fabs(d)) return -1.0; 
    if(0 > d) return -1.0;
	if(0.001 > fabs(t1))
    {
        if (0 < t2) return t2;
        else t1 = -1.0;
    }
    (fabs(t2) < 0.001) ? t2 = -1.0: t2 = t2;
	return (t1 < t2)? t1: t2;
}

/**
* Returns the unit normal vector at a given point.
*/
glm::vec3 Sphere::normal(glm::vec3 pt)
{
    glm::vec3 n = pt-a;
	glm::vec3 normalized = glm::normalize(n);
	return normalized;
}
