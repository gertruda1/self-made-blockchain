#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
#include <ctime>
#include <chrono>
#include <random>
#include<iterator> // for back_inserter

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

class Block
{
    public:
        std::string prev_hash;
        int64_t timestamp;
        std::string version;
        std::string merkel_root_hash;
        int nonce;
        std::string difficulty_target;
        std::vector<Transactions> T;
        Block* next;
        
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


void generate_trans(std::vector<Transactions> &transaction)
{
    std::uniform_int_distribution<unsigned> rnd1(0, 999);
    Transactions tmp;
    
    for(int i = 0; i < 1000; i++)
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

std::string merkle_root_hash(std::vector<std::string> h1)
{
    while(h1.size() != 1)
    {
        std::vector<std::string> h2;
        if(h1.size() % 2 != 0)
        {
            h1.push_back(h1.at(h1.size() - 1));
            //std::cout << h1.at(h1.size() - 1) << " " << h1.at(h1.size() - 2) << std::endl;
        }
        for(int i = 0; i < h1.size() - 1; i += 2)
        {
            std::string str = h1[i] + h1[i + 1];
            std::string hash;
            hash = hash_function(str);
            h2.push_back(hash);
        }
        h1.clear();
        copy(h2.begin(), h2.end(), back_inserter(h1));
        //for(int i = 0; i < h1.size(); i++) std::cout << i << " " << h1[i] << std::endl;
        
    }
    
    return h1.at(0);
}


void addblock(Block* &blokelis, std::vector<Transactions> &Trans)
{

    // std::string prev_hash;
    //    int nonce;
    if(!blokelis)
    {
        blokelis = new Block;

    int64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    blokelis->timestamp = time;
    blokelis->version = "1st";
    blokelis->difficulty_target = "00";
    blokelis->nonce = 0;

    auto start = Trans.begin(); 
    auto end = Trans.begin() + 100; 
  
    std::vector<Transactions> Tr(100); 
  
    copy(start, end, Tr.begin()); 

    
    Trans.erase (Trans.begin(),Trans.begin()+100);

    blokelis->T = Tr;

    //here we hash transactions
    std::vector<std::string> v_string;
    for(int i = 0; i < 100; i++)
    {
        v_string.push_back(hash_function(blokelis->T[i].trans_ID));
    }                            //TODO: MERKEL ROOT HASH

    blokelis->merkel_root_hash = merkle_root_hash(v_string);
    

    blokelis->prev_hash = "0";


    blokelis->next = NULL;

    std::cout << blokelis->prev_hash << std::endl;
    }

    else
    {
        Block *t = blokelis;
        int n = 0;
        while(t->next)
        {
            t = t->next;
            n++;
        }
        t->next = new Block;
        t = t->next;
        Block *prev = blokelis;
        int i = 0;
        while(i < n)
        {
            prev = prev->next;
            i++;
        }


        int64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        t->timestamp = time;
        t->version = "1st";
        t->difficulty_target = "00";
        t->nonce = 0;


        auto start = Trans.begin(); 
        auto end = Trans.begin() + 100; 
    
        std::vector<Transactions> Tr(100); 
    
        copy(start, end, Tr.begin()); 

        
        Trans.erase (Trans.begin(),Trans.begin()+100);

        t->T = Tr;

        //here we hash transactions
        std::vector<std::string> v_string;
        for(int i = 0; i < 100; i++)
        {
            v_string.push_back(hash_function(t->T[i].trans_ID));
        }                            //TODO: MERKEL ROOT HASH

        t->merkel_root_hash = merkle_root_hash(v_string);

        std::string str = prev->difficulty_target + prev->merkel_root_hash + prev->prev_hash + std::to_string(prev->timestamp) + prev->version;

        do
        {
            prev->nonce++;
            std::string stringtohash = str + std::to_string(prev->nonce);
            t->prev_hash = hash_function(stringtohash);
        }while(t->prev_hash.substr(0, 2) != "00");

        std::cout << t->prev_hash << std::endl;
        t->next = NULL;
    }
    
    

}



void print(Block *root) {
  Block *t = root;
  while (t) {
    std::cout << t->prev_hash << " ";
    t = t->next;
  }
  std::cout << std::endl;
}