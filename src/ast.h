#pragma once
//#include <cstddef>
#include<iostream>
#include<memory>
#include<string>
#include<cstring>
#include<type_traits>
#include<vector>
#include<map>
#include"koopa.h"
#include"utils.h"
//using namespace std;
static bool iffirst = true;
//static int valuescope = 0;
static SymbolList symboltable(0);
static BlockMaintainer blocktable;
static LoopList looptable;
static vector<const void*>functable;
static vector<const void*>globalvaluetable;
// 所有 AST 的基类
class BaseAST {
    public:
    virtual ~BaseAST() = default;    
    virtual std::string Dump() const {return "";};
    //virtual koopa_raw_program_t Dumpk() const = 0; /*纯虚函数函数体必须定义在类的外部*/
    virtual const void* to_koopa() const{return nullptr;};
    virtual const void* to_koopa_right() const {return nullptr;};
    virtual const void* to_koopa_left() const {return nullptr;};
    virtual int calculate() const{return -1;};
};

//CompUnit 是 BaseAST
class CompUnitAST : public BaseAST {
    public:
    std::unique_ptr<std::vector<unique_ptr<BaseAST>>> comp_vec;
    std::unique_ptr<BaseAST> func_def;
    const void * to_koopa() const override {
        //在最外层创建一个符号表，作为全局符号表
        symboltable.newscope();
        //std::vector<const void*>funcs;
        koopa_raw_function_data_t *getint = new koopa_raw_function_data_t();
        getint->bbs = make_slice_from_null(KOOPA_RSIK_BASIC_BLOCK);
        getint->name = make_name_from_string("getint");
        koopa_raw_type_kind_t *ty1 = new koopa_raw_type_kind_t();
        ty1->tag = KOOPA_RTT_FUNCTION;
        ty1->data.function.params = make_slice_from_null(KOOPA_RSIK_TYPE);
        ty1->data.function.ret = make_type_from_kind(KOOPA_RTT_INT32);
        getint->ty = ty1;
        getint->params = make_slice_from_null(KOOPA_RSIK_VALUE);
        functable.push_back(getint);
        Value func1;
        func1.type = Func_Type;
        func1.data.func_val = getint;
        symboltable.insert(func1, "getint");

        koopa_raw_function_data_t *getch = new koopa_raw_function_data_t();
        getch->bbs = make_slice_from_null(KOOPA_RSIK_BASIC_BLOCK);
        getch->name = make_name_from_string("getch");
        koopa_raw_type_kind_t *ty2 = new koopa_raw_type_kind_t();
        ty2->tag = KOOPA_RTT_FUNCTION;
        ty2->data.function.params = make_slice_from_null(KOOPA_RSIK_TYPE);
        ty2->data.function.ret = make_type_from_kind(KOOPA_RTT_INT32);
        getch->ty = ty2;
        getch->params = make_slice_from_null(KOOPA_RSIK_VALUE);
        functable.push_back(getch);
        Value func2;
        func2.type = Func_Type;
        func2.data.func_val = getch;
        symboltable.insert(func2, "getch");

        koopa_raw_function_data_t *getarray = new koopa_raw_function_data_t();
        getarray->bbs = make_slice_from_null(KOOPA_RSIK_BASIC_BLOCK);
        getarray->name = make_name_from_string("getarray");
        koopa_raw_type_kind_t *ty3 = new koopa_raw_type_kind_t();
        ty3->tag = KOOPA_RTT_FUNCTION;
        vector<const void *> param3;
        param3.push_back(make_type_from_kind(KOOPA_RTT_INT32));
        ty3->data.function.params = make_slice_from_vector(param3,KOOPA_RSIK_TYPE);
        ty3->data.function.ret = make_type_from_kind(KOOPA_RTT_INT32);
        getarray->ty = ty3;
        param3.clear();
        param3.push_back(make_param_value_from_kind(KOOPA_RTT_INT32));
        getarray->params = make_slice_from_vector(param3,KOOPA_RSIK_VALUE);
        functable.push_back(getarray);
        Value func3;
        func3.type = Func_Type;
        func3.data.func_val = getarray;
        symboltable.insert(func3, "getarray");
        //decl @getarray(*i32): i32

        //decl @putint(i32)
        koopa_raw_function_data_t *putint = new koopa_raw_function_data_t();
        putint->bbs = make_slice_from_null(KOOPA_RSIK_BASIC_BLOCK);
        putint->name = make_name_from_string("putint");
        koopa_raw_type_kind_t *ty4 = new koopa_raw_type_kind_t();
        ty4->tag = KOOPA_RTT_FUNCTION;
        vector<const void *> param4;
        param4.push_back(make_type_from_kind(KOOPA_RTT_INT32));
        ty4->data.function.params = make_slice_from_vector(param4,KOOPA_RSIK_TYPE);
        ty4->data.function.ret = make_type_from_kind(KOOPA_RTT_UNIT);
        putint->ty = ty4;
        param4.clear();
        param4.push_back(make_param_value_from_kind(KOOPA_RTT_INT32));
        putint->params = make_slice_from_vector(param4,KOOPA_RSIK_VALUE);
        functable.push_back(putint);
        Value func4;
        func4.type = Func_Type;
        func4.data.func_val = putint;
        symboltable.insert(func4, "putint");
        //decl @putch(i32)
        koopa_raw_function_data_t *putch = new koopa_raw_function_data_t();
        putch->bbs = make_slice_from_null(KOOPA_RSIK_BASIC_BLOCK);
        putch->name = make_name_from_string("putch");
        koopa_raw_type_kind_t *ty5 = new koopa_raw_type_kind_t();
        ty5->tag = KOOPA_RTT_FUNCTION;
        vector<const void *> param5;
        param5.push_back(make_type_from_kind(KOOPA_RTT_INT32));
        ty5->data.function.params = make_slice_from_vector(param5,KOOPA_RSIK_TYPE);
        ty5->data.function.ret = make_type_from_kind(KOOPA_RTT_UNIT);
        putch->ty = ty5;
        param5.clear();
        param5.push_back(make_param_value_from_kind(KOOPA_RTT_INT32));
        putch->params = make_slice_from_vector(param5,KOOPA_RSIK_VALUE);
        functable.push_back(putch);
        Value func5;
        func5.type = Func_Type;
        func5.data.func_val = putch;
        symboltable.insert(func5, "putch");
        //decl @putarray(i32, *i32)
        koopa_raw_function_data_t *putarray = new koopa_raw_function_data_t();
        putarray->bbs = make_slice_from_null(KOOPA_RSIK_BASIC_BLOCK);
        putarray->name = make_name_from_string("putarray");
        koopa_raw_type_kind_t *ty6 = new koopa_raw_type_kind_t();
        ty6->tag = KOOPA_RTT_FUNCTION;
        vector<const void *> param6;
        param6.push_back(make_type_from_kind(KOOPA_RTT_INT32));
        ty6->data.function.params = make_slice_from_vector(param6,KOOPA_RSIK_TYPE);
        ty6->data.function.ret = make_type_from_kind(KOOPA_RTT_UNIT);
        putarray->ty = ty6;
        param6.clear();
        param6.push_back(make_param_value_from_kind(KOOPA_RTT_INT32));
        param6.push_back(make_param_value_from_kind(KOOPA_RTT_INT32));
        putarray->params = make_slice_from_vector(param6,KOOPA_RSIK_VALUE);
        functable.push_back(putarray);
        Value func6;
        func6.type = Func_Type;
        func6.data.func_val = putarray;
        symboltable.insert(func6, "putarray");
        //decl @starttime()
        koopa_raw_function_data_t *starttime = new koopa_raw_function_data_t();
        starttime->bbs = make_slice_from_null(KOOPA_RSIK_BASIC_BLOCK);
        starttime->name = make_name_from_string("starttime");
        koopa_raw_type_kind_t *ty7 = new koopa_raw_type_kind_t();
        ty7->tag = KOOPA_RTT_FUNCTION;
        ty7->data.function.params = make_slice_from_null(KOOPA_RSIK_TYPE);
        ty7->data.function.ret = make_type_from_kind(KOOPA_RTT_UNIT);
        starttime->ty = ty7;
        starttime->params = make_slice_from_null(KOOPA_RSIK_VALUE);
        functable.push_back(starttime);
        Value func7;
        func7.type = Func_Type;
        func7.data.func_val = starttime;
        symboltable.insert(func7, "starttime");
        //decl @stoptime()
        koopa_raw_function_data_t *stoptime = new koopa_raw_function_data_t();
        stoptime->bbs = make_slice_from_null(KOOPA_RSIK_BASIC_BLOCK);
        stoptime->name = make_name_from_string("stoptime");
        koopa_raw_type_kind_t *ty8 = new koopa_raw_type_kind_t();
        ty8->tag = KOOPA_RTT_FUNCTION;
        ty8->data.function.params = make_slice_from_null(KOOPA_RSIK_TYPE);
        ty8->data.function.ret = make_type_from_kind(KOOPA_RTT_UNIT);
        stoptime->ty = ty8;
        stoptime->params = make_slice_from_null(KOOPA_RSIK_VALUE);
        functable.push_back(stoptime);
        Value func8;
        func8.type = Func_Type;
        func8.data.func_val = stoptime;
        symboltable.insert(func8, "stoptime");
        //正向反向可能需要更改
        for (auto compitem = (*comp_vec).begin();
        compitem != (*comp_vec).end(); compitem++) {  
           (*compitem)->to_koopa();
        }  
        koopa_raw_program_t *ret = new koopa_raw_program_t();
        ret->funcs = make_slice_from_vector(functable,KOOPA_RSIK_FUNCTION);
        ret->values = make_slice_from_vector(globalvaluetable, KOOPA_RSIK_VALUE);
        symboltable.deletescope();
        return ret;
    }
};

