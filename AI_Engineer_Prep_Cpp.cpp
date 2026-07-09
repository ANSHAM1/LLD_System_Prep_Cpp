//import Resource_Manager;
//
//int main() {
//    std::string emps = R"( 
//                EMP | 101 | Rahul | AI | 85000 | 3 | ChatBot, Vision | Python, C++, PyTorch
//                EMP | 102 | Ankit | Backend | 95000 | 5 | API, Database | C++, SQL, Docker
//                EMP | 103 | Rohit | AI | 120000 | 8 | Vision, NLP | Python, CUDA, TensorRT
//                EMP | 104 | Karan | Frontend | 65000 | 2 | Dashboard | React, JS, CSS
//                EMP | 105 | Priya | Backend | 90000 | 4 | API | C++, Redis, SQL
//                      )";
//
//    std::string oprs = R"(
//                SHOW ALL
//                      )";
//
//    Solution sol;
//    sol.output(emps, oprs);
//}

//import Mini_Package_Manager;
//
//int main() {
//	std::string input = R"(
//						PACKAGE | python | 3.1 | 60 | 
//						PACKAGE | numpy | 2.0 | 45 | python |
//						PACKAGE | opencv | 4.10 | 180 | numpy |
//						PACKAGE | torch | 2.7 | 820 | numpy |
//						PACKAGE | fastapi | 1.2 | 10 | python |
//						PACKAGE | myapp | 1.0 | 12 | torch, opencv, fastapi |
//							)";
//
//	PacMan PM;
//	PM.install_packages(input);
//	PM.uninstall("numpy", 1);
//
//	PM.show_packages();
//}

import RealTime_Notification_Delivery_Engine;

#include<iostream>
#include<string>
using namespace std;

int main()
{
    string inputs = R"(
            NOTIFY|1001|101|EMAIL|3|6|0|Your OTP is 483921
            NOTIFY|1002|102|SMS|2|10|10|Your order #A123 has been shipped
            NOTIFY|1003|103|PUSH|7|12|20|Daily workout reminder
            NOTIFY|1004|104|EMAIL|5|5|0|Password changed successfully
            NOTIFY|1005|105|PUSH|0|8|5|Meeting starts in 15 minutes
            NOTIFY|1006|106|SMS|10|20|30|Suspicious login detected
            NOTIFY|1007|107|EMAIL|1|25|120|Weekly newsletter is available
            NOTIFY|1008|108|PUSH|1|18|60|Flash sale starts now!
            NOTIFY|1009|109|SMS|1|24|0|Your appointment is confirmed
            NOTIFY|1010|110|EMAIL|6|9|300|Your monthly account statement is ready)";

    NDEngine Engine;
    Engine.addNotifications(inputs);

    Engine.setUserOnline(101);
    Engine.setUserOnline(103);
    Engine.setUserOnline(108);

    Engine.sendNextNotification();
    std::cin.get();
    Engine.sendNextNotification();
    std::cin.get();
    Engine.sendNextNotification();
    std::cin.get();
}