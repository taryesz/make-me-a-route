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

void input_map_size(int& map_width, int& map_height) {

    cout << "Please, type in the size of your map:" << endl;
    cin >> map_width;
    cin >> map_height;

}

void input_map(int map_width, int map_height, char** map) {

    for (int current_height = 0; current_height < map_height; current_height++) {
        for (int current_width = 0; current_width < map_width; current_width++) {

            cin >> map[current_height][current_width];

        }
    }

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

int main() {

    int map_width = -1;
    int map_height = -1;

    input_map_size(map_width, map_height);

    char** map = new char*[map_height];
    for (int i = 0; i < map_height; i++) {
        map[i] = new char[map_width];
    }

    input_map(map_width, map_height, map);

    vector<City*> cities;

    search_for_city_names(map_width, map_height, map, cities);

    for (City* c : cities) {
        cout << endl << "City: " << c->get_name() << endl << "x: " << c->get_x() << endl << "y: " << c->get_y() << endl;
    }

    return 0;
}
