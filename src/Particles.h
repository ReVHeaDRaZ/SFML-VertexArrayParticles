#include <stdlib.h>
#include <vector>

#define M_PI 3.14159265358979323846
#define MAX_NUM_PARTICLES 1000000	  //Maximum number of particles that can exist
#define ANGLE_UP (M_PI / 2.0f)		  //straight up (90deg)
#define FOUNTAIN_WIDTH (M_PI / 12.0f) //fountain width in radians (pi/12 is 15../deg)

extern uint numParticles;
extern bool fountain;
extern bool steerBehaviour;
extern bool seekOrArrive;
sf::Texture spriteTexture;
float gravity = 0.07;
sf::Vector2f wind = sf::Vector2f(0.f,0.f);

bool LoadTexture();
float RandomNumber(float Min, float Max);
float ReMap(float value, float istart, float istop, float ostart, float ostop);
float GetMagnitude(sf::Vector2f vector);
sf::Vector2f Normalize(sf::Vector2f vector);
sf::Vector2f SetMagnitude(sf::Vector2f vector, float mag);
sf::Vector2f ScaleVector(sf::Vector2f vector, float scale);

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

float GetMagnitude(sf::Vector2f vector)
{
	float mag = (vector.x * vector.x) + (vector.y * vector.y);
	mag = sqrt(mag);
	return mag;
}

sf::Vector2f Normalize(sf::Vector2f vector)
{
	float mag = GetMagnitude(vector);
	if(mag==0)
		return sf::Vector2f(0.f,0.f);
	return sf::Vector2f(vector.x / mag, vector.y / mag);
}

sf::Vector2f SetMagnitude(sf::Vector2f vector, float mag)
{
	sf::Vector2f normalized = Normalize(vector);
	return sf::Vector2f(normalized.x * mag, normalized.y * mag);
}

sf::Vector2f ScaleVector(sf::Vector2f vector, float scale)
{
	return sf::Vector2f(vector.x * scale, vector.y * scale);
}

float ReMap(float value, float istart, float istop, float ostart, float ostop)
{
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

class Particle
{
private:
	sf::RenderWindow& windowref;

	float angle;
	float init_v;
	float maxSpeed = 20.f;
	float maxForce = 0.2f;
	sf::Vector2f accel;

public:
	uint8_t lifetime;
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

		if (fountain)
		{
			angle = RandomNumber(ANGLE_UP - FOUNTAIN_WIDTH / 2.0f, ANGLE_UP + FOUNTAIN_WIDTH / 2.0f);
			init_v = RandomNumber(2.5f, 8.f);
			velocity.y = 0 - sin(angle) * init_v;
			velocity.x = cos(angle) * init_v;

			r = rand() % 255;
			b = rand() % 255;
			g = b;
		}

		else
		{
			velocity.x = (RandomNumber(1.f, 2.5f)) - (RandomNumber(1.f, 2.5f));
			velocity.y = (RandomNumber(1.f, 2.5f)) - (RandomNumber(1.f, 2.5f));

			r 	= 255;
			b 	= rand() % 50;
			g	= b;
		}

		lifetime = rand();
		active = true;

		color = sf::Color(r, lifetime / 2, b, lifetime);
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

	void ApplyBehaviours()
	{
		if(steerBehaviour)
		{
			if(seekOrArrive)
			{
				sf::Vector2f seekForce = Seek(target);
				AddForce(seekForce);
			}
			else{
				sf::Vector2f arriveForce = Arrive(target);
				AddForce(arriveForce);
			}

		// NEED TO ADD WEIGHTING
		}
	}

	void ApplyForces()
	{
		// Add Forces
		ApplyBehaviours();
		AddForce(sf::Vector2f(0.f,gravity));
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
			this->target = seektarget;
			ApplyForces();
			color = sf::Color(r, lifetime / 2, b, lifetime);
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
		// Initialize Arrays
		for (uint i = 0; i < MAX_NUM_PARTICLES; i++)
		{
			particles.push_back(Particle(mywindow));
			vertexarray.append(sf::Vertex(sf::Vector2f(posx, posy), particles.back().color));
		}
	}

	void Emit(uint posx, uint posy)
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

	void Update(sf::Vector2f seektarget)
	{
		for (uint i = 0; i < particles.size(); i++)
		{
			if (particles[i].active)
			{
				if (particles[i].Update(seektarget))
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
