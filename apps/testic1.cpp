#include "api/nvmgraph_basic_include.hpp"
#include <queue>

class  Res
{
private:
    std::string pid;
    std::string lastname;
    int dis;
    std::string birthday;
    std::string creationDate;
    std::string gender;
    std::string browserUsed;
    std::string locationIP;
    std::string email;
    std::string speaks;
    std::string cityName;
    std::string universityName;
    std::string universityClassYear;
    std::string universityCity;
    std::string companyName;
    std::string companyWorkFrom;
    std::string companyCountry;
public:
     Res(/* args */);
     Res(std::string _pid, std::string _lastname, int _dis, std::string _birthday, std::string _creationDate, 
        std::string _gender, std::string _browserUsed, std::string _locationIP, 
        std::string _email, std::string _speaks, std::string _cityName, 
        std::string _universityName, std::string _universityClassYear, std::string _universityCity,
        std::string _companyName, std::string _companyWorkFrom, std::string _companyCountry)
        :pid(_pid), lastname(_lastname), dis(_dis), birthday(_birthday), creationDate(_creationDate), 
        gender(_gender), browserUsed(_browserUsed), locationIP(_locationIP), 
        email(_email), speaks(_speaks), cityName(_cityName), 
        universityName(_universityName), universityClassYear(_universityClassYear), universityCity(_universityCity), 
        companyName(_companyName), companyWorkFrom(_companyWorkFrom), companyCountry(_companyCountry){

        }
    ~ Res();

    void prinfRes(){
        std::string TestPath = get_option_string("TestPath");  // test results path
        std::ofstream ofs;
        ofs.open(TestPath.c_str(), std::ofstream::out | std::ofstream::app );
        // logstream(LOG_DEBUG) << "pid = " << pid << "\n" 
        ofs << "pid = " << pid << "\n" 
            << "lastname = " << lastname << "\n" 
            << "dis = " << dis << "\n" 
            << "birthday = " << birthday << "\n" 
            << "creationDate = " << creationDate << "\n" 
            << "gender = " << gender << "\n" 
            << "browserUsed = " << browserUsed << "\n" 
            << "locationIP = " << locationIP << "\n" 
            << "email = " << email << "\n" 
            << "speaks = " << speaks << "\n" 
            << "cityName = " << cityName << "\n" 
            << "universityName = " << universityName << "\n" 
            << "universityClassYear = " << universityClassYear << "\n" 
            << "universityCity = " << universityCity << "\n" 
            << "companyName = " << companyName << "\n" 
            << "companyWorkFrom = " << companyWorkFrom << "\n" 
            << "companyCountry = " << companyCountry << "\n" 
            << std::endl;
        ofs.close();
    }
};

std::string queryNeighbors(std::string prefix_res, rocksdb::DB* db){
    std::string result;
    auto it_res = db->NewIterator(rocksdb::ReadOptions());
    for (it_res->Seek(prefix_res); it_res->Valid() && it_res->key().starts_with(prefix_res); it_res->Next()) {
        // logstream(LOG_INFO) << it_res->key().ToString() << std::endl;
        std::string str_res = it_res->key().ToString();
        std::vector<std::string> fields_res;
        assert(string_split(str_res, '|', fields_res) == 0);
        assert(fields_res.size() == 4);
        result += fields_res[3] + ",";
    }
    return result;
}

