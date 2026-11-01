#include <stdio.h>
#include "banker.h"


/*
 * Private helper function.
 *
 * Determines whether granting a request
 * would leave the system in a safe state.
 */
static int is_safe_state(
    Bank *bank,
    int customer_num,
    int request[]
)
{
    /*
     * TODO:
     *
     * Implement the Banker's
     * Safety Algorithm.
     */

    return 0;
}


/*
 * Initialize the bank.
 */
void initialize_bank(
    Bank *bank,
    int number_of_resources,
    int resources[]
)
{
    bank->number_of_customers = NUMBER_OF_CUSTOMERS;
    bank->number_of_resources = number_of_resources;

    /* Available starts as the command-line resource counts */
    for (int j = 0; j < number_of_resources; j++)
        bank->available[j] = resources[j];

    /* Nothing is demanded or allocated until customers are added */
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        for (int j = 0; j < number_of_resources; j++)
        {
            bank->maximum[i][j] = 0;
            bank->allocation[i][j] = 0;
            bank->need[i][j] = 0;
        }
    }
}


/*
 * Add a customer and record
 * the customer's maximum demand.
 */
void add_customer(
    Bank *bank,
    int customer_num,
    int max_demand[]
)
{
    if (customer_num < 0 || customer_num >= bank->number_of_customers)
        return;

    /* Allocation is zero, so Need = Maximum */
    for (int j = 0; j < bank->number_of_resources; j++)
    {
        bank->maximum[customer_num][j] = max_demand[j];
        bank->allocation[customer_num][j] = 0;
        bank->need[customer_num][j] = max_demand[j];
    }
}


/*
 * Request resources.
 *
 * Return:
 *
 *  0  = request approved
 * -1  = request denied
 */
int request_resources(
    Bank *bank,
    int customer_num,
    int request[]
)
{
    /*
     * TODO:
     *
     * 1. Validate the request.
     *
     * 2. Check whether the request
     *    exceeds the customer's need.
     *
     * 3. Check whether the requested
     *    resources are available.
     *
     * 4. Determine whether granting
     *    the request leaves the
     *    system in a safe state.
     *
     * 5. If approved, update the
     *    appropriate data structures.
     */

    return -1;
}


/*
 * Release resources held
 * by a customer.
 */
void release_resources(
    Bank *bank,
    int customer_num,
    int release[]
)
{
    /*
     * TODO:
     *
     * Validate and release the resources.
     */
}


/*
 * Display the current bank state:
 *
 * Available
 * Maximum
 * Allocation
 * Need
 */
/* Print one row of a matrix (or the available vector) as [a b c] */
static void print_row(int row[], int length)
{
    printf("[");
    for (int j = 0; j < length; j++)
        printf(j ? " %d" : "%d", row[j]);
    printf("]\n");
}


static void print_matrix(
    const char *name,
    int matrix[][MAX_RESOURCES],
    Bank *bank
)
{
    printf("\n%s:\n", name);
    for (int i = 0; i < bank->number_of_customers; i++)
    {
        printf("Customer %d: ", i);
        print_row(matrix[i], bank->number_of_resources);
    }
}


void get_state(
    Bank *bank
)
{
    printf("Available:\n");
    print_row(bank->available, bank->number_of_resources);

    print_matrix("Maximum", bank->maximum, bank);
    print_matrix("Allocation", bank->allocation, bank);
    print_matrix("Need", bank->need, bank);
}
