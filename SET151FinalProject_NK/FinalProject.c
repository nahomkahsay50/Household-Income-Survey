// --------------------------------------------------------------------------------
// Name: Nahom Kahsay
// Class: SET-151-400
// Abstract: Final Project - Household Income Survey Data Collection and Analysis
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --------------------------------------------------------------------------------
// Constants
// --------------------------------------------------------------------------------
#define MAX_DATE_LENGTH 20
#define MAX_STATE_LENGTH 15
#define MAX_COUNTY_LENGTH 15
#define MAX_RACE_LENGTH 20
#define MAX_LINE_LENGTH 200
#define CSV_FILE "household_survey_data.csv"

// Poverty thresholds
#define POVERTY_HOUSEHOLD_1 12000.0
#define POVERTY_HOUSEHOLD_2 18000.0
#define POVERTY_HOUSEHOLD_3 25000.0
#define POVERTY_HOUSEHOLD_4 30000.0
#define POVERTY_HOUSEHOLD_5_PLUS 40000.0

// --------------------------------------------------------------------------------
// User Defined Types (Structures)
// --------------------------------------------------------------------------------
typedef struct
{
    char strDate[MAX_DATE_LENGTH];
    char strState[MAX_STATE_LENGTH];
    char strCounty[MAX_COUNTY_LENGTH];
    char strRace[MAX_RACE_LENGTH];
    int intHouseholdSize;
    double dblYearlyIncome;
} udtHouseholdDataType;

// --------------------------------------------------------------------------------
// Prototypes
// --------------------------------------------------------------------------------
// Main program flow
void WriteMode();
void DisplayMode();

// Data entry functions
void GetHouseholdData(udtHouseholdDataType* pudtHousehold);
int ValidateAndGetState(char* pstrState);
int ValidateAndGetCounty(char* pstrCounty, const char* pstrState);
int ValidateAndGetRace(char* pstrRace);
int ValidateHouseholdSize();
double ValidateYearlyIncome();

// File I/O functions
void SaveHouseholdToCSV(const udtHouseholdDataType* pudtHousehold);
void LoadHouseholdsFromCSV(udtHouseholdDataType** ppudtHouseholds, int* pintRecordCount);
void DeleteHouseholdArray(udtHouseholdDataType** ppudtHouseholds, int* pintRecordCount);
void ClearCSVFile();

// Display and menu functions
void DisplayAnalysisMenu();
void ProcessDisplayOption(udtHouseholdDataType* audtHouseholds, int intRecordCount, char chrOption);

// Analysis functions
void DisplayTotalHouseholds(udtHouseholdDataType* audtHouseholds, int intRecordCount);
void DisplayTotalHouseholdsByCounty(udtHouseholdDataType* audtHouseholds, int intRecordCount);
void DisplayTotalHouseholdsByRace(udtHouseholdDataType* audtHouseholds, int intRecordCount);
void DisplayAverageHouseholdIncome(udtHouseholdDataType* audtHouseholds, int intRecordCount);
void DisplayAverageIncomeByCounty(udtHouseholdDataType* audtHouseholds, int intRecordCount);
void DisplayAverageIncomeByRace(udtHouseholdDataType* audtHouseholds, int intRecordCount);
void DisplayPercentageBelowPoverty(udtHouseholdDataType* audtHouseholds, int intRecordCount);
void DisplayPercentageBelowPovertyByCounty(udtHouseholdDataType* audtHouseholds, int intRecordCount);
void DisplayPercentageBelowPovertyByRace(udtHouseholdDataType* audtHouseholds, int intRecordCount);

// Utility functions
int IsBelowPoverty(int intHouseholdSize, double dblIncome);
void ClearInputBuffer();

// --------------------------------------------------------------------------------
//	Name: main
//	Abstract: This is where the program starts
// --------------------------------------------------------------------------------
int main()
{
    int intMode = 0;

    printf("=== Census Bureau Household Income Survey System ===\n");

    do
    {
        printf("\n=== Main Menu ===\n");
        printf("1. Write Mode (Enter new survey data)\n");
        printf("2. Display Mode (View and analyze existing data)\n");
        printf("3. Exit Program\n");
        printf("Enter your choice (1-3): ");

        scanf("%d", &intMode);
        ClearInputBuffer();

        switch (intMode)
        {
        case 1:
            WriteMode();
            break;
        case 2:
            DisplayMode();
            break;
        case 3:
            printf("Thank you for using the Census Survey System!\n");
            break;
        default:
            printf("Invalid choice. Please select 1, 2, or 3.\n");
        }

    } while (intMode != 3);

    system("pause");
    return 0;
}

