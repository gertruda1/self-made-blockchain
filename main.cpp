#include "generate.h"

int main() 
{
    std::cout << "useriai\n";
    generate_users();
    std::cout<<"transakcijos\n";
    generate_trans();
    validate_trans();
    std::cout << "transakcijos patikrintos\n";
    
    
    
    Block* jo = NULL;
    int i = 1;



    while(!transaction.empty()){
        std::cout << i << " ";
        addblock(jo);
        i++;
    }
    return 0;
}