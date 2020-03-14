#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <list>

#include <GL/freeglut.h>
#include <GL/glu.h>
#include <math.h>
#include "funcs.h"
#include <fstream>
//#include "rapidjson/document.h"
//#include "rapidjson/writer.h"
//#include "rapidjson/stringbuffer.h"
#include <bits/stdc++.h>


float WinWid = 800.0;
float WinHei = 600.0;
//float WinWid = 1366.0;
//float WinHei = 768.0;
int window;
float cx = 0;
float cy = 0;
float X;
float Y;
float X0;
float Y0;
float XX0;
float YY0;
bool down;
int id = 0;
float Angle = 0.0, Scale = 1.0;
colorAll cAll;
colorAll cFon;
float old_X0;
float old_Y0;
float old_X;
float old_Y;
bool isGrid = true;
std::list<point> sel_points;

std::list<int> selFigures;

struct textura_struct {
    int W;
    int H;
    unsigned char *Image;
} get_textura;

std::vector<GLuint> textura_id;

std::vector<color_t> texture;

int penWidth = 2;
int tool = 1;
float erWidth = 10;


std::list<figure> figures;
point p;
figure fig;
bool start_select = false;

void insert_screenshot(std::string fileName);

void close_app();

void draw_image(int start, int end);

void load_figures();

GLuint LoadTexture(char *FileName, int &w, int &h);

void draw_texture(GLuint text_nun, point p1, point p2, point p3, point p4);


void figures_is_visible() {
    for (auto &f: figures) {
        if ((f.extrem.xmax - cx > 0) && (f.extrem.xmin - cx < WinWid) &&
            (f.extrem.ymax - cy > 0) && (f.extrem.ymin - cy < WinHei)) {
            f.visible = 1;
        } else {
            f.visible = 0;
        }
    }
}


void Render() {
    glClearColor(cAll.fonColorR, cAll.fonColorG, cAll.fonColorB, cAll.fonColorA);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(cAll.colorR, cAll.colorG, cAll.colorB);
//    glPushMatrix();
    int i = 0;
    for (figure f: figures) {
        if ((f.visible == 1)) {

            if (f.name == "poly") {
                points ps = f.p;
                float XX0 = ps[0].x;
                float YY0 = ps[0].y;
                float XX = ps[1].x;
                float YY = ps[1].y;
                line(XX0 - cx, YY0 - cy, XX - cx, YY - cy, f.thickness, f.color);
            } else if (f.name == "line") {
                points ps = f.p;
                float XX0 = ps[0].x;
                float YY0 = ps[0].y;
                float XX = ps[1].x;
                float YY = ps[1].y;
//                    draw_circle_fill(XX0, YY0, f.thickness, f.color.colorR, f.color.colorG, f.color.colorB);
                line(XX0 - cx, YY0 - cy, XX - cx, YY - cy, f.thickness, f.color);
//                    draw_line(XX0 - cx, YY0 - cy, XX - cx, YY - cy, f.thickness * 2, f.color);
//                    line(XX0, YY0, XX, YY, f.thickness, f.color);
            } else if (f.name == "image") {
                border b;
                points ps = f.p;
                b.xmin = ps[0].x - cx;
                b.ymin = ps[0].y - cy;
                b.xmax = ps[1].x - cx;
                b.ymax = ps[1].y - cy;
//                colorAll c;
//                c.colorR = 1.0;
//                c.colorG = 1.0;
//                c.colorB = 1.0;
                point p1, p2, p3, p4;
                p1.x = ps[0].x - cx;
                p1.y = ps[0].y - cy;
                p2.x = ps[1].x - cx;
                p2.y = ps[1].y - cy;
                p3.x = ps[2].x - cx;
                p3.y = ps[2].y - cy;
                p4.x = ps[3].x - cx;
                p4.y = ps[3].y - cy;


                draw_texture(f.start_image, p1, p2, p3, p4);
//                draw_texture(f.start_image, b);

            }
        }

    }
    //Виділені фігури
    if (tool == 8) {
        //Виділені фігури
        for (auto &f: figures) {
            bool l = false;
            for (auto sf : selFigures) {
                if (sf == f.id) {
                    l = true;
                    break;
                }
            }
            if (l) {
                border b = f.extrem;
                draw_rectangle(b.xmin - 4 - cx, b.ymin - 4 - cy, b.xmax + 4 - cx, b.ymax + 4 - cy, 1, 0, 0);
            }
        }
    }

//        if (selFigures.size() == 1) {
//            int sf = selFigures.back();
//            bool l = false;
//            for (auto &f: figures) {
//                if (sf == f.id) {
//                    l = true;
//                    border b = f.extrem;
//                    draw_rectangle(b.xmin - 4 - cx, b.ymin - 4 - cy, b.xmax + 4 - cx, b.ymax + 4 - cy, 1, 0, 0);
//                    break;
//                }
//            }
//
//        } else {
//            float xmin, xmax, ymin, ymax;
//            xmin = figures.back().extrem.xmin;
//            ymin = figures.back().extrem.ymin;
//            xmax = figures.back().extrem.xmax;
//            ymax = figures.back().extrem.ymax;
//            for (auto &f: figures) {
//                bool l = false;
//                border b;
//                for (auto sf : selFigures) {
//                    if (sf == f.id) {
//                        if (xmin > f.extrem.xmin) xmin = f.extrem.xmin;
//                        if (ymin > f.extrem.ymin) ymin = f.extrem.ymin;
//                        if (xmax < f.extrem.xmax) xmax = f.extrem.xmax;
//                        if (ymax < f.extrem.ymax) ymax = f.extrem.ymax;
//                        break;
//                    }
//                }
//            }
//            draw_rectangle(xmin - 4 - cx, ymin - 4 - cy, xmax + 4 - cx, ymax + 4 - cy, 1, 0, 0);
//
//
//        }




}

