
#include <jni.h>
#include "Main.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
using namespace std;

// Custom hash function for vectors to use them as keys in unordered_map
struct VectorHasher
{
    template <typename T>
    size_t operator()(const vector<T> &vec) const
    {
        size_t hash_value = 0;
        for (const auto &item : vec)
        {
            hash_value ^= std::hash<T>{}(item) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
        }
        return hash_value;
    }
};

// Function to check if a given date is in the past
int checkDate(string date)
{
    // Extract day, month, and year from date string (format: DD/MM/YYYY)
    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));

    // Get the current time
    time_t t = time(NULL);
    tm *now = localtime(&t);

    // Get the current date
    int currentYear = now->tm_year + 1900;
    int currentMonth = now->tm_mon + 1;
    int currentDay = now->tm_mday;

    // Check if the given date is in the past
    if (year < currentYear || (year == currentYear && month < currentMonth) ||
        (year == currentYear && month == currentMonth && day < currentDay))
    {
        return 1; // The date is in the past
    }
    else
    {
        return 0; // The date is not in the past
    }
}

// Base class for managing problem reports
class problemReport
{
public:
    string rollno;
    // Maps to store different types of queries with roll numbers as keys
    unordered_map<string, vector<string>> query_wifi;
    unordered_map<string, vector<string>> query_hotwater;
    unordered_map<string, vector<string>> query_waterfilter;
    unordered_map<string, unordered_map<string, string>> query_roomproblem;
    unordered_map<string, vector<string>> query_other;

public:
    // Comparison function for sorting based on room numbers
    static bool cmp(const pair<string, vector<string>> &a, const pair<string, vector<string>> &b)
    {
        int i = stoi(a.second[0].substr(1, a.second[0].size() - 1));
        int j = stoi(b.second[0].substr(1, b.second[0].size() - 1));
        return i < j;
    }

    // Methods to set roll number and add different types of problems
    void set_rollno(string rollno)
    {
        this->rollno = rollno;
    }

    // Add WiFi related problem
    void add_wifi(string rollno, string roomno, string ch)
    {
        query_wifi[rollno] = vector<string>{roomno, ch, ""};
        // Sort data based on room numbers
        vector<pair<string, vector<string>>> sorted_data(query_wifi.begin(), query_wifi.end());
        sort(sorted_data.begin(), sorted_data.end(), cmp);
        query_wifi.clear();
        for (auto pair : sorted_data)
        {
            query_wifi[pair.first] = pair.second;
        }
    }

    // Add hot water related problem
    void add_hotwater(string rollno, string roomno, string ch)
    {
        query_hotwater[rollno] = vector<string>{roomno, ch, ""};
        vector<pair<string, vector<string>>> sorted_data(query_hotwater.begin(), query_hotwater.end());
        sort(sorted_data.begin(), sorted_data.end(), cmp);
        query_hotwater.clear();
        for (const auto &pair : sorted_data)
        {
            query_hotwater[pair.first] = pair.second;
        }
    }

    // Add water filter related problem
    void add_waterfilter(string rollno, string roomno)
    {
        query_waterfilter[rollno] = vector<string>{roomno, "waterfilter", ""};
        vector<pair<string, vector<string>>> sorted_data(query_waterfilter.begin(), query_waterfilter.end());
        sort(sorted_data.begin(), sorted_data.end(), cmp);
        query_waterfilter.clear();
        for (const auto &pair : sorted_data)
        {
            query_waterfilter[pair.first] = pair.second;
        }
    }

    // Add room-specific problem
    void add_roomproblem(string rollno, string roomno, string ch, string prob)
    {
        // Initialize problem categories if not exists
        if (query_roomproblem.count(roomno) == 0)
        {
            query_roomproblem[roomno]["Switchboard not working"] = "";
            query_roomproblem[roomno]["Lightbulb not working"] = "";
            query_roomproblem[roomno]["Broken furniture"] = "";
            query_roomproblem[roomno]["Water leakage"] = "";
            query_roomproblem[roomno]["Other"] = "";
            query_roomproblem[roomno]["Alert"] = "";
        }
        if (prob == "")
            query_roomproblem[roomno][ch] = ch;
        else
            query_roomproblem[roomno][ch] = prob;
    }

