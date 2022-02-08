#pragma once

float RandomNumber(float Min, float Max);
float ReMap(float value, float istart, float istop, float ostart, float ostop);
float GetMagnitude(sf::Vector2f vector);
float GetDistance(sf::Vector2f vector1, sf::Vector2f vector2);
sf::Vector2f Normalize(sf::Vector2f vector);
sf::Vector2f SetMagnitude(sf::Vector2f vector, float mag);
sf::Vector2f ScaleVector(sf::Vector2f vector, float scale);



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

float GetDistance(sf::Vector2f vector1, sf::Vector2f vector2)
{
	float a = (vector2.x - vector1.x) * (vector2.x - vector1.x);
	float b = (vector2.y - vector1.y) * (vector2.y - vector1.y);
	return sqrt(a + b);
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
