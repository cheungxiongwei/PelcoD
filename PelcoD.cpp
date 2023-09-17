//
// Created by root on 2023/9/16.
//

#include "PelcoD.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

// clang-format off
/*!
* requires compiling with c++20
* 如果需要在外部修改,请自行使用装饰器进行修改.
PELCO_D 协议
https://www.pelco.com/
https://www.commfront.com/pages/pelco-d-protocol-tutorial

PELCO_P 协议
https://www.commfront.com/pages/pelco-p-protocol-tutorial

VISCA 协议
https://www.epiphan.com/userguides/LUMiO12x/Content/UserGuides/PTZ/3-operation/rs232.htm

消息格式：
+-----------+---------+-----------+-----------+--------+--------+----------+
|  Byte 1   | Byte 2  |  Byte 3   |  Byte 4   | Byte 5 | Byte 6 |  Byte 7  |
+-----------+---------+-----------+-----------+--------+--------+----------+
| Sync Byte | Address | Command 1 | Command 2 | Data 1 | Data 2 | Checksum |
+-----------+---------+-----------+-----------+--------+--------+----------+

标准命令集：
.-----------.-----------.-----------.-----------.------------------.---------------.------------.-----------.------------.
|           |   Bit 7   |   Bit 6   |   Bit 5   |      Bit 4       |     Bit 3     |   Bit 2    |   Bit 1   |   Bit 0    |
:-----------+-----------+-----------+-----------+------------------+---------------+------------+-----------+------------:
| Command 1 | Sense     | Reserved  | Reserved  | Auto/Manual Scan | Camera On/Off | Iris Close | Iris Open | Focus Near |
:-----------+-----------+-----------+-----------+------------------+---------------+------------+-----------+------------:
| Command 2 | Focus Far | Zoom Wide | Zoom Tele | Down             | Up            | Left       | Right     | Always 0   |
'-----------'-----------'-----------'-----------'------------------'---------------'------------'-----------'------------'
*/
// clang-format on

struct _StandardCMD {
    // Command 1
    uint32_t focus_near : 1;
    uint32_t iris_open  : 1;
    uint32_t iris_close : 1;
    uint32_t camera     : 1;
    uint32_t scan       : 1;
    uint32_t reserve_1  : 1;  // reserve
    uint32_t reserve_2  : 1;  // reserve
    uint32_t sense      : 1;

    // Command 2
    uint32_t always    : 1;
    uint32_t right     : 1;
    uint32_t left      : 1;
    uint32_t up        : 1;
    uint32_t down      : 1;
    uint32_t zoom_tele : 1;
    uint32_t zoom_wide : 1;
    uint32_t focus_far : 1;
};

struct StandardCMD_ {
    uint8_t cmd_1;
    uint8_t cmd_2;
};

union StandardCMD {
    StandardCMD_ cmd;
    _StandardCMD _cmd = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};

void PelcoD::panLeft(u8 speed) {
    speed    = std::clamp<u8>(speed, 0, 0x3f);
    auto cmd = magic();
    StandardCMD standard_cmd;
    standard_cmd._cmd.left = 1;
    auto& std_cmd          = standard_cmd.cmd;
    cmd[2]                 = std_cmd.cmd_1;
    cmd[3]                 = std_cmd.cmd_2;
    cmd[4]                 = speed;
    cmd[5]                 = 0x00;
    cmd[6]                 = checksum(cmd);
    sendData(cmd);
}

void PelcoD::panRight(u8 speed) {
    speed    = std::clamp<std::uint8_t>(speed, 0, 0x3f);
    auto cmd = magic();
    StandardCMD standard_cmd;
    standard_cmd._cmd.right = 1;
    auto& std_cmd           = standard_cmd.cmd;
    cmd[2]                  = std_cmd.cmd_1;
    cmd[3]                  = std_cmd.cmd_2;
    cmd[4]                  = speed;
    cmd[5]                  = 0x00;
    cmd[6]                  = checksum(cmd);
    sendData(cmd);
}

