#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main() {
    FILE *f_input, *f_namtab, *f_deftab, *f_argtab, *f_output;
    int len, i, pos = 1;
    char label[20], opcode[20], operand[20], macroname[20];
    char deftab_opcode[20], deftab_operand[20], arg[20];

    f_input = fopen("input.txt", "r");
    f_namtab = fopen("namtab.txt", "w+");
    f_deftab = fopen("deftab.txt", "w+");
    f_argtab = fopen("argtab.txt", "w+");
    f_output = fopen("output.txt", "w+");

    // Read the first line from input
    fscanf(f_input, "%s%s%s", label, opcode, operand);

    // Process lines until END is found
    while (strcmp(opcode, "END") != 0) {
        // --- Macro Definition ---
        if (strcmp(opcode, "MACRO") == 0) {
            fprintf(f_namtab, "%s\n", label); // Write macro name to NAMTAB
            fseek(f_namtab, SEEK_SET, 0); // Rewind NAMTAB
            fprintf(f_deftab, "%s\t%s\n", label, operand); // Write macro prototype to DEFTAB

            // Process macro body
            fscanf(f_input, "%s%s%s", label, opcode, operand);
            while (strcmp(opcode, "MEND") != 0) {
                if (operand[0] == '&') {
                    fprintf(f_deftab, "%s\t?%d\n", opcode, pos++);
                } else {
                    fprintf(f_deftab, "%s\t%s\n", opcode, operand);
                }
                fscanf(f_input, "%s%s%s", label, opcode, operand);
            }
            fprintf(f_deftab, "%s\n", opcode); // Write MEND to DEFTAB
        }
        // --- Macro Expansion ---
        else {
            fscanf(f_namtab, "%s", macroname);
            if (strcmp(opcode, macroname) == 0) {
                // It's a macro call, prepare ARGTAB
                len = strlen(operand);
                for (i = 0; i < len; i++) {
                    if (operand[i] != ',') {
                        fprintf(f_argtab, "%c", operand[i]);
                    } else {
                        fprintf(f_argtab, "\n");
                    }
                }
                fseek(f_deftab, SEEK_SET, 0); // Rewind DEFTAB
                fseek(f_argtab, SEEK_SET, 0); // Rewind ARGTAB

                // Read macro prototype from DEFTAB (and ignore it)
                fscanf(f_deftab, "%s%s", deftab_opcode, deftab_operand);

                // Read macro body from DEFTAB and expand
                fprintf(f_output, ".\t%s\t%s\n", opcode, operand); // Optional: Comment original macro call
                fscanf(f_deftab, "%s%s", deftab_opcode, deftab_operand);
                while (strcmp(deftab_opcode, "MEND") != 0) {
                    if (deftab_operand[0] == '?') {
                        fscanf(f_argtab, "%s", arg); // Get argument from ARGTAB
                        fprintf(f_output, "%s\t%s\t%s\n", label, deftab_opcode, arg);
                    } else {
                        fprintf(f_output, "%s\t%s\t%s\n", label, deftab_opcode, deftab_operand);
                    }
                    fscanf(f_deftab, "%s%s", deftab_opcode, deftab_operand);
                }
            } else {
                // Not a macro definition or call, just a regular line
                fprintf(f_output, "%s\t%s\t%s\n", label, opcode, operand);
            }
        }
        // Read the next line from input
        fscanf(f_input, "%s%s%s", label, opcode, operand);
    }
    // Write the final END line to output
    fprintf(f_output, "%s\t%s\t%s\n", label, opcode, operand);

    fclose(f_input);
    fclose(f_namtab);
    fclose(f_deftab);
    fclose(f_argtab);
    fclose(f_output);

    printf("Macro Pass 1 processing complete. Check output files.\n");
}