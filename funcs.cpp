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



void draw_button(int id_button, colorAll cAll){
    int width = 32;
    int height = 32;
    int k = 0;
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            float r = ar[w][h][0];
            float g = ar[w][h][1];
            float b = ar[w][h][2];
            if (r*g*b>0.9){
                r=cAll.fonColorR;
                g=cAll.fonColorG;
                b=cAll.fonColorB;
            }
            glColor3f(r,g,b);
            glVertex2d(w + 6 + k, -h +38);
        }
    }
    k = 35;
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            float r = hand[w][h][0];
            float g = hand[w][h][1];
            float b = hand[w][h][2];
            if (r*g*b>0.9){
                r=cAll.fonColorR;
                g=cAll.fonColorG;
                b=cAll.fonColorB;
            }
            glColor3f(r,g,b);
            glVertex2d(w + 6 + k, -h +38);
        }
    }
    k = 70;
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            float r = pen[w][h][0];
            float g = pen[w][h][1];
            float b = pen[w][h][2];
            if (r*g*b>0.9){
                r=cAll.fonColorR;
                g=cAll.fonColorG;
                b=cAll.fonColorB;
            }
            glColor3f(r,g,b);
            glVertex2d(w + 6 + k, -h +38);
        }
    }
    k = 105;
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            float r = err[w][h][0];
            float g = err[w][h][1];
            float b = err[w][h][2];
            if (r*g*b>0.9){
                r=cAll.fonColorR;
                g=cAll.fonColorG;
                b=cAll.fonColorB;
            }
            glColor3f(r,g,b);
            glVertex2d(w + 6 + k, -h +38);
        }
    }
}


typedef struct buttons_t{
    int id;
    string name;
    float* btn;
}  ;
buttons_t button[3];

void aaa(){
    button[0].id = 1;
    button[0].btn = (float *) ar;
    button[1].id = 2;
    button[1].btn = (float *) hand;
}

//void draw_but(){
//    int width = 32;
//    int height = 32;
//    int k = 0;
//    for (int k=0;k<3;k++) {
//        for (int h = 0; h < height; h++) {
//            for (int w = 0; w < width; w++) {
//                float r = button[k].btn
//                float b = ar[w][h][2];
//                glColor3f(r, g, b);
//                glVertex2d(w + 6 + k, -h + 38);
//            }
//        }
//    }
//}

void test_draw(colorAll cAll) {
//    aaa();
    glBegin(GL_POINTS);
//    draw_but();
    draw_button(0, cAll);
    glEnd();
//    glFlush();
}
