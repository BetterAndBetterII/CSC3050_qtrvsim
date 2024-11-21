# encoding: utf-8

import numpy as np

# Reload the new file to inspect its updated structure
updated_file_path = r'D:\Coding\Riscv\3050_Proj3\qtrvsim\tests\cli\matrix\matrix'

with open(updated_file_path, 'r', encoding='utf-8') as file:
    updated_content = file.read()

# Modify the parsing logic to handle ',' or ' ' as delimiters in the .word lines
def parse_updated_matrix_data(content):
    """
    Parses updated matrix data where each matrix starts with 'matrix[n]:'
    and each row starts with '.word', with values separated by ',' or ' '.
    """
    matrices = {}
    lines = content.splitlines()
    current_matrix = None
    matrix_data = []

    for line in lines:
        line = line.strip()
        if line.startswith("matrix") and line.endswith(':'):
            if current_matrix:
                # Save the previous matrix
                matrices[current_matrix] = np.array(matrix_data, dtype=int)
            current_matrix = line[:-1]  # Remove ':' to use as key
            matrix_data = []
        elif line.startswith('.word'):
            # Handle values separated by ',' or ' '
            raw_values = line.split(maxsplit=1)[1]
            delimiters = ',' if ',' in raw_values else ' '
            values = list(map(int, filter(None, raw_values.split(delimiters))))
            matrix_data.append(values)

    # Save the last matrix
    if current_matrix:
        matrices[current_matrix] = np.array(matrix_data, dtype=int)

    return matrices

# Parse the updated file content
updated_matrices = parse_updated_matrix_data(updated_content)

# Display parsed matrices and their shapes for verification
print(updated_matrices.keys(), {key: mat.shape for key, mat in updated_matrices.items()})

# Test matrix1 * matrix2 == matrix3
matrix1 = updated_matrices['matrix1']
matrix2 = updated_matrices['matrix2']
matrix3 = updated_matrices['matrix3']

result_computed_corrected = np.dot(matrix1, matrix2.T)

# Check if the computed result matches the expected result
result_expected = matrix3
result_match = np.array_equal(result_computed_corrected, result_expected)
print("Matrix multiplication result match:", result_match)
print(f"Computed result:\n{result_computed_corrected}")

# 计算第一个元素
print(result_computed_corrected[0][0])
print(matrix1[0])  # 第一行
print(matrix2[0])  # 第一列
print(matrix1[0][32:] @ matrix2[0][32:])  # 第一个元素
