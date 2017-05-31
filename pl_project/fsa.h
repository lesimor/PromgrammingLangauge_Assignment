// PL homework: hw1
// fsa.h

#ifndef _PL_HOMEWORK_FSA_H_
#define _PL_HOMEWORK_FSA_H_

#include <vector>
#include <string>
#include <set>
#include <utility>
using namespace std;


// Valid characters are alphanumeric and underscore (A-Z,a-z,0-9,_).
// Epsilon moves in NFA are represented by empty strings.

struct FSATableElement {
    int state;
    int next_state;
    std::string str;
};


struct NFATableElement {
    bool is_accept;
    set<int> state_set;

    // <next_char,
    vector <pair<std::string, set<int> > > next_char_states_pair;

};

struct FiniteStateAutomaton {
    vector <NFATableElement> NFATable;
};

set<int> follow_e(set<int> start_state_set, const std::vector<FSATableElement> &elements);
void generateFSA(const std::vector<FSATableElement> &elements, const std::vector<int> &accept_states, set<int> start_states, FiniteStateAutomaton* fsa);
void printFSA(FiniteStateAutomaton* fsa);

// Run FSA and return true if str is matched by fsa, and false otherwise.
bool RunFSA(const FiniteStateAutomaton &fsa, const std::string &str){
    // NFA set
    vector<NFATableElement> NFATable_vec = fsa.NFATable;

    NFATableElement current_nfa_element = NFATable_vec[0];

    for(string::const_iterator str_iter = str.begin(); str_iter != str.end() ; ++str_iter){
        // 현재 nfa_element의 next_char_states_pair에 해당 문자열이 있는지 확인.
        bool char_pass_flag = false;
        for(vector<pair<std::string, set<int> > >::iterator pair_vector_it = current_nfa_element.next_char_states_pair.begin() ; pair_vector_it != current_nfa_element.next_char_states_pair.end(); ++pair_vector_it){
            stringstream ss;
            string s;
            char c = *str_iter;
            ss << c;
            ss >> s;

            if(pair_vector_it->first == s){
                // 만약 있으면 NFATable_vec에서 해당 pair의 next_states set을 탐색하여 해당 NFATableElement를 current_nfa_element로 설정.
                for(vector<NFATableElement>::iterator nfa_table_elements_it = NFATable_vec.begin() ; nfa_table_elements_it != NFATable_vec.end() ; ++nfa_table_elements_it){
                    if(nfa_table_elements_it->state_set == pair_vector_it->second){
                        current_nfa_element = *nfa_table_elements_it;
                        char_pass_flag = true;
                        break;
                    }
                }
            }

            if (char_pass_flag) break;
        }
        if(char_pass_flag == false){
            return false;
        }

    }
    // 탐색 후 current_nfa_element가 is_accept가 아닌 경우 false 반환.
    if(current_nfa_element.is_accept){
        return true;
    } else {
        return false;
    }

};

bool BuildFSA(const std::vector<FSATableElement> &elements,
              const std::vector<int> &accept_states,
              FiniteStateAutomaton *fsa){

    // 엡실런을 따라가서 첫번째 set을 만든다.
    // state가 1인 FSATableElement를 다 찾는다.
    set<int> start_set;
    start_set.insert(1);

    set<int> set_init = follow_e(start_set, elements);

    generateFSA(elements, accept_states, set_init, fsa);

    // 디버깅용 프린트문.
    // printFSA(fsa);

    return true;
};

// start_state를 출발점으로 엡실런을 따라간 모든 state를 반환.
set<int> follow_e(set<int> start_state_set, const std::vector<FSATableElement> &elements){
    set<int> result_set;
    set<int>::iterator set_iter;
    vector<FSATableElement>::const_iterator iter;  // 벡터 반복자 시작지점

    for(set_iter = start_state_set.begin() ; set_iter != start_state_set.end() ; ++set_iter){
        // 일단 1은 포함.
        result_set.insert(*set_iter);

        for (iter = (elements).begin(); iter != (elements).end(); ++iter){

            if(iter->state == *set_iter && iter->str == ""){
                result_set.insert(iter->next_state);

                // 재귀호출
                set<int> tmp_set;
                tmp_set.insert(iter->next_state);
                set<int> nested_result_set = follow_e(tmp_set, elements);

                result_set.insert(nested_result_set.begin(), nested_result_set.end());
            }
        }
    }

    return result_set;

}

