#include "ComplexPlane.h"
#include <sstream>
ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
    /*
    Assign m_pixelWidth with the parameter values
    Calculate and assign the aspect ratio of the monitor, m_aspectRatio
    height / width
    Be careful of integer divide
    Our object needs to know this so the plane is not distorted
    */
    pixelW = pixelWidth;
    pixelH = pixelHeight;
    m_aspectRatio = pixelWidth / pixelHeight;
    m_plane_center = { 0,0 };
    m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
    m_zoomCount = 0;
    m_State = State::CALCULATING;
    m_vArray.setPrimitiveType(Points);
    m_vArray.resize(pixelWidth * pixelHeight);

    //sd
    /*
    Initialize VertexArray
    We will use this to draw a color for each pixel
    Set its primitive type to Points
    Resize it to pixelWidth* pixelHeight
    */
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(m_vArray);
}

void ComplexPlane::updateRender()
{
    int j, i;
    Uint8 r;
    Uint8 g;
    Uint8 b;
    if (m_State == State::CALCULATING)
    {
        for (j = 0; j < pixelW; j++)
        {
            for (i = 0; i < pixelH; i++)
            {
                m_vArray[j + i * pixelW].position = { (float)j,(float)i };
                Vector2i v(j, i);
                Vector2f mPixToCoord = ComplexPlane::mapPixelToCoords(v);
                int fIter = ComplexPlane::countIterations(mPixToCoord);
                ComplexPlane::iterationsToRGB(fIter, r, g, b);
                m_vArray[j + i * pixelW].color = { r,g,b };

            }
        }
        m_State = State::DISPLAYING;
    }
}

void ComplexPlane::zoomIn()
{
    double x, y;
    m_zoomCount++;
    x = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
    y = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));
    m_plane_size = { Vector2f(x, y) };
    m_State = State::CALCULATING;

}
void ComplexPlane::zoomOut()
{
    double x, y;
    m_zoomCount--;
    x = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
    y = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));
    m_plane_size = { Vector2f(x, y) };
    m_State = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
    Vector2f co = ComplexPlane::mapPixelToCoords(mousePixel);
    m_plane_center = co;
    m_State = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
    Vector2f loc = ComplexPlane::mapPixelToCoords(mousePixel);
    m_mouseLocation = loc;
}
void ComplexPlane::loadText(Text& text)
{
    //Use a stringstream and the corresponding member variables to create the following output...
    stringstream fo;
    fo << "Mandelbrot Set" << endl
        << "Center: " << m_plane_center.x << " " << m_plane_center.y << endl
        << "Cursor: " << m_mouseLocation.x << " " << m_mouseLocation.y << endl
        << "Left-Click to zoom in" << endl
        << "Right-Click to zoom out" << endl
        << " Size: " << m_plane_size.x << " " << m_plane_size.y << endl;
    text.setString(fo.str());
    
}

size_t ComplexPlane::countIterations(Vector2f coord)
{
    double re = coord.x;
    double im = coord.y;
    complex<double> c(re, im);
    complex<double> z(0, 0);
    int iterations = 0;
    for (int i = 0; i < MAX_ITER; i++)
    {
        //cout << coord.x << " " << coord.y << endl;
        z = z * z + c;
        if (abs(z) > 2)
        {
            iterations++;
        }
    }

    return iterations;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
    //cout << count << endl;
    if (count == MAX_ITER)
    {r = 0; g = 0; b = 0;} // Black

    else if (count > MAX_ITER / 1.5 && count < MAX_ITER)
    {r = 25; g = 25; b = 255;} // Blue

    else if (count > MAX_ITER / 2 && count < MAX_ITER / 1.5)
    { r = 125; g = 255; b = 255;} // 

    else if (count > 0 && count < MAX_ITER / 2)
    {r = 255; g = 25; b = 255;} // Light Blue

    else if (count <= 0)
    { r = 25; g = 25; b = 25;} // Grey
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
    float xPoint = mousePixel.x;
    float yPoint = mousePixel.y;
    m_plane_size;
    m_aspectRatio;
    int Max_X = m_aspectRatio * -1;
    int Max_Y = abs(m_aspectRatio);
    //((n  - a) / (b - a)) * (d - c) + c

    float x = ((xPoint - 0) / (pixelW - 0)) * (m_plane_size.x) + (m_plane_center.x - m_plane_size.x / 2.0);
    float y = ((yPoint - pixelH) / (0 - pixelH)) * (m_plane_size.y) + (m_plane_center.y - m_plane_size.y / 2.0);

    //cout << x << " " << y << endl;

    return Vector2f(x, y);
}
