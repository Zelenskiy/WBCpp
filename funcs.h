//
// Created by alexandr on 29.02.20.
//

#ifndef WBCPP_FUNCS_H
#define WBCPP_FUNCS_H

#include <vector>

using namespace std;

typedef struct colorAll {
   float colorR;
   float colorG;
   float colorB;
   float colorA;
   float fonColorR;
   float fonColorG;
   float fonColorB;
   float fonColorA;
}_colorAll;

typedef struct border {
    float xmin;
    float ymin;
    float xmax;
    float ymax;
}_border;

typedef struct point {
    float x;
    float y;
}_point;

typedef vector <point> points;

typedef struct figure {
    int id;
    bool fordel;
    bool visible;
    string name;
    points p;
    point center;
    int thickness;
    border extrem;
    colorAll color;
}_figure;

typedef struct _AUX_RGBImageRec {
    GLint sizeX, sizeY;
    unsigned char *data;
} AUX_RGBImageRec;


void fig_empty(figure);
border border_polyline(points ps);
void draw_circle(float x0, float y0, float r, float cR, float cG, float cB);
void draw_poly(list <point> ps, float cR, float cG, float cB, float thin);
void line(float x0, float y0, float x1, float y1, float r, colorAll cAll);
void draw_line(float XX0, float YY0, float XX, float YY, int thin, colorAll cAll) ;
void draw_circle_fill(float x0, float y0, float r, float cR, float cG, float cB);
void test_draw(colorAll cAll);
void init_buttons(colorAll cAll);
int check_buttons(float x0, float y0);
void draw_buttons(colorAll cAll);
void insert_screenshot();



#endif //WBCPP_FUNCS_H
