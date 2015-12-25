#include "homework02.h"
#include "helpers.h"

void drawCircle(double x, double y, double radius)
{
    glBegin(GL_POLYGON);
    for (double i = 0; i < 2 * PI; i += PI / 12)
    {
        glVertex2d(x + cos(i) * radius, y + sin(i) * radius);
    }
    glEnd();
}