#include "gtest/gtest.h" 

#include <forward_list>
TEST(TestModern, ForwardList) {
    {
        std::forward_list<int> list;
        for(int i{0};i < 4; ++i) { // 0, 1, 2, 3의 순서로 삽입합니다.
            list.push_front(i); 
        }

        // 가장 마지막에 넣은 요소가 begin입니다.
        std::vector<int> v;
        for(auto val : list) {
            v.push_back(val);
        }
        EXPECT_TRUE(v[0] == 3 && v[1] == 2 && v[2] == 1 && v[3] == 0);

        // insert after는 주어진 위치 뒤에 삽입합니다.
        list.insert_after(list.begin(), 10);
        v.clear();
        for(auto val : list) {
            v.push_back(val);
        }
        EXPECT_TRUE(v[0] == 3 && v[1] == 10 && v[2] == 2 && v[3] == 1 && v[4] == 0);

        // erase_after는 주어진 위치 뒤를 삭제합니다.
        list.erase_after(list.begin());
        v.clear();
        for(auto val : list) {
            v.push_back(val);
        }
        EXPECT_TRUE(v[0] == 3 && v[1] == 2 && v[2] == 1 && v[3] == 0);  

        // before_begin()은 begin()의 앞 이터레이터입니다. erase_after()는 주어진 위치 뒤를 삭제하므로 begin()이 삭제됩니다.
        list.erase_after(list.before_begin());
        v.clear();
        for(auto val : list) {
            v.push_back(val);
        }
        EXPECT_TRUE(v[0] == 2 && v[1] == 1 && v[2] == 0);


        // slice_after()는 주어진 요소 뒤부터 이동합니다.
        std::forward_list<int> temp;
        list.splice_after(list.begin(), temp); 
        for(auto val : list) {
            v.push_back(val);
        }
        EXPECT_TRUE(v[0] == 2); // begin 다음부터 이동했으므로 2만 남았습니다.
    }
    

}