#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void DEFINE();
void GETLINE();

int EXPANDING;
char label[10],opcode[10],operand[40];
FILE *mprog, *deftab, *argtab, *namtab;


void PROCESSLINE() {
	int temp = 0,ema,sma;
	char name[30];
	printf("Processline\n");
    fseek(namtab,0,SEEK_SET);
    namtab = fopen("namtab.txt","r");
    fseek(namtab,0,SEEK_SET);
	while (fscanf(namtab,"%s%d%d",name,&sma,&ema) != EOF){
		if (strcmp(name,opcode) == 0){
			temp = 1;
		}
	}
    fclose(namtab);    
    if (temp == 1) {
     	EXPAND();
    }else if (strcmp(opcode,"MACRO") == 0){
		DEFINE();
	}else if (temp == 0){
		fprintf(output, "%s\t%s\t%s\n", label, opcode, operand);
	}
}

void DEFINE() {
    namtab = fopen("namtab.txt","w");
	int LEVEL = 1, posarg = 1, tind = 0, tind2 = 0, tind4 = 0, sma, ema;
	char tempoperand[20], temp2[20], paravar[100], temp4[20];
	printf("Define\n");
    fseek(namtab,0,SEEK_END);
	fprintf(namtab, "%s\t", label);
	sma = ftell(deftab);
	fprintf(deftab, "%s\t%s\t%s\n", label, opcode, operand);
	strcpy(paravar,operand);
	while (LEVEL > 0) {
		tind = 0;
		strcpy(tempoperand, "");
		GETLINE();
		for (int i=0; i<strlen(operand); i++) {
			if (operand[i] == '&') {
				tind2 = 0;
				strcpy(temp2, "");
				i++;
				while (operand[i] != ',' && operand[i] != '\0' && operand[i] != '\'') {
					temp2[tind2++] = operand[i];
					i++;
				}
                temp2[tind2] = '\0';
                posarg = 0;
				for (int j=0; j<strlen(paravar); j++){
					if (paravar[j] == '&'){
						tind4 = 0;
						strcpy(temp4, "");
						j++;
						while (paravar[j] != ',' && paravar[j] != '\0'){
							temp4[tind4++] = paravar[j];
							j++;
						}
						temp4[tind4] = '\0';
						if (strcmp(temp2,temp4) == 0){
							posarg++;
							break;
						}
					}
					posarg++;
				}
				tempoperand[tind] = '?';
				tind++;
				tempoperand[tind] = posarg + '0'; // Converting posarg integer to equivalent character form
				tind++;
				tempoperand[tind] = operand[i];
				tind++;
			}
			else{
				tempoperand[tind] = operand[i];
				tind++;
			}
			tempoperand[tind] = '\0';
		}
		strcpy(operand, tempoperand);
		fprintf(deftab, "%s\t%s\t%s\n", label, opcode, operand);
		if (strcmp(opcode,"MACRO") == 0) {
			LEVEL += 1;
		}else if (strcmp(opcode,"MEND") == 0) {
			LEVEL -= 1;
		}
        if (strcmp(opcode,"END")==0){
            break;
        }
	}
	ema = ftell(deftab);
	fprintf(namtab, "%d\t%d\n", sma,ema);
    fclose(namtab);
}

void GETLINE() {
	char *argarray, temp[20], arg[10];
	int tind = 0;
	printf("Getline\n");
	if (EXPANDING == 1){
		fscanf(deftab, "%s%s%s", label, opcode, operand);
		for (int i=0; i<strlen(operand); i++) {
			if (operand[i] == '?') {
				i++;
                fseek(argtab,0,SEEK_SET);
				for (int j=1; j<=(int)operand[i]-48; j++){
					fscanf(argtab, "%s", arg);
				}
				for (int j=0; j<strlen(arg); j++){
					temp[tind] = arg[j];
					tind++;
				}
			}else{
				temp[tind] = operand[i];
				tind++;
			}
			temp[tind] = '\0';
		}
		strcpy(operand,temp);
	}else{
		fscanf(mprog, "%s%s%s", label, opcode, operand);
	}
}

	