void draw_pictures_all() {
    glBegin(GL_POINTS);
    for (auto i = texture.begin(); i != texture.end(); ++i) {
        color_t c = *i;
        glColor3f(c.b, c.g, c.r);
        if ((c.x - cx > 0) && (c.x - cx < WinWid) &&
            (c.y - cy > 0) && (c.y - cy < WinHei)) {
            glVertex2d(c.x - cx, c.y - cy);
        } else {

        }
    }
    glEnd();
}


void draw_image(int start, int end) {
    glBegin(GL_POINTS);
    for (int i = start; i <= end; ++i) {
        color_t c = texture[i];
        glColor3f(c.b, c.g, c.r);
        if ((c.x - cx > 0) && (c.x - cx < WinWid) &&
            (c.y - cy > 0) && (c.y - cy < WinHei)) {
            glVertex2d(c.x - cx, c.y - cy);
        } else {

        }
    }
    glEnd();
}

void renderSpacedBitmapString(
        float x,
        float y,
        int spacing,
        void *font,
        char *string) {
    char *c;
    int x1 = x;
    for (c = string; *c != '\0'; c++) {
        glRasterPos2f(x1, y);
        glutBitmapCharacter(font, *c);
        x1 = x1 + glutBitmapWidth(font, *c) + spacing;
    }
}

void Draw() {
    Render();
//    draw_pictures();
    if (isGrid) {
        draw_grid(WinWid, WinHei);
    }
    char *s;
    int n = selFigures.size();

    std::cout << "figuresCount = " << selFigures.size() << std::endl;
//    renderSpacedBitmapString(5, 60, 0,  GLUT_BITMAP_HELVETICA_18, "");
    draw_buttons(cAll);
    glFlush();
}


void draw_to_figures(int XX0, int YY0, int XX, int YY, colorAll cAll, float thin) {
    point p;
    figure fig;
    p.x = XX0 + cx;
    p.y = YY0 + cy;
    fig.p.push_back(p);
    p.x = XX + cx;
    p.y = YY + cy;
    fig.p.push_back(p);
    ++id;
    fig.id = id;
    fig.center.x = (X0 + X) / 2.0 + cx;
    fig.center.y = (Y0 + Y) / 2.0 + cy;
    if (tool == 1)
        fig.name = "poly";
    else if (tool == 3)
        fig.name = "line";
    fig.fordel = false;
    fig.visible = true;
    fig.color = cAll;
    fig.thickness = thin;
    fig.extrem = border_polyline(fig.p);
    figures.push_back(fig);
}

void figures_update() {
//    std::list<figure> tmpList;
    figures.remove_if([](figure n) { return n.fordel == true; });
//    for (auto f: figures) { f.extrem = border_polyline(f.p);}
}

void Timer(int) {
//    ++Angle;
//    glutPostRedisplay();
//    Draw();
    std::cout << "TIMER" << std::endl;
    std::ifstream file;
    file.open("file.bmp");
    file.close();
    if (file) {

//        glutFullScreen();
//        glutReshapeWindow(WinWid, WinHei - 60);
//        glutShowWindow();
        char * command = "wmctrl -a Hello";
        system(command);
        insert_screenshot("file.bmp");
    }


    glutTimerFunc(2000, Timer, 0);
}

void init_colors() {
    cFon.colorR = 0.6;
    cFon.colorG = 0.8;
    cFon.colorB = 0.4;

    cAll.colorR = 0.2;
    cAll.colorG = 0.5;
    cAll.colorB = 0.9;
    cAll.fonColorR = cFon.colorR;
    cAll.fonColorG = cFon.colorG;
    cAll.fonColorB = cFon.colorB;
    cAll.fonColorA = 1.0;
}

void write_ini() {
    std::ofstream fout("wb.ini");
    fout << "[MAIN]" << std::endl;
    fout << "fonColorR = " << cAll.fonColorR << std::endl;
    fout << "fonColorG = " << cAll.fonColorG << std::endl;
    fout << "fonColorB = " << cAll.fonColorB << std::endl;
    fout << "penColorR = " << cAll.colorR << std::endl;
    fout << "penColorG = " << cAll.colorG << std::endl;
    fout << "penColorB = " << cAll.colorB << std::endl;

    fout << "color0R = " << 0.9 << std::endl;
    fout << "color0G = " << 0.0 << std::endl;
    fout << "color0B = " << 0.0 << std::endl;

    fout << "color1R = " << 0.0 << std::endl;
    fout << "color1G = " << 0.5 << std::endl;
    fout << "color1B = " << 0.0 << std::endl;

    fout << "color2R = " << 0.0 << std::endl;
    fout << "color2G = " << 0.0 << std::endl;
    fout << "color2B = " << 0.9 << std::endl;

    fout << "color3R = " << 0.9 << std::endl;
    fout << "color3G = " << 0.5 << std::endl;
    fout << "color3B = " << 0.0 << std::endl;

    fout << "color4R = " << 0.0 << std::endl;
    fout << "color4G = " << 0.0 << std::endl;
    fout << "color4B = " << 0.0 << std::endl;


    if (isGrid)
        fout << "grid = " << "true" << std::endl;
    else
        fout << "grid = " << "false" << std::endl;
    fout << "penWidth = " << penWidth << std::endl;
    fout << "erWidth = " << erWidth << std::endl;
    fout.close();
}

