#include "Particles.h"
#include "Platform/Platform.hpp"
#include "Hud.h"

const float winW = 1024;
const float winH = 768;
sf::Vector2i mousePos;
uint8_t pointerRadius = 5;
float frames;
int count = 0;
bool emitParticles = false; 	// To control emitting with mousebutton
uint numParticles = 150; 		// Number of Particles to emit every frame
bool fountain = false; 			// Emit fountain or standard particles


int main()
{

	sf::RenderWindow window;
	window.create(sf::VideoMode(winW, winH), "RaZ Fountain");
	window.setFramerateLimit(60);
	window.setMouseCursorVisible(false);

	LoadTexture();

	sf::RenderStates renderstate(sf::BlendAdd);
	//renderstate.texture = &spriteTexture;

	// MousePointer
	mousePos = sf::Mouse::getPosition(window);
	sf::CircleShape circle(pointerRadius);
	circle.setFillColor(sf::Color(0, 0, 255, 100));
	circle.setOrigin(pointerRadius, pointerRadius);
	circle.setPosition(mousePos.x, mousePos.y);

	Emitter emitter(mousePos.x, mousePos.y, window);

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

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left)
				emitParticles = true;
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Left)
				emitParticles = false;
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Right)
				fountain = !fountain;

			if (event.type == sf::Event::KeyPressed)
			{
				if(event.key.code == sf::Keyboard::Key::Escape)
					window.close();
				if(event.key.code == sf::Keyboard::Key::I)
				{
					std::cout << particles.size() << std::endl;
					std::cout << frames << std::endl;
				}
				if(event.key.code == sf::Keyboard::Key::Up)
				{
					numParticles+=10;
					if(numParticles>3000) numParticles=3000;
				}
				if(event.key.code == sf::Keyboard::Key::Down)
				{
					numParticles-=10;
					if(numParticles < 1) numParticles = 1;
				}
			}
		}

		mousePos = sf::Mouse::getPosition(window);
		if(emitParticles) emitter.Init(mousePos.x, mousePos.y);



		window.clear(sf::Color::Black);

		emitter.Update();
		window.draw(vertexarray,renderstate);
		circle.setPosition(mousePos.x, mousePos.y);
		window.draw(circle, sf::BlendAdd);
		DrawHud(&window);

		window.display();

		// Calculate Frames per Second
		frames = 1.f / clk.getElapsedTime().asSeconds();
		clk.restart();

		count++;
		if(count>=40)
		{
			pMax.setString(to_string(particles.size()));
			pEmit.setString(to_string(numParticles));
			pFrames.setString(to_string(frames));
			count=0;
		}
	}

	return 0;
}
