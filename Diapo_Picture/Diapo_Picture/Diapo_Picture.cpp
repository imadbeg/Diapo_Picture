#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>

#define GERMS_LEN 3

using namespace std;

struct germe {
	int x;
	int y;
	int v;
	sf::Color color;
};


int sqr_dist(int x1, int y1, int x2, int y2)
{
	int dx = x1 - x2;
	int dy = y1 - y2;
	return dx * dx + dy * dy;
}

vector<germe> get_germs() {
	vector<germe> arr;
	sf::Color colors[] = { sf::Color::Blue, sf::Color::Cyan,sf::Color::Green, sf::Color::Magenta,sf::Color::Yellow, sf::Color::Red };
	for (int i = 0; i < GERMS_LEN; i++) {
		germe ger;

		ger.x = rand() % 800;
		ger.y = rand() % 600;
		ger.color = colors[rand() % 7];

		arr.push_back(ger);
	}

	return arr;
}


int main()
{

	vector<germe> g = get_germs();

	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");

	sf::Image matrix;
	sf::Texture texture;
	sf::Sprite       sprite;
	matrix.create(800, 600, sf::Color::Black);
	sf::Uint8* pixels = new sf::Uint8[800 * 600 * 4];

	for (int x = 0; x < 800; x++)
	{
		for (int y = 0; y < 600; y++)
		{
			int minDist = 100000000;
			for (int t = 0; t < GERMS_LEN; t++) {
				if (sqr_dist(g[t].x, g[t].y, x, y) < minDist) {
					minDist = sqr_dist(g[t].x, g[t].y, x, y);

					matrix.setPixel(x, y, g[t].color);
				}
			}
		}
	}

	texture.loadFromImage(matrix);
	sprite.setTexture(texture, true);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(sprite);//draw voronoi
		//draw germes circles
		for (int i = 0; i < GERMS_LEN; i++) {

			sf::CircleShape circle;
			circle.setRadius(3);
			circle.setFillColor(sf::Color::Black);
			circle.setPosition(g[i].x, g[i].y);

			window.draw(circle);//draw germs
		}
		window.display();
	}
}