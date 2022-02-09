#include "Particles.h"
#include "Platform/Platform.hpp"
#include "Hud.h"

uint winW = 1920;				// Window Resolution
uint winH = 1080;
sf::Vector2i mousePos;			// Mouse Pointer
uint8_t pointerRadius = 5;
float frames;					// To store FramesPerSecond
int count = 0;					// To count loops

uint textureNumber	= 0;		// To select Texture
uint numParticles 	= 150; 		// To set Number of Particles to emit every frame
bool emitParticles 	= false; 	// To control emitting with mousebutton
bool steerBehaviour = false;	// To control steer behaviours on and off with mousebutton
bool BlendingMode	= true;		// To select BlendMode 0-Alpha 1-Color
bool drawQuads 		= true;		// Draw Quads or Points
bool MenuState		= true;		// Menu or running

static void show_usage(std::string name)
{
    std::cout << "Usage: " << name << " <option(s)> "
              << "Options:\n"
              << "\t-h\t\tShow this help message\n"
              << "\t-r Resolution WIDTH HEIGHT\t"
              << std::endl;
}

int main(int argc, char* argv[])
{
    //-------------Command-Line Arguments-----------------
	// No Arguments
	if(argc==1)
	{
        winW = 1920;
		winH = 1080;
	}
	else
	{	// Not enough arguments
		if (argc < 3)
		{
        	std::cout << "Usage: " << argv[0] << "-r Resolution WIDTH HEIGHT" << std::endl;
			show_usage(argv[0]);
        	return 0;
    	}

		std::string widthArg;
		std::string heightArg;
		for (int i = 1; i < argc; ++i)
		{
        	std::string arg = argv[i];
        	if (arg == "-h") {
            	show_usage(argv[0]);
            	return 0;
        	} else if (arg == "-r")
			{
            	if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                	widthArg = argv[i++]; // Increment 'i' so we don't get the argument as the next argv[i].
            	} else
				{ // Uh-oh, there was not enough argument to the option.
                  	std::cout << "-r option requires two arguments." << std::endl;
                	return 0;
            	}
			} else
			{
            	heightArg = argv[i];
        	}
    	}
		winW = stoi(widthArg);
		winH = stoi(heightArg);
	}

	sf::RenderWindow window;
	window.create(sf::VideoMode(winW,winH), "RaZ Particles",sf::Style::Fullscreen); // Having issues on Spanned monitors to overriding with default winW,winH at moment
	window.setFramerateLimit(60);
	window.setMouseCursorVisible(false);
	winW = window.getSize().x;
	winH = window.getSize().y;

	LoadTextures();	// For Sprite use
	sf::RenderStates renderstate(sf::BlendAdd);
	renderstate.texture = &spriteTexture[textureNumber];

	// Setup MousePointer
	mousePos = sf::Mouse::getPosition(window);
	sf::CircleShape circle(pointerRadius);
	circle.setFillColor(sf::Color(0, 0, 255, 100));
	circle.setOrigin(pointerRadius, pointerRadius);
	circle.setPosition(mousePos.x, mousePos.y);

	Emitter emitterQuads(window, true);
	Emitter emitterPoints(window, false);

	InitHud();

	// Use a timer to obtain the time elapsed
	sf::Clock clk;
	clk.restart();

	sf::Event event;

	while (window.isOpen())
	{
		if(MenuState)
		{
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::KeyPressed)
				{
					if(event.key.code == sf::Keyboard::Key::Escape)
						window.close();
					if(event.key.code == sf::Keyboard::Key::Q)
					{
						drawQuads = true;
						MenuState = false;
					}
					if(event.key.code == sf::Keyboard::Key::P)
					{
						drawQuads = false;
						MenuState = false;
					}
				}
			}
		particleType = SLIME;
		emitterQuads.EmitQuads(400, (winH/2)+100);
		particleType = SPARKS;
		emitterPoints.EmitPoints(winW-400, (winH/2)+100);
		emitterQuads.UpdateQuads(sf::Vector2f(0,0));
		emitterPoints.UpdatePoints(sf::Vector2f(0,0));
		// Draw Everything
		window.clear(sf::Color::Black);
		DrawMenu(&window);
		window.draw(vertexarray, renderstate);
		window.draw(vertexarrayPoints, sf::BlendAdd);

		window.display();

		} else // ----------END OF MENU------------
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
					emitterQuads.maxxedOut 	= false;
					emitterPoints.maxxedOut = false;
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
				if(event.key.code == sf::Keyboard::Key::O)
				{
					obstacles.push_back(Obstacle(sf::Vector2f(mousePos.x,mousePos.y), RandomNumber(10.f,60.f)));

				}
			}
		}

		mousePos = sf::Mouse::getPosition(window);
		if(drawQuads)
		{
			if(emitParticles && (!emitterQuads.maxxedOut)) emitterQuads.EmitQuads(mousePos.x, mousePos.y);
		}else
		{
			if(emitParticles && (!emitterPoints.maxxedOut)) emitterPoints.EmitPoints(mousePos.x, mousePos.y);
		}

		// Draw Everything
		window.clear(sf::Color::Black);

		if(drawQuads)
		{
			emitterQuads.UpdateQuads(sf::Vector2f(mousePos.x, mousePos.y));
			window.draw(vertexarray,renderstate);
		}else
		{
			emitterPoints.UpdatePoints(sf::Vector2f(mousePos.x, mousePos.y));
			window.draw(vertexarrayPoints, sf::BlendAdd);
		}

		circle.setPosition(mousePos.x, mousePos.y);
		window.draw(circle, sf::BlendAdd);
		DrawHud(&window);
		for(uint i = 0; i<obstacles.size(); i++) obstacles[i].Draw(window);

		window.display();

		// Calculate Frames per Second
		frames = 1.f / clk.getElapsedTime().asSeconds();
		clk.restart();

		// Update Hud values
		count++;
		if(count>=40)
		{
			if(emitterQuads.maxxedOut || emitterPoints.maxxedOut)
				pMax.setFillColor(sf::Color::Red);
			else pMax.setFillColor(sf::Color::White);

			if(drawQuads) pMax.setString(to_string(particlesQuads.size()));
			else
				pMax.setString(to_string(particlesPoints.size()));

			pEmit.setString(to_string(numParticles));
			pFrames.setString(to_string(frames));
			pForce.setString(to_string(maxForce));
			pSpeed.setString(to_string(maxSpeed));
			count=0;
		}
	}
	} // End of Running
	return 0;
}
