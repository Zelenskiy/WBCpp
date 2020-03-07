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

float WinWid = 1366.0;
float WinHei = 768.0;
int window;
float cx=0;
float cy=0;
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

AUX_RGBImageRec *image;

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
                line(XX0-cx, YY0-cy, XX-cx, YY-cy, f.thickness, f.color);
            } else {
                if (f.name == "line") {
                    points ps = f.p;
                    float XX0 = ps[0].x;
                    float YY0 = ps[0].y;
                    float XX = ps[1].x;
                    float YY = ps[1].y;
//                    draw_circle_fill(XX0, YY0, f.thickness, f.color.colorR, f.color.colorG, f.color.colorB);
                    draw_line(XX0-cx, YY0-cy, XX-cx, YY-cy, f.thickness*2, f.color);
//                    line(XX0, YY0, XX, YY, f.thickness, f.color);
                }
            }
        }
//    glPopMatrix();


    }
}


void Draw() {

    Render();
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
    init_buttons(cAll);
    glClearColor(cAll.fonColorR, cAll.fonColorG, cAll.fonColorB, cAll.fonColorA);
    glMatrixMode(GL_MATRIX_MODE);
    glLoadIdentity();
    glOrtho(0, WinWid, 0, WinHei-60, -1,1);
}

float m_s(float y){
    return  WinHei - y - 60;
}


void on_mouse_down_up(int button, int state, int ax, int ay) {
    if (m_s(ay)<35) {
        // --- check buttons --------
        int t = check_buttons(ax, m_s(ay));
        if (t != 0) {
            tool = t;
        }
    } else {
        // -------------------------

        if (button == GLUT_LEFT_BUTTON) {
            old_X0 = 0;
            old_Y0 = 0;
            old_X = 0;
            old_Y = 0;
            fig.p.clear();
            if (state == GLUT_DOWN) {
                down = true;
                X0 = ax;
                Y0 = m_s(ay);
            } else {    // GLUT_UP

                down = false;
                switch (tool) {
                    case 3:
                        draw_to_figures(X0, Y0, X, Y, cAll, penWidth);
                        break;
                }
            }
            if (tool > 1)
                Draw();

        } else if (button == 3) { //scrolling
            cy += 1;
        } else if (button == 4) { //scrolling
            cy -= 1;
        }
        glutPostRedisplay();
    }

}


void on_mouse_drag(int ax, int ay) {
    if (down) {//                Draw();
        X = ax;
        Y = m_s(ay);
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
                    if ((abs(X+cx - f.center.x) < erWidth) && (abs(Y +cy- f.center.y) < erWidth)) {
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
            case 20:
                cx += (X0-X)*0.03;
                cy += (Y0-Y)*0.03;
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
        case 115:   //S
            insert_screenshot();
            break;
        case 127:
            figures.clear();
//            std::cout << "size " << figures.size() << std::endl;
            Draw();
            break;
    }
}

void keySpecialUp(int key, int x, int y){
    cout<<"key "<<key<<endl;
    switch (key) {   //UP
        case 101:
            cy += 100;
            break;
        case 103: //LEFT
            cy -= 100;
            break;
        case 100:
            cx -= 100;
            break;
        case 102: //RIGHT
            cx += 100;
            break;
    }
    Draw();
}

void mouseWheel(){

}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WinWid, WinHei-40);
    glutInitWindowPosition(0, 0);
    window = glutCreateWindow("Hello OpenGL");
    glutDisplayFunc(Draw);
    glutKeyboardFunc(on_keypress);
    glutSpecialUpFunc(keySpecialUp);
    glutMouseFunc(on_mouse_down_up);
    glutMotionFunc(on_mouse_drag);

//    glutTimerFunc(50, Timer, 0);
    Initialize();
//    image = auxDIBImageLoad("photo.bmp")
//    glutFullScreen();
    glutMainLoop();
    return 0;
}
