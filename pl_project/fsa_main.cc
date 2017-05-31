// PL homework: hw1
// fsa_main.cc

#include <stdio.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "fsa.h"

using namespace std;

// 파일을 읽어서 fsa_elements에 데이터를 추가.
bool LoadFSA(const char* filename,
             vector<FSATableElement>* fsa_elements,
             vector<int>* accept_states) {
  // 파일 오픈.
  FILE* fp = fopen(filename, "rt");

  // 파일이 없는 경우 false 반환.
  if (fp == NULL) return false;

  // fsa_elements 벡터 초기화.
  fsa_elements->clear();

  // accept_states 벡터 초기화.
  accept_states->clear();

  // 첫번째 문자열로부터 accept_states 정보를 받아온다.
  char buf[1024];
  fgets(buf, 1024, fp);
  istringstream iss(buf);
  while (iss.good()) {
    int accept_state;
    iss >> accept_state;
    accept_states->push_back(accept_state);
  }

  // 파일의 마지막까지 한줄씩 차례로 읽는다.
  while (!feof(fp)) {
    FSATableElement elem;
    char str_buf[1024] = "";
    if (fscanf(fp, "%d %d %s", &elem.state, &elem.next_state, str_buf) < 3) {
      break;
    }

    // 엡실런이 아닌 경우에만 str 항목에 해당 문자열 저장.
    if (strcmp(str_buf, "#") != 0) elem.str = str_buf;
    fsa_elements->push_back(elem);
  }
  fclose(fp);
  return true;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr << "give the FSA file.\n";
    return -1;
  }
  vector<FSATableElement> fsa_elements;
  vector<int> accept_states;
  if (LoadFSA(argv[1], &fsa_elements, &accept_states) == false) {
    cerr << "unable to load the FSA file " << argv[1] << ".\n";
    return -1;
  }
  FiniteStateAutomaton fsa;
  if (BuildFSA(fsa_elements, accept_states,  &fsa) == false) {
    cerr << "unable to build the FSA (" << argv[1] << ").\n";
    return -1;
  }

  string input_str;
  while (!cin.eof()) {
    // Get user input.
    cout << "> ";
    std::getline(cin, input_str);
    bool accept = RunFSA(fsa, input_str.c_str());
    cout << "input: '" << input_str << "' = " << (accept? "O" : "X") << endl;
  }
  return 0;
}