    // Add other miscellaneous problems
    void add_other(string rollno, string roomno, string prob)
    {
        query_other[rollno] = vector<string>{roomno, prob, ""};
        vector<pair<string, vector<string>>> sorted_data(query_other.begin(), query_other.end());
        sort(sorted_data.begin(), sorted_data.end(), cmp);
        query_other.clear();
        for (const auto &pair : sorted_data)
        {
            query_other[pair.first] = pair.second;
        }
    }
    friend class admin;
};

// Global instance of problemReport
problemReport problems;

// Admin class for managing problem resolution
class admin
{
public:
    static unordered_map<vector<string>, string, VectorHasher> resDate;

    // Set resolution date for a problem
    void set_resolve_date(string rno, string date, string choice)
    {
        resDate[{rno, choice}] = date;
    }

    // Get resolution dates
    static unordered_map<vector<string>, string, VectorHasher> *get_resdate()
    {
        return &resDate;
    }

    // Delete resolved problem
    void delete_prob(string rno, string choice, string ch)
    {
        resDate.erase({rno, choice});
        if (choice == "wifi")
        {
            problems.query_wifi.erase(rno);
        }
        else if (choice == "hotwater")
        {
            problems.query_hotwater.erase(rno);
        }
        else if (choice == "waterfilter")
        {
            problems.query_waterfilter.erase(rno);
        }
        else if (choice == "roomproblem")
        {
            problems.query_roomproblem[rno][ch] = "";
            int flag = 0;
            for (auto it : problems.query_roomproblem[rno])
            {
                if (it.second != "" && it.first != "Alert")
                    flag = 1;
            }
            if (flag == 0)
            {
                problems.query_roomproblem.erase(rno);
            }
        }
        else if (choice == "other")
        {
            problems.query_other.erase(rno);
        }
    }

    // Get queries based on admin type
    void get_queries(string adminType, string ch)
    {
        // DataCenter admin view
        if (adminType == "DataCenter" || adminType == "1")
        {
            cout << "Queries related to WiFi:\n";
            for (auto &it : problems.query_wifi)
                cout << it.first << " " << it.second[0] << " " << it.second[1] << " " << it.second[2] << endl;
        }
        // Property Manager admin view
        else if (adminType == "PropertyManager" || adminType == "2")
        {
            if (ch == "hotwater")
            {
                cout << "Queries related to Hotwater:\n";
                for (auto &it : problems.query_hotwater)
                    cout << it.first << " : " << it.second[0] << " " << it.second[1] << " " << it.second[2] << endl;
            }
            else if (ch == "waterfilter")
            {
                cout << "Queries related to Waterfilter:\n";
                for (auto &it : problems.query_waterfilter)
                    cout << it.first << " : " << it.second[0] << " " << it.second[1] << " " << it.second[2] << endl;
            }
            else if (ch == "roomproblem")
            {
                cout << "Queries related to Roomproblems:\n";
                for (auto &it : problems.query_roomproblem)
                {
                    cout << it.first << ":";
                    if (problems.query_roomproblem[it.first]["Alert"] != "")
                    {
                        cout << "(Alert!!)";
                    }
                    cout << endl;
                    for (auto &it2 : it.second)
                    {
                        if (it2.second != "" && it2.first != "Alert")
                        {
                            if (it2.first == "Other")
                                cout << "Others: ";
                            cout << it2.second << endl;
                        }
                    }
                }
            }
        }
        // SAC admin view
        else if (adminType == "SAC" || adminType == "3")
        {
            cout << "Queries:\n";
            for (auto &it : problems.query_other)
                cout << it.first << " : " << it.second[0] << it.second[1] << " " << it.second[2] << endl;
        }
    }
};

// Initialize static member
unordered_map<vector<string>, string, VectorHasher> admin::resDate;

// DataCentre class for managing WiFi related issues
class DataCentre : public admin
{
public:
    // Check resolution dates for WiFi problems
    void resolve_date_check()
    {
        for (auto &it : resDate)
        {
            if (it.first[1] == "wifi")
            {
                if (checkDate(it.second) == 1)
                    problems.query_wifi[it.first[0]][2] = "Alert!!";
            }
        }
    }
};

// Property Manager class for managing facility related issues
class PropManager : public admin
{
public:
    // Check resolution dates for property related problems
    void resolve_date_check()
    {
        for (auto &it : resDate)
        {
            if (it.first[1] != "wifi" && it.first[1] != "other")
            {
                if (checkDate(it.second) == 1)
                {
                    if (it.first[1] == "hotwater")
                        problems.query_hotwater[it.first[0]][2] = "Alert!!";
                    else if (it.first[1] == "waterfilter")
                        problems.query_waterfilter[it.first[0]][2] = "Alert!!";
                    else if (it.first[1] == "roomproblem")
                        problems.query_roomproblem[it.first[0]]["Alert"] = "Alert!!";
                }
            }
        }
    }
};

