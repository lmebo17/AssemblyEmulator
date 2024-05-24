#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_REGISTERS 10
#define MAX_LINES 1000
#define MAX_LABELS 100
#define MEMORY_SIZE 1000

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
int memory[MEMORY_SIZE] = {0};

void load_program(const char *filename);
void parse_instruction(char *line);
int find_label_address(const char *label);
void execute_program();
int is_label(const char *instruction);
void switch_instruction(Instruction *instr, int *pc, int reg1, int reg2);
void load_instruction(int reg, const char *operand2);
void add_instruction(int reg1, int reg2);
void sub_instruction(int reg1, int reg2);
void mul_instruction(int reg1, int reg2);
void div_instruction(int reg1, int reg2);
void mov_instruction(int reg1, int reg2);
void store_instruction(int reg, int addr);
void load_from_memory_instruction(int reg, int addr);
void jmp_instruction(const char *operand1, int *pc);
void cmp_instruction(int reg1, int reg2);
void jne_instruction(const char *operand1, int *pc);
void jeq_instruction(const char *operand1, int *pc);
void jgt_instruction(const char *operand1, int *pc);
void jlt_instruction(const char *operand1, int *pc);
void print_instruction(int reg);
void print_registers();
void and_instruction(int dest, int src1, int src2);
void or_instruction(int dest, int src1, int src2);
void xor_instruction(int dest, int src);
void not_instruction(int dest, int src);

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
        
        if (is_label(instr->instruction)) {
            pc++;
            continue;
        } 
        
        int reg1 = instr->operand1[1] - '0';
        int reg2 = instr->operand2[1] - '0';

        switch_instruction(instr, &pc, reg1, reg2);

        pc++;
    }
    print_registers();
}

int is_label(const char *instruction) {
    return strchr(instruction, ':') != NULL;
}

void switch_instruction(Instruction *instr, int *pc, int reg1, int reg2) {
    if (strcmp(instr->instruction, "LOAD") == 0) {
        load_instruction(reg1, instr->operand2);
    } else if (strcmp(instr->instruction, "STORE") == 0) {
        store_instruction(reg1, atoi(instr->operand2));
    } else if (strcmp(instr->instruction, "LOADM") == 0) {
        load_from_memory_instruction(reg1, atoi(instr->operand2));
    } else if (strcmp(instr->instruction, "ADD") == 0) {
        add_instruction(reg1, reg2);
    } else if (strcmp(instr->instruction, "SUB") == 0) {
        sub_instruction(reg1, reg2);
    } else if (strcmp(instr->instruction, "MUL") == 0) {
        mul_instruction(reg1, reg2);
    } else if (strcmp(instr->instruction, "DIV") == 0) {
        div_instruction(reg1, reg2);
    } else if (strcmp(instr->instruction, "MOV") == 0) {
        mov_instruction(reg1, reg2);
    } else if (strcmp(instr->instruction, "JMP") == 0) {
        jmp_instruction(instr->operand1, pc);
    } else if (strcmp(instr->instruction, "CMP") == 0) {
        cmp_instruction(reg1, reg2);
    } else if (strcmp(instr->instruction, "JNE") == 0) {
        jne_instruction(instr->operand1, pc);
    } else if (strcmp(instr->instruction, "JEQ") == 0) {
        jeq_instruction(instr->operand1, pc);
    } else if (strcmp(instr->instruction, "JGT") == 0) {
        jgt_instruction(instr->operand1, pc);
    } else if (strcmp(instr->instruction, "JLT") == 0) {
        jlt_instruction(instr->operand1, pc);
    } else if (strcmp(instr->instruction, "PRINT") == 0) {
        print_instruction(reg1);
    } else if (strcmp(instr->instruction, "AND") == 0) {
        and_instruction(reg1, reg1, reg2);
    } else if (strcmp(instr->instruction, "OR") == 0) {
        or_instruction(reg1, reg1, reg2);
    } else if (strcmp(instr->instruction, "XOR") == 0) {
        xor_instruction(reg1, reg2);
    } else if (strcmp(instr->instruction, "NOT") == 0) {
        not_instruction(reg1, reg1);
    } else {
        printf("Error: Unknown instruction %s\n", instr->instruction);
        exit(1);
    }
}

void load_instruction(int reg, const char *operand){
    registers[reg] = atoi(operand);
}

void add_instruction(int reg1, int reg2) {
    registers[reg1] += registers[reg2];
}

void sub_instruction(int reg1, int reg2) {
    registers[reg1] -= registers[reg2];
}

void mul_instruction(int reg1, int reg2) {
    registers[reg1] *= registers[reg2];
}

void div_instruction(int reg1, int reg2) {
    if (registers[reg2] == 0) {
        printf("Error: Division by zero\n");
        exit(1);
    }
    registers[reg1] /= registers[reg2];
}

void mov_instruction(int reg1, int reg2) {
    registers[reg1] = registers[reg2];
}

void store_instruction(int reg, int addr) {
    if (addr < 0 || addr >= MEMORY_SIZE) {
        printf("Error: Memory address out of bounds %d\n", addr);
        exit(1);
    }
    memory[addr] = registers[reg];
}

void load_from_memory_instruction(int reg, int addr) {
    if (addr < 0 || addr >= MEMORY_SIZE) {
        printf("Error: Memory address out of bounds %d\n", addr);
        exit(1);
    }
    registers[reg] = memory[addr];
}

void jmp_instruction(const char *operand1, int *pc) {
    *pc = find_label_address(operand1);
}

void cmp_instruction(int reg1, int reg2) {
    if (registers[reg1] == registers[reg2]) {
        cmp_flag = 0;
    } else if (registers[reg1] > registers[reg2]) {
        cmp_flag = 1;
    } else {
        cmp_flag = -1;
    }
}

void jne_instruction(const char *operand1, int *pc) {
    if (cmp_flag != 0) {
        *pc = find_label_address(operand1);
    }
}

void jeq_instruction(const char *operand1, int *pc) {
    if (cmp_flag == 0) {
        *pc = find_label_address(operand1);
    }
}

void jgt_instruction(const char *operand1, int *pc) {
    if (cmp_flag == 1) {
        *pc = find_label_address(operand1);
    }
}

void jlt_instruction(const char *operand1, int *pc) {
    if (cmp_flag == -1) {
        *pc = find_label_address(operand1);
    }
}

void print_instruction(int reg) {
    printf("Value in R%d: %d\n", reg, registers[reg]);
}

void print_registers() {
    for (int i = 0; i < MAX_REGISTERS; i++) {
        printf("R%d = %d\n", i, registers[i]);
    }
}

void and_instruction(int dest, int src1, int src2) {
    registers[dest] = registers[src1] & registers[src2];
}

void or_instruction(int dest, int src1, int src2) {
    registers[dest] = registers[src1] | registers[src2];
}

void xor_instruction(int dest, int src) {
    registers[dest] = registers[dest] ^ registers[src];
}

void not_instruction(int dest, int src) {
    registers[dest] = ~registers[src];
}