// --------------------------------------------------------------------------------
//	Name: WriteMode
//	Abstract: Handles data entry mode - allows user to enter household survey data
// --------------------------------------------------------------------------------
void WriteMode()
{
    udtHouseholdDataType udtHousehold;
    char chrContinueEntry = 0;

    printf("\n=== Write Mode - Data Entry ===\n");

    do
    {
        printf("\n--- Enter Household Survey Data ---\n");
        GetHouseholdData(&udtHousehold);
        SaveHouseholdToCSV(&udtHousehold);
        printf("Data saved successfully!\n\n");

        printf("Do you want to enter another household? (y/n): ");
        scanf(" %c", &chrContinueEntry);
        ClearInputBuffer();

    } while (tolower(chrContinueEntry) == 'y');

    printf("Thank you for using the survey system!\n");
}

// --------------------------------------------------------------------------------
//	Name: DisplayMode
//	Abstract: Handles display/analysis mode - loads data and provides analysis menu
// --------------------------------------------------------------------------------
void DisplayMode()
{
    int intRecordCount = 0;
    udtHouseholdDataType* audtHouseholds = 0;
    char chrOption = 0;

    printf("\n=== Display Mode - Data Analysis ===\n");

    do
    {
        // Load fresh data each time in case CSV was cleared
        if (audtHouseholds != 0)
        {
            DeleteHouseholdArray(&audtHouseholds, &intRecordCount);
        }

        LoadHouseholdsFromCSV(&audtHouseholds, &intRecordCount);

        if (audtHouseholds == 0 || intRecordCount == 0)
        {
            printf("No data found. Please add data in Write Mode or check data file.\n");
            printf("Current records in database: 0\n\n");
        }
        else
        {
            printf("Current records in database: %d\n\n", intRecordCount);
        }

        DisplayAnalysisMenu();
        printf("Enter your choice: ");
        scanf(" %c", &chrOption);
        ClearInputBuffer();

        chrOption = toupper(chrOption);
        ProcessDisplayOption(audtHouseholds, intRecordCount, chrOption);

        if (chrOption != 'J')
        {
            printf("\nPress Enter to continue...");
            getchar();
        }
    } while (chrOption != 'J');

    // Clean up memory before returning to main menu
    if (audtHouseholds != 0)
    {
        DeleteHouseholdArray(&audtHouseholds, &intRecordCount);
    }
    printf("Returning to main menu...\n");
}

// --------------------------------------------------------------------------------
//	Name: GetHouseholdData
//	Abstract: Prompts user for all household survey data with validation
// --------------------------------------------------------------------------------
void GetHouseholdData(udtHouseholdDataType* pudtHousehold)
{
    printf("Enter survey date (MM/DD/YYYY): ");
    fgets(pudtHousehold->strDate, MAX_DATE_LENGTH, stdin);
    // Remove newline character
    pudtHousehold->strDate[strcspn(pudtHousehold->strDate, "\n")] = 0;

    ValidateAndGetState(pudtHousehold->strState);
    ValidateAndGetCounty(pudtHousehold->strCounty, pudtHousehold->strState);
    ValidateAndGetRace(pudtHousehold->strRace);
    pudtHousehold->intHouseholdSize = ValidateHouseholdSize();
    pudtHousehold->dblYearlyIncome = ValidateYearlyIncome();
}

// --------------------------------------------------------------------------------
//	Name: ValidateAndGetState
//	Abstract: Prompts user to select state and validates input
// --------------------------------------------------------------------------------
int ValidateAndGetState(char* pstrState)
{
    int intChoice = 0;

    do
    {
        printf("\nSelect State:\n");
        printf("1. Ohio\n");
        printf("2. Kentucky\n");
        printf("Enter choice (1-2): ");
        scanf("%d", &intChoice);
        ClearInputBuffer();

        switch (intChoice)
        {
        case 1:
            strcpy(pstrState, "Ohio");
            return 1;
        case 2:
            strcpy(pstrState, "Kentucky");
            return 1;
        default:
            printf("Invalid choice. Please select 1 or 2.\n");
        }
    } while (1);
}

// --------------------------------------------------------------------------------
//	Name: ValidateAndGetCounty
//	Abstract: Prompts user to select county based on state and validates input
// --------------------------------------------------------------------------------
int ValidateAndGetCounty(char* pstrCounty, const char* pstrState)
{
    int intChoice = 0;

    if (strcmp(pstrState, "Ohio") == 0)
    {
        do
        {
            printf("\nSelect Ohio County:\n");
            printf("1. Hamilton\n");
            printf("2. Butler\n");
            printf("Enter choice (1-2): ");
            scanf("%d", &intChoice);
            ClearInputBuffer();

            switch (intChoice)
            {
            case 1:
                strcpy(pstrCounty, "Hamilton");
                return 1;
            case 2:
                strcpy(pstrCounty, "Butler");
                return 1;
            default:
                printf("Invalid choice. Please select 1 or 2.\n");
            }
        } while (1);
    }
    else // Kentucky
    {
        do
        {
            printf("\nSelect Kentucky County:\n");
            printf("1. Boone\n");
            printf("2. Kenton\n");
            printf("Enter choice (1-2): ");
            scanf("%d", &intChoice);
            ClearInputBuffer();

            switch (intChoice)
            {
            case 1:
                strcpy(pstrCounty, "Boone");
                return 1;
            case 2:
                strcpy(pstrCounty, "Kenton");
                return 1;
            default:
                printf("Invalid choice. Please select 1 or 2.\n");
            }
        } while (1);
    }
}

