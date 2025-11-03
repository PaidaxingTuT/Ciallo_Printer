#include <iostream>
#include <vector>
#include <stack>
#include <graphics.h>
#include <conio.h>
#include <Windows.h>
#include <fstream>
using namespace std;

const int PIXEL_SIZE = 20;
const string filename = "canvas.cav";
const string animeName = "anime";
int animeNum = 1;

/**
 * @struct 画布
 */
struct Canvas {
    int width;
    int height;
    vector<vector<COLORREF>> pixel_color;
};

/**
 * @函数 绘制画布
 */
void drawCanvas(const Canvas &canvas) {
    int offsetX = 400; // UI区宽度
    for (int zong = 0; zong < canvas.height; zong++) {
        for (int heng = 0; heng < canvas.width; heng++) {
            int x1 = heng * PIXEL_SIZE + offsetX;
            int y1 = zong * PIXEL_SIZE;
            int x2 = (heng + 1) * PIXEL_SIZE + offsetX;
            int y2 = (zong + 1) * PIXEL_SIZE;
            setfillcolor(canvas.pixel_color[zong][heng]);
            solidrectangle(x1, y1, x2, y2);
        }
    }
}

/**
 * @函数 保存画布
 */
void saveCanvas(const Canvas &canvas, const string &filename) {
    ofstream fout(filename);
    if (!fout) {
        cout << "无法写入" << endl;
        return;
    } else {
        fout << canvas.width << " " << canvas.height << endl;
        for (int zong = 0; zong < canvas.height; zong++) {
            for (int heng = 0; heng < canvas.width; heng++) {
                fout << canvas.pixel_color[zong][heng] << " ";
            }
        }
        fout.close();
        cout << "画布已保存到文件: " << filename << endl;
    }
}

/**
 * @函数 读取画布
 */
void loadCanvas(Canvas &canvas, const string &filename) {
    ifstream fin(filename);
    if (!fin) {
        cout << "无法读取" << endl;
        return;
    } else {
        fin >> canvas.width >> canvas.height;
        canvas.pixel_color.resize(canvas.height, vector<COLORREF>(canvas.width, WHITE));
        for (int zong = 0; zong < canvas.height; zong++) {
            for (int heng = 0; heng < canvas.width; heng++) {
                fin >> canvas.pixel_color[zong][heng];
            }
        }
        fin.close();
        cout << "画布已从文件加载: " << filename << endl;
    }
}

/**
 * @函数 清空画布
 */
void clearCanvas(Canvas &canvas) {
    for (int zong = 0; zong < canvas.height; zong++) {
        for (int heng = 0; heng < canvas.width; heng++) {
            canvas.pixel_color[zong][heng] = WHITE;
        }
    }
    drawCanvas(canvas);
}

/**
 * @栈 撤回与重做
 */
stack<vector<vector<COLORREF>>> undoStack;
stack<vector<vector<COLORREF>>> redoStack;

/**
 * 左按钮
 */
void showLeftButton(const CHAR *const text , COLORREF color){
    setlinestyle(PS_SOLID, 3);
    setlinecolor(BLACK);
    rectangle(135, 345, 305, 395);
    setfillcolor(RGB(255,102,102));
    solidrectangle(137, 347, 303, 393);

    setbkmode(TRANSPARENT);
    settextcolor(color);
    settextstyle(60, 0, "汉仪像素 11px U");
    outtextxy( 220 - textwidth(text)/2, 374 - textheight(text)/2 - 7, text);
}

/**
 * 右按钮
 */
void showRightButton(const CHAR *const text , COLORREF color){
    setlinestyle(PS_SOLID, 3);
    setlinecolor(BLACK);
    rectangle(495, 345, 665, 395);
    setfillcolor(RGB(102,178,255));
    solidrectangle(497, 347, 663, 393);

    setbkmode(TRANSPARENT);
    settextcolor(color);
    settextstyle(60, 0, "汉仪像素 11px U");
    outtextxy(580 - textwidth(text)/2, 374 - textheight(text)/2 - 7, text);
}

