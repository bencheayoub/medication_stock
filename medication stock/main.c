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

// Time check////////////////////////////////////////////////////////////////////////////////////////

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

void av(struct Date *D) // complet the date of med
{
    do
    {
       printf("enter the year : ");
       scanf(" %d", &D->y);
       printf("enter the month : ");
       scanf(" %d", &D->m);
       printf("enter the day : ");
       scanf(" %d", &D->d);

    }while(today(D->y, D->m, D->d) == 1);

}

//the array of the medications///////////////////////////////////////////////////////////////////////////////

void fillmed(struct med arr[], int i,int cy,int cm,int cd){
    system("clear");
    printf("Enter details for medication %d:\n", i + 1);

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

        printf("Brand: ");
        scanf("%s", arr[i].brand);

        do {
            printf("Quantity: ");
            scanf("%d", &arr[i].q);
            printf("Price: ");
            scanf("%d", &arr[i].pr);
        } while (arr[i].q <= 0 || arr[i].pr <= 0);

        do {
            printf("Enter manufacturing date:\n");
            av(&arr[i].manufacturing_date);
        } while (today(arr[i].manufacturing_date.y, arr[i].manufacturing_date.m, arr[i].manufacturing_date.d) == 1 ||
                 (arr[i].manufacturing_date.y > cy) ||
                 (arr[i].manufacturing_date.y == cy && arr[i].manufacturing_date.m > cm) ||
                 (arr[i].manufacturing_date.y == cy && arr[i].manufacturing_date.m == cm && arr[i].manufacturing_date.d > cd));

        do {
            printf("Enter expiry date:\n");
            av(&arr[i].expiry_date);
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

int expired(struct med arr[], int N, int cy, int cm, int cd, int index) {
    int expiry_year = arr[index].expiry_date.y;
    int expiry_month = arr[index].expiry_date.m;
    int expiry_day = arr[index].expiry_date.d;

    if (expiry_year < cy ||
        (expiry_year == cy && expiry_month < cm) ||
        (expiry_year == cy && expiry_month == cm && expiry_day < cd)) {
        return 1; // Medication is expired
    }

    if ((expiry_year == cy && expiry_month == cm + 1) ||
        (expiry_year == cy + 1 && cm == 12 && expiry_month == 1)) {
        return 2; // Medication will expire soon(in a month or less)
    }
    return 0;
}


void expiredmed(struct med arr[], int N,int cy,int cm,int cd)
{
    for( int i = 0; i < N; i++){
        if(expired(arr, N, cy, cm, cd) == 1)
        {
            printf("The medication %s is Expired\n", arr[i].name);
        }
        else if(expired(arr, N, cy, cm, cd) == 2)
        {
            printf("The medication %s will expire soon\n", arr[i].name);
        }
    }

}

void delexpire(struct med arr[], int *N, int cy, int cm, int cd)
{
    for(int i = 0; i < *N; i++)
    {
        if(expired(arr, *N, cy, cm, cd) == 1)
        {
            for (int j = i; j < *N - 1; j++) {
                arr[j] = arr[j + 1];
            }
            (*N)--;
            printf("Medication '%s' deleted successfully.\n", arr[i].name);
            break;
        }
    }


}

// Affichage of the array//////////////////////////////////////////////////////////////////////////////

void getmed(struct med arr[], int i)
{
    printf("Medication %d:\n", i + 1);
    printf("Name: %s\n", arr[i].name);
    printf("Brand: %s\n", arr[i].brand);
    printf("Quantity: %d\n", arr[i].q);
    printf("Price: %d\n", arr[i].pr);
    printf("Manufacturing Date: %d/%d/%d\n", arr[i].manufacturing_date.d, arr[i].manufacturing_date.m, arr[i].manufacturing_date.y);
    printf("Expiry Date: %d/%d/%d\n", arr[i].expiry_date.d, arr[i].expiry_date.m, arr[i].expiry_date.y);
    printf("\n");
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

void delmed(struct med arr[], int *N) // delet a medication from the stock
{
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
            (*N)--;
            printf("Medication '%s' deleted successfully.\n", name);
            break;
        }
    }

    if (!found) {
        printf("Medication '%s' not found.\n", name);
    }
}

void addmed(struct med arr[], int *N, int cy, int cm, int cd)
{
    int i;
    N = N + 1;
    fillmed(arr, N, cy, cm, cd);
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

// to make the ceses cool////////////////////////////////////////////////////////////////////////////////////

void wait() {
    printf("Press Enter to continue...\n");
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
    getchar();
}



int main()
{
    int N, choice;
    int cy, cm, cd;
    do {
        printf("Enter the current year: ");
        scanf("%d", &cy);
        printf("Enter the current month: ");
        scanf("%d", &cm);
        printf("Enter the current day: ");
        scanf("%d", &cd);
    } while (today(cy, cm, cd) == 1);


    printf("Enter the number of medications: ");
    scanf("%d", &N);

    struct med *medications = malloc(N * sizeof(struct med));


    fill(medications, N, cy, cm, cd);
    do
    {
    system("clear");
    printf("Enter you choice (0-6) :");
    printf("\n0. End the program.");
    printf("\n1. Add a medication.");
    printf("\n2. Show all medications.");
    printf("\n3. Search a medication by its first letter.");
    printf("\n4. Delete a medication.");
    printf("\n5. Update a new quantity to a medication.");
    printf("\n6. Show the expired medications or that will expire soon.");
    printf("\n7. Delete the expired medications");
    printf("\n8. Record the medications in a file\n");

    scanf("%d", &choice);
    switch(choice)
    {
        case 0:
           break;
        case 1:
           addmed(medications, N, cy, cm, cd);
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
           delmed(medications, N);
           wait();
           break;
        case 5:
           updateq(medications, N);
           wait();
           break;
        case 6:
           expiredmed(medications, N, cy, cm, cd);
           wait();
           break;
        case 7:
            delexpire(medications, N, cy, cm, cd);
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
    free(medications);
    return 0;
}
