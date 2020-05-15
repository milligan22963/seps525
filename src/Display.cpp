/**
 * Base display implementation
 * 
 * Copyright 2020 AFM Software
 */

#include "Display.h"
#include "PortFactory.h"

namespace afm
{
    namespace graphic
    {
        Display::Display()
        {

        }

        Display::~Display()
        {

        }

        bool Display::initialize(const nlohmann::json &configuration)
        {
            bool success = false;

            if (configuration.find(sc_ports) != configuration.end())
            {
                nlohmann::json primary_port = configuration[sc_ports][0];
                uint32_t instance = primary_port[sc_instance].get<uint32_t>();
                uint32_t device = primary_port[sc_device].get<uint32_t>();
                std::string port_type = primary_port[sc_port_type].get<std::string>();

                // creat the primary interface
                m_pport = communication::PortFactory::getInstance()->createPort(port_type, instance, device);

                if (m_pport != nullptr)
                {
                    m_xres = configuration[sc_x_resolution].get<uint16_t>();
                    m_yres = configuration[sc_y_resolution].get<uint16_t>();

                    success = on_initialize(configuration);
                }
            }
            return success;
        }

        void Display::clear_screen(const data::Color color)
        {
            // do nothing and hope for the best
        }

        void Display::set_pixel(const data::Coordinate_8t position, const data::Color color)
        {
            
        }

        bool Display::reset()
        {
            return on_reset();
        }
    }
}