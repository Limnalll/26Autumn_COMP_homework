#include <vector>
#include <string>
#include <iostream>

enum ActionType {
    MOVE_POSITION = 1,
    CHANGE_STAND = 2,
    CHANGE_OUTBOX = 3,
    CHANGE_GROUND = 4,
    CHANGE_CURBLOCK = 5,
    CHANGE_INBOX = 6,
    CHANGE_CUR_M = 7,
    CHANGE_HAS_BLOCK = 8
};

struct Adjustment {
    int sta;
    int new_value;
    int extra_pos;

    Adjustment(int s, int v, int ep = -1) : sta(s), new_value(v), extra_pos(ep) {}
};

struct StimulationState {
    // 环境状态
    std::vector<int> inbox;
    int inbox_n;
    std::vector<int> outbox;
    int outbox_n;
    std::vector<int> ground_block;
    std::vector<bool> ground_has_block; 
    
    // 机器人状态
    int position;
    int cur_block;
    bool isStand;
    bool has_block = false;
    
    // 执行控制
    int m;
    int cur_m;
    std::vector<Adjustment> seq; 
    
    // 报错与结束控制
    int cur_line;
    bool is_game_over;
    bool is_error;
    std::string error_msg;

    void update() {
        if (seq.empty()) return;
        
        Adjustment adj = seq.front();
        seq.erase(seq.begin());
        
        switch (adj.sta) {
            case 1: 
                position = adj.new_value;
                break;
            case 2: 
                isStand = (adj.new_value == 1); 
                break;
            case 3: 
                outbox.push_back(adj.new_value); 
                outbox_n++; 
                break; 
            case 4: 
                ground_block[adj.extra_pos] = adj.new_value; 
                ground_has_block[adj.extra_pos] = true; 
                break;
            case 5: 
                cur_block = adj.new_value; 
                break;
            case 6: 
                inbox_n = adj.new_value; 
                break; 
            case 7: 
                cur_m = adj.new_value;
                break;
            case 8:
                has_block = (adj.new_value == 1); 
                break;
        }
    }
};
