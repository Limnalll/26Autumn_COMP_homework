#ifndef STIMULATOR_H
#define STIMULATOR_H

#include <string>
#include <vector>

// ============================================================
// 更新项 Adjustment
// 一条指令会被拆成若干"更新项"，逐项应用到状态上；
// 界面每应用一项就重画一帧，从而做出机器人走路/搬积木的动画。
// ============================================================
enum StaType {
    STA_CUR_M     = 1,  // new_value: 指令编号变为该值(1-based)
    STA_POSITION  = 2,  // new_value: 机器人新位置(-1=输入带侧, 0..ground_n-1=空地, -2=输出带侧)
    STA_ISSTAND   = 3,  // new_value: 1=站起(准备移动) 0=蹲下(工作姿态)
    STA_OUTBOX    = 4,  // new_value: 追加到输出传送带末尾的积木序号
    STA_GROUND    = 5,  // extra_pos: 空地编号; new_value: 放上去的积木序号
    STA_CUR_BLOCK = 6,  // new_value: 机器人手中的积木序号(0=空手)
    STA_INBOX     = 7,  // new_value: 新的"已从输入带取走的积木数"
    STA_VALUE     = 8   // extra_pos: 积木序号; new_value: 该积木的新数字(add/sub 改写手中积木)
};

struct Adjustment {
    int sta;
    int new_value;
    int extra_pos;   // 仅 STA_GROUND(空地编号) / STA_VALUE(积木序号) 使用
};

struct Stimulation_state {
    // ---- 关卡信息（从 level/N.json 载入）----
    int level_m = 1;                  // 当前关卡编号
    std::string level_name;           // 关卡名（可选字段）
    std::vector<std::string> allowed; // 可用指令集
    std::string hint;                 // 任务描述/关卡提示

    // ---- 输入传送带 ----
    //std::vector<int> inbox;           // inbox[i]: 传送带上第 i 块积木的序号
    int inbox_n = 0;                  // 初始积木总数
    int inbox_m = 0;                  // 已经取走了前 inbox_m 块

    // ---- 输出传送带 ----
    std::vector<int> outbox;          // 已放上输出带的积木序号（按放置顺序）
    int outbox_m = 0;                 // 已放上块数（= outbox.size()，冗余字段方便读代码）
    std::vector<int> target_outbox;   // 目标序列（存积木数字）
    int target_outbox_n = 0;

    // ---- 空地 ----
    std::vector<int> ground_block;    // ground_block[i]: i 号空地上的积木序号(0=空地)
    int ground_n = 0;                 // 空地总数

    // ---- 机器人 ----
    int cur_block = 0;                // 手中积木序号(0=空手)
    int position = -1;                // 机器人位置(含义同 STA_POSITION)
    bool isStand = false;             // true=站立 false=下蹲

    // ---- 积木数字 ----
    std::vector<int> val;             // val[s]: 序号 s 的积木当前数字(val[0] 占位不用)
    int next_serial = 1;              // 下一个可分配的新序号

    // ---- 机器人程序 ----
    std::vector<std::string> prog;    // prog[i]: 第 i+1 条指令原文
    int m = 0;                        // 指令条数
    int cur_m = 1;                    // 运行到第几条指令(1-based)
    int executed = 0;                 // 已执行指令总数

    // ---- 当前指令拆出的更新序列----
    std::vector<Adjustment> adjustments;
    int game_result;
    std::string over_intro;
};

#endif