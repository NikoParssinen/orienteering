/*
####################################################################
# TIE-02207 Programming 2: Basics, S2019                           #
#                                                                  #
# Project3: Orienteering                                           #
# Program description: Orienteering map.                           #
#                                                                  #
# Author: Niko Pärssinen, 283244, niko.parssinen@tuni.fi           #
####################################################################
*/
#include "routes.hh"
#include <iomanip>

// Constructor
Routes::Routes(){
}

// Deconstructor
Routes::~Routes(){
}

// Inputs the map size to the datastructure
void Routes::set_map_size(int width, int height)
{
    width_ = width;
    height_ = height;
}

// Inputs the points to the datastructure
void Routes::add_point(std::string &name, int x, int y, int height, char marker)
{
    // Check if the point is inside map
    if(0 < x and x <= width_ and 0 < y and y <= height_) {

        // check if the point is already existing
        bool new_point = true;
        for (auto point : points_) {
            if(point.first == name) {
                new_point = false;
            }
        }
        // Continue if it is a new point
        if(new_point == true) {

            Point point = Point{name, x, y, height, marker};

            points_.insert({name, point});
        }

    }

}

// Inputs the routes to the datastructure
bool Routes::connect_route(std::string &from, std::string &to, std::string &route_name)
{

    // Check if the points exist
    bool unknown_from = true;
    bool unknown_to = true;
    for (auto point : points_) {
        if(point.first == from) {
            unknown_from = false;
        } else if (point.first == to) {
            unknown_to = false;
        }
    }

    // Continue if points exist
    if(unknown_from == false and unknown_to == false) {

        // If this is the start of the route (isn't found in the datastructure)
        if(routes_.find(route_name) == routes_.end()) {

            std::vector<std::string> route = {from, to};
            routes_.insert({route_name, route});

        }

        // Otherwise continue the existing route
        else {
            routes_.at(route_name).push_back(to);
        }
        return true;
    }
    else {
        return false;
    }
}

// Prints out the map
void Routes::print_map() const
{

    std::cout << " ";
    int i = 1;

    // Loop for printing the first row that is the numbers
    while (i <= width_) {
        std::cout << std::setw(3);
        std::cout << i;
        ++i;
    }
    std::cout << std::endl;

    int h = 1;

    // Loop for the rows of the map
    while (h <= height_) {

        std::cout << std::setw(2);
        std::cout << h;
        int w = 1;

        // Loop for every position in a row
        while (w <= width_) {


            bool point_there = false;

            // Loop for checking if there is a point in that position
            for(auto point : points_) {

                Point ppoint = point.second;

                if(ppoint.coord_x == w) {
                    if(ppoint.coord_y == h) {
                        std::cout << "  " << ppoint.marker;
                        point_there = true;
                    }
                }

            }
            // Printing a dot if there isn't
            if(point_there == false) {
                std::cout << "  .";
            }

            ++w;
        }

        std::cout << std::endl;
        ++h;
    }
}

// Prints all route names
void Routes::print_route_names() const
{
    std::cout << "Routes:" << std::endl;
    for(auto route : routes_) {
        std::cout << " - " << route.first << std::endl;
    }
}

// Prints all point names
void Routes::print_points() const
{
    std::cout << "Points:" << std::endl;
    for(auto point : points_) {
        std::cout << " - " << point.first << " : " << point.second.marker << std::endl;
    }
}

// Prints the wanted route
void Routes::print_route(const std::string &name) const
{
    // Loop to find the correct route
    std::pair<std::string, std::vector<std::string>> correct_route;
    bool unknown = true;
    for (std::pair<std::string, std::vector<std::string>> route : routes_) {
        if(route.first == name){
            correct_route = route;
            unknown = false;
        }
    }

    // If the route wasn't found
    if(unknown == true) {
        std::cout << "Error: Route named " << name << " can't be found" << std::endl;
    }

    // Route was found
    else {
        std::cout << correct_route.second.at(0) << std::endl;

        int i = 1;
        int route_size = correct_route.second.size();
        while(i < route_size) {
            std::cout << " -> " << correct_route.second.at(i) << std::endl;
            ++i;
        }
    }
}

