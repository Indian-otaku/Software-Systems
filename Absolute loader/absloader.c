#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void main() {
	char data[80], temp[20], c;
	int st_addr = 0, pg_len = 0, index = 0, mloc, mfs;
	FILE* objprog, * exeprog;
	objprog = fopen("output.txt", "r");
	exeprog = fopen("result.txt", "w");
	fgetc(objprog); fgetc(objprog);
	index = 0;
	while ((c = fgetc(objprog)) != '^') {
		data[index++] = c;
	}
	data[index] = '\0';
	printf("Program name: %s\n", data);
	// Getting starting address and length from object program
	index = 0;
	while ((c = fgetc(objprog)) != '^') {
		data[index++] = c;
	}
	data[index] = '\0';
	st_addr = strtol(data, NULL, 16);
	index = 0;
	while ((c = fgetc(objprog)) != '\n') {
		data[index++] = c;
	}
	data[index] = '\0';
	pg_len = strtol(data, NULL, 16);
	index = 0;
	printf("Enter the memory location: ");
	scanf("%x", &mloc);
	printf("Enter the memory free space: ");
	scanf("%x", &mfs);
	if (mloc <= st_addr && mfs >= pg_len && (mloc + mfs) >= (st_addr + pg_len)) {
		printf("Accepted check restult.txt for answer\n");
		while ((c = fgetc(objprog)) != 'E') {
			fscanf(objprog, "%s", data);
			index = 0;
			for (int i = 1; i <= 6 ; i++) { // Finding the starting memory address for the whole text record
				temp[i - 1] = data[i];
			}
			temp[6] = '\0';
			mloc = strtol(temp, NULL, 16);
			for (int i = 11; i < strlen(data); i++) {
				if (data[i] == '^') {
					temp[index] = '\0';
					fprintf(exeprog, "%06x\t%s\n", mloc, temp);
					mloc += strlen(temp) / 2;
					index = 0;
				}
				else {
					temp[index++] = data[i];
				}
			}
			temp[index] = '\0';
			fprintf(exeprog,"%06x\t%s\n", mloc, temp);
			mloc += strlen(temp) / 2;
			index = 0;
			fgetc(objprog); // To Get rid of \n
		}
	}
	else {
		printf("Rejected\n");
	}
}