// --------------------------------------------------------------------------------
//	Name: ValidateAndGetRace
//	Abstract: Prompts user to select race and validates input
// --------------------------------------------------------------------------------
int ValidateAndGetRace(char* pstrRace)
{
    int intChoice = 0;

    do
    {
        printf("\nSelect Race of Head of Household:\n");
        printf("1. Caucasian\n");
        printf("2. African American\n");
        printf("3. Hispanic\n");
        printf("4. Asian\n");
        printf("5. Other\n");
        printf("Enter choice (1-5): ");
        scanf("%d", &intChoice);
        ClearInputBuffer();

        switch (intChoice)
        {
        case 1:
            strcpy(pstrRace, "Caucasian");
            return 1;
        case 2:
            strcpy(pstrRace, "African American");
            return 1;
        case 3:
            strcpy(pstrRace, "Hispanic");
            return 1;
        case 4:
            strcpy(pstrRace, "Asian");
            return 1;
        case 5:
            strcpy(pstrRace, "Other");
            return 1;
        default:
            printf("Invalid choice. Please select 1-5.\n");
        }
    } while (1);
}

// --------------------------------------------------------------------------------
//	Name: ValidateHouseholdSize
//	Abstract: Prompts for household size and validates it's greater than 0
// --------------------------------------------------------------------------------
int ValidateHouseholdSize()
{
    int intSize = 0;

    do
    {
        printf("Enter number of people in household: ");
        scanf("%d", &intSize);
        ClearInputBuffer();

        if (intSize > 0)
        {
            return intSize;
        }
        else
        {
            printf("Household size must be greater than 0.\n");
        }
    } while (1);
}

// --------------------------------------------------------------------------------
//	Name: ValidateYearlyIncome
//	Abstract: Prompts for yearly income and validates it's greater than 0
// --------------------------------------------------------------------------------
double ValidateYearlyIncome()
{
    double dblIncome = 0.0;

    do
    {
        printf("Enter yearly household income: $");
        scanf("%lf", &dblIncome);
        ClearInputBuffer();

        if (dblIncome > 0)
        {
            return dblIncome;
        }
        else
        {
            printf("Income must be greater than 0.\n");
        }
    } while (1);
}

// --------------------------------------------------------------------------------
//	Name: SaveHouseholdToCSV
//	Abstract: Saves household data to CSV file (appends to existing data)
// --------------------------------------------------------------------------------
void SaveHouseholdToCSV(const udtHouseholdDataType* pudtHousehold)
{
    FILE* pflFile = fopen(CSV_FILE, "a");
    if (pflFile == 0)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(pflFile, "%s,%s,%s,%s,%d,%.2f\n",
        pudtHousehold->strDate, pudtHousehold->strState, pudtHousehold->strCounty,
        pudtHousehold->strRace, pudtHousehold->intHouseholdSize, pudtHousehold->dblYearlyIncome);

    fclose(pflFile);
}