//functiondef也是baseast
class FuncDefAST : public BaseAST {
    public:
    int type = -1;
    std::unique_ptr<BaseAST> func_type;
    std::string ident;
    std::unique_ptr<BaseAST> block;
    std::unique_ptr<std::vector<std::unique_ptr<BaseAST>>> funcdef_vec;
    const void* to_koopa() const override {
        koopa_raw_function_data_t *ret_func = new koopa_raw_function_data_t();
        Value func;
        func.type = Func_Type;
        func.data.func_val = ret_func;
        symboltable.insert(func, ident);
        //如果不行记得改到下面
        symboltable.newscope();//创建新的作用域  
        std::vector<const void *> blocks;
        blocktable.SetBasicBlockBuf(&blocks);
        koopa_raw_basic_block_data_t *ret_block = new koopa_raw_basic_block_data_t();
        ret_block->name = make_name_from_string("entry",0);      
        ret_block->params = make_slice_from_null(KOOPA_RSIK_VALUE);
        ret_block->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
        blocktable.AddBasicBlock(ret_block);
        ret_func->name = make_name_from_string(ident);
        //需要做个判断保证不在其中
        blocktable.SetCurrentFunction(ret_func);
        symboltable.isdeclared(ident);
        koopa_raw_type_kind_t *ret_ty = new koopa_raw_type_kind_t();
        ret_ty->tag = KOOPA_RTT_FUNCTION;
        
        if(type == 1)//不能写funcdef_vec.empty
        {
            std::vector<const void *> funcitem;
            for (auto it = funcdef_vec->rbegin(); it !=funcdef_vec->rend() ;it++) {
                funcitem.push_back((*it)->to_koopa_left());
            }          
            ret_ty->data.function.params = make_slice_from_vector(funcitem,KOOPA_RSIK_TYPE);

            funcitem.clear();
            int i = 0;
            for (auto it = funcdef_vec->rbegin(); it !=funcdef_vec->rend() ;it++) {
                i++;
                koopa_raw_value_data *ret = (koopa_raw_value_data *)((*it)->to_koopa_right());
                ret->kind.data.func_arg_ref.index = i; 
                funcitem.push_back(ret);
            }    
            ret_func->params = make_slice_from_vector(funcitem,KOOPA_RSIK_VALUE);
            for(auto it = funcitem.begin();it != funcitem.end();it++)
            {
                koopa_raw_value_data *ret = (koopa_raw_value_data *)(*it);
                Value symbol;
                string nameuse = ret->name;
                nameuse.erase(nameuse.begin());
                symboltable.isdeclared(nameuse);
                string tmp = ret->name + to_string(symboltable.getcurscope());
                koopa_raw_value_data *alloc= new koopa_raw_value_data();
                alloc->ty = make_type_from_int_pointer();      
                alloc->name = make_name_from_string(nameuse,0);
                alloc->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
                alloc->kind.tag = KOOPA_RVT_ALLOC;      
                blocktable.AddInst(alloc);
                symbol.type = Var_Type;//因为在变量定义的ast下，所以类型是固定的       
                symbol.data.var_val = alloc;//把内存给它
                symboltable.insert(symbol, nameuse);
                koopa_raw_value_data *store= new koopa_raw_value_data();
                store->ty = make_type_from_kind(KOOPA_RTT_UNIT);      
                store->name = nullptr;
                store->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
                store->kind.tag = KOOPA_RVT_STORE;   
                store->kind.data.store.dest = alloc;
                store->kind.data.store.value = ret;    
                blocktable.AddInst(store);
            }
        }  
        else
        {
            ret_ty->data.function.params = make_slice_from_null(KOOPA_RSIK_TYPE);
            ret_func->params = make_slice_from_null(KOOPA_RSIK_VALUE);
        }
        ret_ty->data.function.ret = (koopa_raw_type_kind *)func_type->to_koopa();
        ret_func->ty = ret_ty;
        
        block->to_koopa();
        symboltable.deletescope();
        blocktable.FinishBasicBlock();
        ret_func->bbs = make_slice_from_vector(blocks, KOOPA_RSIK_BASIC_BLOCK);
        
        functable.push_back(ret_func);
        return nullptr;
    }
};

