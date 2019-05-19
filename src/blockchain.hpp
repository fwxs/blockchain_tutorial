#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP

#include "sha256.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <set>
#include <ctime>
#include <vector>


namespace Blockchain
{
    class EmptyVectorException : public std::exception
    {
        public:
            const char * what() const noexcept
            {
                return "Vector is empty!";
            }
    };

    class transaction{
        private:
            long unsigned int _amount = 0;
            std::string _recipient;
            std::string _sender;

        public:
            transaction(std::string &sender, std::string &recipient, long unsigned int amount)
            {
                this->_sender = sender;
                this->_recipient = recipient;
                this->_amount = amount;
            }

            transaction(const char* sender, const char* recipient, long unsigned int amount)
            {
                this->_sender = sender;
                this->_recipient = recipient;
                this->_amount = amount;
            }

            const long unsigned int &amount() const { return _amount; }
            const std::string &recipient() const { return _recipient; }
            const std::string &sender() const { return _sender; }

            inline friend std::ostream& operator << (std::ostream &out, const transaction &transact){
                out << std::to_string(transact.amount()) << transact.recipient() << transact.sender();
                return out;
            }
    };

    class block
    {
        private:
            size_t _index = 0;
            std::string _previous_hash;
            long unsigned int _proof = 0;
            time_t _timestamp = 0;
            std::vector<transaction> _transactions;

        public:
            block();

            block(const size_t index,
                  const time_t timestamp,
                  std::vector<transaction> &transactions,
                  const long unsigned int proof,
                  const std::string &previous_hash)
            {
                this->_index = index;
                this->_timestamp = timestamp;
                this->_transactions = transactions;
                this->_proof = proof;
                this->_previous_hash = previous_hash;
            }

            const size_t &index() const { return _index; }
            const std::string &previous_hash() const { return _previous_hash; }
            const long unsigned int &proof() const { return _proof; }
            const time_t &timestamp() const { return _timestamp; }
            const std::vector<transaction> &transactions() const { return _transactions; }

            inline friend std::ostream& operator << (std::ostream &out, const block &blk)
            {
                out << std::to_string(blk.index()) << blk.previous_hash()
                    << std::to_string(blk.proof()) << std::to_string(blk.timestamp());

                for(Blockchain::transaction transact : blk.transactions())
                {
                    out << transact;
                }

                return out;
            }
    };


    class blockchain
    {
        private:
            std::vector<block> chain;
            std::vector<transaction> current_transactions;
            std::set<std::string, std::greater<std::string>> nodes;

        public:
            blockchain() { blockchain::new_block(100, "1"); }

            inline block last_block(){
                if(!chain.empty())
                    return chain.back();

                throw Blockchain::EmptyVectorException();
            }

            std::string hash(const block &blk);
            void register_node(const std::string &url_address);
            bool valid_chain(const std::vector<block> &new_chain);
            bool resolve_conflicts(void);
            block new_block(const long unsigned int proof, const std::string &previous_hash);
            block new_block(const unsigned long proof);
            size_t new_transaction(const transaction &transact);
            long unsigned int proof_of_work(const block last_block);

    };
} // namespace Blockchain


#endif // BLOCKCHAIN_HPP
