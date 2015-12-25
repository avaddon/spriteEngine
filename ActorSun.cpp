#include "homework02.h"
#include "ActorSun.h"
#include "helpers.h"

ActorSun::ActorSun(double x, double y, double size) :
    GenericActor(x, y),
    m_size(size),
    m_angle(0),
    m_rotaryStartAngle(0),
    m_rotaryEndAngle(0)
{
    RecalcAngles();
}

ActorSun::~ActorSun()
{
}

void ActorSun::Draw()
{
    double radius = m_size / 3;

    glColor3f(1.0f, 1.0f, 0.0f);
    drawCircle(m_x, m_y, radius);

    double length = radius;
    double rays = 16;
    double theta = m_rotaryStartAngle;
    double angleAmount = (m_rotaryEndAngle - m_rotaryStartAngle) / rays;

    for (int i = 0; i < rays; i++)
    {
        float x1 = m_x + cos(theta) * radius * 1.2f;
        float y1 = m_y + sin(theta) * radius * 1.2f;
        float x2 = x1 + cos(theta) * length;
        float y2 = y1 + sin(theta) * length;
        theta += angleAmount;
        glBegin(GL_LINES);
        glVertex2d(x1, y1);
        glVertex2d(x2, y2);
        glEnd();
    }
}

void ActorSun::Animate()
{
    Draw();

    if (m_animated)
    {
        m_angle += 0.005f;
        RecalcAngles();
    }
}

void ActorSun::RecalcAngles()
{
    m_rotaryStartAngle = m_angle * PI / 180;
    m_rotaryEndAngle = (m_angle + 360) * PI / 180;
}