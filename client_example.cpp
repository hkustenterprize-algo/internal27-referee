#include "internal27/data.hpp"
#include "internal27/entrprz_intl27_client.hpp"

using namespace intl27;
RefereeClient client("127.0.0.1");
int main(){
    std::string str;
    while(std::cin>>str){
        Submit newSubmit;
        newSubmit.token = 20061108;
        newSubmit.correct = (str == "correct");
        bool success = client.Send(newSubmit);
        if(success){
            std::cout<<"发送成功，答案: "<<newSubmit.correct<<std::endl;
        }else{
            std::cout<<"发送失败！答案: "<<newSubmit.correct<<std::endl;
        }
    }
    return 0;
}