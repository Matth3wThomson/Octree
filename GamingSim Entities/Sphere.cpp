#include "Sphere.h"

Sphere::Sphere(const Vector3& position, const float& radius, const float& mass, float drag, float elasticity){
	this->position = position;
	this->lastPos = position;
	this->radius = abs(radius);
	this->mass = mass;

	if (drag > 1.0f) drag = 1.0f; // Drag should not be greater than 1
	if (drag < 0.0f) drag = 0.0f; // Drag should not be less than 0

	this->drag = drag;

	if (elasticity > 1.0f) elasticity = 1.0f; //Elasticity should not be greater than 1
	if (elasticity < 0.0f) elasticity = 0.0f; //Elasticity should not be less than 0

	this->elasticity = elasticity;

	//Spheres are green spheres!
	ro = new RenderObject(MeshManager::Instance().GetMesh("sphere2.obj"), ShaderManager::Instance().GetShader("basic"), TextureManager::Instance().GetTexture("green.png"));
}

//THIS NEEDS TESTING MAJORLY
void Sphere::ResolveCollision(Sphere& rhs, const float& time){
	
	//Calculate the depth of the penetration
	float penDepth = this->radius + rhs.radius - this->position.GetDistance(rhs.position);

	//Calculate the contact normal
	Vector3 conNormal = (this->position - rhs.position).GetNormalised();

	//Calculate the point of contact
	Vector3 conPoint = this->position - conNormal * (this->radius - penDepth);

	//Calculate the rough combined elasticity of the two spheres in
	//the collision. An application of the smoke and mirrors technique!
	float elasticity = (this->elasticity + rhs.elasticity) * 0.5f;
	
	//Calculate the velocities of both spheres
	Vector3 va = this->getVelocity(time);
	Vector3 vb = rhs.getVelocity(time);

	//Calculate the Vn of the collision
	float veloNormal = (va - vb).DotProduct(conNormal);

	//Calculate the impulse of the collision
	float impulse = ( -(1 + elasticity) * veloNormal ) / conNormal.DotProduct(conNormal * ( (1 / this->mass) + (1 / rhs.mass) ));
	
	//Calculate the resultant velocties of the collision
	Vector3 vaAfter = va + ((impulse/this->mass) * conNormal);
	Vector3 vbAfter = vb - ((impulse/rhs.mass) * conNormal);

	//Translate the shapes out of contact with each other
	this->translate(conNormal * (penDepth*0.5f));
	rhs.translate(0.0f - (conNormal * (penDepth*0.5f)));

	//Apply the calculated velocties to them
	this->setVelocity(vaAfter, time);
	rhs.setVelocity(vbAfter, time);
}
