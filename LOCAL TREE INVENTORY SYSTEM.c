// LOCAL TREE INVENTORY SYSTEM

#include <stdio.h>
#include <string.h>
#include <time.h>

#define BARANGAYCOUNT 8
#define MANGOVARIETIES 2
#define COCONUTVARIETIES 2

const char *barangayNames[BARANGAYCOUNT] = {
    "Barangay Anislag", "Barangay Canangca-an", "Barangay Canapnapan", "Barangay Cancatac",
    "Barangay Pandol", "Barangay Poblacion", "Barangay Sambog", "Barangay Tanday"};

const char *mangoVarieties[MANGOVARIETIES] = {
    "Carabao (Mangifera indica)", "Pico (Mangifera indica)"};

const char *coconutVarieties[COCONUTVARIETIES] = {
    "Dwarf Tacunan (Cocos nucifera L)", "Laguna Tall (Cocos nucifera L)"};

int mangoTrees[BARANGAYCOUNT][MANGOVARIETIES];
int coconutTrees[BARANGAYCOUNT][COCONUTVARIETIES];


void getCoordinates(int index, double *latitude, double *longitude) {
    double latitudes[BARANGAYCOUNT] = {9.6683, 9.6702, 9.6738, 9.6771, 9.6815, 9.6839, 9.6873, 9.6907};
    double longitudes[BARANGAYCOUNT] = {123.9237, 123.9251, 123.9279, 123.9312, 123.9355, 123.9397, 123.9428, 123.9462};
    *latitude = latitudes[index];
    *longitude = longitudes[index];
}