// --------------------------------------------------------------------------------
//	Name: LoadHouseholdsFromCSV
//	Abstract: Loads all household data from CSV into dynamically allocated array
// --------------------------------------------------------------------------------
void LoadHouseholdsFromCSV(udtHouseholdDataType** ppudtHouseholds, int* pintRecordCount)
{
    FILE* pflFile = fopen(CSV_FILE, "r");
    char astrLine[MAX_LINE_LENGTH];
    int intIndex = 0;
    char* pstrToken = 0;

    if (pflFile == 0)
    {
        printf("No data file found.\n");
        *pintRecordCount = 0;
        *ppudtHouseholds = 0;
        return;
    }

    // Count lines first
    *pintRecordCount = 0;
    while (fgets(astrLine, sizeof(astrLine), pflFile))
    {
        (*pintRecordCount)++;
    }

    if (*pintRecordCount == 0)
    {
        fclose(pflFile);
        *ppudtHouseholds = 0;
        return;
    }

    // Allocate memory for household data array
    *ppudtHouseholds = (udtHouseholdDataType*)malloc(*pintRecordCount * sizeof(udtHouseholdDataType));
    if (*ppudtHouseholds == 0)
    {
        printf("Memory allocation failed.\n");
        fclose(pflFile);
        *pintRecordCount = 0;
        return;
    }

    // Read and parse data
    rewind(pflFile);
    intIndex = 0;
    while (fgets(astrLine, sizeof(astrLine), pflFile) && intIndex < *pintRecordCount)
    {
        // Parse date
        pstrToken = strtok(astrLine, ",");
        if (pstrToken) strcpy((*ppudtHouseholds + intIndex)->strDate, pstrToken);

        // Parse state
        pstrToken = strtok(0, ",");
        if (pstrToken) strcpy((*ppudtHouseholds + intIndex)->strState, pstrToken);

        // Parse county
        pstrToken = strtok(0, ",");
        if (pstrToken) strcpy((*ppudtHouseholds + intIndex)->strCounty, pstrToken);

        // Parse race
        pstrToken = strtok(0, ",");
        if (pstrToken) strcpy((*ppudtHouseholds + intIndex)->strRace, pstrToken);

        // Parse household size
        pstrToken = strtok(0, ",");
        if (pstrToken) (*ppudtHouseholds + intIndex)->intHouseholdSize = atoi(pstrToken);

        // Parse yearly income
        pstrToken = strtok(0, ",");
        if (pstrToken) (*ppudtHouseholds + intIndex)->dblYearlyIncome = atof(pstrToken);

        intIndex++;
    }

    fclose(pflFile);
}

// --------------------------------------------------------------------------------
//	Name: DeleteHouseholdArray
//	Abstract: Frees dynamically allocated memory and resets pointers
// --------------------------------------------------------------------------------
void DeleteHouseholdArray(udtHouseholdDataType** ppudtHouseholds, int* pintRecordCount)
{
    // Free memory and reset pointers
    free(*ppudtHouseholds);
    *ppudtHouseholds = 0;
    *pintRecordCount = 0;
}

// --------------------------------------------------------------------------------
//	Name: DisplayAnalysisMenu
//	Abstract: Displays the analysis menu options
// --------------------------------------------------------------------------------
void DisplayAnalysisMenu()
{
    printf("\n=== Analysis Menu ===\n");
    printf("A. Total Households Surveyed\n");
    printf("B. Total Households Surveyed per County\n");
    printf("C. Total Households Surveyed per Race\n");
    printf("D. Average Household Income\n");
    printf("E. Average Household Income by County and State\n");
    printf("F. Average Household Income by Race\n");
    printf("G. Percentage below Poverty\n");
    printf("H. Percentage below Poverty by County and State\n");
    printf("I. Percentage below Poverty by Race\n");
    printf("K. Clear All Data (Delete CSV file contents)\n");
    printf("J. Return to main menu\n");
}

// --------------------------------------------------------------------------------
//	Name: ProcessDisplayOption
//	Abstract: Processes the user's menu selection and calls appropriate function
// --------------------------------------------------------------------------------
void ProcessDisplayOption(udtHouseholdDataType* audtHouseholds, int intRecordCount, char chrOption)
{
    printf("\n");
    switch (chrOption)
    {
    case 'A':
        DisplayTotalHouseholds(audtHouseholds, intRecordCount);
        break;
    case 'B':
        DisplayTotalHouseholdsByCounty(audtHouseholds, intRecordCount);
        break;
    case 'C':
        DisplayTotalHouseholdsByRace(audtHouseholds, intRecordCount);
        break;
    case 'D':
        DisplayAverageHouseholdIncome(audtHouseholds, intRecordCount);
        break;
    case 'E':
        DisplayAverageIncomeByCounty(audtHouseholds, intRecordCount);
        break;
    case 'F':
        DisplayAverageIncomeByRace(audtHouseholds, intRecordCount);
        break;
    case 'G':
        DisplayPercentageBelowPoverty(audtHouseholds, intRecordCount);
        break;
    case 'H':
        DisplayPercentageBelowPovertyByCounty(audtHouseholds, intRecordCount);
        break;
    case 'I':
        DisplayPercentageBelowPovertyByRace(audtHouseholds, intRecordCount);
        break;
    case 'K':
        ClearCSVFile();
        break;
    case 'J':
        // Return to main menu - handled in DisplayMode()
        break;
    default:
        printf("Invalid option. Please try again.\n");
    }
}

// --------------------------------------------------------------------------------
//	Name: DisplayTotalHouseholds
//	Abstract: Displays total number of households surveyed
// --------------------------------------------------------------------------------
void DisplayTotalHouseholds(udtHouseholdDataType* audtHouseholds, int intRecordCount)
{
    printf("Total Households Surveyed: %d\n", intRecordCount);
}

