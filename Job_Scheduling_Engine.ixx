/*
Problem: AI Job Scheduling Engine

Background:
You are implementing the backend scheduler for an AI cloud platform.

Users submit AI jobs such as model training, inference, embedding generation,
image generation and video generation.

The scheduler receives jobs continuously and decides which one should execute next.

Each job consumes GPU memory, has a priority, an estimated execution time,
and a submission timestamp.

The system has a limited GPU memory budget.

Your task is to build the scheduling engine.

--------------------------------------------------
Input Format

JOB|
JobID|
UserID|
JobType|
Priority|
GPUMemory(MB)|
ExecutionTime(ms)|
Timestamp

Example

JOB|101|U001|TRAIN|9|12000|45000|1751800200
JOB|102|U003|INFERENCE|5|2048|1200|1751800205
JOB|103|U001|EMBEDDING|7|4096|3500|1751800210
JOB|104|U005|IMAGE_GEN|10|8192|18000|1751800212
JOB|105|U002|VIDEO_GEN|8|16384|60000|1751800214

--------------------------------------------------
Supported Job Types

TRAIN
INFERENCE
EMBEDDING
IMAGE_GEN
VIDEO_GEN

Unknown job types are invalid.

--------------------------------------------------
GPU Memory

Maximum GPU Memory

24576 MB

A job can only start if enough GPU memory is available.

Completed jobs release their GPU memory.

--------------------------------------------------
Operations

ADD JOB

Adds a new job.

--------------------------------

RUN NEXT

Select the next executable job according to your scheduling policy.

If no job can currently execute because of memory constraints,
report that no executable job exists.

--------------------------------

COMPLETE JOB JobID

Marks a running job as completed and releases GPU memory.

--------------------------------

CANCEL JOB JobID

Removes a waiting job.

Running jobs cannot be cancelled.

--------------------------------

SHOW STATUS

Display

GPU Memory Used
GPU Memory Free

Waiting Jobs

Running Jobs

Completed Jobs

--------------------------------

SHOW USER U001

Display

Waiting Jobs

Running Jobs

Completed Jobs

for the given user.

--------------------------------

SHOW GPU

Display every currently running job
and its GPU memory usage.

--------------------------------

EXIT

--------------------------------------------------
Scheduling Rules

Higher Priority executes first.

If priorities are equal

↓

Smaller ExecutionTime first.

If still equal

↓

Earlier Timestamp first.

--------------------------------------------------
Invalid Cases

Duplicate Job ID

Negative GPU memory

Negative execution time

Invalid Job Type

Cancel non-existing job

Complete non-existing job

Completing a job that isn't running

Cancelling a running job

Running a completed job

--------------------------------------------------
Expected Output

Maintain the complete scheduler state.

Every operation should update the internal state correctly.

--------------------------------------------------
Bonus

1.
Support multiple GPUs.

GPU0
GPU1
GPU2

Each GPU has its own memory limit.

--------------------------------

2.

SHOW TOP USERS

Users consuming the most GPU memory.

--------------------------------

3.

Automatically finish jobs whose execution time has elapsed
when a simulated current timestamp advances.

--------------------------------

4.

SAVE

Serialize the scheduler state.

--------------------------------

5.

RESTORE

Restore scheduler state.

--------------------------------------------------
Notes

- Focus on clean object-oriented design.
- Don't expose your internal data structures directly.
- Minimize duplicated code.
- Choose data structures that make scheduling efficient.
- Assume thousands of jobs may be submitted.
*/

export module Job_Scheduling_Engine;

#include<iostream>
#include<string>
#include<vector>
#include <limits>
#include<sstream>
#include<unordered_set>
#include<unordered_map>
#include<queue>
using namespace std;

static string strip(string s) {
	auto first = s.find_first_not_of(" \t\n\r\f\v");
	if (first == string::npos)
		return "";

	auto last = s.find_last_not_of(" \t\n\r\f\v");
	return s.substr(first, last - first + 1);
}

static void split(const string& str, vector<string>& results) {
	istringstream iss(strip(str));
	string token;

	while (getline(iss, token)) {
		results.push_back(strip(token));
	}
}

static void split(const string& str, char d, vector<string>& results) {
	istringstream iss(strip(str));
	string token;

	while (getline(iss, token, d)) {
		results.push_back(strip(token));
	}
}

struct Job {
private:
	string STATE = "IDLE";

public:
	size_t JobID;
	size_t UserID;

	string JobType;

	size_t Priority;
	size_t MemoryMB;
	size_t ExecutionTime;
	size_t Timestamp;

	Job() : JobID(std::numeric_limits<size_t>::max()) {}

	Job(size_t jid, size_t uid, string jt, size_t prt, size_t mem, size_t et, size_t ts) :
		JobID(jid), UserID(uid), JobType(jt), Priority(prt), MemoryMB(mem), ExecutionTime(et), Timestamp(ts) {}

	void setState(const string& state) {
		if (state != "COMPLETED" && state != "RUNNING") {
			throw runtime_error("Not a Valid State");
		}
		this->STATE = state;
	}

	bool isCompleted() const {
		return this->STATE == "COMPLETED";
	}

	bool isRunning() const {
		return this->STATE == "RUNNING";
	}

	bool operator==(const Job& other) const {
		return JobID == other.JobID;
	}

	string show() const {
		ostringstream oss;
		oss << "JOB : " << JobID << "|" << "User : " << UserID << "|" << "Memeory Required (MB) : " << MemoryMB;
		return oss.str();
	}
};

namespace std {

	template<>
	struct hash<Job> {
		size_t operator()(const Job& job) const noexcept {
			return hash<size_t>{}(job.JobID);
		}
	};

}

