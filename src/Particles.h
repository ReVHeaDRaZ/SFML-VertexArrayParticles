#include <stdlib.h>
#include <vector>
#include "Hud.h"
#include "RazVectorTools.h"

#define M_PI 3.14159265358979323846
#define MAX_NUM_PARTICLES 500000	  //Maximum number of particles that can exist
#define MAX_NUM_TEXTURES 5
#define ANGLE_UP (M_PI / 2.0f)		  //straight up (90deg)
#define FOUNTAIN_WIDTH (M_PI / 12.0f) //fountain width in radians (pi/12 is 15../deg)

extern uint numParticles;
extern bool steerBehaviour;

float maxSpeed = 20.f;
float maxForce = 0.2f;
sf::Vector2f wind = sf::Vector2f(0.f,0.f);

enum particleTypeEnum {SPARKS, FOUNTAIN, SLIME, FIRE, LARGE, MAX_PARTICLETYPE};
enum behaviourTypeEnum {SEEK, ARRIVE, MAX_BEHAVIOURTYPE};
uint8_t particleType = SPARKS;
uint8_t behaviourType = SEEK;

sf::Texture spriteTexture[MAX_NUM_TEXTURES];
bool LoadTextures();
void SwitchBehaviourType();
void SwitchParticleType();

bool LoadTextures()
{
	if (!spriteTexture[0].loadFromFile("content/Dot64x64.png"))
		return 0;
	if (!spriteTexture[1].loadFromFile("content/Star64x64.png"))
		return 0;
	if (!spriteTexture[2].loadFromFile("content/snowflake64x64.png"))
		return 0;
	if (!spriteTexture[3].loadFromFile("content/pentagram64x64.png"))
		return 0;
	if (!spriteTexture[4].loadFromFile("content/RaZLogo64x64.png"))
		return 0;
	return 1;
}

void SwitchBehaviourType()
{
	behaviourType++;
	if(behaviourType==MAX_BEHAVIOURTYPE)
		behaviourType = 0;

	switch(behaviourType)
	{
		case SEEK:
			behaviourtypeHud.setString("SeeK");
			break;
		case ARRIVE:
			behaviourtypeHud.setString("ArrivE");
			break;
		default :
			behaviourheadingHud.setString("None");
	}
}

void SwitchParticleType()
{
	particleType++;
	if(particleType==MAX_PARTICLETYPE)
		particleType = 0;
}


class Obstacle
{
	public:
	sf::Vector2f location;
	float radius;
	sf::Sprite obstacleSprite;

	Obstacle(sf::Vector2f _location, float _radius)
	{
		location = _location;
		radius = _radius;

		obstacleSprite.setTexture(spriteTexture[0]);
		obstacleSprite.setScale(radius/16,radius/16);
		obstacleSprite.setPosition(location);
		obstacleSprite.setOrigin(obstacleSprite.getLocalBounds().width/2 ,obstacleSprite.getLocalBounds().height/2);
		obstacleSprite.setColor(sf::Color((uint8_t)rand(),(uint8_t)rand(),(uint8_t)rand(),100));

	}

	void Draw(sf::RenderTarget& _rendertarget)
	{
		_rendertarget.draw(obstacleSprite);
	}
};

std::vector<Obstacle> obstacles;


class Particle
{
private:
	sf::RenderWindow& windowref;
	float gravity = 0.07f;
	float angle;
	float init_v;
	sf::Vector2f accel;
	sf::Vector2f seekForce;
	sf::Vector2f arriveForce;
	sf::Vector2f avoidForce;

public:
	uint8_t lifetime;
	float particleSize = 5.f;
	float x, y;
	sf::Vector2f velocity;

	bool active = false;
	uint8_t r = 255;
	uint8_t g;
	uint8_t b;
	sf::Color color;
	sf::Vector2f target;

	Particle(sf::RenderWindow& window) :
		windowref(window)
	{
	}

