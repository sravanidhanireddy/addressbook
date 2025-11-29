#include <stdio.h>
#include "contact.h"


int main()
{
    int choice;
    AddressBook addressBook;
    initialize(&addressBook);

    do
    {
        printf("-------- ADDRESS BOOK MAIN MENU --------\n");
        printf("|  1. Create Contact                  |\n");
        printf("|  2. Search Contact                  |\n");
        printf("|  3. Edit Contact                    |\n");
        printf("|  4. Delete Contact                  |\n");
        printf("|  5. List All Contacts               |\n");
        printf("|  6. Save and Exit                   |\n");
        printf("========================================\n");
        printf("Enter your choice (1-6): ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
                createContact(&addressBook);
                break;

        case 2:
                searchContact(&addressBook);
                break;

        case 3:
                editContact(&addressBook);

                break;

        case 4:
                deleteContact(&addressBook);
                break;

        case 5:
                listContacts(&addressBook, 0);
                break;

        case 6:
                printf("       Saving data and exiting...       \n");
                saveAndExit(&addressBook);
                break;

        default:
                printf("       Invalid choice. Try again.       \n");
        }


    } while (choice != 6);

    return 0;
}