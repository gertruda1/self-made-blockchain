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
#include <string>
#include <bitcoin/bitcoin.hpp>

struct Users
{
    std::string name;
    std::string publicKey = "";
    int CatCoin;
};

std::vector<Users> user;

struct Transactions
{
    std::string trans_ID;
    std::string sender;
    std::string reciever;
    int amount;
};

std::vector<Transactions> transaction;

struct Block
{
        std::string prev_hash;
        std::string hash;
        int64_t timestamp;
        std::string version;
        bc::hash_digest merkel_root_hash;
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
            
            std::uniform_int_distribution<unsigned> rnd2(1, 100000);
            tmp.amount = rnd2(gen);

            std::string str = "";
            str = tmp.sender + tmp.reciever + std::to_string(tmp.amount);
            tmp.trans_ID = hash_function(str);

            transaction.push_back(tmp);
            //std::cout << transaction[i].sender << " " << transaction[i].reciever << " " << transaction[i].amount << " " << transaction[i].trans_ID << "\n";
        }

    std::cout << "Tranasakcijos sugeneruotos\n";
}

void validate_trans() //  ar tranksakcijos informacijos hash'assutampa su transakcijos ID
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


bc::hash_digest create_merkle(bc::hash_list& merkle)
{
    // Stop if hash list is empty or contains one element
    if (merkle.empty())
    return bc::null_hash;
    else if (merkle.size() == 1)
    return merkle[0];
    // While there is more than 1 hash in the list, keep looping...
    while (merkle.size() > 1)
    {
        // If number of hashes is odd, duplicate last hash in the list.
        if (merkle.size() % 2 != 0)
        merkle.push_back(merkle.back());
        // List size is now even.
        assert(merkle.size() % 2 == 0);
        // New hash list.
        bc::hash_list new_merkle;
        // Loop through hashes 2 at a time.
        for (auto it = merkle.begin(); it != merkle.end(); it += 2)
        {
            // Join both current hashes together (concatenate).
            bc::data_chunk concat_data(bc::hash_size * 2);
            auto concat = bc::serializer<
            decltype(concat_data.begin())>(concat_data.begin());
            concat.write_hash(*it);
            concat.write_hash(*(it + 1));
            // Hash both of the hashes.
            bc::hash_digest new_root = bc::bitcoin_hash(concat_data);
            // Add this to the new list.
            new_merkle.push_back(new_root);
        }
        // This is the new list.
        merkle = new_merkle;
        // DEBUG output -------------------------------------
        //std::cout << "Current merkle hash list:" << std::endl;
        //for (const auto& hash: merkle)
        //std::cout << " " << bc::encode_base16(hash) << std::endl;
        //std::cout << std::endl;
        // --------------------------------------------------
    }
    // Finally we end up with a single item.
    return merkle[0];
}

bool mark_to_delete(Transactions a)
{
    bool z;
    if(a.trans_ID == "000") z = true;
    return z;
}

