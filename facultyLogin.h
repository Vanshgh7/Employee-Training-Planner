#include <iostream>
#include <fstream>
#include <string>
#include <cctype>       // for isalpha
#include <algorithm>    // for transform

using namespace std;

// Track schedule status and validation
string scheduleStatus;
static int scheduleFlag = 0;

class Faculty
{
public:
    string facultyId;

    // ---------------- VIEW SCHEDULE ----------------
    void showSchedule()
    {
        string enteredName;
        int scheduleCount = 1;

        cout << "\nEnter your name: ";
        cin >> enteredName;

        // Validate name (only alphabets allowed)
        for (char c : enteredName)
        {
            if (!isalpha(c))
            {
                cout << "\nInvalid name! Please use alphabets only.\n";
                return showSchedule();
            }
        }

        // Prevent viewing schedules of other faculty members
        if (facultyId != enteredName)
        {
            cout << "\nAccess denied! You can only view your own schedule.\n";
            return showSchedule();
        }

        // Read schedules from file
        ifstream scheduleFile("./../data/trainerallotment.csv");
        while (scheduleFile >> batchID >> technology >> startDate >> endDate 
                            >> totalDays >> venue >> participants >> month >> trainerName)
        {
            // Convert both to lowercase for comparison
            transform(trainerName.begin(), trainerName.end(), trainerName.begin(), ::tolower);
            transform(enteredName.begin(), enteredName.end(), enteredName.begin(), ::tolower);

            if (trainerName == enteredName)
            {
                cout << "\nYour Schedule " << scheduleCount << ":" << endl;
                cout << "Batch ID               : " << batchID << endl;
                cout << "Technology             : " << technology << endl;
                cout << "Start Date             : " << startDate << endl;
                cout << "End Date               : " << endDate << endl;
                cout << "Duration (Days)        : " << totalDays << endl;
                cout << "Venue                  : " << venue << endl;
                cout << "Participants           : " << participants << endl;
                cout << "Month                  : " << month << endl;

                scheduleCount++;
                scheduleFlag = 1;
            }
        }
        scheduleFile.close();

        if (scheduleFlag == 1)
        {
            cout << "\nDo you accept this schedule? (yes/no): ";
            cin >> scheduleStatus;
            cout << "\nYour response has been recorded.\n";

            scheduleFlag = 0;  // reset flag

            // Menu after viewing schedule
            int option;
            cout << "\nChoose your next action:\n"
                 << "1 -> Back to Dashboard\n"
                 << "2 -> Request Schedule Cancellation\n"
                 << "3 -> Exit\n"
                 << "Enter choice: ";
            cin >> option;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(512, '\n');
                cout << "\nInvalid input. Returning to dashboard...\n";
                return facultyDashboard();
            }

            switch (option)
            {
            case 1: facultyDashboard(); break;
            case 2: cancelSchedule(); break;
            case 3: cout << "Goodbye!\n"; exit(0);
            default: cout << "\nInvalid choice. Redirecting to dashboard...\n"; facultyDashboard();
            }
        }
        else
        {
            cout << "\nNo schedule has been assigned yet. Please check back later.\n";
            exit(0);
        }
    }

    // ---------------- CANCEL SCHEDULE ----------------
    void cancelSchedule()
    {
        if (scheduleStatus == "yes" || scheduleStatus == "y" ||
            scheduleStatus == "YES" || scheduleStatus == "Y")
        {
            cout << "\nYou already accepted this schedule. Cancellation not allowed.\n";

            string choice;
            cout << "\nDo you want to quit? (Y/N): ";
            cin >> choice;

            if (choice == "N" || choice == "n" || choice == "no" || choice == "NO")
                facultyDashboard();
            else
            {
                cout << "Goodbye!\n";
                exit(0);
            }
        }
        else
        {
            string answer;
            cout << "Do you want to cancel your schedule? (yes/no): ";
            cin >> answer;

            if (answer == "yes" || answer == "y" || answer == "YES" || answer == "Y")
            {
                scheduleStatus = "yes";
                cout << "\nYour cancellation request has been submitted.\nThank you!\n";

                string choice;
                cout << "\nDo you want to quit? (Y/N): ";
                cin >> choice;

                if (choice == "N" || choice == "n" || choice == "no" || choice == "NO")
                    facultyDashboard();
                else
                {
                    cout << "Goodbye!\n";
                    exit(0);
                }
            }
            else
            {
                string choice;
                cout << "\nDo you want to quit? (Y/N): ";
                cin >> choice;

                if (choice == "N" || choice == "n" || choice == "no" || choice == "NO")
                    facultyDashboard();
                else
                {
                    cout << "Goodbye!\n";
                    exit(0);
                }
            }
        }
    }

    // ---------------- DASHBOARD ----------------
    void facultyDashboard()
    {
        cout << "\n================ Faculty Dashboard ================\n";
        cout << "1 -> View Schedule\n";
        cout << "2 -> Cancel Schedule\n";
        cout << "3 -> Exit\n";
        cout << "===================================================\n";

        int option;
        cout << "Enter your choice: ";
        cin >> option;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(512, '\n');
            cout << "\nInvalid input! Please enter a number.\n";
            return facultyDashboard();
        }

        switch (option)
        {
        case 1: showSchedule(); break;
        case 2: cancelSchedule(); break;
        case 3: cout << "Goodbye!\n"; exit(0);
        default: cout << "\nInvalid choice! Try again.\n"; facultyDashboard();
        }
    }
};
