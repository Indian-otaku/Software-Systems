# SIC 2 Pass Assembler program

## Files and their functionalities: 
- input.txt is used to store the input SIC program.
- output.txt is the resulting SIC object program.
- symtab.txt is used to store the symbols during pass 1 to be used later on in pass 2.
- optab.txt is used to store the opcode with its corresponding mnemonic numbers.
- intmed.txt is used to store the intermediate code generated by pass 1 which is later used as input program for pass 2.
- pass1.c is the pass 1 C program.
- pass2.c is the pass 2 C program.
- psize.txt is used to store the program size which is calculated at end of pass 1 which is used later on in pass 2.

## Inputs and outputs of the program: 
### During pass 1
- **Inputs:**
1. input.txt - used to store the SIC program.
2. optab.txt - used to check for valid opcode.
- **Outputs:**
1. intermed.txt - used to store intermediate SIC program.
2. symtab.txt - used to store and check symbols and their respective addresses.
3. psize.txt - used to store program size. 
### During pass 2
- **Inputs:**
1. intmed.txt - used to fetch intermediate code for pass 2.
2. optab.txt - used to fetch opcodes for creating object codes.
3. symtab.txt - used to fetch symbol addresses for creating object codes.
4. psize.txt - used to fetch program size for making header record.
- **Outputs:**
1. output.txt - used to store the final object program.

---
Clear symtab.txt and intmed.txt every time you re run the pass 1 program.
