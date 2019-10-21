/*My shell, YeongJun Seo(dudwns7479@gmail.com), 2016.11.07*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>     //bool???�수 ?�더?�일
#include<sys/wait.h>
#include<string.h>
#include<fcntl.h>      
#define OUTER ">"
#define BACK "&"
void help()
{
    printf("********************HELP********************\n");
    printf("cd      : change directory.\n");
    printf("help    : open this help.\n");
    printf("exit    : exit this shell.\n");
    printf("ls      : look at file name in current directory.\n");
    printf("********************HELP********************\n");
}

int tokenize(char* buf, char* delims, char* tokens[], int maxTokens)
{
    char* token;
    int token_count=0;

    token = strtok(buf,delims);     // 첫번�?strtok ?�출?� 문자?�과  ?��?문자�??�수�??�출

    while(token != NULL&&token_count<maxTokens)     //tokens ?�기가 ?�때까�? 루프
    {
        tokens[token_count++] = token;              // token 배열???�는??
        token = strtok(NULL, delims);               // strtok ?�번�??�출부?�는 NULL값을 ?�달?�다.
    }

    tokens[token_count]=NULL;                       //token 배열 ?�에 NULL???�는??

    return token_count;                             //?�큰 ??반환
}

bool run(char *line)        //C?�서??stdbool.h ?�더?�일 ?�용
{
    char* tokens[1024];     //?�큰 배열
    char* tokens_tmp[1024]; //?�큰 ?�시 ?�?�할 배열
    char* delims = " \n";   // ?�큰??구분??문자 지??
    pid_t pid;              // pid
    int i, j, fd, time;
    int redirect=0;         // > 문자 ?��? 구별?�는 변??

    int token_count = tokenize(line,delims,tokens,sizeof(tokens)/sizeof(char*));    //tokenize ?�수 ?�출

    for(i=0; i<token_count; i++)
    {
        if(!strcmp(tokens[i],OUTER))
        {
            redirect = O_WRONLY | O_TRUNC | O_CREAT;
            break;              // 명령?�에 >가 ?�어가?��? ?�색(?��???
        }
    }

    if(token_count==0)
        return true;            // ?�큰 개수가 0개면 ?�냄

    if((pid = fork())<0)
    {
        perror("fork error\n");
        return false;           //fork ?�러 처리
    }

    if(pid==0)
    {
        for(j=0; j<i; j++)
        {
           tokens_tmp[j]=tokens[j];
        }

        if(redirect>0)
        {
            fd=open(tokens[i+1],redirect,0664);
            close(STDOUT_FILENO);
            dup2(fd,STDOUT_FILENO);         // >가 ?�을???�출?�재처리
        }
        execvp(tokens_tmp[0],tokens_tmp);
        close(fd);
        exit(0);
    }

    else
    {
        if(strcmp(tokens[token_count-1],BACK))   // 명령???�에 &가 ?�다�??�행
            wait(&time);                           //background processing ?�행�?
    }

    return true;
}

int main()
{
    char line[1024];    // 명령?��? ?�을 변??

    while(1)
    {
        printf("%s $ ",get_current_dir_name()); //?�재 ?�치�??��??�다.
        fgets(line,sizeof(line)-1,stdin);   //명령?��? ?�력받는??

        if(strncmp(line,"exit\n",5)==0)    // exit 명령???�행�?
            break;
        if(strncmp(line,"help\n",5)==0)    // help 명령???�행�?
            help();
        run(line);                        // run ?�수 ?�출
    }
    return 0;
}

