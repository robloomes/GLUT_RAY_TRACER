/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The sphere class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#ifndef H_SPHERE
#define H_SPHERE
#include <glm/glm.hpp>
#include "SceneObject.h"


class Sphere : public SceneObject
{
private:
    glm::vec3 a; //centre of sphere
    float b; //radius of sphere

public:
	Sphere(void): a(glm::vec3(0)), b(1)  //Default constructor creates a unit sphere
	{
		color = glm::vec3(1);
	}

    Sphere(glm::vec3 centre, float radius, glm::vec3 col): a(centre), b(radius)
	{
		color = col;
	};

	float intersect(glm::vec3 posn, glm::vec3 dir);

	glm::vec3 normal(glm::vec3 p);

};

#endif //!H_SPHERE
