#include <iostream>
#include <fstream>
#include<string>
#include <sstream>
#include<map>

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
        bool get_valve() {return valve;}
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

class Engine : Write{
    private:
        double fuel_per_second = 5.5;
        bool status;
        Tank internal_tank;
        map<int, Tank*> tanks;
        map<int, Tank*> connected_tanks;
        int iterator;
    public:
        Engine() {
            status = false;
            iterator = 1;
        };



        /*SPECIAL METHODS*/
        //01 TANK_LIST OPERATIONS
        Tank* get_tank(int tank_id) {
            return tanks[tank_id];
        }; // returns tank object
        void list_tanks() {
            int id;
            Tank* tank;
            cout << "LISTING ALL TANKS..." << endl
                << "--------------------------------------------" << endl;
            for (auto itr = tanks.begin(); itr != tanks.end(); ++itr) {
                id = itr->first;
                tank = itr->second;

                cout << "ID: " << id << endl
                    << "CAPACITY: " << tank->get_capacity() << endl
                    << "FUEL QUANTITY: " << tank->get_fuel_quantity() << endl
                    << "IS VALVE OPEN: " << (tank->get_valve() ? " OPEN" : " CLOSE") << endl
                    << "IS TANK BROKEN: " << (tank->get_broken() ? " BROKEN" : " SOLID") << endl
                    << "--------------------------------------------" << endl;
            }
        }
        void add_tank(double capacity) {
            Tank* tank = new Tank();
            tank->set_capacity(capacity);
            tanks.insert(pair<int, Tank*>(iterator, tank));
            iterator++;
        };
        void remove_tank(int tank_id) {
            tanks.erase(tank_id);
        };
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

        //01
        //----------------------------------
        void start_engine() {//+
            string message;
            if (status) {
                message = "The engine is already on!\n";
                write(message);
                return;
            }
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
            if (!is_tank_in_tanks(tank_id)) {
                message = "Tank with id " + to_string(tank_id) + " does not exist!\n";
                write(message);
                return;
            }
            tanks.erase(tank_id);
            message = "Tank with id " + to_string(tank_id) + " removed.\n";
            write(message);
        };
        void connect_fuel_tank_to_engine(int tank_id) {
            string message;
            Tank* tank = tanks[tank_id];
            if(is_tank_connected(tank_id)){
                message = "Tank with id " + to_string(tank_id) + " is already connected to engine!";
                write(message);
                return;
            }
            connected_tanks.insert(pair<int, Tank*>(tank_id, tank));
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
        //02
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
            string message = to_string(fuel_quantity) + " amount of fuel added filled to tank " + to_string(tank_id) + "\n";
            write(message);
        };
        void wait(double seconds) {
            string message = "Waiting for " + to_string(seconds) + " seconds" + "\n";
            write(message);
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
            //getline(cin, filename);
            
            file.open("tester.txt");

            string s;
            while (true) {
                if(file.eof())break;
                getline(file, s); //reads whole line
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
    //Read read;
    //read.read();
    Engine engine = Engine();
    engine.add_fuel_tank(240);
    engine.add_fuel_tank(120);
    engine.list_fuel_tanks();
    engine.add_fuel_tank(110);
    engine.remove_tank(2);
    engine.add_fuel(1, 133);
    engine.add_fuel(3, 109.99);
    engine.list_fuel_tanks();
    engine.print_tank_info(3);
    engine.print_total_fuel_quantity();
    engine.remove_fuel_tank(12);

}

//remove("result.txt");
//Engine engine;
//Tank tank;
//engine.start_engine();
//engine.stop_engine();
//tank.open_valve();
//engine.start_engine();
//tank.close_valve();
//tank.open_valve();
//engine.start_engine();


