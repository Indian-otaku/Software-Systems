#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(){
	int locctr, sa, lctrt, er = 0, temp;
	char label[10], mnemonic[10], operand[30], symbolst[10], mnemonict[10], opcode[3];
	FILE* intmedf, * optabf, * symtabf, * inputf, * psizef;
	inputf = fopen("input.txt", "r");
	intmedf = fopen("intmed.txt", "w");
	fscanf(inputf, "%s %s %s", label, mnemonic, operand);
	if (strcmp(mnemonic, "START") == 0) {
		sa = strtol(operand, NULL, 16);
		locctr = sa;
		fprintf(intmedf, "%x\t%s\t%s\t%s\n", locctr, label, mnemonic, operand);
		fscanf(inputf, "%s %s %s", label, mnemonic, operand);
	}
	else {
		locctr = 0X0000;
	}
	while (strcmp(mnemonic, "END") != 0) {
		fprintf(intmedf, "%x\t", locctr);
		if (strcmp(label, "-") != 0) {
			symtabf = fopen("symtab.txt", "a+");
			while ((fscanf(symtabf, "%x %s", lctrt, symbolst)) != EOF) {
				if (strcmp(symbolst, label) == 0) {
					er++;
				}
			}
			if (er!=0){
				printf("Error duplicate symbol\n");
			}
			else {
				fprintf(symtabf, "%x\t%s\n", locctr, label);
			}
		}
		er = 0;
		optabf = fopen("optab.txt", "r");
		while ((fscanf(optabf, "%s %s", mnemonict, opcode)) != EOF) {
			if (strcmp(mnemonict, mnemonic) == 0) {
				er++;
			}
		}
		if (er != 0) {
			locctr += 3;
		}
		else if (strcmp(mnemonic, "WORD") == 0) {
			locctr += 3;
		}
		else if (strcmp(mnemonic, "RESW") == 0) {
			locctr += 3 * strtol(operand, NULL, 16);
		}
		else if (strcmp(mnemonic, "RESB") == 0) {
			locctr += strtol(operand, NULL, 16);
		}
		else if (strcmp(mnemonic, "BYTE") == 0) {
			if (operand[0] == 'C') {
				temp = strlen(operand) - 2;
			}
			else if (operand[0] == 'X') {
				temp = (strlen(operand) - 2) / 2;
			}
			locctr += temp;
		}
		else {
			printf("Error invalid operation code\n");
		}
		er = 0;
		fprintf(intmedf, "%s\t%s\t%s\n", label, mnemonic, operand);
		fscanf(inputf, "%s %s %s", label, mnemonic, operand);
	}
	fprintf(intmedf, "%x\t%s\t%s\t%s\n", locctr, label, mnemonic, operand);
	fclose(symtabf);
	fclose(intmedf);
	psizef = fopen("psize.txt", "w");
	printf("The program length is %d\n", (locctr - sa));
	fprintf(psizef, "%x", (locctr - sa));
	fclose(psizef);
}
