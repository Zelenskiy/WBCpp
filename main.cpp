#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <list>

#include <GL/glut.h>
#include <math.h>
#include "funcs.h"
//#include <GL/glaux.h>
#include  <math.h>








//glEnable(GL_TEXTURE_2D);



float WinWid = 1366.0;
float WinHei = 768.0;
int window;
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
float old_X0; float old_Y0; float old_X; float old_Y;


int penWidth = 4;
int tool = 1;
float erWidth = 10;


list<figure> figures;
point p;
figure fig;

void Render() {
    glClearColor(0.6, 0.8, 0.4, 1.0);
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
                line(XX0, YY0, XX, YY, f.thickness, f.color);
            } else {
                if (f.name == "line") {
                    points ps = f.p;
                    float XX0 = ps[0].x;
                    float YY0 = ps[0].y;
                    float XX = ps[1].x;
                    float YY = ps[1].y;
//                    draw_circle_fill(XX0, YY0, f.thickness, f.color.colorR, f.color.colorG, f.color.colorB);
                    draw_line(XX0, YY0, XX, YY, f.thickness*2, f.color);
//                    line(XX0, YY0, XX, YY, f.thickness, f.color);
                }
            }
        }
//    glPopMatrix();


    }
}


void Draw() {

    Render();
    test_draw(cAll);
    glFlush();
}


void draw_to_figures(int XX0, int YY0, int XX, int YY, colorAll cAll, float thin) {
    point p;
    figure fig;
    p.x = XX0;
    p.y = YY0;
    fig.p.push_back(p);
    p.x = XX;
    p.y = YY;
    fig.p.push_back(p);
    ++id;
    fig.id = id;
    fig.center.x = (X0 + X) / 2.0;
    fig.center.y = (Y0 + Y) / 2.0;
    if (tool==1)
        fig.name = "poly";
    else if (tool==3)
            fig.name = "line";
    fig.fordel = false;
    fig.visible = true;
    fig.color = cAll;
    fig.thickness = thin;
    fig.extrem = border_polyline(fig.p);
    figures.push_back(fig);
}

void figures_update() {
    list<figure> tmpList;
    figures.remove_if([](figure n) { return n.fordel == true; });

}

void Timer(int) {
//    ++Angle;
//    glutPostRedisplay();
    Draw();
    glutTimerFunc(50, Timer, 0);
}
void init_colors(){
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
void Initialize() {
    init_colors();
    glClearColor(cAll.fonColorR, cAll.fonColorG, cAll.fonColorB, cAll.fonColorA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WinWid, 0, WinHei, -5.0, 5.0);
}

void on_mouse_down_up(int button, int state, int ax, int ay) {
    if (button == GLUT_LEFT_BUTTON) {
        old_X0 = 0; old_Y0 = 0; old_X = 0; old_Y = 0;

        fig.p.clear();
        if (state == GLUT_DOWN) {
            down = true;
            X0 = ax;
            Y0 = WinHei - ay-40;
        } else {
            down = false;
            switch (tool) {
                case 3:
                    draw_to_figures(X0, Y0, X, Y, cAll, penWidth);
                    break;
            }

        }
        if (tool>1)
            Draw();
    }
}


void on_mouse_drag(int ax, int ay) {
    if (down) {//                Draw();
        X = ax;
        Y = WinHei - ay-40;
        switch (tool) {
            case 1:                             // Ручка
                draw_to_figures(X0, Y0, X, Y, cAll, penWidth);
                line(X0, Y0, X, Y, penWidth, cAll);
                X0 = X;
                Y0 = Y;
                break;
            case 2:                             // Гумка
                draw_circle(X0 , Y0 , erWidth, 0.8, 1.0, 0.8);
                for (figure &f:figures) {
                    border c = f.extrem;
                    if ((abs(X - f.center.x) < erWidth) && (abs(Y - f.center.y) < erWidth)) {
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
//                Draw();

                draw_line(old_X0, old_Y0, old_X, old_Y, penWidth*2, cFon);
                draw_line(X0, Y0, X, Y, penWidth*2, cAll);
                old_X0 = X0; old_Y0 = Y0; old_X = X; old_Y = Y;

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
            glutDestroyWindow(window);
            break;
        case 112:
            tool = 1;
            break;
        case 101:
            tool = 2;
            break;
        case 108:
            tool = 3;
            break;
        case 97:
            test_draw(cAll);
            break;
        case 127:
            figures.clear();
//            std::cout << "size " << figures.size() << std::endl;
            Draw();
            break;
    }
}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WinWid, WinHei);
    glutInitWindowPosition(0, 0);
    window = glutCreateWindow("Hello OpenGL");
    glutDisplayFunc(Draw);
    glutKeyboardFunc(on_keypress);
    glutMouseFunc(on_mouse_down_up);
    glutMotionFunc(on_mouse_drag);
//    glutTimerFunc(50, Timer, 0);
    Initialize();
//    glutFullScreen();
    glutMainLoop();
    return 0;
}
