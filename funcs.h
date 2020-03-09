//
// Created by alexandr on 29.02.20.
//

#ifndef WBCPP_FUNCS_H
#define WBCPP_FUNCS_H

#include <vector>


typedef struct colorAll {
    float colorR;
    float colorG;
    float colorB;
    float colorA;
    float fonColorR;
    float fonColorG;
    float fonColorB;
    float fonColorA;
} _colorAll;

typedef struct border {
    float xmin;
    float ymin;
    float xmax;
    float ymax;
} _border;

typedef struct point {
    float x;
    float y;
} _point;

typedef struct color_t {
    float r;
    float g;
    float b;
    int x;
    int y;
} _color_t;


typedef std::vector<point> points;

//typedef struct image_t{
//    vector <color_t> texture;
//};

typedef struct figure {
    int id;
    bool fordel;
    bool visible;
    std::string name;
    std::string file_image;
    points p;
    point center;
    int thickness;
    border extrem;
    colorAll color;
    int start_image;
    int end_image;
} _figure;


// ================
// CIEXYZTRIPLE stuff
typedef int FXPT2DOT30;

typedef struct {
    FXPT2DOT30 ciexyzX;
    FXPT2DOT30 ciexyzY;
    FXPT2DOT30 ciexyzZ;
} CIEXYZ;

typedef struct {
    CIEXYZ ciexyzRed;
    CIEXYZ ciexyzGreen;
    CIEXYZ ciexyzBlue;
} CIEXYZTRIPLE;

// bitmap file header
typedef struct {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} BITMAPFILEHEADER;

// bitmap info header
typedef struct {
    unsigned int biSize;
    unsigned int biWidth;
    unsigned int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    unsigned int biXPelsPerMeter;
    unsigned int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
    unsigned int biRedMask;
    unsigned int biGreenMask;
    unsigned int biBlueMask;
    unsigned int biAlphaMask;
    unsigned int biCSType;
    CIEXYZTRIPLE biEndpoints;
    unsigned int biGammaRed;
    unsigned int biGammaGreen;
    unsigned int biGammaBlue;
    unsigned int biIntent;
    unsigned int biProfileData;
    unsigned int biProfileSize;
    unsigned int biReserved;
} BITMAPINFOHEADER;

// rgb quad
typedef struct {
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved;
} RGBQUAD;

// read bytes
template<typename Type>

void read(std::ifstream &fp, Type &result, std::size_t size) {
    fp.read(reinterpret_cast<char *>(&result), size);
}

// bit extract
unsigned char bitextract(const unsigned int byte, const unsigned int mask);


// ================



void fig_empty(figure);

border border_polyline(points ps);

void draw_circle(float x0, float y0, float r, float cR, float cG, float cB);

void draw_poly(std::list<point> ps, float cR, float cG, float cB, float thin);

void line(float x0, float y0, float x1, float y1, float r, colorAll cAll);

void draw_line(float XX0, float YY0, float XX, float YY, int thin, colorAll cAll);

void draw_circle_fill(float x0, float y0, float r, float cR, float cG, float cB);

void test_draw(colorAll cAll);

void init_buttons(colorAll cAll);

int check_buttons(float x0, float y0);

void draw_buttons(colorAll cAll);

//void draw_pictures(float x0,float y0);
//void insert_screenshot();
unsigned char bitextract(const unsigned int byte, const unsigned int mask);

std::string currentDateToString();


#endif //WBCPP_FUNCS_H
