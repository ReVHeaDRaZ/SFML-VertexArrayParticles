#pragma once

sf::Text heading,pMax,pEmit,pFrames;
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
	pMax = heading;
	pEmit = heading;
	pFrames = heading;

	heading.setString("MaxParticles     ParticlesEmittedPerFrame     FPS                           LMB- EMIT Particles   RMB- FOUNTAINorOMNI   UPnDWN- ParticleAmount");
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
	win->draw(pMax);
	win->draw(pEmit);
	win->draw(pFrames);
}