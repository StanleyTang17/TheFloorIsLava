#include"gameobject/GameObject.h"

class Crate : public GameObject
{
public:
	Crate(glm::vec3 position)
		:
		GameObject(position)
	{
		this->set_graphic_model(ModelClass::Box, new ModelInstance(position, glm::vec3(0.0f), glm::vec3(1.0f)));
		this->set_collision_shape(new Collision::Box(&(this->position), &(this->velocity), 2.0f, 2.0f, 2.0f));
	}
	void update(const float dt)
	{
		this->get_model_instance()->set_position(this->position);
	}
};