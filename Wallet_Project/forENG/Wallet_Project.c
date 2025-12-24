#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>

#define MAX_USERS 100
#define MAX_LEN 50
#define MAX_TRANSACTION 100

/*
    Algorithm (Pseudocode):
    A1: START
    A2: If user does not have an account, go to A3, otherwise go to A4
    A3: Create a new account with Name, Surname, Password
    A4: Login with Name, Surname, Password
    A5: Add incomes
    A6: Add expenses
    A7: Display current balance and transaction history
    A8: END
*/

// Console colors (ANSI Escape Codes)
void red() { printf("\033[1;31m"); }
void green() { printf("\033[1;32m"); }
void yellow() { printf("\033[1;33m"); }
void blue() { printf("\033[1;34m"); }
void purple() { printf("\033[0;35m"); }
void grey() { printf("\033[1;90m"); }
void orange() { printf("\033[38;5;180m"); }
void reset() { printf("\033[0m"); }

// GLOBAL user information
char names[MAX_USERS][MAX_LEN];
char surnames[MAX_USERS][MAX_LEN];
char passwords[MAX_USERS][MAX_LEN];
int userCount = 0;

// Income & expense data per user
float incomes[MAX_USERS][MAX_TRANSACTION];
float expenses[MAX_USERS][MAX_TRANSACTION];
int incomeCount[MAX_USERS];
int expenseCount[MAX_USERS];
float balance[MAX_USERS];

// Wrong login message
void wrongLogin()
{
    red();
    printf("\n Incorrect username or password!\n");
    reset();
}

// Clear input buffer
void clearBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Main menu
int mainMenu()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    orange();
    printf("\n ╔═════════════════════╗");
    printf("\n ║     Main Menu       ║");
    printf("\n ╚═════════════════════╝");
    printf("\n ╔═════════════════════╗");
    printf("\n ║ 1) Login            ║");
    printf("\n ║ 2) Create Account   ║");
    printf("\n ║ 0) Exit             ║");
    printf("\n ╚═════════════════════╝");
    printf("\n Your choice: ");

    int choice;
    if (scanf("%d", &choice) != 1)
    {
        clearBuffer();
        return -1;
    }

    clearBuffer();
    reset();
    return choice;
}

// Wallet menu
int walletMenu()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    orange();
    printf("\n ╔═══════════════════════════╗");
    printf("\n ║        Wallet Menu        ║");
    printf("\n ╚═══════════════════════════╝");
    printf("\n ╔═══════════════════════════╗");
    printf("\n ║ 1) Add Income             ║");
    printf("\n ║ 2) Add Expense            ║");
    printf("\n ║ 3) Show Balance           ║");
    printf("\n ║ 4) Transaction History    ║");
    printf("\n ║ 0) Logout                 ║");
    printf("\n ╚═══════════════════════════╝");
    printf("\n Your choice: ");

    int choice;
    if (scanf("%d", &choice) != 1)
    {
        clearBuffer();
        return -1;
    }

    clearBuffer();
    reset();
    return choice;
}

// Display Balance (by user index)
void showBalance(float balanceAmount)
{
    yellow();
    printf("\n ╔═══════════════════════════╗");
    printf("\n ║          Balance          ║");
    printf("\n ╚═══════════════════════════╝ ");
    printf("\n ╔═══════════════════════════╗");
    printf("\n ║                           ║");
    printf("\n          %.2f  $        ", balanceAmount);
    printf("\n ║                           ║");
    printf("\n ╚═══════════════════════════╝ ");
    reset();
}

// Transaction History (by user index)
void transactionHistory(int userIndex)
{
    purple();
    printf("\n ╔═══════════════════════════╗");
    printf("\n ║     Transaction History   ║");
    printf("\n ╚═══════════════════════════╝ ");

    printf("\n ╔═══════════════════════════╗");
    printf("\n ║                           ║");
    printf("\n ║  [Incomes]                ║\n");

    for (int i = 0; i < incomeCount[userIndex]; i++)
    {
        printf("\t + %d) %.2f $ \n", i + 1, incomes[userIndex][i]);
    }

    printf("\n ║                           ║");
    printf("\n ║  [Expenses]               ║\n");

    for (int i = 0; i < expenseCount[userIndex]; i++)
    {
        printf("\t - %d) %.2f $\n", i + 1, expenses[userIndex][i]);
    }

    printf("\n ║  [Balance]                ║");
    printf("\n \t    %.2f $        ", balance[userIndex]);
    printf("\n ║                           ║");
    printf("\n ╚═══════════════════════════╝");
    reset();
}

// Create Account
void createAccount()
{
    if (userCount >= MAX_USERS)
    {
        red();
        printf("\n Maximum number of users reached!");
        reset();
        return;
    }

    char name[MAX_LEN], surname[MAX_LEN], password[MAX_LEN];

    blue();
    printf("\n +----------------------+");
    printf("\n |    Create Account    |");
    printf("\n +----------------------+");
    printf("\n | Name: ");
    scanf("%s", name);
    printf("\n | Surname: ");
    scanf("%s", surname);
    printf("\n | Password: ");
    scanf("%s", password);
    printf("\n +----------------------+");
    reset();

    // Save new user into arrays
    strncpy(names[userCount], name, MAX_LEN - 1);
    names[userCount][MAX_LEN - 1] = '\0';

    strncpy(surnames[userCount], surname, MAX_LEN - 1);
    surnames[userCount][MAX_LEN - 1] = '\0';

    strncpy(passwords[userCount], password, MAX_LEN - 1);
    passwords[userCount][MAX_LEN - 1] = '\0';

    // Initial values
    incomeCount[userCount] = 0;
    expenseCount[userCount] = 0;
    balance[userCount] = 0.0f;

    userCount++;

    green();
    printf("\n Account successfully created! (User ID: %d)\n", userCount);
    reset();
}

