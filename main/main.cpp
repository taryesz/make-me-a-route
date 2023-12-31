#include <iostream>
#include <vector>
using namespace std;

class City {
private:
    string name;
    int x;
    int y;
public:
    City() {
        this->name = "";
        this->x = -1;
        this->y = -1;
    }
    void set_name(string name) {
        this->name = name;
    }
    void set_x(int x) {
        this->x = x;
    }
    void set_y(int y) {
        this->y = y;
    }
    string get_name() {
        return this->name;
    }
    int get_x() {
        return this->x;
    }
    int get_y() {
        return this->y;
    }
};

class Distance {
private:
    string start;
    string destination;
    int distance;
    bool transportation; // true for planes, false for cars
    vector<string> buffer_cities;
public:
    Distance() {
        this->start = "";
        this->destination = "";
        this->distance = -1;
        this->transportation = false;
    }
    void set_start(string city) {
        this->start = city;
    }
    void set_destination(string city) {
        this->destination = city;
    }
    void set_distance(int distance) {
        this->distance = distance;
    }
    void set_transportation(bool type) {
        this->transportation = type;
    }
    string get_start() {
        return this->start;
    }
    string get_destination() {
        return this->destination;
    }
    int get_distance() {
        return this->distance;
    }
    bool get_transportation() {
        return this->transportation;
    }
    vector<string> get_buffer_cities() {
        return this->buffer_cities;
    }
};

void input_map_size(int& map_width, int& map_height) {

    cout << "INPUT THE SIZE OF YOUR MAP:" << endl << endl;
    cout << ">> width: ";
    cin >> map_width;
    cout << ">> height: ";
    cin >> map_height;
    cout << endl;

}

void input_map(int map_width, int map_height, char** map) {

    cout << "INPUT THE MAP ITSELF:" << endl << endl;

    for (int current_height = 0; current_height < map_height; current_height++) {
        for (int current_width = 0; current_width < map_width; current_width++) {

            cin >> map[current_height][current_width];

        }
    }

    cout << endl;

}

void search_for_cities(int map_width, int map_height, char** map, int current_width, int current_height, int city_name_length, int& x, int& y) {

    int left = current_width - 1;
    int right = current_width + 1;
    int up = current_height - 1;
    int down = current_height + 1;

    if (left >= 0 && map[current_height][left] == '*') {
        x = left;
        y = current_height;
    }
    else if (right < map_width && map[current_height][right] == '*') {
        x = right;
        y = current_height;
    }
    else if (up >= 0 && map[up][current_width] == '*') {
        x = current_width;
        y = up;
    }
    else if (down < map_height && map[down][current_width] == '*') {
        x = current_width;
        y = down;
    }
    else if (left >= 0 && up >= 0 && map[up][left] == '*') {
        x = left;
        y = up;
    }
    else if (left >= 0 && down < map_height && map[down][left] == '*') {
        x = left;
        y = down;
    }
    else if (right < map_width && up >= 0 && map[up][right] == '*') {
        x = right;
        y = up;
    }
    else if (right < map_width && down < map_height && map[down][right] == '*') {
        x = right;
        y = down;
    }
    else {
        search_for_cities(map_width, map_height, map, current_width - 1, current_height, city_name_length, x, y);
    }
}

void search_for_city_names(int map_width, int map_height, char** map, vector<City*>& cities) {

    string city_name = "";
    bool found_city = false;

    for (int current_height = 0; current_height < map_height; current_height++) {
        for (int current_width = 0; current_width < map_width; current_width++) {

            if (isalpha(map[current_height][current_width])) {
                city_name += map[current_height][current_width];
                found_city = true;
            }
            else if (found_city) {

                int last_letter_x = current_width - 1;
                int city_name_length = city_name.length();

                int x = -1;
                int y = -1;

                search_for_cities(map_width, map_height, map, last_letter_x, current_height, city_name_length, x, y);

                City* city = new City();
                city->set_name(city_name);
                city->set_x(x);
                city->set_y(y);

                cities.push_back(city);

                found_city = false;
                city_name = "";

            }
        }
    }
}

