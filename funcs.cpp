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
#include <ctime>
#include <fstream>


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


void draw_circle(float x0, float y0, float r, float cR, float cG, float cB, int contur, int fill) {
    if (fill==1) {
        glColor3f(cR, cG, cB);
        glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i < 50; i++) {
            float a = (float) i / 50.0f * 3.1415f * 2.0f;
            glVertex2f(cos(a) * r + x0, sin(a) * r + y0);
        }
        glEnd();
    }
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    if (contur == 1){
        for (int i = 0; i < 50; i++) {
            float a = (float) i / 50.0f * 3.1415f * 2.0f;
            glVertex2f(cos(a) * r + x0, sin(a) * r + y0);
        }
    }
    glEnd();
}


void draw_poly(list<point> ps, float cR, float cG, float cB, float thin) {
    for (auto p:ps) {
        float x = p.x;
        float y = p.y;
        float r = thin / 2;
        draw_circle(x, y, r, cR, cG, cB, 0,1);
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
        draw_circle(x0, y0, r, cAll.colorR, cAll.colorG, cAll.colorB,0,1);
        for (int x = x0 + sx, y = y0, i = 1; i <= dx; i++, x += sx) {
            if (d > 0) {
                d += d2;
                y += sy;
            } else d += d1;
            draw_circle(x, y, r, cAll.colorR, cAll.colorG, cAll.colorB,0,1);
        }
    } else {
        int d = (dx << 1) - dy;
        int d1 = dx << 1;
        int d2 = (dx - dy) << 1;
        draw_circle(x0, y0, r, cAll.colorR, cAll.colorG, cAll.colorB,0,1);
        for (int y = y0 + sy, x = x0, i = 1; i <= dy; i++, y += sy) {
            if (d > 0) {
                d += d2;
                x += sx;
            } else
                d += d1;
            draw_circle(x, y, r, cAll.colorR, cAll.colorG, cAll.colorB,0,1);
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


string button_name[BUTTONS_COUNT] = {"ar", "hand", "pen", "er", "line", "minimize", "close"};
int button_index[BUTTONS_COUNT] = {8, 20, 1, 2, 3, -1, -2};
int button_visible[BUTTONS_COUNT] = {1, 1, 1, 1, 1, 1, 1};

//void set_rgb(float r, float g, float b, int x, int y, float* var){
//
//}

void draw_but(int i, float x0, float y0, colorAll cAll) {
    float r, g, b;
    float w = 32;
    float h = 32;
    for (int y = h - 1; y >= 0; y--) {
        for (int x = 0; x < w; x++) {
            r = btn[i][x][y][0];
            g = btn[i][x][y][1];
            b = btn[i][x][y][2];
            if (r * g * b > 0.9) {
                r = cAll.fonColorR;
                g = cAll.fonColorG;
                b = cAll.fonColorB;
            }
            glColor3f(r, g, b);
            glVertex2d(x + x0, y + y0);
        }
    }
}

int check_buttons(float x0, float y0) {
    for (int i = 0; i < BUTTONS_COUNT; i++) {
        if ((x0 > buttons[i].x0) && (x0 < buttons[i].x0 + buttons[i].w) &&
            (y0 > buttons[i].y0) && (y0 < buttons[i].y0 + buttons[i].h) &&
            (button_visible[i] == 1)) {
            return buttons[i].tool;
        }
    }
    return 0;
}

void draw_buttons(colorAll cAll) {
    glBegin(GL_POINTS);
    for (int i = 0; i < BUTTONS_COUNT; i++) {
        if (button_visible[i] == 1)
            draw_but(i, buttons[i].x0, buttons[i].y0, cAll);

    }
    glEnd();
}

void init_buttons(colorAll cAll) {
    float k = 4;
    for (int i = 0; i < BUTTONS_COUNT; i++) {
        buttons[i].tool = button_index[i];
        buttons[i].x0 = k;
        buttons[i].y0 = 4;
        buttons[i].w = 32;
        buttons[i].h = 32;
        buttons[i].align = "left";
        buttons[i].name = button_name[i];
        k += 36;
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

std::string currentDateToString(){
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%Y-%m-%d_%H_%M_%S",timeinfo);
    std::string str(buffer);

    return str;
}

std::string trim (std::string s){
    // trim
    while (s[0] == ' '){
        s = s.erase(0,1);
    }
    while (s[s.size()-1] == ' '){
        s = s.erase(s.size()-1,1);
    }
    return s;
}
std::string trim (std::string s, char c){
    // trim
    while (s[0] == c){
        s = s.erase(0,1);
    }
    while (s[s.size()-1] == c){
        s = s.erase(s.size()-1,1);
    }
    return s;
}


std::string right_sym (std::string s, std::string subs ){
    int n = s.find(subs);
    if (n!=-1){
        s = s.substr(n+1);
        s = trim(s);
        return s;
    }
    return "";
}
std::string left_sym (std::string s, std::string subs ){
    int n = s.find(subs);
    if (n!=-1){
        s = s.substr(0, n);
        s = trim(s);
        return s;
    }
    return "";
}

void draw_grid(int w, int h){
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0101);
    glColor3f(0.6,0.6,0.6);
    glLineWidth(1);
    glBegin(GL_LINES);
    int step = 50;
    for (float y=0; y<h; y +=step) {
        glVertex2f(0, y);
        glVertex2f(w, y);
    }
    for (float x=0; x<w; x +=step) {
        glVertex2f(x, 0);
        glVertex2f(x, h);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);

}


void save_figures(std::list<figure> figures){
    std::string comma;
    std::ofstream fout("tmp/figs.json");
    fout << "{" << std::endl;
    fout << "    \"count\": "<<figures.size()<<"," << std::endl;
    fout << "    \"figures\": "<<"[" << std::endl;
    int j = 0;
    int countF = figures.size();
    for (auto f: figures){
        f.name = trim(f.name,'\"');
        f.file_image = trim(f.file_image,'\"');
        j++;
        fout << "      {" << std::endl;
        fout << "      \"id\": "<<f.id<<"," << std::endl;
        fout << "      \"fordel\": "<<f.fordel<<"," << std::endl;
        fout << "      \"visible\": "<<f.visible<<"," << std::endl;
        fout << "      \"name\": \""<<f.name<<"\"," << std::endl;
        fout << "      \"file_image\": \""<<f.file_image<<"\"," << std::endl;
        fout << "      \"p\": [" << std::endl;
        int i = 0; int count = f.p.size();
        for (auto p: f.p) {
            i++;
            fout << "        {" << std::endl;
            fout << "          \"x\": " << p.x << "," << std::endl;
            fout << "          \"y\": " << p.y << "" << std::endl;
            if (i < count) comma = ","; else comma = "";
            fout << "        }"<< comma << std::endl;
        }
        fout << "      ]," << std::endl;
        fout << "      \"center\": {" << std::endl;
        fout << "        \"x\": "<<f.center.x<<"," << std::endl;
        fout << "        \"y\": "<<f.center.y<<"" << std::endl;
        fout << "      }," << std::endl;
        fout << "      \"thickness\": "<<f.thickness<<"," << std::endl;
        fout << "      \"extrem\": {"<< std::endl;
        fout << "        \"xmin\": "<<f.extrem.xmin<<"," << std::endl;
        fout << "        \"ymin\": "<<f.extrem.ymin<<"," << std::endl;
        fout << "        \"xmax\": "<<f.extrem.xmax<<"," << std::endl;
        fout << "        \"ymax\": "<<f.extrem.ymax<<"" << std::endl;
        fout << "      },"<< std::endl;
        fout << "      \"color\": {" << std::endl;
        fout << "        \"colorR\": "<<f.color.colorR<<"," << std::endl;
        fout << "        \"colorG\": "<<f.color.colorG<<"," << std::endl;
        fout << "        \"colorB\": "<<f.color.colorB<<"," << std::endl;
        fout << "        \"colorA\": "<<f.color.colorA<<"," << std::endl;
        fout << "        \"fonColorR\": "<<f.color.fonColorR<<"," << std::endl;
        fout << "        \"fonColorG\": "<<f.color.fonColorG<<"," << std::endl;
        fout << "        \"fonColorB\": "<<f.color.fonColorB<<"" << std::endl;
        fout << "      }," << std::endl;
        fout << "      \"start_image\": "<<f.start_image<<"," << std::endl;
        fout << "      \"end_image\": "<<f.end_image<<"" << std::endl;
        if (j < countF) comma = ","; else comma = "";
        fout <<"        }"<< comma << std::endl;
    }
    fout <<"      ]" << std::endl;
    fout << "}" << std::endl;
    fout.close();
}


