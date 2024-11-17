//
// Created by Gary on 24-11-4.
//

#ifndef QTRVSIM_VECTOR_H
#define QTRVSIM_VECTOR_H

#include "execute/vector_op.h"
#include "memory/address.h"
#include "register_value.h"
#include "vector_register.h"

#include <cstdint>
#include <vector>

namespace machine {


void vector_operate(
    VectorOp op,
    VectorRegister &vd,
    const VectorRegister *vs2,
    const VectorRegister *vs1,
    RegisterValue rs1,
    int32_t imm,
    uint8_t *memory,
    uint32_t addr);

//RegisterValue vector_operate(
//    VectorOp op,
//    const VectorRegister *vs2 = nullptr,
//    const VectorRegister *vs1 = nullptr,
//    RegisterValue rs1 = RegisterValue(0),
//    RegisterValue imm = RegisterValue(0));

RegisterValue vector_operate_vector(
    VectorOp op,
    VectorRegister *vd,
    const VectorRegister *vs2,
    const VectorRegister *vs1,
    RegisterValue rs1,
    RegisterValue imm,
    const Address* base_addr);

// 向量操作函数声明
RegisterValue vector_set_vl(RegisterValue rs1, RegisterValue rs2);

RegisterValue vector_add_vv(VectorRegister *vd,
                  const VectorRegister &vs2,
                  const VectorRegister &vs1);

RegisterValue vector_add_vx(VectorRegister *vd,
                  const VectorRegister &vs2, 
                  RegisterValue rs1);

RegisterValue vector_add_vi(VectorRegister *vd,
                  const VectorRegister &vs2,
                  RegisterValue imm);

RegisterValue vector_mul_vv(VectorRegister *vd, const VectorRegister &vs2,
                  const VectorRegister &vs1);

void vector_load(VectorRegister *vd,
                 const uint8_t *memory,
                 uint32_t base_addr);

void vector_load(VectorRegister *vd,
                 const Address *memory);

void vector_store(const VectorRegister &vs3,
                  uint8_t *memory,
                  uint32_t base_addr);

void vector_store(const VectorRegister &vs3,
                  const Address *memory);

// 获取当前向量长度和类型
uint32_t get_vector_length();
uint32_t get_vector_type();

} // namespace machine

#endif // QTRVSIM_VECTOR_H