void parse_aviaconnection_commands(vector<string>& aviaconnections, vector<Distance*>& connections) {

    string data = "";
    int words_counter = 0;

    for (string command : aviaconnections) {

        Distance* connection = new Distance();
        words_counter = 0;

        for (int i = 0; i < command.length()+1; i++) {
            if (command[i] == ' ' && words_counter == 0) {
                ++words_counter;
                connection->set_start(data);
                data = "";
            }
            else if (command[i] == ' ' && words_counter == 1) {
                ++words_counter;
                connection->set_destination(data);
                data = "";
            }
            else if ((command[i] == ' ' || command[i] == '\n') && words_counter == 2) {
                ++words_counter;
                int dist = atoi(data.c_str());
                connection->set_distance(dist);
                data = "";
            }
            else {
                data += command[i];
            }
        }

        connection->set_transportation(true);
        connections.push_back(connection);

    }

}

void input_aviaconnections(int& aviaconnections_number, vector<string>& aviaconnections, vector<Distance*>& connections) {

    cout << "HOW MANY AVIACONNECTIONS YOU WANT TO CREATE?: " << endl << "remember: aviaconnections don't follow the road on your map (duh)" << endl << endl;
    cout << ">> ";
    cin >> aviaconnections_number;
    cout << endl;

    cout << "INPUT AVIACONNECTIONS FOLLOWING THE PATTERN 'START FINISH DISTANCE':" << endl << endl;

    string command = "";

    char ch = getchar();

    for (int i = 0; i < aviaconnections_number; i++) {

        cout << ">> ";
        while (true) {

            ch = getchar();
            if (ch == '\n') {
                command += ch;
                aviaconnections.push_back(command);
                command = "";
                break;
            }
            else {
                command += ch;
            }
            
        }

    }
    
    cout << endl;

    parse_aviaconnection_commands(aviaconnections, connections);

}

void parse_information_commands(vector<string>& commands, vector<Distance*>& connections) {

    string data = "";
    int words_counter = 0;

    string start = "";
    string finish = "";
    int info_type = -1;

    for (string command : commands) {

        words_counter = 0;

        for (int i = 0; i < command.length() + 1; i++) {
            if (command[i] == ' ' && words_counter == 0) {
                ++words_counter;
                start = data;
                data = "";
            }
            else if (command[i] == ' ' && words_counter == 1) {
                ++words_counter;
                finish = data;
                data = "";
            }
            else if ((command[i] == ' ' || command[i] == '\n') && words_counter == 2) {
                ++words_counter;
                info_type = atoi(data.c_str());
                data = "";
            }
            else {
                data += command[i];
            }
        }

        // cout << "WYNIK: " << endl << start << endl << finish << endl << info_type << endl;

        for (Distance* connection : connections) {

            if (connection->get_start() == start && connection->get_destination() == finish) {
                if (info_type == 1) {

                    string buffer_cities = "";

                    for (string city : connection->get_buffer_cities()) {
                        city += " ";
                        buffer_cities += city;
                    }

                    // LAUNCH DIJKSTRA HERE

                    // cout << "Travel time: " << "***" << endl << "Buffer cities: " << buffer_cities << endl;
                }
                else if (info_type == 0) {

                    // LAUNCH DIJKSTRA HERE

                    //cout << "Travel time: " << "***" << endl;
                }
                else {
                    cout << "No such command. Please, try again." << endl;
                }
            }

        }

    }

}

void input_information(int& commands_number, vector<string>& commands, vector<Distance*>& connections) {

    cout << "HOW MANY ROUTES WOULD YOU LIKE TO RECEIVE?: " << endl << endl << ">> ";
    cin >> commands_number;

    cout << endl << "INPUT YOUR COMMANDS FOLLOWING THE PATTERN 'START FINISH 1' OR 'START FINISH 0'" << endl << "tip: '1' shows time and cities between your start and finish, '0' shows only time required" << endl << endl;

    string command = "";

    char ch = getchar();

    for (int i = 0; i < commands_number; i++) {

        cout << ">> ";
        while (true) {

            ch = getchar();
            if (ch == '\n') {
                command += ch;
                commands.push_back(command);
                command = "";
                break;
            }
            else {
                command += ch;
            }

        }

    }

    parse_information_commands(commands, connections);

}

