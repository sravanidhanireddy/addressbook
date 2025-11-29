#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"


void saveContactsToFile(AddressBook *addressBook)
{
    FILE *fptr = fopen("Contact.txt", "w");
    if (fptr == NULL)
    {
        printf("  Cannot open file: Contact.txt for writing.\n");
        return;
    }

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        fprintf(fptr, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fptr);
   
    printf("  All contacts saved to file: Contact.txt\n");
}

void loadContactsFromFile(AddressBook *addressBook)
{
    FILE *fptr = fopen("Contact.txt", "r");
    if (fptr == NULL)
    {
        printf("  No contact file found (Contact.txt).\n");
        printf("  Starting with an empty address book.\n");
        printf("---------------------------------------------------------\n");

        addressBook->contactCount = 0;
        return;
    }

    int i = 0;
    while (i < MAX_CONTACTS &&
           fscanf(fptr, "%49[^,],%19[^,],%49[^\n]\n",
                  addressBook->contacts[i].name,
                  addressBook->contacts[i].phone,
                  addressBook->contacts[i].email) == 3)
    {
        i++;
    }

    addressBook->contactCount = i;
    fclose(fptr);

    printf("  Loaded %d contact(s) from file: Contact.txt\n", addressBook->contactCount);
   
}