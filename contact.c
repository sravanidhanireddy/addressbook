#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"
#include "file.h"


// Sort contacts alphabetically by name
void sortContacts(AddressBook *addressBook)
{
    for (int i = 0; i < addressBook->contactCount - 1; i++)
    {
        for (int j = 0; j < addressBook->contactCount - 1 - i; j++)
        {
            if (strcmp(addressBook->contacts[j].name, addressBook->contacts[j + 1].name) > 0)
            {
                Contact temp = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j + 1];
                addressBook->contacts[j + 1] = temp;
            }
        }
    }
}

// Display all contacts
void listContacts(AddressBook *addressBook, int sortCriteria)
{
    if (addressBook->contactCount == 0) {
        printf("\n+-------------------------------------------------------------+\n");
        printf("|                  No contacts available                      |\n");
        printf("+-------------------------------------------------------------+\n");
        return;
    }

    printf("\n+--------------------------------------------------------------------+\n");
    printf("|%-3s | %-20s | %-15s | %-30s \n", 
           "S.No", "Name", "Phone", "Email");
    printf("+----------------------------------------------------------------------+\n");

    for (int i = 0; i < addressBook->contactCount; i++) {
        printf("| %-3d | %-20s | %-15s | %-30s \n",
               i + 1,
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }
    printf("+-----------------------------------------------------------------------------------------------+\n");
}


// Initialize address book and load data
void initialize(AddressBook *addressBook)
{
    addressBook->contactCount = 0;
    loadContactsFromFile(addressBook);
    sortContacts(addressBook);
    printf("\nAddress Book initialized.\n");
    printf("%d contact(s) loaded and sorted by name.\n", addressBook->contactCount);
}

// Save and exit program
void saveAndExit(AddressBook *addressBook)
{
    sortContacts(addressBook);
    saveContactsToFile(addressBook);
    printf("All contacts saved. Exiting program.\n");
    exit(EXIT_SUCCESS);
}

// Validation functions
int is_validname(char *name)
{
    if (name == NULL || strlen(name) == 0)
        return 0;
    for (int i = 0; name[i] != '\0'; i++)
    {
        if (!isalpha(name[i]) && name[i] != ' ')
        {
            return 0;
        }
    }
    return 1;
}

int is_validnumber(AddressBook *addressBook, char *number)
{
    int i = 0;
    while (number[i])
    {
        if (number[i] < '0' || number[i] > '9')
            return 0;
        i++;
    }
    return (i == 10);
}

int is_validemail(AddressBook *addressBook, char *mail)
{
    if (mail == NULL || strlen(mail) < 8)
        return 0;
    if (!isalpha(mail[0]))
        return 0;

    char *at = strchr(mail, '@');
    if (at == NULL || strchr(at + 1, '@') != NULL)
        return 0;

    char *dot_com = strstr(at, ".com");
    if (dot_com == NULL || strcmp(dot_com, ".com") != 0 || (dot_com - at) <= 1)
        return 0;

    int alpha_between = 0;
    for (char *p = at + 1; p < dot_com; p++)
    {
        if (isalpha(*p))
            alpha_between = 1;
        if (!isalnum(*p) && *p != '.')
            return 0;
    }
    if (!alpha_between)
        return 0;

    for (int i = 0; i < strlen(mail); i++)
    {
        if (isspace(mail[i])) return 0;
        if (!isalnum(mail[i]) && mail[i] != '.' && mail[i] != '@')
            return 0;
        if (mail[i] == '.' && (i == 0 || i == strlen(mail) - 1 || mail[i + 1] == '.'))
            return 0;
    }
    return 1;
}

int is_duplicate_name(AddressBook *addressBook, char *name)
{
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].name, name) == 0)
            return 1;
    }
    return 0;
}

int is_duplicate_number(AddressBook *addressBook, char *number)
{
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].phone, number) == 0)
            return 1;
    }
    return 0;
}

