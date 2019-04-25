Maximize
  X_2_1_3 + X_2_1_4 + X_2_2_3 + X_2_2_4 + X_2_3_1 + X_2_3_2 + X_2_3_4
   + X_2_4_1 + X_2_4_2 + X_2_4_4 + X_3_1_2 + X_3_1_3 + X_3_1_4 + X_3_2_1
   + X_3_2_3 + X_3_2_4 + X_3_3_1 + X_3_3_2 + X_3_3_4 + X_3_4_1 + X_3_4_2
   + X_3_4_3 + X_3_4_4 + X_4_1_2 + X_4_1_3 + X_4_1_4 + X_4_2_1 + X_4_2_3
   + X_4_2_4 + X_4_3_1 + X_4_3_2 + X_4_3_4 + X_4_4_1 + X_4_4_2 + X_4_4_3
   + X_4_4_4
Subject To
 values_of_cell_(2,1): X_2_1_3 + X_2_1_4 = 1
 values_of_cell_(2,2): X_2_2_3 + X_2_2_4 = 1
 values_of_cell_(2,3): X_2_3_1 + X_2_3_2 + X_2_3_4 = 1
 values_of_cell_(2,4): X_2_4_1 + X_2_4_2 + X_2_4_4 = 1
 values_of_cell_(3,1): X_3_1_2 + X_3_1_3 + X_3_1_4 = 1
 values_of_cell_(3,2): X_3_2_1 + X_3_2_3 + X_3_2_4 = 1
 values_of_cell_(3,3): X_3_3_1 + X_3_3_2 + X_3_3_4 = 1
 values_of_cell_(3,4): X_3_4_1 + X_3_4_2 + X_3_4_3 + X_3_4_4 = 1
 values_of_cell_(4,1): X_4_1_2 + X_4_1_3 + X_4_1_4 = 1
 values_of_cell_(4,2): X_4_2_1 + X_4_2_3 + X_4_2_4 = 1
 values_of_cell_(4,3): X_4_3_1 + X_4_3_2 + X_4_3_4 = 1
 values_of_cell_(4,4): X_4_4_1 + X_4_4_2 + X_4_4_3 + X_4_4_4 = 1
 row_#2_with_value_of_1: X_2_3_1 + X_2_4_1 = 1
 row_#3_with_value_of_1: X_3_2_1 + X_3_3_1 + X_3_4_1 = 1
 row_#4_with_value_of_1: X_4_2_1 + X_4_3_1 + X_4_4_1 = 1
 row_#2_with_value_of_2: X_2_3_2 + X_2_4_2 = 1
 row_#3_with_value_of_2: X_3_1_2 + X_3_3_2 + X_3_4_2 = 1
 row_#4_with_value_of_2: X_4_1_2 + X_4_3_2 + X_4_4_2 = 1
 row_#2_with_value_of_3: X_2_1_3 + X_2_2_3 = 1
 row_#3_with_value_of_3: X_3_1_3 + X_3_2_3 + X_3_4_3 = 1
 row_#4_with_value_of_3: X_4_1_3 + X_4_2_3 + X_4_4_3 = 1
 row_#2_with_value_of_4: X_2_1_4 + X_2_2_4 + X_2_3_4 + X_2_4_4 = 1
 row_#3_with_value_of_4: X_3_1_4 + X_3_2_4 + X_3_3_4 + X_3_4_4 = 1
 row_#4_with_value_of_4: X_4_1_4 + X_4_2_4 + X_4_3_4 + X_4_4_4 = 1
 column_#2_with_value_of_1: X_3_2_1 + X_4_2_1 = 1
 column_#3_with_value_of_1: X_2_3_1 + X_3_3_1 + X_4_3_1 = 1
 column_#4_with_value_of_1: X_2_4_1 + X_3_4_1 + X_4_4_1 = 1
 column_#1_with_value_of_2: X_3_1_2 + X_4_1_2 = 1
 column_#3_with_value_of_2: X_2_3_2 + X_3_3_2 + X_4_3_2 = 1
 column_#4_with_value_of_2: X_2_4_2 + X_3_4_2 + X_4_4_2 = 1
 column_#1_with_value_of_3: X_2_1_3 + X_3_1_3 + X_4_1_3 = 1
 column_#2_with_value_of_3: X_2_2_3 + X_3_2_3 + X_4_2_3 = 1
 column_#4_with_value_of_3: X_3_4_3 + X_4_4_3 = 1
 column_#1_with_value_of_4: X_2_1_4 + X_3_1_4 + X_4_1_4 = 1
 column_#2_with_value_of_4: X_2_2_4 + X_3_2_4 + X_4_2_4 = 1
 column_#3_with_value_of_4: X_2_3_4 + X_3_3_4 + X_4_3_4 = 1
 column_#4_with_value_of_4: X_2_4_4 + X_3_4_4 + X_4_4_4 = 1
 block_#2_with_value_of_1: X_2_3_1 + X_2_4_1 = 1
 block_#3_with_value_of_1: X_3_2_1 + X_4_2_1 = 1
 block_#4_with_value_of_1: X_3_3_1 + X_3_4_1 + X_4_3_1 + X_4_4_1 = 1
 block_#2_with_value_of_2: X_2_3_2 + X_2_4_2 = 1
 block_#3_with_value_of_2: X_3_1_2 + X_4_1_2 = 1
 block_#4_with_value_of_2: X_3_3_2 + X_3_4_2 + X_4_3_2 + X_4_4_2 = 1
 block_#1_with_value_of_3: X_2_1_3 + X_2_2_3 = 1
 block_#3_with_value_of_3: X_3_1_3 + X_3_2_3 + X_4_1_3 + X_4_2_3 = 1
 block_#4_with_value_of_3: X_3_4_3 + X_4_4_3 = 1
 block_#1_with_value_of_4: X_2_1_4 + X_2_2_4 = 1
 block_#2_with_value_of_4: X_2_3_4 + X_2_4_4 = 1
 block_#3_with_value_of_4: X_3_1_4 + X_3_2_4 + X_4_1_4 + X_4_2_4 = 1
 block_#4_with_value_of_4: X_3_3_4 + X_3_4_4 + X_4_3_4 + X_4_4_4 = 1
Bounds
Binaries
 X_2_1_3 X_2_1_4 X_2_2_3 X_2_2_4 X_2_3_1 X_2_3_2 X_2_3_4 X_2_4_1 X_2_4_2
 X_2_4_4 X_3_1_2 X_3_1_3 X_3_1_4 X_3_2_1 X_3_2_3 X_3_2_4 X_3_3_1 X_3_3_2
 X_3_3_4 X_3_4_1 X_3_4_2 X_3_4_3 X_3_4_4 X_4_1_2 X_4_1_3 X_4_1_4 X_4_2_1
 X_4_2_3 X_4_2_4 X_4_3_1 X_4_3_2 X_4_3_4 X_4_4_1 X_4_4_2 X_4_4_3 X_4_4_4
End
