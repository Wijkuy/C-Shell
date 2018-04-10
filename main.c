#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_LINE  80                                  /* the maximum length command */
#define MAX_HISTORY 100

typedef struct node {
    char name[MAX_LINE];
}Node;

void PrintList(Node nodeArray[], int num){
    int i;
    Node *node;
    for(i=0;i<num;i++){
        node = &nodeArray[i];
        printf("Node:%d Name:%s\n-----------\n",i+1,node->name);
    }
        printf("Total:%d\n\n",num);
}

char *getElement(Node nodeArray[],int num){
    Node *node;
    node = &nodeArray[num-1];
    char *c;
    //printf("Node:%d Name:%s\n-----------\n",num,node->name);
    c = node->name;
    //printf("Node:%s\n",c);
    return c;

}

int AddCommand(Node *node,char *c){
    char *input;
    input = c;
    //fgets(input,15,stdin);
    int ok =0;
    int res = 1;
    strncpy(node->name, input, MAX_LINE);
    //int res = sscanf(input,"%s", node->name);

    if(res == 1){
        ok = 1;
        printf("\nDEBUG----ADDED:%s\n\n",node->name);
    } else {
        printf("Sorry, error parsing input\n\n");
    }
    return ok;
}

int AddProcessCommand(char *commands, char *c){
    char *input;
    input = c;
    //fgets(input,15,stdin);
    int ok =0;
    int res = 1;
    strncpy(commands, input, MAX_LINE);

    if(res == 1){
        ok = 1;
        printf("\nPROCESS COMMANDS----ADDED:%s\n\n",commands);
    } else {
       printf("Sorry, error parsing input\n\n");
    }
    return ok;
}

void make_empty(char **arg_array, int rows, int cols){
     int i,j;
     for(i = 0; i <rows; i++)
     {
       for(j=0; j<cols;j++)
       {
          arg_array[i][j] = '\0';
       }
     }
    return;
}


int main(){
    Node linked[MAX_HISTORY];
    int numCommands = 0;
    int numProcessCommands = 0;
    int val;
    int should_run = 1;
    char command[MAX_LINE];
    char modifier[MAX_LINE];
    char input[MAX_LINE];
    char subString[MAX_LINE];
    char temp[MAX_LINE];
    char *args[MAX_LINE/2 + 1];           /* command line arguments */
    //printf("osh>");
    while(should_run){
        printf("osh>%s",temp);
        if(temp[0] != NULL){
           int res = sscanf(input,"%s%s%d",command,modifier);
           if(res>1){
                if(numCommands < MAX_HISTORY){
                    numProcessCommands+=AddProcessCommand(&args[numProcessCommands],command);
                    numProcessCommands+=AddProcessCommand(&args[numProcessCommands],modifier);
                    numProcessCommands+=AddProcessCommand(&args[numProcessCommands],NULL);
                    strcat(command," ");
                    strcat(command,modifier);
                    numCommands += AddCommand(&linked[numCommands],command);
                }else{
                    printf("Sorry List is Full!\n\n");
                }
            }else{
                if(numCommands < MAX_HISTORY){
                    numProcessCommands+=AddProcessCommand(&args[numProcessCommands],command);
                    numProcessCommands+=AddProcessCommand(&args[numProcessCommands],NULL);
                    numProcessCommands+=AddProcessCommand(&args[numProcessCommands],NULL);
                    numCommands += AddCommand(&linked[numCommands],command);
                }else{
                    printf("Sorry List is Full!\n\n");
                }
            }
            memset(temp, 0, MAX_LINE);
            printf("\n");
           printf("osh>%s",temp);
        }
        fgets(input,MAX_LINE-1,stdin);
        int res = sscanf(input,"%s%s",command,modifier);

        if(strncmp(command,"quit",4)==0){
            printf("\n\nExiting...\n");
            break;
        }
        else if(strncmp(command,"print",5)==0){
            PrintList(linked,numCommands);
        }
        else if(strncmp(command,"!!",2)==0){
            strncpy(temp, getElement(linked,numCommands), MAX_LINE);
            printf("\n");
        }else if(strncmp(command,"!",1)==0){
            strncpy(subString, command + 1, MAX_LINE-1);
            val = atoi(subString);
            strncpy(temp, getElement(linked,val), MAX_LINE);
                printf("\n");
        }else if(res>1){
            if(numCommands < MAX_HISTORY){
                strcat(command," ");
                strcat(command,modifier);
                numProcessCommands+=AddProcessCommand(&args[numProcessCommands],command);
                numProcessCommands+=AddProcessCommand(&args[numProcessCommands],modifier);
                numProcessCommands+=AddProcessCommand(&args[numProcessCommands],NULL);

                numCommands += AddCommand(&linked[numCommands],command);
            }else{
                printf("Sorry List is Full!\n\n");
            }
        }else{
            if(numCommands < MAX_HISTORY){
                numProcessCommands+=AddProcessCommand(&args[numProcessCommands],command);
                numProcessCommands+=AddProcessCommand(&args[numProcessCommands],NULL);
                numProcessCommands+=AddProcessCommand(&args[numProcessCommands],NULL);
                numCommands += AddCommand(&linked[numCommands],command);
            }else{
                printf("Sorry List is Full!\n\n");
            }

        }
        /** * After reading user input, the steps are:

        * (1) fork a child process using fork()

        * (2) the child process will invoke execvp()

        * (3) if command included &, parent will invoke wait()

        */
        if(numProcessCommands>0){
                pid_t pid;
                int status;

                pid = fork();
                if (pid == 0) {
                if (execvp(args[0], args) == -1) {
                perror("lsh");
                }
                exit(EXIT_FAILURE);
                } else if (pid < 0) {
                    perror("lsh");
                } else {
                    do {
                        waitpid(pid, &status, WUNTRACED);
                    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
                }
            numProcessCommands=0;
            }
        }

    fflush(stdout);
    return 0;
}
