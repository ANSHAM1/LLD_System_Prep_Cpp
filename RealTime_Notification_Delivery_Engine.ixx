/*
===============================================================
Day 6 - C++ Systems Engineering Track

Difficulty : 10/10

Expected Time : 5-7 Hours

Problem

Real-Time Notification Delivery Engine

===============================================================

Background

You are implementing the backend notification engine used by
a messaging platform similar to Discord, Slack or Microsoft Teams.

Millions of users generate notifications every minute.

Notifications have different priorities.

Users may be offline.

Notifications may expire.

Some notifications should be delayed.

The scheduler continuously decides which notification
should be delivered next.

Everything is in-memory.

===============================================================
Notification Format

NOTIFY|
NotificationID|
UserID|
Channel|
Priority|
ExpireTimestamp|
DelayUntil|
Payload

Example

NOTIFY|
101|
U1002|
EMAIL|
9|
1719009000|
1719000100|
Your model training has completed.

===============================================================
Supported Channels

EMAIL

SMS

PUSH

IN_APP

WEBHOOK

===============================================================
Operations

ADD NOTIFICATION

Adds a notification.

--------------------------------------------------

PROCESS NEXT

Deliver the highest priority notification
that satisfies

CurrentTime >= DelayUntil

AND

CurrentTime < ExpireTimestamp

AND

User is Online

--------------------------------------------------

SET USER ONLINE UserID

--------------------------------------------------

SET USER OFFLINE UserID

--------------------------------------------------

ADVANCE TIME Seconds

Move simulated system clock.

Expired notifications should automatically disappear.

Delayed notifications may become executable.

--------------------------------------------------

CANCEL NotificationID

Only waiting notifications may be cancelled.

--------------------------------------------------

SHOW USER UserID

Waiting Notifications

Delivered Notifications

Expired Notifications

--------------------------------------------------

SHOW CHANNEL EMAIL

Display

Waiting

Delivered

Expired

--------------------------------------------------

SHOW STATS

Current Time

Waiting

Delivered

Expired

Cancelled

Online Users

Offline Users

===============================================================
Scheduling Rules

Highest Priority

↓

Oldest Creation Time

↓

Smallest Notification ID

===============================================================
Invalid Cases

Duplicate Notification ID

Negative timestamps

Unknown channel

Expired before creation

Delay greater than expiration

Cancel delivered notification

Cancel expired notification

Cancel missing notification

===============================================================
Bonus

1.

Support notification retry.

Each notification has

Retry Count

Retry Delay

Failed deliveries
must automatically return
to the scheduler.

--------------------------------------------------

2.

Rate limiting.

A user can receive

Maximum

5 notifications

per simulated minute.

--------------------------------------------------

3.

Notification Groups

Collapse

20 notifications

↓

"You have 20 new messages."

--------------------------------------------------

4.

Persistence

SAVE

LOAD

===============================================================
Expected Design

No class names are provided.

You must decide

- Objects
- Relationships
- Ownership
- Data structures
- Scheduling strategy

===============================================================
Restrictions

Do not use threads.

Do not sleep.

Everything must be simulated.

===============================================================
*/

export module RealTime_Notification_Delivery_Engine;

import <iostream>;
import <string>;
import <vector>;
import <queue>;
import <sstream>;
import <unordered_map>;
import <unordered_set>;
import <thread>;
import <chrono>;
import <limits>;
import <atomic>;
using namespace std;

static string strip(string str) {
	size_t first = str.find_first_not_of("\t\n\r\f\v");

	if (first == string::npos) {
		return "";
	}

	size_t last = str.find_last_not_of("\t\n\r\f\v");

	return str.substr(first, last - first + 1);
}

static void split(vector<string>& tokens, string str) {
	istringstream iss(strip(str));
	string token;

	while (getline(iss, token)) {
		if(strip(token).size() > 0)
			tokens.push_back(strip(token));
	}
}

static void split(vector<string>& tokens, string str, char d) {
	istringstream iss(strip(str));
	string token;

	while (getline(iss, token, d)) {
		if (strip(token).size() > 0)
			tokens.push_back(strip(token));
	}
}

class Notification {
private:
	string Payload;

public:
	size_t NotificationID;
	size_t UserID;
	string Channel;
	size_t Priority;
	size_t CreatedTimestamp;
	size_t ExpireTime;
	size_t DelayUntil;

	Notification() = default;

	Notification(vector<string>& tkns, size_t Time) :
		NotificationID(stoull(tkns[1])), UserID(stoll(tkns[2])), Channel(tkns[3]), Priority(stoll(tkns[4])), CreatedTimestamp(Time), 
		ExpireTime(stoll(tkns[5])), DelayUntil(stoll(tkns[6])), Payload(tkns[7]) {}

	string getPayload() const {
		return Payload;
	}