void read_ini() {
    std::ifstream file_ini;
    file_ini.open("wb.ini");
    if (file_ini) {
        float col[5][3] = {{0, 0, 0},
                           {0, 0, 0},
                           {0, 0, 0},
                           {0, 0, 0},
                           {0, 0, 0}};
        //Читаємо
        while (file_ini) {
            std::string str;
            std::getline(file_ini, str);
            // Обработка строки str
            // Шукаємо перший символ "="
            std::string field = left_sym(str, "=");
            std::string value = right_sym(str, "=");
            if (field == "fonColorR") cFon.colorR = stof(value);
            if (field == "fonColorG") cFon.colorG = stof(value);
            if (field == "fonColorB") cFon.colorB = stof(value);
            if (field == "penColorR") cAll.colorR = stof(value);
            if (field == "penColorG")cAll.colorG = stof(value);
            if (field == "penColorB") cAll.colorB = stof(value);
//            std::vector <colorAll, int> col; col.push_back(stof(value),1);

            if (field == "color0R")
                col[0][0] = stof(value);
            if (field == "color0G")
                col[0][1] = stof(value);
            if (field == "color0B")
                col[0][2] = stof(value);
            if (field == "color1R") col[1][0] = stof(value);
            if (field == "color1G")
                col[1][1] = stof(value);
            if (field == "color1B") col[1][2] = stof(value);
            if (field == "color2R") col[2][0] = stof(value);
            if (field == "color2G") col[2][1] = stof(value);
            if (field == "color2B") col[2][2] = stof(value);
            if (field == "color3R") col[3][0] = stof(value);
            if (field == "color3G") col[3][1] = stof(value);
            if (field == "color3B") col[3][2] = stof(value);
            if (field == "color4R") col[4][0] = stof(value);
            if (field == "color4G") col[4][1] = stof(value);
            if (field == "color4B") col[4][2] = stof(value);

            if (field == "penWidth") penWidth = stof(value);
            if (field == "erWidth") erWidth = stof(value);
            if (field == "grid")
                if ((value == "true") || (value == "True") || (value == "TRUE"))
                    isGrid = true;
                else
                    isGrid = false;
            cAll.fonColorR = cFon.colorR;
            cAll.fonColorG = cFon.colorG;
            cAll.fonColorB = cFon.colorB;

        }
        set_color_button(col);
    } else {
        //Пишемо новий
        write_ini();
    }

    file_ini.close();


}

void init_flags() {
    system("mkdir tmp");
    system("mkdir lessons");
    std::cout << currentDateToString() << std::endl;
    read_ini();
    std::ofstream fout("is_work.txt");
    fout << "Hello. I work!" << std::endl;
    fout.close();


}

void Initialize() {
    init_colors();
    init_flags();

    init_buttons(cAll);
    glClearColor(cAll.fonColorR, cAll.fonColorG, cAll.fonColorB, cAll.fonColorA);
    glMatrixMode(GL_MATRIX_MODE);
    glLoadIdentity();
    glOrtho(0, WinWid, 0, WinHei - 60, -1, 1);

//    FILE* f;
//    char buf[128];
//    f = popen("../ScreenShoot/scrgrub","r");
//    while (fgets(buf,127,f))
//    {
//        printf("%s\n",buf);
//    }
//    fclose(f);
}

float m_s(float y) {
    return WinHei - y - 60;
}


void on_mouse_down_up(int button, int state, int ax, int ay) {
    if (m_s(ay) < 35) {
        // --- check buttons --------
        colorAll tmpColorAll = cAll;
        int t = check_buttons(ax, m_s(ay), tmpColorAll);
        if (t > 0) {
            tool = t;
        } else if (t == -1) { //Згортаємо
            glutIconifyWindow();
        } else if (t == -2) { //Закриваємо
            close_app();
            glutDestroyWindow(window);
        } else if (t < -10) { //Вибираємо колір
            t *= -1;
            cAll.colorR = tmpColorAll.colorR;
            cAll.colorG = tmpColorAll.colorG;
            cAll.colorB = tmpColorAll.colorB;
        }


    } else {
        // -------------------------

        if (button == GLUT_LEFT_BUTTON) {
            old_X0 = 0;
            old_Y0 = 0;
            old_X = ax;
            old_Y = m_s(ay);
            if (state == GLUT_DOWN) {
                start_select = false;
                down = true;
                X0 = ax;
                Y0 = m_s(ay);
                if (tool == 8){
                    //Якщо виділеного немає
                    if (selFigures.size() == 0){
                         bool flag = false;
                         int fig = 0;
                         for (auto f: figures){
                             if ((ax + cx > f.extrem.xmin) && (ax + cx < f.extrem.xmax) &&
                               (m_s(ay) + cy > f.extrem.ymin) && (m_s(ay) + cy < f.extrem.ymax)){
                                flag = true;
                                fig = f.id;
                                break;
                             }
                         }
                         if (!flag){
                        //Якщо під курсором нічого немає
                        //почнемо виділення рамкою
                             start_select = true;
                        } else {
                        //Якщо під курсором є фігура
                        //Виділимо її

                            selFigures.push_back(fig);
                        }
                    } else {
                    //Якщо виділене є
                        points ps;
                        for (auto sF: selFigures){
                            point p;
                            for (auto f: figures){
                                if (f.id == sF) {
                                    p.x = f.extrem.xmin;
                                    p.y = f.extrem.ymin;
                                    ps.push_back(p);
                                    p.x = f.extrem.xmax;
                                    p.y = f.extrem.ymax;
                                    ps.push_back(p);
                                }
                            }
                        }
                        border b = border_polyline(ps);
                        if ((ax + cx > b.xmin) && (ax + cx < b.xmax) &&
                            (m_s(ay) + cy > b.ymin) && (m_s(ay) + cy < b.ymax)){
                            //Якщо курсор оточений виділеним
                            printf("Якщо курсор оточений виділеним\n");

                        } else {
                            //Якщо курсор за межами виділених
                            selFigures.clear();
                        }
                    }
                }

            } else {    // GLUT_UP
                down = false;
                old_X = 0;
                old_Y = 0;
                switch (tool) {
                    case 3:
                        draw_to_figures(X0, Y0, X, Y, cAll, penWidth);
                        break;
                }
                figures_is_visible();
                figures_update();
            }
            if (tool > 1)
                Draw();

        } else if (button == 3) { //scrolling
            cy += 1;

        } else if (button == 4) { //scrolling
            cy -= 1;

        }
        figures_is_visible();
        glutPostRedisplay();
    }

}


