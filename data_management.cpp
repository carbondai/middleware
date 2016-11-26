#include "data_management.h"
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <fstream>

std::string DataManagement::USER_FIELD[4] = {"id", "name", "password", "type"};
std::string DataManagement::LOGINFO_FILED[4] = {"id", "user", "time", "comment"};
std::string DataManagement::WARNING_FIELD[5] = {"id", "time", "unit", "exception", "comment"};
std::string DataManagement::m_db_passwd = "000000";
std::string DataManagement::m_db_file = "/home/wally/.cabinetsvc/data.db";

DataManagement::DataManagement()
{
    m_err = NULL;
    m_tblUser = "users";
    m_tblWarning = "warning";
    m_tblLog = "loginfo";
    createDatabase();
    initDatabase();
}

DataManagement::~DataManagement()
{
    sqlite3_close(m_db);
}

void DataManagement::createDatabase()
{
    ifstream fdb(m_db_file.c_str());  // TODO 检验目录文件存在与否
    if(fdb)
        return;

    sqlite3* db;
    int rc = sqlite3_open_v2((char*)m_db_file.c_str(), &db, SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE, NULL);
    if(SQLITE_OK == rc)
    {
    sqlite3_key(db, (char*)m_db_passwd.c_str(), m_db_passwd.length());

    std::string sql;

    sql = " CREATE TABLE users(id integer primary key not null, name text unique not null,  password text not null,  type integer not null);";
    sql += "INSERT INTO users VALUES(0,'admin','admin',1);";
    rc = sqlite3_exec(db, (char*)sql.c_str(), NULL, NULL, NULL);

    sql = "CREATE TABLE warning(id integer primary key not null,  time text, unit text, exception text, comment text);";
    rc = sqlite3_exec(db, (char*)sql.c_str(), NULL, NULL, NULL);

    sql = "CREATE TABLE loginfo (id integer primary key not null,  user text, time text, comment text);";
    rc = sqlite3_exec(db, (char*)sql.c_str(), NULL, NULL, NULL);
    }
    sqlite3_close(db);
}

bool DataManagement::initDatabase()
{
    int rc = sqlite3_open(m_db_file.c_str(), &m_db);  //, SQLITE_OPEN_READWRITE, "foo");
    if(SQLITE_OK == rc)
    {
        sqlite3_key(m_db, (char*)m_db_passwd.c_str(), m_db_passwd.length());
    }
    if(rc)
    {
        std::cerr << "fail to open database!";
        // m_err = sqlite3_errmsg(m_db);
        sqlite3_close(m_db);
        // exit(1);
    }
    return  true;
}

bool DataManagement::addUser(User u)
{
    m_sql = "insert into " + m_tblUser + " (name, password, type) ";
    std::stringstream ss;
    ss << "values("
        << "'" << u.name << "',"
        << "'" << u.passwd << "',"
        << (u.type?1:0)
        <<")";
    std::string s;
    ss >> s;
    m_sql += s;

    int rc = sqlite3_exec(m_db, m_sql.c_str(), NULL, NULL, NULL);

    return true;
}

bool DataManagement::delUser(int id)
{
    m_sql = "delete from " + m_tblUser + " where id=";
    std::stringstream ss;
    ss << id;
    std::string s;
    ss >> s;
    m_sql += s;

    int rc = sqlite3_exec(m_db, m_sql.c_str(), NULL, NULL, NULL);
    return true;  // TODO
}

bool DataManagement::delUser(string name)
{
    m_sql = "delete from " + m_tblUser + " where name='" + name + "'";

    int rc = sqlite3_exec(m_db, m_sql.c_str(), NULL, NULL, NULL);
    return true;
}

bool DataManagement::modUser(int id, User u)
{
    m_sql = "update " + m_tblUser + " set name='" + u.name + "', password='"
            + u.passwd + "', type=";
    int type = u.type ? 1 : 0;
    std::stringstream ss;
    ss << type << " " << id;
    std::string stype, sid;
    ss >> stype;
    m_sql += stype;
    m_sql += " where id=";
    ss >> sid;
    m_sql += sid;

    int rc = sqlite3_exec(m_db, m_sql.c_str(), NULL, NULL, NULL);

    return true;  // TODO
}

bool DataManagement::modUser(string name, User u)
{
    m_sql = "update " + m_tblUser + " set name='" + u.name + "', password='"
            + u.passwd + "', type=" + (u.type?"1 ":"0 ") + " where name='" + name +"'";
//    std::cout << m_sql << std::endl;
    int rc = sqlite3_exec(m_db, m_sql.c_str(), NULL, NULL, NULL);

    return true;
}

int DataManagement::infoUser(list<User>& users)
{
    m_sql = "select * from " + m_tblUser;

    char** result;
    int row;
    int column;
    int rc = sqlite3_get_table(m_db, m_sql.c_str(), &result, &row, &column, NULL);

    users.clear();
    for(int i=0; i<row; ++i)
    {
        User u;
        u.id = boost::lexical_cast<int>(result[column*(i+1)]);
        u.name = std::string(result[column*(i+1)+1]);
        u.passwd = std::string(result[column*(i+1)+2]);
        u.type = boost::lexical_cast<int>(result[column*(i+1)+3]);
        users.push_back(u);
    }
    return 0;
}

bool DataManagement::checkUser(string user, string passwd)
{
    m_sql = "select * from " + m_tblUser + " where name='" + user + "' ";
    if(!passwd.empty())
        m_sql += "and password='" + passwd + "'";
    int nrows;
    int ncols;
    char** result;
    int rc = sqlite3_get_table(m_db, m_sql.c_str(), &result, &nrows, NULL, NULL);
    if(0==rc && nrows>0)
        return true;
    return false;
}

