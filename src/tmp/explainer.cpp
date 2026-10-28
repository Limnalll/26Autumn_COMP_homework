enum Type { 
    INBOX, 
    OUTBOX, 
    ADD, 
    SUB, 
    JUMP, 
    JUMPFZERO,
    COPYTO,
    COPYFROM, 
    UNKNOWN 
};

void explainer(StimulationState& s, const std::string& str, int place) {
    Type type = UNKNOWN;
    if (str == "inbox") type = INBOX;
    else if (str == "outbox") type = OUTBOX;
    else if (str == "add") type = ADD;
    else if (str == "sub") type = SUB;
    else if (str == "jump") type = JUMP;
    else if (str == "jumpifzero") type = JUMPFZERO;
    else if (str == "copyto") type = COPYTO; 
    else if (str == "copyfrom") type = COPYFROM; 

    switch (type) {
        case INBOX: {
            if (s.inbox_n >= (int)s.inbox.size()) {
                s.is_game_over = true; 
                return;
            }   
            if (s.position != -1) {
                if (!s.isStand) s.seq.push_back(Adjustment(2, 1));
                s.seq.push_back(Adjustment(1, -1));                    
                s.seq.push_back(Adjustment(2, 0));                     
            } else {
                if (s.isStand) s.seq.push_back(Adjustment(2, 0)); 
            }
            int new_block = s.inbox[s.inbox_n];
            s.seq.push_back(Adjustment(5, new_block));             
            s.seq.push_back(Adjustment(6, s.inbox_n + 1));     
            s.seq.push_back(Adjustment(8, 1));                     
            s.seq.push_back(Adjustment(2, 1));                     
            break;
        }
        
        case OUTBOX: {
            if (!s.has_block) {
                s.error_msg = "Error on instruction " + to_string(s.cur_line);
                s.is_error = true;
                return;
            }
            if (s.position != -2) {
                if (!s.isStand) s.seq.push_back(Adjustment(2, 1)); 
                s.seq.push_back(Adjustment(1, -2));                    
                s.seq.push_back(Adjustment(2, 0));                     
            } else {
                if (s.isStand) s.seq.push_back(Adjustment(2, 0)); 
            }
            s.seq.push_back(Adjustment(3, s.cur_block));          
            s.seq.push_back(Adjustment(5, 0));                         
            s.seq.push_back(Adjustment(8, 0));                     
            s.seq.push_back(Adjustment(2, 1));                   
            break;
        }
        
        case ADD:
        case SUB: {
            if (!s.has_block) {
                s.is_error = true;
                s.error_msg = "Error on instruction " + to_string(s.cur_line);
                return;
            }
            if (place < 0 || place >= (int)s.ground_block.size() || !s.ground_has_block[place]) {
                s.is_error = true;
                s.error_msg = "Error on instruction " + to_string(s.cur_line);
                return;
            }
            
            if (s.position != -3) {
                if (!s.isStand) s.seq.push_back(Adjustment(2, 1));
                s.seq.push_back(Adjustment(1, -3));        
                s.seq.push_back(Adjustment(2, 0));
            } else {
                if (s.isStand) s.seq.push_back(Adjustment(2, 0)); 
            }
            
            int new_value = 0;
            if (type == ADD) {
                new_value = s.cur_block + s.ground_block[place];
            } else {
                new_value = s.cur_block - s.ground_block[place];
            }
            s.seq.push_back(Adjustment(5, new_value));  
            s.seq.push_back(Adjustment(2, 1));
            break;
        }
        
        case JUMP:
        case JUMPFZERO: {
            if (place < 1 || place > s.m) {
                s.is_error = true;
                s.error_msg = "Error on instruction " + to_string(s.cur_line);
                return;
            }

            if (type == JUMPFZERO) {
                if (!s.has_block) { 
                    s.is_error = true;
                    s.error_msg = "Error on instruction " + to_string(s.cur_line);
                    return;
                }
                if (s.cur_block != 0) {
                    break; 
                }
            }

            s.cur_m = place - 1; 
            break;
        }
        
        case COPYTO:
        case COPYFROM: {
            if (place < 0 || place >= (int)s.ground_block.size()) {
                s.is_error = true;
                s.error_msg = "Error on instruction " + to_string(s.cur_line);
                return;
            }

            if (type == COPYTO) {
                if (!s.has_block) {
                    s.is_error = true;
                    s.error_msg = "Error on instruction " + to_string(s.cur_line);
                    return;
                }
            } else {
                if (!s.ground_has_block[place]) {
                    s.is_error = true;
                    s.error_msg = "Error on instruction " + to_string(s.cur_line);
                    return;
                }
            }

            if (s.position != -3) {
                if (!s.isStand) s.seq.push_back(Adjustment(2, 1));
                s.seq.push_back(Adjustment(1, -3)); 
                s.seq.push_back(Adjustment(2, 0));
            } else {
                if (s.isStand) s.seq.push_back(Adjustment(2, 0)); 
            }

            if (type == COPYTO) {
                s.seq.push_back(Adjustment(4, s.cur_block, place));
            } else {
                int new_val = s.ground_block[place];
                s.seq.push_back(Adjustment(5, new_val));
                s.seq.push_back(Adjustment(8, 1));
            }
            s.seq.push_back(Adjustment(2, 1)); 
            break;
        }
        default: {
            s.is_error = true;
            s.error_msg = "Error on instruction " + to_string(s.cur_line);
            break;
        }
    }
}
