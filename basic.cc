#include "basic.h"
#include "exception.h"
#include "template_function.h"
#include <iomanip>
#include <sstream>

Tuple::Tuple(const Tuple &tuple_in){
    for(int index=0;index<tuple_in.data_.size();index++)
    {
        this->data_.push_back(tuple_in.data_[index]);
    }
    this->isDeleted_ = tuple_in.isDeleted_;
}

inline int Tuple::getSize(){
    return (int)data_.size();
}

//新增数据
void Tuple::addData(Data data_in){
    this->data_.push_back(data_in);
}

bool Tuple::isDeleted() const {
    return isDeleted_;
}

void Tuple::setDeleted() {
    isDeleted_ = true;
}

//得到元组中的数据
std::vector<Data> Tuple::getData() const{
    return this->data_;
}

void Tuple::showTuple(){
    for(int index=0;index<getSize();index++){
        if(data_[index].type==-1)
            std::cout<<data_[index].datai<<'\t';
        else if(data_[index].type==0)
            std::cout<<data_[index].dataf<<'\t';
        else
            std::cout<<data_[index].datas<<'\t';
    }
    std::cout<<std::endl;
}

//table构造函数
Table::Table(std::string title,Attribute attr){
    this->title_=title;
    this->attr_=attr;
    this->index_.num=0;
}

//table的构造函数，拷贝用
Table::Table(const Table &table_in){
    this->attr_=table_in.attr_;
    this->index_=table_in.index_;
    this->title_=table_in.title_;
    for(int index=0;index<tuple_.size();index++)
        this->tuple_.push_back(table_in.tuple_[index]);
}

//插入元组
// int Table::addTuple(Tuple tuple_in){
//     if(tuple_in.getSize()!=attr_.num)
//     {
//         std::cout<<"Illegal Tuple Insert: The size of column is unequal."<<std::endl;
//         return 0;
//     }
//     for(int index=0;index<attr_.num;index++){
//         //当输入的tuple的type为int或者float时，必须与对应的attr_.type相同。如果为string的话，则需要不大于attr_.type
//         if(tuple_in.getData()[index].type>attr_.type[index]||(tuple_in.getData()[index].type<=0&&tuple_in.getData()[index].type!=attr_.type[index]))
//         {
//             std::cout<<"Illegal Tuple Insert: The types of attributes are unequal."<<std::endl;
//             return 0;
//         }
//     }
//     tuple_.push_back(tuple_in);
//     return 1;
// }

//插入索引
int Table::setIndex(short index,std::string index_name){
    short tmpIndex;
    for(tmpIndex=0;tmpIndex<index_.num;tmpIndex++){
        if(index==index_.location[tmpIndex])  //当该元素已经有索引时，报错
        {
            std::cout<<"Illegal Set Index: The index has been in the table."<<std::endl;
            return 0;
        }
    }
    for(tmpIndex=0;tmpIndex<index_.num;tmpIndex++){
        if(index_name==index_.indexname[tmpIndex])  //当该元素已经有索引时，报错
        {
            std::cout<<"Illegal Set Index: The name has been used."<<std::endl;
            return 0;
        }
    }
    index_.location[index_.num]=index;  //插入索引位置和索引名字，最后把索引个数加一
    index_.indexname[index_.num]=index_name;
    index_.num++;
    return 1;
}

int Table::dropIndex(std::string index_name){
    short tmpIndex;
    for(tmpIndex=0;tmpIndex<index_.num;tmpIndex++){
        if(index_name==index_.indexname[tmpIndex])  //当该元素已经有索引时，跳出
            break;
    }
    if(tmpIndex==index_.num)
    {
        std::cout<<"Illegal Drop Index: No such a index in the table."<<std::endl;
        return 0;
    }

    //交换最后的索引位置和名字，来达到删除的效果
    index_.indexname[tmpIndex]=index_.indexname[index_.num-1];
    index_.location[tmpIndex]=index_.location[index_.num-1];
    index_.num--;
    return 1;
}

//计算每个attribute的大小
// int Table::DataSize(){
//     int result=0;
//     for(int index=0;index<attr_.num;index++){
//         switch (attr_.type[index]) {
//             case -1:
//                 result+=sizeof(int);
//                 break;
//             case 0:
//                 result+=sizeof(float);
//                 break;
//             default:
//                 result+=attr_.type[index];
//                 break;
//         }
//     }
//     return result;
// }

