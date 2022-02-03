#include <stdlib.h>
#include <vector>

#define M_PI 3.14159265358979323846
#define MAX_NUM_PARTICLES 1000000	  //Maximum number of particles that can exist
#define ANGLE_UP (M_PI / 2.0f)		  //straight up (90deg)
#define FOUNTAIN_WIDTH (M_PI / 12.0f) //fountain width in radians (pi/12 is 15../deg)

extern uint numParticles;
extern bool fountain;
sf::Texture spriteTexture;

float RandomNumber(float Min, float Max);
bool LoadTexture();

bool LoadTexture()
{
	if (!spriteTexture.loadFromFile("content/Dot64x64.png"))
		return 0;
	return 1;
}

float RandomNumber(float Min, float Max)
{
	return (Min + (float(rand()) / float(RAND_MAX)) * (Max - Min));
}

class Particle
{
private:
	sf::RenderWindow& windowref;
	float gravity = 0.07;
	float angle;
	float init_v;

public:
	uint8_t lifetime;
	float x, y;
	float xv, yv;
	bool active = false;
	uint8_t r = 255;
	uint8_t gb;
	sf::Color color;

	Particle(sf::RenderWindow& window) :
		windowref(window)
	{
	}

	void Init(int posx, int posy)
	{
		x = posx;
		y = posy;

		if (fountain)
		{
			angle = RandomNumber(ANGLE_UP - FOUNTAIN_WIDTH / 2.0f, ANGLE_UP + FOUNTAIN_WIDTH / 2.0f);
			init_v = RandomNumber(2.5f, 8.f);
			yv = 0 - sin(angle) * init_v;
			xv = cos(angle) * init_v;

			r = rand() % 255;
			gb = rand() % 255;
		}

		else
		{
			xv = (RandomNumber(1.f, 2.5f)) - (RandomNumber(1.f, 2.5f));
			yv = (RandomNumber(1.f, 2.5f)) - (RandomNumber(1.f, 2.5f));

			r = 255;
			gb = rand() % 50;
		}

		lifetime = rand();
		active = true;

		color = sf::Color(r, lifetime / 2, gb, lifetime);
	}

	void ApplyForce()
	{
		yv += gravity;
		x += xv;
		y += yv;
	}

	bool Update()
	{
		if (x < 0)
		{
			active = false;
			lifetime = 0;
			return 0;
		}
		if (x > windowref.getSize().x || y > windowref.getSize().y)
		{
			active = false;
			lifetime = 0;
			return 0;
		}

		if (this->lifetime > 5)
		{
			ApplyForce();
			color = sf::Color(r, lifetime / 2, gb, lifetime);
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
sf::VertexArray vertexarray(sf::Points);

class Emitter
{
private:
	sf::RenderWindow& mywindow;
	uint lastParticleInitialized = 0;

public:
	Emitter(uint posx, uint posy, sf::RenderWindow& window) :
		mywindow(window)
	{
		for (uint i = 0; i < MAX_NUM_PARTICLES; i++)
		{
			particles.push_back(Particle(mywindow));
			//particles.back().Init(posx, posy);
			vertexarray.append(sf::Vertex(sf::Vector2f(posx, posy), particles.back().color));
		}
	}

	void Init(uint posx, uint posy)
	{
		for (uint i = 0; i < numParticles; i++)
		{
			for (uint j = lastParticleInitialized; j < particles.size(); j++)
			{
				if (!particles[j].active)
				{
					particles[j].Init(posx, posy);
					vertexarray[j].position.x = posx;
					vertexarray[j].position.y = posy;
					vertexarray[j].color = particles[j].color;
					lastParticleInitialized = j;
					break;
				}
				else if (j == (particles.size() - 1))
				{
					//particles.push_back(Particle(mywindow));
					//particles.back().Init(posx, posy);
					//vertexarray.append(sf::Vertex(sf::Vector2f(posx,posy),particles.back().color));
					lastParticleInitialized = 0;
					break;
				}
			}
		}
	}

	void Update()
	{
		for (uint i = 0; i < particles.size(); i++)
		{
			if (particles[i].active)
			{
				if (particles[i].Update())
				{
					vertexarray[i].position = sf::Vector2f(particles[i].x, particles[i].y);
					vertexarray[i].color = particles[i].color;
				}
				else
					vertexarray[i].color = sf::Color::Black;
			}
		}
	}
};
