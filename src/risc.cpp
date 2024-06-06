#include"risc.h"
#include<iomanip>
#include<cassert>
#include <iterator>
#include <unordered_map>
#include <string>

using namespace std;
const string SP_POINTER = "sp";
static bool iscall = false;
static int nowsp = 0;
static int lensp = 0;
//static int regtable[500]={0};
unordered_map<int,long long> mapreg;
 
string spcount_str(int a)
{
  return to_string(a) + "(sp)";
}
string tcount_str(int t)
{
  //if(t>=14) return "t"+to_string(t-14);
  if(t>=7) return "a"+to_string(t-7);
  return "t"+to_string(t);
}


string loadrs(const koopa_raw_value_t &value,string reg,const koopa_raw_value_t &value1)
{
  const auto &kind = value->kind;
  if(kind.tag == KOOPA_RVT_INTEGER)
  {
    if(kind.data.integer.value!=0)
    {       
      cout<<"  "<<"li "<<reg<<", ";
      Visit(value);
      cout<<endl;       
      return reg; //不改变
    }
    else return "x0";
  }
  else if(kind.tag == KOOPA_RVT_FUNC_ARG_REF)
  {
    if(kind.data.func_arg_ref.index < 8)
      return loadargs(value, kind.data.func_arg_ref.index);
    else
    {
      string sp = spcount_str((kind.data.func_arg_ref.index - 8)*4 + lensp);
      cout<<"  "<<"lw "<<"t4, "<<sp<<endl;
      mapreg[(long long)value1] = (kind.data.func_arg_ref.index - 8)*4 + lensp;
      nowsp -= 4;//抵消alloc
      return "bad";
    }
  }
  else 
  {
    cout<<"  lw "<<reg<<", "<<spcount_str(mapreg[(long long)(value)])<<endl;
    return reg;
  }
  return "";
}

string loadargs(const koopa_raw_value_t &value,int num)
{
  //const auto &kind = value->kind;
  if(num < 8)
  {
    return "a"+to_string(num);
  }
  else {
    return spcount_str((num - 8)*4);
  }
}

string loadsp(const koopa_raw_value_t &value)
{
  return spcount_str(mapreg[(long long)(value)]);
}
void Visit(const koopa_raw_program_t &raw_program)
{
    Visit(raw_program.values);
    Visit(raw_program.funcs);
    return;
}
void Visit(const koopa_raw_slice_t &slice) 
{
  for (size_t i = 0; i < slice.len; ++i) 
  {
    auto ptr = slice.buffer[i];
    // 根据 slice 的 kind 决定将 ptr 视作何种元素
    switch (slice.kind) {
      case KOOPA_RSIK_FUNCTION:
        // 访问函数
        Visit(reinterpret_cast<koopa_raw_function_t>(ptr));
        break;
      case KOOPA_RSIK_BASIC_BLOCK:
        // 访问基本块
        if(i != 0)
          cout<<reinterpret_cast<koopa_raw_basic_block_t>(ptr)->name+1<<":"<<endl;
        Visit(reinterpret_cast<koopa_raw_basic_block_t>(ptr));
        break;
      case KOOPA_RSIK_VALUE:
        // 访问指令
        Visit(reinterpret_cast<koopa_raw_value_t>(ptr));
        break;
      default:
        // 我们暂时不会遇到其他内容, 于是不对其做任何处理
        assert(false);
    }
  }
}
//访问函数
void Visit(const koopa_raw_function_t &func) {
  // 执行一些其他的必要操作
  // ...
  // 访问所有基本块
  //func->name + 1就可以达成删除名字中第一个字符的目的
  if(func->bbs.len == 0)
  {
    return;
  }
  cout<<"  .text"<<endl;
  cout<<"  .globl "<<func->name+1<<endl;
  cout<<func->name+1<<":"<<endl;
  int lens = 0 ,lenr = 0, lena = 0;
  // prologue
  //目前一个函数只有一次，如果有需求后续再更改
  //求S的长度
  for(size_t i = 0;i<func->bbs.len;i++)
  {
    auto ptr1 = func->bbs.buffer[i];
    for(size_t j = 0; j < reinterpret_cast<koopa_raw_basic_block_t>(ptr1)->insts.len;j++)
    {
      auto ptr2 = reinterpret_cast<koopa_raw_basic_block_t>(ptr1)->insts.buffer[j];

      auto a = reinterpret_cast<koopa_raw_value_t>(ptr2)->ty->tag;
      if(a == (KOOPA_RTT_INT32)) 
      {
        lens += 4;
      }        
      else if (a == (KOOPA_RTT_POINTER))
      {
        if(reinterpret_cast<koopa_raw_value_t>(ptr2)->ty->data.pointer.base->tag == (KOOPA_RTT_INT32))
          lens += 4;
      }
      //s与r、a等分开计算
      int b = reinterpret_cast<koopa_raw_value_t>(ptr2)->kind.tag;
      int l = reinterpret_cast<koopa_raw_value_t>(ptr2)->kind.data.call.args.len;
      if(b == (KOOPA_RVT_CALL))
      {
        //即存在call指令
        lenr = 4;
        iscall = true;
        if(l > 8)//某函数有超过8个参数
        {
          lena = max(lena, (l-8)*4);
        }
      }
    }
  }
  //求R的长度
  //求
  /*addi 指令中立即数的范围是 [−2048,2047], 即 12 位有符号整数的范围 
  超过需要用 li 加载立即数到一个临时寄存器, 然后用 add 指令来更新 sp */
  lensp = lens + lena + lenr;
  if(lensp > 0 && lensp <= 2048 ) 
  {
    lensp = ((lensp-1)/16+1) *16; //对齐至16字节3
    cout << "  addi sp, sp, " << -lensp<<endl;
  }
  if(lenr != 0 )
  {
    cout<< "  sw ra, "<<(lensp - 4)<<"(sp)"<<endl;
  }
  nowsp += lena;
  Visit(func->bbs);
  // for(auto it = mapreg.begin();it != mapreg.end();it++)
  // {
  //     cout<<"look: "<<"  "<<it->second<<endl;
  // }
  nowsp = 0;//重置nowsp，这就是全局变量不好的地方
  cout<<endl;
}

