# self-made-blockchain

Self-made blockchain, written in c++

## Installation instructions

 * Download the latest release code here ([v0.2](https://github.com/gertruda1/self-made-blockchain/releases/tag/v0.2))
 * Install g++ compiler ([GCC](https://gcc.gnu.org/)).
 * Compile program using command line *g++ main.cpp -o exe*
 * Run program *exe*.

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
  
  * When block is added, used transactions are executed - deleted from transaction pool.
  
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
