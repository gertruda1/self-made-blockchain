#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
#include <ctime>
#include <chrono>
#include <random>
#include <algorithm>
#include <iterator>

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
        std::string hash;
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


void generate_trans()
{
    std::uniform_int_distribution<unsigned> rnd1(0, 999);
    Transactions tmp;
    
    for(int i = 0; i < 1000; i++)
    {
        int r = rnd1(gen);
        tmp.sender = user[r].publicKey;
        
        int r2;
        do
        {
            r2 = rnd1(gen);
            tmp.reciever = user[r2].publicKey;
        } while(tmp.sender == tmp.reciever);
            
            std::uniform_int_distribution<unsigned> rnd2(1, 10);
            tmp.amount = rnd2(gen);

            std::string str = "";
            str = tmp.sender + tmp.reciever + std::to_string(tmp.amount);
            tmp.trans_ID = hash_function(str);

            transaction.push_back(tmp);
            //std::cout << transaction[i].sender << " " << transaction[i].reciever << " " << transaction[i].amount << " " << transaction[i].trans_ID << "\n";
        }

    std::cout << "Tranasakcijos sugeneruotos\n";
}

void validate_trans()
{
    for(int i = 0; i < transaction.size(); i++)
    {
        std::string str = "";
        str = transaction[i].sender + transaction[i].reciever + std::to_string(transaction[i].amount);
        if(transaction[i].trans_ID != hash_function(str))
        {
            transaction.erase(transaction.begin() + i);
        }
    }
}

std::string merkle_root_hash(std::vector<std::string> v_string)
{
    std::vector<std::string> h1;
     copy(v_string.begin(), v_string.end(), back_inserter(h1));
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
            std::string hash = hash_function(hash_function(h1[i]) + hash_function(h1[i + 1]));
            h2.push_back(hash);
        }
        h1.clear();
        copy(h2.begin(), h2.end(), back_inserter(h1));
        //for(int i = 0; i < h1.size(); i++) std::cout << i << " " << h1[i] << std::endl;
        
    }
    
    return h1.at(0);
}

bool mark_to_delete(Transactions a)
{
    bool z;
    if(a.trans_ID == "000") z = true;
    return z;
}

Block* powHash(std::vector<Block*> &b, int j)
{
    int a = -1;
    for(int i = 0; i < 5; i++)
    {
        std::string str = b[i]->difficulty_target + b[i]->merkel_root_hash + b[i]->prev_hash + std::to_string(b[i]->timestamp) + b[i]->version;
        do
        {
            b[i]->nonce++;
            std::string stringtohash = str + std::to_string(b[i]->nonce);
            b[i]->hash = hash_function(stringtohash);
        } while (b[i]->nonce < j && b[i]->hash.substr(0, 2) != "00");

        if(b[i]->hash.substr(0,2) == "00")
        {
            a = i; 
            i = 5;
        }
    }

    if(a != -1)
    {
        return b.at(a);
    }
    else return powHash(b, j + 100000);
}

std::vector<Transactions> prepare_transactions()
{
    std::vector<Transactions> Tr;
    //Here we create new vector with 100 transactions in it
    std::vector<Transactions> tmp;
    copy(transaction.begin(), transaction.end(), back_inserter(tmp));


    for(int i = 0; i < 100; i++)
    {
        std::uniform_int_distribution<unsigned> rnd(0, tmp.size() - 1);
        int r = rnd(gen);
        Tr.push_back(tmp[r]);
        tmp.erase(tmp.begin() + r);
        
        //std::cout << i << " " << Tr[i].amount << std::endl;
    }
    tmp.clear();
    
    //Here we are going to check the balance
    std::vector<Users> tmpu;
    copy(user.begin(), user.end(), back_inserter(tmpu));
    for(int i = 0; i < Tr.size(); i++)
    {
        int a = -1, c = -1;
        for(int j = 0; j < tmpu.size(); j++)
        {
            //std::cout << Tr[i].reciever << " " << tmpu[j].publicKey << std::endl;
            if(Tr[i].reciever == tmpu[j].publicKey) a = j;
            if (Tr[i].sender == tmpu[j].publicKey) c = j;

            if(a != -1 && c != -1)
            {
                
                if(Tr[i].amount > tmpu[c].CatCoin)
                {
                    
                    int z = 0;
                    while(Tr[i].trans_ID != transaction[z].trans_ID)
                    {
                        z++;
                    }
                    Tr[i].trans_ID = "000";
                    
                    transaction.erase(transaction.begin() + z);
                    
                }
                else
                {
                    tmpu[c].CatCoin -= Tr[i].amount;
                    tmpu[a].CatCoin += Tr[i].amount;
                }
                j = tmpu.size();
            }
            
        }
        
    }
    
    Tr.erase(std::remove_if(Tr.begin(), Tr.end(), mark_to_delete), Tr.end());

    return Tr;
}


void generate_block(Block* &b, std::string prevhash, std::vector<Transactions> Tr) //sugeneruosim bloko pagrinda
{
    

        int64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        b->timestamp = time;
        b->version = "1st";
        b->difficulty_target = "00";
        b->nonce = 0;
        b->T = Tr;
    std::cout << "as cia2\n";
        //here we hash transactions
        std::vector<std::string> v_string;
        for(int i = 0; i < b->T.size() - 1; i++)
        {
            v_string.push_back(b->T[i].trans_ID);
        }                            

        b->merkel_root_hash = merkle_root_hash(v_string);
            
        b->prev_hash = prevhash;

}

void generate_block(Block* &b, std::vector<Transactions> Tr) //sugeneruosim bloko pagrinda
{
        int64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        b->timestamp = time;
        b->version = "1st";
        b->difficulty_target = "00";
        b->nonce = 0;
        b->T = Tr;
        //here we hash transactions
        std::cout << "as cia1\n";
        std::vector<std::string> v_string;
        for(int i = 0; i < b->T.size() - 1; i++)
        {
            v_string.push_back(b->T[i].trans_ID);
        }                            

        b->merkel_root_hash = merkle_root_hash(v_string);
            

        b->prev_hash = "0";
}

Block* prepare_block(bool is_first, Block* b, std::vector<Transactions> &Tr) //patikrinsim su pow ir grazinsim ta bloka, kuri jau desim i blockchaina
{
    std::vector<Block*> block;
    
    for(int i = 0; i < 5; i++)
    {
        
        Tr = prepare_transactions();
        if (Tr.size() > 0) // tada galim kurti nauja bloka, jei yra transakciju
        {
            Block* tmp = new Block;
            block.push_back(tmp);
            if(is_first == true)
            {
                generate_block(block.at(i), Tr);
            }
            else
            generate_block(block.at(i), b->hash, Tr);
        }
    }

    return powHash(block, 100000);

}

void addblock(Block* &b)
{

    std::vector<Transactions> Tr;


    if(!b)
    {
        bool is_first = true;
        b = new Block;
        b = prepare_block(is_first, b, Tr);
        std::cout << b->hash << std::endl;
        b->next = NULL;

        
    }

    else
    {
        bool is_first = false;
            Block *t = b;
            Block *prev = b;
            while(t->next)
            {
                t = t->next;
                prev = prev->next;
            }
            t->next = new Block;
            t = t->next;
            
            t = prepare_block(is_first, prev, Tr);
            std::cout << t->hash << std::endl;
            t->next = NULL;
    }
        
        
        
        //Execute transactions

        
 
    
}



void print(Block *root) {
  Block *t = root;
  while (t) {
    std::cout << t->prev_hash << " ";
    t = t->next;
  }
  std::cout << std::endl;
}