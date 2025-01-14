#ifndef _BASIC_H_
#define _BASIC_H_ 1
#include <iostream>
#include <vector>
#include <iomanip>

//用于where的判断 分别为小于，小于等于，等于，大于等于，大于，不等于
typedef enum{
    LESS,
    LESS_OR_EQUAL,
    EQUAL,
    GREATER_OR_EQUAL,
    GREATER,
    NOT_EQUAL
} WHERE;

//一个struct存放它的一条信息的类型和值
struct Data{
    int type;
    int datai;
    float dataf;
    std::string datas;
    bool isNull;  // 新增：标识是否为NULL值
    
    Data() : isNull(false) {}  // 构造函数初始化isNull
};

//Where存放一组判断语句
struct Where{
    Data data;
    WHERE relation_character;
};

//存放table的属性信息
struct Attribute{  
    int num;
    std::string name[32];
    short type[32];
    bool unique[32];
    short primary_key;
    bool has_index[32];
};

struct Index{
    int num;
    short location[10];
    std::string indexname[10];
};

class Tuple{
private:
    std::vector<Data> data_;
    bool isDeleted_;
public:
    Tuple() : isDeleted_(false) {};
    Tuple(const Tuple &tuple_in);
    void addData(Data data_in);
    std::vector<Data> getData() const;
    int getSize();
    bool isDeleted();
    void setDeleted();
    void showTuple();
};

class Table{
private:
    std::string title_;
    std::vector<Tuple> tuple_;
    Index index_;
public:
    Attribute attr_;
    Table(){};
    Table(std::string title,Attribute attr);
    Table(const Table &table_in);
    int setIndex(short index,std::string index_name);
    int dropIndex(std::string index_name);
    std::string getTitle();
    Attribute getAttr();
    std::vector<Tuple>& getTuple();
    Index getIndex();
    short gethasKey();
    void showTable();
    void showTable(int limit);
};

// 新增：用于存储连接结果的类
class JoinedTable : public Table {
private:
    std::string leftTableName_;
    std::string rightTableName_;
    std::string joinCondition_;
public:
    JoinedTable(std::string title, Attribute attr, 
                std::string leftTable, std::string rightTable, 
                std::string condition);
    std::string getLeftTableName() const { return leftTableName_; }
    std::string getRightTableName() const { return rightTableName_; }
    std::string getJoinCondition() const { return joinCondition_; }
};

#endif