int DataManagement::infoUserType(string user)
{
    m_sql = "select type from " + m_tblUser + " where name='" + user + "'";
    int nrows;
    int ncols;
    char** result;
    int rc = sqlite3_get_table(m_db, m_sql.c_str(), &result, &nrows, NULL, NULL);
    int type = boost::lexical_cast<int>(result[1]);
    return  type;
}

bool DataManagement::delLog(int id)
{
    m_sql = "delete from " + m_tblLog + " where id=";
    std::stringstream ss;
    ss << id;
    std::string s;
    ss >> s;
    m_sql += s;

    int rc = sqlite3_exec(m_db, m_sql.c_str(), NULL, NULL, NULL);

    return true;  // TODO
}

bool DataManagement::delLog(std::string datetime)
{
    m_sql = "delete from " + m_tblLog + " where time='" + datetime + "'";

    int rc = sqlite3_exec(m_db, m_sql.c_str(), NULL, NULL, NULL);

    return true;
}

void DataManagement::dumpLog(string& content)
{
    m_sql = "select user, time, comment from " + m_tblLog;

    char** result;
    int row;
    int column;
    int rc = sqlite3_get_table(m_db, m_sql.c_str(), &result, &row, &column, NULL);

    content.clear();
    for(int i=0; i<row; ++i)
    {
        content += std::string(result[column*(i+1)]) + "\t"
                + std::string(result[column*(i+1)+1]) + "\t"
                + std::string(result[column*(i+1)+2]) + "\n";
    }
}

int DataManagement::infoLog(list<Loginfo> &log)
{
    m_sql = "select * from " + m_tblLog;

    char** result;
    int row;
    int column;
    int rc = sqlite3_get_table(m_db, m_sql.c_str(), &result, &row, &column, NULL);

    log.clear();
    for(int i=0; i<row; ++i)
    {
        Loginfo l;
        l.id = boost::lexical_cast<int>(result[column*(i+1)]);
        l.name = std::string(result[column*(i+1)+1]);
        l.datetime = std::string(result[column*(i+1)+2]);
        l.comment = std::string(result[column*(i+1)+3]);
        log.push_back(l);
    }

    return 0;  // TODO
}

int DataManagement::countLog()
{
    m_sql = "select * from " + m_tblLog;

    char** result;
    int row;
    int column;
    int rc = sqlite3_get_table(m_db, m_sql.c_str(), &result, &row, &column, NULL);
    return row;
}

bool DataManagement::insLog(Loginfo& log)
{
     m_sql = "insert into " + m_tblLog + " (user, time, comment) "
             + "values('" + log.name + "', '" + log.datetime + "', '" + log.comment + "')";
     int rc = sqlite3_exec(m_db, m_sql.c_str(), NULL, NULL, NULL);

     return true;  // TODO
}

bool DataManagement::delWarning(int id)
{
    m_sql = "delete from " + m_tblWarning + " where id=";
    std::stringstream ss;
    ss << id;
    std::string s;
    ss >> s;
    m_sql += s;

    int rc = sqlite3_exec(m_db, m_sql.c_str(), NULL, NULL, NULL);
    return true;  // TODO
}

bool DataManagement::delWarning(string datetime, string exception)
{
    m_sql = "delete from " + m_tblWarning + " where time='"
            + datetime + "'" + " and exception='" + exception +"'";
    int rc = sqlite3_exec(m_db, m_sql.c_str(), NULL, NULL, NULL);
    return true;
}

bool DataManagement::insWarning(Warning &warning)
{
    m_sql = "insert into " + m_tblWarning + " (time, unit, exception, comment) "
            + "values('" + warning.datetime + "', '" + warning.unit + "', '" + warning.exception +
                                               "', '" + warning.comment + "')";
     int rc = sqlite3_exec(m_db, m_sql.c_str(), NULL, NULL, NULL);

     return true;  // TODO
}

void DataManagement::dumpWarning(string& warninfo)
{
    m_sql = "select * from " + m_tblWarning;

    char** result;
    int row;
    int column;
    int rc = sqlite3_get_table(m_db, m_sql.c_str(), &result, &row, &column, NULL);

    warninfo.clear();
    for(int i=0; i<row; ++i)
    {
        warninfo += std::string(result[column*(i+1)+1]) + "\t"
                + std::string(result[column*(i+1)+2]) + "\t"
                + std::string(result[column*(i+1)+3]) + "\t"
                + std::string(result[column*(i+1)+4]) + "\n";
    }
}

int DataManagement::infoWarning(list<Warning> &warning)
{
    m_sql = "select * from " + m_tblWarning;

    char** result;
    int row;
    int column;
    int rc = sqlite3_get_table(m_db, m_sql.c_str(), &result, &row, &column, NULL);

    warning.clear();
    for(int i=0; i<row; ++i)
    {
        Warning w;
        w.datetime = std::string(result[column*(i+1)+1]);
        w.unit = std::string(result[column*(i+1)+2]);
        w.exception = std::string(result[column*(i+1)+3]);
        w.comment = std::string(result[column*(i+1)+4]);
        warning.push_back(w);
    }

    return 0;
}


int DataManagement::countWarning()
{
     m_sql = "select * from " + m_tblWarning;

    char** result;
    int row;
    int column;
    int rc = sqlite3_get_table(m_db, m_sql.c_str(), &result, &row, &column, NULL);
    return row;
}
