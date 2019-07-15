//written by andersonc0d3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv){
        FILE *fp = fopen("/levels/level10.pass", "r");
        struct {char pass[20], msg_err[20];} pwfile = {{0}};
        char ptr[0];

        if(!fp || argc != 2)
                return -1;

        fread(pwfile.pass, 1, 20, fp);
        pwfile.pass[19] = 0;
        ptr[atoi(argv[1])] = 0;
        fread(pwfile.msg_err, 1, 19, fp);
        fclose(fp);

        if(!strcmp(pwfile.pass, argv[1]))
                execl("/bin/sh", "sh", 0);
        else
                puts(pwfile.msg_err);


        return 0;
}
