#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include "data.hpp"
#include "httplib.h"

namespace intl27{

// TODO: 重写 RefereeServer，当前是纯 vibe coding 的
class RefereeServer{
public:
    RefereeServer() = default;
    ~RefereeServer(){
        Stop();
    }

    RefereeServer(const RefereeServer&) = delete;
    RefereeServer& operator=(const RefereeServer&) = delete;

    bool Start(){
        std::lock_guard<std::mutex> lk(mtx_);
        if(serving_){
            return false;
        }
        auto newServer = std::make_unique<httplib::Server>();
        newServer->Post(
            SUBMIT_PATH.data(),
            [this](const httplib::Request& req,httplib::Response& res)
            {
                {
                    std::lock_guard<std::mutex> lk(mtx_);
                    queue_.emplace(req.body);
                }
                cv_.notify_one();
                res.set_content("ok", "text/plain");
            }
        );
        if(!newServer->bind_to_port("0.0.0.0", SERVER_PORT)){
            return false;
        }
        server_ = std::move(newServer);
        serving_ = true;
        thread_ = std::thread([this] {server_->listen_after_bind();});
        return true;
    }

    void Stop(){
        httplib::Server* svr = nullptr;
        {
            std::lock_guard<std::mutex> lk(mtx_);
            if (!serving_) return;
            serving_ = false;
            svr = server_.get();
        }
        svr->stop();
        if(thread_.joinable()){
            thread_.join();
        }
        {
            std::lock_guard<std::mutex> lk(mtx_);
            server_.reset();
        }
        cv_.notify_all();
    }

    Submit ListenForSubmit(){
        std::unique_lock<std::mutex> lk(mtx_);
        cv_.wait(lk, [this] {return !queue_.empty() || !serving_;});
        if(queue_.empty()){
            return {};
        }
        Submit s = std::move(queue_.front());
        queue_.pop();
        return s;
    }

private:
    std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<Submit> queue_;
    std::unique_ptr<httplib::Server> server_;
    std::thread thread_;
    bool serving_ = false;
};

} // namespace intl27
