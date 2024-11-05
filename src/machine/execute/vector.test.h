#ifndef VECTOR_TEST_H
#define VECTOR_TEST_H

#include <QtTest>

class VectorTest : public QObject {
    Q_OBJECT

private slots:
    static void initTestCase();
    static void test_vector_add_data();
    static void test_vector_add();
    static void test_vector_mul_data();
    static void test_vector_mul();
    static void test_vector_load_store();
    static void test_vector_add_imm_data();
    static void test_vector_add_imm();
    static void test_vector_add_register_data();
    static void test_vector_add_register();
};

#endif // VECTOR_TEST_H