#include "internal27/data.hpp"
#include "internal27/entrprz_intl27_server.hpp"

using namespace intl27;
RefereeServer server;
int main(){
    bool serverSuccess = server.Start();
    if(!serverSuccess){
        std::cout<<"服务器建立失败！"<<std::endl;
        return 1;
    }
    while(true){
        Submit newSubmit = server.ListenForSubmit();
        std::cout<<"收到版本为 "<<newSubmit.version<<" 的答案，token为 "<<newSubmit.token<<std::endl;
        std::cout<<"答案"<<(newSubmit.correct ? "正确" : "错误")<<std::endl;
    }
    server.Stop();
    return 0;
}