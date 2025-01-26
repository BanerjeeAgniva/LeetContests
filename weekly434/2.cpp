// https://leetcode.com/contest/weekly-contest-434/problems/count-mentions-per-user/description/?slug=count-mentions-per-user&region=global_v2
/*
You are given an integer numberOfUsers representing the total number of users and an array events of size n x 3.

Each events[i] can be either of the following two types:

Message Event: ["MESSAGE", "timestampi", "mentions_stringi"]
This event indicates that a set of users was mentioned in a message at timestampi.
The mentions_stringi string can contain one of the following tokens:
id<number>: where <number> is an integer in range [0,numberOfUsers - 1]. There can be multiple ids separated by a single whitespace and may contain duplicates.
This can mention even the offline users.

ALL: mentions all users.
HERE: mentions all online users.
Offline Event: ["OFFLINE", "timestampi", "idi"]

This event indicates that the user idi had become offline at timestampi for 60 time units. The user will automatically be online again at time timestampi + 60.
Return an array mentions where mentions[i] represents the number of mentions the user with id i has across all MESSAGE events.

All users are initially online, and if a user goes offline or comes back online, their status change is processed before handling any message event that occurs at the same timestamp.

Note that a user can be mentioned multiple times in a single message event, and each mention should be counted separately.

Example 1:

Input: numberOfUsers = 2, events = [["MESSAGE","10","id1 id0"],["OFFLINE","11","0"],["MESSAGE","71","HERE"]]

Output: [2,2]

Explanation:

Initially, all users are online.

At timestamp 10, id1 and id0 are mentioned. mentions = [1,1]

At timestamp 11, id0 goes offline.

At timestamp 71, id0 comes back online and "HERE" is mentioned. mentions = [2,2]

Example 2:

Input: numberOfUsers = 2, events = [["MESSAGE","10","id1 id0"],["OFFLINE","11","0"],["MESSAGE","12","ALL"]]

Output: [2,2]

Explanation:

Initially, all users are online.

At timestamp 10, id1 and id0 are mentioned. mentions = [1,1]

At timestamp 11, id0 goes offline.

At timestamp 12, "ALL" is mentioned. This includes offline users, so both id0 and id1 are mentioned. mentions = [2,2]

Example 3:

Input: numberOfUsers = 2, events = [["OFFLINE","10","0"],["MESSAGE","12","HERE"]]

Output: [0,1]

Explanation:

Initially, all users are online.

At timestamp 10, id0 goes offline.

At timestamp 12, "HERE" is mentioned. Because id0 is still offline, they will not be mentioned. mentions = [0,1]
*/
//-------------------
/*
BEFORE COMPARATOR FUNCTION
{
    {"MESSAGE", "10", "user1"},
    {"OFFLINE", "10", "2"},
    {"MESSAGE", "10", "user2"},
    {"OFFLINE", "5", "1"},
    {"MESSAGE", "5", "user3"},
    {"MESSAGE", "15", "ALL"}
}

  */
bool eventComparator(const vector<string>& a, const vector<string>& b) {
    int timeA = stoi(a[1]), timeB = stoi(b[1]);
    if (timeA != timeB) 
        return timeA < timeB;  // Sort by timestamp (ascending)
    return a[0] == "OFFLINE";  // Prioritize "OFFLINE" events over "MESSAGE" events if timestamps are the same
}

/*
AFTER COMPARATOR FUNCTION
{
    {"OFFLINE", "5", "1"},
    {"MESSAGE", "5", "user3"},  -----> IF TIMESTAMP IS SAME, THEN PRIORITIZE OFFLINE TO BE EARLIER 
    {"OFFLINE", "10", "2"},
    {"MESSAGE", "10", "user1"},
    {"MESSAGE", "10", "user2"},
    {"MESSAGE", "15", "ALL"}
}
*/

vector<int> countMentions(int numberOfUsers, vector<vector<string>>& events) {
        vector<int> mentions(numberOfUsers, 0);
        unordered_map<int, int> offline_until;
        sort(events.begin(), events.end(), eventComparator);

        for (const auto& event : events) 
        {
            string event_type = event[0];
            int timestamp = stoi(event[1]);
            string details = event[2];

            if (event_type == "OFFLINE") 
            {
                int user_id = stoi(details);
                offline_until[user_id] = timestamp + 60;
            } 
            else // MESSAGE
            {
                vector<int> mentioned_users;
                if (details == "ALL") // ALL: mentions all users.
                {
                    for (int i = 0; i < numberOfUsers; i++) 
                      mentioned_users.push_back(i);
                } 
                else if (details == "HERE")  // HERE: mentions all online users.
                {
                    for (int i = 0; i < numberOfUsers; i++) 
                    {
                        if (offline_until.find(i) == offline_until.end() || offline_until[i] <= timestamp) 
                        {  // MEMBER IS ONLINE                                MEMBERS OFFLINE TIME IS OVER --> SO NOW HE IS ONLINE
                            mentioned_users.push_back(i);
                        }
                    }
                } 
                else   // ID1 ID0
                {
                    stringstream ss(details);
                    string token;
                    while (ss >> token) 
                    {
                        mentioned_users.push_back(stoi(token.substr(2)));
                    }
                }
                
                for (int user : mentioned_users) 
                {
                    mentions[user]++;
                }
            }
        }
        return mentions;
    }


/*
HOW DOES STRINGSTREAM WORK?

#include <iostream>
#include <sstream>  // Include stringstream
#include <string>

using namespace std;

int main() {
    string input = "hello world 42";
    stringstream ss(input);  // Create a stringstream from the input string
    string word;
    
    // Extract words from the string
    while (ss >> word) {
        cout << word << endl;
    }
    
    return 0;
}


hello
world
42

*/
