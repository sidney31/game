#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include "map.h"
#include "view.h"


using namespace sf;
using namespace std;

class Player {
private: float x, y = 0;
public:
	char dir;
	bool onGround, run;
	double CurrentFrame;
	float w, h, dx, dy, speed;
	enum stateObject { left, right, stay, jump };
	stateObject state;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;

	Player(String F, float X, float Y, float W, float H) {
		onGround = false;
		CurrentFrame = 0;
		state = stay;
		run = false;
		dir = 'r';
		dx = 0; dy = 0; speed = 0;
		File = F;
		w = W; h = H;
		image.loadFromFile("resources/" + File);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		sprite.setTextureRect(IntRect(0, 0, w, h));
		sprite.setScale(2.0, 2.0);
		sprite.setOrigin(w / 2, h / 2);
	};
	void update(float time) {
		control();
		switch (state) {
		case right: 
			dir = 'r';
			dx = speed;
			CurrentFrame += 0.01 * time;
			if (CurrentFrame > 4) CurrentFrame -= 4;
			if (run) {
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 96, 32, 32));
			}
			else {
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 96, 32, 32));
			}
			break;
		case left:
			dir = 'l';
			dx = -speed;
			CurrentFrame += 0.01 * time;
			if (CurrentFrame > 4) CurrentFrame -= 4;
			if (run) {
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame) + 32, 96, -32, 32));
			}
			else {
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame) + 32, 96, -32, 32));
			}
			break;
		case stay:
			dx = 0;
			CurrentFrame += 0.005 * time;
			if (CurrentFrame > 2) CurrentFrame -= 2;
			if (dir == 'r') {
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 0, 32, 32));
			}
			if (dir == 'l') {
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame) + 32, 0, -32, 32));
			}
			break;
		case jump:
			CurrentFrame += 0.005 * time;
			if (CurrentFrame > 2) CurrentFrame -= 2;
			if (dir == 'r') {
				sprite.setTextureRect(IntRect(0 * int(CurrentFrame), 160, 32, 32));
			}
			if (dir == 'l') {
				sprite.setTextureRect(IntRect(0 * int(CurrentFrame) + 32, 160, -32, 32));
			}
		};
		x += dx * time;
		checkCollisionWithMap(dx, 0);
		y += dy * time;
		checkCollisionWithMap(0, dy);
		speed = 0;
		sprite.setPosition(x + w / 2, y + h / 2 - 13);
		dy = dy + 0.0015 * time; checkCollisionWithMap(0, dy);
		if (!onGround) {
			state = jump;
		}else {
			state = stay;
		}
	};
	void control() {
		Clock clock;
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
			if (onGround) state = right; speed = 0.15;
		}
		if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
			if (onGround) state = left; speed = 0.15;
		}
		if ((Keyboard::isKeyPressed(Keyboard::Space) || (Keyboard::isKeyPressed(Keyboard::Up))) && (onGround)) {
			dy = -0.4, onGround = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::LShift)) {
			speed = 0.25;
			run = true;

		}else {
			run = false;
		};
	}
	float getPlayerCoordX() {
		return x;
	};
	float getPlayerCoordY() {
		return y;
	};
	void checkCollisionWithMap(float Dx, float Dy) {
		for (int i = y / 32; i < (y + h) / 32; i++) {
			for (int j = x / 32; j < (x + w) / 32; j++) {
				if ((TitleMap[i][j] == 'g') || (TitleMap[i][j] == 'v') || (TitleMap[i][j] == 'u') || (TitleMap[i][j] == 'i') || (TitleMap[i][j] == 'o')) {
					if (Dy > 0) {
						y = i * 32 - h; dy = 0; onGround = true;
					}
					else {
						onGround = false;
					}
					if (Dy < 0) {
						y = i * 32 + 32; dy = 0;
					}
					if (Dx > 0) {
						x = j * 32 - w;
					}
					if (Dx < 0) {
						x = j * 32 + 32;
					}
				}
			}
		}
	}
	
};

