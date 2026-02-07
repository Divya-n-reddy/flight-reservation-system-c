#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50
#define ADMIN_PASS "admin123"

// Passenger Structure
struct Passenger {
    char name[MAX];
    char passport[MAX];
    char email[MAX];
    char aadhar[MAX];
};

// Function declarations
void userMenu();
void adminMenu();
void reservation();
void displayFlights();
void addFlight();
void savePassenger(char src[], char dest[], char date[], char airline[], int seats);
int adminLogin();

//////////////////////////////////////////////////////////

int main() {

    int choice;

    while(1) {

        printf("\n====== Flight Reservation System ======\n");
        printf("1. User Booking\n");
        printf("2. Admin Login\n");
        printf("3. Exit\n");
        printf("Enter choice: ");

        scanf("%d",&choice);

        switch(choice) {

            case 1: userMenu(); break;
            case 2: adminMenu(); break;
            case 3: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
}

//////////////////////////////////////////////////////////

void userMenu() {
    reservation();
}

//////////////////////////////////////////////////////////

int adminLogin() {

    char pass[20];

    printf("Enter admin password: ");
    scanf("%19s",pass);

    if(strcmp(pass,ADMIN_PASS)==0)
        return 1;

    printf("Wrong password!\n");
    return 0;
}

//////////////////////////////////////////////////////////

void adminMenu() {

    if(!adminLogin())
        return;

    int ch;

    while(1) {

        printf("\n---- ADMIN PANEL ----\n");
        printf("1. View Flights\n");
        printf("2. Add Flight\n");
        printf("3. Back\n");

        scanf("%d",&ch);

        switch(ch) {
            case 1: displayFlights(); break;
            case 2: addFlight(); break;
            case 3: return;
            default: printf("Invalid choice\n");
        }
    }
}

//////////////////////////////////////////////////////////

void displayFlights() {

    FILE *fp = fopen("flights4.txt","r");

    if(!fp) {
        printf("No flights available!\n");
        return;
    }

    char src[20],dest[20],date[20],airline[20];
    int price,booked,available;

    printf("\nSOURCE DEST DATE AIRLINE PRICE AVAILABLE\n");
    printf("--------------------------------------------------\n");

    while(fscanf(fp,"%19s %19s %19s %19s %d %d %d",
          src,dest,date,airline,&price,&booked,&available)==7){

        printf("%s %s %s %s %d %d\n",
        src,dest,date,airline,price,available);
    }

    fclose(fp);
}

//////////////////////////////////////////////////////////

void addFlight() {

    FILE *fp = fopen("flights4.txt","a");

    if(!fp){
        printf("Error opening file!\n");
        return;
    }

    char src[20],dest[20],date[20],airline[20];
    int price,seats;

    printf("Enter Source Destination Date Airline Price Seats:\n");
    scanf("%19s %19s %19s %19s %d %d",
          src,dest,date,airline,&price,&seats);

    fprintf(fp,"%s %s %s %s %d 0 %d\n",
            src,dest,date,airline,price,seats);

    fclose(fp);

    printf("Flight added successfully!\n");
}

//////////////////////////////////////////////////////////

void reservation() {

    FILE *fp = fopen("flights4.txt","r");
    FILE *temp = fopen("temp.txt","w");

    if(!fp || !temp){
        printf("File error!\n");
        return;
    }

    displayFlights();

    char src[20],dest[20],date[20];
    printf("\nEnter Source Destination Date:\n");
    scanf("%19s %19s %19s",src,dest,date);

    char fsrc[20],fdest[20],fdate[20],airline[20];
    int price,booked,available;

    int found=0,seats;

    while(fscanf(fp,"%19s %19s %19s %19s %d %d %d",
          fsrc,fdest,fdate,airline,&price,&booked,&available)==7){

        if(strcmp(src,fsrc)==0 &&
           strcmp(dest,fdest)==0 &&
           strcmp(date,fdate)==0){

            found=1;

            printf("Available seats: %d\n",available);
            printf("Enter seats: ");
            scanf("%d",&seats);

            if(seats<=available){

                available-=seats;
                booked+=seats;

                printf("Total price: %d\n",seats*price);

                savePassenger(fsrc,fdest,fdate,airline,seats);
            }
            else
                printf("Not enough seats!\n");
        }

        fprintf(temp,"%s %s %s %s %d %d %d\n",
        fsrc,fdest,fdate,airline,price,booked,available);
    }

    fclose(fp);
    fclose(temp);

    remove("flights4.txt");
    rename("temp.txt","flights4.txt");

    if(!found)
        printf("Flight not found!\n");
}

//////////////////////////////////////////////////////////

void savePassenger(char src[], char dest[], char date[], char airline[], int seats) {

    FILE *fp = fopen("passengers.txt","a");

    if(!fp){
        printf("Error saving passengers!\n");
        return;
    }

    struct Passenger p;

    for(int i=0;i<seats;i++){

        printf("\nPassenger %d\n",i+1);

        printf("Name: ");
        scanf("%49s",p.name);

        printf("Passport: ");
        scanf("%49s",p.passport);

        printf("Email: ");
        scanf("%49s",p.email);

        printf("Aadhar: ");
        scanf("%49s",p.aadhar);

        fprintf(fp,"\n%s %s %s %s\n",src,dest,date,airline);
        fprintf(fp,"Name: %s\nPassport: %s\nEmail: %s\nAadhar: %s\n",
                p.name,p.passport,p.email,p.aadhar);
        fprintf(fp,"---------------------------\n");
    }

    fclose(fp);

    printf("Passengers saved successfully!\n");
}
