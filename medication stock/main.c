#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// structs////////////////////////////////////////////////////////////////////////////////////////////////////

struct Date
{
    int y, m, d;
};

struct med
{
    char name[50], brand[50];
    int q, pr;
    struct Date manufacturing_date, expiry_date;
};

// to make the ceses cool////////////////////////////////////////////////////////////////////////////////////

void wait() {
    printf("Press Enter to continue...\n");
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
    getchar();
}

void slowprint(const char *message) {
    while (*message) {
        putchar(*message);
        fflush(stdout);
        usleep(10000);
        message++;
    }
}

// Time check/////////////////////////////////////////////////////////////////////////////////////////////////

int leapy(int year) {  // check if the year is leap
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int today(int cy, int cm, int cd) { // check the date if it is logical

    if (cy < 1900 || cy > 3000)
        return 1;


    if (cm < 1 || cm > 12)
        return 1;


    switch (cm) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if (cd < 1 || cd > 31)
                return 1;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            if (cd < 1 || cd > 30)
                return 1;
            break;
        case 2:
            if (leapy(cy)) {
                if (cd < 1 || cd > 29)
                    return 1;
            } else {
                if (cd < 1 || cd > 28)
                    return 1;
            }
            break;
        default:
            return 1;
    }

    return 0;
}

void av(struct Date *D) {

    do {
        // Prompt user for year, month, and day
        printf("Enter the year : ");
        scanf("%d", &D->y);
        printf("Enter the month : ");
        scanf("%d", &D->m);
        printf("Enter the day: ");
        scanf("%d", &D->d);


        // Provide user feedback if the date is invalid
        if (today(D->y, D->m, D->d) == 1) {
            printf("Invalid date. Please enter a valid date.\n");
        }

    } while (today(D->y, D->m, D->d) == 1);
}


//the array of the medications///////////////////////////////////////////////////////////////////////////////

void fillmed(struct med arr[], int i, int cy, int cm, int cd) {
    system("clear");
    printf("Enter details for medication %d:\n", i + 1);

    // Enter medication name
    int l;
    do {
        printf("Name: ");
        scanf("%s", arr[i].name);
        l = 0;
        for (int j = 0; j < i; j++) {
            if (strcmp(arr[i].name, arr[j].name) == 0) {
                l = 1;
                printf("Error: Medication with this name already exists. Please enter a different name.\n");
                break;
            }
        }
    } while (l == 1);

    // Enter brand name
    printf("Brand: ");
    scanf("%s", arr[i].brand);

    // Enter quantity and price
    do {
        printf("Quantity : ");
        scanf("%d", &arr[i].q);
        printf("Price : ");
        scanf("%d", &arr[i].pr);
        if (arr[i].q <= 0 || arr[i].pr <= 0) {
            printf("Error: Quantity and price must be greater than zero. Please try again.\n");
        }
    } while (arr[i].q <= 0 || arr[i].pr <= 0);

    // Enter manufacturing date and validate it
    do {
        printf("Enter manufacturing date :\n");
        av(&arr[i].manufacturing_date);
        if (today(arr[i].manufacturing_date.y, arr[i].manufacturing_date.m, arr[i].manufacturing_date.d) == 1 ||
            (arr[i].manufacturing_date.y > cy) ||
            (arr[i].manufacturing_date.y == cy && arr[i].manufacturing_date.m > cm) ||
            (arr[i].manufacturing_date.y == cy && arr[i].manufacturing_date.m == cm && arr[i].manufacturing_date.d > cd)) {
            printf("Error: Invalid manufacturing date. Please enter a valid date.\n");
        }
    } while (today(arr[i].manufacturing_date.y, arr[i].manufacturing_date.m, arr[i].manufacturing_date.d) == 1 ||
             (arr[i].manufacturing_date.y > cy) ||
             (arr[i].manufacturing_date.y == cy && arr[i].manufacturing_date.m > cm) ||
             (arr[i].manufacturing_date.y == cy && arr[i].manufacturing_date.m == cm && arr[i].manufacturing_date.d > cd));

    // Enter expiry date and validate it
    do {
        printf("Enter expiry date :\n");
        av(&arr[i].expiry_date);
        if (arr[i].expiry_date.y < arr[i].manufacturing_date.y ||
            (arr[i].expiry_date.y == arr[i].manufacturing_date.y && arr[i].expiry_date.m < arr[i].manufacturing_date.m) ||
            (arr[i].expiry_date.y == arr[i].manufacturing_date.y && arr[i].expiry_date.m == arr[i].manufacturing_date.m && arr[i].expiry_date.d < arr[i].manufacturing_date.d)) {
            printf("Error: Invalid expiry date. Expiry date cannot be earlier than the manufacturing date. Please enter a valid date.\n");
        }
    } while (arr[i].expiry_date.y < arr[i].manufacturing_date.y ||
             (arr[i].expiry_date.y == arr[i].manufacturing_date.y && arr[i].expiry_date.m < arr[i].manufacturing_date.m) ||
             (arr[i].expiry_date.y == arr[i].manufacturing_date.y && arr[i].expiry_date.m == arr[i].manufacturing_date.m && arr[i].expiry_date.d < arr[i].manufacturing_date.d));
}