void PelcoD::tiltUp(u8 speed) {
    speed    = std::clamp<std::uint8_t>(speed, 0, 0x3f);
    auto cmd = magic();
    StandardCMD standard_cmd;
    standard_cmd._cmd.up = 1;
    auto& std_cmd        = standard_cmd.cmd;
    cmd[2]               = std_cmd.cmd_1;
    cmd[3]               = std_cmd.cmd_2;
    cmd[4]               = 0x00;
    cmd[5]               = speed;
    cmd[6]               = checksum(cmd);
    sendData(cmd);
}

void PelcoD::tiltDown(u8 speed) {
    speed    = std::clamp<std::uint8_t>(speed, 0, 0x3f);
    auto cmd = magic();
    StandardCMD standard_cmd;
    standard_cmd._cmd.down = 1;
    auto& std_cmd          = standard_cmd.cmd;
    cmd[2]                 = std_cmd.cmd_1;
    cmd[3]                 = std_cmd.cmd_2;
    cmd[4]                 = 0x00;
    cmd[5]                 = speed;
    cmd[6]                 = checksum(cmd);
    sendData(cmd);
}

void PelcoD::moveLeftUp(u8 speed) {
    speed    = std::clamp<std::uint8_t>(speed, 0, 0x3f);
    auto cmd = magic();
    StandardCMD standard_cmd;
    standard_cmd._cmd.left = 1;
    standard_cmd._cmd.up   = 1;
    auto& std_cmd          = standard_cmd.cmd;
    cmd[2]                 = std_cmd.cmd_1;
    cmd[3]                 = std_cmd.cmd_2;
    cmd[4]                 = speed;
    cmd[5]                 = speed;
    cmd[6]                 = checksum(cmd);
    sendData(cmd);
}

void PelcoD::moveRightUp(u8 speed) {
    speed    = std::clamp<std::uint8_t>(speed, 0, 0x3f);
    auto cmd = magic();
    StandardCMD standard_cmd;
    standard_cmd._cmd.right = 1;
    standard_cmd._cmd.up    = 1;
    auto& std_cmd           = standard_cmd.cmd;
    cmd[2]                  = std_cmd.cmd_1;
    cmd[3]                  = std_cmd.cmd_2;
    cmd[4]                  = speed;
    cmd[5]                  = speed;
    cmd[6]                  = checksum(cmd);
    sendData(cmd);
}

void PelcoD::moveLeftDown(u8 speed) {
    speed    = std::clamp<std::uint8_t>(speed, 0, 0x3f);
    auto cmd = magic();
    StandardCMD standard_cmd;
    standard_cmd._cmd.left = 1;
    standard_cmd._cmd.down = 1;
    auto& std_cmd          = standard_cmd.cmd;
    cmd[2]                 = std_cmd.cmd_1;
    cmd[3]                 = std_cmd.cmd_2;
    cmd[4]                 = speed;
    cmd[5]                 = speed;
    cmd[6]                 = checksum(cmd);
    sendData(cmd);
}

void PelcoD::moveRightDown(u8 speed) {
    speed    = std::clamp<std::uint8_t>(speed, 0, 0x3f);
    auto cmd = magic();
    StandardCMD standard_cmd;
    standard_cmd._cmd.right = 1;
    standard_cmd._cmd.down  = 1;
    auto& std_cmd           = standard_cmd.cmd;
    cmd[2]                  = std_cmd.cmd_1;
    cmd[3]                  = std_cmd.cmd_2;
    cmd[4]                  = speed;
    cmd[5]                  = speed;
    cmd[6]                  = checksum(cmd);
    sendData(cmd);
}

void PelcoD::stopMotion() {
    auto cmd = magic();
    StandardCMD standard_cmd;
    auto& std_cmd = standard_cmd.cmd;
    cmd[2]        = std_cmd.cmd_1;
    cmd[3]        = std_cmd.cmd_2;
    cmd[4]        = 0x00;
    cmd[5]        = 0x00;
    cmd[6]        = checksum(cmd);
    sendData(cmd);
}

