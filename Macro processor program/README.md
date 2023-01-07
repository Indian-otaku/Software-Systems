# Macro processor program
## Algorithm
![image](https://user-images.githubusercontent.com/84381682/211160211-a06fcd9a-a748-4fad-9532-7750d3c581e3.png)
![image](https://user-images.githubusercontent.com/84381682/211160219-9f63c79d-96b9-4ac1-b8c8-0a63994d9df4.png)
![image](https://user-images.githubusercontent.com/84381682/211160240-76666b66-1b12-4b93-99ef-af092fc72238.png)
![image](https://user-images.githubusercontent.com/84381682/211160251-eac9e854-82ce-443b-ba85-5cc5891d0a41.png)

## Explanation

> Procedure DEFINE which is called when the beginning of a macro definition is recognized makes the appropriate entries in DEFTAB and NAMTAB.

> EXPAND is called to set up the argument values in ARGTAB and expand a Macro Invocation statement.

> Procedure GETLINE is called to get the next line to be processed either from the DEFTAB or from the input file .

> Handling of macro definition within macro:- When a macro definition is encountered it is entered in the DEFTAB. The normal approach is to continue entering till MEND is encountered. If there is a program having a Macro defined within another Macro.While defining in the DEFTAB the very first MEND is taken as the end of the Macro definition. This does not complete the definition as there is another outer Macro which completes the definition of Macro as a whole. Therefore the DEFINE procedure keeps a counter variable LEVEL.Every time a Macro directive is encountered this counter is incremented by 1. The moment the innermost Macro ends indicated by the directive MEND it starts decreasing the value of the counter variable by one. The last MEND should make the counter value set to zero. So when LEVEL becomes zero, the MEND corresponds to the original MACRO directive.

