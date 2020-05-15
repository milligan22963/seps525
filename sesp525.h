/**
 * SESP525 display driver
 * Copyright 2020 AFM Software
 */

#ifndef _H_SESP525
#define _H_SESP525

#include "DataTypes.h"
#include "Display.h"
#include "IPort.h"

namespace afm
{
    namespace graphic
    {
        class SESP525Display : public Display
        {
            public:
                SESP525Display();
                virtual ~SESP525Display();

                virtual void clear_screen(const data::Color color) override;
                virtual void set_pixel(const data::Coordinate_8t position, const data::Color color) override;

            protected:
                virtual bool on_initialize(const nlohmann::json &configuration) override;
                virtual bool on_reset() override;

            private:
                void select_window(data::Coordinate_8t start, data::Coordinate_8t end);
                void set_position(data::Coordinate_8t position);
                void write_register(uint8_t target_register, uint8_t value);
                void write_data_start();
                void write_pixel(data::Color color);

            private:
                communication::IPortSPtr m_rs_pin = nullptr;
                communication::IPortSPtr m_reset_pin = nullptr;
        };
    }
}
#endif