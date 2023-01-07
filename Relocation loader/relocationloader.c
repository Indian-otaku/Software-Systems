#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main(){
    FILE *input, *output;
    char objcode[20], progname[7],temp[10],operation[2];
    int sa,locctr,progaddr,hblen,locsearch,seek;
    input = fopen("input.txt","r");
    output = fopen("output.txt","w");
    printf("Enter the PROGADDR: ");
    scanf("%x",&progaddr);
    fscanf(input, "%s%s%x%s",objcode,progname,&locctr,objcode);
    locctr += progaddr;
    fscanf(input, "%s", objcode);
    while (strcmp(objcode,"M") != 0){
        if (strcmp(objcode,"T") == 0){
            fscanf(input, "%s%s%s", objcode,objcode,objcode);
        }
        fprintf(output, "%06x\t%s\n", locctr, objcode);
        locctr += strlen(objcode)/2;
        fscanf(input, "%s", objcode);
    }
    fclose(output);
    while (strcmp(objcode,"E") != 0){
        fscanf(input,"%x",&locsearch);
        fscanf(input,"%x",&hblen);
        fscanf(input,"%s",operation);
        fscanf(input,"%s",objcode);
        output = fopen("output.txt","r");
        fscanf(output,"%x%s",&locctr,objcode);
        while (locsearch+progaddr > locctr){
            fscanf(output,"%x%s",&locctr,objcode);
        }
        seek = ftell(output)-hblen-10-strlen(objcode);
        fseek(output,seek,SEEK_SET);
        fscanf(output,"%x",&locsearch);
        fclose(output);
        output = fopen("output.txt","r+");
        fseek(output,seek,SEEK_SET);
        fprintf(output,"%x",locsearch+progaddr);
        fclose(output);
        fscanf(input,"%s",objcode);
    }
}
