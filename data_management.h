#ifndef DATA_MANAGEMENT_H
#define DATA_MANAGEMENT_H

#define SQLITE_HAS_CODEC

#include "common.h"
#include <string>
#include <list>
#include <sqlcipher/sqlite3.h>

using namespace std;



class DataManagement
{
public:
    DataManagement();
    ~DataManagement();

    /**
     * @brief addUser add a user to database
     * @param u user infomation
     * @return  true if succeed, false elsewise
     */
    bool addUser(struct User u);

    /**
     * @brief delUser delete a user from database
     * @param id user id
     * @return true if succeed, false elsewise
     */
    bool delUser(int id);
    bool delUser(std::string name);

    /**
     * @brief modUser modify user mode, name authority
     * @param id user id
     * @return true if succeed, false elsewise
     */
    bool modUser(int id, struct User u);

    bool modUser(std::string name, struct User u);  // assume that name is unique, why not?
    /**
     * @brief info get information of all users
     * @param users
     * @return
     */
    int infoUser(list<struct User>& users);

    /**
     * @brief infoUser check whether given user exists
     * @param user
     * @param passwd
     * @return
     */
     bool checkUser(string user, string passwd="");

    /**
     * @brief infoUserType get user type
     * @param user
     * @return
     */
    int infoUserType(string user);

    bool delLog(int id);
    bool delLog(std::string datetime);  // assume time is unique
    bool insLog(class Loginfo& log);
    void dumpLog(string& content);
    int infoLog(list<struct Loginfo> &log);
    int countLog();

    bool delWarning(int id);
    bool delWarning(std::string datetime, std::string exception);
    bool insWarning(class Warning& warning);
    void dumpWarning(string& warninfo);
    int infoWarning(list<struct Warning> &warning);
    int countWarning();


private:
    /**
     * @brief init open database
     * @return true if succeed, false elsewise
     */
    bool initDatabase();
    void createDatabase();

private:
    sqlite3 *m_db;  ///< handler of database
    string m_sql;  ///< SQL syntax
    string m_tblUser;  ///< table name for user
    string m_tblWarning;  ///< table name for warning information
    string m_tblLog;  ///< table name for log information
    char *m_err;  ///< error inform of database manipulation
    list<struct User> m_users;  ///< list containing all users' information
    list<struct Warning> m_warning;  ///< list containing all warning information
    list<struct Loginfo> m_loginfo;  ///< list containing all log information

    static std::string USER_FIELD[4];
    static std::string LOGINFO_FILED[4];
    static std::string WARNING_FIELD[5];
    static std::string m_db_passwd;
    static std::string m_config_path;
    static std::string m_db_file;
};

#endif // DATA_MANAGEMENT_H