void fill(struct med arr[], int N, int cy, int cm, int cd) {

    for (int i = 0; i < N; i++) {
    fillmed(arr, i, cy, cm, cd);
    system("clear");
    }
}

//Expired fonctions//////////////////////////////////////////////////////////////////////////////////////////////
void expired(struct med arr[], int N, int cy, int cm, int cd) {
    int i, j;
    j=0;
    for( i = 0; i < N; i++)
    if (arr[i].expiry_date.y < cy ||
        (arr[i].expiry_date.y == cy && arr[i].expiry_date.m < cm) ||
        (arr[i].expiry_date.y == cy && arr[i].expiry_date.m == cm && arr[i].expiry_date.d < cd)) {
         printf("The medication %s is Expired\n", arr[i].name);
         j++;
    }
    if ((arr[i].expiry_date.y == cy && arr[i].expiry_date.m == cm + 1) ||
        (arr[i].expiry_date.y == cy + 1 && cm == 12 && arr[i].expiry_date.m == 1)) {
        printf("The medication %s will expire soon\n", arr[i].name);
        j++;
    }
    if(j == 0)
    {
        slowprint("There is no Expired medications or that will expired soon.\n");
    }

}

// Function to delete expired medications from the array
void delexpire(struct med *arr, int *N, int cy, int cm, int cd) {
int E;
    for (int i = 0; i < *N; i++) {
        // Check if the current medication is expired
        if (arr[i].expiry_date.y < cy ||
            (arr[i].expiry_date.y == cy && arr[i].expiry_date.m < cm) ||
            (arr[i].expiry_date.y == cy && arr[i].expiry_date.m == cm && arr[i].expiry_date.d < cd)) {

            // Shift all medications one position back from index i to remove the expired medication
            for (int j = i; j < *N - 1; j++) {
                arr[j] = arr[j + 1];
            }

            // Decrement the count of medications
            (*N)--;

            // Print a message indicating successful deletion
            printf("Medication '%s' deleted successfully.\n", arr[i].name);

            // Decrement i to continue checking the same index after removal
            i--;
            E++;
        }
    }
    if(E == 0)
    {
        slowprint("there is no expired medications.\n");
    }
}




// Affichage of the array//////////////////////////////////////////////////////////////////////////////

