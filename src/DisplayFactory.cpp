/**
 * DisplayFactory.cpp
 * 
 * Creates a display based on user needs
 * 
 * Copyright 2020 AFM Software
 */

#include "DisplayFactory.h"
#include "sesp525.h"

namespace afm
{
    namespace graphic
    {
        DisplayFactory::DisplayFactory()
        {

        }

        DisplayFactory::~DisplayFactory()
        {

        }

        DisplayFactorySPtr DisplayFactory::getInstance()
        {
            static DisplayFactorySPtr p_instance = nullptr;

            if (p_instance == nullptr)
            {
                p_instance = std::make_shared<DisplayFactory>();
            }

            return p_instance;
        }

        IDisplaySPtr DisplayFactory::createDisplay(data::DisplayType display_type, const nlohmann::json &configuration)
        {
            IDisplaySPtr p_display = nullptr;

            switch (display_type)
            {
                case data::DisplayType::SESP525:
                {
                    p_display = std::make_shared<SESP525Display>();
                }
                break;
                default:
                {
                    // error
                }
                break;
            }

            if (p_display != nullptr)
            {
                if (p_display->initialize(configuration) == false)
                {
                    p_display = nullptr;
                }
            }
            return p_display;
        }

        IDisplaySPtr DisplayFactory::createDisplay(std::string display_type, const nlohmann::json &configuration)
        {
            IDisplaySPtr p_display = nullptr;
            data::DisplayType type = data::DisplayType::END_DISPLAY_TYPES;

            if (display_type == constants::sc_sesp525_display)
            {
                type = data::DisplayType::SESP525;
            }

            if (type != data::DisplayType::END_DISPLAY_TYPES)
            {
                p_display = createDisplay(type, configuration);
            }

            return p_display;
        }
    }
}