// Prints out the length of the route
void Routes::route_length(const std::string &name) const
{
    // Loop to find the correct route
    std::pair<std::string, std::vector<std::string>> correct_route;
    bool unknown = true;
    for (std::pair<std::string, std::vector<std::string>> route : routes_) {
        if(route.first == name){
            correct_route = route;
            unknown = false;
        }
    }

    // If the route wasn't found
    if(unknown == true) {
        std::cout << "Error: Route named " << name << " can't be found" << std::endl;
    }

    // Route was found
    else {
        double length = 0;
        int i = 1;
        int route_size = correct_route.second.size();
        while(i < route_size) {
            std::string point_name1 = correct_route.second.at(i-1);
            std::string point_name2 = correct_route.second.at(i);

            Point point1;
            Point point2;

            for(auto point : points_) {
                if(point.first == point_name1) {
                    point1 = point.second;
                }
                else if(point.first == point_name2) {
                    point2 = point.second;
                }


            }

            length += pythagoras(point1.coord_x, point2.coord_x, point1.coord_y, point2.coord_y);
            ++i;

        }

        if(length < 10) {
            std::cout.precision(1);
            std::cout << std::fixed;
            std::cout << "Route " << name << " length was " << length << std::endl;
        } else if( length >= 10) {
            std::cout << "Route " << name << " length was " << int(round(length)) << std::endl;
        }

    }
}

//
void Routes::greatest_rise(const std::string &point_name) const
{
    // Loop to find the correct point
    Point the_point;
    bool unknown = true;
    for (auto point : points_) {
        if(point.first == point_name){
            the_point = point.second;
            unknown = false;
        }
    }

    // If the point wasn't found
    if(unknown == true) {
        std::cout << "Error: Point named " << point_name << " can't be found" << std::endl;
    }

    // Point was found
    else {
        std::vector<std::string> route_names;
        std::map<int, std::vector<std::string>> height_diffs;

        // Go through all the routes
        for(auto route: routes_) {
            std::vector<std::string> point_names = route.second;
            int height1 = the_point.height;
            int height2 = 0;
            int last_height = the_point.height;
            bool after_the_point = false;

            // Loop to find the greatest height after the point
            for(std::string point_name : point_names) {
                if (after_the_point == true) {
                    Point point2;
                    // Loop for getting the point structure
                    for(auto point : points_) {
                        if(point_name == point.first) {
                            point2 = point.second;
                        }
                    }
                    height2 = point2.height;
                    // Checking if a downhill starts
                    if(last_height > height2) {
                        break;
                    } else {
                        last_height = height2;
                    }

                }
                else if (point_name == the_point.name) {
                    after_the_point = true;
                }

            }
            int height_diff = last_height - height1;

            // Height difference is new
            if(height_diffs.find(height_diff) == height_diffs.end()) {
                route_names = {route.first};
                height_diffs.insert({height_diff, route_names});
            }

            // Height difference is not new
            else {
                route_names = height_diffs[height_diff];
                route_names.push_back(route.first);
                height_diffs[height_diff] = route_names;
            }

        }

        int map_size = height_diffs.size();

        // If there were no points after it on any routes
        if(map_size == 0) {
            std::cout << "No route rises after point " << point_name << std::endl;
        }

        // If there is a biggest height difference
        else {
            int biggest_height_diff = height_diffs.rbegin()->first;

            // If there is a positive height difference
            if(biggest_height_diff > 0) {
                std::cout << "Greatest rise after point " << point_name << ", "
                          << biggest_height_diff << " meters, is on route(s):" << std::endl;

                std::vector<std::string> routes = height_diffs.rbegin()->second;
                for(auto route : routes) {
                    std::cout << " - " << route << std::endl;
                }

            }

            // If all height differences are negative
            else {
                std::cout << "No route rises after point " << point_name << std::endl;
            }
        }



    }
}





// This calculates the distance between two points
double pythagoras(int x1, int x2, int y1, int y2){
    double result = sqrt(abs(x1 - x2) * abs(x1 - x2) + abs(y1 - y2) * abs(y1 - y2));

    return result;
}



