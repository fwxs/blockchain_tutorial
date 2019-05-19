#include "blockchain.hpp"
#include <regex>

using namespace Blockchain;

static size_t length = 0;
static std::string guess_hash;
static std::stringstream str_stream;


std::string blockchain::hash(const block &blk)
{
    str_stream.str("");
    str_stream << blk;
    return sha256(str_stream.str());
}

static std::string extract_url_netloc(const std::string &url_address)
{
    length = url_address.length();
    size_t schema_inx = url_address.find("//", 0);
    size_t netloc_inx = 0;

    if(schema_inx < length)
        netloc_inx = url_address.find_first_of('/', schema_inx);

    if(netloc_inx != 0 && netloc_inx < length)
        return url_address.substr(netloc_inx + 2, url_address.find_first_of('/', length - 1));

    return url_address;
}

block blockchain::new_block(const unsigned long proof, const std::string &init_val)
{
    block blk(blockchain::chain.size() + 1,
              std::time(nullptr),
              blockchain::current_transactions,
              proof,
              init_val);

    blockchain::current_transactions.clear();
    blockchain::chain.push_back(blk);

    return blk;
}

block blockchain::new_block(const unsigned long proof)
{
    block blk(blockchain::chain.size() + 1,
              std::time(nullptr),
              blockchain::current_transactions,
              proof,
              blockchain::hash(blockchain::last_block()));

    blockchain::current_transactions.clear();
    blockchain::chain.push_back(blk);

    return blk;
}

size_t blockchain::new_transaction(const transaction &transact)
{
    blockchain::current_transactions.push_back(transact);
    return blockchain::last_block().index();
}

void blockchain::register_node(const std::string &url_address)
{
    std::string parsed_url = extract_url_netloc(url_address);
    blockchain::nodes.insert(parsed_url);
}

static bool valid_proof(const size_t last_proof, const long unsigned int proof, const std::string &last_hash)
{
    str_stream.str("");

    str_stream << last_proof << proof << last_hash;

    guess_hash = sha256(str_stream.str());
    length = guess_hash.length();

    return guess_hash.substr(length - 4, length - 1) == "0000";
}

long unsigned int blockchain::proof_of_work(const block last_block)
{
    size_t last_proof = last_block.proof();
    std::string last_hash = blockchain::hash(last_block);

    long unsigned int proof = 0;

    while (valid_proof(last_proof, proof, last_hash)) {
        ++proof;
    }

    return proof;
}

bool blockchain::valid_chain(const std::vector<block> &new_chain)
{
    block blk = new_chain.front();
    block last_block = new_chain.back();
    std::string last_block_hash;

    for(block new_blk : new_chain){
        blk = new_blk;

        last_block_hash = blockchain::hash(last_block);

        if(blk.previous_hash() != last_block_hash)
            return false;

        if(valid_proof(last_block.proof(), blk.proof(), last_block_hash))
            return false;

        last_block = blk;
    }

    return true;
}