// Save Data to File
void saveInventory() {
    FILE *file = fopen("inventory.txt", "w");
    if (!file) {
        printf("Error saving inventory.\n");
        return;
    }

    for (int i = 0; i < BARANGAYCOUNT; i++) {
        fprintf(file, "%s\n", barangayNames[i]);
        for (int j = 0; j < MANGOVARIETIES; j++) {
            fprintf(file, "%d", mangoTrees[i][j]);
            if (j < MANGOVARIETIES - 1) fprintf(file, " ");
        }
        fprintf(file, "\n");

        for (int j = 0; j < COCONUTVARIETIES; j++) {
            fprintf(file, "%d", coconutTrees[i][j]);
            if (j < COCONUTVARIETIES - 1) fprintf(file, " ");
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

// Load Data from File
void loadInventory() {
    FILE *file = fopen("inventory.txt", "r");
    if (!file) {
        printf("Could not open inventory.txt for reading. Using default values.\n");

        for (int i = 0; i < BARANGAYCOUNT; i++) {
            for (int j = 0; j < MANGOVARIETIES; j++) {
                mangoTrees[i][j] = 10;
            }
            for (int j = 0; j < COCONUTVARIETIES; j++) {
                coconutTrees[i][j] = 10;
            }
        }
        saveInventory();
        return;
    }

    char buffer[100];
    for (int i = 0; i < BARANGAYCOUNT; i++) {
        if (fgets(buffer, sizeof(buffer), file) == NULL) {
            printf("Error reading barangay name from file.\n");
            break;
        }
        buffer[strcspn(buffer, "\n")] = 0;

        for (int j = 0; j < MANGOVARIETIES; j++) {
            if (fscanf(file, "%d", &mangoTrees[i][j]) != 1) {
                printf("Error reading mango data for barangay %s. Using default value 10.\n", barangayNames[i]);
                mangoTrees[i][j] = 10;
            }
        }

        for (int j = 0; j < COCONUTVARIETIES; j++) {
            if (fscanf(file, "%d", &coconutTrees[i][j]) != 1) {
                printf("Error reading coconut data for barangay %s. Using default value 10.\n", barangayNames[i]);
                coconutTrees[i][j] = 10;
            }
        }


        fgets(buffer, sizeof(buffer), file);
    }

    fclose(file);
}

// View All Barangays
void viewAll() {
    time_t t = time(NULL);
    char datetime[100];
    strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", localtime(&t));
    printf("\n--- All Barangays Inventory ---\n\nDate and Time: %s\n", datetime);

    for (int i = 0; i < BARANGAYCOUNT; i++) {
        double BaseLat, BaseLon;
        getCoordinates(i, &BaseLat, &BaseLon);
        printf("\n%s:\n", barangayNames[i]);
        printf("  Barangay Location: (%.4f, %.4f)\n", BaseLat, BaseLon);

        printf("  Mango Trees:\n");
        for (int j = 0; j < MANGOVARIETIES; j++) {
            printf("    %s: %d trees\n", mangoVarieties[j], mangoTrees[i][j]);
            for (int k = 0; k < mangoTrees[i][j]; k++) {
                double treeLat = BaseLat + (k * 0.0001);
                double treeLon = BaseLon + (k * 0.0001);
                printf("      Tree %d Location: (%.4f, %.4f)\n", k + 1, treeLat, treeLon);
            }
        }

        printf("  Coconut Trees:\n");
        for (int j = 0; j < COCONUTVARIETIES; j++) {
            printf("    %s: %d trees\n", coconutVarieties[j], coconutTrees[i][j]);
            for (int k = 0; k < coconutTrees[i][j]; k++) {
                double treeLat = BaseLat + (k * 0.0001);
                double treeLon = BaseLon - (k * 0.0001);
                printf("      Tree %d Location: (%.4f, %.4f)\n", k + 1, treeLat, treeLon);
            }
        }
    }
}

// Search by Barangay
void searchBarangay() {
    int choice;
    while (1) {
        printf("\n--- Barangay List ---\n");
        for (int i = 0; i < BARANGAYCOUNT; i++) {
            printf("%d. %s\n", i + 1, barangayNames[i]);
        }

        printf("\nEnter the number corresponding to the Barangay: ");
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > BARANGAYCOUNT) {
            printf("Invalid input. Please enter a number between 1 and %d.\n", BARANGAYCOUNT);
            while (getchar() != '\n');
            continue;
        }

        break;
    }

    int index = choice - 1;

    time_t t = time(NULL);
    char datetime[100];
    strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", localtime(&t));

    printf("\n%s:\n", barangayNames[index]);
    printf("  Date and Time: %s\n", datetime);
    double lat, lon;
    getCoordinates(index, &lat, &lon);
    printf("  Location: (%.4f, %.4f)\n", lat, lon);
    printf("  Mango Trees:\n");
    for (int j = 0; j < MANGOVARIETIES; j++) {
        printf("    %s: %d\n", mangoVarieties[j], mangoTrees[index][j]);
    }
    printf("  Coconut Trees:\n");
    for (int j = 0; j < COCONUTVARIETIES; j++) {
        printf("    %s: %d\n", coconutVarieties[j], coconutTrees[index][j]);
    }
}

// Search by Fruit Type
void searchByFruit() {
    int choice;
    while (1) {
        printf("\nSearch by Fruit:\n1. Mango\n2. Coconut\nChoice: ");
        if (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
            printf("Invalid input. Please enter 1 for Mango or 2 for Coconut.\n");
            while (getchar() != '\n');
            continue;
        }

        break;
    }

    time_t t = time(NULL);
    char datetime[100];
    strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", localtime(&t));

    if (choice == 1) {
        printf("\n--- Mango Trees Inventory ---\n");
        printf("Date and Time: %s\n", datetime);
        for (int i = 0; i < BARANGAYCOUNT; i++) {
            double lat, lon;
            getCoordinates(i, &lat, &lon);
            printf("\n%s:\n", barangayNames[i]);
            printf("  Location: (%.4f, %.4f)\n", lat, lon);
            for (int j = 0; j < MANGOVARIETIES; j++) {
                printf("  %s: %d trees\n", mangoVarieties[j], mangoTrees[i][j]);
            }
        }
    } else if (choice == 2) {
        printf("\n--- Coconut Trees Inventory ---\n");
        printf("Date and Time: %s\n", datetime);
        for (int i = 0; i < BARANGAYCOUNT; i++) {
            double lat, lon;
            getCoordinates(i, &lat, &lon);
            printf("\n%s:\n", barangayNames[i]);
            printf("  Location: (%.4f, %.4f)\n", lat, lon);
            for (int j = 0; j < COCONUTVARIETIES; j++) {
                printf("  %s: %d trees\n", coconutVarieties[j], coconutTrees[i][j]);
            }
        }
    }
}

void addNewTree() {
    int treeType, variety, barangay, count;

    // Select tree type
    while (1) {
        printf("\nSelect tree type:\n1. Mango\n2. Coconut\nChoice: ");
        if (scanf("%d", &treeType) != 1 || (treeType != 1 && treeType != 2)) {
            printf("Invalid input. Please enter 1 for Mango or 2 for Coconut.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    }

    // Select variety
    if (treeType == 1) {
        printf("\nSelect Mango Variety:\n");
        for (int i = 0; i < MANGOVARIETIES; i++) {
            printf("%d. %s\n", i + 1, mangoVarieties[i]);
        }
        while (1) {
            printf("Choice: ");
            if (scanf("%d", &variety) != 1 || variety < 1 || variety > MANGOVARIETIES) {
                printf("Invalid input. Please enter a valid variety number.\n");
                while (getchar() != '\n');
                continue;
            }
            break;
        }
    } else {
        printf("\nSelect Coconut Variety:\n");
        for (int i = 0; i < COCONUTVARIETIES; i++) {
            printf("%d. %s\n", i + 1, coconutVarieties[i]);
        }
        while (1) {
            printf("Choice: ");
            if (scanf("%d", &variety) != 1 || variety < 1 || variety > COCONUTVARIETIES) {
                printf("Invalid input. Please enter a valid variety number.\n");
                while (getchar() != '\n');
                continue;
            }
            break;
        }
    }

    // Select barangay
    printf("\nSelect Barangay:\n");
    for (int i = 0; i < BARANGAYCOUNT; i++) {
        printf("%d. %s\n", i + 1, barangayNames[i]);
    }
    while (1) {
        printf("Choice: ");
        if (scanf("%d", &barangay) != 1 || barangay < 1 || barangay > BARANGAYCOUNT) {
            printf("Invalid input. Please enter a valid barangay number.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    }

    // Enter number of trees to add
    while (1) {
        printf("Enter number of trees to add: ");
        if (scanf("%d", &count) != 1 || count < 1) {
            printf("Invalid input. Please enter a positive number.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    }

    // Add trees and save
    if (treeType == 1) {
        mangoTrees[barangay - 1][variety - 1] += count;
        printf("%d %s mango tree(s) added to %s.\n", count, mangoVarieties[variety - 1], barangayNames[barangay - 1]);
    } else {
        coconutTrees[barangay - 1][variety - 1] += count;
        printf("%d %s coconut tree(s) added to %s.\n", count, coconutVarieties[variety - 1], barangayNames[barangay - 1]);
    }
    saveInventory();
    printf("Inventory saved to file.\n");

    // Optional: Show updated inventory for confirmation
    viewAll();
}


// Set all trees to 10
void setAllTreesToTen() {
    for (int i = 0; i < BARANGAYCOUNT; i++) {
        for (int j = 0; j < MANGOVARIETIES; j++) {
            mangoTrees[i][j] = 10;
        }
        for (int j = 0; j < COCONUTVARIETIES; j++) {
            coconutTrees[i][j] = 10;
        }
    }
    saveInventory();
    printf("All mango and coconut tree counts set to 10 and saved to file.\n");
}

int main() {
    loadInventory();
    // setAllTreesToTen(); // <-- Add this line to update and save all to 10
    int choice;
    do {
        printf("\n--- Corella's Tree Inventory ---\n");
        printf("1. Add New Tree\n");
        printf("2. View All Trees\n");
        printf("3. Search by Barangay\n");
        printf("4. Search by Fruit Type\n");
        printf("5. Exit\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1 and 5.\n");
            while (getchar() != '\n');
            continue;
        }

        if (choice < 1 || choice > 5) {
            printf("Invalid choice. Please enter a number between 1 and 5.\n");
            continue;
        }

        switch (choice) {
            case 1:
                addNewTree();
                break;
            case 2:
                viewAll();
                break;
            case 3:
                searchBarangay();
                break;
            case 4:
                searchByFruit();
                break;
            case 5:
                saveInventory();
                printf("Data saved. Goodbye!\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 5);

    return 0;
}