struct Compare {
	bool operator()(const Job& a, const Job& b) const {
		if (a.Priority != b.Priority)
			return a.Priority < b.Priority;

		if (a.ExecutionTime != b.ExecutionTime)
			return a.ExecutionTime > b.ExecutionTime;

		return a.Timestamp > b.Timestamp;
	}
};

class GPU {
public:
	size_t GpuID;
	string GpuName;
	size_t MemeorySize;

	size_t FreeMemory;
	vector<Job> Running_Jobs;

	GPU() = default;

	GPU(size_t gid, string gname, size_t mem) :
		GpuID(gid), GpuName(gname), MemeorySize(mem) {
		this->FreeMemory = MemeorySize;
		this->Running_Jobs = {};
	}

	bool assign(const Job& jb) {
		if (this->FreeMemory < jb.MemoryMB) return false;

		this->FreeMemory -= jb.MemoryMB;
		this->Running_Jobs.push_back(jb);
		return true;
	}

	Job complete(size_t jid) {
		for (auto it = Running_Jobs.begin(); it != Running_Jobs.end(); ++it) {
			if (it->JobID == jid) {
				FreeMemory += it->MemoryMB;
				Job jb = *it;
				Running_Jobs.erase(it);
				return jb;
			}
		}

		cout << "No such Job is Running on GPU: " << GpuID << endl;
		return Job();
	}

	string topUser() {
		if (Running_Jobs.empty())
			return "No Running Jobs";

		const Job* jb = &Running_Jobs[0];

		for (const auto& rjb : Running_Jobs)
			if (rjb.MemoryMB > jb->MemoryMB)
				jb = &rjb;

		return jb->show();
	}

	string show() {
		string result;

		result += "GPU ID: " + to_string(GpuID) + "\n";
		result += "GPU Name: " + GpuName + "\n";
		result += "Memory Size: " + to_string(MemeorySize) + " MB\n";
		result += "Free Memory: " + to_string(FreeMemory) + " MB\n";
		result += "Running Jobs (" + to_string(Running_Jobs.size()) + "):\n";

		if (Running_Jobs.empty()) {
			result += "None\n";
		}
		else {
			for (const auto& job : Running_Jobs) {
				result += "  - " + job.show() + "\n";
			}
		}

		return result;
	}
};

class JSEngine {
private:
	unordered_map<size_t, GPU> GPUs;
	priority_queue<Job, vector<Job>, Compare> Jobs;
 
	unordered_set<Job> Completed;

	unordered_set<size_t> Unique_Jobs;
	unordered_map<size_t, size_t> Running;

	const unordered_set<string> Types = {"TRAIN", "INFERENCE", "EMBEDDING", "IMAGE_GEN", "VIDEO_GEN"};

	GPU* getGPU(size_t mem) {
		GPU* best = nullptr;

		for (auto& gpu : GPUs) {
			if (gpu.second.FreeMemory >= mem) {
				if (best == nullptr || gpu.second.FreeMemory < best->FreeMemory) {
					best = &gpu.second;
				}
			}
		}

		return best;
	}

public:
	JSEngine() {}

	void addGPU(size_t gid, string gname, size_t mem) {
		if (GPUs.find(gid) != GPUs.end()) {
			cout << "GPU with same ID already Exist" << endl;
			return;
		}
		GPUs[gid] = GPU(gid, gname, mem);
		cout << "GPU : " << gid << "--" << gname << "Installed Successfully" << endl;
	}

	void add_job(string& str) {
		vector<string> tokens;
		split(str, '|', tokens);
		if (tokens.size() != 8) {
			cout << "Invalid or Corrupted Job Data Entry" << endl;
			return;
		}
		if (Types.find(tokens[3]) == Types.end()) {
			cout << "Invalid Job Type" << endl;
			return;
		}
		size_t id = stoull(tokens[1]);
		if (Unique_Jobs.find(id) != Unique_Jobs.end()) return;

		Jobs.push(Job(id, stoull(tokens[2]), tokens[3],
			stoull(tokens[4]), stoull(tokens[5]), stoull(tokens[6]), stoull(tokens[7])));
		Unique_Jobs.insert(id);
	}

	void add_jobs(string& str) {
		vector<string> lines;
		split(str, lines);

		for (auto& line : lines) {
			add_job(line);
		}
	}

	void showTopUsers() {
		for (auto& gpu : GPUs) {
			cout << gpu.second.GpuName << " : " << gpu.second.topUser() << '\n';
		}
	}

	void runNextJob() {
		if (GPUs.size() == 0) {
			cout<< "No GPU Found : Add a GPU using fun -> addGPU(ID, NAME, MEMORY_SIZE)" << endl;
			return;
		}

		if (Jobs.empty()) {
			cout << "No Jobs to Schedule" << endl;
			return;
		}

		Job jb = Jobs.top();
		Jobs.pop();

		GPU* gpu = getGPU(jb.MemoryMB);
		if (gpu == nullptr) {
			cout << "Not Sufficient Memeory to Run : Wait for Other Job to Complete" << jb.show() << endl;
			return;
		}

		jb.setState("RUNNING");
		gpu->assign(jb);
		Running[jb.JobID] = gpu->GpuID;
		cout << jb.JobID << " is Running on GPU " << gpu->GpuID << " " << gpu->GpuName << endl;
	}

	void completeJob(size_t jid) {
		auto it = Running.find(jid);

		if (it == Running.end()) {
			cout << "Job not running\n";
			return;
		}

		Job jb = GPUs[it->second].complete(jid);
		if (jb.JobID != numeric_limits<size_t>::max()) {
			jb.setState("COMPLETED");
			Completed.insert(jb);
			Running.erase(jid);
		}
	}
};