//返回一些private的值
std::string Table::getTitle(){
    return title_;
}
Attribute Table::getAttr(){
    return attr_;
}
std::vector<Tuple>& Table::getTuple(){
    return tuple_;
}
Index Table::getIndex(){
    return index_;
}


void Table::showTable(){
    // 计算每列的最大宽度
    std::vector<size_t> col_widths(attr_.num, 0);
    
    // 计算列名的宽度
    for(int i = 0; i < attr_.num; i++) {
        col_widths[i] = std::max(col_widths[i], attr_.name[i].length());
    }
    
    // 计算数据的宽度
    for(const auto& tuple : tuple_) {
        if(tuple.isDeleted()) continue;
        const auto& data = tuple.getData();
        for(int i = 0; i < attr_.num; i++) {
            std::string value;
            if(data[i].type == -1)
                value = std::to_string(data[i].datai);
            else if(data[i].type == 0)
                value = std::to_string(data[i].dataf);
            else
                value = data[i].datas;
            col_widths[i] = std::max(col_widths[i], value.length());
        }
    }
    
    // 打印列名
    for(int i = 0; i < attr_.num; i++) {
        std::cout << std::left << std::setw(col_widths[i] + 4) << attr_.name[i];
    }
    std::cout << std::endl;
    
    // 打印分隔线
    for(int i = 0; i < attr_.num; i++) {
        std::cout << std::string(col_widths[i] + 4, '-');
    }
    std::cout << std::endl;
    
    // 打印数据
    for(const auto& tuple : tuple_) {
        if(tuple.isDeleted()) continue;
        const auto& data = tuple.getData();
        for(int i = 0; i < attr_.num; i++) {
            std::string value;
            if(data[i].type == -1)
                value = std::to_string(data[i].datai);
            else if(data[i].type == 0) {
                std::ostringstream ss;
                ss << std::fixed << std::setprecision(2) << data[i].dataf;
                value = ss.str();
            }
            else
                value = data[i].datas;
            std::cout << std::left << std::setw(col_widths[i] + 4) << value;
        }
        std::cout << std::endl;
    }
}

void Table::showTable(int limit) {
    // 计算每列的最大宽度
    std::vector<size_t> col_widths(attr_.num, 0);
    
    // 计算列名的宽度
    for(int i = 0; i < attr_.num; i++) {
        col_widths[i] = std::max(col_widths[i], attr_.name[i].length());
    }
    
    // 计算数据的宽度
    int count = 0;
    for(const auto& tuple : tuple_) {
        if(count >= limit) break;
        if(tuple.isDeleted()) continue;
        const auto& data = tuple.getData();
        for(int i = 0; i < attr_.num; i++) {
            std::string value;
            if(data[i].type == -1)
                value = std::to_string(data[i].datai);
            else if(data[i].type == 0)
                value = std::to_string(data[i].dataf);
            else
                value = data[i].datas;
            col_widths[i] = std::max(col_widths[i], value.length());
        }
        count++;
    }
    
    // 打印列名
    for(int i = 0; i < attr_.num; i++) {
        std::cout << std::left << std::setw(col_widths[i] + 4) << attr_.name[i];
    }
    std::cout << std::endl;
    
    // 打印分隔线
    for(int i = 0; i < attr_.num; i++) {
        std::cout << std::string(col_widths[i] + 4, '-');
    }
    std::cout << std::endl;
    
    // 打印数据
    count = 0;
    for(const auto& tuple : tuple_) {
        if(count >= limit) break;
        if(tuple.isDeleted()) continue;
        const auto& data = tuple.getData();
        for(int i = 0; i < attr_.num; i++) {
            std::string value;
            if(data[i].type == -1)
                value = std::to_string(data[i].datai);
            else if(data[i].type == 0) {
                std::ostringstream ss;
                ss << std::fixed << std::setprecision(2) << data[i].dataf;
                value = ss.str();
            }
            else
                value = data[i].datas;
            std::cout << std::left << std::setw(col_widths[i] + 4) << value;
        }
        std::cout << std::endl;
        count++;
    }
}