void generateFSA(const std::vector<FSATableElement> &elements, const std::vector<int> &accept_states, set<int> start_states, FiniteStateAutomaton* fsa){
    // Finite State Automation
    // 입력의 모든 경우의 수를 파악.
    set<string> all_occurrence;
    set<string>::iterator string_set_iter;
    vector<FSATableElement>::const_iterator iter;  // 벡터 반복자 시작지점
    set<int>::iterator set_iter;

    vector<set<int> > uniqueness_check;
    vector<set<int> >::iterator uniqueness_check_iter;

    uniqueness_check.push_back(start_states);

    set<int> accept_states_set;
    for(int i = 0 ; i < accept_states.size() ; i++){
        accept_states_set.insert(accept_states[i]);
    }

    for (iter = (elements).begin(); iter != (elements).end(); ++iter){
        if(iter->str == "") continue;
        // 문자열 하나씩 쪼개기
        string tokens = iter->str;
        for(int i = 0 ; i < tokens.length() ; i++){
            stringstream ss;
            string s;
            char c = tokens[i];
            ss << c;
            ss >> s;
            all_occurrence.insert(s);
        }

    }

    for(int i = 0 ; i < uniqueness_check.size() ; i++){
        // 모든 경우의 수에 대해서 FiniteStateAutomation 생성.

        vector<set<int> >::iterator nested_uniqueness_check_iter;
        NFATableElement fsa_tmp;

        for(string_set_iter = all_occurrence.begin() ; string_set_iter != all_occurrence.end(); ++string_set_iter){

            // set의 각 요소에서 next_char를 따라간 state들의 집합.
            set<int> next_states;

            // 각 set 요소마다 탐색.
            // 엡실런 확장.
            set<int> start_states_original = uniqueness_check[i];
            start_states = follow_e(uniqueness_check[i], elements);
            for(set_iter = start_states.begin() ; set_iter != start_states.end() ; ++set_iter){
                // 테이블 전체 탐색
                for (iter = (elements).begin(); iter != (elements).end(); ++iter){
                    // state와 next_char를 확인.
                    // 해당 문자가 문자열에 포함된다면..
                    if(iter->state == *set_iter && (iter->str).find(*string_set_iter) != std::string::npos){
                        next_states.insert(iter->next_state);
                    }
                }
            }

            // next_char_states_pair 설정.
            fsa_tmp.next_char_states_pair.push_back(pair<string, set<int> >(*string_set_iter, next_states));

            if(std::find(uniqueness_check.begin(), uniqueness_check.end(), next_states) == uniqueness_check.end()){
                uniqueness_check.push_back(next_states);
            }


            fsa_tmp.state_set = start_states_original;

            // fsa_tmp.is_accept 판단
            set<int> accept_states_intersect;
            set_intersection(accept_states_set.begin(),accept_states_set.end(),start_states_original.begin(),start_states_original.end(),
                             std::inserter(accept_states_intersect,accept_states_intersect.begin()));
            if (accept_states_intersect.size() > 0 ) {
                fsa_tmp.is_accept = true;
            } else {
                fsa_tmp.is_accept = false;
            }

        }
        fsa->NFATable.push_back(fsa_tmp);
    }
}

void printFSA(FiniteStateAutomaton* fsa){
    vector <NFATableElement>::iterator NFATable_iter;
    for(NFATable_iter = fsa->NFATable.begin() ; NFATable_iter != fsa->NFATable.end(); ++NFATable_iter){
        set<int>::iterator set_iter;
        cout << "<";
        for(set_iter = NFATable_iter->state_set.begin() ; set_iter != NFATable_iter->state_set.end() ; ++set_iter){
            cout << " " << *set_iter << " ";
        }
        cout << ">";
        if(NFATable_iter->is_accept){
            cout << "*";
        }

        cout << "|";

        vector <pair<std::string, set<int> > >::iterator pair_iter;
        for(pair_iter = NFATable_iter->next_char_states_pair.begin() ; pair_iter != NFATable_iter->next_char_states_pair.end() ; ++pair_iter){
            cout << get<0>(*pair_iter) << " ";
            set<int>::iterator nested_set_iter;
            cout << "{";
            for(nested_set_iter = get<1>(*pair_iter).begin() ; nested_set_iter != get<1>(*pair_iter).end(); ++nested_set_iter){
                cout << " " << *nested_set_iter << " ";
            }
            cout << "}";
        }
        cout << endl;
    }
}


#endif //_PL_HOMEWORK_FSA_H_