/**
 * @函数 油漆桶
 */
void dfsFill(Canvas &canvas, int y, int x, COLORREF oldColor, COLORREF newColor) {
    if (x < 0 || y < 0 || x >= canvas.width || y >= canvas.height) return;
    if (canvas.pixel_color[y][x] != oldColor || canvas.pixel_color[y][x] == newColor) return;

    canvas.pixel_color[y][x] = newColor;

    dfsFill(canvas, y + 1, x, oldColor, newColor);
    dfsFill(canvas, y - 1, x, oldColor, newColor);
    dfsFill(canvas, y, x + 1, oldColor, newColor);
    dfsFill(canvas, y, x - 1, oldColor, newColor);
}

/**
 * @函数 颜色吸管
 */
void colorStrawber(Canvas &canvas, int x, int y, COLORREF &Color){
    COLORREF pickedColor = canvas.pixel_color[y][x];
    Color = pickedColor;
}

/**
 * @函数 保存动画
 */
void saveAnime(const Canvas &canvas, const string &animeName) {
    ofstream fout(animeName);
    if (!fout) {
        cout << "无法写入" << endl;
        return;
    } else {
        fout << canvas.width << " " << canvas.height << endl;
        for (int zong = 0; zong < canvas.height; zong++) {
            for (int heng = 0; heng < canvas.width; heng++) {
                fout << canvas.pixel_color[zong][heng] << " ";
            }
        }
        fout.close();
        cout << "已保存第 " << animeNum << " 帧" << endl;
    }
}

/**
 * @函数 加载动画
 */
void loadAnime(Canvas &canvas, const string &animeName) {
    ifstream fin(animeName);
    if (!fin) {
        cout << "无法读取" << endl;
        return;
    } else {
        fin >> canvas.width >> canvas.height;
        canvas.pixel_color.resize(canvas.height, vector<COLORREF>(canvas.width, WHITE));
        for (int zong = 0; zong < canvas.height; zong++) {
            for (int heng = 0; heng < canvas.width; heng++) {
                fin >> canvas.pixel_color[zong][heng];
            }
        }
        fin.close();
        cout << "文件 " << animeName << " 已加载" << endl;
    }
}

/**
 * @函数 删除动画
 */
void deleteAnime(const string &animeName) {
    remove(animeName.c_str());
    cout << "文件 " << animeName << " 已删除"<< endl;
}


/**
 * @主程序
 */
