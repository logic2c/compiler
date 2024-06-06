#include"koopa.h"
#include<iostream>
#include"ast.h"
using namespace std;
string spcount_str(int a);
string tcount_str(int t);
//string loadrsb(const koopa_raw_binary_t &reb);
string loadrs(const koopa_raw_value_t &value,string reg,const koopa_raw_value_t &value1 =nullptr);
string loadsp(const koopa_raw_value_t &value);
string loadargs(const koopa_raw_value_t &value,int num);
void Visit(const koopa_raw_program_t &raw_program);
void Visit(const koopa_raw_slice_t &slice);
void Visit(const koopa_raw_function_t &func);
void Visit(const koopa_raw_basic_block_t &bb);
void Visit(const koopa_raw_value_t &value);
void Visit(const koopa_raw_return_t &ret);
void Visit(const koopa_raw_integer_t &rint);
void Visit(const koopa_raw_binary_t &reb);
void Visit(const koopa_raw_store_t &res);
void Visit(const koopa_raw_load_t &rel);
void Visit(const koopa_raw_jump_t &rej);
void Visit(const koopa_raw_branch_t &reb);
void Visit(const koopa_raw_call_t &rec, const koopa_raw_type_tag_t &ty);
//int Visit_to_sp(const koopa_raw_value_t &value);