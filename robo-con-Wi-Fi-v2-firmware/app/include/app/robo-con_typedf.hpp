/**
 * @file robo-con_typedf.hpp
 * @author your name (you@domain.com)
 * @brief 入力の型
 * @version 0.1
 * @date 2026-09-02
 *
 * @copyright Copyright (c) 2026
 *
 */

#pragma once

#include <cstdint>

struct joystick_state_t {
    uint8_t LX;
    uint8_t LY;
    uint8_t RX;
    uint8_t RY;
    uint8_t L_Push : 1;
    uint8_t R_Push : 1;
} __attribute__((__packed__));

enum LiPo_state_e {
    FULL,
    LITTLE,
    EMPTY,
};

struct button_state_t {
    uint8_t R1 : 1;
    uint8_t R2 : 1;
    uint8_t R3 : 1;
    uint8_t R4 : 1;
    uint8_t L5 : 1;
    uint8_t L6 : 1;
    uint8_t L7 : 1;
    uint8_t L8 : 1;
} __attribute__((__packed__));

struct up_senter_side_button_t {
    uint8_t RU       : 1;
    uint8_t LU       : 1;
    uint8_t RS       : 1;
    uint8_t LS       : 1;
    uint8_t reserved : 4;
} __attribute__((__packed__));
