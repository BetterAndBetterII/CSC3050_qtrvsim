//
// Created by Gary on 24-11-4.
//
#include "vector.h"

#include "common/logging.h"

#include <QDebug>

namespace machine {

// 添加vector dispatcher函数
void vector_operate(
    VectorOp op,
    VectorRegister &vd,
    const VectorRegister *vs2,
    const VectorRegister *vs1,
    RegisterValue rs1,
    int32_t imm,
    uint8_t *memory,
    uint32_t addr) {
    
    switch(op) {
    case VectorOp::VSETVL:
        vector_set_vl(rs1, RegisterValue(imm));
        break;
    case VectorOp::VADDV:
        vector_add_vv(&vd, *vs2, *vs1);
        break; 
    case VectorOp::VADDX:
        vector_add_vx(&vd, *vs2, rs1);
        break;
    case VectorOp::VADDI:
        vector_add_vi(&vd, *vs2, imm);
        break;
//    case VectorOp::VMULV:
//        vector_mul_vv(vd, *vs2, *vs1);
//        break;
//    case VectorOp::VL:
//        vector_load(vd, memory, addr);
//        break;
//    case VectorOp::VS:
//        vector_store(vd, memory, addr);
//        break;
    default:
        qDebug("ERROR: Unknown vector operation");
    }
}

RegisterValue vector_operate_vector(
    VectorOp op,
    VectorRegister *vd,
    const VectorRegister *vs2,
    const VectorRegister *vs1,
    RegisterValue rs1,
    RegisterValue imm,
    const Address* base_addr) {

    switch(op) {
    case VectorOp::VSETVL:
        return vector_set_vl(rs1, imm);
    case VectorOp::VMULV:
        return vector_mul_vv(*vs2, *vs1);
    case VectorOp::VADDV:
        return vector_add_vv(vd, *vs2, *vs1);
    case VectorOp::VADDX:
        return vector_add_vx(vd, *vs2, rs1);
    case VectorOp::VADDI:
        return vector_add_vi(vd, *vs2, imm);
    case VectorOp::VL:
        vector_load(vd, base_addr);
        return {0};
    case VectorOp::VS:
        vector_store(*vs2, base_addr);
        return {0};
    default:
        qDebug("ERROR: Unknown vector operation");
        return {0};
    }
}

//VectorRegister vector_operate_vector(
//    VectorOp op,
//    const VectorRegister *vd,
//    const VectorRegister *vs2,
//    const VectorRegister *vs1,
//    RegisterValue rs1,
//    RegisterValue imm){
//    switch(op) {
//    case VectorOp::VADDV:
//        return vector_add_vv(*vd, *vs2, *vs1);
//    case VectorOp::VADDX:
//        return vector_add_vx(vd, *vs2, rs1);
//    case VectorOp::VADDI:
//        return vector_add_vi(vd, *vs2, imm);
//    default:
//        qDebug("ERROR: Unknown vector operation");
//        return {};
//    }
//}

// 向量长度和类型的全局状态
static struct {
    uint32_t vl;       // 向量长度寄存器
    uint32_t vtype;    // 向量类型寄存器(8/16/32)
} vector_state;

// 设置向量长度和类型
RegisterValue vector_set_vl(RegisterValue rs1, RegisterValue rs2) {
    // rs1包含请求的向量长度
    // rs2包含向量元素类型(8/16/32)
    vector_state.vl = rs1.as_u32();
    vector_state.vtype = rs2.as_u32();
    
    // 返回实际设置的向量长度
    return {vector_state.vl};
}

// 向量-向量加法
RegisterValue vector_add_vv(VectorRegister *vd,
                  const VectorRegister &vs2,
                  const VectorRegister &vs1) {
    for(uint32_t i = 0; i < vector_state.vl; i++) {
        switch(vector_state.vtype) {
            case 32:
                vd->set_u32(i, vs2.get_u32(i) + vs1.get_u32(i));
                break;
            default:
                qDebug("ERROR: Unsupported vector type: %u", vector_state.vtype);
        }
    }
    return {0};
}

//VectorRegister vector_add_vv(const VectorRegister &vs2,
//                            const VectorRegister &vs1) {
//    VectorRegister vd;
//    for(uint32_t i = 0; i < vector_state.vl; i++) {
//        switch(vector_state.vtype) {
//        case 32:
//            vd.set_u32(i, vs2.get_u32(i) + vs1.get_u32(i));
//            break;
//        default:
//            qDebug("ERROR: Unsupported vector type: %u", vector_state.vtype);
//        }
//    }
//    return vd;
//}

// 向量-标量加法
RegisterValue vector_add_vx(VectorRegister *vd,
                  const VectorRegister &vs2, 
                  RegisterValue rs1) {
    for(uint32_t i = 0; i < vector_state.vl; i++) {
        switch(vector_state.vtype) {
            case 32:
                vd->set_u32(i, vs2.get_u32(i) + rs1.as_u32());
                break;
            default:
                qDebug("ERROR: Unsupported vector type: %u", vector_state.vtype);
        }
    }
    return {0};
}

// 向量-立即数加法
RegisterValue vector_add_vi(VectorRegister *vd,
                  const VectorRegister &vs2,
                  RegisterValue imm) {
    for(uint32_t i = 0; i < vector_state.vl; i++) {
        switch(vector_state.vtype) {
            case 32:
                vd->set_u32(i, vs2.get_u32(i) + static_cast<uint32_t>(imm));
                break;
            default:
                qDebug("ERROR: Unsupported vector type: %u", vector_state.vtype);
        }
    }
    return {0};
}

RegisterValue vector_mul_vv(const VectorRegister &vs2,
                            const VectorRegister &vs1) {
    auto result = RegisterValue(0);

    for(uint32_t i = 0; i < vector_state.vl; i++) {
        switch(vector_state.vtype) {
            case 32:
                result = RegisterValue(result.as_u32() + 
                         vs2.get_u32(i) * vs1.get_u32(i));
                break;
        }
    }
    return result;
}

// 向量加载
void vector_load(VectorRegister *vd,
                 const uint8_t *memory,
                 uint32_t base_addr) {
    uint32_t offset = 0;
    
    for(uint32_t i = 0; i < vector_state.vl; i++) {
        switch(vector_state.vtype) {
            case 32:
                vd->set_u32(i, *reinterpret_cast<const uint32_t*>(
                                  memory + base_addr + offset));
                offset += 4;
                break;
            default:
                qDebug("ERROR: Unsupported vector type: %u", vector_state.vtype);
        }
    }
}

void vector_load(VectorRegister *vd,
                 const Address *memory) {
    uint32_t offset = 0;

    for(uint32_t i = 0; i < vector_state.vl; i++) {
        switch(vector_state.vtype) {
        case 32:
            vd->set_u32(i, *reinterpret_cast<const uint32_t*>(memory->get_raw()));
            offset += 4;
            break;
        default:
            qDebug("ERROR: Unsupported vector type: %u", vector_state.vtype);
        }
    }
}

// 向量存储
void vector_store(const VectorRegister &vs3,
                  uint8_t *memory,
                  uint32_t base_addr) {
    uint32_t offset = 0;
    
    for(uint32_t i = 0; i < vector_state.vl; i++) {
        switch(vector_state.vtype) {
            case 32:
                *reinterpret_cast<uint32_t*>(memory + base_addr + offset)
                    = vs3.get_u32(i);
                offset += 4;
                break;
            default:
                qDebug("ERROR: Unsupported vector type: %u", vector_state.vtype);
        }
    }
}

void vector_store(const VectorRegister &vs3,
                  const Address *memory) {
    for(uint32_t i = 0; i < vector_state.vl; i++) {
        switch(vector_state.vtype) {
        case 32:
            *reinterpret_cast<uint32_t*>(
                reinterpret_cast<uint8_t*>(memory->get_raw()) + (i * 4))
                = vs3.get_u32(i);
            break;
        default:
            qDebug("ERROR: Unsupported vector type: %u", vector_state.vtype);
        }
    }
}

} // namespace machine
