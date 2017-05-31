// PL homework: hw2
// regexp_matcher.h

#ifndef _PL_HOMEWORK_REGEXP_MATCHER_H_
#define _PL_HOMEWORK_REGEXP_MATCHER_H_

#include <map>
#include <set>
#include <vector>
#include <iostream>

using namespace std;


struct RegExpMatcher {

};

enum LRAction {
    INVALID = 0,
    SHIFT = 1,
    REDUCE = 2,
    ACCEPT = 3,
    GOTO = 4
};

struct LRParser {
    map<int,pair<int, int>> rules; // (number,(lhs symbol,num of rhs))
    map<pair<int, int>,pair<LRAction, int>> LRTable; // ((state,symbol),(action,next state))
};

struct NFA_structure
{
    map<pair<int, char>, int> nfa;
    int start;
    int end;
}typedef NFA;

struct Stack{
    int top;
    char *stack = NULL;

    Stack(int len)
    {
        top = -1;
        stack = (char*)malloc(sizeof(char)*len);
    }

    void push(char value)
    {
        stack[++top] = value;
    }

    char pop()
    {
        return stack[top--];
    }

    void deleteStack()
    {
        free(stack);
    }
};

int num_elements = 81;
int LRTable[][4] = {
        {0,'a',SHIFT,6},
        {0,'(',SHIFT,5},
        {0,'[',SHIFT,7},
        {1,'$',ACCEPT,0},
        {2,')',REDUCE,1},
        {2,'|',SHIFT,8},
        {2,'$',REDUCE,1},
        {3,'a',SHIFT,6},
        {3,'(',SHIFT,5},
        {3,')',REDUCE,3},
        {3,'|',REDUCE,3},
        {3,'[',SHIFT,7},
        {3,'$',REDUCE,3},
        {4,')',REDUCE,5},
        {4,'|',REDUCE,5},
        {4,'*',SHIFT,10},
        {4,'$',REDUCE,5},
        {4,'(',REDUCE,5},
        {4,'a',REDUCE,5},
        {4,'[',REDUCE,5},
        {5,'a',SHIFT,6},
        {5,'(',SHIFT,5},
        {5,'[',SHIFT,7},
        {6,')',REDUCE,8},
        {6,'(',REDUCE,8},
        {6,'|',REDUCE,8},
        {6,'*',REDUCE,8},
        {6,'$',REDUCE,8},
        {6,'[',REDUCE,8},
        {6,'a',REDUCE,8},
        {7,'a',SHIFT,13},
        {8,'a',SHIFT,6},
        {8,'(',SHIFT,5},
        {8,'[',SHIFT,7},
        {9,')',REDUCE,4},
        {9,'|',REDUCE,4},
        {9,'$',REDUCE,4},
        {10,')',REDUCE,6},
        {10,'|',REDUCE,6},
        {10,'$',REDUCE,6},
        {10,'(',REDUCE,6},
        {10,'a',REDUCE,6},
        {10,'[',REDUCE,6},
        {11,')',SHIFT,15},
        {12,']',SHIFT,16},
        {13,']',REDUCE,11},
        {13,'a',SHIFT,13},
        {14,')',REDUCE,2},
        {14,'$',REDUCE,2},
        {15,')',REDUCE,7},
        {15,'|',REDUCE,7},
        {15,'*',REDUCE,7},
        {15,'$',REDUCE,7},
        {15,'(',REDUCE,7},
        {15,'[',REDUCE,7},
        {15,'a',REDUCE,7},
        {16,')',REDUCE,9},
        {16,'(',REDUCE,9},
        {16,'|',REDUCE,9},
        {16,'*',REDUCE,9},
        {16,'$',REDUCE,9},
        {16,'a',REDUCE,9},
        {16,'[',REDUCE,9},
        {17,']',REDUCE,10},
        {0,'E',GOTO,1},
        {0,'T',GOTO,2},
        {0,'F',GOTO,3},
        {0,'C',GOTO,4},
        {3,'T',GOTO,9},
        {3,'F',GOTO,3},
        {3,'C',GOTO,4},
        {5,'E',GOTO,11},
        {5,'T',GOTO,2},
        {5,'F',GOTO,3},
        {5,'C',GOTO,4},
        {7,'A',GOTO,12},
        {8,'E',GOTO,14},
        {8,'T',GOTO,2},
        {8,'F',GOTO,3},
        {8,'C',GOTO,4},
        {13,'A',GOTO,17}
};

