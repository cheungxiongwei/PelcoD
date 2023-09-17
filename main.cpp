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
