#include <iostream>
#include <fstream>
#include<string>
#include <sstream>
#include<map>
#include <time.h>
using namespace std;

class Write {
    private:
        ofstream file;
    public:
        void write(string message) {
            file.open("result.txt", fstream::app);
            file << message;
            file.close();
        };
};

class Tank : Write{
    private:
        double capacity;
        double fuel_quantity;
        bool broken;
        bool valve;
    public:
        Tank() {
            capacity = 0;
            fuel_quantity = 0;
            broken = false;
            valve = false;
        };
        /*GETTER METHODS*/
        double get_capacity() {return capacity;};
        double get_fuel_quantity() {return fuel_quantity;};
        bool get_broken() {return broken;};
        bool get_valve() {return valve;};
        /*SETTER METHODS*/
        void set_capacity(double capacity_) {
            capacity = capacity_;
        };
        void set_fuel_quantity(double fuel_quantity_) {
            fuel_quantity = fuel_quantity_; 
        };
        void set_broken(bool broken_) {
            broken = broken_;
        };
        void set_valve(bool valve_) { 
            valve = valve_;
        }
        /*COMPLICATED METHODS*/
        void add_fuel(double quantity) {
            if (quantity + fuel_quantity >= capacity) {
                fuel_quantity = capacity;
            }
            else {
                fuel_quantity += quantity;
            };
        };

};

class InternalTank : Write {
    private:
        double capacity = 55;
        double fuel_quantity = 0;
    public:
        /*SETTER METHODS*/
        void increase_fuel_quantity(double quantity) {
            if (quantity >= capacity) {
                fuel_quantity = 55;
                return;
            }
            fuel_quantity += quantity;
        };
        void decrease_fuel_quantity(double quantity) {
            if (quantity >= fuel_quantity) {
                fuel_quantity = 0;
                return;
            }
            fuel_quantity -= quantity;
        };
        /*GETTER METHODS*/
        double get_fuel_quantity() {
            return fuel_quantity;
        };
        double get_capacity() {
            return capacity;
        };
};

class Engine : Write{
    private:
        double fuel_per_second = 5.5;
        bool status;
        InternalTank internal_tank;
        map<int, Tank*> tanks;
        map<int, Tank*> connected_tanks;
        int iterator;
    public:
        Engine() {
            status = false;
            iterator = 1;
        };

        /*INTERNAL TANK METHODS*/
        int get_random_tank_id() { // returns a pseudo-random proper tank_id 
            auto itr = connected_tanks.begin();
            advance(itr, rand() % connected_tanks.size()); //O(n)
            int random_key = itr->first;
            return random_key;
        };

        //Complicated
        void burn_fuel(double quantity, double seconds = 1) {
            string message;
            double in_quantity = internal_tank.get_fuel_quantity();
            if (in_quantity - (seconds*quantity) >= 0) {
                internal_tank.decrease_fuel_quantity(seconds*quantity);
                if (internal_tank.get_fuel_quantity() < 20) {
                    if (!connected_tanks.size()) return;
                    int random_tank_id = get_random_tank_id();
                    Tank* tank = connected_tanks[random_tank_id];
                    transfer_fuel(random_tank_id, tank->get_fuel_quantity());
                }
                return;
            }

        };

        //Complicated
        void transfer_fuel(int tank_id, double amount) {
            if (!is_tank_connected(tank_id)) {
                return;
            }
            Tank* tank = connected_tanks[tank_id];
            double given;
            if (tank->get_fuel_quantity() <= amount) {
                given = tank->get_fuel_quantity();
                tank->set_fuel_quantity(0);
                internal_tank.increase_fuel_quantity(given);
            }
        };
        /*END OF INTERNAL TANK METHODS*/


