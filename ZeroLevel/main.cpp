#include "testtools.h"

#include <SFML/Graphics.hpp>
#include <math.h>

// scale - size of one pixel
void makeMSet (sf::Texture& pic, sf::Vector2f center_pos, sf::Vector2u res, float scale); 

int main () {
    sf::Vector2u resolution(800, 600);
    sf::Vector2f MsetPosition(0.f, 0.f);
    sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Render");
    startFPScnt(&window);

    sf::Sprite  MondSet;
    sf::Texture texture;

    float scale = 3.f / 1000.f;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    resolution = window.getSize();
                    break;
                case sf::Event::KeyReleased:
                    switch (event.key.code) {
                        case sf::Keyboard::Up:
                            MsetPosition.y += 10.f * scale;
                            break;
                        case sf::Keyboard::Down:
                            MsetPosition.y -= 10.f * scale;
                            break;
                        case sf::Keyboard::Right:
                            MsetPosition.x += 10.f * scale;
                            break;
                        case sf::Keyboard::Left:
                            MsetPosition.x -= 10.f * scale;
                            break;
                        case sf::Keyboard::Num1:
                            scale *= 1.25f;
                            break;
                        case sf::Keyboard::Num2:
                            scale *= 0.8f;
                            break;
                        default:
                            break;
                    }


                   break;
                default:
                    break;
            }
        }

        makeMSet(texture, MsetPosition, resolution, scale);
        MondSet.setTexture(texture);

        window.clear();

        window.draw(MondSet);
        updateFPS();
        drawFPS();

        window.display();
    }
    
    return 0;
}

void makeMSet (sf::Texture& pic, sf::Vector2f center_pos, sf::Vector2u res, float scale) {
    sf::Image img;
    img.create(res.x, res.y, sf::Color(0, 0, 0));
    const int maxstep = 255;
    pic.setSrgb(true);

    float maxr  = 100.f;
    float xcord = center_pos.x - (res.x * scale) * 0.5f;
    float ycord = center_pos.y - (res.y * scale) * 0.5f;

    float ybuf = ycord;
    for (size_t xi = 0; xi < res.x; xi++, xcord += scale) {
        ycord = ybuf;

        for (size_t yi = 0; yi < res.y; yi++, ycord += scale) {
            float x = xcord;
            float y = ycord;

            int step = 0;
            for (; step < maxstep; step++) {
                float x2 = x * x;
                float y2 = y * y;
                float xy = x * y;

                if (x2 + y2 >= maxr) {
                    break;
                }

                x = x2 - y2 + xcord;
                y = 2 * xy + ycord;
            }

            img.setPixel(xi, res.y - yi - 1, sf::Color(
                        sqrt(sqrt((double)(256 - (uint8_t)step))) * 64, 
                        sqrt(sqrt((double)(256 - (uint8_t)step))) * 64, 
                        0));
        }
    }


    pic.create(res.x, res.y);
    pic.update(img);
}

