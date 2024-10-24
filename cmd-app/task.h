#pragma once
#include <string> // std::string

enum Priority {
    HIGH,
    MED,
    LOW
};

class Task {
    public:
    // name (cannot be null)
    // id (cannot be null)
    // description
    // isCompleted
    // deadline
    // pattern??? like pattern of repeating??? (immediately, x daily, x weekly) tbd
    // priorities and stuff
        int id = -1; // null
        std::string name;
        //Priority p = MED;
        std::string priority = "MED";

        Task(std::string name) {
            this->name = name;
        }

        Task(char* name) {
            this->name = std::string(name);
        }
        
        Task(int id, char* name) {
            this->id = id;
            this->name = std::string(name);
        }

        //int setPriority(std::string priority) {
        //    if (priority == "HIGH") {
        //        this->p = HIGH;
        //        return 0;
        //    }
        //    else if (priority == "MED") {
        //        this->p = MED;
        //        return 0;
        //    }
        //    else if (priority == "LOW") {
        //        this->p = LOW;
        //        return 0;
        //    }
        //    else {
        //        return 1; // error
        //    }
        //}

        int setPriority(char* priority) {
            std::string s = std::string(priority);
            if (s != std::string("HIGH") && s != std::string("MED") && s != std::string("LOW")) {
                return 1;
            }
            this->priority = s;
        }
};