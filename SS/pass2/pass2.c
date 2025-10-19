#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *intermediate, *optab, *symtab, *lengthFile, *output;
    char label[20], opcode[20], operand[20];
    char symLabel[20], symAddr[20], opCode[20], opVal[20];
    char startAddr[20], progName[20];
    int length;

    // Open files
    intermediate = fopen("input/intermediate.txt", "r");
    optab = fopen("input/optab.txt", "r");
    symtab = fopen("input/symtab.txt", "r");
    lengthFile = fopen("input/length.txt", "r");
    output = fopen("output/objectcode.txt", "w");

    if (!intermediate || !optab || !symtab || !lengthFile || !output) {
        printf("Error opening files.\n");
        exit(1);
    }

    fscanf(lengthFile, "%d", &length);

    // Read first line (START line)
    fscanf(intermediate, "%s\t%s\t%s", label, opcode, operand);
    strcpy(progName, label);
    strcpy(startAddr, operand);

    fprintf(output, "H^%s^%s^%d\n", progName, startAddr, length);
    printf("H^%s^%s^%d\n", progName, startAddr, length);

    // Read next line after START
    fscanf(intermediate, "%s\t%s\t%s\t%s", label, opcode, operand, operand);

    fprintf(output, "T^00%s^", startAddr);
    printf("T^00%s^", startAddr);

    // Process instructions
    while (strcmp(opcode, "END") != 0) {
        rewind(optab);
        int found = 0;

        // Check if opcode exists in OPTAB
        while (fscanf(optab, "%s\t%s", opCode, opVal) != EOF) {
            if (strcmp(opcode, opCode) == 0) {
                rewind(symtab);
                while (fscanf(symtab, "%s\t%s", symLabel, symAddr) != EOF) {
                    if (strcmp(operand, symLabel) == 0) {
                        fprintf(output, "%s%s^", opVal, symAddr);
                        printf("%s%s^", opVal, symAddr);
                        break;
                    }
                }
                found = 1;
                break;
            }
        }

        // Handle WORD and BYTE
        if (!found) {
            if (strcmp(opcode, "WORD") == 0) {
                fprintf(output, "0000%s^", operand);
                printf("0000%s^", operand);
            } else if (strcmp(opcode, "BYTE") == 0) {
                for (int i = 2; i < strlen(operand) - 1; i++) {
                    fprintf(output, "%X", operand[i]);
                    printf("%X", operand[i]);
                }
                fprintf(output, "^");
                printf("^");
            }
        }

        fscanf(intermediate, "%s\t%s\t%s\t%s", label, opcode, operand, operand);
    }

    fprintf(output, "\nE^00%s", startAddr);
    printf("\nE^00%s\n", startAddr);

    fclose(intermediate);
    fclose(optab);
    fclose(symtab);
    fclose(lengthFile);
    fclose(output);

    printf("PASS 2 Completed Successfully.\n");

    return 0;
}