void getmed(struct med arr[], int i) {
    char buffer[256]; // A buffer to hold the formatted message

    // Format and print each part of the medication information
    sprintf(buffer, "Medication %d:\n", i + 1);
    slowprint(buffer);

    sprintf(buffer, "Name: %s\n", arr[i].name);
    slowprint(buffer);

    sprintf(buffer, "Brand: %s\n", arr[i].brand);
    slowprint(buffer);

    sprintf(buffer, "Quantity: %d\n", arr[i].q);
    slowprint(buffer);

    sprintf(buffer, "Price: %d\n", arr[i].pr);
    slowprint(buffer);

    sprintf(buffer, "Manufacturing Date: %d/%d/%d\n", arr[i].manufacturing_date.d, arr[i].manufacturing_date.m, arr[i].manufacturing_date.y);
    slowprint(buffer);

    sprintf(buffer, "Expiry Date: %d/%d/%d\n", arr[i].expiry_date.d, arr[i].expiry_date.m, arr[i].expiry_date.y);
    slowprint(buffer);

    slowprint("\n");
}


void show(struct med arr[], int N) // show the information of the med
{
    printf("\nEntered medications:\n");
    for (int i = 0; i < N; i++)
    {
        getmed(arr, i);
    }
}

void search(struct med arr[], int N) {
    int count = 0, choice, in = -1;
    char c;
    printf("Enter the first character of the medication name: ");
    scanf(" %c", &c);

    // Count medications starting with the first letter
    for (int i = 0; i < N; i++) {
        if (arr[i].name[0] == c) {
            count++;
        }
    }

    // If there is more than one medication
    if (count > 1) {
        printf("Multiple medications found. Choose one of the following:\n");
        int currentCount = 0;
        for (int i = 0; i < N; i++) {
            if (arr[i].name[0] == c) {
                currentCount++;
                printf("%d. Name: %s\n", currentCount, arr[i].name);
            }
        }
        printf("Enter your choice (1-%d): ", count);
        scanf("%d", &choice);

        // Validate choice and get the corresponding medication
        if (choice > 0 && choice <= count) {
            currentCount = 0;
            for (int i = 0; i < N; i++) {
                if (arr[i].name[0] == c) {
                    currentCount++;
                    if (currentCount == choice) {
                        in = i;
                        break;
                    }
                }
            }
            if (in != -1) {
                getmed(arr, in);
            }
        } else {
            printf("Invalid choice.\n");
        }
    }
    // If there is only one medication starting with the given letter
    else if (count == 1) {
        for (int i = 0; i < N; i++) {
            if (arr[i].name[0] == c) {
                printf("The information of the medication you searched for is:\n");
                getmed(arr, i);
                return;
            }
        }
    }
    // If no medication is found
    else {
        printf("No medication found with the starting letter '%c'.\n", c);
    }
}

// modify the array///////////////////////////////////////////////////////////////////////////////////////

void updateq(struct med arr[], int N) // update new qantity
{
    char name[50];
    int newq;
    int nf;

    printf("Enter the name of the medication you want to update: ");
    scanf("%s", name);

    for(int i = 0; i < N; i++) {
        if(strcmp(arr[i].name, name) == 0) {
            printf("Current quantity of %s: %d\n", arr[i].name, arr[i].q);
            printf("Enter the new quantity: ");
            scanf("%d", &newq);
            arr[i].q = newq;
            printf("Quantity updated successfully.\n");
            nf = 0;
        }
    }
    if(nf != 0){
        printf("Medication not found.\n");
    }

}

void delmed(struct med arr[], int *N) {
    char name[50];
    int found = 0;

    printf("Enter the name of the medication you want to delete: ");
    scanf("%s", name);

    for (int i = 0; i < *N; i++) {
        if (strcmp(arr[i].name, name) == 0) {
            found = 1;
            for (int j = i; j < *N - 1; j++) {
                arr[j] = arr[j + 1];
            }
            memset(&arr[*N - 1], 0, sizeof(struct med));
            (*N)--;
            printf("Medication '%s' deleted successfully.\n", name);
            break;
        }
    }

    if (!found) {
        printf("Medication '%s' not found.\n", name);
    }
}

