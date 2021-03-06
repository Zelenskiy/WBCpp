//
// Created by alexandr on 29.02.20.
//

#ifndef WBCPP_FUNCS_H
#define WBCPP_FUNCS_H

#include <vector>
//#include "buttons.h"

//bool isPrev =true;
//
//float WinWid = 800.0;
//float WinHei = 600.0;




typedef struct colorAll {
    float colorR=0;
    float colorG=0;
    float colorB=0;
    float colorA=1.0;
    float fonColorR=0;
    float fonColorG=0;
    float fonColorB=0;
    float fonColorA=0;
} _colorAll;



typedef struct border {
    float xmin=0;
    float ymin=0;
    float xmax=0;
    float ymax=0;
} _border;

typedef struct point {
    float x=0;
    float y=0;
} _point;

typedef struct color_t {
    float r=0;
    float g=0;
    float b=0;
    int x=0;
    int y=0;
} _color_t;


typedef std::vector<point> points;

//typedef struct image_t{
//    vector <color_t> texture;
//};

typedef struct figure {
    int id;
    bool fordel = false;
    bool visible = true;
    std::string name = "";
    std::string file_image = "";
    points p;
    point center;
    int thickness = 2;
    border extrem;
    colorAll color;
    GLuint start_image = 0;
    int end_image = 0;
    bool select = false;
} _figure;

typedef std::list<figure> figures_t;



void fig_empty(figure);

border border_polyline(points ps);

void draw_circle(float x0, float y0, float r, float cR, float cG, float cB, int contur, int fill);

void draw_poly(std::list<point> ps, float cR, float cG, float cB, float thin);

void line(float x0, float y0, float x1, float y1, float r, colorAll cAll);

void draw_line(float XX0, float YY0, float XX, float YY, int thin, colorAll cAll);

void draw_circle_fill(float x0, float y0, float r, float cR, float cG, float cB);

void init_buttons(float w, float h, colorAll cAll, std::list<figure> figures) ;

int check_buttons(float x0, float y0, colorAll &tmpColorAll);

void draw_buttons(float w, float h, colorAll cAll, std::list<figure> figures) ;

void draw_rectangle_fill(float x0, float y0, float x, float y, float r, float g, float b);

void draw_line_for_preview(float XX0, float YY0, float XX, float YY, int thin, colorAll cAll);

std::string left_symR(std::string s, char subs);


//void draw_pictures(float x0,float y0);
//void insert_screenshot();
unsigned char bitextract(const unsigned int byte, const unsigned int mask);

std::string currentDateToString();

std::string trim(std::string s);

std::string right_sym(std::string s, std::string subs);

std::string left_sym(std::string s, std::string subs);

void draw_grid(int w, int h);

void save_figures(std::list<figure> figures);

std::string trim (std::string s, char c);

void set_color_button(float col[5][3]);
void draw_rectangle(float x0, float y0, float x, float y, float thin, float r, float g, float b);

template <typename T>
std::string toString(T val);

//void draw_texture(GLuint textura_id, border b);

#endif //WBCPP_FUNCS_H
