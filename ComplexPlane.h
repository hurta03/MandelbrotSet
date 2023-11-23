#include <iostream>
#include <cmath>
#include <complex>
#include <SFML/Graphics.hpp>
#include <cstdlib>

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

using namespace sf;
using namespace std;

enum class State {CALCULATING, DISPLAYING};

class ComplexPlane : public Drawable
{
    private:
        VertexArray m_vArray;
        State m_State;
        Vector2f m_mouseLocation;
        Vector2i m_pixel_size;
        Vector2f m_plane_center;
        Vector2f m_plane_size;
        int pixelH, pixelW;
        int m_zoomCount;
        float m_aspectRatio;

        size_t  countIterations(Vector2f coord);
        void iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b);
        Vector2f mapPixelToCoords(Vector2i mousePixel);

    public:
        ComplexPlane(int pixelWidth, int pixelHeight);
        void ComplexPlane::draw(RenderTarget& target, RenderStates states) const;
        void updateRender();
        void zoomIn();
        void zoomOut();
        void setCenter(Vector2i mousePixel);
        void setMouseLocation(Vector2i mousPixel);
        void loadText(Text& text);
        State getState(){return m_State; };
};
