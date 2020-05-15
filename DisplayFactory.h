/**
 * DisplayFactory.h
 * 
 * Creates a display based on user needs
 * 
 * Copyright 2020 AFM Software
 */

#ifndef _H_DISPLAY_FACTORY
#define _H_DISPLAY_FACTORY

#include <map>
#include <memory>
#include <nlohmann/json.hpp>

#include "DataTypes.h"
#include "IDisplay.h"

namespace afm
{
    namespace graphic
    {
        class DisplayFactory;

        using DisplayFactorySPtr = std::shared_ptr<DisplayFactory>;

        class DisplayFactory
        {
            public:
                DisplayFactory();
                virtual ~DisplayFactory();

                static DisplayFactorySPtr getInstance();

                IDisplaySPtr createDisplay(data::DisplayType display_type, const nlohmann::json &configuration);
                IDisplaySPtr createDisplay(std::string display_type, const nlohmann::json &configuration);

            private:
        };
    }
}
#endif