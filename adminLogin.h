#include <iostream>     // Input/output
#include <fstream>      // File handling
#include <sstream>      // String stream for parsing
#include <string>       // Strings
#include <vector>       // Dynamic arrays

using namespace std;

// ------------------- DATE VALIDATION -------------------
bool isValidDate(int day, int month, int year)
{
    if (year < 1000 || year > 2030) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    // Invalid dates handling
    if (day == 31 && (month == 2 || month == 4 || month == 6 || month == 9 || month == 11)) return false;
    if (day == 30 && month == 2) return false;

    // Leap year check
    if (month == 2 && day == 29) {
        if (year % 400 == 0) return true;
        if (year % 100 == 0) return false;
        if (year % 4 == 0) return true;
        return false;
    }
    return true;
}

const int daysTillMonth[12] = {31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

int leapYearDays(int dateArr[])
{
    int years = dateArr[2];
    if (dateArr[1] <= 2) years--;
    return (years / 4) - (years / 100) + (years / 400);
}

int countDaysBetween(int date1[], int date2[])
{
    long days1 = date1[2] * 365 + daysTillMonth[date1[1]] + date1[0] + leapYearDays(date1);
    long days2 = date2[2] * 365 + daysTillMonth[date2[1]] + date2[0] + leapYearDays(date2);
    return abs(days1 - days2);
}

// ------------------- STRING VALIDATION -------------------
bool isAlphaOnly(const string &str)
{
    for (char ch : str) {
        if (!isalpha(ch)) {
            cout << "\nPlease enter letters only.\n";
            return false;
        }
    }
    return true;
}

// ------------------- GLOBAL VARIABLES -------------------
int scheduleId, totalDays, participants;
string techName, startDate, endDate, venue, trainingMonth, trainerName;

// ------------------- ADMIN CLASS -------------------
class admin
{
public:

    // ---------- Create new schedule ----------
    void createSchedule()
    {
        cout << "\nFill in the following details to create a new training schedule:\n";
        cout << "\n1. Batch ID         2. Technology Name"
             << "\n3. Start Date       4. End Date"
             << "\n5. Number of Days   6. Venue"
             << "\n7. Participants     8. Month\n";

        cout << "\nBatch ID: ";
        cin >> scheduleId;
        if (cin.fail()) {
            cin.clear(); cin.ignore(512, '\n');
            cout << "\nInvalid input! Try again.\n";
            return createSchedule();
        }
        cin.ignore();

        cout << "Technology: ";
        getline(cin, techName);
        if (!isAlphaOnly(techName)) return createSchedule();

        cout << "Start Date (DD-MM-YYYY): ";
        getline(cin, startDate);

        string temp; int arr[3], idx = 0;
        stringstream ss(startDate);
        while (getline(ss, temp, '-')) arr[idx++] = stoi(temp);

        if (!isValidDate(arr[0], arr[1], arr[2])) {
            cout << "\nInvalid start date! Try again.\n";
            return createSchedule();
        }
        int date1[] = {arr[0], arr[1], arr[2]};

        cout << "End Date (DD-MM-YYYY): ";
        getline(cin, endDate);
        idx = 0; stringstream ss2(endDate);
        while (getline(ss2, temp, '-')) arr[idx++] = stoi(temp);

        if (!isValidDate(arr[0], arr[1], arr[2])) {
            cout << "\nInvalid end date! Try again.\n";
            return createSchedule();
        }
        int date2[] = {arr[0], arr[1], arr[2]};

        cout << "Number of Days: ";
        cin >> totalDays;
        if (cin.fail()) {
            cin.clear(); cin.ignore(512, '\n');
            cout << "\nInvalid input! Try again.\n";
            return createSchedule();
        }

        int calculatedDays = countDaysBetween(date1, date2);
        if (calculatedDays != totalDays) {
            cout << "\nMismatch! Number of days should be: " << calculatedDays << "\n";
            return createSchedule();
        }
        cin.ignore();

        cout << "Venue: ";
        getline(cin, venue);
        if (!isAlphaOnly(venue)) return createSchedule();

        cout << "Participants: ";
        cin >> participants;
        if (cin.fail()) {
            cin.clear(); cin.ignore(512, '\n');
            cout << "\nInvalid input! Try again.\n";
            return createSchedule();
        }

        cout << "Month: ";
        cin >> trainingMonth;
        if (!isAlphaOnly(trainingMonth)) return createSchedule();

        ofstream fout("./../data/schedule.csv", ios::app);
        fout << scheduleId << " " << techName << " " << startDate << " " << endDate << " "
             << totalDays << " " << venue << " " << participants << " " << trainingMonth << endl;
        fout.close();

        cout << "\nSchedule created successfully!\n";

        string opt;
        cout << "\nDo you want to create another schedule? (yes/no): ";
        cin >> opt;
        if (opt == "yes" || opt == "y") createSchedule();
        else adminModules();
    }

    // ---------- Update existing schedule ----------
    void updateSchedule()
    {
        fstream file("./../data/schedule.csv", ios::in);
        vector<string> data;
        string line, word;
        int recordCount = 0;

        while (getline(file, line)) recordCount++;
        file.close();

        file.open("./../data/schedule.csv", ios::in);
        for (int i = 0; i < recordCount; i++) {
            getline(file, line);
            stringstream ss(line);
            while (getline(ss, word, ' ')) data.push_back(word);
        }
        file.close();

        string id;
        cout << "\nEnter Batch ID to update: ";
        cin >> id;

        bool found = false;
        for (int i = 0; i < data.size(); i += 8) {
            if (data[i] == id) {
                found = true;
                cout << "Technology: "; cin >> data[i+1];
                cout << "Start Date: "; cin >> data[i+2];
                cout << "End Date: "; cin >> data[i+3];
                cout << "Days: "; cin >> data[i+4];
                cout << "Venue: "; cin >> data[i+5];
                cout << "Participants: "; cin >> data[i+6];
                cout << "Month: "; cin >> data[i+7];
            }
        }

        if (!found) {
            cout << "\nNo record found for Batch ID " << id << ". Returning to Dashboard.\n";
            return adminModules();
        }

        file.open("./../data/schedule.csv", ios::out | ios::trunc);
        for (int i = 0; i < data.size(); i += 8) {
            file << data[i] << " " << data[i+1] << " " << data[i+2] << " "
                 << data[i+3] << " " << data[i+4] << " " << data[i+5] << " "
                 << data[i+6] << " " << data[i+7] << endl;
        }
        file.close();

        cout << "\nSchedule updated successfully!\n";
        string opt;
        cout << "Update another? (yes/no): ";
        cin >> opt;
        if (opt == "yes" || opt == "y") updateSchedule();
        else adminModules();
    }

    // ---------- Allot trainer ----------
    void allotTrainer()
    {
        cout << "\nFaculty Name: ";
        cin >> trainerName;
        if (!isAlphaOnly(trainerName)) return allotTrainer();

        string tech;
        cout << "Technology: ";
        cin >> tech;
        if (!isAlphaOnly(tech)) return allotTrainer();

        ifstream fin("./../data/schedule.csv");
        ofstream fout("./../data/trainerallotment.csv", ios::app);
        bool assigned = false;

        while (fin >> scheduleId >> techName >> startDate >> endDate >> totalDays >> venue >> participants >> trainingMonth) {
            string lowerTech = techName, searchTech = tech;
            transform(lowerTech.begin(), lowerTech.end(), lowerTech.begin(), ::tolower);
            transform(searchTech.begin(), searchTech.end(), searchTech.begin(), ::tolower);

            if (lowerTech == searchTech) {
                fout << scheduleId << " " << techName << " " << startDate << " "
                     << endDate << " " << totalDays << " " << venue << " "
                     << participants << " " << trainingMonth << " " << trainerName << endl;
                assigned = true;
            }
        }
        fin.close(); fout.close();

        if (!assigned) cout << "\nNo matching technology found.\n";

        string opt;
        cout << "Assign another trainer? (yes/no): ";
        cin >> opt;
        if (opt == "yes" || opt == "y") allotTrainer();
        else adminModules();
    }

    // ---------- Generate report ----------
    void generateReport()
    {
        cout << "\nEnter Month: ";
        cin >> trainingMonth;
        if (!isAlphaOnly(trainingMonth)) return generateReport();

        cout << "Enter Technology: ";
        cin >> techName;
        if (!isAlphaOnly(techName)) return generateReport();

        ifstream fin("./../data/schedule.csv");
        int count = 0;

        while (fin >> scheduleId >> techName >> startDate >> endDate >> totalDays >> venue >> participants >> trainingMonth) {
            string mon = trainingMonth, tech = techName;
            transform(mon.begin(), mon.end(), mon.begin(), ::tolower);
            transform(tech.begin(), tech.end(), tech.begin(), ::tolower);

            if (mon == trainingMonth && tech == techName) {
                cout << "\nSchedule " << (++count) << ":\n"
                     << "Batch ID        : " << scheduleId
                     << "\nTechnology      : " << techName
                     << "\nStart Date      : " << startDate
                     << "\nEnd Date        : " << endDate
                     << "\nDays            : " << totalDays
                     << "\nVenue           : " << venue
                     << "\nParticipants    : " << participants
                     << "\nMonth           : " << trainingMonth << "\n";
            }
        }
        fin.close();

        if (count == 0) cout << "\nNo schedules found for the given month/technology.\n";

        string opt;
        cout << "Continue session? (yes/no): ";
        cin >> opt;
        if (opt == "yes" || opt == "y") adminModules();
        else { cout << "Goodbye!\n"; exit(0); }
    }

    // ---------- Admin Dashboard ----------
    void adminModules()
    {
        cout << "\n--- Admin Dashboard ---\n"
             << "1. Create Schedule\n"
             << "2. Update Schedule\n"
             << "3. Allot Trainer\n"
             << "4. Generate Report\n"
             << "5. Exit\n";

        int choice;
        cout << "\nYour choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear(); cin.ignore(512, '\n');
            cout << "\nInvalid input! Try again.\n";
            return adminModules();
        }

        switch (choice) {
            case 1: createSchedule(); break;
            case 2: updateSchedule(); break;
            case 3: allotTrainer(); break;
            case 4: generateReport(); break;
            case 5:
                cout << "\n1. Back to Dashboard\n2. Exit\nChoice: ";
                int next; cin >> next;
                if (next == 1) adminModules();
                else { cout << "Goodbye!\n"; exit(0); }
                break;
            default:
                cout << "\nInvalid option! Try again.\n";
                adminModules();
        }
    }
};
