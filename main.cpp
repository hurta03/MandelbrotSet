#include "ComplexPlane.h"
using namespace std;
int main()
{
    int xRes = VideoMode::getDesktopMode().width;
    int yRes = VideoMode::getDesktopMode().height;
    
    sf::Font font;
	if (!font.loadFromFile("fonts/KOMIKAP_.ttf"))
	{
		cout << "Failed" << endl;
    }

    VideoMode vm(xRes, yRes);

    RenderWindow window(vm, "Mandelbrot", Style::Default);

    sf::Text text;
	text.setFont(font);
    sf::Event event;
    ComplexPlane plane(xRes, yRes);
    State state;

    while (window.isOpen())
	{
        if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
            window.close();
        }

        while (window.pollEvent(event))
		{
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    plane.zoomOut();
                    plane.setCenter(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    state = State::CALCULATING;

                }


                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    plane.zoomIn();
                    plane.setCenter(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    state = State::CALCULATING;
                   
                }
            }

            if (event.type == sf::Event::MouseMoved)
            {
                plane.setMouseLocation(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
            }
        }
        //update plane segment
        plane.updateRender();
        plane.loadText(text);
        window.clear();
        
        window.draw(plane);
        window.draw(text);
        window.display();
        window.clear();
    }

    return 0;
}
