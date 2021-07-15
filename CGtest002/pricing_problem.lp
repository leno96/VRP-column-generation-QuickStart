\ENCODING=ISO-8859-1
\Problem name: pricing_problem

Minimize
 obj1: - 19990 a_1_1 - 19990 a_1_2 + 10000 a_3_1 + 10000 a_3_2 + 15 a_4_1
       + 15 a_4_2
Subject To
 At_most_one_lecture_in_room_1:  a_1_1 + a_2_1 + a_3_1 + a_4_1 <= 1
 At_most_one_lecture_in_room_2:  a_1_2 + a_2_2 + a_3_2 + a_4_2 <= 1
 Professor_1_teaches_at_most_one_lecture: a_1_1 + a_1_2 + a_2_1 + a_2_2 <= 1
 Professor_2_teaches_at_most_one_lecture: a_3_1 + a_3_2 + a_4_1 + a_4_2 <= 1
Bounds
 0 <= a_1_1 <= 1
 0 <= a_1_2 <= 1
 0 <= a_2_1 <= 1
 0 <= a_2_2 <= 1
 0 <= a_3_1 <= 1
 0 <= a_3_2 <= 1
 0 <= a_4_1 <= 1
 0 <= a_4_2 <= 1
Binaries
 a_1_1  a_1_2  a_2_1  a_2_2  a_3_1  a_3_2  a_4_1  a_4_2 
End
