#include "utility.h"

void formatText(sf::Text* text, int x, int y, sf::Color fillColor, sf::Color outlineColor, bool outline)
{
	text->setFillColor(fillColor);
	if (outline)
	{
		text->setOutlineThickness(3);
		text->setOutlineColor(outlineColor);
	}
	text->setPosition(x, y);
}