int main() {
    int width = 60;
    int height = 45;
    string saveName, loadName, deleteName;

    Canvas canvas = {
            width,
            height,
            vector<vector<COLORREF>>(height, vector<COLORREF>(width, WHITE))
    };

    // 启动器部分完全保留
    initgraph(800, 492);
    Sleep(100);
    IMAGE bg;
    loadimage(&bg, _T("resources/background.bmp"));
    putimage(0, 0, &bg);
    mciSendString(R"(open resources/bgm.mp3 alias bgm)", NULL, 0, NULL);
    mciSendString(R"(play bgm repeat)", NULL, 0, NULL);
    Sleep(500);
    settextcolor(RGB(192, 192, 192));
    setbkmode(TRANSPARENT);
    settextstyle(172, 0, "汉仪像素 11px U");
    int text_width = textwidth("Ciallo画板");
    int text_height = textheight("Ciallo画板");
    int x6 = 400 - text_width / 2;
    int y6 = 492 / 4 - text_height / 2;
    outtextxy(x6 + 3, y6 + 3, "Ciallo画板");
    settextcolor(BLACK);
    outtextxy(x6, y6, "Ciallo画板");
    settextstyle(64, 0, "汉仪像素 11px U");
    int text_width1 = textwidth("Ciallo～(∠· ω < )⌒★");
    int text_height1 = textheight("Ciallo～(∠· ω < )⌒★");
    int x7 = 400 - text_width1 / 2;
    int y7 = 492 / 2 - text_height1 / 2 + 10;
    settextcolor(RGB(255, 153, 153));
    outtextxy(x7 + 2, y7 + 2, "Ciallo～(∠· ω < )⌒★");
    settextcolor(RGB(204, 0, 0));
    outtextxy(x7, y7, "Ciallo～(∠· ω < )⌒★");
    settextstyle(32, 0, "汉仪像素 11px U");
    const CHAR *const author = "BY 派大星";
    int text_width2 = textwidth(author);
    int text_height2 = textheight(author);
    int x1 = 800 - text_width2 - 10;
    int y1 = 492 - text_height2 - 10;
    settextcolor(BLUE + GREEN);
    outtextxy(x1, y1, author);

    showLeftButton("开始", WHITE);
    showRightButton("退出", WHITE);

    while (true) {
        MOUSEMSG msg = GetMouseMsg();
        if (msg.x >= 165 && msg.x <= 275 && msg.y >= 354 && msg.y <= 394) {
            BeginBatchDraw();
            mciSendString(R"(play resources/click.mp3 from 0)", NULL, 0, NULL);
            showLeftButton("开始", YELLOW);
            EndBatchDraw();
        } else if (msg.x >= 525 && msg.x <= 635 && msg.y >= 354 && msg.y <= 394) {
            BeginBatchDraw();
            mciSendString(R"(play resources/click.mp3 from 0)", NULL, 0, NULL);
            showRightButton("退出", YELLOW);
            EndBatchDraw();
        } else {
            BeginBatchDraw();
            showLeftButton("开始", WHITE);
            showRightButton("退出", WHITE);
            EndBatchDraw();
        }

        if (msg.uMsg == WM_LBUTTONDOWN) {
            if (msg.x >= 165 && msg.x <= 275 && msg.y >= 354 && msg.y <= 394) {
                mciSendString(R"(play resources/logo.mp3 from 0)", NULL, 0, NULL);
                settextcolor(BLACK);
                setbkmode(TRANSPARENT);
                settextstyle(48, 0, "汉仪像素 11px U");
                int t1 = textwidth("加载中");
                int t2 = textheight("加载中");
                int x12 = 400 - t1 / 2;
                int y12 = 246 - t2 / 2 + 230;
                outtextxy(x12, y12-45, "加载中");

                setlinestyle(PS_SOLID, 3);
                setlinecolor(BLACK);
                rectangle(148,460,652,484);

                for (int i = 150; i <= 650; ++i) {
                    Sleep(10);
                    for(int j = 462;j <= 482;j++){
                        int r = 255 - (i - 150) * 120 / 500;
                        int g = 182 + (i - 150) * 24 / 500;
                        int b = 193 + (i - 150) * 42 / 500;
                        putpixel(i, j, RGB(r, g, b));
                    }
                }
                mciSendString(R"(play resources/ciallo.mp3 from 0)", NULL, 0, NULL);
                cleardevice();
                goto start;
            }
            if (msg.x >= 525 && msg.x <= 635 && msg.y >= 354 && msg.y <= 394) {
                mciSendString("stop bgm", NULL, 0, NULL);
                closegraph();
                cout << "欢迎再次使用！" << endl;
                return 0;
            }
        }
    }

    start:
    cout << "欢迎使用Ciallo画板!" << endl;
    cout << "鼠标左键涂色，右键清除" << endl;

    initgraph(width * PIXEL_SIZE + 400, height * PIXEL_SIZE);
    IMAGE img;
    loadimage(&img, _T("resources/ui.bmp"), 400, 903); // 左边宽 400
    putimage(0, 0, &img);

    drawCanvas(canvas);
    COLORREF Color = BLACK;
    bool fillMode = false;
    bool strawberMode = false;

    while (true) {
        MOUSEMSG msg = GetMouseMsg();
        int zong = msg.y / PIXEL_SIZE;
        int heng = (msg.x - 400) / PIXEL_SIZE; // X偏移400

        if (msg.uMsg == WM_LBUTTONDOWN) {
            //颜色选择区域
            //第一行
            if(msg.x >=32 and msg.x <= 131 and msg.y >= 28 and msg.y <= 91){

                Color = RGB(255, 50, 50);
                mciSendString(R"(play resources/red.mp3 from 0)", NULL, 0, NULL);
            }
            if(msg.x >=139 and msg.x <= 262 and msg.y >= 28 and msg.y <= 91){

                Color = RGB(255, 140, 50);
                mciSendString(R"(play resources/orange.mp3 from 0)", NULL, 0, NULL);
            }
            if(msg.x >=270 and msg.x <= 365 and msg.y >= 28 and msg.y <= 91){

                Color = RGB(255, 230, 50);
                mciSendString(R"(play resources/yellow.mp3 from 0)", NULL, 0, NULL);
            }

            //第二行
            if(msg.x >=32 and msg.x <= 131 and msg.y >= 100 and msg.y <= 184){

                Color = RGB(50, 220, 50);
                mciSendString(R"(play resources/green.mp3 from 0)", NULL, 0, NULL);
            }

            if(msg.x >=139 and msg.x <= 262 and msg.y >= 100 and msg.y <= 184){

                Color = RGB(50, 200, 255);
                mciSendString(R"(play resources/cyan.mp3 from 0)", NULL, 0, NULL);
            }

            if(msg.x >=270 and msg.x <= 365 and msg.y >= 100 and msg.y <= 184){

                Color = RGB(50, 100, 255);
                mciSendString(R"(play resources/blue.mp3 from 0)", NULL, 0, NULL);
            }

            //第三行
            if(msg.x >=32 and msg.x <= 131 and msg.y >= 191 and msg.y <= 248){

                Color = RGB(50, 50, 255);
                mciSendString(R"(play resources/purple.mp3 from 0)", NULL, 0, NULL);
            }

            if(msg.x >=139 and msg.x <= 262 and msg.y >= 191 and msg.y <= 248){

                Color = RGB(0, 0, 0);
                mciSendString(R"(play resources/black.mp3 from 0)", NULL, 0, NULL);
            }

            if(msg.x >=270 and msg.x <= 365 and msg.y >= 191 and msg.y <= 248){

                Color = RGB(255, 255, 255);
                mciSendString(R"(play resources/white.mp3 from 0)", NULL, 0, NULL);
            }

            //菜单区域
            if(msg.x >=35 and msg.x <= 194 and msg.y >= 279 and msg.y <= 368){
                mciSendString(R"(play resources/ding.mp3 from 0)", NULL, 0, NULL);
                saveCanvas(canvas, filename);
            }

            if(msg.x >=202 and msg.x <= 367 and msg.y >= 279 and msg.y <= 368){
                mciSendString(R"(play resources/ding.mp3 from 0)", NULL, 0, NULL);
                loadCanvas(canvas, filename);
                drawCanvas(canvas);
            }

            if(msg.x >=35 and msg.x <= 194 and msg.y >= 377 and msg.y <= 497){
                if(!undoStack.empty()) {
                    mciSendString(R"(play resources/ding.mp3 from 0)", NULL, 0, NULL);
                    redoStack.push(canvas.pixel_color);
                    canvas.pixel_color = undoStack.top();
                    undoStack.pop();
                    drawCanvas(canvas);
                }
            }

            if(msg.x >=202 and msg.x <= 367 and msg.y >= 377 and msg.y <= 497) {
                if (!redoStack.empty()) {
                    mciSendString(R"(play resources/ding.mp3 from 0)", NULL, 0, NULL);
                    undoStack.push(canvas.pixel_color);
                    canvas.pixel_color = redoStack.top();
                    redoStack.pop();
                    drawCanvas(canvas);
                }
            }

            if(msg.x >=35 and msg.x <= 194 and msg.y >= 507 and msg.y <= 628){
                fillMode = true;
                mciSendString(R"(play resources/senpai.mp3 from 0)", NULL, 0, NULL);
            }

            if(msg.x >=202 and msg.x <= 367 and msg.y >= 507 and msg.y <= 628){
                strawberMode = true;
                mciSendString(R"(play resources/senpai.mp3 from 0)", NULL, 0, NULL);
            }

            if(msg.x >=35 and msg.x <= 142 and msg.y >= 636 and msg.y <= 761){
                mciSendStringA(R"(play resources/ciallo.mp3 from 0)", NULL, 0, NULL);
                saveName = animeName + to_string((animeNum)) + ".cav";
                saveAnime(canvas, saveName);
                animeNum++;
            }

            if(msg.x >=149 and msg.x <= 257 and msg.y >= 636 and msg.y <= 761){
                if(animeNum == 1){
                    cout << "无动画被加载" << endl;
                    mciSendString(R"(play resources/zako.mp3 from 0)", NULL, 0, NULL);
                } else {
                    mciSendString(R"(play resources/ciallo.mp3 from 0)", NULL, 0, NULL);
                    for (int i = 1; i < animeNum; ++i) {
                        loadName = animeName + to_string(i) + ".cav";
                        loadAnime(canvas, loadName);
                        drawCanvas(canvas);
                        Sleep(100);
                    }
                }
            }

            if(msg.x >=264 and msg.x <= 367 and msg.y >= 636 and msg.y <= 761){
                if(animeNum == 1){
                    cout << "无动画被删除" << endl;
                    mciSendString(R"(play resources/zako.mp3 from 0)", NULL, 0, NULL);
                } else {
                    mciSendString(R"(play resources/clear.mp3 from 0)", NULL, 0, NULL);
                    for (int i = 1; i < animeNum; ++i) {
                        deleteName = animeName + to_string(i) + ".cav";
                        deleteAnime(deleteName);
                    }
                    animeNum = 1;
                }
            }

            if(msg.x >=35 and msg.x <= 194 and msg.y >= 768 and msg.y <= 866){
                clearCanvas(canvas);
                drawCanvas(canvas);
                mciSendString(R"(play resources/clear.mp3 from 0)", NULL, 0, NULL);
            }

            if(msg.x >=202 and msg.x <= 367 and msg.y >= 768 and msg.y <= 866){
                goto stop;
            }

            if (heng >= 0 && heng < canvas.width && zong >= 0 && zong < canvas.height) {
                mciSendString(R"(play resources/ciallo.mp3 from 0)", NULL, 0, NULL);
                undoStack.push(canvas.pixel_color);
                if (strawberMode) {
                    COLORREF pickedColor = canvas.pixel_color[zong][heng];
                    Color = pickedColor;
                    colorStrawber(canvas, heng, zong, Color);
                    strawberMode = false;
                } else if (fillMode) {
                    COLORREF oldColor = canvas.pixel_color[zong][heng];
                    if (oldColor != Color) {
                        dfsFill(canvas, zong, heng, oldColor, Color);
                        drawCanvas(canvas);
                    }
                    fillMode = false;
                } else {
                    canvas.pixel_color[zong][heng] = Color;
                    drawCanvas(canvas);
                }
            }
        }

        if (msg.uMsg == WM_RBUTTONDOWN) {
            if (heng >= 0 && heng < canvas.width && zong >= 0 && zong < canvas.height) {
                mciSendString(R"(play resources/Right.mp3 from 0)", NULL, 0, NULL);
                undoStack.push(canvas.pixel_color);
                canvas.pixel_color[zong][heng] = WHITE;
                drawCanvas(canvas);
            }
        }
    }

    stop:
    closegraph();
    cout << "欢迎再次使用！" << endl;
    return 0;
}
