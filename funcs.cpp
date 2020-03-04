#include <iostream>
#include <map>
#include <list>
#include <GL/glut.h>
#include <math.h>
#include <list>
#include "funcs.h"
//#include <SOIL/SOIL.h>
//#include <png.h>
#include "buttons.h"


using namespace std;

//typedef struct point {
//    float x;
//    float y;
//}_point;
//
//typedef struct border {
//    float xmin;
//    float ymin;
//    float xmax;
//    float ymax;
//}_border;
//
//typedef struct point {
//    float x;
//    float y;
//}_point;
//
//typedef list <point> points;
//
//typedef struct figure {
//    int id;
//    bool fordel;
//    bool visible;
//    string name;
//    points p;
//    point center;
//    int thickness;
//    border extrem;
//}_figure;
//

border border_polyline(points ps) {
    border c;
    if (ps.size() > 0)
        c.xmax = -1000000000;
    c.ymax = -1000000000;
    c.xmin = 1000000000;
    c.ymin = 1000000000;
    for (point p: ps) {
        if (p.x > c.xmax)
            c.xmax = p.x;
        if (p.y > c.ymax)
            c.ymax = p.y;
        if (p.x < c.xmin)
            c.xmin = p.x;
        if (p.y < c.ymin)
            c.ymin = p.y;
    }
    return c;
}


void draw_circle(float x0, float y0, float r, float cR, float cG, float cB) {
    glColor3f(cR, cG, cB);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < 50; i++) {
        float a = (float) i / 50.0f * 3.1415f * 2.0f;
        glVertex2f(cos(a) * r + x0, sin(a) * r + y0);
    }
    glEnd();
}

void draw_circle_fill(float x0, float y0, float r, float cR, float cG, float cB) {
//    glColor3f(cR, cG, cB);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 50; i++) {
        float a = (float) i / 50.0f * 3.1415f * 2.0f;
        glVertex2f(cos(a) * r + x0, sin(a) * r + y0);
    }
    glEnd();
}


void draw_poly(list<point> ps, float cR, float cG, float cB, float thin) {
    for (auto p:ps) {
        float x = p.x;
        float y = p.y;
        float r = thin / 2;
        draw_circle(x, y, r, cR, cG, cB);
    }


}


void line(int x0, int y0, int x1, int y1, float r, colorAll cAll) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x1 >= x0 ? 1 : -1;
    int sy = y1 >= y0 ? 1 : -1;
    if (dy <= dx) {
        int d = (dy << 1) - dx;
        int d1 = dy << 1;
        int d2 = (dy - dx) << 1;
        draw_circle(x0, y0, r, cAll.colorR, cAll.colorG, cAll.colorB);
        for (int x = x0 + sx, y = y0, i = 1; i <= dx; i++, x += sx) {
            if (d > 0) {
                d += d2;
                y += sy;
            } else d += d1;
            draw_circle(x, y, r, cAll.colorR, cAll.colorG, cAll.colorB);
        }
    } else {
        int d = (dx << 1) - dy;
        int d1 = dx << 1;
        int d2 = (dx - dy) << 1;
        draw_circle(x0, y0, r, cAll.colorR, cAll.colorG, cAll.colorB);
        for (int y = y0 + sy, x = x0, i = 1; i <= dy; i++, y += sy) {
            if (d > 0) {
                d += d2;
                x += sx;
            } else
                d += d1;
            draw_circle(x, y, r, cAll.colorR, cAll.colorG, cAll.colorB);
        }
    }
}

void draw_line(int XX0, int YY0, int XX, int YY, int thin, colorAll cAll) {
    glColor3f(cAll.colorR, cAll.colorG, cAll.colorB);
    glLineWidth(thin);
    glBegin(GL_LINES);
    glVertex3d(XX0, YY0, 0.0);
    glVertex3d(XX, YY, 0.0);
    glEnd();
    glFlush();
}


void test_draw() {
    int width = 32;
    int height = 32;
    // (Horizontal) byte array of bitmap of 32 x 32 px:

    glBegin(GL_POINTS);
    for( int k = 0, n=0; n<3 ;k+=38,n++) {
        for (int w = 0; w < width; w++) {
            for (int h = 0; h < height; h++) {
                glColor3f(tool1_coords[w][h][0], tool1_coords[w][h][1], tool1_coords[w][h][2]);
                glVertex2d(w + 6 + k, h + 6);
            }
        }
    }
    int k = 200;
    for (int w = 0; w < width; w++) {
        for (int h = 0; h < height; h++) {
            glColor3f(palitra[w][h][0], palitra[w][h][1], palitra[w][h][2]);
            glVertex2d(w + 6 + k, h + 6);
        }
    }

    glEnd();

    glFlush();
}