void addmed(struct med **arr, int *N, int cy, int cm, int cd) {
    // Increment the number of arr
    int new_size = *N + 1;
    printf("%d", sizeof(struct med));
    struct med *new_medications = realloc(*arr, new_size * sizeof(struct med));
    printf("%d", sizeof(struct med));
    if (new_medications == NULL) {
        printf("Failed to allocate memory for new medication.\n");
        return;
    }

    *arr = new_medications;
    *N = new_size;

    fillmed(*arr, *N - 1, cy, cm, cd);
}



// the FILE of the medications////////////////////////////////////////////////////////////////////////////////////////

void  print_frame(FILE *fichier, int width) {
        fprintf(fichier, "+");
        for (int i = 0; i < width - 2; ++i) {
            fprintf(fichier, "-");
        }
        fprintf(fichier, "+\n");
}

void fgetmed(FILE *fichier, struct med *arr, int i)
{
    fprintf(fichier, "Medication %d:\n", i + 1);
    fprintf(fichier, "Name: %s\n", arr[i].name);
    fprintf(fichier, "Brand: %s\n", arr[i].brand);
    fprintf(fichier, "Quantity: %d\n", arr[i].q);
    fprintf(fichier, "Price: %d\n", arr[i].pr);
    fprintf(fichier, "Manufacturing Date: %d/%d/%d\n", arr[i].manufacturing_date.d, arr[i].manufacturing_date.m, arr[i].manufacturing_date.y);
    fprintf(fichier, "Expiry Date: %d/%d/%d\n", arr[i].expiry_date.d, arr[i].expiry_date.m, arr[i].expiry_date.y);
    fprintf(fichier, "\n");
}


void medfile(struct med arr[], int N)
{
    FILE *fichier;
    fichier = fopen("medications.txt", "w");
    if (fichier == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for(int i = 0; i < N; i++)
    {
        print_frame(fichier, 45);
        fgetmed(fichier, arr, i);
    }
    fclose(fichier);
}

int main()
{
    int N, choice;
    int cy, cm, cd;
      do {
        printf("Enter the current year : ");
        scanf("%d", &cy);
        printf("Enter the current month : ");
        scanf("%d", &cm);
        printf("Enter the current day: ");
        scanf("%d", &cd);

        if (today(cy, cm, cd) == 1) {
            printf("Invalid date. Please enter a valid date.\n");
        }

    } while (today(cy, cm, cd) == 1);


    slowprint("Enter the number of medications: ");
    scanf("%d", &N);

    struct med *medications = malloc(N * sizeof(struct med));


    fill(medications, N, cy, cm, cd);
    do
    {
    system("clear");
    slowprint("Enter you choice (0-8) :");
    slowprint("\n0. End the program.");
    slowprint("\n1. Add a medication.");
    slowprint("\n2. Show all medications.");
    slowprint("\n3. Search a medication by its first letter.");
    slowprint("\n4. Delete a medication.");
    slowprint("\n5. Update a new quantity to a medication.");
    slowprint("\n6. Show the expired medications or that will expire soon.");
    slowprint("\n7. Delete the expired medications");
    slowprint("\n8. Record the medications in a file\n");

    scanf("%d", &choice);
    switch(choice)
    {
        case 0:
           break;
        case 1:
           addmed(&medications, &N, cy, cm, cd);
           wait();
           break;
        case 2:
           show(medications, N);
           wait();
           break;
        case 3:
           search(medications, N);
           wait();
           break;
        case 4:
           delmed(medications, &N);
           wait();
           break;
        case 5:
           updateq(medications, N);
           wait();
           break;
        case 6:
           expired(medications, N, cy, cm, cd);
           wait();
           break;
        case 7:
            delexpire(medications, &N, cy, cm, cd);
            break;
        case 8:
            medfile(medications, N);
           break;
        default :
           printf("The choice should be in the range (0-8)\n");
           wait();
           break;
    }

    }while(choice != 0);
    system("clear");
    slowprint("Thank you for your trust in our program.");
    sleep(1);
    slowprint("\nSee you Next Time.");

    free(medications);
    return 0;
}
