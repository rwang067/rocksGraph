#include "api/nvmgraph_basic_include.hpp"
#include "importgraph.hpp"

int main(int argc, const char ** argv) {

    set_argc(argc, argv);

    /* Metrics object for keeping track of performance count_invectorers
     and other information. Currently required. */
    metrics m("nvmgraph-db");

    m.start_time("testdb");

    rocksdb::DB* db;
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::Status status = rocksdb::DB::Open(options, "/Users/rui/Downloads/datasets/sf1db", &db);
    assert(status.ok());
    std::cout << "Open rocksdb success." << std::endl;

    importGraph("/Users/rui/Downloads/datasets/SF1", db, m);

    std::string key3 = "4139|lastName", value3;
    status = db->Get(rocksdb::ReadOptions(), key3, &value3);
    assert(status.ok());
    logstream(LOG_INFO) << "Get key[" << key3 << "] = " << value3 << std::endl;

    delete db;

    m.stop_time("testdb");

    /* Report execution metrics */
    metrics_report(m);

    return 0;
}


