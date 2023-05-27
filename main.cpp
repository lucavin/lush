#include "helpers.hpp"
#include <cstdlib>
#include <iostream>
#include <spawn.h>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;

void lush_loop();

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  lush_loop();

  return 0;
}

void lush_loop() {
  vector<string> args;

  while (true) {
    cout << "\n> ";
    string input = read_line();
    split_input(input, &args);
    lush_exec(args);
    args.clear();
  }
}