// 访问基本块
void Visit(const koopa_raw_basic_block_t &bb) {
  Visit(bb->insts);
}

// 访问指令
void Visit(const koopa_raw_value_t &value) {
  // 根据指令类型判断后续需要如何访问
  const auto &kind = value->kind;
  const auto &type = value->ty->tag;
  switch (kind.tag) {
    case KOOPA_RVT_RETURN:
      // 访问 return 指令
      Visit(kind.data.ret);
      break;
    case KOOPA_RVT_INTEGER:
      // 访问 integer 指令
      Visit(kind.data.integer);
      break;
    case KOOPA_RVT_BINARY:
      // 访问 运算 指令
      Visit(kind.data.binary);
      mapreg[(long long)value] = nowsp;
      nowsp += 4;
      break;
    case KOOPA_RVT_ALLOC:
      //访问 局部变量申请 指令
      //不生成指令，只是改变sp    
      mapreg[(long long)value] = nowsp; 
      if(value->ty->data.pointer.base->tag == (KOOPA_RTT_INT32))
      {
        nowsp += 4;
      } 
      break;
    case KOOPA_RVT_STORE:
      //访问 存储 指令 a = ...
      //没有返回值 不改变sp 
      Visit(kind.data.store);
      break;
    case KOOPA_RVT_LOAD:
      //访问 访问 指令 ... = a
      //返回值为a 改变sp
      Visit(kind.data.load);
      mapreg[(long long)value] = nowsp;
      nowsp += 4;
      break;
    case KOOPA_RVT_JUMP:
      //访问 跳转 指令
      Visit(kind.data.jump);
      break;
    case KOOPA_RVT_BRANCH:
      //访问 条件跳转 指令
      Visit(kind.data.branch);
      break;
    case KOOPA_RVT_CALL:
      //访问 调用函数 指令
      Visit(kind.data.call,type);
      if(type == KOOPA_RTT_INT32)
      {
        mapreg[(long long)value] = nowsp;
        nowsp += 4;
      }
      break;
    case KOOPA_RVT_FUNC_ARG_REF:
      //访问  函数参数 指令
      //与 integer 同类型 事实上没有这个指令
      
      break;
    default:
      // 其他类型暂时遇不到
      assert(false);
  }
}

void Visit(const koopa_raw_integer_t &rint)
{ 
  cout<<rint.value;
}

void Visit(const koopa_raw_return_t &ret)
{
    if(ret.value == nullptr)
    {
      cout<<"  ret"<<endl;
      return;
    }
    if(ret.value->kind.tag == KOOPA_RVT_INTEGER)
    {
      cout<<"  li a0, ";
      Visit(ret.value);
      cout<<endl;
    }
    else {
      string rs = loadsp(ret.value);
      cout<<"  "<<"lw "<<"a0, "<<rs<<endl;
    }
    if(iscall == true)
      cout<<"  lw ra, "<<spcount_str(lensp-4)<<endl;
    if(lensp != 0)
      cout << "  addi sp, sp, " << lensp<<endl;
    cout<<"  ret"<<endl;
}