int num_rules = 11;
int rules[][3] = {
        {1,'E',1},
        {2,'E',3},
        {3,'T',1},
        {4,'T',2},
        {5,'F',1},
        {6,'F',2},
        {7,'C',3},
        {8,'C',1},
        {9,'C',3},
        {10,'A',2},
        {11,'A',1}
};


bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher);
bool RunRegExpMatcher(const RegExpMatcher* regexp_matcher, const char* str);
void BuildLRParser(LRParser* lr_parser);
bool RunLRParser(const LRParser* lr_parser, const char* str, int *reduces,int *num_reduces);

// Homework 2.2
bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher){

    LRParser lr_parser;
    BuildLRParser(&lr_parser);

    int regexp_len = (int)strlen(regexp);
    char* conv_regexp = (char*)malloc(sizeof(char)*regexp_len+1);

    // convert all terminals to 'a' except operators
    for(int i=0;i<regexp_len+1;i++)
    {
        if(regexp[i] != '(' && regexp[i] != ')' && regexp[i] != '|' && regexp[i] != '*' && regexp[i] != '[' && regexp[i] != ']' && regexp[i] != '$')
            conv_regexp[i] = 'a';
        else
            conv_regexp[i] = regexp[i];

        if(i == regexp_len)
            conv_regexp[i] = '$';
    }

    int *reduces = (int*)malloc(sizeof(int)*regexp_len);
    int num_reduces = 0;
    if(RunLRParser(&lr_parser, conv_regexp, reduces, &num_reduces) == false)
        return false;

    //for(int i=0;i<num_reduces;i++)
    //    cout<<reduces[i]<<" ";

    NFA *nfas = (NFA*)malloc(0);
    int nfas_size = 0;

    int k=0;
    for(int i=0;i<num_reduces;i++)
    {
        if(reduces[i] == 11 || reduces[i] == 10 || reduces[i] == 8)
        {
            for(;k<regexp_len;k++)
            {
                if(conv_regexp[k] == 'a')
                {
                    break;
                }
            }
            k++;
        }
        else if(reduces[i] == 2)
        {

        }
        else if(reduces[i] == 4)
        {

        }
        else if(reduces[i] == 6)
        {

        }
        else if(reduces[i] == 7)
        {

        }
        else if(reduces[i] == 9)
        {

        }
    }

    return true;
};

// Homework 2.2
bool RunRegExpMatcher(const RegExpMatcher* regexp_matcher, const char* str){
    return false;
};

void BuildLRParser(LRParser* lr_parser) {

    for(int i=0;i<num_elements;i++)
    {
        lr_parser->LRTable.insert(make_pair(make_pair(LRTable[i][0], LRTable[i][1]), make_pair((LRAction)LRTable[i][2], LRTable[i][3])));

    }

    for(int i=0;i<num_rules;i++)
    {
        lr_parser->rules.insert(make_pair(rules[i][0],make_pair(rules[i][1], rules[i][2])));
    }
}

bool RunLRParser(const LRParser* lr_parser, const char* str, int *reduces,int *num_reduces) {

    int inputStrLen = (int)strlen(str);
    int index_str = 0; // index of str


    // stack and top index
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


        if(find_result.first == REDUCE && (find_result.second == 2 || find_result.second == 4 || find_result.second == 6 || find_result.second == 7 || find_result.second == 8 || find_result.second == 9 || find_result.second == 10 || find_result.second == 11))
        {
            reduces[(*num_reduces)++] = find_result.second;
        }

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
                // 스택에서 제거.
                for(int i=0;i<rule.second*2;i++)
                {
                    stack[top--] = NULL;
                }
                int temp_top = stack[top];
                stack[++top] = rule.first;
                // GOTO func
                stack[++top] = lr_parser->LRTable.find(make_pair(temp_top,rule.first))->second.second;

                break;
        }
    }
}
#endif  //_PL_HOMEWORK_REGEXP_MATCHER_H_

