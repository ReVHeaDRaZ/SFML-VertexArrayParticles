#pragma once

extern const float winW;
extern const float winH;
sf::Text heading,pMax,pEmit,pFrames, instructions, behaviourheading, behaviourtype;
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

void InitHud(){
	heading.setFont(font);
	heading.setCharacterSize(20);
	heading.setFillColor(sf::Color::White);
	heading.setPosition(10,10);
	pMax 			= heading;
	pEmit 			= heading;
	pFrames 		= heading;
	instructions 	= heading;
	behaviourheading= heading;
	behaviourtype	= heading;

	heading.setString("MaxParticles     ParticlesEmittedPerFrame     FPS   ");

	instructions.setString("LMB-          Emit Particles\nMMB or P-   Particle Type\nRMB-          Apply Behaviour\nUPnDWN-     ParticleAmount\nLEFTnRIGHT- Wind Amount\nB-             Change Behaviour");
	instructions.setPosition(winW-220,20);

	behaviourheading.setString("BehaviourType");
	behaviourheading.setPosition(winW-100,winH-80);

	behaviourtype.setString("SeeK");
	behaviourtype.setPosition(winW-70,winH-60);

	pMax.setString("0");
	pMax.setPosition(20,30);

	pEmit.setString("0");
	pEmit.setPosition(180,30);

	pFrames.setString("0");
	pFrames.setPosition(300,30);

	//load the font
	//note that this path is relative to the workspace
	if (!font.loadFromFile("content/shredded.ttf"))
	{
		std::cout << "Failed to load font" << std::endl;
	}
}

void DrawHud(sf::RenderWindow* win){
	win->draw(heading);
	win->draw(instructions);
	win->draw(behaviourheading);
	win->draw(behaviourtype);
	win->draw(pMax);
	win->draw(pEmit);
	win->draw(pFrames);
}