	void Init(int posx, int posy)
	{
		x = posx;
		y = posy;

		switch(particleType)
		{
			case FOUNTAIN:
				gravity = 0.07f;
				angle = RandomNumber(ANGLE_UP - FOUNTAIN_WIDTH / 2.0f, ANGLE_UP + FOUNTAIN_WIDTH / 2.0f);
				init_v = RandomNumber(2.5f, 8.f);
				velocity.y = 0 - sin(angle) * init_v;
				velocity.x = cos(angle) * init_v;
				particleSize = RandomNumber(1.f,7.f);
				r = rand() % 25;
				b = rand() % 255;
				g = r;
				lifetime = rand();
				break;

			case SPARKS:
				gravity = 0.06f;
				velocity.x = (RandomNumber(1.f, 2.5f)) - (RandomNumber(1.f, 2.5f));
				velocity.y = (RandomNumber(1.f, 2.5f)) - (RandomNumber(1.f, 2.5f));
				particleSize = RandomNumber(1.f,8.f);
				r 	= 255;
				b 	= rand() % 50;
				g	= 1+rand()%150;
				lifetime = 1+ rand() % 200;
				break;
			case SLIME:
				gravity = 0.1f;
				velocity.x = (RandomNumber(1.f, 4.5f)) - (RandomNumber(1.f, 4.5f));
				velocity.y = (RandomNumber(0.5f, 1.5f)) - (RandomNumber(1.f, 2.5f));
				particleSize = RandomNumber(1.f,35.f);
				r 	= 0;
				b 	= rand() % 20;
				g	= 10 + rand() % 90;
				lifetime = rand();
				break;
			case FIRE:
				gravity = 0.f;
				x+=RandomNumber(-5.f,5.f);
				velocity.x = (RandomNumber(0.f, 0.5f)) - (RandomNumber(0.f, 0.5f));
				velocity.y = 0.f - (RandomNumber(2.5f, 5.5f));
				particleSize = RandomNumber(2.f,20.f);
				r 	= 255;
				b 	= rand() % 50;
				g	= rand();
				lifetime = 1 + rand() % 80;
				break;
			case LARGE:
				gravity = 0.02f;
				x+=RandomNumber(-25.f,25.f);
				velocity.x = (RandomNumber(-5.f, 5.f));
				velocity.y = 0.f - (RandomNumber(2.5f, 5.5f));
				particleSize = RandomNumber(10.f,50.f);
				r 	= rand() % 150;
				b 	= rand() % 150;
				g	= rand() % 150;
				lifetime = 1 + rand() % 150;
				break;
			default:
				break;
		}


		active = true;
		color = sf::Color(r, g, b, lifetime);
	}

	// --------------------------STEERING BEHAVIOURS-------------------------------------
	void AddForce(sf::Vector2f force)
	{
		this->accel += force;
	}

	sf::Vector2f Seek(sf::Vector2f seektarget)
	{
		sf::Vector2f steer = sf::Vector2f(0.f, 0.f);
		sf::Vector2f position = sf::Vector2f(this->x, this->y);
		//float attractDist = 250.f;

		// Subtract location from target then normalize and multiply by maxSpeed
		sf::Vector2f desired = seektarget - position;
		desired = SetMagnitude(desired, maxSpeed);

		steer = desired - velocity;
		if(GetMagnitude(steer) > maxForce)
			steer = SetMagnitude(steer, maxForce);

		return steer;
	}

	sf::Vector2f Arrive(sf::Vector2f arrivetarget)
	{
		sf::Vector2f steer 	= sf::Vector2f(0.f, 0.f);
		sf::Vector2f position = sf::Vector2f(this->x, this->y);
		float attractDist 	= 350.f;
		float slowDownDist 	= 100.f;

		// Subtract location from target then normalize and multiply by maxSpeed
		sf::Vector2f desired = arrivetarget - position;
		float dist = GetMagnitude(desired);

		// Slowdown when distance less than slowDownDist
		if (dist < slowDownDist)
		{
			float m = ReMap(dist, 0.f, slowDownDist, 0.f, maxSpeed);
			desired = SetMagnitude(desired, m);
		}
		else
		{
			desired = SetMagnitude(desired, maxSpeed);
		}

		// Ignore if distance greater than attractDist
		if (dist < attractDist)
		{
			steer = desired - sf::Vector2f(velocity.x, velocity.y);
			// LIMIT maxForce
			if (GetMagnitude(steer) > maxForce)
				steer = SetMagnitude(steer, maxForce);
		}
		return steer;
	}

