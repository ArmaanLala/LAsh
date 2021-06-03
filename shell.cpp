#include "lash.h"

int main()
{

    string input;
    char **split;

    while (!cin.eof())
    {
        cout << "$ ";
        getline(cin, input);

        input = subshell(input);

        char *conversion = (char *)malloc(input.length() + 1);
        strcpy(conversion, input.c_str());

        int separateCmd = 0;
        char **seperateCommands = parse_input(conversion, &separateCmd, ";");

        for (int i = 0; i < separateCmd; i++)
        {
            handleLogic(seperateCommands[i]);
        }
        // cout << "length: " << length << endl;
    }
    return 0;
}

string subshell(string input)
{

    // Handle Subshell Parsing
    if (input.find("(") != -1)
    {
        string temp = input.substr(input.find('(') + 1);
        temp = temp.substr(0, temp.find(')'));
        subshell(temp);
        temp = input.substr(0, input.find('('));
        input = input.substr(input.find(')') + 1);
        input = temp + input;
    }

    // cout << "SUBSHELL" << endl;
    int pid;
    int status;
    if ((pid = fork()) == 0)
    {
        char *conversion = (char *)malloc(input.length() + 1);
        strcpy(conversion, input.c_str());

        int separateCmd = 0;
        char **seperateCommands = parse_input(conversion, &separateCmd, ";");

        for (int i = 0; i < separateCmd; i++)
        {
            handleLogic(seperateCommands[i]);
        }
        exit(0);
    }
    else
    {
        wait(&status); // Blocking Wait
    }
    return input;
}

int execute(char *input)
{

    char **split;
    int length = 0;
    split = parse_input(input, &length);

    int pid;
    int status;
    int error = 1;

    // cd is not a shell function and when we cd in the child
    // this does not change the parent processes location so we must do something else
    if (strcmp("cd", split[0]) == 0)
    {
        chdir(split[1]);
        return 0;
    }

    if (strcmp("exec", split[0]) == 0)
    {
        int val = execvp(split[1], split);
        return val;
    }

    if ((pid = fork()) < 0)
    {
        // Failure Case
        cout << "fork failed" << endl;
        exit(1);
    }
    else if (pid == 0)
    {
        //Child Process
        error = execvp(split[0], split);
        exit(error);
    }
    else
    {
        //Parent
        wait(&status); // Blocking Wait
        if (WIFEXITED(status))
        {
            // Debug statements
            // cout << "Job Exited with status " << WEXITSTATUS(status) << endl;
        }
        else if (WIFSIGNALED(status))
        {
            // cout << "Child was signaled, sig = " << WTERMSIG(status) << endl;
        }
        return (status);
    }
}

void handleLogic(char *input)
{
    char *executable;
    string command = input;
    int andLoc = command.find("&&");
    int orLoc = command.find("||");
    int errCode;
    bool skip = false;

    while (andLoc != -1 || orLoc != -1)
    { //OR
        if ((orLoc > 0 && andLoc < 0) || (andLoc > 0 && orLoc > 0 && orLoc < andLoc))
        {
            // cout << "OR" << endl;
            executable = (char *)malloc(command.substr(0, orLoc).length() + 1);
            strcpy(executable, command.substr(0, orLoc).c_str());
            command = command.substr(orLoc + 2);
            // cout << executable << endl;
            if (!skip)
            {
                errCode = execute(executable);
                cout << errCode << endl;

                if (errCode != 0)
                {
                    skip = false;
                }
                else
                {
                    skip = true;
                }
            }
        }
        //AND
        else if ((andLoc > 0 && orLoc < 0) || (andLoc > 0 && orLoc > 0 && orLoc > andLoc))
        {
            executable = (char *)malloc(command.substr(0, andLoc).length() + 1);
            strcpy(executable, command.substr(0, andLoc).c_str());
            command = command.substr(andLoc + 2);
            // cout << executable << endl;
            if (!skip)
            {
                errCode = execute(executable);
                // cout << errCode << endl;
                if (errCode != 0)
                {
                    skip = true;
                }
                else
                {
                    skip = false;
                }
            }
        }
        command = trim(command, " ");
        // cout << "TEST " << command << endl;
        andLoc = command.find("&&");
        orLoc = command.find("||");
    }
    if (!skip)
    {
        executable = (char *)malloc(command.length() + 1);
        strcpy(executable, command.c_str());
        command = "";
        errCode = execute(executable);
    }
    skip = false;
}

char **parse_input(char *input, int *index, char *separator)
{
    char **returnArr = (char **)malloc(8 * sizeof(char *));
    char *parsed;

    parsed = strtok(input, separator);
    while (parsed != NULL)
    {
        returnArr[*index] = parsed;
        (*index)++;
        parsed = strtok(NULL, separator);
        // cout << *index << endl;
    }

    returnArr[*index] = NULL;
    return returnArr;
}