void testIC1(std::string queryId, std::string queryName, rocksdb::DB* db, metrics &m){
    m.start_time("testic1");
    m.start_time("testic1-0-Start");
    std::string TestPath = get_option_string("TestPath");  // test results path
    std::ofstream ofs;
    ofs.open(TestPath.c_str(), std::ofstream::out | std::ofstream::app );
    ofs << "\nTestIC1 for queryId = " << queryId << ", queryName = " << queryName << ":" << std::endl;
    ofs.close();
    logstream(LOG_INFO) << "testIC1 for queryId = " << queryId << ", queryName = " << queryName << "..." << std::endl;

    std::vector<Res*> friends_with_certern_name;
    std::queue<std::pair<std::string, int> > neighbors;
    neighbors.push(make_pair(queryId, 0));
    m.stop_time("testic1-0-Start");

    m.start_time("testic1-1-Search");
    while(!neighbors.empty()){
        m.start_time("testic1-1-Search-0-GetCur");
        std::pair<std::string, int> cur = neighbors.front();
        neighbors.pop();
        std::string curId = cur.first;
        int dis = cur.second;

        // Search friends
        auto it = db->NewIterator(rocksdb::ReadOptions());
        std::string prefix = curId + "|knows|person|";
        rocksdb::Status status;
        m.stop_time("testic1-1-Search-0-GetCur");
        m.start_time("testic1-1-Search-1-Seek");
        it->Seek(prefix);
        m.stop_time("testic1-1-Search-1-Seek");
        m.start_time("testic1-1-Search-2-query");
        for (; it->Valid() && it->key().starts_with(prefix); ) {
            // m.start_time("testic1-1-Search-2-query-0-String_split");
            // logstream(LOG_INFO) << it->key().ToString() << std::endl;
            std::string str = it->key().ToString();
            std::vector<std::string> fields;
            assert(string_split(str, '|', fields) == 0);
            // m.stop_time("testic1-1-Search-2-query-0-String_split");
            if(fields.size() == 4 && dis < 3){
                // m.start_time("testic1-GetProperties");
                std::string desID = fields[3];
                std::string key = desID + "|firstName", firstName;
                status = db->Get(rocksdb::ReadOptions(), key, &firstName);
                assert(status.ok());
                if(firstName == queryName){
                    std::string lastname, birthday, creationDate, gender, browserUsed, locationIP, email, speaks, cityName; 
                    std::string universityName, universityClassYear, universityCity, companyName, companyWorkFrom, companyCountry;
                    key = desID + "|lastName";
                    status = db->Get(rocksdb::ReadOptions(), key, &lastname);
                    assert(status.ok());
                    key = desID + "|birthday";
                    status = db->Get(rocksdb::ReadOptions(), key, &birthday);
                    assert(status.ok());
                    key = desID + "|creationDate";
                    status = db->Get(rocksdb::ReadOptions(), key, &creationDate);
                    assert(status.ok());
                    key = desID + "|gender";
                    status = db->Get(rocksdb::ReadOptions(), key, &gender);
                    assert(status.ok());
                    key = desID + "|browserUsed";
                    status = db->Get(rocksdb::ReadOptions(), key, &browserUsed);
                    assert(status.ok());
                    key = desID + "|locationIP";
                    status = db->Get(rocksdb::ReadOptions(), key, &locationIP);
                    assert(status.ok());

                    email = queryNeighbors(desID + "|email|emailaddress|", db);
                    speaks = queryNeighbors(desID + "|speaks|language|", db);

                    cityName = queryNeighbors(desID + "|isLocatedIn|place|", db);
                    // std::string cityId = queryNeighbors(desID + "|isLocatedIn|place|", db);
                    // key = cityId + "|name";
                    // logstream(LOG_DEBUG) << key << std::endl;
                    // status = db->Get(rocksdb::ReadOptions(), key, &cityName);
                    // assert(status.ok());

                    // std::string universityId = queryNeighbors(desID + "|studyAt|organisation|", db);
                    // key = universityId + "|name";
                    // status = db->Get(rocksdb::ReadOptions(), key, &universityName);
                    // assert(status.ok());

                    Res *res = new Res(desID, lastname, dis+1, birthday, creationDate, gender, browserUsed, locationIP, email, speaks, cityName, 
                    universityName, universityClassYear, universityCity, companyName, companyWorkFrom, companyCountry);
                    friends_with_certern_name.push_back(res);
                }
                // m.stop_time("testic1-GetProperties");

                // Search friends of friends
                // m.start_time("testic1-Push_friends");
                neighbors.push(make_pair(desID, dis+1));
                // m.stop_time("testic1-Push_friends");
            }

            // m.start_time("testic1-1-Search-2-query-9-Next");
            it->Next();
            // m.stop_time("testic1-1-Search-2-query-9-Next");
        }
        m.stop_time("testic1-1-Search-2-query");
    }
    m.stop_time("testic1-1-Search");

    m.start_time("testic1-2-prinfRes");
    for(auto it = friends_with_certern_name.begin(); it != friends_with_certern_name.end(); it++){
        (*it)->prinfRes();
    }
    m.stop_time("testic1-2-prinfRes");
    m.stop_time("testic1");
}