	sf::Vector2f Avoid(std::vector<Obstacle>& _obstacles, int _amountOfObstacles){
    	float aheadDist = 25;
    	sf::Vector2f steer = sf::Vector2f(0,0);
		sf::Vector2f location = sf::Vector2f(x,y);

		// Calculate Ahead points
		sf::Vector2f velNormal = velocity;
    	velNormal = SetMagnitude(velNormal, aheadDist);
    	sf::Vector2f ahead = location + velNormal;
    	velNormal = SetMagnitude(velNormal, aheadDist/2);
    	sf::Vector2f ahead2 = location + velNormal;

    	sf::Vector2f mostThreatening = FindMostThreateningLocation(ahead, ahead2, _obstacles, _amountOfObstacles);

    	if(mostThreatening != sf::Vector2f(0,0))
		{
      		steer = ahead - mostThreatening;
      		steer = SetMagnitude(steer, maxForce*2);
    	}

    	return steer;
  	}

	// ***********For finding Most Threatening obstacle****************
  	bool lineIntersectsObstacle(sf::Vector2f _ahead, sf::Vector2f _ahead2, sf::Vector2f _obstacle, float _radius)
	{
    	return GetDistance(_obstacle, _ahead) <= _radius || GetDistance(_obstacle, _ahead2) <= _radius;
	}

	sf::Vector2f FindMostThreateningLocation(sf::Vector2f _ahead, sf::Vector2f _ahead2, std::vector<Obstacle>& _obstacles, int amountObstacles)
	{
    	sf::Vector2f mostThreatening = sf::Vector2f(0,0);

    	for(int i=0; i<amountObstacles; i++)
		{
      		sf::Vector2f obstacle = _obstacles[i].location;
      		bool collision = lineIntersectsObstacle(_ahead,_ahead2, obstacle, obstacles[i].radius);

      		// "position" is the character's current position
        	if (collision && (GetDistance(sf::Vector2f(x,y), obstacle) < GetDistance(sf::Vector2f(x,y), mostThreatening)))
            	mostThreatening = obstacle;
    	}
    	return mostThreatening;
  	}
  // ********End Most Threatening*********************

	void ApplyBehaviours()
	{
		if(steerBehaviour)
		{
			switch(behaviourType)
			{
				case SEEK:
					seekForce = Seek(target);
					AddForce(seekForce);
					break;
				case ARRIVE:
					arriveForce = Arrive(target);
					AddForce(arriveForce);
					break;
				default:
					AddForce(sf::Vector2f(0.f,0.f));
			}
		// NEED TO ADD WEIGHTING
		}

			avoidForce = Avoid(obstacles, (int)obstacles.size());
			avoidForce = ScaleVector(avoidForce, 2.5f);
			AddForce(avoidForce);

	}

	void ApplyForces()
	{
		// Add Forces
		ApplyBehaviours();
		AddForce(sf::Vector2f(0.f,this->gravity));

		AddForce(wind);

		// Add Acceleration
		velocity += accel;

		// LIMIT to maxSpeed
		if (GetMagnitude(velocity) > maxSpeed)
			velocity = SetMagnitude(velocity, maxSpeed);

		// Reset Acceleration
		accel.x = 0;
		accel.y = 0;

		// Update position
		x += velocity.x;
		y += velocity.y;
	}

	bool Update(sf::Vector2f seektarget)
	{
		if (x < -200)
		{
			active = false;
			lifetime = 0;
			return 0;
		}
		if (x > windowref.getSize().x + 200 || y > windowref.getSize().y + 200)
		{
			active = false;
			lifetime = 0;
			return 0;
		}

		if (this->lifetime > 5)
		{
			this->target = seektarget;
			ApplyForces();
			color = sf::Color(r, g, b, lifetime);
			if(!steerBehaviour)
				this->lifetime -= 1;
			return 1;
		}
		else
		{
			active = false;
			lifetime = 0;
			return 0;
		}
	}
};