class FuncFAST : public BaseAST {
    public:
    std::string ident;
    std::unique_ptr<BaseAST> btype;
    //left只返回类型，right负责返回一堆
    const void* to_koopa_left() const override {
        //目前只有int型
        koopa_raw_type_kind * temp_type = (koopa_raw_type_kind *)btype->to_koopa();
        if(temp_type->tag != KOOPA_RTT_INT32) 
        {
            assert(false);
        }
        return make_type_from_kind(KOOPA_RTT_INT32);
    }  
    const void* to_koopa_right() const override {
        //目前只有int型
        koopa_raw_value_data *ret_value = new koopa_raw_value_data();
        ret_value->ty = make_type_from_kind(KOOPA_RTT_INT32);      
        ret_value->name = make_name_from_string(ident);
        ret_value->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
        ret_value->kind.tag = KOOPA_RVT_FUNC_ARG_REF;       
        return ret_value;
    }
};

class BlockAST : public BaseAST {
    public: 
    int type = -1;
    std::unique_ptr<std::vector<std::unique_ptr<BaseAST>>> blockitem_vec;

    const void* to_koopa() const override {
        if(type == 0)
        {
            for (auto blockitem = (*blockitem_vec).rbegin();
            blockitem != (*blockitem_vec).rend(); blockitem++) {          
                (*blockitem)->to_koopa();
            }     
        }
        return nullptr;
    }
};

class FuncTypeAST : public BaseAST {
    public:
    std::string temp;
    const void* to_koopa() const override {
        koopa_raw_type_kind * ret_type = new koopa_raw_type_kind();
        if(temp == "int") ret_type->tag = KOOPA_RTT_INT32;
        if(temp == "void") ret_type->tag = KOOPA_RTT_UNIT;
        return ret_type;
    }
};

class VarDeclAST : public BaseAST {
    public:
    std::unique_ptr<BaseAST> btype;
    std::unique_ptr<std::vector<std::unique_ptr<BaseAST>>> vardef_vec;
    const void* to_koopa() const override {
        for (auto vardef = (*vardef_vec).rbegin();
        vardef != (*vardef_vec).rend(); vardef++) {   
            (*vardef)->to_koopa();         
        }   
        return nullptr;
    }
};

class VarDefAST : public BaseAST {
    public:
    int type = -1;
    std::string ident;
    std::unique_ptr<BaseAST> ival;
    const void* to_koopa() const override {        
        Value symbol;
        symboltable.isdeclared(ident);
        string tmp = ident + to_string(symboltable.getcurscope());
        koopa_raw_value_data *ret_value = new koopa_raw_value_data();
        //类型出错，耗时5h+，警钟长鸣
        if(symboltable.getcurscope() == 1)//全局变量
        {
            ret_value->ty = make_type_from_int_pointer();      
            ret_value->name = make_name_from_string(tmp);
            ret_value->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            ret_value->kind.tag = KOOPA_RVT_GLOBAL_ALLOC;
            if(type != 1)
                ret_value->kind.data.global_alloc.init = make_value_from_init();
            else
                ret_value->kind.data.global_alloc.init = (koopa_raw_value_data *)ival->to_koopa();
            symbol.type = Var_Type;      
            symbol.data.var_val = ret_value;
            symboltable.insert(symbol, ident);
            globalvaluetable.push_back(ret_value);
            return nullptr;
        }
        ret_value->ty = make_type_from_int_pointer();      
        ret_value->name = make_name_from_string(tmp);
        ret_value->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
        ret_value->kind.tag = KOOPA_RVT_ALLOC;       
        blocktable.AddInst(ret_value);
        symbol.type = Var_Type;//因为在变量定义的ast下，所以类型是固定的       
        symbol.data.var_val = ret_value;//把内存给它
        symboltable.insert(symbol, ident);
        //将数值存入变量
        if(type == 1)
        {
            koopa_raw_value_data *store = new koopa_raw_value_data();
            store->ty = make_type_from_kind(KOOPA_RTT_UNIT);
            store->name = ret_value->name;
            store->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            store->kind.tag = KOOPA_RVT_STORE;
            store->kind.data.store.dest = ret_value;
            store->kind.data.store.value = (koopa_raw_value_data *)ival->to_koopa();
            blocktable.AddInst(store);
        }      
        return nullptr;
    } 
};

