#include "testtools.h"

#include <SFML/Graphics.hpp>
#include <immintrin.h>
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
    pic.setSrgb(true);

    const int maxstep = 255;
    const float maxr  = 100.f;

    float xcord = center_pos.x - (res.x * scale) * 0.5f;
    float ycord = center_pos.y - (res.y * scale) * 0.5f;

    __m256 steps  = _mm256_setr_ps(0.f * scale, 1.f * scale,
                                   2.f * scale, 3.f * scale,
                                   4.f * scale, 5.f * scale,
                                   6.f * scale, 7.f * scale);
    __m256 r2_lim = _mm256_set1_ps(maxr);
    
    __m256i cnt_limit = _mm256_set1_epi32(maxstep);
    __m256i mask      = _mm256_set1_epi32(1);
    __m256i zero      = _mm256_set1_epi32(0);
    int* cmp_vect_int = NULL;
    int* cnt_vect_int = NULL;

    float ybuf = ycord;
    float scale_8 = 8 * scale;
    for (size_t xi = 0; xi < res.x; xi++, xcord += scale) {
        ycord = ybuf;
        for (size_t yi = 0; yi < res.y; yi += 8, ycord += scale_8) {
            __m256 cmp_vect  = _mm256_set1_ps(0.f);

            __m256i cnt_vect = _mm256_set1_epi32(0);
            __m256 x_vect    = _mm256_set1_ps(xcord);
            __m256 y_vect    = _mm256_set1_ps(ycord);

            y_vect = _mm256_add_ps(y_vect, steps);

            __m256 x0_vect   = x_vect;
            __m256 y0_vect   = y_vect;

            cmp_vect_int = (int*)&cmp_vect;
            cnt_vect_int = (int*)&cnt_vect;

            for (int i = 0; i < maxstep; i++) {
                int bufsum     = 0;

                __m256 x2_vect = _mm256_mul_ps(x_vect, x_vect);
                __m256 y2_vect = _mm256_mul_ps(y_vect, y_vect);
                __m256 xy_vect = _mm256_mul_ps(x_vect, y_vect);
                
                cmp_vect = _mm256_cmp_ps(_mm256_add_ps(x2_vect, y2_vect), r2_lim, _CMP_LT_OQ);
                //cmp_vect = _mm256_and_ps(cmp_vect, _mm256_castsi256_ps(mask));              // here we get last bytes

                for (int j = 0; j < 8; j++) {
                    if (cmp_vect_int[j]) {
                        cnt_vect_int[j]++;
                        bufsum++;
                    }
                }

                if (!bufsum) {
                    break;
                }

                x_vect = _mm256_add_ps(_mm256_sub_ps(x2_vect, y2_vect), x0_vect);
                y_vect = _mm256_add_ps(_mm256_add_ps(xy_vect, xy_vect), y0_vect);
            }

            cmp_vect_int = (int*)&cmp_vect;
            for (int i = 0; i < 8; i++) {
                img.setPixel(xi, res.y - yi - i - 1, sf::Color(
                        255 - (uint8_t)cnt_vect_int[i],
                        255 - (uint8_t)cnt_vect_int[i], 
                        0));
            }
        }
    }

    if (pic.getSize() != res) {
        pic.create(res.x, res.y);
    }
    pic.update(img);
}

