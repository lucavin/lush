#include <iostream>
#include <unistd.h>

using namespace std;

#define LUSH_RL_BUFSIZE 1024
#define LUSH_TOK_BUFSIZE 64
#define LUSH_TOK_DELIM " \t\r\n\a"

void lush_loop(void);
char *lush_read_line(void);
char **lush_split_line(char *line);
int lush_launch(char **args);

int lush_num_builtins();
int lush_cd(char **args);
int lush_help(char **args);
int lush_exit(char **args);

int lush_exec(char **args);

const char *builtin_str[] = {"cd", "help", "exit"};

int (*builtin_func[])(char **) = {&lush_cd, &lush_help, &lush_exit};

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  lush_loop();

  return 0;
}

void lush_loop(void) {
  char *line;
  char **args;
  int status;

  do {
    cout << "> ";
    line = lush_read_line();
    args = lush_split_line(line);
    status = lush_exec(args);

    delete[] line;
    delete[] args;
  } while (status);
}

char *lush_read_line(void) {
  int bufsize = LUSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = new char[bufsize];
  int c;

  if (!buffer) {
    cerr << "lush: allocation error\n";
    exit(1);
  }

  while (1) {
    c = getchar();

    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    if (position >= bufsize) {
      bufsize += LUSH_RL_BUFSIZE;
      buffer = (char *)realloc(buffer, bufsize);

      if (!buffer) {
        cerr << "lush: allocation error\n";
        exit(1);
      }
    }
  }
}

char **lush_split_line(char *line) {
  int bufsize = LUSH_TOK_BUFSIZE;
  int position = 0;
  char **tokens = new char *[bufsize];
  char *token;

  if (!tokens) {
    cerr << "lush: allocation error\n";
    exit(1);
  }

  token = strtok(line, LUSH_TOK_DELIM);

  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LUSH_TOK_BUFSIZE;
      tokens = (char **)realloc(tokens, bufsize * sizeof(char *));

      if (!tokens) {
        cerr << "lush: allocation error\n";
        exit(1);
      }
    }

    token = strtok(NULL, LUSH_TOK_DELIM);
  }

  tokens[position] = NULL;
  return tokens;
}

int lush_launch(char **args) {
  pid_t pid;
  int status;

  pid = fork();

  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      cerr << "lsh";
    }
    exit(1);
  } else if (pid < 0) {
    cerr << "lsh";
  } else {
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int lush_num_builtins() { return sizeof(builtin_str) / sizeof(char *); }

int lush_cd(char **args) {
  if (args[1] == NULL) {
    cerr << "lush: unexpected argument to \"cd\"\n";
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;
}

int lush_help(char **args) {
  (void)args;
  int i;
  cout << "Luca's lush\n";
  cout << "Type program names and arguments, and hit enter.\n";
  cout << "The following are built in:\n";

  for (i = 0; i < lush_num_builtins(); i++) {
    cout << "\t" << builtin_str[i] << '\n';
  }

  cout << "Use the man command for information on other programs.\n";
  return 1;
}

int lush_exit(char **args) {
  (void)args;
  return 0;
}

int lush_exec(char **args) {
  int i;

  if (args[0] == NULL) {
    // Triggered when empty command is entered.
    return 1;
  }

  for (i = 0; i < lush_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return lush_launch(args);
}