// --------------------------------------------------------------------------------
//	Name: DisplayTotalHouseholdsByCounty
//	Abstract: Displays total households by county and state
// --------------------------------------------------------------------------------
void DisplayTotalHouseholdsByCounty(udtHouseholdDataType* audtHouseholds, int intRecordCount)
{
    int intOhioHamilton = 0;
    int intOhioButler = 0;
    int intKyBoone = 0;
    int intKyKenton = 0;
    int intIndex = 0;

    // Count households by county
    for (intIndex = 0; intIndex < intRecordCount; intIndex++)
    {
        if (strcmp((audtHouseholds + intIndex)->strState, "Ohio") == 0)
        {
            if (strcmp((audtHouseholds + intIndex)->strCounty, "Hamilton") == 0)
                intOhioHamilton++;
            else if (strcmp((audtHouseholds + intIndex)->strCounty, "Butler") == 0)
                intOhioButler++;
        }
        else if (strcmp((audtHouseholds + intIndex)->strState, "Kentucky") == 0)
        {
            if (strcmp((audtHouseholds + intIndex)->strCounty, "Boone") == 0)
                intKyBoone++;
            else if (strcmp((audtHouseholds + intIndex)->strCounty, "Kenton") == 0)
                intKyKenton++;
        }
    }

    printf("Total Households Surveyed per County:\n\n");
    printf("Ohio: %d\n", intOhioHamilton + intOhioButler);
    printf("   Hamilton: %d\n", intOhioHamilton);
    printf("   Butler: %d\n", intOhioButler);
    printf("\nKentucky: %d\n", intKyBoone + intKyKenton);
    printf("   Boone: %d\n", intKyBoone);
    printf("   Kenton: %d\n", intKyKenton);
}

// --------------------------------------------------------------------------------
//	Name: DisplayTotalHouseholdsByRace
//	Abstract: Displays total households by race
// --------------------------------------------------------------------------------
void DisplayTotalHouseholdsByRace(udtHouseholdDataType* audtHouseholds, int intRecordCount)
{
    int intCaucasian = 0;
    int intAfricanAmerican = 0;
    int intHispanic = 0;
    int intAsian = 0;
    int intOther = 0;
    int intIndex = 0;

    // Count households by race
    for (intIndex = 0; intIndex < intRecordCount; intIndex++)
    {
        if (strcmp((audtHouseholds + intIndex)->strRace, "Caucasian") == 0)
            intCaucasian++;
        else if (strcmp((audtHouseholds + intIndex)->strRace, "African American") == 0)
            intAfricanAmerican++;
        else if (strcmp((audtHouseholds + intIndex)->strRace, "Hispanic") == 0)
            intHispanic++;
        else if (strcmp((audtHouseholds + intIndex)->strRace, "Asian") == 0)
            intAsian++;
        else if (strcmp((audtHouseholds + intIndex)->strRace, "Other") == 0)
            intOther++;
    }

    printf("Total Households Surveyed per Race:\n\n");
    printf("Caucasian: %d\n", intCaucasian);
    printf("African American: %d\n", intAfricanAmerican);
    printf("Hispanic: %d\n", intHispanic);
    printf("Asian: %d\n", intAsian);
    printf("Other: %d\n", intOther);
}

// --------------------------------------------------------------------------------
//	Name: DisplayAverageHouseholdIncome
//	Abstract: Displays overall average household income
// --------------------------------------------------------------------------------
void DisplayAverageHouseholdIncome(udtHouseholdDataType* audtHouseholds, int intRecordCount)
{
    double dblTotal = 0.0;
    int intIndex = 0;

    // Calculate total income
    for (intIndex = 0; intIndex < intRecordCount; intIndex++)
    {
        dblTotal += (audtHouseholds + intIndex)->dblYearlyIncome;
    }

    printf("Average Household Income: $%.2f\n", dblTotal / intRecordCount);
}

