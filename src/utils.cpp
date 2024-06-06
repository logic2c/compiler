#include"utils.h"
#include <vector>

const char * make_name_from_string(string name,int tag)
{
    char *ret = new char[name.length() + 1];
    if(tag == 1) ("@" + name).copy(ret, name.length() + 1);
    else ("%" + name).copy(ret, name.length() + 1);
    ret[name.length() + 1] = '\0';
    return ret;
}

koopa_raw_slice_t make_slice_from_vector(std::vector<const void*> vec,koopa_raw_slice_item_kind_t kind1)
{
    koopa_raw_slice_t ret_slice;
    ret_slice.kind = kind1;
    ret_slice.buffer = new const void *[vec.size()];
    std::copy(vec.begin(),vec.end(),ret_slice.buffer);
    ret_slice.len = vec.size();
    return ret_slice;
}

koopa_raw_slice_t make_slice_from_null(koopa_raw_slice_item_kind_t kind1)
{
    koopa_raw_slice_t ret_slice;
    ret_slice.kind = kind1;
    ret_slice.buffer = nullptr;
    ret_slice.len = 0;
    return ret_slice;
}

koopa_raw_type_kind *make_type_from_kind(koopa_raw_type_tag_t kind)
{
    koopa_raw_type_kind *ret = new koopa_raw_type_kind();
    ret->tag = kind;
    return ret;
}

koopa_raw_type_kind *make_type_from_int_pointer()
{
    koopa_raw_type_kind *ret = new koopa_raw_type_kind();
    ret->tag = KOOPA_RTT_POINTER;
    ret->data.pointer.base = make_type_from_kind(KOOPA_RTT_INT32);
    return ret;
}
/*用来处理单目运算符的负号等*/
koopa_raw_value_data *make_value_from_int(int val)
{
    koopa_raw_value_data *ret_zero = new koopa_raw_value_data();
    ret_zero->ty = make_type_from_kind(KOOPA_RTT_INT32);
    ret_zero->name = nullptr;
    ret_zero->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
    ret_zero->kind.tag = KOOPA_RVT_INTEGER;
    ret_zero->kind.data.integer.value = val;
    return ret_zero;
}

koopa_raw_value_data *make_jump_from_block(koopa_raw_basic_block_t target)
{
    koopa_raw_value_data *res = new koopa_raw_value_data();
    res->ty = make_type_from_kind(KOOPA_RTT_UNIT);
    res->name = nullptr;
    res->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
    res->kind.tag = KOOPA_RVT_JUMP;
    res->kind.data.jump.args = make_slice_from_null(KOOPA_RSIK_VALUE);
    res->kind.data.jump.target = target;
    return res;
}

koopa_raw_value_data *make_branch_from_block(const void* cond,koopa_raw_basic_block_t target1,koopa_raw_basic_block_t target2)
{
    koopa_raw_value_data *ret_br = new koopa_raw_value_data();
    ret_br->ty = make_type_from_kind(KOOPA_RTT_UNIT);
    ret_br->name = nullptr;
    ret_br->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
    ret_br->kind.tag = KOOPA_RVT_BRANCH;       
    ret_br->kind.data.branch.cond = (koopa_raw_value_data *)cond;
    ret_br->kind.data.branch.true_bb = target1;
    ret_br->kind.data.branch.true_args = make_slice_from_null(KOOPA_RSIK_VALUE);
    ret_br->kind.data.branch.false_bb = target2; 
    ret_br->kind.data.branch.false_args = make_slice_from_null(KOOPA_RSIK_VALUE);
    return ret_br;
}

koopa_raw_value_data *make_param_value_from_kind(koopa_raw_type_tag_t ky)
{
    koopa_raw_value_data *ret_value = new koopa_raw_value_data();
    ret_value->ty = make_type_from_kind(ky);      
    ret_value->name = nullptr;
    ret_value->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
    ret_value->kind.tag = KOOPA_RVT_FUNC_ARG_REF;  
    ret_value->kind.data.func_arg_ref.index = 1;     
    return ret_value;
}

koopa_raw_value_data *make_value_from_init()
{
    koopa_raw_value_data *ret_zero = new koopa_raw_value_data();
    ret_zero->ty = make_type_from_kind(KOOPA_RTT_INT32);
    ret_zero->name = nullptr;
    ret_zero->used_by = make_slice_from_null(KOOPA_RSIK_VALUE);
    ret_zero->kind.tag = KOOPA_RVT_ZERO_INIT;
    return ret_zero;
}
    