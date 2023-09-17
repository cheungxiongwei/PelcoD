#pragma once

#include <cstdint>
#include <span>

namespace {
using u8  = uint8_t;
using u16 = uint16_t;
using f32 = float_t;
}  // namespace

// Pelco-D is a popular PTZ (Pan / Tilt / Zoom) camera control protocol used in the CCTV industry
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
