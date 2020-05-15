/**
 * Main.cpp
 */

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <nlohmann/json.hpp>
#include <unistd.h>

#include "Constants.h"
#include "DisplayFactory.h"

int main(int argc, char * argv[])
{
    if (argc > 1)
    {
        std::ifstream t(argv[1]);
        std::string str;

        t.seekg(0, std::ios::end);   
        str.reserve(t.tellg());
        t.seekg(0, std::ios::beg);

        str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

        nlohmann::json configuration_data;
        configuration_data = nlohmann::json::parse(str);

        afm::graphic::IDisplaySPtr p_display = afm::graphic::DisplayFactory::getInstance()->createDisplay(afm::constants::sc_sesp525_display, configuration_data);

        if (p_display != nullptr)
        {
std::cout << "Have display, will run\n";
            while (1)
            {
                p_display->clear_screen(afm::constants::BLUE);

                sleep(1);

                p_display->clear_screen(afm::constants::RED);

                sleep(1);

                p_display->clear_screen(afm::constants::GREEN);

                sleep(1);
            }
            p_display = nullptr;
        }
    }
}
