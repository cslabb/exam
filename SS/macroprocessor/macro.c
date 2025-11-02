/*
 * Fixed SS/macroprocessor/macro.c
 * This is a major rewrite as the original logic was incorrect.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Simple struct to hold argument mappings
typedef struct {
    char formal[20];
    char positional[5];
} ArgMap;

// Function to find the positional argument for a formal argument
char* get_positional(char* formal, ArgMap arg_map[], int arg_count) {
    for (int i = 0; i < arg_count; i++) {
        if (strcmp(arg_map[i].formal, formal) == 0) {
            return arg_map[i].positional;
        }
    }
    return NULL;
}

int main() {
    FILE *f_input, *f_namtab, *f_deftab, *f_output;
    char label[20], opcode[20], operand[20];
    char macro_name[20];
    char line[100];

    f_input = fopen("input.txt", "r");
    f_namtab = fopen("namtab.txt", "w+");
    f_deftab = fopen("deftab.txt", "w+");
    f_output = fopen("output.txt", "w");
    // ARGTAB is now handled internally during expansion

    if (!f_input || !f_namtab || !f_deftab || !f_output) {
        printf("Error opening files.\n");
        exit(1);
    }

    // Read first line
    fscanf(f_input, "%s\t%s\t%s", label, opcode, operand);

    while (strcmp(opcode, "END") != 0) {
        if (strcmp(opcode, "MACRO") == 0) {
            // --- MACRO DEFINITION ---
            fprintf(f_namtab, "%s\n", label); // Write macro name to NAMTAB
            fprintf(f_deftab, "%s\t%s\n", label, operand); // Write prototype

            // --- BUG FIX: Correct positional arg mapping ---
            ArgMap local_arg_map[10];
            int arg_count = 0;
            char *token;
            char operand_copy[50];
            strcpy(operand_copy, operand);

            token = strtok(operand_copy, ",");
            while (token != NULL) {
                strcpy(local_arg_map[arg_count].formal, token);
                sprintf(local_arg_map[arg_count].positional, "?%d", arg_count + 1);
                arg_count++;
                token = strtok(NULL, ",");
            }
            // --- END BUG FIX ---

            // Process macro body
            fscanf(f_input, "%s\t%s\t%s", label, opcode, operand);
            while (strcmp(opcode, "MEND") != 0) {
                // --- BUG FIX: Substitute correct positional arg ---
                char* pos_arg = get_positional(operand, local_arg_map, arg_count);
                if (pos_arg != NULL) {
                    fprintf(f_deftab, "%s\t%s\n", opcode, pos_arg);
                } else {
                    fprintf(f_deftab, "%s\t%s\n", opcode, operand);
                }
                // --- END BUG FIX ---
                fscanf(f_input, "%s\t%s\t%s", label, opcode, operand);
            }
            fprintf(f_deftab, "%s\n", opcode); // Write MEND
        } else {
            // --- MACRO EXPANSION ---
            int is_macro_call = 0;
            rewind(f_namtab);
            while (fscanf(f_namtab, "%s", macro_name) != EOF) {
                if (strcmp(opcode, macro_name) == 0) {
                    is_macro_call = 1;
                    break;
                }
            }

            if (is_macro_call) {
                // --- BUG FIX: Correctly store args and expand ---
                char call_args[10][20];
                int call_arg_count = 0;
                char *token;
                char operand_copy[50];
                
                strcpy(operand_copy, operand);
                token = strtok(operand_copy, ",");
                while (token != NULL) {
                    strcpy(call_args[call_arg_count++], token);
                    token = strtok(NULL, ",");
                }

                // Find macro in DEFTAB and expand
                rewind(f_deftab);
                char def_line[100], def_opcode[20], def_operand[20];
                
                // Find the macro definition
                while (fgets(def_line, sizeof(def_line), f_deftab)) {
                    sscanf(def_line, "%s\t%s", def_opcode, def_operand);
                    if (strcmp(def_opcode, macro_name) == 0) {
                        break; // Found it, start expansion from next line
                    }
                }
                
                // Write comment for original call
                fprintf(f_output, ".\t%s\t%s\n", opcode, operand);

                // Expand the body
                while (fgets(def_line, sizeof(def_line), f_deftab)) {
                    sscanf(def_line, "%s\t%s", def_opcode, def_operand);
                    if (strcmp(def_opcode, "MEND") == 0) {
                        break; // End of this macro
                    }

                    if (def_operand[0] == '?') {
                        // It's a positional argument
                        int pos = atoi(&def_operand[1]); // Get '1' from '?1'
                        if (pos > 0 && pos <= call_arg_count) {
                            fprintf(f_output, "%s\t%s\t%s\n", label, def_opcode, call_args[pos - 1]);
                        }
                    } else {
                        // It's a regular instruction
                        fprintf(f_output, "%s\t%s\t%s\n", label, def_opcode, def_operand);
                    }
                }
                // --- END BUG FIX ---
            } else {
                // Not a macro call or definition, just write to output
                fprintf(f_output, "%s\t%s\t%s\n", label, opcode, operand);
            }
        }
        // Read next line from input
        fscanf(f_input, "%s\t%s\t%s", label, opcode, operand);
    }
    // Write the final END line
    fprintf(f_output, "%s\t%s\t%s\n", label, opcode, operand);

    fclose(f_input);
    fclose(f_namtab);
    fclose(f_deftab);
    fclose(f_output);

    printf("Macro Pass 1 processing complete. Check output files.\n");
    return 0;
}
