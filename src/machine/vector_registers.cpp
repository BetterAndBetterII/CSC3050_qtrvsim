#include "vector_registers.h"
#include "simulator_exception.h"

namespace machine {

VectorRegisters::VectorRegisters() : QObject() {
    reset();
}

VectorRegisters::VectorRegisters(const VectorRegisters &orig) : QObject() {
    this->vr = orig.vr;
    this->vl = orig.vl;
    this->vtype = orig.vtype;
}

VectorRegister& VectorRegisters::read_vr(RegisterId reg) {
    VectorRegister &value = this->vr.at(reg);
    emit vr_read(reg, value);
    return value;
}

const VectorRegister& VectorRegisters::read_vr(RegisterId reg) const {
    const VectorRegister &value = this->vr.at(reg);
    emit vr_read(reg, value);
    return value;
}

void VectorRegisters::write_vr(RegisterId reg, const VectorRegister &value) {
    this->vr.at(reg) = value;
    emit vr_update(reg, value);
}

bool VectorRegisters::operator==(const VectorRegisters &c) const {
    return this->vr == c.vr && 
           this->vl == c.vl && 
           this->vtype == c.vtype;
}

bool VectorRegisters::operator!=(const VectorRegisters &c) const {
    return !this->operator==(c);
}

void VectorRegisters::reset() {
    // 初始化所有向量寄存器为0
    for (auto &reg : vr) {
        reg.resize(32 * 4);  // 默认32个32位元素
        for (size_t i = 0; i < reg.size(); i++) {
            reg.set_u32(i, 0);
        }
    }
    
    // 初始化向量控制寄存器
    vl = 0;
    vtype = 32;  // 默认32位元素
}

} // namespace machine