void PelcoD::zoomIn() {
    auto cmd = magic();
    StandardCMD standard_cmd;
    standard_cmd._cmd.zoom_tele = 1;
    auto& std_cmd               = standard_cmd.cmd;
    cmd[2]                      = std_cmd.cmd_1;
    cmd[3]                      = std_cmd.cmd_2;
    cmd[4]                      = 0x00;
    cmd[5]                      = 0x00;
    cmd[6]                      = checksum(cmd);
    sendData(cmd);
}

void PelcoD::zoomOut() {
    auto cmd = magic();
    StandardCMD standard_cmd;
    standard_cmd._cmd.zoom_wide = 1;
    auto& std_cmd               = standard_cmd.cmd;
    cmd[2]                      = std_cmd.cmd_1;
    cmd[3]                      = std_cmd.cmd_2;
    cmd[4]                      = 0x00;
    cmd[5]                      = 0x00;
    cmd[6]                      = checksum(cmd);
    sendData(cmd);
}

void PelcoD::focusIn() {
    auto cmd = magic();
    StandardCMD standard_cmd;
    standard_cmd._cmd.focus_near = 1;
    auto& std_cmd                = standard_cmd.cmd;
    cmd[2]                       = std_cmd.cmd_1;
    cmd[3]                       = std_cmd.cmd_2;
    cmd[4]                       = 0x00;
    cmd[5]                       = 0x00;
    cmd[6]                       = checksum(cmd);
    sendData(cmd);
}

void PelcoD::focusOut() {
    auto cmd = magic();
    StandardCMD standard_cmd;
    standard_cmd._cmd.focus_far = 1;
    auto& std_cmd               = standard_cmd.cmd;
    cmd[2]                      = std_cmd.cmd_1;
    cmd[3]                      = std_cmd.cmd_2;
    cmd[4]                      = 0x00;
    cmd[5]                      = 0x00;
    cmd[6]                      = checksum(cmd);
    sendData(cmd);
}

void PelcoD::irisIn() {
    auto cmd = magic();
    StandardCMD standard_cmd;
    standard_cmd._cmd.iris_open = 1;
    auto& std_cmd               = standard_cmd.cmd;
    cmd[2]                      = std_cmd.cmd_1;
    cmd[3]                      = std_cmd.cmd_2;
    cmd[4]                      = 0x00;
    cmd[5]                      = 0x00;
    cmd[6]                      = checksum(cmd);
    sendData(cmd);
}

void PelcoD::irisOut() {
    auto cmd = magic();
    StandardCMD standard_cmd;
    standard_cmd._cmd.iris_close = 1;
    auto& std_cmd                = standard_cmd.cmd;
    cmd[2]                       = std_cmd.cmd_1;
    cmd[3]                       = std_cmd.cmd_2;
    cmd[4]                       = 0x00;
    cmd[5]                       = 0x00;
    cmd[6]                       = checksum(cmd);
    sendData(cmd);
}

void PelcoD::setPreset(u8 presetID) {
    presetID = std::clamp<std::uint8_t>(presetID, 0, 0xff);
    auto cmd = magic();
    cmd[2]   = 0x00;
    cmd[3]   = 0x03;
    cmd[4]   = 0x00;
    cmd[5]   = presetID;
    cmd[6]   = checksum(cmd);
    sendData(cmd);
}

void PelcoD::clearPreset(u8 presetID) {
    presetID = std::clamp<std::uint8_t>(presetID, 0, 0xff);
    auto cmd = magic();
    cmd[2]   = 0x00;
    cmd[3]   = 0x05;
    cmd[4]   = 0x00;
    cmd[5]   = presetID;
    cmd[6]   = checksum(cmd);
    sendData(cmd);
}

void PelcoD::callPreset(u8 presetID) {
    presetID = std::clamp<std::uint8_t>(presetID, 0, 0xff);
    auto cmd = magic();
    cmd[2]   = 0x00;
    cmd[3]   = 0x07;
    cmd[4]   = 0x00;
    cmd[5]   = presetID;
    cmd[6]   = checksum(cmd);
    sendData(cmd);
}