// SAC class for managing other issues
class SAC : public admin
{
public:
    // Check resolution dates for other problems
    void resolve_date_check()
    {
        for (auto &it : resDate)
        {
            if (it.first[1] == "other")
            {
                if (checkDate(it.second) == 1)
                    problems.query_other[it.first[0]][2] = "Alert!!";
            }
        }
    }
};

// WiFi class for handling WiFi related complaints
class WiFi : public problemReport
{
private:
    string ch;
    string roomno;
    int count1; // Counter for router issues
    int count2; // Counter for speed issues

public:
    // Constructor
    WiFi(string ch, string rollno, string roomno)
    {
        this->set_rollno(rollno);
        this->roomno = roomno;
        this->count1 = 0;
        this->count2 = 0;
        this->ch = ch;
    }
    void writemail(string roomno, string problem) {} // Placeholder for email notification
    
    // Add WiFi related query
    void addQuery()
    {
        if (ch == "Router not working")
        {
            problems.add_wifi(rollno, roomno, ch);
            count1++;
            writemail(this->roomno, "Router not working");
        }
        else if (ch == "Wifi speed slow")
        {
            problems.add_wifi(rollno, roomno, ch);
            count2++;
            if (count2 >= 50)
            {
                writemail("All", "Wifi speed slow");
            }
        }
        else
        {
            cout << "Please enter a valid choice" << endl;
        }
    }

    // Getter methods
    int get_count_1()
    {
        return count1;
    }

    int get_count_2()
    {
        return count2;
    }

    int get_total_count()
    {
        return count1 + count2;
    }
    friend class DataCentre;
};

// HotWater class for handling hot water related issues
class HotWater : public problemReport
{
private:
    string ch;
    string nearRoom;

public:
    HotWater(string ch, string rollno, string nearRoom)
    {
        this->set_rollno(rollno);
        this->ch = ch;
        this->nearRoom = nearRoom;
    }
    void addQuery()
    {
        if (ch == "Not getting hot water during respective timing")
        {
            problems.add_hotwater(rollno, nearRoom, ch);
        }
        else if (ch == "Geyser not working")
        {
            problems.add_hotwater(rollno, nearRoom, ch);
        }
        else
        {
            cout << "Please enter a valid choice" << endl;
        }
    }
    friend class PropManager;
};
// Class for handling water filter related issues
// Inherits from problemReport base class
class WaterFilter : public problemReport
{
private:
    string nearRoom;  // Stores the location of nearest room to the water filter

public:
    // Constructor initializes roll number and nearby room location
    WaterFilter(string rollno, string nearRoom)
    {
        this->set_rollno(rollno);
        this->nearRoom = nearRoom;
    }
    // Adds water filter query to the problems database
    void addQuery()
    {
        problems.add_waterfilter(rollno, nearRoom);
    }
    friend class PropManager;  // Allows PropManager class to access private members
};

// Base class for room-specific problems
// Inherits from problemReport base class
class RoomproblemReport : public problemReport
{
protected:
    string roomno;  // Stores the room number where problem is reported

public:
    // Constructor initializes roll number
    RoomproblemReport(string rollno)
    {
        this->set_rollno(rollno);
    }
    // Sets the room number for the problem
    void set_room(string roomno)
    {
        this->roomno = roomno;
    }
    friend class PropManager;  // Allows PropManager class to access protected members
};

// Class for switchboard related issues
// Inherits from RoomproblemReport
class SwitchBoard : public RoomproblemReport
{
public:
    // Constructor adds switchboard problem to database
    SwitchBoard(string rollno, string roomno) : RoomproblemReport(rollno)
    {
        this->set_room(roomno);
        problems.add_roomproblem(rollno, roomno, "Switchboard not working", "");
    }
    friend class PropManager;
};

// Class for light bulb related issues
// Inherits from RoomproblemReport
class LightBulb : public RoomproblemReport
{
public:
    // Constructor adds light bulb problem to database
    LightBulb(string rollno, string roomno) : RoomproblemReport(rollno)
    {
        this->set_room(roomno);
        problems.add_roomproblem(rollno, roomno, "Lightbulb not working", "");
    }
    friend class PropManager;
};