// --------------------------------------------------------------------------------
//	Name: DisplayAverageIncomeByCounty
//	Abstract: Displays average income by county and state
// --------------------------------------------------------------------------------
void DisplayAverageIncomeByCounty(udtHouseholdDataType* audtHouseholds, int intRecordCount)
{
    double dblOhioTotal = 0.0;
    double dblOhioHamiltonTotal = 0.0;
    double dblOhioButlerTotal = 0.0;
    double dblKyTotal = 0.0;
    double dblKyBooneTotal = 0.0;
    double dblKyKentonTotal = 0.0;
    int intOhioCount = 0;
    int intOhioHamiltonCount = 0;
    int intOhioButlerCount = 0;
    int intKyCount = 0;
    int intKyBooneCount = 0;
    int intKyKentonCount = 0;
    int intIndex = 0;

    // Calculate totals and counts by county
    for (intIndex = 0; intIndex < intRecordCount; intIndex++)
    {
        if (strcmp((audtHouseholds + intIndex)->strState, "Ohio") == 0)
        {
            dblOhioTotal += (audtHouseholds + intIndex)->dblYearlyIncome;
            intOhioCount++;
            if (strcmp((audtHouseholds + intIndex)->strCounty, "Hamilton") == 0)
            {
                dblOhioHamiltonTotal += (audtHouseholds + intIndex)->dblYearlyIncome;
                intOhioHamiltonCount++;
            }
            else if (strcmp((audtHouseholds + intIndex)->strCounty, "Butler") == 0)
            {
                dblOhioButlerTotal += (audtHouseholds + intIndex)->dblYearlyIncome;
                intOhioButlerCount++;
            }
        }
        else if (strcmp((audtHouseholds + intIndex)->strState, "Kentucky") == 0)
        {
            dblKyTotal += (audtHouseholds + intIndex)->dblYearlyIncome;
            intKyCount++;
            if (strcmp((audtHouseholds + intIndex)->strCounty, "Boone") == 0)
            {
                dblKyBooneTotal += (audtHouseholds + intIndex)->dblYearlyIncome;
                intKyBooneCount++;
            }
            else if (strcmp((audtHouseholds + intIndex)->strCounty, "Kenton") == 0)
            {
                dblKyKentonTotal += (audtHouseholds + intIndex)->dblYearlyIncome;
                intKyKentonCount++;
            }
        }
    }

    printf("Average Household Income by County and State:\n\n");
    if (intOhioCount > 0)
    {
        printf("Ohio: $%.2f\n", dblOhioTotal / intOhioCount);
        if (intOhioHamiltonCount > 0)
            printf("   Hamilton: $%.2f\n", dblOhioHamiltonTotal / intOhioHamiltonCount);
        if (intOhioButlerCount > 0)
            printf("   Butler: $%.2f\n", dblOhioButlerTotal / intOhioButlerCount);
    }
    printf("\n");
    if (intKyCount > 0)
    {
        printf("Kentucky: $%.2f\n", dblKyTotal / intKyCount);
        if (intKyBooneCount > 0)
            printf("   Boone: $%.2f\n", dblKyBooneTotal / intKyBooneCount);
        if (intKyKentonCount > 0)
            printf("   Kenton: $%.2f\n", dblKyKentonTotal / intKyKentonCount);
    }
}

// --------------------------------------------------------------------------------
//	Name: DisplayAverageIncomeByRace
//	Abstract: Displays average income by race
// --------------------------------------------------------------------------------
void DisplayAverageIncomeByRace(udtHouseholdDataType* audtHouseholds, int intRecordCount)
{
    double dblCaucasianTotal = 0.0;
    double dblAfricanAmericanTotal = 0.0;
    double dblHispanicTotal = 0.0;
    double dblAsianTotal = 0.0;
    double dblOtherTotal = 0.0;
    int intCaucasianCount = 0;
    int intAfricanAmericanCount = 0;
    int intHispanicCount = 0;
    int intAsianCount = 0;
    int intOtherCount = 0;
    int intIndex = 0;

    // Calculate totals and counts by race
    for (intIndex = 0; intIndex < intRecordCount; intIndex++)
    {
        if (strcmp((audtHouseholds + intIndex)->strRace, "Caucasian") == 0)
        {
            dblCaucasianTotal += (audtHouseholds + intIndex)->dblYearlyIncome;
            intCaucasianCount++;
        }
        else if (strcmp((audtHouseholds + intIndex)->strRace, "African American") == 0)
        {
            dblAfricanAmericanTotal += (audtHouseholds + intIndex)->dblYearlyIncome;
            intAfricanAmericanCount++;
        }
        else if (strcmp((audtHouseholds + intIndex)->strRace, "Hispanic") == 0)
        {
            dblHispanicTotal += (audtHouseholds + intIndex)->dblYearlyIncome;
            intHispanicCount++;
        }
        else if (strcmp((audtHouseholds + intIndex)->strRace, "Asian") == 0)
        {
            dblAsianTotal += (audtHouseholds + intIndex)->dblYearlyIncome;
            intAsianCount++;
        }
        else if (strcmp((audtHouseholds + intIndex)->strRace, "Other") == 0)
        {
            dblOtherTotal += (audtHouseholds + intIndex)->dblYearlyIncome;
            intOtherCount++;
        }
    }

    printf("Average Household Income by Race:\n\n");
    if (intCaucasianCount > 0)
        printf("Caucasian: $%.2f\n", dblCaucasianTotal / intCaucasianCount);
    if (intAfricanAmericanCount > 0)
        printf("African American: $%.2f\n", dblAfricanAmericanTotal / intAfricanAmericanCount);
    if (intHispanicCount > 0)
        printf("Hispanic: $%.2f\n", dblHispanicTotal / intHispanicCount);
    if (intAsianCount > 0)
        printf("Asian: $%.2f\n", dblAsianTotal / intAsianCount);
    if (intOtherCount > 0)
        printf("Other: $%.2f\n", dblOtherTotal / intOtherCount);
}

