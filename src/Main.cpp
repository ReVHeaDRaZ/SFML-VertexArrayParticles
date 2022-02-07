#include "Particles.h"
#include "Platform/Platform.hpp"
#include "Hud.h"

uint winW = 1024;				// Window Resolution
uint winH = 768;
sf::Vector2i mousePos;			// Mouse Pointer
uint8_t pointerRadius = 5;
float frames;					// To store FramesPerSecond
int count = 0;					// To count loops

uint textureNumber	= 0;		// To select Texture
uint numParticles 	= 150; 		// To set Number of Particles to emit every frame
bool emitParticles 	= false; 	// To control emitting with mousebutton
bool steerBehaviour = false;	// To control steer behaviours on and off with mousebutton
bool BlendingMode	= true;		// To select BlendMode 0-Alpha 1-Color

int main()
{
	sf::VideoMode videomode = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window;
	winW = videomode.width;
	winH = videomode.height;
	window.create(videomode, "RaZ Fountain",sf::Style::Fullscreen);
	window.setFramerateLimit(60);
	window.setMouseCursorVisible(false);

	LoadTextures();	// For Sprite use
	sf::RenderStates renderstate(sf::BlendAdd);
	renderstate.texture = &spriteTexture[textureNumber];

	// MousePointer
	mousePos = sf::Mouse::getPosition(window);
	sf::CircleShape circle(pointerRadius);
	circle.setFillColor(sf::Color(0, 0, 255, 100));
	circle.setOrigin(pointerRadius, pointerRadius);
	circle.setPosition(mousePos.x, mousePos.y);

	Emitter emitter(window);

	InitHud();

	// Use a timer to obtain the time elapsed
	sf::Clock clk;
	clk.restart();

	sf::Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			// MOUSE Events
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left)
				emitParticles = true;
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Left)
				{
					emitParticles = false;
					emitter.maxxedOut = false;
				}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Middle)
				SwitchParticleType();
			// Apply behavior
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Right)
				steerBehaviour = true;
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Right)
				steerBehaviour = false;

			// KEYBOARD Events
			if (event.type == sf::Event::KeyPressed)
			{
				if(event.key.code == sf::Keyboard::Key::Escape)
					window.close();
				if(event.key.code == sf::Keyboard::Key::B)
				{
					SwitchBehaviourType();
				}
				if(event.key.code == sf::Keyboard::Key::P)
				{
					SwitchParticleType();
				}
				// Change amount to emit
				if(event.key.code == sf::Keyboard::Key::Up)
				{
					numParticles+=10;
					if(numParticles>4000) numParticles=4000;
				}
				if(event.key.code == sf::Keyboard::Key::Down)
				{
					numParticles-=10;
					if(numParticles < 10) numParticles = 10;
				}
				// Change Wind
				if(event.key.code == sf::Keyboard::Key::Left)
				{
					wind.x = wind.x - 0.005f;
					if(wind.x < -0.1) wind.x = -0.1f;
				}
				if(event.key.code == sf::Keyboard::Key::Right)
				{
					wind.x = wind.x + 0.005f;
					if(wind.x > 0.1) wind.x = 0.1f;
				}
				// Change maxSpeed
				if(event.key.code == sf::Keyboard::Key::PageUp)
				{
					maxSpeed += 0.5f;
					if(maxSpeed > 40.f) maxSpeed = 40.f;
				}
				if(event.key.code == sf::Keyboard::Key::PageDown)
				{
					maxSpeed -= 0.5f;
					if(maxSpeed < 1.f) maxSpeed = 1.f;
				}
				// Change maxForce
				if(event.key.code == sf::Keyboard::Key::Home)
				{
					maxForce += 0.05f;
					if(maxForce > 1.f) maxForce = 1.f;
				}
				if(event.key.code == sf::Keyboard::Key::End)
				{
					maxForce -= 0.05f;
					if(maxForce < 0.05f) maxForce = 0.05f;
				}
				if(event.key.code == sf::Keyboard::Key::T)
				{
					textureNumber++;
					if(textureNumber > MAX_NUM_TEXTURES-1) textureNumber = 0;
					renderstate.texture = &spriteTexture[textureNumber];
				}
				if(event.key.code == sf::Keyboard::Key::M)
				{
					BlendingMode = !BlendingMode;
					if(BlendingMode) renderstate.blendMode = sf::BlendAdd;
					else renderstate.blendMode = sf::BlendAlpha;
				}
			}
		}

		mousePos = sf::Mouse::getPosition(window);
		if(emitParticles && (!emitter.maxxedOut)) emitter.Emit(mousePos.x, mousePos.y);

		window.clear(sf::Color::Black);

		emitter.Update(sf::Vector2f(mousePos.x, mousePos.y));
		window.draw(vertexarray,renderstate);
		circle.setPosition(mousePos.x, mousePos.y);
		window.draw(circle, sf::BlendAdd);
		DrawHud(&window);

		window.display();

		// Calculate Frames per Second
		frames = 1.f / clk.getElapsedTime().asSeconds();
		clk.restart();

		// Update Hud values
		count++;
		if(count>=40)
		{
			if(emitter.maxxedOut)
				pMax.setFillColor(sf::Color::Red);
			else pMax.setFillColor(sf::Color::White);

			pMax.setString(to_string(particles.size()));
			pEmit.setString(to_string(numParticles));
			pFrames.setString(to_string(frames));
			pForce.setString(to_string(maxForce));
			pSpeed.setString(to_string(maxSpeed));
			count=0;
		}
	}

	return 0;
}