class InitValAST : public BaseAST {
    public:
    std::unique_ptr<BaseAST> exp;
    const void* to_koopa() const override {
        return exp->to_koopa();
    }
};

class ConstDeclAST : public BaseAST {
    public:
    std::unique_ptr<BaseAST> btype;
    std::unique_ptr<std::vector<std::unique_ptr<BaseAST>>> constdef_vec;
    const void* to_koopa() const override {
        for (auto constdef = (*constdef_vec).rbegin();
        constdef != (*constdef_vec).rend(); constdef++) {   
            (*constdef)->to_koopa();         
        }   
        return nullptr;
    }
};

class BTypeAST : public BaseAST {
    public:
    std::string temp;
    const void* to_koopa() const override {
        koopa_raw_type_kind * ret_type = new koopa_raw_type_kind();
        ret_type->tag = KOOPA_RTT_INT32;
        return ret_type;
    }
};

class ConstDefAST : public BaseAST {
    public:
    std::string ident;
    std::unique_ptr<BaseAST> cival;
    const void* to_koopa() const override {
        Value symbol;
        symboltable.isdeclared(ident);
        symbol.type = Const_Type;   
        symbol.data.const_val = cival->calculate();
        symboltable.insert(symbol, ident);
        return nullptr;
    } 
};

class ConstInitValAST : public BaseAST {
    public:
    std::unique_ptr<BaseAST> conexp;
    int calculate() const override {
        return conexp->calculate();
    }
};

class ConstExpAST : public BaseAST {
    public:
    std::unique_ptr<BaseAST> exp;
    int calculate() const override {
        return exp->calculate();
    }
};

class LValAST : public BaseAST {
    public:
    std::string ident;
    const void* to_koopa_left() const override {
        Value temp = symboltable.getval(ident);
        return temp.data.var_val;       
    }
    const void* to_koopa_right() const override {
        Value temp = symboltable.getval(ident);
        if(temp.type == ERROR_TYPE) assert(false); //未定义就使用问题
        
        if(temp.type == Const_Type) {
            koopa_raw_value_data *ret = new koopa_raw_value_data();
            ret->ty = make_type_from_kind(KOOPA_RTT_INT32);
            ret->name = nullptr;
            ret->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            ret->kind.tag = KOOPA_RVT_INTEGER;
            ret->kind.data.integer.value = temp.data.const_val;
            return ret;
        }
        if(temp.type == Var_Type) { //必定是指针类型
            if(temp.data.var_val->ty->data.pointer.base->tag == KOOPA_RTT_INT32) //int型指针
            {
                koopa_raw_value_data *load0 = new koopa_raw_value_data();
                load0->ty = make_type_from_kind(KOOPA_RTT_INT32);
                load0->name = nullptr;
                load0->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
                load0->kind.tag = KOOPA_RVT_LOAD;
                load0->kind.data.load.src = temp.data.var_val;
                blocktable.AddInst(load0);
                return load0;         
            }
        }
        return nullptr;       
    }
    int calculate() const override {
        Value temp = symboltable.getval(ident);
        if(temp.type == ERROR_TYPE) assert(false); //重复定义问题
        return temp.data.const_val;
    }
};

