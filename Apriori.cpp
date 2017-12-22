/***********************************
Apriori算法描述：
输入:数据库D，最小支持度阈值min_sup
输出:D中的频繁集L
(1)Begin
(2)L[1]=1-频繁项集
(3)for(k=2;L[k-1]!=∅;k++) do Begin
(4)C[k]=Apriori_gen(L[k-1]); //调用函数Apriori_gen(L[k-1])通过频繁(k-1)-项集产生候选k-项集
(5)for所有数据集t∈D do Begin  //扫描D用于计数
(6)C[t]=subset(C[k],t)       //用subset找出该事物中候选的所有子集
(7)for所有候选集c∈C[t] do
(8)c.count++
(9)end
(10)L[k]={c∈C[k]|c.count>=min_sup}
(11)end
(12)end
(13)return L[1]∪L[2]∪L[k]∪...∪L[m]   //形成频繁项集的集合
**********************************/
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<algorithm>
#include<cstring>
using namespace std;

class Apriori
{
    public:
      Apriori(size_t is=0,unsigned int mv=0)
      {
          item_size=is;
          min_value=mv;
      }
      //~Apriori() {}
      void getItem();
      map<vector<string>,unsigned int> find_freitem();//求事物的频繁项
      //连接两个k-1级频繁项，得到第k级频繁项
      map<vector<string>,unsigned int> apri_gen(unsigned int K,map<vector<string>,unsigned int> K_item);
      //展示频繁项目
      void showAprioriItem(unsigned int K,map<vector<string>,unsigned int> showmap);
    private:
      map<int,vector<string> > item;             //存储所有最开始的事务及其项
      map<vector<string>,unsigned int> K_item;   //存储频繁项集
      size_t item_size;                          //事务数目
      unsigned int min_value;                    //最小阈值
};

void Apriori::getItem()                          //用户输入最初的事务集
{
  int ci=item_size;
  for(int i=0;i<ci;i++)
  {
    string str;
    vector<string> temp;
    cout<<"请输入第 "<<i+1<<"个事务的项集(123 end):"<<endl;
    while(cin>>str&&str!="123")
    {
      temp.push_back(str);
    }
    sort(temp.begin(),temp.end());
    pair<map<int,vector<string> >::iterator, bool>ret = item.insert(make_pair(i+1,temp));
    if(!ret.second)
    {
      --i;
      cout<<"输入的元素已经存在，请重新输入:"<<endl;
    }
  }
  cout<<"------------运行结果如下:-------------"<<endl;
}

