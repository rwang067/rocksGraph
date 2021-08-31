#include "api/nvmgraph_basic_include.hpp"

void importVertices(std::string filename, std::string vlabel, rocksdb::DB* db){
    
    logstream(LOG_DEBUG) << "Importing vertices in " << filename << "..." << std::endl;

    FILE *inf = fopen(filename.c_str(), "r");
    if (inf == NULL) {
        logstream(LOG_FATAL) << "Could not load :" << filename << " error: " << strerror(errno) << std::endl;
    }
    assert(inf != NULL);

    // Read keys from the first line
    char s[8192];
    assert(fgets(s, 8192, inf) != NULL);
    logstream(LOG_INFO) << " s= " << s << std::endl;
    std::vector<std::string> keys;
    assert(string_split(s, '|', keys) == 0);
    unsigned k = keys.size();
    keys[k-1].pop_back();
    // for(int i = 0; i < k; i++){
    //     logstream(LOG_DEBUG) << "keys[" << i << "] = " << keys[i] << std::endl;
    // }
    // char* temp = strtok(s, "|");
    // std::string pkey = std::string(temp);
    // std::string keys[64];
    // int k = 0;
    // temp = strtok(NULL, "|");
    // while(temp != NULL)
    // {
    //     keys[k] = std::string(temp);
    //     logstream(LOG_DEBUG) << "keys[" << k << "] = " << keys[k] << std::endl;
    //     k++;
    //     temp = strtok(NULL, "|");
    // }
    // keys[k-1].pop_back();
    // logstream(LOG_DEBUG) << "k = " << k << std::endl;
    
    // Read values from the following lines
    rocksdb::Status status;
    std::string key;
    int linecount = 0;
    while(fgets(s, 8192, inf) != NULL) {
    // while(fgets(s, 1024, inf) != NULL && linecount < 3) {
        // if (s[0] == '#') continue; // Comment
        // if (s[0] == '%') continue; // Comment
        // logstream(LOG_DEBUG) << " s= " << s << std::endl;
        std::vector<std::string> values;
        assert(string_split(s, '|', values) == 0);
        assert(k == values.size());
        values[k-1].pop_back();
        // Put vertex with vlabel value
        status = db->Put(rocksdb::WriteOptions(), values[0], vlabel);
        assert(status.ok());
        // Put vertex' properties
        for(unsigned i = 1; i < k; i++){
            key = values[0] + "|" + keys[i];
            status = db->Put(rocksdb::WriteOptions(), key, values[i]);
            assert(status.ok());
            // logstream(LOG_INFO) << "Put[" << key << "," << values[i] << "] success." << std::endl;
        }
        linecount++;
        // std::string pkvalue = strtok(s, "|");
        // logstream(LOG_DEBUG) << "pkey = " << keys[0] << ", pkvalue = " << pkvalue << std::endl;
        // for(int i = 1; i < k; i++){
        //     key = pkvalue+keys[i];
        //     value = strtok(NULL, "|");
        //     if(i == k-1) value.pop_back();
        //     status = db->Put(rocksdb::WriteOptions(), key, value);
        //     assert(status.ok());
        //     logstream(LOG_INFO) << "Put[" << key << "," << value << "] success." << std::endl;
        //     // std::string value3;
        //     // status = db->Get(rocksdb::ReadOptions(), key, &value3);
        //     // assert(status.ok());
        //     // logstream(LOG_DEBUG) << "Get key[" << key << "] = " << value3 << std::endl;
        // }
    }
    
    logstream(LOG_DEBUG) << "Imported linecount = " << linecount << ", #properties of each line = " << k-1 << "\n" << std::endl;
}

void importEdges(std::string filename, std::string elabel, std::string tlabel, rocksdb::DB* db){
    
    logstream(LOG_DEBUG) << "Importing edges in " << filename << "..." << std::endl;

    FILE *inf = fopen(filename.c_str(), "r");
    if (inf == NULL) {
        logstream(LOG_FATAL) << "Could not load :" << filename << " error: " << strerror(errno) << std::endl;
    }
    assert(inf != NULL);

    // Read keys from the first line
    char s[8192];
    assert(fgets(s, 8192, inf) != NULL);
    logstream(LOG_INFO) << " s= " << s << std::endl;
    std::vector<std::string> keys;
    assert(string_split(s, '|', keys) == 0);
    unsigned k = keys.size();
    keys[k-1].pop_back();
    // for(int i = 0; i < k; i++){
    //     logstream(LOG_DEBUG) << "keys[" << i << "] = " << keys[i] << std::endl;
    // }
    
    // Read values from the following lines
    rocksdb::Status status;
    std::string key;
    int linecount = 0;
    while(fgets(s, 8192, inf) != NULL) {
    // while(fgets(s, 1024, inf) != NULL && linecount < 3) {
        // if (s[0] == '#') continue; // Comment
        // if (s[0] == '%') continue; // Comment
        // logstream(LOG_DEBUG) << " s= " << s << std::endl;
        std::vector<std::string> values;
        assert(string_split(s, '|', values) == 0);
        assert(k == values.size());
        values[k-1].pop_back();
        // Put edge with empty value
        std::string edge_key = values[0] + "|" + elabel + "|" + tlabel + "|" + values[1];
        status = db->Put(rocksdb::WriteOptions(), edge_key, "");
        assert(status.ok());
        // Put edge properties
        for(unsigned i = 2; i < k; i++){
            key = edge_key + "|" + keys[i];
            status = db->Put(rocksdb::WriteOptions(), key, values[i]);
            assert(status.ok());
            // logstream(LOG_INFO) << "Put[" << key << "," << values[i] << "] success." << std::endl;
        }
        linecount++;
    }
    
    logstream(LOG_DEBUG) << "Imported linecount = " << linecount << ", #properties of each line = " << k-2 << "\n" << std::endl;
}

