#include <string>
#include <vector>

using namespace std;

string read_line();
void split_input(string input, vector<string> *args);
string get_exec_path(string program_name);
bool is_valid_path(string path);
void lush_exec(vector<string> args);