void on_mouse_drag(int ax, int ay) {
    if (down) {//                Draw();
        X = ax;
        Y = m_s(ay);
        float dx = (old_X - X);
        float dy = (old_Y - Y);
        switch (tool) {
            case 1:                             // Ручка
                draw_to_figures(X0, Y0, X, Y, cAll, penWidth);
                line(X0, Y0, X, Y, penWidth, cAll);
                X0 = X;
                Y0 = Y;
                break;
            case 2:                             // Гумка
//                draw_circle(X0, Y0, erWidth+10, cAll.fonColorR, cAll.fonColorG, cAll.fonColorB,0,1);
                draw_circle(X0, Y0, erWidth, cAll.fonColorR, cAll.fonColorG, cAll.fonColorB, 0, 1);
//                draw_circle(X0, Y0, erWidth, 0.8, 1.0, 0.8,1);
                for (figure &f:figures) {
                    border c = f.extrem;
                    if ((abs(X + cx - f.center.x) < erWidth) && (abs(Y + cy - f.center.y) < erWidth)) {
                        f.fordel = true;
                        f.visible = false;
                        break;
                    }
                }
                figures_update();
                X0 = X;
                Y0 = Y;
                break;
            case 3:                     //Лінія
                draw_line(old_X0, old_Y0, old_X, old_Y, penWidth * 2, cFon);
                draw_line(X0, Y0, X, Y, penWidth * 2, cAll);
                old_X0 = X0;
                old_Y0 = Y0;
                old_X = X;
                old_Y = Y;
                break;
            case 20:                    // Тягаємо полотно
                cx += (old_X - X);
                cy += (old_Y - Y);
                old_X = X;
                old_Y = Y;
                glutPostRedisplay();
                break;
            case 8:                    // Тягаємо виділене
                if (!start_select) {
                    for (auto &f: figures) {
                        bool l = false;
                        for (auto sf : selFigures) {
                            if (sf == f.id) {
                                l = true;
                                break;
                            }
                        }
                        if (l) {
                            for (auto &p: f.p) {
                                p.x -= (dx);
                                p.y -= (dy);
                            }
                            f.extrem = border_polyline(f.p);
                            f.center.x -= (dx);
                            f.center.y -= (dy);
                        }
                    }
                } else { //Процес виділення
                    draw_rectangle(X0, Y0, X, Y, 1, 0, 0);
                    float xx0 = std::fmin(X0, X);
                    float xx = std::fmax(X0, X);
                    float yy0 = std::fmin(Y0, Y);
                    float yy = std::fmax(Y0, Y);
                    selFigures.clear();
                    for (auto f: figures) {
                        point b;
                        b.x = (f.extrem.xmin + f.extrem.xmax)/2;
                        b.y = (f.extrem.ymin + f.extrem.ymax)/2;

                        if ((xx0+cx < b.x) && (xx+cx > b.x) && (yy0+cy < b.y) && (yy+cy > b.y)) {
                            selFigures.push_back(f.id);
                        }
                    }
                }


                old_X = X;
                old_Y = Y;
                glutPostRedisplay();
                break;
        }
    }
    XX0 = X;
    YY0 = Y;
    glFlush();
}


void on_keypress(unsigned char key, int x, int y) {
    std::cout << (int) key << std::endl;
    switch ((int) key) {   //ESC
        case 27:
            std::cout << "ESC" << std::endl;
            close_app();
            glutDestroyWindow(window);
            break;
        case 112:
            tool = 1;
            break;
        case 101:
            tool = 2;
            break;
//        case 108:
//            tool = 3;
//            break;
        case 97:
            test_draw(cAll);
            break;
        case 115:   //S
            insert_screenshot("file.bmp");
            break;
        case 108:   //L
            load_figures();
            break;
        case 127:
            figures.clear();
            Draw();
            break;
        case 102: //F
            glutFullScreen();
            break;
    }
}


void keySpecialUp(int key, int x, int y) {
    std::cout << "key " << key << std::endl;
    switch (key) {   //UP
        case 101:
            cy += 100;
            figures_is_visible();
            break;
        case 103: //LEFT
            cy -= 100;
            figures_is_visible();
            break;
        case 100:
            cx -= 100;
            figures_is_visible();
            break;
        case 102: //RIGHT
            cx += 100;
            figures_is_visible();
            break;
    }
    Draw();
}

void close_app() {
    std::ifstream file;
    file.open("is_work.txt");
    file.close();
    if (file) {
        std::cout << "Удаляем файл is_work.txt.\n";
        int n;
        n = remove("is_work.txt");
    }
    file.open("flag.txt");
    file.close();
    if (file) {
        std::cout << "Удаляем файл flag.txt.\n";
        int n;
        n = remove("flag.txt");
    }


    //Зберігаємо фігури до файлу
    save_figures(figures);


    //Зберігаємо налаштування
    write_ini();
    //архівуємо файли, формуємо унікальне ім'я
    std::string name_zip = "lessons/" + currentDateToString() + ".zip";

    std::string command = "zip -m " + name_zip + " tmp/*.*";
    system(command.c_str());

}

