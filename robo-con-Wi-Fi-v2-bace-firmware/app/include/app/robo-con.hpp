/**
 * @file robo-con.hpp
 * @author your name (you@domain.com)
 * @brief RTOSないで使う関数
 * @version 0.1
 * @date 2026-09-02
 *
 * @copyright Copyright (c) 2026
 *
 */

#pragma once

#include "app/robo-con.h"
#include "app/robo-con_typedf.hpp"
#include "stdint.h"

__attribute__ITCM__ void update_joystick(joystick_state_t& hjoystick);
__attribute__ITCM__ void update_LiPo_State(LiPo_state_e& hLiPo);
__attribute__ITCM__ void update_button(button_state_t& hbutton);
__attribute__ITCM__ void get_lever_pin(lever_pin_state_t& hlever_pin);
__attribute__ITCM__ void update_lever(lever_state_t& hlever);

