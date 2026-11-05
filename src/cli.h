#ifndef CLI_H
#define CLI_H

#include <cstdio>
#include <string>
#include "stimulator.h"

void init_console();
void clear_screen();
bool file_exist(std::string route);
void alert_wait(std::string info);
int start_page(int max_level,int passed_level,Stimulation_state& S);
bool ready_page(Stimulation_state &S);
void play_page(Stimulation_state &S);
void over_page(Stimulation_state &S);

#endif