Table Table::leftJoin(const Table& right_table, const JoinCondition& condition) const {
    // 找到连接属性在两个表中的位置
    int left_attr_pos = -1, right_attr_pos = -1;
    for(int i = 0; i < attr_.num; i++) {
        if(attr_.name[i] == condition.left_attr) {
            left_attr_pos = i;
            break;
        }
    }
    for(int i = 0; i < right_table.attr_.num; i++) {
        if(right_table.attr_.name[i] == condition.right_attr) {
            right_attr_pos = i;
            break;
        }
    }
    
    if(left_attr_pos == -1 || right_attr_pos == -1) {
        throw attribute_not_exist();
    }

    // 检查数据类型是否匹配
    if(attr_.type[left_attr_pos] != right_table.attr_.type[right_attr_pos]) {
        throw data_type_conflict();
    }

    // 创建结果表的属性
    Attribute result_attr;
    result_attr.num = attr_.num + right_table.attr_.num;
    result_attr.primary_key = -1;  // 连接结果没有主键
    
    // 复制左表属性
    for(int i = 0; i < attr_.num; i++) {
        result_attr.name[i] = title_ + "." + attr_.name[i];
        result_attr.type[i] = attr_.type[i];
        result_attr.unique[i] = false;  // 连接后的属性都不唯一
        result_attr.has_index[i] = false;
    }
    
    // 复制右表属性
    for(int i = 0; i < right_table.attr_.num; i++) {
        result_attr.name[i + attr_.num] = right_table.title_ + "." + right_table.attr_.name[i];
        result_attr.type[i + attr_.num] = right_table.attr_.type[i];
        result_attr.unique[i + attr_.num] = false;
        result_attr.has_index[i + attr_.num] = false;
    }

    // 创建结果表
    Table result_table(title_ + "_" + right_table.title_ + "_join", result_attr);
    
    // 对左表的每条记录
    for(const auto& left_tuple : tuple_) {
        if(left_tuple.isDeleted()) continue;
        
        bool found_match = false;
        // 在右表中寻找匹配的记录
        for(const auto& right_tuple : right_table.tuple_) {
            if(right_tuple.isDeleted()) continue;
            
            // 检查连接条件
            const std::vector<Data>& left_data = left_tuple.getData();
            const std::vector<Data>& right_data = right_tuple.getData();
            
            bool match = false;
            switch(condition.relation_character) {
                case EQUAL: match = left_data[left_attr_pos] == right_data[right_attr_pos]; break;
                case LESS: match = left_data[left_attr_pos] < right_data[right_attr_pos]; break;
                case LESS_OR_EQUAL: match = left_data[left_attr_pos] <= right_data[right_attr_pos]; break;
                case GREATER: match = left_data[left_attr_pos] > right_data[right_attr_pos]; break;
                case GREATER_OR_EQUAL: match = left_data[left_attr_pos] >= right_data[right_attr_pos]; break;
                case NOT_EQUAL: match = left_data[left_attr_pos] != right_data[right_attr_pos]; break;
            }
            
            if(match) {
                found_match = true;
                // 创建新的元组
                Tuple new_tuple;
                // 添加左表数据
                for(const auto& data : left_data) {
                    new_tuple.addData(data);
                }
                // 添加右表数据
                for(const auto& data : right_data) {
                    new_tuple.addData(data);
                }
                result_table.getTuple().push_back(new_tuple);
            }
        }
        
        // 如果没有找到匹配，添加一条右表为NULL的记录
        if(!found_match) {
            Tuple new_tuple;
            // 添加左表数据
            for(const auto& data : left_tuple.getData()) {
                new_tuple.addData(data);
            }
            // 添加右表NULL数据
            for(int i = 0; i < right_table.attr_.num; i++) {
                Data null_data;
                null_data.type = right_table.attr_.type[i];
                switch(null_data.type) {
                    case -1: null_data.datai = 0; break;
                    case 0: null_data.dataf = 0.0; break;
                    default: null_data.datas = "NULL"; break;
                }
                new_tuple.addData(null_data);
            }
            result_table.getTuple().push_back(new_tuple);
        }
    }
    
    return result_table;
}
