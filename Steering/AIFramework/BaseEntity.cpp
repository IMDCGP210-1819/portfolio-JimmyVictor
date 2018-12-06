#include "BaseEntity.h"
#include "UtilRandom.h"

std::vector<BaseEntity*> BaseEntity::Renderables;

BaseEntity::BaseEntity()
{
	// with no further information, we make some assumptions and set default values.
	filename = "assets\\boid.png";
	colourTint = sf::Color::Green;
	Initialize();
}

BaseEntity::BaseEntity(std::string file) : filename(file)
{
	// with just a texture file, we default to a white tint (so no tint at all).
	colourTint = sf::Color::Green;
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

	int neighbourCount = 0;

	sf::Vector2f mediumVelocity;
	sf::Vector2f mediumPosition;
	sf::Vector2f separationV;

	const float MAX_NEIGHBOUR_DISTANCE = 100.0f;

	// build a new position vector by adding a scaled version of the velocity vector
	sf::Vector2f pos = getPosition() + (velocity * 0.08f);
	// update our position
	setPosition(pos);

	std::vector<BaseEntity*> neighbours;

	for (auto neighbour : BaseEntity::Renderables) 
	{
		if (neighbour == this) continue;

		auto distVec = getPosition() - neighbour->getPosition();
		float distance = std::sqrt(distVec.x * distVec.x + distVec.y * distVec.y);

		if (distance > MAX_NEIGHBOUR_DISTANCE) continue;

		neighbours.push_back(neighbour);
	}


	if (neighbourCount > 0)
	{
		separationV = separationV / (float)neighbourCount;
		//normalizing
		separationV = separationV / std::sqrt(separationV.x * separationV.x + separationV.y * separationV.y);


		mediumVelocity = mediumVelocity / (float)neighbourCount;
		//normalizing
		mediumVelocity = mediumVelocity / abs(sqrt(mediumVelocity.x * mediumVelocity.x + mediumVelocity.y * mediumVelocity.y));

		mediumPosition = mediumPosition / (float)neighbourCount;
		mediumPosition = mediumPosition - getPosition();
		//normalizing
		mediumPosition = mediumPosition / abs(sqrt(mediumPosition.x * mediumPosition.x + mediumPosition.y * mediumPosition.y));
	}



	//Make it so that the BOIDs come out the opposite side of the screen once they hit a border
	//this is so that I don't have to make the walls act like neighbours and just for ease of testing
	if (getPosition().x <= 0)
	{
		setPosition(sf::Vector2f(800.0f, getPosition().y));
	}
	else if (getPosition().x >= 800.0f)
	{
		setPosition(sf::Vector2f(0.0f, getPosition().y));
	}

	if (getPosition().y <= 0)
	{
		setPosition(sf::Vector2f(getPosition().x, 600.0f));
	}
	else if (getPosition().y >= 600.0f)
	{
		setPosition(sf::Vector2f(getPosition().x, 0.0f));
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