#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
#include <ctime>
#include <chrono>
#include <random>

class Users
{
    public:
        std::string name;
        std::string publicKey = "";
        int CatCoin;
};

std::vector<Users> user;

class Transactions
{
    public:
        std::string trans_ID;
        std::string sender;
        std::string reciever;
        int amount;
};

std::vector<Transactions> transaction;

class Blockchain
{
    std::string prev_hash;
    int timestamp;
    int version  = 1;
    std::string merkel_root_hash;
    int nonce;
    int difficulty_target;
    std::vector<Transactions> T;
    
};
std::random_device rd;
    std::mt19937::result_type seed = rd() ^ (
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
                ).count() +
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()
                ).count() );

    std::mt19937 gen(seed);

void generate_users()
{
    std::uniform_int_distribution<unsigned> rnd1(0, 15);
    std::uniform_int_distribution<unsigned> rnd2(100, 1000000);

    char symbols[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    for(int i = 0; i < 1000; i++)
    {
        Users tmp;
        tmp.name = "Name" + std::to_string(i);
        for(int j = 0; j < 64; j++)
        {
            tmp.publicKey = tmp.publicKey + symbols[rnd1(gen)];
        }
        tmp.CatCoin = rnd2(gen);
        user.push_back(tmp);

        //std::cout<< user[i].name << "\t" << user[i].publicKey << "\t" << user[i].CatCoin << std::endl;
    }
}


std::string hash_function(std::string str)
{
    int hash_length = 64;
    std::string result;
    std::string default_str = "O Fortuna velut luna statu variabilis, semper crescis aut dec...";
    std::string copy = default_str;
    

    if(str.empty())
    {
        for(int i = 0; i < hash_length; i++)
        {
            int a = 0;
                do
                {
                    a++;
                    copy[i] = (((copy[i] + a*i + a) ^ ((default_str[i] ^ i) + a)) & (default_str[(i * a - 1 * ~(i*a))%hash_length] + ~a)) % 128;
                } while ((copy[i] < '0' || copy[i] > '9') && (copy[i] < 'a' || copy[i] > 'f'));
        }
    }
    else
    {
        
        for(int i = 0; i < str.length(); i++)
        {
            for(int j = 0; j < hash_length; j++)
            {
                int a = 0;
                do
                {
                    a++;
                    copy[j] = (str[i] * str[i] + copy[j] + a + i * j * a + (copy[a * i % hash_length] * default_str[j]) ^ (str[a % str.length()] * copy[3])) % 128;
                } while ((copy[j] < '0' || copy[j] > '9') && (copy[j] < 'a' || copy[j] > 'f'));
            }
        }
    }
    
    
    return copy;
    
}


void generate_trans()
{
    std::uniform_int_distribution<unsigned> rnd1(0, 999);
    Transactions tmp;
    
    for(int i = 0; i < 10000; i++)
    {
        int r = rnd1(gen);
        tmp.sender = user[r].publicKey;
        if (user[r].CatCoin > 0)
        {
            int r2;
            do
            {
                r2 = rnd1(gen);
                tmp.reciever = user[r2].publicKey;
            } while(tmp.sender == tmp.reciever);
            
            std::uniform_int_distribution<unsigned> rnd2(1, user[r].CatCoin);
            tmp.amount = rnd2(gen);
            user[r].CatCoin = user[r].CatCoin - tmp.amount;
            user[r2].CatCoin = user[r2].CatCoin + tmp.amount;

            std::string str = "";
            str = tmp.sender + tmp.reciever + std::to_string(tmp.amount);
            tmp.trans_ID = hash_function(str);

            transaction.push_back(tmp);
            //std::cout << transaction[i].sender << " " << transaction[i].reciever << " " << transaction[i].amount << " " << transaction[i].trans_ID << "\n";
        }

        else i--;
    }

    std::cout << "Tranasakcijos sugeneruotos\n";
}