class StmtAST : public BaseAST {
    public:
    int type = -1;
    std::unique_ptr<BaseAST> lval;
    std::unique_ptr<BaseAST> exp;
    std::unique_ptr<BaseAST> stmt;
    const void* to_koopa() const override {
        if(type == 0) //return exp ;
        {
            koopa_raw_value_data *ret_value = new koopa_raw_value_data();
            ret_value->ty = make_type_from_kind(KOOPA_RTT_UNIT);
            ret_value->name = nullptr;
            ret_value->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            ret_value->kind.tag = KOOPA_RVT_RETURN;       
            ret_value->kind.data.ret.value = (koopa_raw_value_data *)exp->to_koopa();
            blocktable.AddInst(ret_value);
        }
        if(type == 1)// lval = exp;
        {
            koopa_raw_value_data *ret = new koopa_raw_value_data();
            ret = (koopa_raw_value_data *)lval->to_koopa_left();
            koopa_raw_value_data *ret3 = new koopa_raw_value_data();
            ret3->ty = make_type_from_kind(KOOPA_RTT_UNIT);
            ret3->name = nullptr;
            ret3->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            ret3->kind.tag = KOOPA_RVT_STORE;
            ret3->kind.data.store.dest = ret;
            ret3->kind.data.store.value = (koopa_raw_value_data *)exp->to_koopa();
            blocktable.AddInst(ret3);
        }
        if(type == 2)// block;
        {
            symboltable.newscope();//创建新的作用域
            exp->to_koopa();
            symboltable.deletescope();
        }
        if(type == 3)// exp;
        {
            exp->to_koopa();
        }
        if(type == 4)// ;
        {
            ;
        }
        if(type == 5)// return;
        {
            koopa_raw_value_data *ret_value = new koopa_raw_value_data();
            ret_value->ty = make_type_from_kind(KOOPA_RTT_UNIT);
            ret_value->name = nullptr;
            ret_value->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            ret_value->kind.tag = KOOPA_RVT_RETURN;       
            ret_value->kind.data.ret.value = nullptr;
            blocktable.AddInst(ret_value);           
        }
        if(type == 7) //->if 
        {
            //为什么不需要创建新作用域的原因：
            //1.到block则上文中已经实现了，到exp则事实上没有产生新作用域(exp不能产生定义)
            //如果后续有函数调用等，可能产生更改
            //if用branch ，结束后的语句块用jump
            //我倾向于认为整个ret_br都没有问题，但为什么会发生错误，我仍不得而知
            koopa_raw_basic_block_data_t *ret_block_t = new koopa_raw_basic_block_data_t();
            koopa_raw_basic_block_data_t *ret_block = new koopa_raw_basic_block_data_t();
            blocktable.AddInst(make_branch_from_block(lval->to_koopa(),ret_block_t,ret_block));

            ret_block_t->name = make_name_from_string("then",0);          
            ret_block_t->params = make_slice_from_null(KOOPA_RSIK_VALUE);
            ret_block_t->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            blocktable.AddBasicBlock(ret_block_t);
            exp->to_koopa();
            blocktable.AddInst(make_jump_from_block(ret_block));
           
            ret_block->name = make_name_from_string("end",0);                  
            ret_block->params = make_slice_from_null(KOOPA_RSIK_VALUE);
            ret_block->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            blocktable.AddBasicBlock(ret_block);
        }
        if(type == 6) // ->ifelse
        {        
            koopa_raw_basic_block_data_t *ret_block_t = new koopa_raw_basic_block_data_t();
            koopa_raw_basic_block_data_t *ret_block_f = new koopa_raw_basic_block_data_t();
            koopa_raw_basic_block_data_t *ret_block = new koopa_raw_basic_block_data_t();

            blocktable.AddInst(make_branch_from_block(lval->to_koopa(),ret_block_t,ret_block_f));

            ret_block_t->name = make_name_from_string("then",0);          
            ret_block_t->params = make_slice_from_null(KOOPA_RSIK_VALUE);
            ret_block_t->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            blocktable.AddBasicBlock(ret_block_t);
            exp->to_koopa();
            blocktable.AddInst(make_jump_from_block(ret_block));
            
            ret_block_f->name = make_name_from_string("else",0);          
            ret_block_f->params = make_slice_from_null(KOOPA_RSIK_VALUE);
            ret_block_f->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            blocktable.AddBasicBlock(ret_block_f);
            stmt->to_koopa();
            blocktable.AddInst(make_jump_from_block(ret_block));

            ret_block->name = make_name_from_string("end",0);                  
            ret_block->params = make_slice_from_null(KOOPA_RSIK_VALUE);
            ret_block->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            blocktable.AddBasicBlock(ret_block);       
        }
        if(type == 8)//while
        {
            koopa_raw_basic_block_data_t *while_entry = new koopa_raw_basic_block_data_t();
            koopa_raw_basic_block_data_t *while_body = new koopa_raw_basic_block_data_t();
            koopa_raw_basic_block_data_t *end = new koopa_raw_basic_block_data_t();
            while_entry->name = make_name_from_string("while_entry",0);          
            while_entry->params = make_slice_from_null(KOOPA_RSIK_VALUE);
            while_entry->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);          
            while_body->name = make_name_from_string("while_body",0);          
            while_body->params = make_slice_from_null(KOOPA_RSIK_VALUE);
            while_body->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            end->name = make_name_from_string("end",0);                  
            end->params = make_slice_from_null(KOOPA_RSIK_VALUE);
            end->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            looptable.AddLoop(while_entry, while_body, end);
            blocktable.AddInst(make_jump_from_block(while_entry));
            blocktable.AddBasicBlock(while_entry);
            blocktable.AddInst(make_branch_from_block(exp->to_koopa(), while_body, end));
            blocktable.AddBasicBlock(while_body);
            stmt->to_koopa();
            blocktable.AddInst(make_jump_from_block(while_entry));
            blocktable.AddBasicBlock(end);
            looptable.PopLoop();
        }
        if(type == 9)//break
        {
            //就是jump到end
            if(looptable.GetLoop(2)==nullptr) //处理语义错误，但还有另一种方法
            {
                cout<< "break is not in while stmt!"<<endl;
                assert(false);
            }
            koopa_raw_basic_block_data_t *end = (koopa_raw_basic_block_data_t *)looptable.GetLoop(2);
            blocktable.AddInst(make_jump_from_block(end));
        }
        if(type == 10)//continue
        {
            if(looptable.GetLoop(2)==nullptr) //处理语义错误，但还有另一种方法
            {
                cout<< "break is not in while stmt!"<<endl;
                assert(false);
            }
            koopa_raw_basic_block_data_t *entry = (koopa_raw_basic_block_data_t *)looptable.GetLoop(0);
            blocktable.AddInst(make_jump_from_block(entry));
        }
        return nullptr;
    }
};

class ExpAST : public BaseAST {
    public:
    std::unique_ptr<BaseAST> lor;
    
    std::string Dump() const override {
        return lor->Dump();
    }
    const void * to_koopa() const override {
        return lor->to_koopa();
    }
    int calculate() const override {
        return lor->calculate();
    }
};

