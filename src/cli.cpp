#include "cli.h"
#define ll long long

#include <iostream>
#include <windows.h>
#include <string>
#include <conio.h>

using namespace std;

const int MAX_PROG = 10000;   // 机器人程序的指令条数上限

void init_console() {
    HWND hwnd = GetConsoleWindow();
    if (hwnd != NULL) {
        ShowWindow(hwnd, SW_MAXIMIZE);
    }
}

void clear_screen(){system("cls");}

void print_at(int x, int y, const string& text) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD pos;
    pos.X = x;
    pos.Y = y;

    SetConsoleCursorPosition(h, pos);
    cout << text;
}

string center_num(int x){
    if(x<10){return " "+to_string(x)+" ";}
    else if(x<100){return to_string(x/10)+" "+to_string(x%10);}
    else{return to_string(x);}
}

void print_robot_body(int x,int y,int block=0){
    print_at(x,y+0,"  .~*~.  ");
    print_at(x,y+1,"  [0_0]  ");
    print_at(x,y+2," +-----+ ");
    if(block){
  //print_at(x,y+3," |  1  |  ")
        print_at(x,y+3," | "+center_num(block)+" | ");}
    else{
    print_at(x,y+3," |  X  | ");
    }
    print_at(x,y+4,"|+-----+|");
}
void print_robot_foot_stand(int x,int y){
    print_at(x,y+0,"\\  | |  /");
    print_at(x,y+1,"  // \\\\  ");
    print_at(x,y+2," /     \\ ");
    print_at(x,y+3,"_|     |_");
}
void print_robot_foot_kneel(int x,int y){
    print_at(x,y+0,"\\  / \\  /");
    print_at(x,y+1," //   \\\\ ");
    print_at(x,y+2,"_|     |_");
}
void print_robot(int x,int y,int block,int sta){
    // sta=0:站立；sta=1:下蹲
    if(sta){
        print_at(x,y+0,"           ");
        print_robot_body(x,y+1,block);
        print_robot_foot_kneel(x,y+6);
    }else{
        print_robot_body(x,y,block);
        print_robot_foot_stand(x,y+5);
    }
}

int getch_native() {return _getch();}

void wait_key(){
    while (true) {
        int c = getch_native();
        if (c == 0 || c == 224) { getch_native(); continue; }  // 方向键等功能键的前缀
        if (c == ' ' || c == '\r' || c == '\n') return;
    }
}

void trim(string& s) {
    size_t a = 0, b = s.size();
    while (a < b && (s[a] == ' ' || s[a] == '\t' || s[a] == '\r' || s[a] == '\n')) a++;
    while (b > a && (s[b - 1] == ' ' || s[b - 1] == '\t' || s[b - 1] == '\r' || s[b - 1] == '\n')) b--;
    s = s.substr(a, b - a);
}

void read_line(string& s){
    getline(cin,s);
    trim(s);
}

bool ask_int(int& v) {
    string l;
    read_line(l);
    size_t i = 0;
    bool neg = 0;
    if (i < l.size() && (l[i] == '+' || l[i] == '-')) { neg = (l[i] == '-'); i++; }
    if (i >= l.size()) return 0;
    long long x = 0;
    for (; i < l.size(); i++) {
        if (!isdigit((unsigned char)l[i])) return 0;
        x = x * 10 + (l[i] - '0');
        if (x > (ll)2e9) return 0;   //防溢出
    }
    v = (int)(neg ? -x : x);
    return 1;
}

void alert_wait(string info){
    cout << info + "(按回车或空格以继续)" << endl;
    wait_key();
}

int start_page(int max_level,int passed_level){
    while(true){
        clear_screen();
        cout <<
            "==========================================================================\n"
            "                积 木 机 器 人  ·  小 明 的 未 来 房 间                    \n"
            "==========================================================================\n";
        print_robot(0,4,0,0);
        cout <<
            "\n\n[玩法简介]\n"
            "  输入传送带(INBOX)送来一串积木，每块积木上有一个数字；\n"
            "  房间里有若干块带编号的空地(从 0 开始)，每块空地能存放一个积木；\n"
            "  机器人按你编写的程序工作：inbox 从输入带取块 / outbox 放块到输出带 /\n"
            "  copyto、copyfrom 在空地上存取积木 / add、sub 做加减法 / jump、jumpifzero 跳转。\n"
            "  目标：让输出传送带(OUTBOX)上的积木序列与目标序列完全一致！\n";
        cout << "\n---------------------------- 关卡列表 ----------------------------\n";
        for (int i = 1; i <= max_level; i++) {
            cout << "  第 " << i << " 关  ";
            if(i<=passed_level){cout << "[已通过]";}
            else{cout << "[未通过]";}
            cout << "\n";
        }
        if (max_level <= 0){alert_wait("在level文件夹中未找到任何关卡文件!");return 0;}
        cout << "------------------------------------------------------------------\n";
        cout << "请输入想要开始的关卡编号（输入 0 退出游戏）：";
        int cur_level;
        if(!ask_int(cur_level)){alert_wait("只能输出整数");continue;}
        if(cur_level==0){return 0;}
        if(cur_level<1 || cur_level>max_level){alert_wait("关卡不存在!");}
        else if(cur_level>passed_level+1){alert_wait("请先通过第"+to_string(passed_level+1)+"关!");}
        else{return cur_level;}
    }
}