void on_window_status(int status) {
    if (status == GLUT_HIDDEN) {
        std::cout << "Згорнуло" << std::endl;
        std::ofstream fout("flag.txt");
        fout.close();
    } else {
        std::ifstream file;
        file.open("flag.txt");
        file.close();
        if (file) {
            std::cout << "Удаляем файл flag.txt.\n";
            int n;
            n = remove("flag.txt");
        }
    }
}


void on_exit() {
    close_app();
    printf("Good by");
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    WinWid = glutGet(GLUT_SCREEN_WIDTH);
    WinHei = glutGet(GLUT_SCREEN_HEIGHT);

    glutInitWindowSize(WinWid, WinHei - 40);
    glutInitWindowPosition(0, 0);
    window = glutCreateWindow("Hello OpenGL");
    glutDisplayFunc(Draw);
    glutKeyboardFunc(on_keypress);
    glutSpecialUpFunc(keySpecialUp);
    glutMouseFunc(on_mouse_down_up);
    glutMotionFunc(on_mouse_drag);
    glutWindowStatusFunc(on_window_status);
    glutCloseFunc(on_exit);


    glutTimerFunc(2000, Timer, 0);
    Initialize();

//    glutHideWindow();
//    glutIconifyWindow();
//    glutShowWindow();
    glutMainLoop();
//
    return 0;
}

void insert_screenshot(std::string fileName) {
    char *cfileName = const_cast<char *>(fileName.c_str());
    int w = 0;
    int h = 0;
    int res = LoadTexture(cfileName, w, h);
    if (res == -1)
        return;
    // Прокручення сторінки до вільного місця
    cx = 0;
    //шукаємо фігуру з найнижчою координатою
    int y = 2000;
    for (auto f:figures) {
        for (auto p: f.p) {
            if (p.y < y) {
                y = p.y;
            }
        }
    }
    cy = y - 50 - WinHei;


    // ======================================
    float y0 = WinHei - h - 60; //400;
    float x0 = WinWid - w;
    figure fig;
//    p.x = x0 + cx;
//    p.y = y0 + cy;
//    fig.p.push_back(p);
//    p.x = x0 + cx + w;
//    p.y = y0 + cy + h;
//    fig.p.push_back(p);
    p.x = x0 + cx;
    p.y = y0 + cy;
    fig.p.push_back(p);
    p.x = x0 + cx;
    p.y = y0 + cy + h;
    fig.p.push_back(p);
    p.x = x0 + cx + w;
    p.y = y0 + cy + h;
    fig.p.push_back(p);
    p.x = x0 + cx + w;
    p.y = y0 + cy;
    fig.p.push_back(p);
    ++id;
    fig.id = id;
    fig.center.x = (X0 + X0 + w) / 2.0 + cx;
    fig.center.y = (Y0 + Y0 + h) / 2.0 + cy;
    fig.name = "image";
    fig.fordel = false;
    fig.visible = true;
    fig.color = cAll;
    fig.thickness = 0;
    fig.extrem = border_polyline(fig.p);
    fig.start_image = textura_id.size() - 1;
    fig.end_image = 0;


    //Вигадуємо унікальне ім'я файлу
    std::string name = currentDateToString() + ".bmp";
    fig.file_image = name;
    figures.push_back(fig);
    // =============
    if (fileName != "tmp/" + name) {
        std::cout << "Переміщаємо файл " << name << ".\n";
        int n;

        n = rename(fileName.c_str(), ("tmp/" + name).c_str());
    }


}

void _draw_texture(GLuint text_nun, border b) {
    /* Вывод изображения в окне */
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, textura_id[text_nun]);
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2d(b.xmin, b.ymin);
    glTexCoord2d(0, 1);
    glVertex2d(b.xmin, b.ymax);
    glTexCoord2d(1, 1);
    glVertex2d(b.xmax, b.ymax);
    glTexCoord2d(1, 0);
    glVertex2d(b.xmax, b.ymin);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void draw_texture(GLuint text_nun, point p1, point p2, point p3, point p4) {
    /* Вывод изображения в окне */
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, textura_id[text_nun]);
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2d(p1.x, p1.y);
    glTexCoord2d(0, 1);
    glVertex2d(p2.x, p2.y);
    glTexCoord2d(1, 1);
    glVertex2d(p3.x, p3.y);
    glTexCoord2d(1, 0);
    glVertex2d(p4.x, p4.y);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

