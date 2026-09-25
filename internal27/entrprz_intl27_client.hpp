#pragma once

#include <string>

#include "data.hpp"
#include "httplib.h"

namespace intl27{

class RefereeClient{
public:
    RefereeClient() = default;
    RefereeClient(const std::string& ip){
        SetServer(ip);
    }

    // 设置服务器的 ip，端口采用约定的协议端口
    void SetServer(const std::string& ip){
        client_ = std::make_unique<httplib::Client>("http://" + ip + ":" + std::to_string(SERVER_PORT));
        client_->set_connection_timeout(3);
        client_->set_read_timeout(3);
        return ;
    }

    // 发送一次答案
    bool Send(Submit submit){
        if(!client_){
            return false;
        }
        
        auto result = client_->Post(
            SUBMIT_PATH.data(),
             submit.GeneratePayload(), 
             "text/plain"
        );
        return result && (result->status == 200);
    }

private:
    std::unique_ptr<httplib::Client> client_;
};

} // namespace intl27
