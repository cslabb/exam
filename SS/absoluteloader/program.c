#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main() {
    FILE *fp_in, *fp_out;
    char input_line[100], prog_name[10];
    char record_type;
    int start_addr, exec_addr, record_addr, record_len, i, j;

    fp_in = fopen("INPUT.txt", "r");
    fp_out = fopen("OUTPUT.txt", "w");

    if (fp_in == NULL || fp_out == NULL) {
        printf("Error opening files.\n");
        exit(1);
    }

    // Read the Header record
    fscanf(fp_in, "%c^%s^%x^%*x", &record_type, prog_name, &start_addr);
    printf("Loading program: %s\n", prog_name);

    // Read records until the End record
    fscanf(fp_in, " %c", &record_type); // Read next record type, skipping whitespace
    while (record_type != 'E') {
        if (record_type == 'T') {
            // Read the Text record
            fscanf(fp_in, "^%x^%x", &record_addr, &record_len);
            
            // Skip the '^' before the object code
            fgetc(fp_in); 

            printf("Loading text record at address %x, length %x\n", record_addr, record_len);

            // "Load" the object code byte by byte
            for (i = 0; i < record_len; i++) {
                char byte_str[3];
                // Read two characters (one byte)
                byte_str[0] = fgetc(fp_in);
                byte_str[1] = fgetc(fp_in);
                byte_str[2] = '\0';
                
                // Write to output file simulating memory
                fprintf(fp_out, "%06X\t%s\n", record_addr + i, byte_str);
            }
        }
        fscanf(fp_in, " %c", &record_type); // Read the next record type
    }

    // Read the execution start address from the End record
    fscanf(fp_in, "^%x", &exec_addr);
    printf("Execution starts at address: %06X\n", exec_addr);

    fclose(fp_in);
    fclose(fp_out);

    printf("Program loaded successfully into OUTPUT.txt\n");
}