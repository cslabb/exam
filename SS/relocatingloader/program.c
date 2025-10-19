#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Global variables to hold the bitmask and its binary representation
char bitmask[12];
char bit[128]; // Increased size for safety

// Function to convert a hex bitmask to a binary string
void convert(char hex_mask[]) {
    int i;
    strcpy(bit, ""); // Clear the previous binary string
    for (i = 0; i < strlen(hex_mask); i++) {
        switch (hex_mask[i]) {
            case '0': strcat(bit, "0000"); break;
            case '1': strcat(bit, "0001"); break;
            case '2': strcat(bit, "0010"); break;
            case '3': strcat(bit, "0011"); break;
            case '4': strcat(bit, "0100"); break;
            case '5': strcat(bit, "0101"); break;
            case '6': strcat(bit, "0110"); break;
            case '7': strcat(bit, "0111"); break;
            case '8': strcat(bit, "1000"); break;
            case '9': strcat(bit, "1001"); break;
            case 'A': strcat(bit, "1010"); break;
            case 'B': strcat(bit, "1011"); break;
            case 'C': strcat(bit, "1100"); break;
            case 'D': strcat(bit, "1101"); break;
            case 'E': strcat(bit, "1110"); break;
            case 'F': strcat(bit, "1111"); break;
        }
    }
}

void main() {
    FILE *fp_in, *fp_out;
    char record_type;
    char prog_name[10], input_line[100];
    int start_addr, len, t_addr, t_len, opcode, addr, actual_addr;
    int i;

    // 1. Get the actual starting address from the user
    printf("Enter the actual starting address: ");
    scanf("%x", &start_addr);

    // 2. Open input and output files
    fp_in = fopen("RLIN.txt", "r");
    fp_out = fopen("RLOUT.txt", "w");
    if (fp_in == NULL || fp_out == NULL) {
        printf("Error opening files.\n");
        exit(1);
    }

    // 3. Write header to output file
    fprintf(fp_out, " ----------------------------\n");
    fprintf(fp_out, " ADDRESS\tCONTENT\n");
    fprintf(fp_out, " ----------------------------\n");

    // 4. Read the Header record (and ignore most of it)
    fscanf(fp_in, "%c^%s^%*x^%*x", &record_type, prog_name);

    // 5. Process records until the End record
    while ((record_type = fgetc(fp_in)) != 'E') {
        if (record_type == 'T') {
            // Read the Text record details
            fscanf(fp_in, "^%x^%x^%s", &t_addr, &t_len, bitmask);
            
            // Convert hex bitmask to binary
            convert(bitmask);

            // Relocate the base address for this text record
            t_addr += start_addr;

            // Process each 3-byte instruction in the record
            len = strlen(bit);
            for (i = 0; i < len; i++) {
                // Read the opcode and address part
                fscanf(fp_in, "%2x%4x", &opcode, &addr);

                // Relocation Logic
                if (bit[i] == '1') {
                    // Relocatable: add the starting address
                    actual_addr = addr + start_addr;
                } else {
                    // Absolute: use the address as is
                    actual_addr = addr;
                }
                
                // Write the relocated instruction to the output file
                fprintf(fp_out, "%06X\t%02X%04X\n", t_addr, opcode, actual_addr);
                t_addr += 3; // Move to the next instruction's address
            }
        }
    }

    // 6. Finalization
    fprintf(fp_out, " ----------------------------\n");
    fclose(fp_in);
    fclose(fp_out);
    printf("Successfully loaded program '%s'.\n", prog_name);
    printf("Check RLOUT.txt for the memory map.\n");
}