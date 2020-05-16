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
                virtual void clear_screen(const data::Color &color) = 0;
                virtual void set_pixel(const data::Coordinate_8t &position, const data::Color &color) = 0;
                virtual void draw_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) = 0;
                virtual void draw_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t thickness) = 0;
                virtual void fill_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) = 0;
                virtual void draw_line(const data::Coordinate_8t &start, const data::Coordinate_8t &end, const data::Color &color) = 0;
                virtual void print(char *data) = 0;
                virtual void print_line(char *data) = 0;
                virtual void printf(const char * __format, ...) = 0;
                virtual bool reset() = 0;
        };

        using IDisplaySPtr = std::shared_ptr<IDisplay>;
    }
}
#endif