/*!
* 1) the value to use to set the pan position to 45 degrees is 4500.
*/
void PelcoD::setPanPosition(u16 pos) {
    pos      = std::clamp<std::uint16_t>(pos, 0, 35999);
    auto cmd = magic();
    cmd[2]   = 0x00;
    cmd[3]   = 0x4B;
    cmd[4]   = (pos >> 8) & 0xff;  // msb
    cmd[5]   = pos & 0xff;         //lsb
    cmd[6]   = checksum(cmd);
    sendData(cmd);
}

/*!
* 1) the value used to set the tilt position to 45 degrees below the horizon, is 4500.
* 2) the value used to set the tilt position 30 degrees above the horizon, is 33000.
* 3) Zero degrees indicates that the device is pointed horizontally (at the horizon).
* 4) Ninety degrees indicates that the device is pointed straight down.
*/
void PelcoD::setTiltPosition(u16 pos) {
    pos      = std::clamp<std::uint16_t>(pos, 0, 35999);
    auto cmd = magic();
    cmd[2]   = 0x00;
    cmd[3]   = 0x4D;
    cmd[4]   = (pos >> 8) & 0xff;  // msb
    cmd[5]   = pos & 0xff;         //lsb
    cmd[6]   = checksum(cmd);
    sendData(cmd);
}

void PelcoD::setZoomPosition(u16 pos) {
    auto cmd = magic();
    cmd[2]   = 0x00;
    cmd[3]   = 0x4F;
    cmd[4]   = (pos >> 8) & 0xff;  // msb
    cmd[5]   = pos & 0xff;         //lsb
    cmd[6]   = checksum(cmd);
    sendData(cmd);
}

void PelcoD::setPanPosition(f32 pos) {
    pos        = std::clamp<f32>(pos, 0.f, 360.f);
    auto value = static_cast<u16>(100.f * pos);
    setPanPosition(value);
}

void PelcoD::setTiltPosition(f32 pos) {
    pos = std::clamp<f32>(pos, -90.f, 90.f);
    if(std::isless(pos, 0.f)) {  // <
        return setTiltPosition(static_cast<u16>((pos * -100.f)));
    } else if(std::isgreater(pos, 0.f)) {  // >
        return setTiltPosition(static_cast<u16>(360000.f - pos * 100.f));
    }
    return setTiltPosition(static_cast<u16>(0));
}

void PelcoD::setZoomPosition(f32 pos) {
    pos = std::clamp<f32>(pos, 0.f, 1.f);
    setZoomPosition(u16(std::numeric_limits<u16>::max() * pos));
}

void PelcoD::queryPanPosition() {
    auto cmd = magic();
    cmd[2]   = 0x00;
    cmd[3]   = 0x51;
    cmd[4]   = 0x00;
    cmd[5]   = 0x00;
    cmd[6]   = checksum(cmd);
    sendData(cmd);
}

void PelcoD::queryTiltPosition() {
    auto cmd = magic();
    cmd[2]   = 0x00;
    cmd[3]   = 0x53;
    cmd[4]   = 0x00;
    cmd[5]   = 0x00;
    cmd[6]   = checksum(cmd);
    sendData(cmd);
}

void PelcoD::queryZoomPosition() {
    auto cmd = magic();
    cmd[2]   = 0x00;
    cmd[3]   = 0x55;
    cmd[4]   = 0x00;
    cmd[5]   = 0x00;
    cmd[6]   = checksum(cmd);
    sendData(cmd);
}

void PelcoD::sendRaw(const std::vector<u8>& data) {
    throw std::logic_error("using an unimplemented function");
}

void PelcoD::sendData(std::span<u8> data) {
    throw std::logic_error("using an unimplemented function");
}

void PelcoD::receiveData(std::span<u8> data) {
    throw std::logic_error("using an unimplemented function");
}
