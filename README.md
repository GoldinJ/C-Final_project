# C-Course-Assembler-Project
The final assignment (#14) of the C course (20465) at The Open University.  
Written by: 

 - [GoldinJ](https://github.com/GoldinJ)
 - [MishkaZK](https://github.com/MishkaZK)

This project is based on the **_two-pass assembler_** model.  
**Note:** the computer model for this project and the given assembly language are **imaginary**.

## Getting Started

The project was coded and compiled using Ubuntu, but it may run on all Linux versions.

### Usage

Use makefile to compile the project like this:
```
>   make
```
**Note:** A **full path** should be provided in case the `<path>/<file>.as` file is not in a **root** directory.

After preparing assembly files **with an `.as` extension**, open *terminal* and pass file names as arguments (without the file extensions) as following:

As for the files x.as, y.as, hello.as we will run:
```
>   assembler x y hello
```
The assembler will generate output files with the same filenames and the following extensions:  
- `.ob`  - Object file
- `.ent` - Entries file
- `.ext` - Externals file
- `.am`  - After Macro files

An example of input and output files can be found under the 'tests' folder.

## Hardware
- CPU
- RAM (including a stack), with the size of 1024 *words*.
- A *word*'s size in memory is **12 bits**.
- Uses signed *2's complement* arithmetic for integers (with no support for real numbers).

   ### Registers
   The CPU has 9 registers: 8 general registers (named r0,r2,...,r7).

## Instruction Structure
Every software *instruction* is encoded into a few *words* in memory (max is 5 *words*).
The first word is of the following structure:

|  11&emsp;8  |  8&emsp;5  |  4&emsp;2  |  1&emsp;0  |
| ------------------------ | ---------- | ---------- | ---------- |
|           Source operand addressing method         | Opcode | Destination operand addressing method | A,R,E

Encoding of each instruction word is done using **base64** as defined here: [link](https://en.wikipedia.org/wiki/Base64)


## INSTRUCTION LINE

Instruction line will consist of 3-4 fields:

| LABEL   | OPCODE | OPERAND1  | OPERAND2 |
|---------|--------|-----------|----------|
OR

| OPCODE | OPERAND1  | OPERAND2 |
|---------|--------|-----------|

- for example: 

```
>   HELLO: mov @r3 , @r7
```


- LABEL: is a word consists of letter or numbers and is a maximum of 31 characters long and ends with ' : ' , it will appear at the beginning of an instruction (note: the label can't be one of the reserved word of the language).


- OPCODE: 1 of the 16 allowed opcode(see commands section) , it will determine the action done on the operand fields.


- OPERAND1:depending on the opcode type can receive 1 of 3 :( LABEL , REGISTER , INTEGER ).


- OPERAND2:depending on the opcode type can receive 1 of 3 :( LABEL , REGISTER , INTEGER ).
- note: in the operand fields the label will not contain ':'


- INTEGER: in our language integer is a whole number positive or negative. 


## Commands
The commands and operand field that are allowed:

| Opcode (decimal) | Command Name | Operand1               | Operand2           |
| ---------------- | ------------ |------------------------|--------------------|
|	0	|	mov	| Label,Integer,Register | Label,Register     |
|	1	|	cmp	| Label,Integer,Register | Label,Integer,Register | 
|	2	|	add	| Label,Integer,Register | Label,Register     | 
|	3	|	sub	| Label,Integer,Register | Label,Register     | 
|	4	|	not	| null                   | Label,Register | 
|	5	|	clr	| null                   | Label,Register | 
|	6	|	lea	| Label                  | Label,Register     |
|	7	|	inc	| null                   | Label,Register | 
|	8	|	dec	| null                   | Label,Register  | 
|	9	|	jmp	| null                   |  Label,Register  | 
|	10	|	bne	| null                   | Label,Register |
|	11	|	red	| null                   |  Label,Register| 
|	12	|	prn	| null                   |  Label,Integer,Register|
|	13	|	jsr	| null                   |  Label,Register  |
|	14	|	rts	| null                   | null               |
|	15	|	stop	| null                   | null               | 

## Directives
A **_directive_** line of the following structure:

1. An **optional** preceding *label*. e.g. `PLACE1: `.
2. A _directive_: `.data`, `.string`, `.struct`, `.entry` or `.extern`.
3. Operands according to the type of the *directive*.

   ### `.data`
   This directive allocates memory in the data image for storing received integers later in memory (also increases the _data counter_ and updates the _symbol table_).
   The parameters of `.data` are valid integers (separated by a comma).  
   e.g. `LABEL1: .data      +7, -56, 4, 9`.
   
   ### `.string`
   This direcive receives a string as an operand and stores it in the data image. It stores all characters by their order in the string, encoded ny their *ASCII* values.
   e.g. `STRING1: .string "abcdef"` is a valid directive.
   
 
   ### `.entry`
   This directive outputs a received name of a *label* to the *symbol table*, so that later it will be recognized by other assembly files (and they would be able to use it).
   e.g. 
   ```
   ; file1.as
   .entry HELLO
   HELLO: add @1, @r4 
   ```
   ### `.extern`
   This directive receives a name of a *label* as a parameter and declares the *label* as being external (defined in another file) and that the current file shall use it.  
   This way, the directive `.extern HELLO` in `file2.as` will match the `.entry` directive in the previous example.
