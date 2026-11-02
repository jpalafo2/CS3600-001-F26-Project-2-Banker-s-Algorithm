#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "banker.h"
#include <limits.h>

int main(int argc, char *argv[])
{
    /*
     * Usage:
     *
     * ./banker <input-file> <R0> <R1> ... <Rm-1>
     *
     * Example:
     *
     * ./banker inPutFile.txt 10 5 7 8
     */

    if (argc < 3)
    {
        printf(
            "Usage: %s <input-file> "
            "<R0> <R1> ... <Rm-1>\n",
            argv[0]
        );

        return 1;
    }


    /*
     * Create the Bank.
     */
    Bank bank;


    /*
     * Read the input filename and the
     * initial resource counts from argv[].
     */
    const char *filename = argv[1];
    int number_of_resources = argc - 2;

    if (number_of_resources > MAX_RESOURCES)
    {
        printf("Error: at most %d resource types supported\n", MAX_RESOURCES);
        return 1;
    }

    int resources[MAX_RESOURCES];

    for (int j = 0; j < number_of_resources; j++)
    {
        char *end;
        long value = strtol(argv[j + 2], &end, 10);

        if (*argv[j + 2] == '\0' || *end != '\0' || value < 0)
        {
            printf("Error: invalid resource value '%s'\n", argv[j + 2]);
            return 1;
        }

        resources[j] = (int) value;
    }

    initialize_bank(&bank, number_of_resources, resources);


    /*
     * Read each customer's maximum demand
     * (one comma-separated line per customer).
     */
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        perror(filename);
        return 1;
    }

    char line[256];

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        if (fgets(line, sizeof line, file) == NULL)
        {
            printf("Error: %s has fewer than %d customer lines\n",
                   filename, NUMBER_OF_CUSTOMERS);
            fclose(file);
            return 1;
        }

        int max_demand[MAX_RESOURCES];
        int count = 0;
        char *token = strtok(line, ",\r\n");

        while (token != NULL)
        {
            char *end;
            long value = strtol(token, &end, 10);

            if (count == number_of_resources || *end != '\0' || value < 0)
            {
                count = -1;
                break;
            }

            max_demand[count++] = (int) value;
            token = strtok(NULL, ",\r\n");
        }

        if (count != number_of_resources)
        {
            printf("Error: line %d of %s must have %d "
                   "non-negative integers\n",
                   i + 1, filename, number_of_resources);
            fclose(file);
            return 1;
        }

        add_customer(&bank, i, max_demand);
    }

    fclose(file);


        /*
     * Read user commands until the program ends.
     */
    char command[512];

    while (1)
    {
        printf("banker> ");
        fflush(stdout);

        // Read the user's input
        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            break;
        }

        // Remove the newline character
        command[strcspn(command, "\r\n")] = '\0';

        // Display the current bank state
        if (strcmp(command, "*") == 0)
        {
            get_state(&bank);
        }
        else if (strncmp(command, "RQ", 2) == 0 &&
         (command[2] == ' ' || command[2] == '\t' ||
          command[2] == '\0'))
        {
        // Read the customer number
        char *token = strtok(command, " \t");
        token = strtok(NULL, " \t");

        if (token == NULL)
        {
            printf("Invalid RQ format.\n");
            continue;
        }

        char *end;
        long customer = strtol(token, &end, 10);

        if (end == token || *end != '\0' ||
            customer < 0 || customer >= NUMBER_OF_CUSTOMERS)
        {
            printf("Invalid customer number.\n");
            continue;
        }

        // Read the requested amount for each resource
        int request[MAX_RESOURCES];
        int valid = 1;

        for (int j = 0; j < bank.number_of_resources; j++)
        {
            token = strtok(NULL, " \t");

            if (token == NULL)
            {
                valid = 0;
                break;
            }

            long value = strtol(token, &end, 10);

            if (end == token || *end != '\0' ||
                value < 0 || value > INT_MAX)
            {
                valid = 0;
                break;
            }

            request[j] = (int)value;
        }

        // Reject missing, extra, or invalid values
        if (!valid || strtok(NULL, " \t") != NULL)
        {
            printf("Invalid resource request.\n");
            continue;
        }

        // Ask the banker whether the request is safe
        if (request_resources(&bank, (int)customer, request) == 0)
        {
            printf("Approved\n");
        }
        else
        {
            printf("Denied\n");
        }
        }
        else if (command[0] != '\0')
        {
            printf("Unknown command: %s\n", command);
        }
    }

    return 0;
}