int is_duplicate_email(AddressBook *addressBook, char *mail)
{
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].email, mail) == 0)
            return 1;
    }
    return 0;
}

// Confirm save
int askToSaveContact()
{
    char choice;
    while (1)
    {
        printf("Do you want to save this contact? (y/n): ");
        scanf(" %c", &choice);
        while (getchar() != '\n');
        if (choice == 'y' || choice == 'Y')
            return 1;
        else if (choice == 'n' || choice == 'N')
            return 0;
        else
            printf("Invalid choice. Please enter y or n.\n");
    }
}

// Create new contact
void createContact(AddressBook *addressBook)
{
    if (addressBook->contactCount >= MAX_CONTACTS)
    {
        printf("\nThe address book is full. Cannot add more contacts.\n");
        return;
    }

    Contact newContact;
    int valid_input = 0;

    // Name
    while (!valid_input)
    {
        printf("\nEnter name: ");
        scanf(" %[^\n]", newContact.name);
        while (getchar() != '\n');

        if (is_validname(newContact.name))
        {
            if (!is_duplicate_name(addressBook, newContact.name))
                valid_input = 1;
            else
                printf("Name already exists. Please enter a unique name.\n");
        }
        else
        {
            printf("Invalid name. Only letters and spaces are allowed.\n");
        }
    }

    // Phone
    char number[20];
    valid_input = 0;
    while (!valid_input)
    {
        printf("Enter phone number (10 digits): ");
        scanf(" %19s", number);
        while (getchar() != '\n');

        if (is_validnumber(addressBook, number))
        {
            if (!is_duplicate_number(addressBook, number))
                valid_input = 1;
            else
                printf("Phone number already exists.\n");
        }
        else
        {
            printf("Invalid phone number. Must be exactly 10 digits.\n");
        }
    }
    strcpy(newContact.phone, number);

    // Email
    char mail[50];
    valid_input = 0;
    while (!valid_input)
    {
        printf("Enter email: ");
        scanf(" %[^\n]", mail);
        while (getchar() != '\n');

        if (is_validemail(addressBook, mail))
        {
            if (!is_duplicate_email(addressBook, mail))
                valid_input = 1;
            else
                printf("Email already exists.\n");
        }
        else
        {
            printf("Invalid email format. Example: name@example.com\n");
        }
    }
    strcpy(newContact.email, mail);


    printf("\nContact details entered successfully.\n");
    if (askToSaveContact())
    {
        addressBook->contacts[addressBook->contactCount++] = newContact;
        sortContacts(addressBook);
        saveContactsToFile(addressBook);
        printf("Contact saved to address book.\n");
    }
    else
    {
        printf("Contact not saved.\n");
    }
}

// Search contacts
void searchContact(AddressBook *addressBook)
{
    int choice;
    char searchTerm[50];
    int found = 0;

    printf("\nSearch by:\n");
    printf("1. Name\n2. Phone Number\n3. Email\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    while (getchar() != '\n');

    printf("Enter search term: ");
    scanf(" %[^\n]", searchTerm);
    while (getchar() != '\n');

    printf("\nSearch Results:\n");
    printf("--------------------------------------------------------------\n");

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if ((choice == 1 && strstr(addressBook->contacts[i].name, searchTerm) != NULL) ||
            (choice == 2 && strcmp(addressBook->contacts[i].phone, searchTerm) == 0) ||
            (choice == 3 && strcmp(addressBook->contacts[i].email, searchTerm) == 0))
        {
            printf("Name: %s\nPhone: %s\nEmail: %s\n\n",
                   addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,
                   addressBook->contacts[i].email);
            found = 1;
        }
    }

    if (!found)
        printf("No matching contact found.\n");
}