        /*SPECIAL METHODS*/
        double get_total_fuel_quantity() { // needed in print_total_fuel_quantity();
            double result = 0;
            for (auto itr = tanks.begin(); itr != tanks.end(); ++itr) {
                Tank* tank = itr->second;
                result += tank->get_fuel_quantity();
            };
            return result;
        };
        bool is_tank_in_tanks(int tank_id) {//needed
            if(tanks.find(tank_id) == tanks.end()){
                return false;
            }
            return true;
        };
        bool is_tank_connected(int tank_id) {
            if (connected_tanks.find(tank_id) == connected_tanks.end()) {
                return false;
            }
            return true;
        };
        /*END OF SPECIAL METHODS*/

        //HW-1 FUNCTIONS
        //----------------------------------
        void start_engine() {//+
            string message;
            if (status) {
                message = "The engine is already on!\n";
                write(message);
                return;
            }
            status = true;
            message = "The engine has been started\n";
            write(message);
        };
        void stop_engine() {//+
            string message;
            if (!status) {
                message = "Engine is already dead!\n";
                write(message);
                return;
            }
            status = false;
            message = "The engine has been stopped\n";
            write(message);
        };
        void add_fuel(int tank_id, double quantity) {
            Tank* tank = tanks[tank_id];
            tank->add_fuel(quantity);
            //change
            string message ="Added " + to_string(quantity) + " to Tank " + to_string(tank_id) + "\n";
            write(message);
        };
        void add_fuel_tank(double capacity) {
            Tank* tank = new Tank();
            tank->set_capacity(capacity);
            tanks.insert(pair<int, Tank*>(iterator, tank));
            iterator++;

            string message = "Tank with " + to_string(capacity) + " capacity added\n";
            write(message);
        };
        void list_fuel_tanks() {
            string message;
            message = "--------------------------------------------\nLISTING ALL TANKS...\n--------------------------------------------\n";
            if (tanks.size() == 0) {
                message = "THERE ARE NO TANKS YET!\n";
                write(message);
                return;
            }
            int id;
            Tank* tank;

            for (auto itr = tanks.begin(); itr != tanks.end(); ++itr) {
                id = itr->first;
                tank = itr->second;

                 message += "ID: " + to_string(id) + "\n"
                         + "CAPACITY: " + to_string(tank->get_capacity()) + "\n"
                         + "FUEL QUANTITY: " + to_string(tank->get_fuel_quantity()) + "\n"
                         + "IS VALVE OPEN: " + (tank->get_valve() ? " OPEN\n" : " CLOSE\n")
                         + "IS TANK BROKEN: " + (tank->get_broken() ? " BROKEN\n" : " SOLID\n")
                         + "--------------------------------------------\n";
            }
            write(message);
        };
        void remove_fuel_tank(int tank_id) {
            string message;
            Tank* tank = tanks[tank_id];
            if (!is_tank_in_tanks(tank_id)) {
                message = "Tank with id " + to_string(tank_id) + " does not exist!\n";
                write(message);
                return;
            }
            //tank->set_valve(false);
            tanks.erase(tank_id);
            message = "Tank with id " + to_string(tank_id) + " removed.\n";
            write(message);
        };
        void connect_fuel_tank_to_engine(int tank_id) {
            string message;
            if (!is_tank_in_tanks(tank_id)) {
                message = "There is no tank with id " + to_string(tank_id) + " !\n";
                write(message);
                return;
            }
            Tank* tank = tanks[tank_id];
            if(is_tank_connected(tank_id)){
                message = "Tank with id " + to_string(tank_id) + " is already connected to engine!";
                write(message);
                return;
            }
            connected_tanks.insert(pair<int, Tank*>(tank_id, tank));
            transfer_fuel(tank_id, tank->get_fuel_quantity());
            message = "Tank with id " + to_string(tank_id) + " connected to engine.\n";
            write(message);
        };
        void disconnect_fuel_tank_from_engine(int tank_id) {
            string message;
            if (!is_tank_connected(tank_id)) {
                message = "Tank with id " + to_string(tank_id) + " is already not connected to engine!";
                write(message);
                return;
            }
            connected_tanks.erase(tank_id);
            message = "Tank with id " + to_string(tank_id) + " disconnected from engine.\n";
            write(message);
        };
        void open_valve(int tank_id) {//+
            string message;
            if (!is_tank_in_tanks(tank_id)) {
                return;
            }
            Tank* tank = tanks[tank_id];
            if (tank->get_valve()) {
                message = "Tank's valve is already open!\n";
                write(message);
                return;
            }
            tank->set_valve(true);
            message = "Valve of tank " + to_string(tank_id) + " has been opened.\n";
            write(message);
        };
        void close_valve(int tank_id) {//+
            string message;
            if (!is_tank_in_tanks(tank_id)) {
                return;
            }
            Tank* tank = tanks[tank_id];
            if (!tank->get_valve()) {
                message = "Tank's valve is already closed!\n";
                write(message);
                return;
            }
            tank->set_valve(false);
            message = "Valve of tank " + to_string(tank_id) + " has been closed.\n";
            write(message);
        };
        void break_fuel_tank(int tank_id) {//+
            string message;
            Tank* tank = tanks[tank_id];
            if (tank->get_broken()) {
                message = "Tank is already broken!\n";
                write(message);
                return;
            }
            tank->set_broken(true);
            message = "Tank with id" + to_string(tank_id) + " has been broken.\n";
            write(message);
        };
        void repair_fuel_tank(int tank_id) {//+
            string message;
            Tank* tank = tanks[tank_id];
            if (!tank->get_broken()) {
                message = "Tank is already solid!\n";
                write(message);
                return;
            }
            tank->set_broken(false);
            message = "Tank with id" + to_string(tank_id) + " has been repaired.\n";
            write(message);
        };
        void stop_simulation() {
            string message;
            message = "Terminating the programme...\n";
            write(message);
            exit(0);
        };
        //HW-2 FUNCTIONS
        //----------------------------------
        void print_fuel_tank_count() { //+
            string message;
            int fuel_tank_count = tanks.size();
            message = "There are " + to_string(fuel_tank_count) + " fuel tank" + (fuel_tank_count%2 ? "\n" : "s\n");
            write(message);
        };
        void list_connected_tanks() {
            string message;
            message = "--------------------------------------------\nLISTING ALL *CONNECTED* TANKS...\n--------------------------------------------\n";
            if (connected_tanks.size() == 0) {
                message = "THERE ARE NO CONNECTED TANKS YET!\n";
                write(message);
                return;
            }
            int id;
            Tank* tank;
            for (auto itr = connected_tanks.begin(); itr != connected_tanks.end(); ++itr) {
                id = itr->first;
                tank = itr->second;

                message += "ID: " + to_string(id) + "\n"
                    + "CAPACITY: " + to_string(tank->get_capacity()) + "\n"
                    + "FUEL QUANTITY: " + to_string(tank->get_fuel_quantity()) + "\n"
                    + "IS VALVE OPEN: " + (tank->get_valve() ? " OPEN\n" : " CLOSE\n")
                    + "IS TANK BROKEN: " + (tank->get_broken() ? " BROKEN\n" : " SOLID\n")
                    + "--------------------------------------------\n";
            }
            write(message);
        };
        void print_total_fuel_quantity() {//+
            double total_fuel_quantity = get_total_fuel_quantity();
            string message = "Total fuel quantity is: " + to_string(total_fuel_quantity) + ".\n";
            write(message);
        };
        void print_total_consumed_fuel_quantity() {
            string message = "Printed total consumed fuel quantity.\n";
            write(message);
        };
        void print_tank_info(int tank_id) {
            string message;
            if (!is_tank_in_tanks(tank_id)) {
                message = "Tank with id " + to_string(tank_id) + " does not exist!\n";
                write(message);
                return;
            };
            Tank* tank = tanks[tank_id];
            message = "INFORMATION OF TANK "
                    + to_string(tank_id) + "\n" 
                    + "-------------------\n"
                    + "ID: " + to_string(tank_id) + "\n"
                    + "CAPACITY: " + to_string(tank->get_capacity()) + "\n"
                    + "FUEL QUANTITY: " + to_string(tank->get_fuel_quantity()) + "\n"
                    + "IS VALVE OPEN: "  + (tank->get_valve() ? " OPEN" : " CLOSE") + "\n"
                    + "STATUS: " + (tank->get_broken() ? " BROKEN" : " SOLID") + "\n"
                    + "-------------------\n";

            write(message);
        };
        void fill_tank(int tank_id, double fuel_quantity) {
            string message = to_string(fuel_quantity) + " amount of fuel added to tank " + to_string(tank_id) + "\n";
            write(message);
        };
        void wait(double seconds) {
            string message = "Waiting for " + to_string(seconds) + " seconds" + "\n";
            write(message);
            double quantity = 5.5;
            if (!status) {
                return;
            }
            burn_fuel(quantity, seconds);
        };

};

