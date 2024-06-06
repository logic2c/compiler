
#define _CRT_SECURE_NO_WARNINGS
#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include<cstring>
#include "ast.h"
#include "koopa.h"
#include "risc.h"
using namespace std;

// 声明 lexer 的输入, 以及 parser 函数
// 为什么不引用 sysy.tab.hpp 呢? 因为首先里面没有 yyin 的定义
// 其次, 因为这个文件不是我们自己写的, 而是被 Bison 生成出来的
// 你的代码编辑器/IDE 很可能找不到这个文件, 然后会给你报错 (虽然编译不会出错)
extern FILE *yyin;
extern int yyparse(unique_ptr<BaseAST> &ast);
int main(int argc, const char *argv[]) 
{
    // 解析命令行参数. 测试脚本/评测平台要求你的编译器能接收如下参数:
    // compiler 模式 输入文件 -o 输出文件
    assert(argc == 5);
    auto mode = argv[1];
    auto input = argv[2];
    auto output = argv[4];

    // 打开输入文件, 并且指定 lexer 在解析的时候读取这个文件
    yyin = fopen(input, "r");
    assert(yyin);

    // 调用 parser 函数, parser 函数会进一步调用 lexer 解析输入文件的
    unique_ptr<BaseAST> ast;
    auto ret = yyparse(ast);
    assert(!ret);
    koopa_raw_program_t raw_program = *(koopa_raw_program_t *)ast->to_koopa();
    //assert(false);
    if(strcmp(mode,"-koopa")==0)
    {
      koopa_program_t program;//已经转化完成的ir程序
      koopa_error_code_t error_no = koopa_generate_raw_to_koopa(&raw_program, &program);
      
      if(error_no != KOOPA_EC_SUCCESS) {
        cout<< "Generate raw to koopa error! error number :"<<error_no<<endl;
        return 0;
      }
      koopa_dump_to_file(program, output);
      koopa_delete_program(program);
    }
    if(strcmp(mode,"-riscv")==0)
    {
      koopa_program_t kp;
      koopa_error_code_t eno = koopa_generate_raw_to_koopa(&raw_program, &kp);
      char *buffer = new char[1000000];
      size_t sz = 1000000u;
      eno = koopa_dump_to_string(kp, buffer, &sz);
      if (eno != KOOPA_EC_SUCCESS)
      {
          std::cout << "koopa dump to string error: " << (int)eno << std::endl;
          return 0;
      }
      koopa_delete_program(kp);

      koopa_program_t new_kp;
      eno = koopa_parse_from_string(buffer, &new_kp);
      if (eno != KOOPA_EC_SUCCESS)
      {
          std::cout << "generate raw to koopa error: " << (int)eno << std::endl;
          return 0;
      }
      koopa_raw_program_builder_t kp_builder = koopa_new_raw_program_builder();
      koopa_raw_program_t new_krp = koopa_build_raw_program(kp_builder, new_kp);
      koopa_delete_program(new_kp);
      freopen(output,"w",stdout);
      Visit(new_krp);
      fclose(stdout);
    }
    // if(strcmp(mode,"-koopa")==0) 
    // {
    //   string str=ast->Dump();
    //   const char*p = str.data();
    //   cout<<str<<endl;
    //   // 解析字符串 str, 得到 Koopa IR 程序
    //   koopa_program_t program;
    //   koopa_error_code_t error_no = koopa_parse_from_string(p, &program);
    //   assert(ret == KOOPA_EC_SUCCESS);  // 确保解析时没有出错
    //   // 创建一个 raw program builder, 用来构建 raw program
    //   koopa_raw_program_builder_t builder = koopa_new_raw_program_builder();
    //   // 将 Koopa IR 程序转换为 raw program
    //   koopa_raw_program_t raw = koopa_build_raw_program(builder, program);
    //   koopa_dump_to_file(program, output);
    //   // 释放 Koopa IR 程序占用的内存
    //   koopa_delete_program(program);
    //   // 处理 raw program
    //   // ...
    //   for (size_t i = 0; i < raw.funcs.len; ++i) 
    //   {
    //     // 正常情况下, 列表中的元素就是函数, 我们只不过是在确认这个事实
    //     // 当然, 你也可以基于 raw slice 的 kind, 实现一个通用的处理函数
    //     assert(raw.funcs.kind == KOOPA_RSIK_FUNCTION);
    //     // 获取当前函数
    //     koopa_raw_function_t func = (koopa_raw_function_t) raw.funcs.buffer[i];
    //     // 进一步处理当前函数
    //     // ...
    //   }
    //   // 处理完成, 释放 raw program builder 占用的内存
    //   // 注意, raw program 中所有的指针指向的内存均为 raw program builder 的内存
    //   // 所以不要在 raw program 处理完毕之前释放 builder
    //   koopa_delete_raw_program_builder(builder);
    // }
    return 0;
}
