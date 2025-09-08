// Find and isolate the leaf nodes of any given xml file

#define cimg_use_imagemagick
#include "include/CImg.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool isLeafNode(std::string line) {
        // if it begins with <node and ends with /> it should be a leaf node hopefully

        // if the line is less than 8 characters it shouldnt be a leaf node
        if (line.size() < 8) {
                return false;
        }
        
        // check for if the beginning is "<node" and the end is "/>" or "</node>"
        if (line.substr(0, 5) == "<node" && 
          ((line.substr(line.size() - 2, line.size()) == "/>") ||
           (line.substr(line.size() - 7, line.size()) == "</node>"))) {
                return true;
        }

        return false;
}

std::string isolateBounds(std::string line) {
        // isolates the bounds field and returns a std::string of the format [a,b][c,d]
        // (a,b) and (c,d) are coords of the corners of bounds I think?

        int begin = line.find("bounds=\"") + std::string("bounds=\"").size();
        int end = begin + line.substr(begin).find_first_of('\"');

        std::string ret = line.substr(begin, end - begin);
        return ret;
}

std::vector<int> convertBounds(std::string line) {
        // converts a std::string of the format [a,b][c,d] into std::vector {a, b, c, d}

        std::vector<int> ret;
        ret.push_back(stoi(line.substr(1, line.find_first_of(','))));
        line = line.substr(line.find_first_of(','));
        ret.push_back(stoi(line.substr(1, line.find_first_of(']'))));
        line = line.substr(line.find_first_of('['));
        ret.push_back(stoi(line.substr(1, line.find_first_of(','))));
        line = line.substr(line.find_first_of(','));
        ret.push_back(stoi(line.substr(1, line.find_first_of(']'))));
        return ret;
}

std::vector<std::vector<int>> isolateLeafBounds(std::string xml_file_name) {
        // uses the above methods to find all the bounds in the xml file for leaf nodes
        // returns a vector x, where x_n is {a, b, c, d} (the coords of the bounds)

        // read from the xml file
        std::ifstream xml(xml_file_name);

        std::vector<std::vector<int>> bounds;

        std::string line;
        bool is_writing = false;
        char c;
        while (xml.get(c)) {
                switch (c) {
                        case '<':
                                is_writing = true;
                                line.push_back(c);
                                break;
                        case '>':
                                is_writing = false;
                                line.push_back(c);
                                if (isLeafNode(line)) {
                                        bounds.push_back(convertBounds(isolateBounds(line)));
                                        //std::cout << convertBounds(isolateBounds(line))[0] << " " << convertBounds(isolateBounds(line))[1] << " "<< convertBounds(isolateBounds(line))[2] << " "<< convertBounds(isolateBounds(line))[3] << " "  << "\n";//bounds.push_back(isolateBounds(line));
                                }
                                line = "";
                                break;
                        default:
                                if (is_writing) {line.push_back(c);}
                                break;
                }
        }

        xml.close();
        return bounds;
}

int main() {

        // get input from the user to know the name of desired screenshot/xml pair
        std::string input;
        std::cout << "Please type the filename of the desired xml/png pair: ";
        std::cin >> input;

        // initialize two variables so we can have one std::string for xml file and one std::string for png file
        std::string input_png = "inputs/"; 
        std::string input_xml = "inputs/";
        input_png.append(input);
        input_xml.append(input);
        input_png.append(".png");
        input_xml.append(".xml");

        // isolate the leaf nodes so we can know where to highlight
        std::vector<std::vector<int>> all_bounds = isolateLeafBounds(input_xml);
        // open the image were going to draw in
        cimg_library::CImg<unsigned char> img(input_png.c_str());

        // draw yellow lines in a square according to the all the bounds in the app screenshot
        const unsigned char yellow[] = {255, 255, 0};
        for (auto bounds : all_bounds) {
                img.draw_rectangle(bounds[0], bounds[1], bounds[2], bounds[1] + 5, yellow);
                img.draw_rectangle(bounds[0], bounds[1], bounds[0] + 5, bounds[3], yellow);
                img.draw_rectangle(bounds[0], bounds[3] - 5, bounds[2], bounds[3], yellow);
                img.draw_rectangle(bounds[2] - 5, bounds[1], bounds[2], bounds[3], yellow);
        }

        std::string output_directory = "outputs/" + input + "_" + "output.png";

        img.save_png(output_directory.c_str());

        //std::cout << input_png << "   " << input_xml << "\n";

        return 0;

}


