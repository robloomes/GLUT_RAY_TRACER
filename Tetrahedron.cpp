/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Tetrahedron class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Tetrahedron.h"
#include <math.h>


/**
* Checks if a point pt is inside the current polygon
* Implement a point inclusion test using 
* member variables a, b, and c..
*/
bool Tetrahedron::isInside(glm::vec3 pt)
{
	glm::vec3 n = normal(pt);
	
	glm::vec3 x_a = glm::cross(b-a, pt-a), x_b = glm::cross(c-b, pt-b), x_c = glm::cross(a-c, pt-c);

	if (0 <= glm::dot(x_a, n) && 0 <= glm::dot(x_b, n) &&
	    0 <= glm::dot(x_c, n))
	{
		return true;
	}
	else {
		return false;
	}	
}


/**
* Tetrahedron's intersection method.  The input is a ray (pos, dir). 
*/
float Tetrahedron::intersect(glm::vec3 posn, glm::vec3 dir)
{
	glm::vec3 n = normal(posn);
	glm::vec3 vdif = a - posn;
	float vdotn = glm::dot(dir, n);
	if (fabs(vdotn) < 1.e-4) return -1;
    float t = glm::dot(vdif, n)/vdotn;
    if (fabs(t) < 0.0001) return -1;
	glm::vec3 q = posn + dir*t;
	if (isInside(q)) return t;
    else return -1;
}

/**
* Returns the unit normal vector at a given point.
* Compute the tetrahedrons's normal vector using 
* member variables a, b, and c.
* The parameter pt is a dummy variable and is not used.
*/
glm::vec3 Tetrahedron::normal(glm::vec3 pt)
{
	glm::vec3 n = glm::cross(b-a, c-a);
	glm::vec3 normalized = glm::normalize(n);
	return normalized;
}
