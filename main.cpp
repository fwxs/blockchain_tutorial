#include "src/blockchain.hpp"
#include <uuid/uuid.h>
#include <array>

static std::string node_identifier;
static Blockchain::blockchain blkchain;
static std::string self = "0";

void mine(){
    Blockchain::block last_block = blkchain.last_block();
    size_t proof = blkchain.proof_of_work(last_block);

    blkchain.new_transaction(Blockchain::transaction(self, node_identifier, 1));
    std::string previous_hash = blkchain.hash(last_block);
    Blockchain::block block = blkchain.new_block(proof, previous_hash);

    std::cout << "[*] New block created.\n";
    std::cout << "\t[+] Index: " << block.index() << '\n';
    std::cout << "\t[+] No Transactions: " << block.transactions().size() << '\n';
    std::cout << "\t[+] Proof: " << block.proof() << '\n';
    std::cout << "\t[+] Previous hash: " << block.previous_hash() << '\n';
}

void new_transaction(std::string &sender, std::string recipient, long unsigned int amount)
{
    size_t index = blkchain.new_transaction(Blockchain::transaction(sender, recipient, amount));
    std::cout << "[*] Transaction will be added to block " << index << std::endl;
}

void register_node(std::vector<std::string> &url_addresses){
    if(url_addresses.empty()){
        std::cerr << "[+] Please supply a list of nodes." << std::endl;
        return;
    }

    for(std::string url_address : url_addresses) {
        blkchain.register_node(url_address);
    }

    std::cout << "[*] " << url_addresses.size() << " nodes have been added." << std::endl;
}

int main()
{
    uuid_t uuid;
    char out[37];
    char sndr[37];

    uuid_generate_random(uuid);
    uuid_unparse(uuid, out);

    node_identifier = out;

    int counter = 0;
    int sub_counter = 0;

    while(counter != 15){
        mine();

        while(sub_counter != 10){
            uuid_generate_random(uuid);
            uuid_unparse(uuid, sndr);

            std::string sender(sndr);

            new_transaction(sender, node_identifier, std::time(nullptr));

            ++sub_counter;
        }

        sub_counter = 0;
        ++counter;
    }

    std::vector<std::string> url_addresses;

    url_addresses.push_back("test-ui.uieq.com");
    url_addresses.push_back("test-uk.ukeq.com");
    url_addresses.push_back("http://test-ul.uleq.com");
    url_addresses.push_back("test-um.umeq.com");
    url_addresses.push_back("https://test-un.uneq.com");

    register_node(url_addresses);

    return 0;
}