// --------------------------------------------------------------------------------
//	Name: DisplayPercentageBelowPoverty
//	Abstract: Displays overall percentage of households below poverty line
// --------------------------------------------------------------------------------
void DisplayPercentageBelowPoverty(udtHouseholdDataType* audtHouseholds, int intRecordCount)
{
    int intBelowPovertyCount = 0;
    int intIndex = 0;
    double dblPercentage = 0.0;

    // Count households below poverty
    for (intIndex = 0; intIndex < intRecordCount; intIndex++)
    {
        if (IsBelowPoverty((audtHouseholds + intIndex)->intHouseholdSize,
            (audtHouseholds + intIndex)->dblYearlyIncome))
        {
            intBelowPovertyCount++;
        }
    }

    dblPercentage = (double)intBelowPovertyCount / intRecordCount * 100.0;
    printf("Percentage below Poverty: %.2f%%\n", dblPercentage);
}

// --------------------------------------------------------------------------------
//	Name: DisplayPercentageBelowPovertyByCounty
//	Abstract: Displays percentage below poverty by county and state
// --------------------------------------------------------------------------------
void DisplayPercentageBelowPovertyByCounty(udtHouseholdDataType* audtHouseholds, int intRecordCount)
{
    int intOhioTotal = 0;
    int intOhioHamiltonTotal = 0;
    int intOhioButlerTotal = 0;
    int intKyTotal = 0;
    int intKyBooneTotal = 0;
    int intKyKentonTotal = 0;
    int intOhioPoverty = 0;
    int intOhioHamiltonPoverty = 0;
    int intOhioButlerPoverty = 0;
    int intKyPoverty = 0;
    int intKyBoonePoverty = 0;
    int intKyKentonPoverty = 0;
    int intIndex = 0;
    int blnIsPoverty = 0;

    // Count totals and poverty cases by county
    for (intIndex = 0; intIndex < intRecordCount; intIndex++)
    {
        blnIsPoverty = IsBelowPoverty((audtHouseholds + intIndex)->intHouseholdSize,
            (audtHouseholds + intIndex)->dblYearlyIncome);

        if (strcmp((audtHouseholds + intIndex)->strState, "Ohio") == 0)
        {
            intOhioTotal++;
            if (blnIsPoverty) intOhioPoverty++;
            if (strcmp((audtHouseholds + intIndex)->strCounty, "Hamilton") == 0)
            {
                intOhioHamiltonTotal++;
                if (blnIsPoverty) intOhioHamiltonPoverty++;
            }
            else if (strcmp((audtHouseholds + intIndex)->strCounty, "Butler") == 0)
            {
                intOhioButlerTotal++;
                if (blnIsPoverty) intOhioButlerPoverty++;
            }
        }
        else if (strcmp((audtHouseholds + intIndex)->strState, "Kentucky") == 0)
        {
            intKyTotal++;
            if (blnIsPoverty) intKyPoverty++;
            if (strcmp((audtHouseholds + intIndex)->strCounty, "Boone") == 0)
            {
                intKyBooneTotal++;
                if (blnIsPoverty) intKyBoonePoverty++;
            }
            else if (strcmp((audtHouseholds + intIndex)->strCounty, "Kenton") == 0)
            {
                intKyKentonTotal++;
                if (blnIsPoverty) intKyKentonPoverty++;
            }
        }
    }

    printf("Percentage below Poverty by County and State:\n\n");
    if (intOhioTotal > 0)
    {
        printf("Ohio: %.2f%%\n", (double)intOhioPoverty / intOhioTotal * 100.0);
        if (intOhioHamiltonTotal > 0)
            printf("   Hamilton: %.2f%%\n", (double)intOhioHamiltonPoverty / intOhioHamiltonTotal * 100.0);
        if (intOhioButlerTotal > 0)
            printf("   Butler: %.2f%%\n", (double)intOhioButlerPoverty / intOhioButlerTotal * 100.0);
    }
    printf("\n");
    if (intKyTotal > 0)
    {
        printf("Kentucky: %.2f%%\n", (double)intKyPoverty / intKyTotal * 100.0);
        if (intKyBooneTotal > 0)
            printf("   Boone: %.2f%%\n", (double)intKyBoonePoverty / intKyBooneTotal * 100.0);
        if (intKyKentonTotal > 0)
            printf("   Kenton: %.2f%%\n", (double)intKyKentonPoverty / intKyKentonTotal * 100.0);
    }
}

