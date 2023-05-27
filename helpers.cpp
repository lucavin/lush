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

extern char **environ;

string read_line() {
  string input;
  int c;

  while (true) {
    c = getchar();

    if (c == EOF) {
      cerr << "\n"
           << "input EOF: exiting lush\n";
      exit(1);
    } else if (c == '\n') {
      input += '\0';
      return input;
    } else {
      input += c;
    }
  }
}

void split_input(string input, vector<string> *args) {
  istringstream input_ss(input);

  string argument;

  while (input_ss >> argument) {
    args->push_back(argument);
  }
}

void lush_exec(vector<string> args) {
  string command_path = get_exec_path(args[0]);

  if (command_path.empty()) {
    cout << "lush: command not found: " << args[0] << '\n';
  } else {
    pid_t pid;
    size_t i;
    char **c_args = new char *[args.size() + 1];
    for (i = 0; i < args.size(); i++) {
      c_args[i] = new char[args[i].size() + 1];
      strcpy(c_args[i], args[i].c_str());
    }
    c_args[i] = NULL;

    if (posix_spawnp(&pid, c_args[0], NULL, NULL, c_args, environ) != 0) {
      perror("spawn");
    }

    // wait for spawned processes to finish
    int exit_status;
    if (waitpid(pid, &exit_status, 0) == -1) {
      perror("waitpid");
    }

    cout << command_path << " exit status was " << exit_status << '\n';

    for (size_t i = 0; i < args.size() + 1; i++) {
      delete[] c_args[i];
    }
    delete[] c_args;
  }
}

string get_exec_path(string command_name) {
  // looks through $PATH folders and returns the path to the program that was
  // called.

  string path = getenv("PATH");
  stringstream path_ss(path);
  string dir;

  while (getline(path_ss, dir, ':')) {
    string executable_path = dir.append("/");
    executable_path.append(command_name);
    if (is_valid_path(executable_path)) {
      return executable_path;
    }
  }
  return "";
}

bool is_valid_path(string path) {
  struct stat s;

  if (stat(path.c_str(), &s) != 0) {
    return false;
  } else {
    return true;
  }
}