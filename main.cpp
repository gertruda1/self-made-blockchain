#include "generate.h"

int main() 
{
    std::cout << "useriai\n";
    generate_users();
    std::cout<<"transakcijos\n";
    generate_trans(transaction);
    std::cout<<"transakcijos baigesi\n";
    Block* jo = NULL;
    int i = 1;
    while(!transaction.empty()){
        std::cout << i << " ";
        addblock(jo, transaction);
        i++;
    }

    return 0;
}