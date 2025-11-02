/* * Fixed SS/pass2/pass2.c
 * This is a significant rewrite as the original was non-functional.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORD_LEN 60 // Max bytes in one T record (30 instructions)

int main() {
    FILE *intermediate, *optab, *symtab, *lengthFile, *output;
    char label[20], opcode[20], operand[20], locctr_str[20];
    char symLabel[20], symAddr_str[20], opCode[20], opVal[20];
    char startAddr_str[20], progName[20], record[MAX_RECORD_LEN + 1];
    int length, startAddr, locctr, record_len = 0, record_start_addr = 0;

    intermediate = fopen("input/intermediate.txt", "r");
    optab = fopen("input/optab.txt", "r");
    symtab = fopen("input/symtab.txt", "r");
    lengthFile = fopen("input/length.txt", "r");
    output = fopen("output/objectcode.txt", "w");

    if (!intermediate || !optab || !symtab || !lengthFile || !output) {
        printf("Error opening files.\n");
        exit(1);
    }

    // Read program length (as hex)
    fscanf(lengthFile, "%x", &length);

    // Read first line (START line)
    fscanf(intermediate, "\t%s\t%s\t%s", progName, opcode, startAddr_str);
    startAddr = (int)strtol(startAddr_str, NULL, 16);
    locctr = startAddr;
    record_start_addr = startAddr;

    // Print Header record
    fprintf(output, "H^%-6s^%06s^%06X\n", progName, startAddr_str, length);

    // Read the first instruction line
    fscanf(intermediate, "%s\t%s\t%s\t%s", locctr_str, label, opcode, operand);
    locctr = (int)strtol(locctr_str, NULL, 16);
    record_start_addr = locctr;
    strcpy(record, ""); // Clear the record buffer

    // Process instructions
    while (strcmp(opcode, "END") != 0) {
        int found = 0;
        char obj_code_part[7]; // e.g., "001009" + null
        strcpy(obj_code_part, "");

        // --- BUG FIX: Search OPTAB ---
        rewind(optab);
        while (fscanf(optab, "%s\t%s", opCode, opVal) != EOF) {
            if (strcmp(opcode, opCode) == 0) {
                // --- BUG FIX: Search SYMTAB ---
                rewind(symtab);
                char operand_addr_str[5] = "0000"; // Default to 0000 if no operand
                if (strcmp(operand, "**") != 0) {
                    while (fscanf(symtab, "%s\t%s", symLabel, symAddr_str) != EOF) {
                        if (strcmp(operand, symLabel) == 0) {
                            strcpy(operand_addr_str, symAddr_str);
                            break;
                        }
                    }
                }
                sprintf(obj_code_part, "%s%s", opVal, operand_addr_str);
                found = 1;
                break;
            }
        }
        // --- END BUG FIX ---

        if (!found) {
            if (strcmp(opcode, "WORD") == 0) {
                sprintf(obj_code_part, "%06X", atoi(operand));
            } else if (strcmp(opcode, "BYTE") == 0) {
                // C'...'
                if (operand[0] == 'C' || operand[0] == 'c') { 
                    for (int i = 2; i < strlen(operand) - 1; i++) {
                        char hex_char[3];
                        sprintf(hex_char, "%X", operand[i]);
                        strcat(obj_code_part, hex_char);
                    }
                }
                // X'...'
                else if (operand[0] == 'X' || operand[0] == 'x') {
                    for (int i = 2; i < strlen(operand) - 1; i++) {
                        strncat(obj_code_part, &operand[i], 1);
                    }
                }
            }
        }

        int part_len = strlen(obj_code_part);
        
        // --- BUG FIX: T Record Management ---
        if (part_len > 0) {
            // Check if new part fits in the current record
            if (record_len + part_len > MAX_RECORD_LEN) {
                // Write out the old record
                fprintf(output, "T^%06X^%02X^%s\n", record_start_addr, record_len / 2, record);
                // Start a new record
                strcpy(record, obj_code_part);
                record_len = part_len;
                record_start_addr = locctr;
            } else {
                strcat(record, obj_code_part);
                record_len += part_len;
            }
        }

        // RESW/RESB: Write current record and reset
        if (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0) {
            if (record_len > 0) {
                fprintf(output, "T^%06X^%02X^%s\n", record_start_addr, record_len / 2, record);
            }
            strcpy(record, "");
            record_len = 0;
            // Set record start for the *next* piece of code
            int next_locctr = 0;
            fscanf(intermediate, "%s\t%s\t%s\t%s", locctr_str, label, opcode, operand);
            next_locctr = (int)strtol(locctr_str, NULL, 16);
            record_start_addr = next_locctr;
            locctr = next_locctr;
            continue; // Skip the normal fscanf at the end
        }
        // --- END BUG FIX ---

        fscanf(intermediate, "%s\t%s\t%s\t%s", locctr_str, label, opcode, operand);
        locctr = (int)strtol(locctr_str, NULL, 16);
    }

    // Write the last Text record
    if (record_len > 0) {
        fprintf(output, "T^%06X^%02X^%s\n", record_start_addr, record_len / 2, record);
    }

    // Write End record
    fprintf(output, "E^%06s\n", startAddr_str);

    fclose(intermediate);
    fclose(optab);
    fclose(symtab);
    fclose(lengthFile);
    fclose(output);

    printf("PASS 2 Completed Successfully.\n");
    return 0;
}
