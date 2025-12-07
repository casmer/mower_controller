// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once

namespace cotsbotics::mower_controller
{
typedef int digitalPin_t;
typedef int analogPin_t;
typedef int dacAnalogPin_t;

static constexpr digitalPin_t D25 = 25;	
static constexpr digitalPin_t D27 = 27;	
static constexpr digitalPin_t D23 = 23;	
static constexpr digitalPin_t D31 = 31;	
static constexpr digitalPin_t D33 = 33;	
static constexpr digitalPin_t D35 = 35;	
static constexpr digitalPin_t D37 = 37;	
static constexpr digitalPin_t D29 = 29;	


static constexpr digitalPin_t D22 = 22;
static constexpr digitalPin_t D32 = 32;
static constexpr digitalPin_t D24 = 24;
static constexpr digitalPin_t D34 = 34;
static constexpr digitalPin_t D26 = 26;
static constexpr digitalPin_t D36 = 36;
static constexpr digitalPin_t D28 = 28;
static constexpr digitalPin_t D30 = 30;

static constexpr analogPin_t ADC0 = 0;
static constexpr analogPin_t ADC1 = 1;
static constexpr analogPin_t ADC2 = 2;
static constexpr analogPin_t ADC3 = 3;

 static constexpr analogPin_t MCP4728_CHANNEL_A = 0;
 static constexpr analogPin_t  MCP4728_CHANNEL_B = 1;
 static constexpr analogPin_t  MCP4728_CHANNEL_C = 2;
 static constexpr analogPin_t  MCP4728_CHANNEL_D = 3;

}; // cotsbotics::mower_controller