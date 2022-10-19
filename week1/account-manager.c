#include <stdio.h>

struct account
{
    char username[100];
    char password[100];
    int status;
};

int main(int argc, char const *argv[])
{
    int choice = 0;
    while (1)
    {
        printf("USER MANAGEMENT PROGRAM\n");
        printf("----------------------------------------------------------------\n");
        printf("1. Register \n");
        printf("2. Sign in \n");
        printf("3. Search\n");
        printf("4. Sign out\n");
        printf("Your choice (1-4, other to quit): ");
        //get choice
        scanf("%d", &choice);
        if (choice < 1||choice >4)
        {
            break;
        }
        else 
        {
            switch (choice)
            {
            case 1:
            {
                //register

                break;
            }
            case 2:
            {
                // sign in
                break;
            }
            case 3:
            {
                // search
                break;
            }
            case 4:
            {
                // sign out
                break;
            }
            default:
                break;
            }
        }

    }

    return 0;
}

