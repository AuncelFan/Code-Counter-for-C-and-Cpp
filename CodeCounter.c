#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("ERROR: Invalid arguments\nUse as: \"count <filename>\"\n");
        exit(1);
    }
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("ERROR: No such file\n");
        exit(1);
    }
    printf("%s\n", argv[1]);
    unsigned int codes = 0;
    unsigned int comments = 0;
    char temp;
    while (!feof(fp))
    {
        temp = fgetc(fp);
        // blanks
        if (temp == ' ' || temp == '\n')
            continue;
        // quoted passage, regarded as valid characters
        if (temp == '\"')
        {
            while (!feof(fp))
            {
                if (fgetc(fp) == '\"')
                    break;
                codes++;
            }
            codes += 2;
            continue;
        }
        // special
        if (temp == '\\')
        {
            fgetc(fp);
            codes++;
            continue;
        }
        if (temp == '/')
        {
            temp = fgetc(fp);
            if (temp == '/')
            { // single comment
                while (!feof(fp))
                {
                    temp = fgetc(fp);
                    if (temp == ' ')
                        continue;
                    if (temp == '\n')
                        break;
                    comments++;
                }
                continue;
            }
            if (temp == '*')
            { // multiple comments
                while (!feof(fp))
                {
                    temp = fgetc(fp);
                    if (temp == ' ' || temp == '\n')
                        continue;
                    if (temp == '*')
                    {
                        temp = fgetc(fp);
                        if (temp == '/')
                            break;
                        else
                        {
                            comments++;
                            continue;
                        }
                    }
                    comments++;
                }
                continue;
            }
        }
        // valid characters
        codes++;
    }
    fclose(fp);
    unsigned int total = codes + comments;
    float per = 100 * (float)comments / (float)(total);
    printf("-----------------------------------------------\n");
    printf("   codes   comments    total   comments/total\n");
    printf("%8d %10d %8d          %%%6.2f\n", codes, comments, total, per);
    getchar();
}