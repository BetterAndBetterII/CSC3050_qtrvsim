#ifndef VECTOR_REGISTER_H
#define VECTOR_REGISTER_H

#include <QMetaType>
#include <vector>
#include <cstdint>

namespace machine {

/**
 * 向量寄存器类
 * 
 * 使用字节数组存储向量数据，支持不同宽度(8/16/32位)的访问
 * 所有方法都是内联实现以提高性能
 */
class VectorRegister {
public:
    // 构造函数
    inline explicit VectorRegister(size_t size) : data(size) {}  // 移除constexpr
    inline VectorRegister() = default;  // 默认构造函数

    [[nodiscard]] inline uint32_t get_u32(uint32_t index) const {
        return *reinterpret_cast<const uint32_t*>(&data[index * 4]);
    }

    inline void set_u32(uint32_t index, uint32_t value) {
        *reinterpret_cast<uint32_t*>(&data[index * 4]) = value;
    }

    // 获取向量大小
    [[nodiscard]] inline size_t size() const {
        return data.size(); 
    }

    // 调整向量大小
    inline void resize(size_t new_size) {
        data.resize(new_size); 
    }

    // 比较操作符
    inline bool operator==(const VectorRegister &other) const {
        return data == other.data;
    }

    inline bool operator!=(const VectorRegister &other) const {
        return !(*this == other);
    }

    // 获取原始数据
    [[nodiscard]] inline const std::vector<uint32_t>& raw_data() const {
        return data; 
    }
    inline std::vector<uint32_t>& raw_data() {
        return data; 
    }

private:
    std::vector<uint32_t> data;  // 使用字节数组存储向量数据
};

} // namespace machine

Q_DECLARE_METATYPE(machine::VectorRegister)

#endif // VECTOR_REGISTER_H