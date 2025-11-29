#ifndef CONTACT_H
#define CONTACT_H
#include <stdio.h>
#include <stdlib.h>
#define MAX_CONTACTS 100
typedef struct Contact
{
    char name[50];
    char phone[20];
    char email[50];
} Contact;
typedef struct
{
    Contact contacts[MAX_CONTACTS];
    int contactCount;
} AddressBook;
void createContact(AddressBook *addressBook);
void searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook, int sortCriteria);
void initialize(AddressBook *addressBook);
void saveContactsToFile(AddressBook *addressBook);
void loadContactsFromFile(AddressBook *addressBook);
void saveAndExit(AddressBook *addressBook);
int is_validname(char *name);
int is_validnumber(AddressBook *addressBook, char *number);
int is_validemail(AddressBook *addressBook, char *mail);
int is_duplicate_name(AddressBook *addressBook, char *name);
int is_duplicate_number(AddressBook *addressBook, char *number);
int is_duplicate_email(AddressBook *addressBook, char *mail);
int askToSaveContact();
void sortContacts(AddressBook *addressBook);
#endif