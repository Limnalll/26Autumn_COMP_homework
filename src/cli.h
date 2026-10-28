#ifndef CLI_H
#define CLI_H

#include <cstdio>
#include <string>
#include "stimulator.h"

void init_console();
void alert_wait(std::string info);
int start_page(int max_level,int passed_level);

#endif