	void show() const {
		cout << "\n==================================================\n";
		cout << "               Notification Details\n";
		cout << "==================================================\n";

		cout << "Notification ID   : " << NotificationID << '\n';
		cout << "User ID           : " << UserID << '\n';
		cout << "Channel           : " << Channel << '\n';
		cout << "Priority          : " << Priority << '\n';
		cout << "Created Time      : " << CreatedTimestamp << " sec\n";
		cout << "Expire Time       : " << ExpireTime << " sec\n";
		cout << "Delay Until       : " << DelayUntil << " sec\n";
		cout << "Payload           : " << Payload << '\n';

		cout << "==================================================\n";
	}
};

export class NDEngine {
private:
	unordered_set<string> ValidChannel = { "EMAIL", "SMS", "PUSH", "IN_APP", "WEBHOOK" };

	unordered_map<size_t, Notification> NotifMap;
	unordered_map<size_t, bool> Users;

	priority_queue<pair<size_t, size_t>> NotifiQueue;
	priority_queue<pair<size_t, size_t>> NotifiQueuePending;


	atomic<size_t> TIME{ 0 };
	atomic<bool> running{ true };

	thread timerThread;
	thread cleanerThread;

	void removeNotif() {
		while (running) {
			std::this_thread::sleep_for(std::chrono::seconds(1));

			for (auto Nit = NotifMap.begin(); Nit != NotifMap.end();) {
				auto Uit = Users.find(Nit->first);
				if (Uit == Users.end() || !Uit->second) {
					size_t cts = Nit->second.CreatedTimestamp;
					size_t ets = Nit->second.ExpireTime;

					if (TIME >= cts + ets) {
						cout << "Notification Expired : " << Nit->second.NotificationID << endl << endl;
						Nit = NotifMap.erase(Nit);
						continue;
					}
				}
				++Nit;
			}
		}
	}

	void timer() {
		while (running) {
			std::this_thread::sleep_for(std::chrono::seconds(1));

			if (++TIME >= numeric_limits<size_t>::max())
				TIME = 0;
		}
	}

	void restorePending()
	{
		while (!NotifiQueuePending.empty()) {
			NotifiQueue.push(NotifiQueuePending.top());
			NotifiQueuePending.pop();
		}
	}

public:
	NDEngine() {
		timerThread = thread(&NDEngine::timer, this);
		cleanerThread = thread(&NDEngine::removeNotif, this);
	}

	~NDEngine() {
		running = false;

		if (timerThread.joinable())
			timerThread.join();

		if (cleanerThread.joinable())
			cleanerThread.join();
	}

	size_t getTime() const {
		return TIME.load();
	}

	void addNotification(string str) {
		if (str.size() <= 0) return;

		vector<string> tokens;
		split(tokens, str, '|');

		if (tokens.size() != 8) {
			cout << "Invalid Tokens Size or Missing Data" <<endl;
			return;
		}
		if (ValidChannel.find(tokens[3]) == ValidChannel.end()) {
			cout << "Invalid Channel Type" << endl;
			return;
		}

		NotifMap[stoull(tokens[1])] = Notification(tokens, TIME);
		Users[stoll(tokens[2])] = false;

		NotifiQueue.push({ stoull(tokens[4]), stoull(tokens[1]) });

		cout << "Notification : " << tokens[1] << " By User :" << tokens[2] << " Add Successfully" << endl;
	}

	void addNotifications(string str) {
		vector<string> lines;
		split(lines, str);

		for (auto& line : lines) {
			addNotification(line);
		}
	}

	void setUserOffline(size_t uid) {
		auto it = Users.find(uid);
		if (it != Users.end()) {
			it->second = false;
			cout << "User : " << uid << " Got Offline" << endl;
			return;
		}
		cout << "User : " << uid << " Not Found" << endl;
	}

	void setUserOnline(size_t uid) {
		auto it = Users.find(uid);
		if (it != Users.end()) {
			it->second = true;
			cout << "User : " << uid << " Got Online" << endl;
			return;
		}
		cout << "User : " << uid << " Not Found" << endl;
	}

	void sendNextNotification() {
		if (NotifiQueue.empty()) {
			cout << "No Notification to Send" << endl;
			return;
		}

		while (!NotifiQueue.empty()) {
			auto ntf = NotifiQueue.top();
			NotifiQueue.pop();

			auto Nit = NotifMap.find(ntf.second);
			if (Nit == NotifMap.end()) {
				//cout << "Notification Already Expired : " << ntf.second << endl << endl;
				continue;
			}
			auto Uit = Users.find(Nit->second.UserID);
			if (Uit == Users.end() || !Uit->second) {
				NotifiQueuePending.push(ntf);
				continue;
			}

			Nit->second.show();
			NotifMap.erase(Nit);
			
			restorePending();
			return;
		}

		restorePending();
	}
};