#include <algorithm>
#include <bits/stdc++.h>
#include <cstring>
using namespace std;
static const int limited_elementnum = 500;
template<class T, int info_len = 2>
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
public:
    MemoryRiver() = default;

    MemoryRiver(const string& file_name) : file_name(file_name) {}

    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        if (access(file_name.c_str(), F_OK) == 0) 
          return;//这句话如果不加会导致文件狂狂重开清零，玛德
        file.open(file_name, std::ios::out);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i)
          file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    //读出第n个int的值赋给tmp，1_base
    void get_info(int &tmp, int n) {
        if (n > info_len) return;
        file.open(file_name);
        file.seekg((n - 1) * sizeof(int), std::ios::beg);
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    //将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n) {
        if (n > info_len) return;
        file.open(file_name);
        file.seekp((n - 1) * sizeof(int), std::ios::beg);
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    /*void write(T &t, long long place_size) {
      file.open(file_name, std::ios::in | std::ios::out);
      file.seekp(0, std::ios::end);
      //int index = file.tellp();
      file.write(reinterpret_cast<char *>(&t), sizeof(T));
      file.close();
      //return index;
      return;
    }*/
    void write(T &t, long long place_size, int size = 1) {
      file.open(file_name);
      file.seekp(place_size, std::ios::beg);
      int index = file.tellp();
      file.write(reinterpret_cast<char *>(&t), sizeof(T) * size);
      file.close();
      return;
    }
    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void update(T &t, const int index) {
      file.open(file_name);
      file.seekp(index, std::ios::beg);
      file.write(reinterpret_cast<char *>(&t), sizeof(T));
      file.close();
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    
    void read(T &t, const int index, int size = 1) {
      file.open(file_name);
      file.seekg(index);
      file.read(reinterpret_cast<char *>(&t), sizeof(T) * size);
      file.close();
      return;
    }
    //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
    void Delete(int index) {
      file.open(file_name);
      file.seekp(index);
      int tmp = 0;
      file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
      file.close();
      return;
    }
};

class Block
{
public:
  int value;//元素值
  int block_elementnum;//某一个块里有多少组数据
  int block_pos;//这是第几个块
  int block_next;//用数组模拟链表时查找块的后继
  bool insert_flag;//是否未能插入
  bool find_flag;//是否成功找到
  char index[100];
  static const int limited_elementnum;
  Block()
  {
    value = 0;
    block_elementnum = 0;
    block_pos = 0;
    block_next = 0;
    insert_flag = 0;
    find_flag = 0;
    memset(index, '\0', sizeof(index));
  }
  bool operator<(const Block &other) const 
  {
    if (strcmp(index, other.index) != 0)
    {
      if (strcmp(index, other.index) < 0) return 1;
      else return 0;
    }
    return value < other.value;
  }
  bool operator>(const Block &other) const 
  {
    if (strcmp(index, other.index) != 0)
    {
      if (strcmp(index, other.index) > 0) return 1;
      else return 0;
    }
    return value > other.value;
  }
  bool operator==(const Block &other) const 
  {
    return (strcmp(index, other.index) == 0) && value == other.value;
  }
};
Block list_index[1000];//要找到是第几个块的索引数组（用数组模拟链表），采用块头index和value记录数据的方法进行查找等功能，这里感谢zjx同学提供的思路
Block list_data[1000];//在找到某个块以后，用这个数组来存储块中的数据
MemoryRiver<Block, 2> DataBase;
int pos;//第几个块

int find_whichblock(Block &target_data)
{
  int blocknum;//一共有多少个块
  int index = 1;//最终应该是在第几个块里进行操作
  DataBase.get_info(blocknum, 1);
  //cout << blocknum << endl;
  DataBase.read(list_index[1], 8, blocknum);//把整个模拟链表的数组给读出来
  //cout << list_index[1].block_next << endl;
  for (int index_now = 1;index_now != 0;index_now = list_index[index_now].block_next)
  {
    if (target_data < list_index[index_now])
    {
     // cout << index << endl;
      return index;
    }
    index = index_now;
  }
  
  return index;
}//找到应该在哪一个块里操作这个数据


Block insert(Block &target_data, int element_num, bool insert_flag)
{
  DataBase.read(list_data[1], pos * sizeof(Block) * limited_elementnum + 8, element_num);//把整个块读出来
  //cout << list_data[1].index << " " << list_data[1].value << " " <<list_data[2].index << " " << list_data[2].value << endl;
  int insert_pos = lower_bound(list_data + 1, list_data + element_num + 1, target_data) - list_data - 1;
  //cout << insert_pos << endl;
  if (list_data[insert_pos + 1] == target_data) 
  {
    insert_flag = 1;
    return target_data;
  }//如果已经有重复的，就不插入进去了
  else if (insert_pos == 0)
  {
    DataBase.write(target_data, pos * sizeof(Block) * limited_elementnum + 8);
    DataBase.write(list_data[1], pos * sizeof(Block) * limited_elementnum + 8 + sizeof (Block), element_num);
    return target_data;
  }//如果是最小的一个，那就放到第一个位置
  else  
  {
    DataBase.write(list_data[1], pos * sizeof(Block) * limited_elementnum + 8, insert_pos);//插入原数组的前inset_pos个
    DataBase.write(target_data, pos * sizeof(Block) * limited_elementnum + 8 + insert_pos * sizeof(Block));
    if (insert_pos != element_num)
      DataBase.write(list_data[insert_pos + 1], pos * sizeof(Block) * limited_elementnum + 8 + (insert_pos + 1) * sizeof(Block), element_num - insert_pos);
    //再插后面部分
    return list_data[1];
  }
}//这一部分利用重新魔改write函数然后分别插入数组前半段和后半段的思路来源于llz同学，感恩

bool find(Block &target_data, int element_num, bool &find_flag)
{
  DataBase.read(list_data[1], pos * sizeof(Block) * limited_elementnum + 8, element_num);//把整个块读出来
  //cout << list_data[1].index << " " << list_data[1].value << " " <<list_data[2].index << " " << list_data[2].value << endl;
  int find_pos = lower_bound(list_data + 1, list_data + element_num + 1, target_data) - list_data - 1;
  for (int i = find_pos + 1; i <= element_num; i++)
  {
    if (strcmp (target_data.index, list_data[i].index) == 0)
    {
      find_flag = 1;
      cout << list_data[i].value << " ";
    }//找到就输出所有符合条件的条目
    else return 0;
  }
  return 1;
}

bool delete_data(Block &target_data, int element_num)
{
  DataBase.read(list_data[1], pos * sizeof(Block) * limited_elementnum + 8, element_num);
  int delete_pos = lower_bound(list_data + 1, list_data + element_num + 1, target_data) - list_data - 1;
  //cout << delete_pos << endl;
  if ((target_data == list_data[delete_pos + 1]) == 0) return 0;//删的东西不存在
  else if (delete_pos == 0)
  {
    DataBase.write(list_data[2], pos * sizeof(Block) * limited_elementnum + 8, element_num - 1);
    return 1;
  }//如果删的是第一个
  else  
  {
    DataBase.write(list_data[1], pos * sizeof(Block) * limited_elementnum + 8, delete_pos);
    DataBase.write(list_data[delete_pos + 2], pos * sizeof(Block) * limited_elementnum + 8 + delete_pos * sizeof(Block), element_num - delete_pos - 1);
    return 1;
  }//否则就删掉其中的那个符合的
}

void block_split (int block_pos, int element_num)
{
  int blocknum;
  DataBase.get_info(blocknum, 1);
  blocknum++;
  DataBase.write_info(blocknum, 1);
  int mid = element_num / 2;
  Block temp_now;
  DataBase.read(temp_now, (block_pos - 1) * sizeof(Block) + 8);//读出来原来的块头
  DataBase.read(list_data[1], temp_now.block_pos * limited_elementnum * sizeof(Block) + 8, element_num);//读出来原来的块
  //cout << list_data[1].index << " " << list_data[1].value << " " <<list_data[2].index << " " << list_data[2].value << endl;
  Block temp_split;
  temp_split = list_data[mid + 1];
  temp_split.block_elementnum = element_num - mid;
  temp_split.block_next = temp_now.block_next;
  temp_split.block_pos = blocknum;//把新块块头相关设定设定好
  //cout << temp_now.block_pos << " " << temp_split.block_pos << endl;
  DataBase.write(list_data[mid + 1], blocknum * limited_elementnum * sizeof(Block) + 8, element_num - mid);//把裂出来的新块放到末尾
  DataBase.write(temp_split, (blocknum - 1) * sizeof(Block) + 8);//把新块头写到块头末尾
  temp_now.block_elementnum = mid;//llz同学证实了似乎确实可以通过改变这里的数据个数来假装后半部分没有元素了（）（）
  temp_now.block_next = blocknum;
  DataBase.write(temp_now, (block_pos - 1) * sizeof(Block) + 8);//把更新过的原块头写进去
  return ;
}//进行裂块操作