std::vector<std::pair<std::string, std::string> > readQueries(std::string filename){
    std::vector<std::pair<std::string, std::string> > queries;

    FILE * inf = fopen(filename.c_str(), "r");
    if (inf == NULL) {
        logstream(LOG_FATAL) << "Could not load :" << filename << " error: " << strerror(errno) << std::endl;
    }
    assert(inf != NULL);

    char s[1024];
    while(fgets(s, 1024, inf) != NULL) {
        if (s[0] == '#') continue; // Comment
        if (s[0] == '%') continue; // Comment
        if (s[0] == 'p') continue; // Title: personId|firstName
        
        // logstream(LOG_INFO) << " s= " << s << std::endl;
        char *t1, *t2;
        t1 = strtok(s, "|");
        t2 = strtok(NULL, "\n");
        if (t1 == NULL || t2 == NULL ) {
            logstream(LOG_ERROR) << "Input file is not in right format. "
            << "Expecting <from> <to>. "
            << "Current line: " << s << "\n";
            assert(false);
        }
        logstream(LOG_INFO) << " std::string(t1) = " << std::string(t1) << ", std::string(t2) = " << std::string(t2) << "." << std::endl;
        queries.push_back(make_pair(std::string(t1), std::string(t2)));
    }
    return queries;
}


int main(int argc, const char ** argv) {

    set_argc(argc, argv);

    /* Metrics object for keeping track of performance count_invectorers
     and other information. Currently required. */
    metrics m("test-IC1");
    m.start_time("Runtime(s)");

    /* Basic arguments for application */
    // std::string filename = get_option_string("file", "../../dataset/SF1/substitution_parameters/interactive_1_param.txt");  // Base filename
    std::string DBPath = get_option_string("DBPath");  // database path
    std::string GraphPath = get_option_string("GraphPath");  // graph dataset path
    std::string IC1_Param = GraphPath + "/substitution_parameters/interactive_1_param.txt";
    std::string TestPath = get_option_string("TestPath");  // test results path
    logstream(LOG_INFO) << "DBPath = " << DBPath << "\n" 
                         << "GraphPath = " << GraphPath << "\n" 
                         << "IC1_Param = " << IC1_Param << "\n" 
                         << "TestPath = " << TestPath << std::endl;
    
    std::ofstream ofs(TestPath.c_str(), std::ofstream::out);
    ofs << "Begin TestIC1!" << std::endl;
    ofs.close();

    std::vector<std::pair<std::string, std::string> > queries = readQueries(IC1_Param);

    rocksdb::DB* db;
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::Status status = rocksdb::DB::Open(options, DBPath, &db);
    assert(status.ok());
    logstream(LOG_DEBUG) << "Open rocksdb success." << std::endl;

    std::string queryId, queryName;
    for(auto it = queries.begin(); it != queries.end(); it++){
        queryId = it->first;
        queryName = it->second;
        testIC1(queryId, queryName, db, m);
    }
    
    delete db;
    m.stop_time("Runtime(s)");

    /* Report execution metrics */
    metrics_report(m);

    return 0;
}