// --------------------------------------------------------------------------------
//	Name: DisplayPercentageBelowPovertyByRace
//	Abstract: Displays percentage below poverty by race
// --------------------------------------------------------------------------------
void DisplayPercentageBelowPovertyByRace(udtHouseholdDataType* audtHouseholds, int intRecordCount)
{
    int intCaucasianTotal = 0;
    int intAfricanAmericanTotal = 0;
    int intHispanicTotal = 0;
    int intAsianTotal = 0;
    int intOtherTotal = 0;
    int intCaucasianPoverty = 0;
    int intAfricanAmericanPoverty = 0;
    int intHispanicPoverty = 0;
    int intAsianPoverty = 0;
    int intOtherPoverty = 0;
    int intIndex = 0;
    int blnIsPoverty = 0;

    // Count totals and poverty cases by race
    for (intIndex = 0; intIndex < intRecordCount; intIndex++)
    {
        blnIsPoverty = IsBelowPoverty((audtHouseholds + intIndex)->intHouseholdSize,
            (audtHouseholds + intIndex)->dblYearlyIncome);

        if (strcmp((audtHouseholds + intIndex)->strRace, "Caucasian") == 0)
        {
            intCaucasianTotal++;
            if (blnIsPoverty) intCaucasianPoverty++;
        }
        else if (strcmp((audtHouseholds + intIndex)->strRace, "African American") == 0)
        {
            intAfricanAmericanTotal++;
            if (blnIsPoverty) intAfricanAmericanPoverty++;
        }
        else if (strcmp((audtHouseholds + intIndex)->strRace, "Hispanic") == 0)
        {
            intHispanicTotal++;
            if (blnIsPoverty) intHispanicPoverty++;
        }
        else if (strcmp((audtHouseholds + intIndex)->strRace, "Asian") == 0)
        {
            intAsianTotal++;
            if (blnIsPoverty) intAsianPoverty++;
        }
        else if (strcmp((audtHouseholds + intIndex)->strRace, "Other") == 0)
        {
            intOtherTotal++;
            if (blnIsPoverty) intOtherPoverty++;
        }
    }

    printf("Percentage below Poverty by Race:\n\n");
    if (intCaucasianTotal > 0)
        printf("Caucasian: %.2f%%\n", (double)intCaucasianPoverty / intCaucasianTotal * 100.0);
    if (intAfricanAmericanTotal > 0)
        printf("African American: %.2f%%\n", (double)intAfricanAmericanPoverty / intAfricanAmericanTotal * 100.0);
    if (intHispanicTotal > 0)
        printf("Hispanic: %.2f%%\n", (double)intHispanicPoverty / intHispanicTotal * 100.0);
    if (intAsianTotal > 0)
        printf("Asian: %.2f%%\n", (double)intAsianPoverty / intAsianTotal * 100.0);
    if (intOtherTotal > 0)
        printf("Other: %.2f%%\n", (double)intOtherPoverty / intOtherTotal * 100.0);
}

// --------------------------------------------------------------------------------
//	Name: IsBelowPoverty
//	Abstract: Determines if a household is below poverty line based on size and income
// --------------------------------------------------------------------------------
int IsBelowPoverty(int intHouseholdSize, double dblIncome)
{
    switch (intHouseholdSize)
    {
    case 1:
        return dblIncome < POVERTY_HOUSEHOLD_1;
    case 2:
        return dblIncome < POVERTY_HOUSEHOLD_2;
    case 3:
        return dblIncome < POVERTY_HOUSEHOLD_3;
    case 4:
        return dblIncome < POVERTY_HOUSEHOLD_4;
    default: // 5 or more
        return dblIncome < POVERTY_HOUSEHOLD_5_PLUS;
    }
}

// --------------------------------------------------------------------------------
//	Name: ClearCSVFile
//	Abstract: Clears all data from the CSV file
// --------------------------------------------------------------------------------
void ClearCSVFile()
{
    char chrConfirm = 0;
    FILE* pflFile = 0;

    printf("WARNING: This will delete ALL household survey data!\n");
    printf("Are you sure you want to clear all data? (y/n): ");
    scanf(" %c", &chrConfirm);
    ClearInputBuffer();

    if (tolower(chrConfirm) == 'y')
    {
        pflFile = fopen(CSV_FILE, "w");
        if (pflFile != 0)
        {
            fclose(pflFile); // Creates empty file
            printf("All data has been cleared from the database.\n");
            printf("You can add new data using Write Mode or restart the program to reload sample data.\n");
        }
        else
        {
            printf("Error clearing data file.\n");
        }
    }
    else
    {
        printf("Clear operation cancelled.\n");
    }
}

// --------------------------------------------------------------------------------
//	Name: ClearInputBuffer
//	Abstract: Clears the input buffer to prevent issues with subsequent input
// --------------------------------------------------------------------------------
void ClearInputBuffer()
{
    int intChar = 0;
    while ((intChar = getchar()) != '\n' && intChar != EOF);
}