//int insert_screenshot_old(std::string fileName) {
//    //char *fileName = "file.bmp";
//    // открываем файл
//    std::ifstream fileStream(fileName, std::ifstream::binary);
//    if (!fileStream) {
//        std::cout << "Error opening file '" << fileName << "'." << std::endl;
//        return 0;
//    }
//
//
//    // заголовк изображения
//    BITMAPFILEHEADER fileHeader;
//    read(fileStream, fileHeader.bfType, sizeof(fileHeader.bfType));
//    read(fileStream, fileHeader.bfSize, sizeof(fileHeader.bfSize));
//    read(fileStream, fileHeader.bfReserved1, sizeof(fileHeader.bfReserved1));
//    read(fileStream, fileHeader.bfReserved2, sizeof(fileHeader.bfReserved2));
//    read(fileStream, fileHeader.bfOffBits, sizeof(fileHeader.bfOffBits));
//
//    if (fileHeader.bfType != 0x4D42) {
//        std::cout << "Error: '" << fileName << "' is not BMP file." << std::endl;
//        return 0;
//    }
//
//    // информация изображения
//    BITMAPINFOHEADER fileInfoHeader;
//    read(fileStream, fileInfoHeader.biSize, sizeof(fileInfoHeader.biSize));
//
//    // bmp core
//    if (fileInfoHeader.biSize >= 12) {
//        read(fileStream, fileInfoHeader.biWidth, sizeof(fileInfoHeader.biWidth));
//        read(fileStream, fileInfoHeader.biHeight, sizeof(fileInfoHeader.biHeight));
//        read(fileStream, fileInfoHeader.biPlanes, sizeof(fileInfoHeader.biPlanes));
//        read(fileStream, fileInfoHeader.biBitCount, sizeof(fileInfoHeader.biBitCount));
//    }
//
//    // получаем информацию о битности
//    int colorsCount = fileInfoHeader.biBitCount >> 3;
//    if (colorsCount < 3) {
//        colorsCount = 3;
//    }
//
//    int bitsOnColor = fileInfoHeader.biBitCount / colorsCount;
//    int maskValue = (1 << bitsOnColor) - 1;
//
//    // bmp v1
//    if (fileInfoHeader.biSize >= 40) {
//        read(fileStream, fileInfoHeader.biCompression, sizeof(fileInfoHeader.biCompression));
//        read(fileStream, fileInfoHeader.biSizeImage, sizeof(fileInfoHeader.biSizeImage));
//        read(fileStream, fileInfoHeader.biXPelsPerMeter, sizeof(fileInfoHeader.biXPelsPerMeter));
//        read(fileStream, fileInfoHeader.biYPelsPerMeter, sizeof(fileInfoHeader.biYPelsPerMeter));
//        read(fileStream, fileInfoHeader.biClrUsed, sizeof(fileInfoHeader.biClrUsed));
//        read(fileStream, fileInfoHeader.biClrImportant, sizeof(fileInfoHeader.biClrImportant));
//    }
//
//    // bmp v2
//    fileInfoHeader.biRedMask = 0;
//    fileInfoHeader.biGreenMask = 0;
//    fileInfoHeader.biBlueMask = 0;
//
//    if (fileInfoHeader.biSize >= 52) {
//        read(fileStream, fileInfoHeader.biRedMask, sizeof(fileInfoHeader.biRedMask));
//        read(fileStream, fileInfoHeader.biGreenMask, sizeof(fileInfoHeader.biGreenMask));
//        read(fileStream, fileInfoHeader.biBlueMask, sizeof(fileInfoHeader.biBlueMask));
//    }
//
//    // если маска не задана, то ставим маску по умолчанию
//    if (fileInfoHeader.biRedMask == 0 || fileInfoHeader.biGreenMask == 0 || fileInfoHeader.biBlueMask == 0) {
//        fileInfoHeader.biRedMask = maskValue << (bitsOnColor * 2);
//        fileInfoHeader.biGreenMask = maskValue << bitsOnColor;
//        fileInfoHeader.biBlueMask = maskValue;
//    }
//
//    // bmp v3
//    if (fileInfoHeader.biSize >= 56) {
//        read(fileStream, fileInfoHeader.biAlphaMask, sizeof(fileInfoHeader.biAlphaMask));
//    } else {
//        fileInfoHeader.biAlphaMask = maskValue << (bitsOnColor * 3);
//    }
//
//    // bmp v4
//    if (fileInfoHeader.biSize >= 108) {
//        read(fileStream, fileInfoHeader.biCSType, sizeof(fileInfoHeader.biCSType));
//        read(fileStream, fileInfoHeader.biEndpoints, sizeof(fileInfoHeader.biEndpoints));
//        read(fileStream, fileInfoHeader.biGammaRed, sizeof(fileInfoHeader.biGammaRed));
//        read(fileStream, fileInfoHeader.biGammaGreen, sizeof(fileInfoHeader.biGammaGreen));
//        read(fileStream, fileInfoHeader.biGammaBlue, sizeof(fileInfoHeader.biGammaBlue));
//    }
//
//    // bmp v5
//    if (fileInfoHeader.biSize >= 124) {
//        read(fileStream, fileInfoHeader.biIntent, sizeof(fileInfoHeader.biIntent));
//        read(fileStream, fileInfoHeader.biProfileData, sizeof(fileInfoHeader.biProfileData));
//        read(fileStream, fileInfoHeader.biProfileSize, sizeof(fileInfoHeader.biProfileSize));
//        read(fileStream, fileInfoHeader.biReserved, sizeof(fileInfoHeader.biReserved));
//    }
//
//    // проверка на поддерку этой версии формата
//    if (fileInfoHeader.biSize != 12 && fileInfoHeader.biSize != 40 && fileInfoHeader.biSize != 52 &&
//        fileInfoHeader.biSize != 56 && fileInfoHeader.biSize != 108 && fileInfoHeader.biSize != 124) {
//        std::cout << "Error: Unsupported BMP format." << std::endl;
//        return 0;
//    }
//
//    if (fileInfoHeader.biBitCount != 16 && fileInfoHeader.biBitCount != 24 && fileInfoHeader.biBitCount != 32) {
//        std::cout << "Error: Unsupported BMP bit count." << std::endl;
//        return 0;
//    }
//
//    if (fileInfoHeader.biCompression != 0 && fileInfoHeader.biCompression != 3) {
//        std::cout << "Error: Unsupported BMP compression." << std::endl;
//        return 0;
//    }
//
//    // rgb info
//    RGBQUAD **rgbInfo = new RGBQUAD *[fileInfoHeader.biHeight];
//
//    for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
//        rgbInfo[i] = new RGBQUAD[fileInfoHeader.biWidth];
//    }
//
//    // определение размера отступа в конце каждой строки
//    int linePadding = ((fileInfoHeader.biWidth * (fileInfoHeader.biBitCount / 8)) % 4) & 3;
//
//    // чтение
//    unsigned int bufer;
//
//    for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
//        for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
//            read(fileStream, bufer, fileInfoHeader.biBitCount / 8);
//
//            rgbInfo[i][j].rgbRed = bitextract(bufer, fileInfoHeader.biRedMask);
//            rgbInfo[i][j].rgbGreen = bitextract(bufer, fileInfoHeader.biGreenMask);
//            rgbInfo[i][j].rgbBlue = bitextract(bufer, fileInfoHeader.biBlueMask);
//            rgbInfo[i][j].rgbReserved = bitextract(bufer, fileInfoHeader.biAlphaMask);
//        }
//        fileStream.seekg(linePadding, std::ios_base::cur);
//    }
//    int w, h, x0, y0;
//    w = fileInfoHeader.biWidth;
//    h = fileInfoHeader.biHeight;
//    y0 = WinHei - h - 60; //400;
//    x0 = WinWid - w;
//    color_t c;
//
//    // вывод
//
//    // ============
//    figure fig;
//    p.x = x0 + cx;
//    p.y = y0 + cy;
//    fig.p.push_back(p);
//    p.x = x0 + cx + w;
//    p.y = y0 + cy + h;
//    fig.p.push_back(p);
//    ++id;
//    fig.id = id;
//    fig.center.x = (X0 + X0 + w) / 2.0 + cx;
//    fig.center.y = (Y0 + Y0 + h) / 2.0 + cy;
//    fig.name = "image";
//    fig.fordel = false;
//    fig.visible = true;
//    fig.color = cAll;
//    fig.thickness = 0;
//
//    fig.extrem = border_polyline(fig.p);
//    fig.start_image = texture.size();
//
//    for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
//        for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
//            c.y = y0 + i + cy;
//            c.x = j + x0 + cx;
//            c.b = rgbInfo[i][j].rgbRed / 256.0;
//            c.g = rgbInfo[i][j].rgbGreen / 256.0;
//            c.r = rgbInfo[i][j].rgbBlue / 256.0;
//            texture.push_back(c);
//
//        }
//    }
//    fig.end_image = texture.size();
//    //Вигадуємо унікальне ім'я файлу
//    std::string name = currentDateToString() + ".bmp";
//    fig.file_image = name;
//    figures.push_back(fig);
//    // =============
//
//    if (fileName != "tmp/" + name) {
//        std::cout << "Переміщаємо файл " << name << ".\n";
//        int n;
//        n = rename(fileName.c_str(), ("tmp/" + name).c_str());
//    }
//
////    n = remove("file.bmp");
//    return 1;
//
//}


