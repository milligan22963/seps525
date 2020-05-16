/**
 * SESP525 display driver
 *
 * Copyright 2020 AFM Software
 */

#include <iostream>
#include <unistd.h>
#include <sys/param.h>

#include "Constants.h"
#include "PortFactory.h"
#include "sesp525.h"

namespace afm
{
    namespace graphic
    {
        const uint32_t sc_1_millisecond = 1000;
        const uint8_t sc_6_bits = 0x3F; // mask for 6 bit color
        const uint8_t sc_screen_width = 160;
        const uint8_t sc_screen_height = 128;

        const std::string sc_rs_pin = "RS";
        const std::string sc_reset_pin = "RESET";

        enum SESP525_Command
        {
            SESP525_INDEX,
            SESP525_STATUS_READ,
            SESP525_OSC_CTL,
            SESP525_CLOCK_DIV,
            SESP525_REDUCE_CURRENT,
            SESP525_SOFT_RESET,
            SESP525_DISP_ON_OFF,
            SESP525_PRECHARGE_TIME_R = 0x08,
            SESP525_PRECHARGE_TIME_G,
            SESP525_PRECHARGE_TIME_B,
            SESP525_PRECHARGE_CURRENT_R,
            SESP525_PRECHARGE_CURRENT_G,
            SESP525_PRECHARGE_CURRENT_B,
            SESP525_DRIVING_CURRENT_R = 0x10,
            SESP525_DRIVING_CURRENT_G,
            SESP525_DRIVING_CURRENT_B,
            SESP525_DISPLAY_MODE,
            SESP525_RGB_INTERFACE,
            SESP525_RGB_POLARITY,
            SESP525_MEMORY_WRITE_MODE,
            SESP525_MX1_ADDRESS,
            SESP525_MX2_ADDRESS,
            SESP525_MY1_ADDRESS,
            SESP525_MY2_ADDRESS,
            SESP525_MEMORY_ACCESS_POINTER_X = 0x20,
            SESP525_MEMORY_ACCESS_POINTER_Y,
            SESP525_DDRAM_DATA_ACCESS_PORT,
            SESP525_DISPLAY_DUTY_RATIO = 0x28,
            SESP525_DISPLAY_START_LINE,
            SESP525_D1_DDRAM_FAC_HORIZONTAL = 0x2E,
            SESP525_D1_DDRAM_FAR_VERTICAL,
            SESP525_D2_DDRAM_SAC_HORIZONTAL = 0x31,
            SESP525_D2_DDRAM_SAR_VERTICAL,
            SESP525_SCREEN1_FX1,
            SESP525_SCREEN1_FX2,
            SESP525_SCREEN1_FY1,
            SESP525_SCREEN1_FY2,
            SESP525_SCREEN2_SX1,
            SESP525_SCREEN2_SX2,
            SESP525_SCREEN2_SY1,
            SESP525_SCREEN2_SY2,
            SESP525_SCREEN_SAVER_CONTROL,
            SESP525_SCREEN_SAVER_SLEEP_TIMER,
            SESP525_SCREEN_SAVER_MODE,
            SESP525_SCREEN_SAVER_SCREEN1_FU,
            SESP525_SCREEN_SAVER_SCREEN1_MXY,
            SESP525_SCREEN_SAVER_SCREEN2_FU,
            SESP525_SCREEN_SAVER_SCREEN2_MXY,
            SESP525_MOVING_DIRECTION,
            SESP525_SCREEN_SAVER_SCREEN2_SX1 = 0x47,
            SESP525_SCREEN_SAVER_SCREEN2_SX2,
            SESP525_SCREEN_SAVER_SCREEN2_SY1,
            SESP525_SCREEN_SAVER_SCREEN2_SY2,
            SESP525_GRAY_SCALE_TABLE_INDEX = 0x50,
            SESP525_GRAY_SCALE_TABLE_DATA,
            SESP525_IREF = 0x80,
            END_SESP525_Commands
        };

        SESP525Display::SESP525Display()
        {

        }

        SESP525Display::~SESP525Display()
        {
            m_rs_pin = nullptr;
            m_reset_pin = nullptr;
        }