class Read {
    private:
        ifstream file;
        string filename;
        Engine engine = Engine();
    public:

        void read() {
            cout << "Enter the filename: ";
            getline(cin, filename);
            
            file.open(filename);

            string s;
            while (true) {
                if(file.eof())break;
                getline(file, s); //reads whole line
                engine.burn_fuel(5.5);
                s.pop_back();
                string command;
                int integer; 
                double doub;
                stringstream ss(s);
                while (ss >> command) {
                    /*IF-ELSE START*/
                    if (command == "start_engine") {
                        engine.start_engine();
                    }
                    else if (command == "stop_engine") {
                        engine.stop_engine();
                    }
                    else if (command == "add_fuel_tank") {
                        ss >> doub;
                        engine.add_fuel_tank(doub);
                    }
                    else if (command == "list_fuel_tanks") {
                        engine.list_fuel_tanks();
                    }
                    else if (command == "print_fuel_tank_count") {
                        engine.print_fuel_tank_count();
                    }
                    else if (command == "remove_fuel_tank") {
                        ss >> integer;
                        engine.remove_fuel_tank(integer);
                    }
                    else if (command == "connect_fuel_tank_to_engine") {
                        ss >> integer;
                        engine.connect_fuel_tank_to_engine(integer);
                    }
                    else if (command == "disconnect_fuel_tank_to_engine") {
                        ss >> integer;
                        engine.disconnect_fuel_tank_from_engine(integer);
                    }
                    else if (command == "list_connected_tanks") {
                        engine.list_connected_tanks();
                    }
                    else if (command == "print_total_fuel_quantity") {
                        engine.print_total_fuel_quantity();
                    }
                    else if (command == "print_total_consumed_fuel_quantity") {
                        engine.print_total_consumed_fuel_quantity();
                    }
                    else if (command == "print_tank_info") {
                        ss >> integer;
                        engine.print_tank_info(integer);
                    }
                    else if (command == "fill_tank") {
                        ss >> integer;
                        ss >> doub;
                        engine.fill_tank(integer, doub);
                    }
                    else if (command == "open_valve") {
                        ss >> integer;
                        engine.open_valve(integer);
                    }
                    else if (command == "close_valve") {
                        ss >> integer;
                        engine.close_valve(integer);

                    }
                    else if (command == "break_fuel_tank") {
                        ss >> integer;
                        engine.break_fuel_tank(integer);
                    }
                    else if (command == "repair_fuel_tank") {
                        ss >> integer;
                        engine.repair_fuel_tank(integer);
                    }
                    else if (command == "wait") {
                        ss >> doub;
                        engine.wait(doub);
                    }
                    else if(command == "stop_simulation") {
                        engine.stop_simulation();
                    }
                    else {
                        cout << "Command cannot resolved!" << endl;
                    }
                    /*IF-ELSE END*/
                }
            }
            file.close();
        };
};

int main()
{    
    remove("result.txt");
    Read read;
    read.read();
}