Block* powHash(std::vector<Block*> b, int j)
{
    int a = -1;
    std::random_shuffle ( b.begin(), b.end() );
    for(int i = 0; i < 5; i++)
    {
        //std::cout << "is kelinto karto: " << i << std::endl; 
        std::string str = b[i]->difficulty_target + bc::encode_base16(b[i]->merkel_root_hash) + b[i]->prev_hash + std::to_string(b[i]->timestamp) + b[i]->version;
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
    std::vector<Transactions> Tra;
    //Here we create new vector with 100 transactions in it
    std::vector<Transactions> tmp;
    copy(transaction.begin(), transaction.end(), back_inserter(tmp));


    for(int i = 0; i < 100; i++)
    {
        std::uniform_int_distribution<unsigned> rnd(0, tmp.size() - 1);
        int r = rnd(gen);
        Tra.push_back(tmp[r]);
        tmp.erase(tmp.begin() + r);
        if (tmp.size() == 0) i = 100;
        //std::cout << i << " " << Tr[i].amount << std::endl;
    }
    tmp.clear();
    
    //Here we are going to check the balance
    std::vector<Users> tmpu;
    copy(user.begin(), user.end(), back_inserter(tmpu));
    for(int i = 0; i < Tra.size(); i++)
    {
        int a = -1, c = -1;
        for(int j = 0; j < tmpu.size(); j++)
        {
            //std::cout << Tr[i].reciever << " " << tmpu[j].publicKey << std::endl;
            if(Tra[i].reciever == tmpu[j].publicKey) a = j;
            if (Tra[i].sender == tmpu[j].publicKey) c = j;

            if(a != -1 && c != -1)
            {
                
                if(Tra[i].amount > tmpu[c].CatCoin)
                {
                    
                    int z = 0;
                    while(Tra[i].trans_ID != transaction[z].trans_ID)
                    {
                        z++;
                    }
                    Tra[i].trans_ID = "000";
                    
                    transaction.erase(transaction.begin() + z);
                    
                }
                else
                {
                    tmpu[c].CatCoin -= Tra[i].amount;
                    tmpu[a].CatCoin += Tra[i].amount;
                }
                j = tmpu.size();
            }
            
        }
        
    }
    
    Tra.erase(std::remove_if(Tra.begin(), Tra.end(), mark_to_delete), Tra.end());

    return Tra;
}


void generate_block(Block* &b, std::string prevhash, std::vector<Transactions> Tr) //sugeneruosim bloko pagrinda
{
    
        int64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        b->prev_hash = prevhash;
        b->timestamp = time;
        b->version = "1st";
        b->difficulty_target = "00";
        b->nonce = 0;
        b->T = Tr;
        //here we hash transactions
        bc::hash_list tx_hashes {};
        for(int i = 0; i < b->T.size() - 1; i++)
        {
            char arr[65];
            b->T[i].trans_ID.copy(arr, 65);
            tx_hashes.push_back(bc::hash_literal(arr));
        }                            

        b->merkel_root_hash = create_merkle(tx_hashes);
            

}

void generate_block(Block* &b, std::vector<Transactions> Tr)
{
        int64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        b->timestamp = time;
        b->version = "1st";
        b->difficulty_target = "00";
        b->nonce = 0;
        b->T = Tr;
        //here we hash transactions
        //std::cout << "as cia1\n";
        bc::hash_list tx_hashes {};
        for(int i = 0; i < b->T.size() - 1; i++)
        {
            char arr[65];
            b->T[i].trans_ID.copy(arr, 65);
            tx_hashes.push_back(bc::hash_literal(arr));
        }                            

        b->merkel_root_hash = create_merkle(tx_hashes);
            

        b->prev_hash = "0";
}

Block* prepare_block(bool is_first, std::string prevhash) //patikrinsim su pow ir grazinsim ta bloka, kuri jau desim i blockchaina
{
    std::vector<Transactions> Tr;
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
            if (is_first == false)
           
            generate_block(block.at(i), prevhash, Tr);
        }
    }
    return powHash(block, 100000);

}

void execute_transactions(std::vector<Transactions> tr)
{


    for(int i = 0; i < tr.size(); i++)
        {
            int a = -1, b = -1;
            for(int j = 0; j < user.size(); j++)
            {
                if(tr[i].reciever == user[j].publicKey) a = j;
                if(tr[i].sender == user[j].publicKey) b = j;

                if(a != -1 && b != -1)
                {
                    user[b].CatCoin -= tr[i].amount;
                    user[a].CatCoin += tr[i].amount;
                }
            }

            for(int j = 0; j < transaction.size(); j++)
            {
                if(tr[i].trans_ID == transaction[j].trans_ID)
                {
                    transaction.erase(transaction.begin() + j);
                    j = transaction.size();
                }
            }
        }
}


void addblock(Block* &b)
{

    if(!b)
    {
        bool is_first = true;
        b = new Block;
        std::string lol;
        b = prepare_block(is_first, lol);
        //std::cout << b->hash << std::endl;
        b->next = NULL;

        execute_transactions(b->T);


        // print block

        std::cout << "\nBlock is added to blockchain\nBlock information:\n\nPrevious block hash: " << b->prev_hash;
        std::cout << "\nTimestamp: " << b->timestamp << "\nVersion: " << b->version << "\nMerkle root hash: " << bc::encode_base16(b->merkel_root_hash);
        std::cout << "\nNonce: " << b->nonce << "\nDifficulty target: " << b->difficulty_target << std::endl;
        std::cout << "Hash: " << b->hash;

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
            std::string prevhash = prev->hash;
            t = prepare_block(is_first, prevhash);
            //std::cout << t->hash << std::endl;
            t->next = NULL;

            execute_transactions(t->T);

            // print block

            std::cout << "\nBlock is added to blockchain\nBlock information:\n\nPrevious block hash: " << t->prev_hash;
            std::cout << "\nTimestamp: " << t->timestamp << "\nVersion: " << t->version << "\nMerkle root hash: " << bc::encode_base16(t->merkel_root_hash);
            std::cout << "\nNonce: " << t->nonce << "\nDifficulty target: " << t->difficulty_target << std::endl;
            std::cout << "Hash: " << t->hash;
    } 
    
}

