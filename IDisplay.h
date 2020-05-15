/**
 * IDisplay.h
 * 
 * Interface definition for displays
 * 
 * Copyright 2020 AFM Software
 */

#ifndef _H_IDISPLAY
#define _H_IDISPLAY

#include <cstdint>
#include <memory>
#include <nlohmann/json.hpp>

#include "Constants.h"

namespace afm
{
    namespace graphic
    {
        class IDisplay
        {
            public:
                virtual ~IDisplay() {}

                virtual bool initialize(const nlohmann::json &configuration) = 0;
                virtual void clear_screen(const data::Color color) = 0;
                virtual void set_pixel(const data::Coordinate_8t position, const data::Color color) = 0;

                virtual bool reset() = 0;
        };

        using IDisplaySPtr = std::shared_ptr<IDisplay>;
    }
}
#endif