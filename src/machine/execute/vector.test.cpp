#include "vector.h"

#include "vector.test.h"
#include "vector_registers.h"

#include <iostream>

using namespace machine;

void VectorTest::test_vector_add_data() {
    QTest::addColumn<QVector<uint32_t>>("vs2_data");
    QTest::addColumn<QVector<uint32_t>>("vs1_data");
    QTest::addColumn<QVector<uint32_t>>("expected");

    QTest::newRow("basic_add") 
        << QVector<uint32_t>{1, 2, 3, 4}
        << QVector<uint32_t>{5, 6, 7, 8}
        << QVector<uint32_t>{6, 8, 10, 12};

    QTest::newRow("overflow_add") 
        << QVector<uint32_t>{0xFFFFFFFF, 2, 3, 4}
        << QVector<uint32_t>{1, 6, 7, 8}
        << QVector<uint32_t>{0, 8, 10, 12};
}

void VectorTest::test_vector_add() {
    QFETCH(QVector<uint32_t>, vs2_data);
    QFETCH(QVector<uint32_t>, vs1_data);
    QFETCH(QVector<uint32_t>, expected);

    // 创建向量寄存器并初始化
    VectorRegister vs2(vs2_data.size());
    VectorRegister vs1(vs1_data.size());
    VectorRegister vd(expected.size());
    
    // 设置测试数据
    for(int i = 0; i < vs2_data.size(); i++) {
        vs2.set_u32(i, vs2_data[i]);
        vs1.set_u32(i, vs1_data[i]);
    }

    // 执行向量加法
    vector_operate_vector(VectorOp::VADDV, &vd, &vs2, &vs1, RegisterValue(0), RegisterValue(0), nullptr);

    // 验证结果
    for(int i = 0; i < expected.size(); i++) {
        QCOMPARE(vd.get_u32(i), expected[i]);
    }
}


void VectorTest::test_vector_mul_data() {
    QTest::addColumn<QVector<uint32_t>>("vs2_data");
    QTest::addColumn<QVector<uint32_t>>("vs1_data");
    QTest::addColumn<uint32_t>("expected_result");

    QTest::newRow("basic_mul") 
        << QVector<uint32_t>{1, 2, 3, 4}
        << QVector<uint32_t>{5, 6, 7, 8}
        << uint32_t(70);  // 1*5 + 2*6 + 3*7 + 4*8
}

void VectorTest::test_vector_mul() {
    QFETCH(QVector<uint32_t>, vs2_data);
    QFETCH(QVector<uint32_t>, vs1_data);
    QFETCH(uint32_t, expected_result);

    VectorRegister vs2(vs2_data.size() * 4);
    VectorRegister vs1(vs1_data.size() * 4);
    VectorRegister vd(4);  // 只需要一个元素存储点乘结果
    
    for(int i = 0; i < vs2_data.size(); i++) {
        vs2.set_u32(i, vs2_data[i]);
        vs1.set_u32(i, vs1_data[i]);
    }

    RegisterValue res = vector_operate_vector(VectorOp::VMULV, &vd, &vs2, &vs1, RegisterValue(0), RegisterValue(0), nullptr);
    QCOMPARE(res, expected_result);
}

void VectorTest::test_vector_load_store() {
    const Address base_addr(0x2000);
    const QVector<uint32_t> test_data{1, 2, 3, 4};
    
    // 创建内存和向量寄存器
    std::vector<uint8_t> memory(0x2000, 0);
    VectorRegisters vregs = VectorRegisters();
    VectorRegister* vd = vregs.get_vr(RegisterId(0));
    
    // 设置向量类型和长度
    uint32_t size = test_data.size();
    vector_set_vl(RegisterValue(size), RegisterValue(32));
    
    // 设置向量数据
    for(uint32_t i = 0; i < test_data.size(); i++) {
        vd->set_u32(i, test_data[i]);
    }
    
    // 执行向量存储
    vector_operate_vector(VectorOp::VS, nullptr, vd, nullptr, RegisterValue(0), RegisterValue(0), &base_addr);

    // 验证内存内容
    for(uint32_t i = 0; i < test_data.size(); i++) {
        // 使用Address的+运算符来计算偏移
        Address curr_addr = base_addr + (i * 4);
        uint32_t stored_value = *reinterpret_cast<uint32_t*>(&memory[curr_addr.get_raw()]);
        QCOMPARE(stored_value, test_data[i]);
    }

    // 测试加载
    VectorRegister vd2(test_data.size() * 4);
    vector_operate_vector(VectorOp::VL, &vd2, nullptr, nullptr, RegisterValue(0), RegisterValue(0), &base_addr);
    
    // 验证加载的数据
    for(uint32_t i = 0; i < test_data.size(); i++) {
        QCOMPARE(vd2.get_u32(i), test_data[i]);
    }
}


void VectorTest::test_vector_add_imm_data() {
    QTest::addColumn<QVector<uint32_t>>("vs2_data");
    QTest::addColumn<uint32_t>("imm");
    QTest::addColumn<QVector<uint32_t>>("expected");

    QTest::newRow("basic_add_imm")
        << QVector<uint32_t>{1, 2, 3, 4}
        << uint32_t(5)
        << QVector<uint32_t>{6, 7, 8, 9};
}

void VectorTest::test_vector_add_imm() {
    QFETCH(QVector<uint32_t>, vs2_data);
    QFETCH(uint32_t, imm);
    QFETCH(QVector<uint32_t>, expected);

    VectorRegister vs2(vs2_data.size() * 4);
    VectorRegister vd(expected.size() * 4);

    for(int i = 0; i < vs2_data.size(); i++) {
        vs2.set_u32(i, vs2_data[i]);
    }

    vector_operate_vector(VectorOp::VADDI, &vd, &vs2, nullptr, RegisterValue(0), RegisterValue(imm), nullptr);

    for(int i = 0; i < expected.size(); i++) {
        QCOMPARE(vd.get_u32(i), expected[i]);
    }
}

void VectorTest::test_vector_add_register_data() {
    QTest::addColumn<QVector<uint32_t>>("vs2_data");
    QTest::addColumn<uint32_t>("rs1_data");
    QTest::addColumn<QVector<uint32_t>>("expected");

    QTest::newRow("basic_add_register")
        << QVector<uint32_t>{1, 2, 3, 4}
        << uint32_t(5)
        << QVector<uint32_t>{6, 7, 8, 9};

    QTest::newRow("overflow_add_register")
        << QVector<uint32_t>{0xFFFFFFFF, 2, 3, 4}
        << uint32_t(1)
        << QVector<uint32_t>{0, 3, 4, 5};
}

void VectorTest::test_vector_add_register() {
    QFETCH(QVector<uint32_t>, vs2_data);
    QFETCH(uint32_t, rs1_data);
    QFETCH(QVector<uint32_t>, expected);

    VectorRegister vs2(vs2_data.size() * 4);
    VectorRegister vd(expected.size() * 4);

    for(int i = 0; i < vs2_data.size(); i++) {
        vs2.set_u32(i, vs2_data[i]);
    }

    vector_operate_vector(VectorOp::VADDX, &vd, &vs2, nullptr, RegisterValue(rs1_data), RegisterValue(0), nullptr);

    for(int i = 0; i < expected.size(); i++) {
        QCOMPARE(vd.get_u32(i), expected[i]);
    }
}

void VectorTest::initTestCase() {
    // 设置默认向量长度和类型
    vector_set_vl(RegisterValue(4), RegisterValue(32));
}

QTEST_APPLESS_MAIN(VectorTest)
