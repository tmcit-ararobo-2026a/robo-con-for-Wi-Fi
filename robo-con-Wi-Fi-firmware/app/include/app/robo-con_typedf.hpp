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
    uint8_t L1       : 1;
    uint8_t L2       : 1;
    uint8_t L3       : 1;
    uint8_t L4       : 1;
    uint8_t R5       : 1;
    uint8_t R6       : 1;
    uint8_t R7       : 1;
    uint8_t reserved : 1;
} __attribute__((__packed__));

struct lever_pin_state_t {
    uint8_t L0       : 1;
    uint8_t L1       : 1;
    uint8_t L2       : 1;
    uint8_t L3       : 1;
    uint8_t L4       : 1;
    uint8_t R0       : 1;
    uint8_t R1       : 1;
    uint8_t R2       : 1;
    uint8_t R3       : 1;
    uint8_t R4       : 1;
    uint8_t reserved : 6;
} __attribute__((__packed__));

enum class LeverPosition : uint8_t {
    FRONT,
    RIGHT,
    RIGHT_DEEP,
    LEFT,
    LEFT_DEEP,
    PUSH,
};

struct lever_state_t {
    LeverPosition Left  : 4;
    LeverPosition Right : 4;
} __attribute__((__packed__));

/**
 * @brief ロボットの操縦信号値
 *
 */
struct teleop_t {
    uint8_t header;  // 認識番号

    struct {
        int8_t stick_right[2];             // 0:x, 1:y
        int8_t stick_left[2];              // 0:x, 1:y
    } __attribute__((__packed__)) analog;  // 4byte

    struct {
        LeverPosition lever_right : 3;
        LeverPosition lever_left  : 3;
        uint8_t stick_push_right  : 1;
        uint8_t stick_push_left   : 1;
        uint8_t up                : 1;
        uint8_t down              : 1;
        uint8_t right             : 1;
        uint8_t left              : 1;
        uint8_t circle            : 1;
        uint8_t cross             : 1;
        uint8_t triangle          : 1;
        uint8_t reserved          : 1;
    } __attribute__((__packed__)) buttons;  // 2byte

    /**
     * checksum以外を除いた7Byteの和の補数
     * ただし計算結果の8bitより大きい値は切り捨て
     */
    uint8_t data_checksum;
} __attribute__((__packed__));