std::vector<Particle> particles;
sf::VertexArray vertexarray(sf::Quads,MAX_NUM_PARTICLES*4);

class Emitter
{
private:
	sf::RenderWindow& mywindow;
	uint lastParticleInitialized = 0;
	bool doFullSearch 	= false;  	// Set true when entire array has been searched for an inactive particle


public:
	bool maxxedOut 		= false;	// then set maxxedOut when can't find an inactive particle

	Emitter(sf::RenderWindow& window) :
		mywindow(window)
	{
		// Initialize Arrays
		for (uint i = 0; i < MAX_NUM_PARTICLES; i++)
		{
			particles.push_back(Particle(mywindow));
			vertexarray[i*4].texCoords = (sf::Vector2f(0,0));
			vertexarray[i*4+1].texCoords = (sf::Vector2f(64,0));
			vertexarray[i*4+2].texCoords = (sf::Vector2f(64,64));
			vertexarray[i*4+3].texCoords = (sf::Vector2f(0,64));
		}
	}

	void Emit(uint posx, uint posy)
	{
		if(!maxxedOut)
		{
			for (uint i = 0; i < numParticles; i++)
			{
				for (uint j = lastParticleInitialized; j < particles.size(); j++)
				{
					if (!particles[j].active)
					{
						particles[j].Init(posx, posy);

							vertexarray[j*4].position.x = posx-particles[j].particleSize/2;
							vertexarray[j*4].position.y = posy;
							vertexarray[j*4].color = particles[j].color;

							vertexarray[j*4+1].position.x = posx + particles[j].particleSize/2;
							vertexarray[j*4+1].position.y = posy;
							vertexarray[j*4+1].color = particles[j].color;

							vertexarray[j*4+2].position.x = posx + particles[j].particleSize/2;
							vertexarray[j*4+2].position.y = posy + particles[j].particleSize;
							vertexarray[j*4+2].color = particles[j].color;

							vertexarray[j*4+3].position.x = posx-particles[j].particleSize/2;
							vertexarray[j*4+3].position.y = posy + particles[j].particleSize;
							vertexarray[j*4+3].color = particles[j].color;

						lastParticleInitialized = j;
						doFullSearch = false;
						break;
					}
					else if (j == (particles.size() - 1))
					{
						if(doFullSearch)
							{
								lastParticleInitialized = particles.size()-2;
								maxxedOut = true;
							}
							else
							lastParticleInitialized = 0;
						doFullSearch = true;
						break;
					}
				}
			}
		}
	}

	void Update(sf::Vector2f seektarget)
	{
		for (uint i = 0; i < particles.size(); i++)
		{
			if (particles[i].active)
			{
				if (particles[i].Update(seektarget))
				{
					vertexarray[i*4].position = sf::Vector2f(particles[i].x - particles[i].particleSize/2, particles[i].y);
					vertexarray[i*4+1].position = sf::Vector2f(particles[i].x + particles[i].particleSize/2, particles[i].y);
					vertexarray[i*4+2].position = sf::Vector2f(particles[i].x + particles[i].particleSize/2, particles[i].y + particles[i].particleSize);
					vertexarray[i*4+3].position = sf::Vector2f(particles[i].x - particles[i].particleSize/2, particles[i].y + particles[i].particleSize);
					vertexarray[i*4].color = particles[i].color;
					vertexarray[i*4+1].color = particles[i].color;
					vertexarray[i*4+2].color = particles[i].color;
					vertexarray[i*4+3].color = particles[i].color;
				}
				else
				{
					vertexarray[i*4].color = sf::Color::Transparent;
					vertexarray[i*4+1].color = sf::Color::Transparent;
					vertexarray[i*4+2].color = sf::Color::Transparent;
					vertexarray[i*4+3].color = sf::Color::Transparent;
				}
			}
		}
	}
};