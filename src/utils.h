#pragma once
#include<iostream>
#include <string>
#include<unordered_map>
#include<vector>
#include<cassert>
#include"koopa.h"
using namespace std;
koopa_raw_type_kind *make_type_from_int_pointer();
koopa_raw_type_kind *make_type_from_kind(koopa_raw_type_tag_t kind); 
koopa_raw_slice_t make_slice_from_vector(std::vector<const void*> vec,koopa_raw_slice_item_kind_t kind1);
koopa_raw_slice_t make_slice_from_null(koopa_raw_slice_item_kind_t kind1);
koopa_raw_value_data *make_value_from_int(int val);
koopa_raw_value_data *make_jump_from_block(koopa_raw_basic_block_t target);
koopa_raw_value_data *make_branch_from_block(const void* cond,koopa_raw_basic_block_t target1,koopa_raw_basic_block_t target2);
koopa_raw_value_data *make_param_value_from_kind(koopa_raw_type_tag_t ky);
koopa_raw_value_data *make_value_from_init();
const char * make_name_from_string(string name,int tag = 1);

enum ValueType { Const_Type, Var_Type, Func_Type, Array, Pointer ,ERROR_TYPE};

struct Value{
    //string name;
    ValueType type;
    union Valueval{
        int const_val;
        koopa_raw_value_t var_val;
        koopa_raw_function_t func_val;
    }data;
};

//维护该作用域所代表的一条链而不是维护整个符号表所代表的树
class SymbolList {
    private:
    vector<unordered_map<string,Value>> symbollist;
    int valuescope;
    public:
    ~SymbolList() = default;
    SymbolList(int init1)//用来赋初值
    {
        valuescope = init1;
    }
    int getcurscope()
    {
        return valuescope;
    }
    void insert(Value val,string name)
    {
        symbollist.back()[name] = val;
    }
    Value getval(string name)
    {
        Value retv;
        /*经典错误，vector逆序遍历下标一定要先减*/
        for (auto it = symbollist.rbegin(); it != symbollist.rend();it++) 
        {            //return it->at(name);
            if (it->find(name) != it->end()) {
                return it->at(name);
            }
        }
        //出来了说明没有找到
        retv.type = ERROR_TYPE;
        retv.data.const_val = -1;
        return retv;
    }
    Value dirgetval(string name)
    {
        Value retv;
        auto it = symbollist.end();
        it --;
        /*经典错误，vector逆序遍历下标一定要先减*/
        if (it->find(name) != it->end()) {
            return it->at(name);
        }
        //出来了说明没有找到
        retv.type = ERROR_TYPE;
        retv.data.const_val = -1;
        return retv;
    }
    
    void newscope()
    {
        symbollist.push_back(unordered_map<string,Value>());
        valuescope++;
    }
    void deletescope()
    {
        symbollist.pop_back();
        valuescope--;
    }
    void isdeclared(string ident)
    {
        Value temp;
        string tmp = ident + to_string(valuescope);
        temp = dirgetval(ident);
        if(temp.type != ERROR_TYPE) //重复定义
        {
            cout<< tmp << " is redeclared!"<<endl;
            assert(false);
        } 
        //只要能正常退出就没问题
    }
};

//维护一个基本块栈，基本块有个很好的性质
//结束之后不会再被调用
class BlockMaintainer {
    private:
    koopa_raw_function_t cur_func;
    vector<const void* >  block_insts;
    vector<const void* > *basic_block_buf;
    public:
    //基本就是完成ast.h中block需要完成的内容，完成后先进行基础测试.
    vector<const void *> GetBasicBlock()
    {
        return *basic_block_buf;
    }
    void SetCurrentFunction(koopa_raw_function_t _cur_func)
    {
        cur_func = _cur_func;
    }
    void SetBasicBlockBuf(std::vector<const void *> *_basic_block_buf)
    {
        basic_block_buf = _basic_block_buf;
    }
    void FinishBasicBlock()
    {
        if(basic_block_buf->size() > 0)
        {
            //给basic_block一个指针
            koopa_raw_basic_block_data_t *last_block = 
            (koopa_raw_basic_block_data_t *)(*basic_block_buf)[basic_block_buf->size() - 1];
            bool ifret = true;
            for(size_t i = 0; i < block_insts.size(); i++)
            {
                koopa_raw_value_t tmp = (koopa_raw_value_data*) block_insts[i];
                if(tmp->kind.tag == KOOPA_RVT_RETURN || tmp->kind.tag == KOOPA_RVT_JUMP || tmp->kind.tag == KOOPA_RVT_BRANCH )
                {
                    block_insts.resize(i + 1);//控制不需要多余的指令
                    ifret = false;
                    break;
                }
            }
            if(ifret)
            {
                koopa_raw_value_data *ret = new koopa_raw_value_data();
                ret->ty = make_type_from_kind(KOOPA_RTT_UNIT);
                ret->name = nullptr;
                ret->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
                ret->kind.tag = KOOPA_RVT_RETURN;
                if(cur_func->ty->data.function.ret->tag == KOOPA_RTT_UNIT)
                    ret->kind.data.ret.value = nullptr;
                //先不添加int型函数的返回值，因为int型return不能省略
                block_insts.push_back(ret);
            }
            if (!last_block->insts.buffer)
                last_block->insts = make_slice_from_vector(block_insts, KOOPA_RSIK_VALUE);
        }
        block_insts.clear();
    }       
    void AddBasicBlock(koopa_raw_basic_block_data_t * basic_block)
    {
        FinishBasicBlock();
        basic_block->insts.buffer = nullptr;
        //感觉没有必要，后续测试->类似于初始化的作用
        basic_block_buf->push_back(basic_block);
    }
    void AddInst(const void *inst)
    {
        block_insts.push_back(inst);
    }
};

//维护一个用来记录循环开头与结尾的数据结构
struct WhileStruct {
    koopa_raw_basic_block_data_t *entry = new koopa_raw_basic_block_data_t();
    koopa_raw_basic_block_data_t *body = new koopa_raw_basic_block_data_t();
    koopa_raw_basic_block_data_t *end = new koopa_raw_basic_block_data_t();
};

class LoopList {
    private:
    vector<WhileStruct> looplist;
    public:
    void AddLoop(koopa_raw_basic_block_data_t *e1,koopa_raw_basic_block_data_t *b1,koopa_raw_basic_block_data_t *e2)
    {
        WhileStruct ws;
        ws.entry = e1;
        ws.body = b1;
        ws.end = e2;
        looplist.push_back(ws);
    }
    void PopLoop()
    {
        looplist.pop_back();
    }
    const void * GetLoop(int type)
    {
        if(!looplist.empty())
        {
            if(type == 0) return looplist.back().entry;
            else if(type == 1) return looplist.back().body;
            else return looplist.back().end;
        }
        else return nullptr;
        
    }
};