        void SESP525Display::clear_screen(const data::Color &color)
        {
            set_position(1, 1);

            write_data_start();
            for (uint32_t index = 0; index < get_x_resolution() * get_y_resolution(); index++)
            {
                write_pixel(color);
            }
        }

        void SESP525Display::set_pixel(const data::Coordinate_8t &position, const data::Color &color)
        {
            set_pixel(position.x, position.y, color);
        }

        void SESP525Display::draw_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
        {
        }
        
        void SESP525Display::draw_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t thickness)
        {
        }
        
        void SESP525Display::fill_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
        {
        }
        
        void SESP525Display::draw_line(const data::Coordinate_8t &start, const data::Coordinate_8t &end, const data::Color &color)
        {
            uint8_t x1 = MIN(start.x, end.x);
            uint8_t x2 = MAX(start.x, end.x);
            uint8_t y1 = MIN(start.y, end.y);
            uint8_t y2 = MAX(start.y, end.y);
            
            uint8_t dy = y2 - y1;
            uint8_t dx = x2 - x1;
                        
            if (x1 >= sc_screen_width)
            {
                x1 = sc_screen_width - 1;
            }
            
            if (x2 >= sc_screen_width)
            {
                x2 = sc_screen_width - 1;
            }
            
            if (x1 == x2)
            {
                x2++; // give us one pixel width
            }
            
            /* Utilizing the Bresenham algorithm */
            int eps = 0;

            if (y1 != y2)
            {
                for (/* set above */; y1 < y2; y1++)
                {
                    for (uint8_t xCord = x1; xCord < x2; xCord++)
                    {
                        set_pixel(xCord, y1, color);
                        eps += dy;

                        if ((eps << 1) >= dx)
                        {
                            y1++;
                            eps -= dx;
                        }
                    }
                }
            }
            else// if (y1 == y2)
            {
                for (uint8_t xCord = x1; xCord < x2; xCord++)
                {
                    set_pixel(xCord, y1, color);
                }
            }
        }
        
        void SESP525Display::print(char *data)
        {
        }
        
        void SESP525Display::print_line(char *data)
        {
        }
        
        void SESP525Display::printf(const char * __format, ...)
        {
        }

        // internal parts
        bool SESP525Display::on_initialize(const nlohmann::json &configuration)
        {
            bool success = false;

            // there should be multiple ports in order
            // to control the required pins
            if (configuration[sc_ports].size() > 1)
            {
                for (auto iter : configuration[sc_ports])
                {
                    if (iter[sc_port_name] == sc_rs_pin)
                    {
                        m_rs_pin = communication::PortFactory::getInstance()->createPort(data::PortType::PORT_GPIO,
                            iter[sc_instance].get<uint32_t>(), iter[sc_device].get<uint32_t>());
                    }
                    if (iter[sc_port_name] == sc_reset_pin)
                    {
                        m_reset_pin = communication::PortFactory::getInstance()->createPort(data::PortType::PORT_GPIO,
                            iter[sc_instance].get<uint32_t>(), iter[sc_device].get<uint32_t>());
                    }
                }

                if (m_rs_pin != nullptr)
                {
                    if (m_reset_pin != nullptr)
                    {
                        m_reset_pin->write(constants::sc_gpio_high);
                    }

                    /**
                     * Settings based off of:
                     * https://github.com/NewhavenDisplay/NHD-1.69-160128ASC3_Example/blob/master/examples/test/test.ino
                     */
                    write_register(SESP525_Command::SESP525_REDUCE_CURRENT, 1);
                    usleep(sc_1_millisecond);
                    write_register(SESP525_Command::SESP525_REDUCE_CURRENT, 0);

                    // Turn display off
                    write_register(SESP525_Command::SESP525_DISP_ON_OFF, 0);
                    usleep(sc_1_millisecond);

                    // Internal oscillator using external resistor
                    write_register(SESP525_Command::SESP525_OSC_CTL, 1);

                    // 90 hz frame rate
                    write_register(SESP525_Command::SESP525_CLOCK_DIV, 0x30);

                    // 127 duty cycle
                    write_register(SESP525_Command::SESP525_DISPLAY_DUTY_RATIO, 0x7F);

                    // display start line of 1
                    write_register(SESP525_Command::SESP525_DISPLAY_START_LINE, 0);

                    // set rgb interface
                    write_register(SESP525_Command::SESP525_RGB_INTERFACE, 0x01);    

                    // set rgb polarity
                    write_register(SESP525_Command::SESP525_RGB_POLARITY, 0);

                    // set memory write mode - triple xfr, 262K, increase horiz, vert, write horiz
                    write_register(SESP525_Command::SESP525_MEMORY_WRITE_MODE, 0x76);

                    // set driving current value is in uA
                    write_register(SESP525_Command::SESP525_DRIVING_CURRENT_R, 0x45);
                    write_register(SESP525_Command::SESP525_DRIVING_CURRENT_G, 0x34);
                    write_register(SESP525_Command::SESP525_DRIVING_CURRENT_B, 0x33);

                    // set precharge timer (x * clk)
                    write_register(SESP525_Command::SESP525_PRECHARGE_CURRENT_R, 0x04);
                    write_register(SESP525_Command::SESP525_PRECHARGE_CURRENT_G, 0x05);
                    write_register(SESP525_Command::SESP525_PRECHARGE_CURRENT_B, 0x05);

                    // set prechargce current (8xuA)
                    write_register(SESP525_PRECHARGE_CURRENT_R, 0x9D);
                    write_register(SESP525_PRECHARGE_CURRENT_G, 0x8C);
                    write_register(SESP525_PRECHARGE_CURRENT_B, 0x57);

                    // set reference voltage controlled by extern resistor
                    write_register(SESP525_Command::SESP525_IREF, 0);

                    // set mode
                    write_register(SESP525_Command::SESP525_DISPLAY_MODE, 0x0);    
    
                    select_window(data::Coordinate_8t(1, 1),
                        data::Coordinate_8t(get_x_resolution(), get_y_resolution()));

                    set_position(1, 1);

                    // Turn display om
                    write_register(SESP525_Command::SESP525_DISP_ON_OFF, 1);
                    usleep(sc_1_millisecond);

                    success = true;
                }
            }

            return success;
        }