// Edit contact
void editContact(AddressBook *addressBook)
{
    char nameToEdit[50];
    int foundIndex = -1;
    int choice_field;
    int modified = 0;

    printf("Enter name of contact to edit: ");
    scanf(" %[^\n]", nameToEdit);
    while (getchar() != '\n');

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].name, nameToEdit) == 0)
        {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1)
    {
        printf("Contact not found.\n");
        return;
    }

    Contact original = addressBook->contacts[foundIndex];
    Contact updated = original;

    printf("\nContact found:\n");
    printf("Name: %s\nPhone: %s\nEmail: %s\n",
           original.name, original.phone, original.email);

    do
    {
        printf("\nEdit Menu:\n1. Name\n2. Phone\n3. Email\n4. Finish Editing\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice_field) != 1)
        {
            printf("Invalid input. Enter a number between 1 and 4.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (choice_field)
        {
        case 1: 
        {
            char newName[50];
            int valid = 0;
            while (!valid)
            {
                printf("Enter new name: ");
                scanf(" %[^\n]", newName);
                while (getchar() != '\n');

                if (!is_validname(newName))
                {
                    printf("Invalid name. Only letters and spaces allowed.\n");
                    continue;
                }
                if (strcmp(original.name, newName) == 0 || !is_duplicate_name(addressBook, newName))
                {
                    strcpy(updated.name, newName);
                    modified = 1;
                    valid = 1;
                }
                else
                {
                    printf("Name already exists.\n");
                }
            }
            break;
        }
        case 2: 
        {
            char newPhone[20];
            int valid = 0;
            while (!valid)
            {
                printf("Enter new phone number: ");
                scanf(" %19s", newPhone);
                while (getchar() != '\n');

                if (!is_validnumber(addressBook, newPhone))
                {
                    printf("Invalid phone number. Must be 10 digits.\n");
                    continue;
                }
                if (strcmp(original.phone, newPhone) == 0 || !is_duplicate_number(addressBook, newPhone))
                {
                    strcpy(updated.phone, newPhone);
                    modified = 1;
                    valid = 1;
                }
                else
                {
                    printf("Phone number already exists.\n");
                }
            }
            break;
        }
        case 3: 
        {
            char newEmail[50];
            int valid = 0;
            while (!valid)
            {
                printf("Enter new email: ");
                scanf(" %[^\n]", newEmail);
                while (getchar() != '\n');

                if (!is_validemail(addressBook, newEmail))
                {
                    printf("Invalid email format.\n");
                    continue;
                }
                if (strcmp(original.email, newEmail) == 0 || !is_duplicate_email(addressBook, newEmail))
                {
                    strcpy(updated.email, newEmail);
                    modified = 1;
                    valid = 1;
                }
                else
                {
                    printf("Email already exists.\n");
                }
            }
            break;
        }
        case 4:
            printf("Editing finished.\n");
            break;
        default:
            printf("Invalid choice. Select between 1 and 4.\n");
        }
    }
    while (choice_field != 4);

    if (modified)
    {
        addressBook->contacts[foundIndex] = updated;
        sortContacts(addressBook);
        if (askToSaveContact())
        {
            saveContactsToFile(addressBook);
            printf("Contact updated and saved.\n");
        }
        else
        {
            printf("Changes applied but not saved to file.\n");
        }
    }
    else
    {
        printf("No changes made to this contact.\n");
    }
}

// Delete contact
void deleteContact(AddressBook *addressBook)
{
    char nameToDelete[50];
    int foundIndex = -1;

    printf("Enter name of contact to delete: ");
    scanf(" %[^\n]", nameToDelete);
    while (getchar() != '\n');

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].name, nameToDelete) == 0)
        {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1)
    {
        for (int i = foundIndex; i < addressBook->contactCount - 1; i++)
        {
            addressBook->contacts[i] = addressBook->contacts[i + 1];
        }
        addressBook->contactCount--;
        sortContacts(addressBook);
        saveContactsToFile(addressBook);
        printf("Contact deleted successfully.\n");
    }
    else
    {
        printf("Contact not found.\n");
    }
}