#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main() {
	int ctr1 = 0, ctr2 = 0, source_int, txtp=0, count=0;
	FILE* intmedf, * psizef, * symtabf, * optabf, * outputf;
	char locctr[10], label[10], mnemonic[10], operand[30], name[6]="      ", psize[5], objcode[100], 
		opcode[10],mno[10], lctrt[10], symbolst[10], opaddr[10]="0000", target_string[3], sa[10], sao[10];
	char txtrec[30][60];
	intmedf = fopen("intmed.txt", "r");
	outputf = fopen("output.txt", "a");
	psizef = fopen("psize.txt", "r");
	optabf = fopen("optab.txt", "r");
	fscanf(intmedf, "%s %s %s %s", locctr, label, mnemonic, operand);
	strcpy(sa, locctr);
	strcpy(sao, locctr);
	if (strcmp("START", mnemonic) == 0) {
		strcpy(name, label);
	}
	fscanf(psizef, "%s", psize);
	fclose(psizef);
	fprintf(outputf,"H^%6s^%06s^%06s\n", name, sao, psize);
	fscanf(intmedf, "%s %s %s %s", locctr, label, mnemonic, operand);
	while (strcmp("END", mnemonic) != 0) {
		ctr1 = 0; ctr2 = 0;
		optabf = fopen("optab.txt", "r");
		while ((fscanf(optabf, "%s %s", opcode, mno)) != EOF) {
			if (strcmp(opcode, mnemonic) == 0) {
				ctr1++;
				break;
			}
		}
		fclose(optabf);
		if (ctr1 > 0) {
			if (strcmp(operand, "-") != 0) {
				symtabf = fopen("symtab.txt", "r");
				while ((fscanf(symtabf, "%s %s", lctrt, symbolst)) != EOF) {
					if (strcmp(operand, symbolst) == 0) {
						ctr2++;
						break;
					}
				}
				fclose(symtabf);
				if (ctr2 > 0) {
					strcpy(opaddr, lctrt);
				}
				else {
					printf("Error flag: Undefined symbol\n");
				}
			}
			strcat(mno, opaddr);
			strcpy(objcode, mno);
		}
		else if (strcmp(mnemonic, "BYTE") == 0) {
			strcpy(objcode, "");
			for (int i = 2; i < strlen(operand); i++) {
				source_int = operand[i];
				sprintf(target_string, "%x", source_int);
				strcat(objcode, target_string);
			}
		}
		else if (strcmp(mnemonic, "WORD") == 0) {
			sprintf(name, "%x", strtol(operand, NULL, 10));
			strcpy(objcode, name);
		}
		else {
			strcpy(objcode, "");
		}
		count = 0;
		for (int i = 0; i < txtp; i++) {
			if (strlen(txtrec[i]) > 6 || strcmp(mnemonic, "BYTE") == 0) {
				for (int j = 0; j < strlen(txtrec[i]); j++) {
					count++;
				}
			}
			else {
				count += 6;
			}
		}
		if (count + strlen(objcode) + 1 > 60) {
			fprintf(outputf, "T^%06s^%02x", sao, count / 2);
			printf("Count value is %d;\n", count);
			for (int i = 0; i < txtp; i++) {
				fprintf(outputf, "^%06s", txtrec[i]);
			}
			fprintf(outputf, "\n");
			txtp = 0;
			strcpy(sao, locctr);
			memset(txtrec, 0, 30 * 60 * sizeof txtrec[0][0]);
		}
		if (strcmp(objcode, "") != 0) {
			strcpy(txtrec[txtp], objcode);
			txtp++;
		}
		fscanf(intmedf, "%s %s %s %s", locctr, label, mnemonic, operand);
	}
	fprintf(outputf, "T^%06s^%02x", sao, count/2);
	for (int i = 0; i < txtp; i++) {
		fprintf(outputf, "^%06s", txtrec[i]);
	}
	fprintf(outputf, "\n");
	fprintf(outputf, "E^%06s\n",sa );
	fclose(outputf);
	printf("Pass 2 complete\nCheck output.txt file\n");
}