// Login
int login()
{
    if (userCount == 0)
    {
        red();
        printf("\n You must create an account first.");
        reset();
        return -1;
    }

    char name[MAX_LEN], surname[MAX_LEN], password[MAX_LEN];

    blue();
    printf("\n +----------------------+");
    printf("\n |        Login         |");
    printf("\n +----------------------+");
    printf("\n | Name: ");
    scanf("%s", name);
    printf("\n | Surname: ");
    scanf("%s", surname);
    printf("\n | Password: ");
    scanf("%s", password);
    printf("\n +----------------------+");
    reset();

    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(name, names[i]) == 0 &&
            strcmp(surname, surnames[i]) == 0 &&
            strcmp(password, passwords[i]) == 0)
        {
            green();
            printf("\n Login successful! (User ID: %d)\n", i + 1);
            reset();
            return i; // active user index
        }
    }

    wrongLogin();
    return -1;
}

// Save data to file
void saveData()
{
    FILE *fp = fopen("data.txt", "w");

    if (fp == NULL)
    {
        red();
        printf("\nFILE COULD NOT BE CREATED!");
        reset();
        return;
    }

    for (int i = 0; i < userCount; i++)
    {
        fprintf(fp, "%s;%s;%s;%d;%d;",
                names[i], surnames[i], passwords[i],
                incomeCount[i], expenseCount[i]);

        // Write incomes
        for (int j = 0; j < incomeCount[i]; j++)
        {
            fprintf(fp, "%.2f", incomes[i][j]);
            if (j < incomeCount[i] - 1)
                fprintf(fp, ",");
        }
        fprintf(fp, ";");

        // Write expenses
        for (int j = 0; j < expenseCount[i]; j++)
        {
            fprintf(fp, "%.2f", expenses[i][j]);
            if (j < expenseCount[i] - 1)
                fprintf(fp, ",");
        }

        fprintf(fp, ";%.2f\n", balance[i]);
    }

    fclose(fp);
}

// Load data from file
void loadData()
{
    FILE *fp = fopen("data.txt", "r");
    if (!fp)
        return;

    char line[1024];

    while (fgets(line, sizeof(line), fp))
    {
        char *token; // holds parsed tokens

        token = strtok(line, ";");
        strcpy(names[userCount], token);

        token = strtok(NULL, ";");
        strcpy(surnames[userCount], token);

        token = strtok(NULL, ";");
        strcpy(passwords[userCount], token);

        token = strtok(NULL, ";");
        incomeCount[userCount] = atoi(token); // ASCII TO INTEGER

        token = strtok(NULL, ";");
        expenseCount[userCount] = atoi(token);

        // Incomes
        token = strtok(NULL, ";");
        char *inc = strtok(token, ",");
        for (int i = 0; i < incomeCount[userCount]; i++)
        {
            incomes[userCount][i] = atof(inc); // ASCII TO FLOAT
            inc = strtok(NULL, ",");
        }

        // Expenses
        token = strtok(NULL, ";");
        char *exp = strtok(token, ",");
        for (int i = 0; i < expenseCount[userCount]; i++)
        {
            expenses[userCount][i] = atof(exp);
            exp = strtok(NULL, ",");
        }

        token = strtok(NULL, ";");
        balance[userCount] = atof(token);

        userCount++;
    }

    fclose(fp);
}

// Main function
int main()
{
    loadData();

    while (1)
    {
        int choice = mainMenu();

        if (choice == 0)
        {
            grey();
            printf("Exiting program...");
            reset();
            return 0;
        }
        else if (choice == 2)
        {
            createAccount();
            saveData();
        }
        else if (choice == 1)
        {
            int activeUser = login();
            if (activeUser == -1)
            {
                saveData();
                continue;
            }

            // Wallet menu loop
            int walletChoice = 0;
            do
            {
                walletChoice = walletMenu();

                if (walletChoice == 1) // Add Income
                { 
                    if (incomeCount[activeUser] < MAX_TRANSACTION)
                    {
                        float amount;
                        printf("\n Income Amount: ");
                        scanf("%f", &amount);

                        incomes[activeUser][incomeCount[activeUser]++] = amount;
                        balance[activeUser] += amount;
                        saveData();

                        green();
                        printf("\n Transaction Successful!");
                        reset();
                    }
                    else
                    {
                        red();
                        printf("\n Maximum income limit reached!");
                        reset();
                    }
                }
                else if (walletChoice == 2) // Add Expense
                { 
                    if (expenseCount[activeUser] < MAX_TRANSACTION)
                    {
                        float amount;
                        printf("\n Expense Amount: ");
                        scanf("%f", &amount);

                        expenses[activeUser][expenseCount[activeUser]++] = amount;
                        balance[activeUser] -= amount;
                        saveData();

                        green();
                        printf("\n Transaction Successful!");
                        reset();
                    }
                    else
                    {
                        red();
                        printf("\n Maximum expense limit reached!");
                        reset();
                    }
                }
                else if (walletChoice == 3) // Show Balance
                { 
                    showBalance(balance[activeUser]);
                }
                else if (walletChoice == 4) // Transaction History
                { 
                    transactionHistory(activeUser);
                }
                else if (walletChoice == 0) // Logout
                { 
                    saveData();
                    grey();
                    printf("Logging out...");
                    reset();
                }
                else
                {
                    red();
                    printf("\n Please enter a valid option.");
                    reset();
                }
            } while (walletChoice != 0);
        }
        else
        {
            red();
            printf("\n Please enter a valid option.");
            reset();
        }
    }

    return 0;
}