void importGraph(std::string filename, rocksdb::DB* db, metrics &m){
    m.start_time("importVertices");
    importVertices(filename + "/social_network/static/organisation_0_0.csv", "organisation", db);
    importVertices(filename + "/social_network/static/place_0_0.csv", "place", db);
    importVertices(filename + "/social_network/static/tag_0_0.csv", "tag", db);
    importVertices(filename + "/social_network/static/tagclass_0_0.csv", "tagclass", db);
    importVertices(filename + "/social_network/dynamic/person_0_0.csv", "person", db);
    importVertices(filename + "/social_network/dynamic/post_0_0.csv", "post", db);
    importVertices(filename + "/social_network/dynamic/comment_0_0.csv", "comment", db);
    importVertices(filename + "/social_network/dynamic/forum_0_0.csv", "forum", db);
    m.stop_time("importVertices");

    m.start_time("importEdges");
    importEdges(filename + "/social_network/static/organisation_isLocatedIn_place_0_0.csv", "isLocatedIn", "place", db);
    importEdges(filename + "/social_network/static/place_isPartOf_place_0_0.csv", "isPartOf", "lace", db);
    importEdges(filename + "/social_network/static/tag_hasType_tagclass_0_0.csv", "hasType", "tagclass", db);
    importEdges(filename + "/social_network/static/tagclass_isSubclassOf_tagclass_0_0.csv", "isSubclassOf", "tagclass", db);
    importEdges(filename + "/social_network/dynamic/person_knows_person_0_0.csv", "knows", "person", db);
    importEdges(filename + "/social_network/dynamic/person_likes_post_0_0.csv", "likes", "post", db);
    importEdges(filename + "/social_network/dynamic/person_likes_comment_0_0.csv", "likes", "comment", db);
    importEdges(filename + "/social_network/dynamic/person_speaks_language_0_0.csv", "speaks", "language", db);
    importEdges(filename + "/social_network/dynamic/person_studyAt_organisation_0_0.csv", "studyAt", "organisation", db);
    importEdges(filename + "/social_network/dynamic/person_workAt_organisation_0_0.csv", "workAt", "organisation", db);
    importEdges(filename + "/social_network/dynamic/person_isLocatedIn_place_0_0.csv", "isLocatedIn", "place", db);
    importEdges(filename + "/social_network/dynamic/person_hasInterest_tag_0_0.csv", "hasInterest", "tag", db);
    importEdges(filename + "/social_network/dynamic/person_email_emailaddress_0_0.csv", "email", "emailaddress", db);
    importEdges(filename + "/social_network/dynamic/post_hasCreator_person_0_0.csv", "hasCreator", "person", db);
    importEdges(filename + "/social_network/dynamic/post_hasTag_tag_0_0.csv", "hasTag", "tag", db);
    importEdges(filename + "/social_network/dynamic/post_isLocatedIn_place_0_0.csv", "isLocatedIn", "place", db);
    importEdges(filename + "/social_network/dynamic/comment_hasCreator_person_0_0.csv", "hasCreator", "person", db);
    importEdges(filename + "/social_network/dynamic/comment_hasTag_tag_0_0.csv", "hasTag", "tag", db);
    importEdges(filename + "/social_network/dynamic/comment_isLocatedIn_place_0_0.csv", "isLocatedIn", "place", db);
    importEdges(filename + "/social_network/dynamic/comment_replyOf_comment_0_0.csv", "replyOf", "comment", db);
    importEdges(filename + "/social_network/dynamic/comment_replyOf_post_0_0.csv", "replyOf", "post", db);
    importEdges(filename + "/social_network/dynamic/forum_containerOf_post_0_0.csv", "containerOf", "post", db);
    importEdges(filename + "/social_network/dynamic/forum_hasMember_person_0_0.csv", "hasMember", "person", db);
    importEdges(filename + "/social_network/dynamic/forum_hasModerator_person_0_0.csv", "hasModerator", "person", db);
    importEdges(filename + "/social_network/dynamic/forum_hasTag_tag_0_0.csv", "hasTag", "tag", db);
    m.stop_time("importEdges");
}