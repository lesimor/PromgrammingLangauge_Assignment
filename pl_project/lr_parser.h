// PL homework: hw2
// lr_parser.h

#ifndef _PL_HOMEWORK_LR_PARSER_H_
#define _PL_HOMEWORK_LR_PARSER_H_

#include <map>
#include <set>

using namespace std;

enum LRAction {
    INVALID = 0,
    SHIFT = 1,
    REDUCE = 2,
    ACCEPT = 3,
    GOTO = 4
};

// The data fields for each type of action:
//   SHIFT : symbol = the input character, next_state = the next state.
//   REDUCE : symbol = lhs of the rule, next_state = the rule id.
//   ACCEPT : none.
//   GOTO : symbol = the state on the stack top, next_state = the next state.

struct LRTableElement {
    int state;
    int symbol;
    LRAction action;
    int next_state;
};

struct LRRule {
    int id;
    int lhs_symbol;
    int num_rhs;
};

struct LRParser {

    // grammar rules
    map<int,pair<int, int> > rules; // (number,(lhs symbol,num of rhs))

    // LRTable
    map<pair<int, int>,pair<LRAction, int> > LRTable; // ((state,symbol),(action,next state))
};

bool BuildLRParser(const LRTableElement* elements, int num_elements,
                   const LRRule* rules, int num_rules,
                   LRParser* lr_parser);
bool RunLRParser(const LRParser* lr_parser, const char* str);

// Homework 2.1
bool BuildLRParser(const LRTableElement* elements, int num_elements,
                   const LRRule* rules, int num_rules,
                   LRParser* lr_parser){

    if(elements == NULL || rules == NULL || lr_parser == NULL)
        return false;

    for(int i=0;i<num_elements;i++)
    {
        lr_parser->LRTable.insert(make_pair(make_pair(elements[i].state,elements[i].symbol),
                                            make_pair(elements[i].action, elements[i].next_state)));
    }

    for(int i=0;i<num_rules;i++)
    {
        lr_parser->rules.insert(make_pair(rules[i].id,make_pair(rules[i].lhs_symbol, rules[i].num_rhs)));
    }


    return true;
};


// Homework 2.1
bool RunLRParser(const LRParser* lr_parser, const char* str){

    int inputStrLen = (int)strlen(str);
    int index_str = 0; // 문자열의 위치.

    // 스택 초기화.
    int* stack = (int*)malloc(sizeof(int)*inputStrLen*2);
    int top=0;
    memset(stack, NULL, sizeof(int)*inputStrLen*2);
    stack[top] = 0;

    LRAction nextAction;
    pair<LRAction, int> find_result;
    pair<int,int> rule;
    while(true)
    {

        if(lr_parser->LRTable.find(make_pair(stack[top],str[index_str])) == lr_parser->LRTable.end())
            return false;

        find_result = lr_parser->LRTable.find(make_pair(stack[top],str[index_str]))->second;

        nextAction = find_result.first;

        switch(nextAction)
        {
            case INVALID: // 0
                free(stack);
                return false;
            case ACCEPT: // 3
                free(stack);
                return true;
            case SHIFT: // 1
                stack[++top] = str[index_str++];
                stack[++top] = find_result.second;
                break;
            case REDUCE: // 2
                rule = lr_parser->rules.find(find_result.second)->second; // (lhs, rhs_num)
                // 스택으로부터 pop.
                for(int i=0;i<lr_parser->rules.find(find_result.second)->second.second*2;i++)
                {
                    stack[top--] = NULL;
                }
                int temp_top = stack[top];
                stack[++top] = rule.first;
                // GOTO
                stack[++top] = lr_parser->LRTable.find(make_pair(temp_top,rule.first))->second.second;
                break;
        }
    }
};

#endif //_PL_HOMEWORK_LR_PARSER_H_