void add_to_neighbors_vector(vector<City*>& cities, vector<Distance*>& connections, int move_x, int move_y, string city_name, int& hash_counter, int& original_dist) {

    string destination_city = "";

    for (City* city : cities) {

        if (city->get_x() == move_x && city->get_y() == move_y) {

            destination_city = city->get_name();

            Distance* neighborhood = new Distance();
            neighborhood->set_start(city_name);
            neighborhood->set_destination(destination_city);
            neighborhood->set_distance(hash_counter);

            bool duplicate = false;

            for (Distance* check : connections) {

                if (check->get_start() == neighborhood->get_destination() && check->get_destination() == neighborhood->get_start() && check->get_distance() == neighborhood->get_distance()) {
                    duplicate = true;
                    break;
                }

            }

            if (!duplicate) {
                connections.push_back(neighborhood);
            }

            hash_counter = original_dist;

        }
    }

}

void dfs(char** map, bool** visited, int& current_height, int& current_width, vector<Distance*>& connections, int map_width, int map_height, string city_name, vector<City*>& cities, int& hash_counter) {
    
    int original_dist = hash_counter;
    int directions_x[] = { -1, 1, 0, 0 };
    int directions_y[] = { 0, 0, -1, 1 };

    visited[current_height][current_width] = true;

    for (int i = 0; i < 4; i++) {

        int move_x = current_width + directions_x[i];
        int move_y = current_height + directions_y[i];

        if (move_y >= 0 && move_y < map_height && move_x >= 0 && move_x < map_width) {

            if (map[move_y][move_x] == '#' && !visited[move_y][move_x]) {

                ++hash_counter;
                dfs(map, visited, move_y, move_x, connections, map_width, map_height, city_name, cities, hash_counter);
                --hash_counter;

            }
            else if (map[move_y][move_x] == '*' && visited[move_y][move_x]) {
                continue;
            }
        }

        add_to_neighbors_vector(cities, connections, move_x, move_y, city_name, hash_counter, original_dist);

    }

}

void reset_visited_map(bool** visited, int map_width, int map_height) {

    for (int current_height = 0; current_height < map_height; current_height++) {
        for (int current_width = 0; current_width < map_width; current_width++) {
            visited[current_height][current_width] = false;
        }
    }

}

void create_neighbors_list(int map_width, int map_height, char** map, bool** visited, vector<Distance*>& connections, vector<City*>& cities, int& hash_counter) {
    
    for (int current_height = 0; current_height < map_height; current_height++) {
        for (int current_width = 0; current_width < map_width; current_width++) {

            if (map[current_height][current_width] == '*' && !visited[current_height][current_width]) {

                for (City* city : cities) {

                    if (city->get_x() == current_width && city->get_y() == current_height) {

                        string city_name = city->get_name();
                        int x = current_width;
                        int y = current_height;
                        dfs(map, visited, y, x, connections, map_width, map_height, city_name, cities, hash_counter);

                        reset_visited_map(visited, map_width, map_height);

                    }

                }

            }

        }
    }
}

int main() {

    int map_width = -1;
    int map_height = -1;

    input_map_size(map_width, map_height);

    // initialize 2D array for map
    char** map = new char*[map_height];
    for (int i = 0; i < map_height; i++) {
        map[i] = new char[map_width];
    }

    // innitialize 2D array for visited flags
    bool** map_visited = new bool* [map_height];
    for (int i = 0; i < map_height; i++) {
        map_visited[i] = new bool[map_width];
    }

    // set all map spots as not visited
    reset_visited_map(map_visited, map_width, map_height);

    input_map(map_width, map_height, map);

    vector<City*> cities;

    search_for_city_names(map_width, map_height, map, cities);

    int commands_number = -1;
    vector<string> aviaconnections;
    vector<Distance*> connections;

    input_aviaconnections(commands_number, aviaconnections, connections);

    commands_number = -1;
    vector<string> commands;

    input_information(commands_number, commands, connections);

    int hash_counter = 0; // <<< variable that stores the distance between cities

    create_neighbors_list(map_width, map_height, map, map_visited, connections, cities, hash_counter);

    cout << endl << "RESULTS:" << endl;
    for (Distance* d : connections) {
        cout << endl << ">> ----------" << endl << "Start: " << d->get_start() << endl << "Destination: " << d->get_destination() << endl << "Travel time: " << d->get_distance() << endl << "Type: " << d->get_transportation() << endl;
    }

    return 0;
}
