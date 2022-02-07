#pragma once

extern const float winW;
extern const float winH;
sf::Text heading, heading2, pMax, pEmit, pFrames, pForce, pSpeed, instructions, instructions2, behaviourheadingHud, behaviourtypeHud;
sf::Font font;

template <typename T>
//function to convert a non-string variable to a string.
std::string to_string(T value)
{
	std::ostringstream os;
	os << value;
	return os.str();
}
void InitHud();
void DrawHud(sf::RenderWindow* win);

void InitHud()
{
	heading.setFont(font);
	heading.setCharacterSize(20);
	heading.setFillColor(sf::Color::White);
	heading.setPosition(10, 10);
	pMax = heading;
	pEmit = heading;
	pFrames = heading;
	pForce = heading;
	pSpeed = heading;
	instructions = heading;
	behaviourheadingHud = heading;
	behaviourtypeHud = heading;
	heading2 = heading;
	instructions2 = heading;

	heading.setString("MaxParticles     ParticlesEmittedPerFrame     FPS   ");

	instructions.setString("LMB-          Emit Particles\nMMB or P-   Particle Type\nRMB-          Apply Behaviour\nUPnDWN-     ParticleAmount\nLEFTnRIGHT- Wind Amount\nB-             Change Behaviour\nT-              Change Texture");
	instructions.setPosition(winW - 220, 20);

	behaviourheadingHud.setString("BehaviourType");
	behaviourheadingHud.setPosition(winW - 100, winH - 60);

	behaviourtypeHud.setString("SeeK");
	behaviourtypeHud.setPosition(winW - 70, winH - 40);

	pMax.setString("0");
	pMax.setPosition(20, 30);

	pEmit.setString("0");
	pEmit.setPosition(180, 30);

	pFrames.setString("0");
	pFrames.setPosition(300, 30);

	pForce.setString("0");
	pForce.setPosition(winW - 120, winH - 100);

	pSpeed.setString("0");
	pSpeed.setPosition(winW - 50, winH - 100);

	heading2.setString("MaxForce   MaxSpeed");
	heading2.setPosition(winW - 150, winH - 120);

	instructions2.setString("HOMEnEND-    Force\nPAGEUPnDWN- Speed");
	instructions2.setPosition(winW - 150, winH - 180);

	//load the font
	//note that this path is relative to the workspace
	if (!font.loadFromFile("content/shredded.ttf"))
	{
		std::cout << "Failed to load font" << std::endl;
	}
}

void DrawHud(sf::RenderWindow* win)
{
	win->draw(heading);
	win->draw(instructions);
	win->draw(behaviourheadingHud);
	win->draw(behaviourtypeHud);
	win->draw(pMax);
	win->draw(pEmit);
	win->draw(pFrames);
	win->draw(pForce);
	win->draw(pSpeed);
	win->draw(heading2);
	win->draw(instructions2);
}