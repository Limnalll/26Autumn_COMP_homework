#include "cli.h"
#define ll long long

#include <iostream>
#include <windows.h>
#include <string>
#include <conio.h>
#include <fstream>

using namespace std;

const int MAX_PROG = 10000;   // 机器人程序的指令条数上限

void init_console() {
    HWND hwnd = GetConsoleWindow();
    if (hwnd != NULL) {
        ShowWindow(hwnd, SW_MAXIMIZE);
    }
}

//判断文件是否存在
bool file_exist(string route){
    ifstream f(route);
    return f.is_open();
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

bool read_line(string& s,istream& source=cin){
    if(!getline(source,s)){return 0;}
    trim(s);
    return (!s.empty());
}

bool ask_int(int& v,istream& source=cin) {
    string l;
    if(!read_line(l,source)){return 0;}
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
    while(1){
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

void print_info(const Stimulation_state S) {
    cout << "========================== 关卡 " << S.level_m<< " · " << S.level_name<< " ==========================\n";
    cout << "任务描述：" << S.hint << "\n";
    cout << "输入序列（共 " << S.inbox_n << " 块积木）：";
    for (int i = 0; i < S.inbox_n; i++) cout << S.val[i] << " ";
    cout << "\n";
    cout << "空地数(编号从0开始)：" << S.ground_n << endl;
    cout << "可用指令：";
    for (size_t i = 0; i < S.allowed.size(); i++) cout << S.allowed[i] << " ";
    cout << "\n";
    cout << "目标输出序列（共 " << S.target_outbox_n << " 块积木）：";
    for (int i = 0; i < S.target_outbox_n; i++) cout << S.target_outbox[i] << " ";
    cout << "\n";
    cout << "（关卡结束时，输出序列与上面目标完全一致即通关）\n";
}

void print_instruction_sheet(const Stimulation_state& S) {
    const char* names[8] = {"inbox", "outbox", "copyto", "copyfrom", "add", "sub", "jump", "jumpifzero"};
    const char* descs[8] = {
        "从输入传送带拿起第一块积木（原有手中积木被舍弃）",
        "把手中积木放上输出传送带",
        "复制手中积木到 X 号空地（若原有积木则被舍弃）",
        "把 X 号空地积木的数字复制到手中（原有手中积木被舍弃）",
        "手中积木数字加上 X 号空地积木数字（空地不变）",
        "手中积木数字减去 X 号空地积木数字（空地不变）",
        "跳转到第 X 条指令继续执行",
        "若手中积木数字为 0，跳转到第 X 条指令"
    };
    cout << "\n[指令说明]\n";
    for (int i = 0; i < 8; i++) {
        bool ok = 0;
        for (size_t k = 0; k < S.allowed.size(); k++)
            if (S.allowed[k] == names[i]) { ok = 1; break; }
        cout << names[i];
        if (i >= 2 && i <= 7) cout << " X";
        cout << (ok ? " [可用] " : " [禁用] ");
        cout << " : " << descs[i] << "\n";
    }
}

bool prob_num(int &m,istream& source=cin){
    if(!ask_int(m,source)){alert_wait("指令数量只能为整数");return 0;}
    if(m<=0 || m >MAX_PROG){alert_wait("指令数量 M 需在 1~"+to_string(MAX_PROG)+"之间");return 0;}
    return 1;
}

bool keyboard_input(Stimulation_state &S){
    int m;
    cout << "\n请输入指令数量：";
    if(!prob_num(m)){return 0;}
    string s;
    for(int i=1;i<=m;i++){
        if(!read_line(s)){
            cout << "指令不能为空" << endl;
            i--;
            continue;
        }
        S.prog.push_back(s);
    }
    alert_wait("成功读取"+to_string(m)+"条指令，准备开始游戏");
    return 1;
}

//除去路径的引号
void path_preproduce(string &path){
    if (path.size() >= 2 &&
    ((path[0] == '"' && path[path.size() - 1] == '"') ||
        (path[0] == '\'' && path[path.size() - 1] == '\'')))
    path = path.substr(1, path.size() - 2);
}

bool file_input(Stimulation_state &S){
    cout << "\n请输入机器人程序文件的路径(请注意，文件中不能有空行)：";
    string path;
    read_line(path);
    path_preproduce(path);
    if(!file_exist(path)){alert_wait("文件不存在！");return 0;}
    //cout << "成功打开文件" << path << endl;
    ifstream f(path);
    int m;
    if(!prob_num(m,f)){return 0;}
    //cout << "准备开始读入，指令数M：" << m << endl;
    string s;
    while (read_line(s,f)) {S.prog.push_back(s);}
    if(S.prog.size()!=m){alert_wait("文件格式错误：读取到"+to_string(S.prog.size())+"条指令，与M不符");S.prog.clear();return 0;}
    alert_wait("成功读取"+to_string(m)+"条指令，准备开始游戏");
    return 1;
}

void ready_page(Stimulation_state &S){
    while(1){
        clear_screen();
        print_info(S);
        print_instruction_sheet(S);
        cout << "\n-------------------- 准备运行机器人程序 --------------------\n";
        cout << "请选择机器人程序的输入方式：\n";
        cout << "  1. 键盘输入（先输入指令数量 M，再逐条输入 M 条指令）\n";
        cout << "  2. 从文件读取（文件第一行为 M，之后每行一条指令）\n";
        cout << "请输入 1 或 2：";
        string choice;
        read_line(choice);
        if (choice == "1") {
            if (keyboard_input(S)) return;
        } else if (choice == "2") {
            if (file_input(S)) return;
        }
    }
}