void Visit(const koopa_raw_binary_t &reb)
{
  string rs1="t0",rs2="t1"; 
  const auto &op = reb.op;
  rs1 = loadrs(reb.lhs,rs1);
  rs2 = loadrs(reb.rhs,rs2);
  string rs="t0";
  switch (op) {
    case KOOPA_RBO_EQ:
      cout<<"  "<<"xor "<<rs<<", "<<rs1<<", "<<rs2<<endl;
      cout<<"  "<<"seqz "<<rs<<", "<<rs1<<endl;
      break;
    case KOOPA_RBO_SUB:    
      cout<<"  " <<"sub "<<rs<<", "<<rs1<<", "<<rs2<<endl;
      break;
    case KOOPA_RBO_ADD:
      cout<<"  " <<"add "<<rs<<", "<<rs1<<", "<<rs2<<endl;
      break;
    case KOOPA_RBO_MUL:
      cout<<"  " <<"mul "<<rs<<", "<<rs1<<", "<<rs2<<endl;
      break;
    case KOOPA_RBO_DIV:
      cout<<"  " <<"div "<<rs<<", "<<rs1<<", "<<rs2<<endl;
      break;
    case KOOPA_RBO_MOD:
      cout<<"  " <<"rem "<<rs<<", "<<rs1<<", "<<rs2<<endl;
      break;
    case KOOPA_RBO_AND:
      cout<<"  " <<"and "<<rs<<", "<<rs1<<", "<<rs2<<endl;
      break;
    case KOOPA_RBO_OR:
      cout<<"  " <<"or "<<rs<<", "<<rs1<<", "<<rs2<<endl;
      break;
    case KOOPA_RBO_NOT_EQ:
      cout<<"  " <<"xor "<<rs<<", "<<rs1<<", "<<rs2<<endl;
      cout<<"  " <<"snez "<<rs<<", "<<rs1<<endl;
      break;
    case KOOPA_RBO_GT:
      cout<<"  " <<"sgt "<<rs<<", "<<rs1<<", "<<rs2<<endl;
      break;
    case KOOPA_RBO_LT:
      cout<<"  " <<"slt "<<rs<<", "<<rs1<<", "<<rs2<<endl;
      break;
    case KOOPA_RBO_GE:
      cout<<"  " <<"slt "<<rs<<", "<<rs1<<", "<<rs2<<endl;
      cout<<"  " <<"seqz "<<rs<<", "<<rs1<<endl;
      break;
    case KOOPA_RBO_LE:
      cout<<"  " <<"sgt "<<rs<<", "<<rs1<<", "<<rs2<<endl;
      cout<<"  " <<"seqz "<<rs<<", "<<rs1<<endl;
      break;
    default:
      break;
  }
  rs = spcount_str(nowsp);
  cout<<"  sw "<<"t0"<<", "<<rs<<endl;
}

// sw rs2 , offset(rs1)
void Visit(const koopa_raw_store_t &res)
{ 
  string rs1 = "t0"; 
  rs1=loadrs(res.value,rs1,res.dest); 
  if(rs1 == "bad") return;
  string rs2 = loadsp(res.dest);
  cout<<"  sw "<<rs1<<", "<<rs2<<endl;
}

void Visit(const koopa_raw_load_t &rel)
{ 
  string rs1 = "t0";
  rs1=loadrs(rel.src,rs1);  
  string rs2 = spcount_str(nowsp);
  cout<<"  sw "<<rs1<<", "<<rs2<<endl;
}

void Visit(const koopa_raw_jump_t &rej)
{ 
  string t2 = rej.target->name;
  t2.erase(t2.begin());
  cout<<"  "<<"j "<<t2<<endl;
}

void Visit(const koopa_raw_branch_t &reb)
{ 
  //cond
  string rs1 = "t0";
  string t1 = reb.true_bb->name;
  t1.erase(t1.begin());
  string t2 = reb.false_bb->name;
  t2.erase(t2.begin());
  rs1 = loadrs(reb.cond, rs1);
  cout<<"  "<<"bnez "<<rs1<<", "<<t1<<endl;
  cout<<"  "<<"j "<<t2<<endl;
}

void Visit(const koopa_raw_call_t &rec, const koopa_raw_type_tag_t &ty)
{ 
  //逆序遍历必须使用int，否则i--会导致无符号异常
  for(int i = rec.args.len - 1; i >= 0 ; i--)
  {
    auto ptr = rec.args.buffer[i]; 
    auto b = reinterpret_cast<koopa_raw_value_t>(ptr);
    string reg = loadargs(b,i);
    auto a = b->kind.tag;
    
    if(i < 8)
    {
      if(a == KOOPA_RVT_INTEGER)
      {
        auto c=b->kind.data.integer.value;
        if(c == 0)
        {
          cout<<"  "<<"lw "<<reg<<", "<<"x0"<<endl;
        }
        else {
          cout<<"  li "<<reg<<", "<<c<<endl;
        }
      }
      else
      {
        cout<<"  lw "<<reg<<", "<< spcount_str(mapreg[(long long)b])<<endl;     
      }
    }
    else 
    {
      string sp = spcount_str((i-8)*4);
      if(a == KOOPA_RVT_INTEGER)
      {
        auto c=b->kind.data.integer.value;
        if(c == 0)
        {
          cout<<"  "<<"lw "<<"a0"<<", "<<"x0"<<endl;
        }
        else {
          cout<<"  li "<<"a0"<<", "<<c<<endl;
        }
      }
      else
      {
        cout<<"  lw "<<"a0"<<", "<< spcount_str(mapreg[(long long)b])<<endl;     
      }
      cout<<"  "<<"sw "<<"a0"<<", "<<sp<<endl;
    }

  }
  cout<<"  call "<<rec.callee->name+1<<endl;
  if(ty == KOOPA_RTT_INT32)
  {
    string rs = spcount_str(nowsp);
    cout<<"  sw "<<"a0, "<<rs<<endl;
  }
}