// Class for furniture damage reports
// Inherits from RoomproblemReport
class BrokenFurniture : public RoomproblemReport
{
public:
    // Constructor adds furniture problem to database
    BrokenFurniture(string rollno, string roomno) : RoomproblemReport(rollno)
    {
        this->set_room(roomno);
        problems.add_roomproblem(rollno, roomno, "Broken furniture", "");
    }
    friend class PropManager;
};

// Class for water leakage issues
// Inherits from RoomproblemReport
class WaterLeakage : public RoomproblemReport
{
public:
    // Constructor adds water leakage problem to database
    WaterLeakage(string rollno, string roomno) : RoomproblemReport(rollno)
    {
        this->set_room(roomno);
        problems.add_roomproblem(rollno, roomno, "Water leakage", "");
    }
    friend class PropManager;
};

// Class for miscellaneous room problems
// Inherits from RoomproblemReport
class RoomOther : public RoomproblemReport
{
public:
    // Constructor adds custom room problem to database
    RoomOther(string prob, string roomno, string rollno) : RoomproblemReport(rollno)
    {
        this->set_room(roomno);
        problems.add_roomproblem(rollno, roomno, "Other", prob);
    }
    friend class PropManager;
};

// Class for general problems not specific to rooms
// Inherits from problemReport base class
class Other : public problemReport
{
protected:
    string room;  // Stores related room information

public:
    // Constructor adds miscellaneous problem to database
    Other(string prob, string room)
    {
        this->set_rollno(rollno);
        problems.add_other(rollno, room, prob);
    }
    friend class SAC;  // Allows SAC class to access protected members
};
extern "C"
{
    // Processes student queries for various facility issues
    JNIEXPORT void JNICALL Java_Main_processStudentQuery(JNIEnv *env, jobject obj, jstring jrollno, jstring jroomno, int choice, jstring jch)
    {
        // Convert Java strings to C++ strings
        const char *rollnoc = env->GetStringUTFChars(jrollno, nullptr);
        const char *roomnoc = env->GetStringUTFChars(jroomno, nullptr);
        const char *chc = env->GetStringUTFChars(jch, nullptr);
        string ch(chc);
        string rollno(rollnoc);
        string roomno(roomnoc);
        
        // Release JNI string references
        env->ReleaseStringUTFChars(jrollno, rollnoc);
        env->ReleaseStringUTFChars(jroomno, roomnoc);
        env->ReleaseStringUTFChars(jch, chc);

        // Handle WiFi related issues (choice 1)
        if (choice == 1)
        {
            if (ch == "1")
                ch = "Router not working";
            else if (ch == "2")
                ch = "Wifi speed slow";
            else
            {
                ch = "";
                cout << "Error: Invalid Choice" << endl;
            }
            if (ch != "")
            {
                WiFi *problem = new WiFi(ch, rollno, roomno);
                problem->addQuery();
            }
        }
        // Handle Hot Water issues (choice 2)
        else if (choice == 2)
        {
            if (ch == "1")
                ch = "Not getting hot water during respective timing";
            else if (ch == "2")
                ch = "Geyser not working";
            else
            {
                ch = "";
                cout << "Error: Invalid Choice" << endl;
            }
            if (ch != "")
            {
                HotWater *problem = new HotWater(ch, rollno, roomno);
                problem->addQuery();
            }
        }
        // Handle Water Filter issues (choice 3)
        else if (choice == 3)
        {
            WaterFilter *problem = new WaterFilter(rollno, roomno);
            problem->addQuery();
        }
        // Handle Room problems (choice 4)
        else if (choice == 4)
        {
            if (ch == "1")
                SwitchBoard *problem = new SwitchBoard(rollno, roomno);
            else if (ch == "2")
                LightBulb *problem = new LightBulb(rollno, roomno);
            else if (ch == "3")
                BrokenFurniture *problem = new BrokenFurniture(rollno, roomno);
            else if (ch == "4")
                WaterLeakage *problem = new WaterLeakage(rollno, roomno);
            else if (ch == "5")
            {
                cout << "Enter your specific problem" << endl;
                getline(cin, ch);
                RoomOther *problem = new RoomOther(ch, roomno, rollno);
            }
            else
            {
                ch = "";
                cout << "Error: Invalid Choice" << endl;
            }
        }
        // Handle Other issues (choice 5)
        else if (choice == 5)
        {
            cout << "Enter your specific problem" << endl;
            getline(cin, ch);
            Other *problem = new Other(ch, roomno);
        }
        else
        {
            cout << "Error: Invalid Choice" << endl;
        }
    }

    // Allows admin to view queries based on admin type
    JNIEXPORT void JNICALL Java_Main_adminViewQuery(JNIEnv *env, jobject obj, jstring jadminType, jstring jch)
    {
        // Convert Java strings to C++ strings
        const char *adminTypec = env->GetStringUTFChars(jadminType, nullptr);
        string adminType(adminTypec);
        env->ReleaseStringUTFChars(jadminType, adminTypec);
        const char *chc = env->GetStringUTFChars(jch, nullptr);
        string ch(chc);
        env->ReleaseStringUTFChars(jch, chc);
        
        admin *ad = new admin();
        ad->get_queries(adminType, ch);
    }

    // Handles admin resolution date updates and problem deletion
    JNIEXPORT void JNICALL Java_Main_adminResolveDate(JNIEnv *env, jobject obj, jstring jrollno, jstring jroomno, jstring jresolvedate, jstring jadminType, jstring jchoice, jstring jch)
    {
        // Convert all Java strings to C++ strings
        const char *rollnoc = env->GetStringUTFChars(jrollno, nullptr);
        const char *resolvedatec = env->GetStringUTFChars(jresolvedate, nullptr);
        const char *adminTypec = env->GetStringUTFChars(jadminType, nullptr);
        const char *choicec = env->GetStringUTFChars(jchoice, nullptr);
        const char *chc = env->GetStringUTFChars(jch, nullptr);
        const char *roomnoc = env->GetStringUTFChars(jroomno, nullptr);
        
        string rollno(rollnoc);
        string resolvedate(resolvedatec);
        string adminType(adminTypec);
        string choice(choicec);
        string ch(chc);
        string roomno(roomnoc);

        // Release all JNI string references
        env->ReleaseStringUTFChars(jrollno, rollnoc);
        env->ReleaseStringUTFChars(jresolvedate, resolvedatec);
        env->ReleaseStringUTFChars(jadminType, adminTypec);
        env->ReleaseStringUTFChars(jchoice, choicec);
        env->ReleaseStringUTFChars(jch, chc);
        env->ReleaseStringUTFChars(jroomno, roomnoc);

        // Handle DataCenter admin operations
        if (adminType == "DataCenter" || adminType == "1")
        {
            DataCentre *ad = new DataCentre;
            if (resolvedate == "")
            {
                ad->delete_prob(rollno, choice, ch);
            }
            else
            {
                ad->set_resolve_date(rollno, resolvedate, choice);
            }
            ad->resolve_date_check();
        }
        // Handle Property Manager operations
        else if (adminType == "PropertyManager" || adminType == "2")
        {
            PropManager *ad = new PropManager();
            string rno = rollno;
            if (choice == "roomproblem")
                rno = roomno;

            if (resolvedate == "")
            {
                ad->delete_prob(rno, choice, ch);
            }
            else
            {
                ad->set_resolve_date(rno, resolvedate, choice);
            }
            ad->resolve_date_check();
        }
        // Handle SAC admin operations
        else if (adminType == "SAC" || adminType == "3")
        {
            SAC *ad = new SAC();
            if (resolvedate == "")
            {
                ad->delete_prob(rollno, choice, ch);
            }
            else
            {
                ad->set_resolve_date(rollno, resolvedate, choice);
            }
            ad->resolve_date_check();
        }
    }

    // Saves all query data to respective files at end of session
    JNIEXPORT void JNICALL Java_Main_endSessionQuery(JNIEnv *env, jobject obj)
    {
        ofstream fout;
        
        // Save WiFi queries
        fout.open("DataBaseWifi.txt");
        if (fout)
        {
            for (auto &it : problems.query_wifi)
            {
                fout << it.first << ",";
                for (auto &p : it.second)
                {
                    fout << p << ",";
                }
                fout << "end" << endl;
            }
        }
        fout.close();

        // Save Hot Water queries
        fout.open("DataBaseHotWater.txt");
        if (fout)
        {
            for (auto &it : problems.query_hotwater)
            {
                fout << it.first << ",";
                for (auto &p : it.second)
                {
                    fout << p << ",";
                }
                fout << "end" << endl;
            }
        }
        fout.close();

        // Save Water Filter queries
        fout.open("DataBaseWaterFilter.txt");
        if (fout)
        {
            for (auto &it : problems.query_waterfilter)
            {
                fout << it.first << ",";
                for (auto &p : it.second)
                {
                    fout << p << ",";
                }
                fout << "end" << endl;
            }
        }
        fout.close();

        // Save Room Problem queries
        fout.open("DataBaseRoomProblem.txt");
        if (fout)
        {
            for (auto &it : problems.query_roomproblem)
            {
                fout << it.first << ",";
                for (auto &p : it.second)
                {
                    fout << p.first << "," << p.second << ",";
                }
                fout << "end" << endl;
            }
        }
        fout.close();

        // Save Other queries
        fout.open("DataBaseOther.txt");
        if (fout)
        {
            for (auto &it : problems.query_other)
            {
                fout << it.first << ",";
                for (auto &p : it.second)
                {
                    fout << p << ",";
                }
                fout << "end" << endl;
            }
        }
        fout.close();

        // Save Resolution Dates
        fout.open("DataBaseResDate.txt");
        if (fout)
        {
            for (auto &it : admin::resDate)
            {
                fout << it.second << ",";
                for (auto &p : it.first)
                {
                    fout << p << ",";
                }
                fout << "end" << endl;
            }
        }
        fout.close();
    }

    // Loads all query data from files at start of session
    JNIEXPORT void JNICALL Java_Main_startSessionQuery(JNIEnv *env, jobject obj)
    {
        // Clear existing data
        problems.query_wifi.clear();
        problems.query_hotwater.clear();
        problems.query_waterfilter.clear();
        problems.query_roomproblem.clear();
        problems.query_other.clear();

        ifstream fin;
        string line;

        // Load WiFi queries
        fin.open("DataBaseWifi.txt");
        while (getline(fin, line))
        {
            istringstream iss(line);
            string roll;
            vector<string> t;
            getline(iss, roll, ',');
            string temp = "";
            while (getline(iss, temp, ','))
            {
                if (temp == "end")
                    break;
                t.push_back(temp);
            }
            problems.query_wifi[roll] = t;
        }
        fin.close();

        // Load Hot Water queries
        fin.open("DataBaseHotWater.txt");
        while (getline(fin, line))
        {
            istringstream iss(line);
            string roll;
            vector<string> t;
            getline(iss, roll, ',');
            string temp = "";
            while (getline(iss, temp, ','))
            {
                if (temp == "end")
                    break;
                t.push_back(temp);
            }
            problems.query_hotwater[roll] = t;
        }
        fin.close();

        // Load Water Filter queries
        fin.open("DataBaseWaterFilter.txt");
        while (getline(fin, line))
        {
            istringstream iss(line);
            string roll;
            vector<string> t;
            getline(iss, roll, ',');
            string temp = "";
            while (getline(iss, temp, ','))
            {
                if (temp == "end")
                    break;
                t.push_back(temp);
            }
            problems.query_waterfilter[roll] = t;
        }
        fin.close();

        // Load Other queries
        fin.open("DataBaseOther.txt");
        while (getline(fin, line))
        {
            istringstream iss(line);
            string roll;
            vector<string> t;
            getline(iss, roll, ',');
            string temp = "";
            while (getline(iss, temp, ','))
            {
                if (temp == "end")
                    break;
                t.push_back(temp);
            }
            problems.query_other[roll] = t;
        }
        fin.close();

        // Load Room Problem queries
        fin.open("DataBaseRoomProblem.txt");
        while (getline(fin, line))
        {
            istringstream iss(line);
            string room;
            unordered_map<string, string> t;
            getline(iss, room, ',');
            string prob;
            while (getline(iss, prob, ','))
            {
                if (prob != "end")
                {
                    t[room] = prob;
                    getline(iss, room, ',');
                }
            }
            problems.query_roomproblem[room] = t;
        }
        fin.close();

        // Load Resolution Dates
        fin.open("DataBaseResDate.txt");
        if (!fin)
        {
            cerr << "Error: Unable to open DataBaseResDate.txt" << endl;
            return;
        }

        while (getline(fin, line))
        {
            istringstream iss(line);
            string temp;
            vector<string> t;
            string str;
            getline(iss, str, ',');
            while (getline(iss, temp, ','))
            {
                if (temp == "end")
                    break;
                t.push_back(temp);
            }
            if (!t.empty())
            {
                admin::resDate[t] = str;
            }
            else
            {
                cerr << "Warning: Malformed line in DataBaseResDate.txt: " << line << endl;
            }
        }
        fin.close();
    }
}