GLuint LoadTexture(char *FileName, int &w, int &h) {

    FILE *F;
    /* Открываем файл */
    if ((F = fopen(FileName, "rb")) == NULL)
        return -1;


    /*Перемещаемся в bmp-файле на нужную позицию, и считываем ширину и длинну */
    fseek(F, 18, SEEK_SET);
    fread(&(get_textura.W), 2, 1, F);
    fseek(F, 2, SEEK_CUR);
    fread(&(get_textura.H), 2, 1, F);


    w = get_textura.W;
    h = get_textura.H;
    printf("%d x %d\n", get_textura.W, get_textura.H);

    /* Выделяем память под изображение. Если память не выделилась, закрываем файл и выходим с ошибкой */
    if ((get_textura.Image = (unsigned char *) malloc(sizeof(unsigned char) * 3 * get_textura.W * get_textura.H)) ==
        NULL) {
        fclose(F);
        return -1;
    }
    /* Считываем изображение в память по 3 бита, то бишь RGB для каждого пикселя */
    fseek(F, 30, SEEK_CUR);
    fread(get_textura.Image, 3, get_textura.W * get_textura.H, F);

    //http://www.opengl.org.ru/books/open_gl/chapter5.4.html

    textura_id.push_back(1);

    glGenTextures(1, &textura_id[textura_id.size() - 1]);
    glBindTexture(GL_TEXTURE_2D, textura_id[textura_id.size() - 1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, get_textura.W, get_textura.H, 0, GL_BGR,
                 GL_UNSIGNED_BYTE, get_textura.Image);

    free(get_textura.Image);
    fclose(F);

    return 1;
}


void load_figures() {
    texture.empty();
    std::ifstream file;
    file.open("figs.json");
    if (file) {
        //Читаємо
        int num = 0;
        int count;
        figure fig;
        figures.clear();
        while (file) {
            std::string s;
            std::getline(file, s);
            s = trim(s);
            int n = s.find("\"count\"");
            if (n != -1) {
                s = right_sym(s, ":");
                s = trim(s, ',');
                count = std::stoi(s);
                id = count;
                std::cout << "count=" << count << std::endl;
                continue;
            }
            n = s.find("\"id\"");
            if (n != -1) {
                s = right_sym(s, ":");
                s = trim(s, ',');
                int id = std::stoi(s);
                fig.id = id;
                continue;
            }
            n = s.find("\"fordel\"");
            if (n != -1) {
                s = right_sym(s, ":");
                s = trim(s, ',');
                bool fordel = std::stoi(s);
                fig.fordel = fordel;
                continue;
            }
            n = s.find("\"visible\"");
            if (n != -1) {
                s = right_sym(s, ":");
                s = trim(s, ',');
                bool visible = std::stoi(s);
                fig.visible = visible;
                continue;
            }
            n = s.find("\"name\"");
            if (n != -1) {
                s = right_sym(s, ":");
                s = trim(s, ',');
                std::string name = s;
                s = trim(s, '\"');
                fig.name = s;
                continue;
            }
            n = s.find("\"file_image\"");
            if (n != -1) {
                s = right_sym(s, ":");
                s = trim(s, ',');
                std::string file_image = s;
                s = trim(s, '\"');
                fig.file_image = s;
                //Підвантажуємо фото
                insert_screenshot("tmp/" + fig.file_image);
                continue;
            }
            n = s.find("\"p\"");
            if (n != -1) {
                //Тут обробляєм масив точок
                points ps;
                std::getline(file, s);
                s = trim(s);
                float x, y;
                point p;
                while (s != "],") {
                    std::getline(file, s);
                    s = trim(s);
                    n = s.find("\"x\"");
                    if (n != -1) {
                        s = right_sym(s, ":");
                        s = trim(s, ',');
                        x = std::stof(s);
                        continue;
                    }
                    n = s.find("\"y\"");
                    if (n != -1) {
                        s = right_sym(s, ":");
                        s = trim(s, ',');
                        y = std::stof(s);
                        continue;
                    }
                    n = s.find("}");
                    if (n != -1) {
                        p.x = x;
                        p.y = y;
                        ps.push_back(p);
                        continue;
                    }

                }
                fig.p = ps;
                continue;
            }
            n = s.find("\"center\"");
            if (n != -1) {
                float x, y;
                while ((s != "}") && (s != "},")) {
                    std::getline(file, s);
                    s = trim(s);
                    n = s.find("\"x\"");
                    if (n != -1) {
                        s = right_sym(s, ":");
                        s = trim(s, ',');
                        x = std::stof(s);
                        continue;
                    }
                    n = s.find("\"y\"");
                    if (n != -1) {
                        s = right_sym(s, ":");
                        s = trim(s, ',');
                        y = std::stof(s);
                    }
                }
                fig.center.x = x;
                fig.center.y = y;
            }
            n = s.find("\"thickness\"");
            if (n != -1) {
                s = right_sym(s, ":");
                s = trim(s, ',');
                fig.thickness = std::stoi(s);
                continue;
            }
            n = s.find("\"extrem\"");
            if (n != -1) {
                float xmin, ymin, xmax, ymax;
                while ((s != "}") && (s != "},")) {
                    std::getline(file, s);
                    s = trim(s);
                    n = s.find("\"xmin\"");
                    if (n != -1) {
                        s = right_sym(s, ":");
                        s = trim(s, ',');
                        xmin = std::stof(s);
                        continue;
                    }
                    n = s.find("\"ymin\"");
                    if (n != -1) {
                        s = right_sym(s, ":");
                        s = trim(s, ',');
                        ymin = std::stof(s);
                    }
                    n = s.find("\"xmax\"");
                    if (n != -1) {
                        s = right_sym(s, ":");
                        s = trim(s, ',');
                        xmax = std::stof(s);
                        continue;
                    }
                    n = s.find("\"ymax\"");
                    if (n != -1) {
                        s = right_sym(s, ":");
                        s = trim(s, ',');
                        ymax = std::stof(s);
                    }
                }
                fig.extrem.xmin = xmin;
                fig.extrem.ymin = ymin;
                fig.extrem.xmax = xmax;
                fig.extrem.ymax = ymax;
            }
            n = s.find("\"color\"");
            if (n != -1) {
                float colorR, colorG, colorB, colorA, fonColorR, fonColorG, fonColorB;
                while ((s != "}") && (s != "},")) {
                    std::getline(file, s);
                    s = trim(s);
                    n = s.find("\"colorR\"");
                    if (n != -1) {
                        s = right_sym(s, ":");
                        s = trim(s, ',');
                        colorR = std::stof(s);
                        continue;
                    }
                    n = s.find("\"colorG\"");
                    if (n != -1) {
                        s = right_sym(s, ":");
                        s = trim(s, ',');
                        colorG = std::stof(s);
                    }
                    n = s.find("\"colorB\"");
                    if (n != -1) {
                        s = right_sym(s, ":");
                        s = trim(s, ',');
                        colorB = std::stof(s);
                        continue;
                    }
                    n = s.find("\"colorA\"");
                    if (n != -1) {
                        s = right_sym(s, ":");
                        s = trim(s, ',');
                        colorA = std::stof(s);
                    }
                    n = s.find("\"fonColorR\"");
                    if (n != -1) {
                        s = right_sym(s, ":");
                        s = trim(s, ',');
                        fonColorR = std::stof(s);
                    }
                    n = s.find("\"fonColorG\"");
                    if (n != -1) {
                        s = right_sym(s, ":");
                        s = trim(s, ',');
                        fonColorG = std::stof(s);
                    }
                    n = s.find("\"fonColorB\"");
                    if (n != -1) {
                        s = right_sym(s, ":");
                        s = trim(s, ',');
                        fonColorB = std::stof(s);
                    }
                }
                fig.color.colorR = colorR;
                fig.color.colorG = colorG;
                fig.color.colorB = colorB;
                fig.color.colorA = colorA;
                fig.color.fonColorR = fonColorR;
                fig.color.fonColorG = fonColorG;
                fig.color.fonColorB = fonColorB;
            }
            n = s.find("\"start_image\"");
            if (n != -1) {
                s = right_sym(s, ":");
                s = trim(s, ',');
                fig.start_image = std::stoi(s);
                continue;
            }
            n = s.find("\"end_image\"");
            if (n != -1) {
                s = right_sym(s, ":");
                s = trim(s, ',');
                fig.end_image = std::stoi(s);
                figures.push_back(fig);
            }
        }

    } else {
    }
    file.close();
    Draw();


}

