#ifndef VECTOR_REGISTERS_H
#define VECTOR_REGISTERS_H

#include "register_value.h"
#include "registers.h"
#include "vector_register.h"

#include <QObject>
#include <array>

namespace machine {

class VectorRegisters : public QObject {
    Q_OBJECT
public:
    VectorRegisters();
    VectorRegisters(const VectorRegisters &);

    // 读写向量寄存器
    VectorRegister read_vr(RegisterId reg);
    [[nodiscard]] const VectorRegister& read_vr(RegisterId reg) const;
    VectorRegister* get_vr(RegisterId reg);
    void write_vr(RegisterId reg, const VectorRegister &value);

    // 获取/设置向量长度和类型
    [[nodiscard]] uint32_t get_vl() const { return vl; }
    void set_vl(uint32_t new_vl) { 
        vl = new_vl; 
        emit vl_update(vl);
    }
    
    [[nodiscard]] uint32_t get_vtype() const { return vtype; }
    void set_vtype(uint32_t new_vtype) { 
        vtype = new_vtype;
        emit vtype_update(vtype);
    }

    bool operator==(const VectorRegisters &c) const;
    bool operator!=(const VectorRegisters &c) const;

    void reset();  // 重置所有向量寄存器

signals:
    void vr_update(RegisterId reg, const VectorRegister &val);
    void vr_read(RegisterId reg, const VectorRegister &val) const;
    void vl_update(uint32_t new_vl);
    void vtype_update(uint32_t new_vtype);

private:
    std::vector<VectorRegister> vr = std::vector<VectorRegister>(32, VectorRegister(32));
    uint32_t vl = 0;     // 向量长度寄存器
    uint32_t vtype = 32;  // 向量类型寄存器
};

} // namespace machine

Q_DECLARE_METATYPE(machine::VectorRegisters)

#endif // VECTOR_REGISTERS_H