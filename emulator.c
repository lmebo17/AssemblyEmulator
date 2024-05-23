#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_REGISTERS 10
#define MAX_LINES 1000
#define MAX_LABELS 100

typedef struct {
    char label[20];
    int address;
} Label;

typedef struct {
    char instruction[10];
    char operand1[10];
    char operand2[10];
} Instruction;

int registers[MAX_REGISTERS] = {0};
Instruction instructions[MAX_LINES];
Label labels[MAX_LABELS];
int label_count = 0;
int instruction_count = 0;
int cmp_flag = 0;

void load_program(const char *filename);
void parse_instruction(char *line);
int find_label_address(const char *label);
void execute_program();

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <program_file>\n", argv[0]);
        return 1;
    }

    load_program(argv[1]);
    execute_program();

    return 0;
}

void load_program(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        exit(1);
    }

    char line[256];
    instruction_count = 0;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (strchr(line, ':')) {
            Instruction instr;
            strcpy(instr.instruction, line);
            instructions[instruction_count] = instr;
            char *label = strtok(line, ":");
            strcpy(labels[label_count].label, label);
            labels[label_count].address = instruction_count;
            label_count++;
            instruction_count++;
        } else {
            parse_instruction(line);
        }
    }

    fclose(file);
}

void parse_instruction(char *line) {
    char *token = strtok(line, " ");
    if (!token) return;

    strcpy(instructions[instruction_count].instruction, token);

    token = strtok(NULL, " ,");
    if (token) strcpy(instructions[instruction_count].operand1, token);

    token = strtok(NULL, " ,");
    if (token) strcpy(instructions[instruction_count].operand2, token);

    instruction_count++;
}

int find_label_address(const char *label) {
    for (int i = 0; i < label_count; i++) {
        if (strcmp(labels[i].label, label) == 0) {
            return labels[i].address;
        }
    }
    printf("Error: Undefined label %s\n", label);
    exit(1);
}

void execute_program() {
    int pc = 0;

    while (pc < instruction_count) {
        Instruction *instr = &instructions[pc];
        
        if (strchr(instr->instruction, ':') != NULL) {
            pc++;
            continue;
        } 
        
        int reg1 = instr->operand1[1] - '0';
        int reg2 = instr->operand2[1] - '0';
        if (strcmp(instr->instruction, "LOAD") == 0) {
            registers[reg1] = atoi(instr->operand2);
        } else if (strcmp(instr->instruction, "ADD") == 0) {
            registers[reg1] += registers[reg2];
        } else if (strcmp(instr->instruction, "SUB") == 0) {
            registers[reg1] -= registers[reg2];
        } else if (strcmp(instr->instruction, "MUL") == 0) {
            registers[reg1] *= registers[reg2];
        } else if (strcmp(instr->instruction, "DIV") == 0) {
            if (registers[reg2] == 0) {
                printf("Error: Division by zero\n");
                exit(1);
            }
            registers[reg1] /= registers[reg2];
        } else if (strcmp(instr->instruction, "MOV") == 0) {
            registers[reg1] = registers[reg2];
        } else if (strcmp(instr->instruction, "JMP") == 0) {
            pc = find_label_address(instr->operand1);
        } else if (strcmp(instr->instruction, "CMP") == 0) {
            if (registers[reg1] == registers[reg2]) {
                cmp_flag = 0;
            } else if (registers[reg1] > registers[reg2]) {
                cmp_flag = 1;
            } else {
                cmp_flag = -1;
            }
        } else if (strcmp(instr->instruction, "JNE") == 0) {
            if(cmp_flag != 0){
                pc = find_label_address(instr->operand1);
            }
        } else if (strcmp(instr->instruction, "JEQ") == 0) {
            if(cmp_flag == 0){
                pc = find_label_address(instr->operand1); 
            }
        } else if (strcmp(instr->instruction, "JGT") == 0) {
            if(cmp_flag == 1){
                pc = find_label_address(instr->operand1);
            }
        } else if (strcmp(instr->instruction, "JLT") == 0) {
            if(cmp_flag == -1){
                pc = find_label_address(instr->operand1);
            }
        } else if (strcmp(instr->instruction, "PRINT") == 0) {
            printf("Value in R%d: %d\n", reg1, registers[reg1]);
        } else {
            printf("Error: Unknown instruction %s\n", instr->instruction);
            exit(1);
        }

        pc++;
    }

    for (int i = 0; i < MAX_REGISTERS; i++) {
        printf("R%d = %d\n", i, registers[i]);
    }
}
