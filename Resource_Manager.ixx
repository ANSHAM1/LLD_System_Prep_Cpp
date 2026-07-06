//Problem: Company Resource Management System

//Background:
//You are building the backend of a company's resource management system.
//The company receives employee records as plain text.
//Each employee belongs to a department, has a salary, experience, projects, and skills.
//Your job is to parse the data, store it using proper OOP design, and answer different commands.

//Input Format
//Each employee record is represented as :

//EMP |
//EmployeeID |
//Name |
//Department |
//Salary |
//Experience |
//Project1, Project2, ... |
//Skill1, Skill2, ...

export module Resource_Manager;

#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

namespace emp {
    struct EmployeeID {
        int id;

        bool operator==(const EmployeeID& other) const {
            return id == other.id;
        }
    };

    struct Name { std::string name; };

    struct Department { std::string dep; };

    struct Salary { float sal; };

    struct Experience { float exp; };

    struct Project { std::list<std::string> proj; };

    struct Skill { std::list<std::string> skill; };

    struct Employee {
        Name         name;
        Department   dep;
        Salary       sal;
        Experience   exp;

        Employee() = default;

        Employee(std::string n, std::string d, float s, float e) : name(n), dep(d), sal(s), exp(e) {};

        std::string to_str() const {
            std::ostringstream out;

            out << "Name: " << name.name
                << ", Department: " << dep.dep
                << ", Salary: " << sal.sal
                << ", Experience: " << exp.exp;

            return out.str();
        }
    };
}

namespace std {
    template<>
    struct hash<emp::EmployeeID> {
        size_t operator()(const emp::EmployeeID& e) const {
            return hash<int>()(e.id);
        }
    };
}

export class Solution {
private:
    std::unordered_map<emp::EmployeeID, emp::Employee> Employees;
    std::unordered_map<emp::EmployeeID, emp::Project> Projects;
    std::unordered_map<emp::EmployeeID, emp::Skill> Skills;

    std::vector<std::string> Operations;
    std::unordered_set<std::string> ValidOprs = { "SHOW ALL", "SHOW NEW" };

    std::string strip(std::string s) const {
        auto first = s.find_first_not_of(" \t\n\r\f\v");

        if (first == std::string::npos)
            return "";

        auto last = s.find_last_not_of(" \t\n\r\f\v");

        return s.substr(first, last - first + 1);
    }

    void parseEmployeeRecord(const std::string& records) {
        std::istringstream iss(records);
        std::string line;

        std::list<std::string> projs;
        std::list<std::string> skills;

        while (std::getline(iss, line)) {
            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> result;

            while (std::getline(ss, token, '|')) {
                result.push_back(strip(token));
            }

            if (result.size() != 8) continue;

            emp::EmployeeID EID{ stoi(strip(result[1])) };

            projs.clear();
            skills.clear();

            std::stringstream ssp(result[6]);
            std::stringstream sss(result[7]);

            while (std::getline(ssp, token, ',')) {
                projs.push_back(strip(token));
            }

            while (std::getline(sss, token, ',')) {
                skills.push_back(strip(token));
            }

            this->Employees[EID] = emp::Employee(strip(result[2]), strip(result[3]), stof(strip(result[4])), stof(strip(result[5])));
            this->Projects[EID] = { projs };
            this->Skills[EID] = { skills };
        }
    }

    void parseOperations(std::string& oprs) {
        std::istringstream iss(strip(oprs));
        std::string line;

        while (std::getline(iss, line)) {
            if (this->ValidOprs.find(strip(line)) != this->ValidOprs.end()) this->Operations.push_back(strip(line));
        }
    }

    void showAll() {
        for (auto& emp : this->Employees) {
            std::cout << emp.first.id << "->" << emp.second.to_str() << std::endl;
            showProjs(emp.first);
            showSkills(emp.first);

            std::cout << std::endl;
            std::cout << std::endl;
        }
    }

    void showProjs(emp::EmployeeID id) {
        std::stringstream ss;
        ss << "projects :";

        for (std::string& p : this->Projects[id].proj) {
            ss << " " << p << ",";
        }

        std::cout << ss.str() << std::endl;
    }

    void showSkills(emp::EmployeeID id) {
        std::stringstream ss;
        ss << "skills :";

        for (std::string& s : this->Skills[id].skill) {
            ss << " " << s << ",";
        }

        std::cout << ss.str() << std::endl;
    }

    void performOperation(std::string op) {
        if (op == "SHOW ALL") {
            this->showAll();
        }
    }

public:
    void output(std::string& emps, std::string& oprs) {
        parseEmployeeRecord(emps);
        parseOperations(oprs);

        for (auto& op : this->Operations) {
            this->performOperation(op);
        }
    }
};