        bool SESP525Display::on_reset()
        {
            bool success = false;

            // reset is active low
            if (m_reset_pin != nullptr)
            {
                m_reset_pin->write(constants::sc_gpio_low);
                usleep(2 * sc_1_millisecond);
                m_reset_pin->write(constants::sc_gpio_high);
            }
            else
            {
                // soft reset
                write_register(SESP525_Command::SESP525_SOFT_RESET, 1);
            }
            
            return success;
        }

        // private parts
        void SESP525Display::select_window(data::Coordinate_8t start, data::Coordinate_8t end)
        {
            // convert to 0 based indicies
            write_register(SESP525_Command::SESP525_MX1_ADDRESS, start.x - 1);
            write_register(SESP525_Command::SESP525_MX2_ADDRESS, end.x - 1);
            write_register(SESP525_Command::SESP525_MY1_ADDRESS, start.y - 1);
            write_register(SESP525_Command::SESP525_MY2_ADDRESS, end.y - 1);
        }

        void SESP525Display::set_position(uint8_t x, uint8_t y)
        {
            write_register(SESP525_Command::SESP525_MEMORY_ACCESS_POINTER_X, x - 1);
            write_register(SESP525_Command::SESP525_MEMORY_ACCESS_POINTER_Y, y - 1);
        }

        void SESP525Display::write_register(uint8_t target_register, uint8_t value)
        {
            m_rs_pin->write(constants::sc_gpio_low);

            get_port()->write(target_register);

            m_rs_pin->write(constants::sc_gpio_high);

            get_port()->write(value);
        }

        void SESP525Display::write_data_start()
        {
            m_rs_pin->write(constants::sc_gpio_low);

            get_port()->write(SESP525_Command::SESP525_DDRAM_DATA_ACCESS_PORT);

            m_rs_pin->write(constants::sc_gpio_high);
        }

        void SESP525Display::write_pixel(data::Color color)
        {
            get_port()->write(color.red & sc_6_bits);
            get_port()->write(color.green & sc_6_bits);
            get_port()->write(color.blue & sc_6_bits);
        }

        void SESP525Display::set_pixel(const uint8_t x, const uint8_t y, const data::Color color)
        {
            set_position(x, y);

            write_data_start();
            write_pixel(color);
        }
    }
}
