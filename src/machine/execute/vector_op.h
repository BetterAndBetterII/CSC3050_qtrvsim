#ifndef VECTOR_OP_H
#define VECTOR_OP_H

#include <QMetaType>
#include <cstdint>

namespace machine {

enum class VectorOp : uint8_t {
    // Vector Load Instructions (LOAD-FP major opcode: 0b0000111)
    VL      = 0b0000111,  // Vector unit-stride load

    // Vector Store Instructions (STORE-FP major opcode: 0b0100111)
    VS      = 0b0100111,  // Vector unit-stride store
    VADDV    = 0b000000,  // Vector-Vector arithmetic
    VADDX    = 0b000001,  // Vector-Scalar arithmetic
    VADDI    = 0b000010,  // Vector-Immediate arithmetic

    VMULV    = 0b000100,  // Vector-Vector multiplication

    // Vector Configuration Instructions (OP-V major opcode: 0b1010111)
    VSETVL  = 0b1010111,  // Vector set length
};

}

Q_DECLARE_METATYPE(machine::VectorOp)

#endif // VECTOR_OP_H
