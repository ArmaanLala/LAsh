#include <iostream>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>

using namespace std;

char **parse_input(char *input, int* index, char * separator = " ") {
    char **returnArr = (char**)malloc(8 * sizeof(char *));
    char *parsed;

    parsed = strtok(input, separator);
    while (parsed != NULL) {
        returnArr[*index] = parsed;
        (*index)++;
        parsed = strtok(NULL, separator);
        cout << *index << endl;
    }

    returnArr[*index] = NULL;
    return returnArr;
}

int counter(char** input) {
    int count = 0;

    while(*(input[0]) != '\0'){

    }
    return count;
}

void execute(char** split) {
    int pid;
    int status;
    if (strcmp("cd", split[0]) == 0) {
        chdir(split[1]);
        return;
    }
    
    if ((pid = fork()) < 0) {
        // Failure Case
        cout << "fork failed" << endl;
        exit(1);

    } else if (pid == 0) { 
        //Child Process
        execvp(split[0],split);
        exit(0);

    } else {
        //Parent
        wait(&status);      // Blocking Wait
        if(WIFEXITED(status)) {
            // Debug statements
            cout << "Job Exited with status " << WEXITSTATUS(status) << endl;
        } else if (WIFSIGNALED(status) ) {
            cout << "Child was signaled, sig = " << WTERMSIG(status) << endl;
        }
    }
}


int main() {

    string input;
    char** split;

    while(!cin.eof()) {

        cout << "$ ";
        getline(cin, input);
        char* conversion = (char*) malloc(input.length() + 1);
        strcpy(conversion,input.c_str());
        
        int separateCmd = 0;

        
    
        int length = 0;
        split = parse_input(conversion,&length);

        execute(split);
        // cout << "length: " << length << endl;
        // cd is not a shell function and when we cd in the child
        // this does not change the parent processes location so we must do something else

        
    }
    return 0;

}
