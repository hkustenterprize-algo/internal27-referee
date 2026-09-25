#pragma once
#include <string_view>
#include <string>
#include <cstdint>
#include <sstream>

namespace intl27{

// 通讯端点，若测试时端口被占用可手动修改 SERVER_PORT 并编译
inline constexpr std::string_view SUBMIT_PATH = "/submit";
inline constexpr uint16_t SERVER_PORT = 12019;

struct Submit{
    // 当前头文件的版本号，随 internal 规则改动或 api 改动变化
    inline static constexpr std::string_view VER = "0.0.1-alpha";
    // Submit 版本号，发送时无须手动填入
    std::string version;
    // 每队在赛前收到的 token，用于辨别身份
    uint64_t token;
    // 实际提交信息，目前仅为占位
    bool correct;

    std::string GeneratePayload(){
        std::ostringstream payload;
        payload<<VER<<" "<<token<<" "<<correct;
        return payload.str();
    }
    Submit() = default;
    Submit(const std::string& payload){
        std::istringstream payloadStream(payload);
        payloadStream>>version>>token>>correct;
    }
};

}