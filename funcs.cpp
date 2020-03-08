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



void draw_poly(list<point> ps, float cR, float cG, float cB, float thin) {
    for (auto p:ps) {
        float x = p.x;
        float y = p.y;
        float r = thin / 2;
        draw_circle(x, y, r, cR, cG, cB);
    }
}


void line(float x0, float y0, float x1, float y1, float r, colorAll cAll) {
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

void draw_line(float XX0, float YY0, float XX, float YY, int thin, colorAll cAll) {
    glColor3f(cAll.colorR, cAll.colorG, cAll.colorB);
    glLineWidth(thin);
    glBegin(GL_LINES);
    glVertex3d(XX0, YY0, 0.0);
    glVertex3d(XX, YY, 0.0);
    glEnd();
    glFlush();
}



void test_draw(colorAll cAll) {
    init_buttons(cAll);
}



string button_name [BUTTONS_COUNT] = {"ar", "hand", "pen", "er", "line", "minimize", ""};
int button_index [BUTTONS_COUNT] = {8, 20, 1, 2, 3, -1, -2};
int button_visible [BUTTONS_COUNT] = {1, 1, 1, 1, 1, 1, 1};

void draw_but(int i, float x0,float y0, colorAll cAll){
    float r,g,b;
    float w=32;
    float h = 32;
//    for (int y = 0; y < h; y++) {
    for (int y = h-1; y >= 0; y--) {
        for (int x = 0; x < w; x++) {
            switch (i){
                case 0:
                    r = ar[x][y][0];
                    g = ar[x][y][1];
                    b = ar[x][y][2];
                    break;
                case 1:
                    r = hand[x][y][0];
                    g = hand[x][y][1];
                    b = hand[x][y][2];
                    break;
                case 2:
                    r = pen[x][y][0];
                    g = pen[x][y][1];
                    b = pen[x][y][2];
                    break;
                case 3:
                    r = err[x][y][0];
                    g = err[x][y][1];
                    b = err[x][y][2];
                    break;
                case 4:
                    r = lineFig[x][y][0];
                    g = lineFig[x][y][1];
                    b = lineFig[x][y][2];
                    break;
                case 5:
                    r = minimize[x][y][0];
                    g = minimize[x][y][1];
                    b = minimize[x][y][2];
                    break;
                case 6:
                    r = saveApp[x][y][0];
                    g = saveApp[x][y][1];
                    b = saveApp[x][y][2];
                    break;
            }
            if (r*g*b>0.9){
                r=cAll.fonColorR;
                g=cAll.fonColorG;
                b=cAll.fonColorB;
            }
            glColor3f(r,g,b);

            glVertex2d(x+x0, y+y0);

        }
    }
}
int check_buttons(float x0, float y0){
    for (int i=0; i < BUTTONS_COUNT; i++) {
        if ((x0 > buttons[i].x0) && (x0 < buttons[i].x0 + buttons[i].w) &&
            (y0 > buttons[i].y0) && (y0 < buttons[i].y0 + buttons[i].h) &&
                (button_visible[i] == 1) ) {
            return buttons[i].tool;
        }
    }
    return 0;
}

void draw_buttons(colorAll cAll){
    glBegin(GL_POINTS);
    for (int i=0; i < BUTTONS_COUNT; i++){
        if (button_visible[i] == 1)
            draw_but(i,buttons[i].x0,buttons[i].y0, cAll);
    }
    glEnd();
}

void init_buttons(colorAll cAll){
    float k = 4;
    for (int i=0; i < BUTTONS_COUNT; i++){
        buttons[i].tool = button_index[i];
        buttons[i].x0 = k;
        buttons[i].y0 = 4;
        buttons[i].w = 32;
        buttons[i].h = 32;
        buttons[i].align = "left";
        buttons[i].name = button_name[i];
        k+=36;
    }
    draw_buttons(cAll);
}



unsigned char bitextract(const unsigned int byte, const unsigned int mask) {
    if (mask == 0) {
        return 0;
    }

    // определение количества нулевых бит справа от маски
    int
            maskBufer = mask,
            maskPadding = 0;

    while (!(maskBufer & 1)) {
        maskBufer >>= 1;
        maskPadding++;
    }

    // применение маски и смещение
    return (byte & mask) >> maskPadding;
}