class LOrExpAST : public BaseAST {
    public:
    int type = -1;
    std::string op;
    std::unique_ptr<BaseAST> land;
    std::unique_ptr<BaseAST> lor;
    const void * to_koopa() const override {
        if(type == 0) return land->to_koopa();
        else {
            //实现逻辑或 将a||b 转化为 !a==0|!b==0
            //短路求值，继续转化为 int result = 1;
            //if(lhs == 0) result = (rhs != 0)
            //不需要新建作用域，记得
            koopa_raw_value_data *res = new koopa_raw_value_data();
            koopa_raw_value_data *store1 = new koopa_raw_value_data();
            koopa_raw_value_data *store2= new koopa_raw_value_data();
            koopa_raw_value_data *lhs1 = new koopa_raw_value_data();
            koopa_raw_value_data *rhs1 = new koopa_raw_value_data();          
            koopa_raw_value_data *load0 = new koopa_raw_value_data(); 
            res->ty = make_type_from_int_pointer();      
            res->name = make_name_from_string("result_1_no");
            //加入一点奇怪的符号防止符号表出错
            res->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            res->kind.tag = KOOPA_RVT_ALLOC;       
            blocktable.AddInst(res);        

            store1->ty = make_type_from_kind(KOOPA_RTT_UNIT);
            store1->name = nullptr;
            store1->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            store1->kind.tag = KOOPA_RVT_STORE;
            store1->kind.data.store.dest = res;
            store1->kind.data.store.value = make_value_from_int(1);
            blocktable.AddInst(store1);
            //上面实现 int result = 1;
            lhs1->ty = make_type_from_kind(KOOPA_RTT_INT32);
            lhs1->name = nullptr;
            lhs1->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            lhs1->kind.tag = KOOPA_RVT_BINARY;
            lhs1->kind.data.binary.op = KOOPA_RBO_EQ;
            lhs1->kind.data.binary.lhs = (koopa_raw_value_data *)lor->to_koopa();
            lhs1->kind.data.binary.rhs = make_value_from_int(0);
            blocktable.AddInst(lhs1);
            //上面实现lhs == 0
            koopa_raw_basic_block_data_t *rtrue = new koopa_raw_basic_block_data_t();
            koopa_raw_basic_block_data_t *rfalse = new koopa_raw_basic_block_data_t();
            blocktable.AddInst(make_branch_from_block(lhs1,rtrue,rfalse));
            //
            rtrue->name = make_name_from_string("then",0);          
            rtrue->params = make_slice_from_null(KOOPA_RSIK_VALUE);
            rtrue->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            blocktable.AddBasicBlock(rtrue);

            rhs1->ty = make_type_from_kind(KOOPA_RTT_INT32);
            rhs1->name = nullptr;
            rhs1->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            rhs1->kind.tag = KOOPA_RVT_BINARY;
            rhs1->kind.data.binary.op = KOOPA_RBO_NOT_EQ;
            rhs1->kind.data.binary.lhs = (koopa_raw_value_data *)land->to_koopa();
            rhs1->kind.data.binary.rhs = make_value_from_int(0);
            blocktable.AddInst(rhs1);        
            //上面是rhs != 0
            //下面是result = ...
            store2->ty = make_type_from_kind(KOOPA_RTT_UNIT);
            store2->name = nullptr;
            store2->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            store2->kind.tag = KOOPA_RVT_STORE;
            store2->kind.data.store.dest = res;
            store2->kind.data.store.value = rhs1;
            blocktable.AddInst(store2);
            blocktable.AddInst(make_jump_from_block(rfalse));
           
            rfalse->name = make_name_from_string("end",0);                  
            rfalse->params = make_slice_from_null(KOOPA_RSIK_VALUE);
            rfalse->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            blocktable.AddBasicBlock(rfalse);    
            //下面是return result
            load0->ty = make_type_from_kind(KOOPA_RTT_INT32);
            load0->name = nullptr;
            load0->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            load0->kind.tag = KOOPA_RVT_LOAD;
            load0->kind.data.load.src = res;
            blocktable.AddInst(load0);
            return load0;
        }
    } 

    int calculate() const override {
        if(type == 0) return land->calculate();
        else
        {
            return lor->calculate()||land->calculate();
        };
    } 
};

class LAndExpAST : public BaseAST {
    public:
    int type = -1;
    std::string op;
    std::unique_ptr<BaseAST> eq;
    std::unique_ptr<BaseAST> land;
    const void * to_koopa() const override {
        if(type == 0) return eq->to_koopa();
        else {
            //实现逻辑与 将a&&b 转化为 
            //result = 0
            // if (a == 1) result = ...
            koopa_raw_value_data *res = new koopa_raw_value_data();
            koopa_raw_value_data *store1 = new koopa_raw_value_data();
            koopa_raw_value_data *store2= new koopa_raw_value_data();
            koopa_raw_value_data *lhs1 = new koopa_raw_value_data(); 
            koopa_raw_value_data *rhs1 = new koopa_raw_value_data();       
            koopa_raw_value_data *load0 = new koopa_raw_value_data(); 
            res->ty = make_type_from_int_pointer();      
            res->name = make_name_from_string("result_0_no");
            //加入一点奇怪的符号防止符号表出错
            res->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            res->kind.tag = KOOPA_RVT_ALLOC;       
            blocktable.AddInst(res);        

            store1->ty = make_type_from_kind(KOOPA_RTT_UNIT);
            store1->name = nullptr;
            store1->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            store1->kind.tag = KOOPA_RVT_STORE;
            store1->kind.data.store.dest = res;
            store1->kind.data.store.value = make_value_from_int(0);
            blocktable.AddInst(store1);
            //上面实现 int result = 0;
            lhs1->ty = make_type_from_kind(KOOPA_RTT_INT32);
            lhs1->name = nullptr;
            lhs1->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            lhs1->kind.tag = KOOPA_RVT_BINARY;
            lhs1->kind.data.binary.op = KOOPA_RBO_EQ;
            lhs1->kind.data.binary.lhs = (koopa_raw_value_data *)land->to_koopa();
            lhs1->kind.data.binary.rhs = make_value_from_int(1);//a == 1
            blocktable.AddInst(lhs1);
            //上面实现lhs == 1
            koopa_raw_basic_block_data_t *rtrue = new koopa_raw_basic_block_data_t();
            koopa_raw_basic_block_data_t *rfalse = new koopa_raw_basic_block_data_t();
            blocktable.AddInst(make_branch_from_block(lhs1,rtrue,rfalse));
            rtrue->name = make_name_from_string("then",0);          
            rtrue->params = make_slice_from_null(KOOPA_RSIK_VALUE);
            rtrue->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            blocktable.AddBasicBlock(rtrue);   
            rhs1->ty = make_type_from_kind(KOOPA_RTT_INT32);
            rhs1->name = nullptr;
            rhs1->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            rhs1->kind.tag = KOOPA_RVT_BINARY;
            rhs1->kind.data.binary.op = KOOPA_RBO_NOT_EQ;
            rhs1->kind.data.binary.lhs = (koopa_raw_value_data *)land->to_koopa();
            rhs1->kind.data.binary.rhs = make_value_from_int(0);
            blocktable.AddInst(rhs1); 
            //上面实现rhs != 0  
            //下面是result = rhs
            store2->ty = make_type_from_kind(KOOPA_RTT_UNIT);
            store2->name = nullptr;
            store2->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            store2->kind.tag = KOOPA_RVT_STORE;
            store2->kind.data.store.dest = res;
            store2->kind.data.store.value = rhs1;
            blocktable.AddInst(store2);
            blocktable.AddInst(make_jump_from_block(rfalse));  
            rfalse->name = make_name_from_string("end",0);                  
            rfalse->params = make_slice_from_null(KOOPA_RSIK_VALUE);
            rfalse->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            blocktable.AddBasicBlock(rfalse);    
            //下面是return result
            load0->ty = make_type_from_kind(KOOPA_RTT_INT32);
            load0->name = nullptr;
            load0->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            load0->kind.tag = KOOPA_RVT_LOAD;
            load0->kind.data.load.src = res;
            blocktable.AddInst(load0);
            return load0;
        }
    }  
    int calculate() const override {
        if(type == 0) return eq->calculate();
        else
        {
            return land->calculate()&&eq->calculate();
        };
    }  
};

