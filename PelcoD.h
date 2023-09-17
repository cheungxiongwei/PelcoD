#pragma once
#include <numeric>
#include <vector>
#include <array>
#include "IPelcoD.h"

class PelcoD : public IPelcoD
{
public:
    PelcoD(u8 id)
      : mDeivceId(id) {}

    void panLeft(u8 speed) override;
    void panRight(u8 speed) override;
    void tiltUp(u8 speed) override;
    void tiltDown(u8 speed) override;
    void moveLeftUp(u8 speed) override;
    void moveRightUp(u8 speed) override;
    void moveLeftDown(u8 speed) override;
    void moveRightDown(u8 speed) override;
    void stopMotion() override;
    void zoomIn() override;
    void zoomOut() override;
    void focusIn() override;
    void focusOut() override;
    void irisIn() override;
    void irisOut() override;
    void setPreset(u8 presetID) override;
    void clearPreset(u8 presetID) override;
    void callPreset(u8 presetID) override;
    void setPanPosition(u16 pos) override;
    void setTiltPosition(u16 pos) override;
    void setZoomPosition(u16 pos) override;
    void setPanPosition(f32 pos /*[0.0,360.0]*/) override;
    void setTiltPosition(f32 pos /*[-90,90]*/) override;
    void setZoomPosition(f32 pos /*[0.00,1.00]*/) override;
    void queryPanPosition() override;
    void queryTiltPosition() override;
    void queryZoomPosition() override;

    // 发送用户自定义数据
    void sendRaw(const std::vector<u8>& data) override;

    // 继承重载该类，然后通过 socket 或 串口发送该数据。
    void sendData(std::span<u8> data) override;
    void receiveData(std::span<u8> data) override;

protected:
    inline uint8_t checksum(std::span<u8> data) {
        auto slice   = data.subspan(1, 5);
        uint32_t sum = std::accumulate(slice.begin(), slice.end(), 0);
        return uint8_t(sum % 0x100);
    }

    inline std::array<u8, 7> magic() { return std::array<u8, 7> {0xff, mDeivceId, 0x00, 0x00, 0x00, 0x00, 0x00}; }

private:
    u8 mDeivceId {0x01};
};
