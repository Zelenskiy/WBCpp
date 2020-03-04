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



void fig_empty(figure);
border border_polyline(points ps);
void draw_circle(float x0, float y0, float r, float cR, float cG, float cB);
void draw_poly(list <point> ps, float cR, float cG, float cB, float thin);
void line(int x0, int y0, int x1, int y1, float r, colorAll cAll);
void draw_line(int XX0, int YY0, int XX, int YY, int thin, colorAll cAll) ;
void draw_circle_fill(float x0, float y0, float r, float cR, float cG, float cB);
void test_draw();




#endif //WBCPP_FUNCS_H