map<vector<string>,unsigned int >Apriori::find_freitem()
{
  unsigned int i=1;
  bool isEmpty=false;
  map<int,vector<string> >::iterator mit;
  for(mit=item.begin();mit!=item.end();mit++)
  {
    vector<string>vec=mit->second;
    if(vec.size()!=0)
      break;
  }
  if(mit==item.end())
  {
    isEmpty=true;
    cout<<"事务集为空，程序无法继续运行..."<<endl;
    map<vector<string>,unsigned int> empty;
    return empty;
  }
  while(true)
  {
    map<vector<string>,unsigned int> K_itemTemp=K_item;

    K_item=apri_gen(i++,K_item);

    if(K_itemTemp==K_item)
    {
      i=UINT_MAX;
      break;
    }
    //判断是否需要进行下一次寻找
    map<vector<string>,unsigned int> pre_K_item=K_item;
    size_t Kitemsize=K_item.size();
    //存储应该删除的第K级频繁项集，不能和其他K级频繁项集构成第K+1项集的集合
    if(Kitemsize!=1 && i!=1)
    {
      vector<map<vector<string>,unsigned int>::iterator> eraseVecMit;
      map<vector<string>,unsigned int>::iterator pre_K_item_it1=pre_K_item.begin(),pre_K_item_it2;
      while(pre_K_item_it1!=pre_K_item.end())
      {
         map<vector<string>,unsigned int>::iterator mit=pre_K_item_it1;
         bool isExist=true;
         vector<string> vec1;
         vec1=pre_K_item_it1->first;
         vector<string> vec11(vec1.begin(),vec1.end()-1);
         while(mit==pre_K_item.end())
         {
          vector<string> vec2;
          vec2=mit->first;
          vector<string> vec22(vec2.begin(),vec2.end()-1);
         if(vec11==vec22)
            break;
         ++mit;
         }
        if(mit==pre_K_item.end())
           isExist=false;
        if(!isExist && pre_K_item_it1!=pre_K_item.end())
           eraseVecMit.push_back(pre_K_item_it1);       //该第K级频繁项应该删除

        ++pre_K_item_it1;
      }
      size_t eraseSetSize=eraseVecMit.size();
      if(eraseSetSize=Kitemsize)
        break;
       else
       {
       vector<map<vector<string>,unsigned int>::iterator>::iterator currentErs=eraseVecMit.begin();
       while(currentErs!=eraseVecMit.end())    //删除所有应该删除的第K级频繁项
       {
         map<vector<string>,unsigned int>::iterator eraseMit= *currentErs;
         K_item.erase(eraseMit);
         ++currentErs;
       }
     }
   }
   else
      if(Kitemsize==1)
        break;
  }
  cout<<endl;
  showAprioriItem(i,K_item);
  return K_item;
}

map<vector<string>,unsigned int>Apriori::apri_gen(unsigned int K,map<vector<string>,unsigned int>K_item)
{
  if(1==K)      //求候选集C1
  {
    size_t c1=item_size;
    map<int,vector<string> >::iterator mapit=item.begin();
    vector<string> vec;
    map<string,unsigned int>c1_itemtemp;
    while(mapit!=item.end())    //统计原事务中的所有单项
    {
      vector<string>temp=mapit->second;
      vector<string>::iterator vecit=temp.begin();
      while(vecit!=temp.end())
      {
        pair<map<string,unsigned int>::iterator,bool>ret=c1_itemtemp.insert(make_pair(*vecit++,1));
        if(!ret.second)
        {
          ++ret.first->second;
        }
      }
      ++mapit;
    }
     map<string,unsigned int>::iterator item_it=c1_itemtemp.begin();
     map<vector<string>,unsigned int>c1_item;
     while(item_it!=c1_itemtemp.end())       //构造第1级频繁项集
     {
        vector<string>temp;
        if(item_it->second>=min_value)
        {
          temp.push_back(item_it->first);
          c1_item.insert(make_pair(temp,item_it->second));
        }
        ++item_it;
     }
     return c1_item;
  }
  else
  {
    cout<<endl;
    showAprioriItem(K-1,K_item);
    map<vector<string>,unsigned int>::iterator ck_item_it1=K_item.begin(),ck_item_it2;
    map<vector<string>,unsigned int>ck_item;
    while(ck_item_it1!=K_item.end())
    {
      ck_item_it2=ck_item_it1;
      ++ck_item_it2;
      map<vector<string>,unsigned int>::iterator mit=ck_item_it2;

      //取当前第K级频繁项与后面的第K级频繁项集联合，注意联合条件
      //联合条件：两个频繁项的前K-1项完全相同，只是第K项不同，然后两个来联合生成第K+1级候选频繁项
      while(mit!=K_item.end())
      {
        vector<string>vec,vec1,vec2;
        vec1=ck_item_it1->first;
        vec2=mit->first;
        vector<string>::iterator vit1,vit2;

        vit1=vec1.begin();
        vit2=vec2.begin();

        while(vit1<vec1.end() && vit2<vec2.end())
        {
          string str1=*vit1;
          string str2=*vit2;
          ++vit1;
          ++vit2;
          if(K==2||str1==str2)
          {
            if(vit1!=vec1.end() && vit2!=vec2.end())
            {
              vec.push_back(str1);
            }
          }
          else{
            break;
          }
        }
        if(vit1!=vec1.end() && vit2!=vec2.end())
        {
          --vit1;
          --vit2;
          string str1=*vit1;
          string str2=*vit2;
          if(str1>str2)
          {
            vec.push_back(str2);
            vec.push_back(str1);
          }
          else
          {
            vec.push_back(str1);
            vec.push_back(str2);
          }
          map<int,vector<string> >::iterator base_item=item.begin();
          unsigned int Acount=0;
          while(base_item!=item.end())     //统计该K+1级候选项在原事务集中出现的次数
          {
             unsigned int count=0,mincount=UINT_MAX;
             vector<string>vv=base_item->second;
             vector<string>::iterator vecit,bvit;
             for(vecit=vec.begin();vecit<vec.end();vecit++)
             {
               string t=*vecit;
               count=0;
               for(bvit=vv.begin();bvit!=vv.end();bvit++)
               {
                 if(t==*bvit)
                   count++;
               }
               mincount=(count<mincount?count:mincount);
             }
             if(mincount>=1 && mincount!=UINT_MAX)
                Acount+=mincount;
             ++base_item;
          }
          if(Acount>=min_value &&Acount!=0)
          {
            sort(vec.begin(),vec.end());
            //该第K+1级候选项为频繁项，插入频繁项集
            pair<map<vector<string>,unsigned int>::iterator,bool>ret=ck_item.insert(make_pair(vec,Acount));
            if(!ret.second)
            {
              ret.first->second+=Acount;
            }
          }
        }
        ++mit;
      }
      ++ck_item_it1;
    }
    if(ck_item.empty())     //该第K+1级频繁项集为空，说梦调用结束，把上一级频繁项集返回
        return K_item;
    else
        return ck_item;
  }
}