int main()
{
	vector <VideoMode> scrRes = VideoMode::getFullscreenModes();
	RenderWindow window(scrRes[0], "game", Style::Fullscreen);
	view.reset(FloatRect(0, 0, scrRes[0].width, scrRes[0].height));

	//подгрузка спрайтов окружения
	Image map_image;
	map_image.loadFromFile("resources/map.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);
	//

	//спрайты фона
	Image bg_image;
	bg_image.loadFromFile("resources/sky.png");
	Texture bg;
	bg.loadFromImage(bg_image);
	Sprite s_bg;
	s_bg.setTexture(bg);
	s_bg.setScale(10.0, 10.0);
	s_bg.setPosition(0, -500);
	//
	Player ped("char.png", 300, 680, 32.0, 32.0);

	float currentFrame = 0;
	Clock clock;

	//инициализация текста на экране
	Font font;
	font.loadFromFile("resources/Ubuntu.ttf");
	Text dMenu("", font, 14);
	dMenu.setFillColor(Color::White);
	dMenu.setOutlineColor(Color::Black);
	dMenu.setOutlineThickness(2);
	Text help("", font, 16);
	help.setFillColor(Color::White);
	help.setOutlineColor(Color::Black);
	help.setOutlineThickness(3);

	bool showDebugMenu = false;


	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;

		Event event;
		while (window.pollEvent(event))
		{
			if ((event.type == Event::Closed) || (Keyboard::isKeyPressed(Keyboard::Escape))) {
				window.close();
			}
			if (Keyboard::isKeyPressed(Keyboard::F1)) {
				showDebugMenu = !showDebugMenu;
			}
		}
		getPlayerCoordForView(ped.getPlayerCoordX(), ped.getPlayerCoordY());
		ped.update(time);
		window.setView(view);

		window.clear();
		window.draw(s_bg);
		for (int i = 0; i < HEIGHT_MAP; i++){
			for (int j = 0; j < WIDTH_MAP; j++){
				if (TitleMap[i][j] == 'g')  s_map.setTextureRect(IntRect(0, 0, 32, 32));
				if (TitleMap[i][j] == 'i')  s_map.setTextureRect(IntRect(32, 0, 32, 32));
				if ((TitleMap[i][j] == 'u')) s_map.setTextureRect(IntRect(64, 0, 32, 32));
				if ((TitleMap[i][j] == 'o')) s_map.setTextureRect(IntRect(64+32, 0, -32, 32));
				if ((TitleMap[i][j] == 'd')) s_map.setTextureRect(IntRect(96, 0, 32, 32));
				if ((TitleMap[i][j] == ' ')) s_map.setTextureRect(IntRect(128, 0, 32, 32));
				if ((TitleMap[i][j] == 'v')) s_map.setTextureRect(IntRect(128, 0, 32, 32));
				if ((TitleMap[i][j] == 'r')) s_map.setTextureRect(IntRect(160, 0, 32, 32));
				if ((TitleMap[i][j] == 'b')) s_map.setTextureRect(IntRect(192, 0, 32, 32));
				s_map.setPosition(j * 32, i * 32);
				window.draw(s_map);
			}
		}
		ostringstream onGround, state, dir, x, y, dx, dy, run;
		onGround << ped.onGround;
		state << ped.state;
		dir << ped.dir;
		x << ped.getPlayerCoordX();
		y << ped.getPlayerCoordY();
		dx << ped.dx;
		dy << ped.dy;
		run << ped.run;
		dMenu.setString(L"onGround: " + onGround.str() + L"\nstate: " + state.str() + L"\ndir: " + dir.str() +
		L"\nx: " + x.str() + L"\ny: " + y.str() + L"\ndx: " + dx.str() + L"\ndy: " + dy.str() + L"\nrun: " +
		run.str());
		dMenu.setPosition(view.getCenter().x - 950, view.getCenter().y - 400);
		help.setString("D/left - left\nA/Right - right\nSpace/up - jump\nShift - boost\nF1 - hide/show\ndebug menu");
		help.setPosition(view.getCenter().x - 950, view.getCenter().y - 530);

		if (showDebugMenu) {
			window.draw(dMenu);
		}
		window.draw(help);
		window.draw(ped.sprite);
		window.display();
	}
	return 0;
}
