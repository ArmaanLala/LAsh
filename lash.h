#ifndef LASH_H
#define LASH_H

#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <string.h>

// String Helper Functions
std::string &rtrim(std::string &s, const char *t = " ");
std::string &ltrim(std::string &s, const char *t = " ");
std::string &trim(std::string &s, const char *t = " ");

char **parse_input(char *input, int *index, char *separator = " ");

int execute(char *input);
void handleLogic(char *input);
int main();


using namespace std;

#endif
