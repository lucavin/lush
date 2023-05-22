#include <iostream>
#include <unistd.h>

using namespace std;

#define INPUT_SIZE 1024

void lush_loop();

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
  char *input = new char[INPUT_SIZE];
  char **args = new char *[1];

  while (true) {
    cout << "> ";
    read_input(input);
    split_input(input, args);
    exec_input(args);
  }

  delete input;
  delete[] args;
}

// fn lush_read_line
// take command input

// fn split_line
// split line into args

// fn lush_launch
// launch process with respective args

// helper functions
// cd
// help
// exit
