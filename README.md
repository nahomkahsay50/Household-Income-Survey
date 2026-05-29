# Household Income Survey — Data Collection and Analysis

A C console application that collects and analyzes household income survey data for the Cincinnati/Northern Kentucky area. Demonstrates dynamic memory allocation, pointer usage, struct arrays, CSV file I/O, and modular program design.

---

## Overview

The program operates in two modes — **Write Mode** for data entry and **Display Mode** for statistical analysis. In Write Mode, validated survey data is appended to a CSV file. In Display Mode, the CSV is loaded into a dynamically allocated structure array and the user can run nine different statistical queries from a menu.

---

## Files

| File | Description |
|---|---|
| `SET151FinalProject_NK/` | C source project folder containing `FinalProject.c` |
| `SET151FinalProject_NK.sln` | Visual Studio solution file |

---

## Tech Stack

- **C** — core language
- **Visual Studio** — IDE and build environment
- Standard C libraries: `stdio.h`, `stdlib.h`, `string.h`, `ctype.h`
- Dynamic memory allocation via `malloc` / `free`
- CSV file I/O (`fopen`, `fgets`, `fprintf`, `strtok`) for persistent data storage

---

## How It Works

### Startup

```
=== Census Bureau Household Income Survey System ===

=== Main Menu ===
1. Write Mode (Enter new survey data)
2. Display Mode (View and analyze existing data)
3. Exit Program
```

---

### Write Mode — Data Entry

Collects the following survey fields in order, using numbered menus where applicable. All input is validated before saving.

| Field | Input Method | Validation |
|---|---|---|
| Date of survey | Manual text entry | Required |
| State | Menu (1=Ohio, 2=Kentucky) | Required |
| County | Menu (based on state) | Required |
| Race of head of household | Menu (1–5) | Required |
| Number in household | Numeric | Required, must be > 0 |
| Yearly household income | Numeric (`double`) | Required, must be > 0 |

**Counties by state:**
- Ohio: Hamilton, Butler
- Kentucky: Boone, Kenton

**Race options:** Caucasian, African American, Hispanic, Asian, Other

After each entry the user is asked whether to enter another household. All data is **appended** to `household_survey_data.csv` — no prior data is overwritten between sessions.

---

### Display Mode — Analysis Menu

On entry, the CSV is loaded into a **dynamically allocated array of structs** (`malloc`). The user selects from the following menu, which repeats after each query:

| Option | Query |
|---|---|
| A | Total households surveyed (overall) |
| B | Total households surveyed per county and state |
| C | Total households surveyed per race |
| D | Average household income (overall) |
| E | Average household income by state and county |
| F | Average household income by race |
| G | Percentage below poverty (overall) |
| H | Percentage below poverty by county and state |
| I | Percentage below poverty by race |
| K | Clear all data (wipe CSV with confirmation prompt) |
| J | Return to main menu |

Memory is properly freed via `free()` before returning to the main menu.

**Example output (Option E):**
```
Average Household Income by County and State:

Ohio: $35,000.00
   Hamilton: $40,000.00
   Butler: $28,000.00

Kentucky: $37,500.00
   Boone: $40,000.00
   Kenton: $35,000.00
```

---

### Poverty Thresholds

Poverty status is determined by the `IsBelowPoverty()` function using the following thresholds:

| Household Size | Income Threshold |
|---|---|
| 1 person | Less than $12,000 |
| 2 people | Less than $18,000 |
| 3 people | Less than $25,000 |
| 4 people | Less than $30,000 |
| 5 or more | Less than $40,000 |

---

## Program Structure

The program is fully modular — every distinct operation has its own function:

| Function | Purpose |
|---|---|
| `main()` | Entry point, main menu loop |
| `WriteMode()` | Data entry loop |
| `DisplayMode()` | Analysis menu loop with memory management |
| `GetHouseholdData()` | Collects all fields for one survey record |
| `ValidateAndGetState()` | State selection with loop validation |
| `ValidateAndGetCounty()` | County selection based on state |
| `ValidateAndGetRace()` | Race selection menu |
| `ValidateHouseholdSize()` | Validates household size > 0 |
| `ValidateYearlyIncome()` | Validates income > 0 |
| `SaveHouseholdToCSV()` | Appends one record to CSV |
| `LoadHouseholdsFromCSV()` | Dynamically allocates and loads full CSV |
| `DeleteHouseholdArray()` | Frees allocated memory, resets pointer |
| `DisplayAnalysisMenu()` | Prints analysis menu |
| `ProcessDisplayOption()` | Routes menu selection to analysis functions |
| `DisplayTotalHouseholds()` | Query A |
| `DisplayTotalHouseholdsByCounty()` | Query B |
| `DisplayTotalHouseholdsByRace()` | Query C |
| `DisplayAverageHouseholdIncome()` | Query D |
| `DisplayAverageIncomeByCounty()` | Query E |
| `DisplayAverageIncomeByRace()` | Query F |
| `DisplayPercentageBelowPoverty()` | Query G |
| `DisplayPercentageBelowPovertyByCounty()` | Query H |
| `DisplayPercentageBelowPovertyByRace()` | Query I |
| `IsBelowPoverty()` | Poverty threshold utility function |
| `ClearCSVFile()` | Wipes CSV with confirmation |
| `ClearInputBuffer()` | Flushes stdin between inputs |

---

## Key Technical Concepts Demonstrated

- **Pointers** — used throughout: struct pointer parameters (`udtHouseholdDataType*`), double pointers for dynamic array management (`udtHouseholdDataType**`), pointer arithmetic for array access (`audtHouseholds + intIndex`)
- **Dynamic memory allocation** — `malloc` allocates the struct array in `LoadHouseholdsFromCSV()`; `free()` releases it in `DeleteHouseholdArray()`
- **CSV file I/O** — append mode write in `WriteMode`, full file read with `strtok` parsing in `DisplayMode`
- **Structs** — `udtHouseholdDataType` holds all six fields per survey record
- **Modular design** — 25 functions with single responsibilities and descriptive names

---

## Building & Running

### In Visual Studio
1. Open `SET151FinalProject_NK.sln`
2. Press **F5** to build and run

### From the Command Line
```
cl SET151FinalProject_NK\FinalProject.c /Fe:HouseholdSurvey.exe
HouseholdSurvey.exe
```

> The program reads and writes `household_survey_data.csv` in the working directory. The file is created automatically on first use.

---

## Author

**Nahom Kahsay** — SET-151-400, C Programming Final Project
