#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

using namespace std;

#define INPUT_SIZE 1024

void lush_loop();
void split_input(std::string input, vector<string> *args);

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  lush_loop();

  return 0;
}

// fn lush_loop
// initialise main variables
// read input
// split input into array of arguments
// run input
// loop
void lush_loop() {
  string input;
  vector<string> args;

  while (true) {
    cout << "\n> ";
    getline(cin, input);

    if (input == "") {
      continue;
    }

    split_input(input, &args);

    for (string i : args) {
      cout << i << '\n';
    }
    args.clear();
  }
}

// fn lush_read_line
// take command input
void split_input(string input, vector<string> *args) {
  istringstream ss(input);

  string argument;

  while (ss >> argument) {
    args->push_back(argument);
  }
}

// fn split_line
// split line into args

// fn lush_launch
// launch process with respective args

// helper functions
// cd
// help
// exit
