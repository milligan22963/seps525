/**
 * Fonts.h
 * 
 * Basic font
 * 
 * Copyright 2020 AFM Software
 */

#ifndef _H_FONT
#define _H_FONT

#include <cstdint>
#include <memory>

#include "Constants.h"

namespace afm
{
    namespace graphic
    {
        class IFont
        {
            public:
                virtual ~IFont() {}
        };

        using IFontSPtr = std::shared_ptr<IFont>;
    }
}
#endif
