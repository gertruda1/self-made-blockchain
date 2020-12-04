# self-made-blockchain

Self-made blockchain, written in c++. In this version I have used Merkle tree function create_merkle() which is based on [libbitcoin library](https://github.com/libbitcoin/libbitcoin-system)
## Installation instructions

 * Download the program code.
 * Download [libbitcoin library](https://github.com/libbitcoin/libbitcoin-system).
 * Install c++ compiler
 
 `sudo apt-get update`
 
 `sudo apt-get install build-essential manpages-dev`
 * Compile program using command line
 
 `g++ -std=c++11 -o exe block.cpp $(pkg-config --cflags --libs libbitcoin)`
 
 * Run program
 
 `./exe`

 ## What does this programm do?
 
  * Generates 1000 network users, who have three atributes:
    * name
    * public key
    * balance
    
  * Generates a pool of 10 000 transactions that have:
    * transaction ID
    * sender (user's public key)
    * reciever (user's public key)
    * amount of money that is being sent
  
  * Transaction verification
    * Balance check: if user A sends a certain amount that is more than the amount of his balance to user B, transaction is being deleted from the transaction pool.
    * Transaction hash check: it is checked if hashed transaction information matches transaction ID, if not - the transaction is deleted from the transaction pool.
    * It is secured that the same transaction is not repeated in the same block.
    
  * Generates five potential block candidates with ~100 transactions.
  
  * Randomly selects one of the five generated potential block candidates and mines it until if fulfills Proof of Work quality within 100000 tries.
    * If block is mined - it is added to the blockchain, if not the process is repeated with another randomly chosen potential block.
  
  * When block is added, used transactions are executed and deleted from transaction pool.
  
 Generated block contains:
  
  * Header
    * Previous block hash
    * Current block hash
    * Timestamp
    * Version
    * Merkle root hash
    * Nonce
    * Difficulty target
  
  * Transactions list
  
Blockchain is being kept in linked list.
