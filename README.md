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

> PELCO_D Protocol
https://www.pelco.com/
https://www.commfront.com/pages/pelco-d-protocol-tutorial

> PELCO_P Protocol
https://www.commfront.com/pages/pelco-p-protocol-tutorial

> VISCA Protocol
https://www.epiphan.com/userguides/LUMiO12x/Content/UserGuides/PTZ/3-operation/rs232.htm
