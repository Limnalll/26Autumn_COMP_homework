#include <iostream>
#include <fstream>
#include "cli.h"
#include "stimulator.h"
#include "json_io.h"
#include <cstdio>
#include <string>
using namespace std;

//防止死循环
static const int EXEC_LIMIT = 2000;

// 游戏阶段
enum GameSta { STA_START, STA_READY, STA_PLAY, STA_OVER };

//关卡数据路径
string level_file(int id) { return "level/" + to_string(id) + ".json"; }

//获取关卡总数
int max_level() {
    int id = 1;
    while (id < 1000) {
        ifstream f(level_file(id));
        if (!f.is_open()) break;
        id++;
    }
    return id - 1;
}

//判断文件是否存在
bool file_exist(string route){
    ifstream f(route);
    return f.is_open();
}

//加载关卡数据
bool load_level(int id, Stimulation_state& s){
    if(!file_exist(level_file(id))){return 0;}
    try{
        json data=loadJson(level_file(id));
        s.level_m=id;
        s.level_name=data.value("name","未命名关卡");
        s.hint=data.value("hint","没有提示哦");
        s.inbox=data.at("inbox").get<vector<int>>();
        s.inbox_n=s.inbox.size();
        s.target_outbox=data.at("target_outbox").get<vector<int>>();
        s.target_outbox_n=s.target_outbox.size();
        s.ground_n=data["ground_n"].get<int>();
        s.allowed=data["instructions"].get<vector<string>>();    
        return 1;
    }catch(const exception& e){
        printf("关卡文件损坏：%s\n",e.what());
        return 0;
    }
}

//获取关卡完成进度
int process_value(){
    if(!file_exist("level/process.json")){return 0;}
    json data = loadJson("level/process.json");
    return data.at("passed").get<int>();
}

//保存关卡完成进度
void save_process(int id){
    json data;
    data["passed"]=id;
    saveJson("level/process.json",data);
}

int main(){
    GameSta game_sta = STA_START;
    init_console();
    start_page(max_level(),process_value());
    return 0;
}
