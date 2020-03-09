#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <list>

#include <GL/glut.h>
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
bool isGrid = false;


std::vector<color_t> texture;

int penWidth = 4;
int tool = 1;
float erWidth = 10;


std::list<figure> figures;
point p;
figure fig;

int insert_screenshot();

void close_app();

void draw_image(int start, int end);


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
                draw_image(f.start_image, f.end_image);
            }
        }

    }
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


void Draw() {
    Render();
//    draw_pictures();
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
        glutFullScreen();
        glutReshapeWindow(WinWid, WinHei - 60);
        glutShowWindow();
        insert_screenshot();
//        glutShowWindow();
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

void write_ini(){
    std::ofstream fout("wb.ini");
    fout << "[MAIN]" << std::endl;
    fout << "fonColorR = " << cAll.fonColorR << std::endl;
    fout << "fonColorG = " << cAll.fonColorG << std::endl;
    fout << "fonColorB = " << cAll.fonColorB << std::endl;
    fout << "penColorR = " << cAll.colorR << std::endl;
    fout << "penColorG = " << cAll.colorG << std::endl;
    fout << "penColorB = " << cAll.colorB << std::endl;
    if (isGrid)
        fout << "grid = " << "true" << std::endl;
    else
        fout << "grid = " << "false" << std::endl;
    fout << "penWidth = " << penWidth << std::endl;
    fout << "erWidth = " << erWidth << std::endl;
    fout.close();
}

void read_ini(){
    std::ifstream file_ini;
    file_ini.open("wb.ini");
    if (file_ini) {
        //Читаємо
        while(file_ini) {
            std::string str;
            std::getline(file_ini, str);
            // Обработка строки str
            // Шукаємо перший символ "="
            std::string field = left_sym(str,"=");
            std::string value = right_sym(str,"=");
            if (field == "fonColorR") cFon.colorR = stof(value);
            if (field == "fonColorG") cFon.colorG = stof(value);
            if (field == "fonColorB") cFon.colorB = stof(value);
            if (field == "penColorR") cAll.colorR = stof(value);
            if (field == "penColorG") cAll.colorR = stof(value);
            if (field == "penColorB") cAll.colorR = stof(value);
            if (field == "penWidth") penWidth = stof(value);
            if (field == "erWidth") erWidth = stof(value);
            if (field == "grid")
                if ((value == "true")||(value == "True")||(value == "TRUE"))
                    isGrid = true;
                else
                    isGrid = false;

        }

    } else {
        //Пишемо новий
        write_ini();
    }
    file_ini.close();
    cAll.fonColorR = cFon.colorR;
    cAll.fonColorG = cFon.colorG;
    cAll.fonColorB = cFon.colorB;

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
        int t = check_buttons(ax, m_s(ay));
        if (t > 0) {
            tool = t;
        } else if (t == -1) { //Згортаємо
            glutIconifyWindow();
        } else if (t == -2) { //Згортаємо
            close_app();
        }
    } else {
        // -------------------------

        if (button == GLUT_LEFT_BUTTON) {
            old_X0 = 0;
            old_Y0 = 0;
            old_X = ax;
            old_Y = m_s(ay);
//            fig.p.clear();
            if (state == GLUT_DOWN) {
                down = true;
                X0 = ax;
                Y0 = m_s(ay);
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
        switch (tool) {
            case 1:                             // Ручка
                draw_to_figures(X0, Y0, X, Y, cAll, penWidth);
                line(X0, Y0, X, Y, penWidth, cAll);
                X0 = X;
                Y0 = Y;
                break;
            case 2:                             // Гумка
                draw_circle(X0, Y0, erWidth, 0.8, 1.0, 0.8);
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
//                Draw();
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
    //Зберігаємо фігури до файлу


    //архівуємо файли
    //формуємо унікальне ім'я
    std::string name_zip = "lessons/" + currentDateToString() + ".zip";
    std::string command = "zip -m " + name_zip+ " tmp/*.*";
    system(command.c_str());
    glutDestroyWindow(window);
}

void on_window_status(int status) {
    if (status == GLUT_HIDDEN) {
        std::cout << "Згорнуло" << std::endl;
        std::ofstream fout("flag.txt");
//        fout << "" << std::endl;
        fout.close();
    }

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


    glutTimerFunc(2000, Timer, 0);
    Initialize();

//    glutHideWindow();
//    glutIconifyWindow();
//    glutShowWindow();
    glutMainLoop();
    return 0;
}

int insert_screenshot() {
    char *fileName = "file.bmp";
    // открываем файл
    std::ifstream fileStream(fileName, std::ifstream::binary);
    if (!fileStream) {
        std::cout << "Error opening file '" << fileName << "'." << std::endl;
        return 0;
    }


    // заголовк изображения
    BITMAPFILEHEADER fileHeader;
    read(fileStream, fileHeader.bfType, sizeof(fileHeader.bfType));
    read(fileStream, fileHeader.bfSize, sizeof(fileHeader.bfSize));
    read(fileStream, fileHeader.bfReserved1, sizeof(fileHeader.bfReserved1));
    read(fileStream, fileHeader.bfReserved2, sizeof(fileHeader.bfReserved2));
    read(fileStream, fileHeader.bfOffBits, sizeof(fileHeader.bfOffBits));

    if (fileHeader.bfType != 0x4D42) {
        std::cout << "Error: '" << fileName << "' is not BMP file." << std::endl;
        return 0;
    }

    // информация изображения
    BITMAPINFOHEADER fileInfoHeader;
    read(fileStream, fileInfoHeader.biSize, sizeof(fileInfoHeader.biSize));

    // bmp core
    if (fileInfoHeader.biSize >= 12) {
        read(fileStream, fileInfoHeader.biWidth, sizeof(fileInfoHeader.biWidth));
        read(fileStream, fileInfoHeader.biHeight, sizeof(fileInfoHeader.biHeight));
        read(fileStream, fileInfoHeader.biPlanes, sizeof(fileInfoHeader.biPlanes));
        read(fileStream, fileInfoHeader.biBitCount, sizeof(fileInfoHeader.biBitCount));
    }

    // получаем информацию о битности
    int colorsCount = fileInfoHeader.biBitCount >> 3;
    if (colorsCount < 3) {
        colorsCount = 3;
    }

    int bitsOnColor = fileInfoHeader.biBitCount / colorsCount;
    int maskValue = (1 << bitsOnColor) - 1;

    // bmp v1
    if (fileInfoHeader.biSize >= 40) {
        read(fileStream, fileInfoHeader.biCompression, sizeof(fileInfoHeader.biCompression));
        read(fileStream, fileInfoHeader.biSizeImage, sizeof(fileInfoHeader.biSizeImage));
        read(fileStream, fileInfoHeader.biXPelsPerMeter, sizeof(fileInfoHeader.biXPelsPerMeter));
        read(fileStream, fileInfoHeader.biYPelsPerMeter, sizeof(fileInfoHeader.biYPelsPerMeter));
        read(fileStream, fileInfoHeader.biClrUsed, sizeof(fileInfoHeader.biClrUsed));
        read(fileStream, fileInfoHeader.biClrImportant, sizeof(fileInfoHeader.biClrImportant));
    }

    // bmp v2
    fileInfoHeader.biRedMask = 0;
    fileInfoHeader.biGreenMask = 0;
    fileInfoHeader.biBlueMask = 0;

    if (fileInfoHeader.biSize >= 52) {
        read(fileStream, fileInfoHeader.biRedMask, sizeof(fileInfoHeader.biRedMask));
        read(fileStream, fileInfoHeader.biGreenMask, sizeof(fileInfoHeader.biGreenMask));
        read(fileStream, fileInfoHeader.biBlueMask, sizeof(fileInfoHeader.biBlueMask));
    }

    // если маска не задана, то ставим маску по умолчанию
    if (fileInfoHeader.biRedMask == 0 || fileInfoHeader.biGreenMask == 0 || fileInfoHeader.biBlueMask == 0) {
        fileInfoHeader.biRedMask = maskValue << (bitsOnColor * 2);
        fileInfoHeader.biGreenMask = maskValue << bitsOnColor;
        fileInfoHeader.biBlueMask = maskValue;
    }

    // bmp v3
    if (fileInfoHeader.biSize >= 56) {
        read(fileStream, fileInfoHeader.biAlphaMask, sizeof(fileInfoHeader.biAlphaMask));
    } else {
        fileInfoHeader.biAlphaMask = maskValue << (bitsOnColor * 3);
    }

    // bmp v4
    if (fileInfoHeader.biSize >= 108) {
        read(fileStream, fileInfoHeader.biCSType, sizeof(fileInfoHeader.biCSType));
        read(fileStream, fileInfoHeader.biEndpoints, sizeof(fileInfoHeader.biEndpoints));
        read(fileStream, fileInfoHeader.biGammaRed, sizeof(fileInfoHeader.biGammaRed));
        read(fileStream, fileInfoHeader.biGammaGreen, sizeof(fileInfoHeader.biGammaGreen));
        read(fileStream, fileInfoHeader.biGammaBlue, sizeof(fileInfoHeader.biGammaBlue));
    }

    // bmp v5
    if (fileInfoHeader.biSize >= 124) {
        read(fileStream, fileInfoHeader.biIntent, sizeof(fileInfoHeader.biIntent));
        read(fileStream, fileInfoHeader.biProfileData, sizeof(fileInfoHeader.biProfileData));
        read(fileStream, fileInfoHeader.biProfileSize, sizeof(fileInfoHeader.biProfileSize));
        read(fileStream, fileInfoHeader.biReserved, sizeof(fileInfoHeader.biReserved));
    }

    // проверка на поддерку этой версии формата
    if (fileInfoHeader.biSize != 12 && fileInfoHeader.biSize != 40 && fileInfoHeader.biSize != 52 &&
        fileInfoHeader.biSize != 56 && fileInfoHeader.biSize != 108 && fileInfoHeader.biSize != 124) {
        std::cout << "Error: Unsupported BMP format." << std::endl;
        return 0;
    }

    if (fileInfoHeader.biBitCount != 16 && fileInfoHeader.biBitCount != 24 && fileInfoHeader.biBitCount != 32) {
        std::cout << "Error: Unsupported BMP bit count." << std::endl;
        return 0;
    }

    if (fileInfoHeader.biCompression != 0 && fileInfoHeader.biCompression != 3) {
        std::cout << "Error: Unsupported BMP compression." << std::endl;
        return 0;
    }

    // rgb info
    RGBQUAD **rgbInfo = new RGBQUAD *[fileInfoHeader.biHeight];

    for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
        rgbInfo[i] = new RGBQUAD[fileInfoHeader.biWidth];
    }

    // определение размера отступа в конце каждой строки
    int linePadding = ((fileInfoHeader.biWidth * (fileInfoHeader.biBitCount / 8)) % 4) & 3;

    // чтение
    unsigned int bufer;

    for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
        for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
            read(fileStream, bufer, fileInfoHeader.biBitCount / 8);

            rgbInfo[i][j].rgbRed = bitextract(bufer, fileInfoHeader.biRedMask);
            rgbInfo[i][j].rgbGreen = bitextract(bufer, fileInfoHeader.biGreenMask);
            rgbInfo[i][j].rgbBlue = bitextract(bufer, fileInfoHeader.biBlueMask);
            rgbInfo[i][j].rgbReserved = bitextract(bufer, fileInfoHeader.biAlphaMask);
        }
        fileStream.seekg(linePadding, std::ios_base::cur);
    }
    int w, h, x0, y0;
    w = fileInfoHeader.biWidth;
    h = fileInfoHeader.biHeight;
    y0 = WinHei - h - 60; //400;
    x0 = WinWid - w;
    color_t c;

    // вывод

    // ============
    figure fig;
    p.x = x0 + cx;
    p.y = y0 + cy;
    fig.p.push_back(p);
    p.x = x0 + cx + w;
    p.y = y0 + cy + h;
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
    fig.start_image = texture.size();

    for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
        for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
            c.y = y0 + i + cy;
            c.x = j + x0 + cx;
            c.b = rgbInfo[i][j].rgbRed / 256.0;
            c.g = rgbInfo[i][j].rgbGreen / 256.0;
            c.r = rgbInfo[i][j].rgbBlue / 256.0;
            texture.push_back(c);

        }
    }
    fig.end_image = texture.size();
    //Вигадуємо унікальне ім'я файлу
    std::string name = currentDateToString() + ".bmp";
    fig.file_image = name;
    figures.push_back(fig);
    // =============
    std::cout << "Переміщаємо файл " << name << ".\n";
    int n;
    n = rename("file.bmp", ("tmp/" + name).c_str());
//    n = remove("file.bmp");
    return 1;

}