class EqExpAST : public BaseAST {
    public:
    int type = -1;
    std::string op;
    std::unique_ptr<BaseAST> eq;
    std::unique_ptr<BaseAST> rel;
    const void * to_koopa() const override {
        if(type == 0) return rel->to_koopa();
        else {
            koopa_raw_value_data *rev = new koopa_raw_value_data();
            rev->ty = make_type_from_kind(KOOPA_RTT_INT32);
            rev->name = nullptr;
            rev->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            rev->kind.tag = KOOPA_RVT_BINARY;     
            if(op == "==")  rev->kind.data.binary.op = KOOPA_RBO_EQ;
            if(op == "!=")  rev->kind.data.binary.op = KOOPA_RBO_NOT_EQ;
            rev->kind.data.binary.lhs = (koopa_raw_value_data *)eq->to_koopa();
            rev->kind.data.binary.rhs = (koopa_raw_value_data *)rel->to_koopa();       
            blocktable.AddInst(rev);
            return rev;
        }
    }  
    int calculate() const override {
        if(type == 0) return rel->calculate();
        else
        {
            if(op == "==") return (rel->calculate()==eq->calculate());
            else return (rel->calculate()!=eq->calculate());
        };
    }  
};

class RelExpAST : public BaseAST {
    public:
    int type = -1;
    std::string op;
    std::unique_ptr<BaseAST> add;
    std::unique_ptr<BaseAST> rel;
    const void * to_koopa() const override {
        if(type==0) return add->to_koopa();
        else {
            koopa_raw_value_data *rev = new koopa_raw_value_data();
            rev->ty = make_type_from_kind(KOOPA_RTT_INT32);
            rev->name = nullptr;
            rev->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            rev->kind.tag = KOOPA_RVT_BINARY;     
            if(op == ">")  rev->kind.data.binary.op = KOOPA_RBO_GT;
            if(op == "<")  rev->kind.data.binary.op = KOOPA_RBO_LT;
            if(op == ">=")  rev->kind.data.binary.op = KOOPA_RBO_GE;
            if(op == "<=")  rev->kind.data.binary.op = KOOPA_RBO_LE;
            rev->kind.data.binary.lhs = (koopa_raw_value_data *)rel->to_koopa();
            rev->kind.data.binary.rhs = (koopa_raw_value_data *)add->to_koopa();       
            blocktable.AddInst(rev);
            return rev;
        }
    }  
    int calculate() const override {
        if(type == 0) return add->calculate();
        else
        {
            if(op == ">") return (rel->calculate()>add->calculate());
            if(op == ">=") return (rel->calculate()>=add->calculate());
            if(op == "<") return (rel->calculate()<add->calculate());
            if(op == "<=") return (rel->calculate()<=add->calculate());
        };
        return 0;
    }   
};

class AddExpAST : public BaseAST {
    public:
    int type = -1;
    std::string op;
    std::unique_ptr<BaseAST> addexp;
    std::unique_ptr<BaseAST> mulexp;
    const void * to_koopa() const override {
        if(type==0) return mulexp->to_koopa();
        else {
            koopa_raw_value_data *rev = new koopa_raw_value_data();
            rev->ty = make_type_from_kind(KOOPA_RTT_INT32);
            rev->name = nullptr;
            rev->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            rev->kind.tag = KOOPA_RVT_BINARY;     
            if(op == "+")  rev->kind.data.binary.op = KOOPA_RBO_ADD;
            if(op == "-")  rev->kind.data.binary.op = KOOPA_RBO_SUB;
            rev->kind.data.binary.lhs = (koopa_raw_value_data *)addexp->to_koopa();
            rev->kind.data.binary.rhs = (koopa_raw_value_data *)mulexp->to_koopa();       
            blocktable.AddInst(rev);
            return rev;
        }
    }   
    int calculate() const override {
        if(type == 0) return mulexp->calculate();
        else
        {
            if(op == "+") 
            {
                // int a=addexp->calculate();
                // int b=mulexp->calculate();
                
                // cout<<a<<" "<<b<<endl;
                return (addexp->calculate()+mulexp->calculate());
                //return a;
            }
            if(op == "-") return (addexp->calculate()-mulexp->calculate());
        };
        return 0;
    }  
};

