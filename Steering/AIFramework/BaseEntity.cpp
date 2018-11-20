#include "BaseEntity.h"
#include "UtilRandom.h"

std::vector<BaseEntity*> BaseEntity::Renderables;

BaseEntity::BaseEntity()
{
	// with no further information, we make some assumptions and set default values.
	filename = "assets\\boid.png";
	colourTint = sf::Color::White;
	Initialize();
}

BaseEntity::BaseEntity(std::string file) : filename(file)
{
	// with just a texture file, we default to a white tint (so no tint at all).
	colourTint = sf::Color::White;
	Initialize();
}

BaseEntity::BaseEntity(std::string file, sf::Color colour) : filename(file), colourTint(colour)
{
	Initialize();
}

BaseEntity::~BaseEntity()
{
	
}

void BaseEntity::Think()
{
	const float MAX_NEIGHBOUR_DISTANCE = 250.0f;

	// build a new position vector by adding a scaled version of the velocity vector
	sf::Vector2f pos = getPosition() + (velocity * 0.1f);
	// update our position
	setPosition(pos);

	std::vector<BaseEntity*> neighbours;

	for (auto entity : BaseEntity::Renderables) {
		if (entity == this) continue;

		auto distVec = getPosition() - entity->getPosition();
		float distance = std::sqrtf(distVec.x * distVec.x + distVec.y * distVec.y);

		if (distance > MAX_NEIGHBOUR_DISTANCE) continue;

		neighbours.push_back(entity);
	}

	
	sf::Vector2f mediumVelocity;
	sf::Vector2f mediumPosition;

	for (auto neighbour : neighbours) {
		if (std::sqrt(std::pow(neighbour->getOrigin().x - this->getOrigin().x, 2) + std::pow(neighbour->getOrigin().y - this->getOrigin().y, 2)))
		{
			sf::Vector2f weight = neighbour->getPosition() - this->getPosition();
			weight.x = weight.x / std::sqrt(weight.x * weight.x + weight.y * weight.y);
			weight.y = weight.y / std::sqrt(weight.x * weight.x + weight.y * weight.y);
			weight = weight / std::sqrt(std::pow(neighbour->getOrigin().x - this->getOrigin().x, 2 + std::pow(neighbour->getOrigin().y - this->getOrigin().y, 2)));
			mediumVelocity += neighbour->getVelocity() + weight;
			mediumPosition += neighbour->getPosition() + weight;
		}
	}
}

void BaseEntity::Initialize()
{
	// set a default position - could be randomised.
	setPosition(30.0f, 30.0f);

	// load the texture into memory
	texture.loadFromFile(filename);
	texture.setSmooth(true);
	// and provide the sprite with a pointer to the texture object
	// if the texture object is destroyed (goes out of scope etc) then the sprite will display weirdly
	sprite.setTexture(texture);
	// set up our colour tint
	sprite.setColor(colourTint);
	
	// select a random angle
	float angle = UtilRandom::instance()->GetRange(0, 361);
	// set our rotation value
	setRotation(angle);
	// and assign a velocity, we need to convert angle to radians so it plays nicely with cos and sin.
	velocity = sf::Vector2f(cos(angle * M_PI / 180), sin(angle * M_PI / 180));

	// add the entity to the list of renderables.
	Renderables.push_back(this);
}