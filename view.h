#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

View view;

void getPlayerCoordForView(float x, float y) {
	float tempX = x, tempY = y;
	if (x < 900) tempX = 900;
	if (x > 3630) tempX = 3630;
	if (y > 570) tempY = 570;
	
	view.setCenter(tempX+100, tempY);
};
