/**
 * Base display implementation
 * 
 * Copyright 2020 AFM Software
 */

#ifndef _H_DISPLAY
#define _H_DISPLAY

#include "IDisplay.h"
#include "IPort.h"

namespace afm
{
    namespace graphic
    {
        const std::string sc_ports = "ports";
        const std::string sc_port_type = "port_type";
        const std::string sc_instance = "instance";
        const std::string sc_device = "device";
        const std::string sc_port_name = "port_name";
        const std::string sc_x_resolution = "x_resolution";
        const std::string sc_y_resolution = "y_resolution";

        class Display : public IDisplay
        {
            public:
                Display();
                virtual ~Display();

                virtual bool initialize(const nlohmann::json &configuration) final;
                virtual void clear_screen(const data::Color &color) override;
                virtual void set_pixel(const data::Coordinate_8t &position, const data::Color &color) override;
                virtual bool reset() final;

            protected:
                virtual bool on_initialize(const nlohmann::json &configuration) { return true; }
                virtual bool on_reset() { return true; }
                const communication::IPortSPtr &get_port() const;
                uint16_t get_x_resolution() const { return m_xres; }
                uint16_t get_y_resolution() const { return m_yres; }

                communication::IPortSPtr get_port() { return m_pport; }

            private:
                communication::IPortSPtr m_pport = nullptr;
                uint16_t m_xres = 0;
                uint16_t m_yres = 0;
        };
    }
}
#endif
