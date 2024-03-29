/*My shell, YeongJun Seo(dudwns7479@gmail.com), 2016.11.07*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>     //bool???¨ì ?¤ë?ì¼
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

    token = strtok(buf,delims);     // ì²«ë²ì§?strtok ?¸ì¶? ë¬¸ì?´ê³¼  ?ë?ë¬¸ìë¥??¸ìë¡??¸ì¶

    while(token != NULL&&token_count<maxTokens)     //tokens ?¬ê¸°ê° ? ëê¹ì? ë£¨í
    {
        tokens[token_count++] = token;              // token ë°°ì´???£ë??
        token = strtok(NULL, delims);               // strtok ?ë²ì§??¸ì¶ë¶?°ë NULLê°ì ?ë¬?ë¤.
    }

    tokens[token_count]=NULL;                       //token ë°°ì´ ?ì NULL???£ë??

    return token_count;                             //? í° ??ë°í
}

bool run(char *line)        //C?ì??stdbool.h ?¤ë?ì¼ ?¬ì©
{
    char* tokens[1024];     //? í° ë°°ì´
    char* tokens_tmp[1024]; //? í° ?ì ??¥í  ë°°ì´
    char* delims = " \n";   // ? í°??êµ¬ë¶??ë¬¸ì ì§??
    pid_t pid;              // pid
    int i, j, fd, time;
    int redirect=0;         // > ë¬¸ì ?¬ë? êµ¬ë³?ë ë³??

    int token_count = tokenize(line,delims,tokens,sizeof(tokens)/sizeof(char*));    //tokenize ?¨ì ?¸ì¶

    for(i=0; i<token_count; i++)
    {
        if(!strcmp(tokens[i],OUTER))
        {
            redirect = O_WRONLY | O_TRUNC | O_CREAT;
            break;              // ëªë ¹?´ì >ê° ?¤ì´ê°?ì? ?ì(?¬ì???
        }
    }

    if(token_count==0)
        return true;            // ? í° ê°ìê° 0ê°ë©´ ?ë

    if((pid = fork())<0)
    {
        perror("fork error\n");
        return false;           //fork ?ë¬ ì²ë¦¬
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
            dup2(fd,STDOUT_FILENO);         // >ê° ?ì???ì¶?¥ì¬ì²ë¦¬
        }
        execvp(tokens_tmp[0],tokens_tmp);
        close(fd);
        exit(0);
    }

    else
    {
        if(strcmp(tokens[token_count-1],BACK))   // ëªë ¹???ì &ê° ?ë¤ë©??¤í
            wait(&time);                           //background processing ?¤íë¬?
    }

    return true;
}

int main()
{
    char line[1024];    // ëªë ¹?´ë? ?´ì ë³??

    while(1)
    {
        printf("%s $ ",get_current_dir_name()); //?ì¬ ?ì¹ë¥??í??¸ë¤.
        fgets(line,sizeof(line)-1,stdin);   //ëªë ¹?´ë? ?ë ¥ë°ë??

        if(strncmp(line,"exit\n",5)==0)    // exit ëªë ¹???¤íë¬?
            break;
        if(strncmp(line,"help\n",5)==0)    // help ëªë ¹???¤íë¬?
            help();
        run(line);                        // run ?¨ì ?¸ì¶
    }
    return 0;
}

