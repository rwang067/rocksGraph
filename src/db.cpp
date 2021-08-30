#include "api/nvmgraph_basic_include.hpp"

using namespace std;

int main() {
    /* Metrics object for keeping track of performance count_invectorers
     and other information. Currently required. */
    metrics m("nvmgraph-db");

    m.start_time("testdb");

    rocksdb::DB* db;
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::Status status = rocksdb::DB::Open(options, "/tmp/testdb", &db);
    assert(status.ok());
    cout << "Open rocksdb success." << endl;

    string key1 = "1";
    string value1 = "aaaaa";
    status = db->Put(rocksdb::WriteOptions(), key1, value1);
    assert(status.ok());
    printf("Put[%s,%s] success.\n", key1.c_str(), value1.c_str());
    
    string value;
    status = db->Get(rocksdb::ReadOptions(), key1, &value);
    assert(status.ok());
    printf("Get key[%s] = %s\n", key1.c_str(), value.c_str());

    // status = db->Delete(rocksdb::WriteOptions(), key1);
    // assert(status.ok());
    // printf("Delete key[%s] success.\n", key1.c_str());

    delete db;

    status = rocksdb::DB::Open(options, "/tmp/testdb", &db);
    assert(status.ok());
    cout << "Open rocksdb success." << endl;

    string value2;
    status = db->Get(rocksdb::ReadOptions(), key1, &value2);
    assert(status.ok());
    printf("Get key[%s] = %s\n", key1.c_str(), value2.c_str());
    
    
    logstream(LOG_INFO) << "Get key[" << key1 << "] = " << value2 << std::endl;

    delete db;

    m.stop_time("testdb");

    /* Report execution metrics */
    metrics_report(m);

    return 0;
}