class MulExpAST : public BaseAST {
    public:
    int type = -1;
    std::string op;
    std::unique_ptr<BaseAST> unaryexp;
    std::unique_ptr<BaseAST> mulexp;
    const void * to_koopa() const override {
        if(type==0) return unaryexp->to_koopa();
        else {
            koopa_raw_value_data *rev = new koopa_raw_value_data();
            rev->ty = make_type_from_kind(KOOPA_RTT_INT32);
            rev->name = nullptr;
            rev->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            rev->kind.tag = KOOPA_RVT_BINARY;     
            if(op == "*")  rev->kind.data.binary.op = KOOPA_RBO_MUL;
            if(op == "/")  rev->kind.data.binary.op = KOOPA_RBO_DIV;
            if(op == "%")  rev->kind.data.binary.op = KOOPA_RBO_MOD;
            rev->kind.data.binary.lhs = (koopa_raw_value_data *)mulexp->to_koopa();
            rev->kind.data.binary.rhs = (koopa_raw_value_data *)unaryexp->to_koopa();       
            blocktable.AddInst(rev);
            return rev;
        }
    }   
    int calculate() const override {
        if(type == 0) return unaryexp->calculate();
        else
        {
            if(op == "*") return (mulexp->calculate()*unaryexp->calculate());
            else {
                int divisor=mulexp->calculate(),dividend = unaryexp->calculate();
                if(dividend == 0) assert(false);//被除数为0，报错
                else {
                    if(op == "/") return divisor/dividend;
                    else return divisor%dividend;
                }
            }
            
            //op == "%"
        };
        return -100;
    }  
};

class UnaryExpAST : public BaseAST {
    public:
    int type = -1;
    std::string opexp="";
    std::unique_ptr<BaseAST> pexp;
    std::unique_ptr<std::vector<std::unique_ptr<BaseAST>>> unary_vec;
    const void * to_koopa() const override {
        if(type == 1)
        {
            return pexp->to_koopa();
        }
        if(type == 0) 
        {
            if(opexp == "+")
            {
                return pexp->to_koopa();
            }
            else {
                koopa_raw_value_data *rev = new koopa_raw_value_data();
                rev->ty = make_type_from_kind(KOOPA_RTT_INT32);
                rev->name = nullptr;
                rev->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
                rev->kind.tag = KOOPA_RVT_BINARY;       
                if(opexp == "-")
                {
                    rev->kind.data.binary.op = KOOPA_RBO_SUB;
                    rev->kind.data.binary.lhs = make_value_from_int(0);
                    rev->kind.data.binary.rhs = (koopa_raw_value_data *)pexp->to_koopa();
                }
                else {
                    rev->kind.data.binary.op = KOOPA_RBO_EQ;
                    rev->kind.data.binary.lhs = (koopa_raw_value_data *)pexp->to_koopa();
                    rev->kind.data.binary.rhs = make_value_from_int(0);
                }         
                blocktable.AddInst(rev);
                return rev;
            }          
        }
        if(type == 2)//调用无参函数
        {
            //首先检查ident所代表函数的类型
            //不用处理函数名被内部覆盖的情况 sysy规则不允许，就算要允许，也可以选择特制函数来处理
            Value temp = symboltable.getval(opexp);
            if(temp.type != Func_Type)
            {
                assert(false);
            }
            koopa_raw_value_data * call = new koopa_raw_value_data();
            if(temp.data.func_val->ty->data.function.ret->tag == KOOPA_RTT_INT32)
                call->ty = make_type_from_kind(KOOPA_RTT_INT32);
            else 
                call->ty = make_type_from_kind(KOOPA_RTT_UNIT);
            call->name = nullptr;
            call->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            call->kind.tag = KOOPA_RVT_CALL;
            call->kind.data.call.callee = temp.data.func_val;
            call->kind.data.call.args = make_slice_from_null(KOOPA_RSIK_VALUE);
            blocktable.AddInst(call);
            if(temp.data.func_val->ty->data.function.ret->tag == KOOPA_RTT_INT32) return call;
            //temp.data.func_val->name
        }
        if(type == 3)//调用有参的
        {
            Value temp = symboltable.getval(opexp);
            if(temp.type != Func_Type)
            {
                assert(false);
            }
            koopa_raw_value_data * call = new koopa_raw_value_data();
            if(temp.data.func_val->ty->data.function.ret->tag == KOOPA_RTT_INT32)
                call->ty = make_type_from_kind(KOOPA_RTT_INT32);
            else 
                call->ty = make_type_from_kind(KOOPA_RTT_UNIT);
            call->name = nullptr;
            call->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
            call->kind.tag = KOOPA_RVT_CALL;
            call->kind.data.call.callee = temp.data.func_val;
            vector<const void*> argss;
            for(auto it = unary_vec->rbegin();it != unary_vec->rend();it++)
            {
                argss.push_back((*it)->to_koopa());
            }
            call->kind.data.call.args = make_slice_from_vector(argss, KOOPA_RSIK_VALUE);
            blocktable.AddInst(call);
            if(temp.data.func_val->ty->data.function.ret->tag == KOOPA_RTT_INT32) return call;
        }
        return nullptr;
    }
    int calculate() const override {
        if(type == 0) return pexp->calculate();
        else
        {          
            if(opexp == "-") return -pexp->calculate();
            else if(opexp == "!") return !pexp->calculate();
            else return pexp->calculate();//(opexp == "+")
        };
    }  
};

class PrimaryExpAST : public BaseAST {
    public:
    int type = -1;
    std::unique_ptr<BaseAST> num;

    std::string Dump() const override {
        return num->Dump();
    }

    const void * to_koopa() const override {
        if(type == 1) return num->to_koopa_right();
        else return num->to_koopa(); // ->( exp )的情况
    }

    int calculate() const override {
        return num->calculate();
    }  

};

class NumberAST : public BaseAST {
    public:
    int number;
    std::string Dump() const override {
        return std::to_string(number);
    }
    const void * to_koopa() const override {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        ret->ty = make_type_from_kind(KOOPA_RTT_INT32);
        ret->name = nullptr;
        ret->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
        ret->kind.tag = KOOPA_RVT_INTEGER;
        ret->kind.data.integer.value = number;
        return ret;
    }
    int calculate() const override {
        return number;
    }  
};


