# PelcoD

C++20 impl PelcoD

```
Message format：
+-----------+---------+-----------+-----------+--------+--------+----------+
|  Byte 1   | Byte 2  |  Byte 3   |  Byte 4   | Byte 5 | Byte 6 |  Byte 7  |
+-----------+---------+-----------+-----------+--------+--------+----------+
| Sync Byte | Address | Command 1 | Command 2 | Data 1 | Data 2 | Checksum |
+-----------+---------+-----------+-----------+--------+--------+----------+
```

```
Standard command set：
.-----------.-----------.-----------.-----------.------------------.---------------.------------.-----------.------------.
|           |   Bit 7   |   Bit 6   |   Bit 5   |      Bit 4       |     Bit 3     |   Bit 2    |   Bit 1   |   Bit 0    |
:-----------+-----------+-----------+-----------+------------------+---------------+------------+-----------+------------:
| Command 1 | Sense     | Reserved  | Reserved  | Auto/Manual Scan | Camera On/Off | Iris Close | Iris Open | Focus Near |
:-----------+-----------+-----------+-----------+------------------+---------------+------------+-----------+------------:
| Command 2 | Focus Far | Zoom Wide | Zoom Tele | Down             | Up            | Left       | Right     | Always 0   |
'-----------'-----------'-----------'-----------'------------------'---------------'------------'-----------'------------'
```

# Example

```c++
#include <print>  // c++23(msvc 17.4)
#include "PelcoD.h"

class CameraController : public PelcoD
{
public:
    CameraController(u8 deviceId)
      : PelcoD(deviceId) {}

    bool connect(const std::string ip, u16 port) {}

    void disconnect() {}

    void sendData(std::span<u8> data) override {
        // socket.write(data.data(),data.size());

        // debug
        std::print("[ ");
        for(auto v: data) {
            std::print("{:#04x} ", (uint32_t)v);
        }
        std::println("]");
    }

private:
    struct socket;  // impl
};

int main() {
    CameraController ipc(0x01);
    ipc.panLeft(32);
    ipc.panRight(32);
    ipc.tiltUp(32);
    ipc.tiltDown(32);
    return 0;
}

/*output
[ 0xff 0x01 0x00 0x04 0x20 0x00 0x25 ]
[ 0xff 0x01 0x00 0x02 0x20 0x00 0x23 ]
[ 0xff 0x01 0x00 0x08 0x00 0x20 0x29 ]
[ 0xff 0x01 0x00 0x10 0x00 0x20 0x31 ]
*/
```

# Interface
```
class IPelcoD
{
public:
    // 向左平移 ←
    virtual void panLeft(u8 speed) = 0;
    // 向右平移 →
    virtual void panRight(u8 speed) = 0;
    // 向上倾斜 ↑
    virtual void tiltUp(u8 speed) = 0;
    // 向下倾斜 ↓
    virtual void tiltDown(u8 speed) = 0;
    // 左上移动 ←↑
    virtual void moveLeftUp(u8 speed) = 0;
    // 右上移动 →↑
    virtual void moveRightUp(u8 speed) = 0;
    // 左下移动 ←↓
    virtual void moveLeftDown(u8 speed) = 0;
    // 右下移动 →↓
    virtual void moveRightDown(u8 speed) = 0;

    // 停止移动
    virtual void stopMotion() = 0;

    // zoomIn/放大/焦距变大/倍率变大/zoomTele
    virtual void zoomIn() = 0;
    // zoomOut/缩小/焦距变小/倍率变小/zoomWide
    virtual void zoomOut() = 0;

    // 焦点前调/调近焦点/聚焦近
    virtual void focusIn() = 0;
    // 焦点后调/调远焦点/聚焦远
    virtual void focusOut() = 0;

    // 光圈扩大/irisOpen
    virtual void irisIn() = 0;
    // 光圈缩小/irisClose
    virtual void irisOut() = 0;

    // 扩展指令
    // 设置预置点
    virtual void setPreset(u8 presetID) = 0;
    // 清除预置点
    virtual void clearPreset(u8 presetID) = 0;
    // 调用预置点
    virtual void callPreset(u8 presetID) = 0;

    virtual void setPanPosition(u16 pos)  = 0;
    virtual void setTiltPosition(u16 pos) = 0;
    virtual void setZoomPosition(u16 pos) = 0;

    // 便捷的浮点操作函数
    // 0.0 ~ 360.0
    virtual void setPanPosition(f32 pos) = 0;
    // -90.0 ~ 90.0
    virtual void setTiltPosition(f32 pos) = 0;
    // 0.0 ~ 1.0
    virtual void setZoomPosition(f32 pos) = 0;

    // 查询指令
    virtual void queryPanPosition()  = 0;
    virtual void queryTiltPosition() = 0;
    virtual void queryZoomPosition() = 0;

    virtual void sendRaw(const std::vector<u8> &data) = 0;
    virtual void sendData(std::span<u8> data)         = 0;
    virtual void receiveData(std::span<u8> data)      = 0;
};
```

> PELCO_D Protocol
https://www.pelco.com/
https://www.commfront.com/pages/pelco-d-protocol-tutorial

> PELCO_P Protocol
https://www.commfront.com/pages/pelco-p-protocol-tutorial

> VISCA Protocol
https://www.epiphan.com/userguides/LUMiO12x/Content/UserGuides/PTZ/3-operation/rs232.htm
