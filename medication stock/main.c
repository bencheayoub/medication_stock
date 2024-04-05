#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void av(struct Date *D) // check if the date of med is logical
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

void fill(struct med arr[], int N) {
    int cy, cm, cd;
    do {
        printf("Enter the current year: ");
        scanf("%d", &cy);
        printf("Enter the current month: ");
        scanf("%d", &cm);
        printf("Enter the current day: ");
        scanf("%d", &cd);
    } while (today(cy, cm, cd) == 1);

    for (int i = 0; i < N; i++) {
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

        if (arr[i].expiry_date.y < cy ||
            (arr[i].expiry_date.y == cy && arr[i].expiry_date.m < cm) ||
            (arr[i].expiry_date.y == cy && arr[i].expiry_date.m == cm && arr[i].expiry_date.d < cd)) {
            printf("The medication %s is Expired\n", arr[i].name);
        } else if ((arr[i].expiry_date.y == cy + 1 && arr[i].expiry_date.m == 1 && cm == 12) ||
                   (arr[i].expiry_date.y == cy && arr[i].expiry_date.m == cm + 1) ||
                   (arr[i].expiry_date.y == cy && arr[i].expiry_date.m == cm && arr[i].expiry_date.d > cd)) {
            printf("The medication %s will expire soon\n", arr[i].name);
        }
    }
}


void show(struct med arr[], int N) // show the information of the med
{
    printf("\nEntered medications:\n");
    for (int i = 0; i < N; i++)
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
}

void search(struct med arr[], int N) // search the med by the first letter
{
    int dax;
    char c;
    printf("enter the first char of med : ");
    scanf(" %c", &c);
    for(int i = 0; i < N; i++) {
        if(arr[i].name[0] == c) {
        dax = 0;
            printf("The information of the medication you searched is:\n");
            printf("Name: %s\n", arr[i].name);
            printf("Brand: %s\n", arr[i].brand);
            printf("Quantity: %d\n", arr[i].q);
            printf("Price: %d\n", arr[i].pr);
            printf("Manufacturing Date: %d/%d/%d\n", arr[i].manufacturing_date.d, arr[i].manufacturing_date.m, arr[i].manufacturing_date.y);
            printf("Expiry Date: %d/%d/%d\n", arr[i].expiry_date.d, arr[i].expiry_date.m, arr[i].expiry_date.y);
            printf("\n");
        }
    }
    if (dax != 0)
    {
        printf("not found med");
    }
}

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


int main()
{
    int N;
    printf("Enter the number of medications: ");
    scanf("%d", &N);
    struct med *medications = malloc(N * sizeof(struct med));
    fill(medications, N);
    show(medications, N);
    search(medications, N);
    updateq(medications, N);
    delmed(medications, N);
    free(medications);
    return 0;
}


