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

//加载关卡数据
bool load_level(int id, Stimulation_state& S){
    if(!file_exist(level_file(id))){return 0;}
    try{
        json data=loadJson(level_file(id));
        S.level_m=id;
        S.level_name=data.value("name","未命名关卡");
        S.hint=data.value("hint","没有提示哦");
        S.val=data.at("inbox").get<vector<int>>();
        S.inbox_n=S.val.size();
        S.target_outbox=data.at("target_outbox").get<vector<int>>();
        S.target_outbox_n=S.target_outbox.size();
        S.ground_n=data["ground_n"].get<int>();
        S.allowed=data["instructions"].get<vector<string>>();    
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
    int cur_level;
    Stimulation_state S;
    init_console();
    while(1){
        if(game_sta==STA_START){
            cur_level=start_page(max_level(),process_value());
            cout << "You Choose Task" << cur_level << endl;
            if(!cur_level){break;}
            if(!load_level(cur_level,S)){alert_wait("关卡文件损坏或缺失，无法开始!");}
            else{game_sta=STA_READY;}
        }else if(game_sta==STA_READY){
            ready_page(S);
            alert_wait(" ");
            break;
        }
    }
    
    return 0;
}