void Apriori::showAprioriItem(unsigned int K,map<vector<string>,unsigned int> showmap)
{
  map<vector<string>,unsigned int>::iterator showit=showmap.begin();
  if(K!=UINT_MAX)
     cout<<endl<<"第"<<K<<"级频繁项集合为:"<<endl;
  else
     cout<<"最终的频繁项集为:"<<endl;
  cout<<"项 集"<<" \t "<<"频率"<<endl;
  while(showit!=showmap.end())
  {
    vector<string>vec=showit->first;
    vector<string>::iterator vecit=vec.begin();
    cout<<"{";
    while(vecit!=vec.end())
    {
      cout<<*vecit<<" ";
      ++vecit;
    }
    cout<<"}"<<" \t ";
    cout<<showit->second<<endl;
    ++showit;
  }
}


unsigned int parseNumber(const char *str)   //对用户输入的数字进行判断和转换
{
  if(str==NULL)
    return 0;
  else
  {
    unsigned int num=0;
    size_t len=strlen(str);
    for(size_t i=0;i<len;i++)
    {
      num *= 10;
      if(str[i]>='0' && str[i]<='9')
          num += str[i]-'0';
      else
          return 0;
    }
    return num;
  }
}


int main()
{
  //Apriori a;
  unsigned int itemsize=0;
  unsigned int min;

  do
  {
    cout<<"请输入事务数:"<<endl;
    char *str=new char;
    cin>>str;
    itemsize=parseNumber(str);
    if(itemsize==0)
        cout<<"请输入大于0的整数"<<endl;
  }while(itemsize==0);

  do
  {
    cout<<"请输入最小阈值:"<<endl;
    char *str=new char;
    cin>>str;
    min=parseNumber(str);
    if(min==0)
       cout<<"请输入大于0的整数:"<<endl;
  }while(min==0);

  Apriori a(itemsize,min);
  a.getItem();
  map<vector<string>,unsigned int>AprioriMap=a.find_freitem();
  //a.showAprioriItem(UINT_MAX,AprioriMap);

  return 0;
}
