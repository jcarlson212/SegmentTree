#include <iostream>
#include <vector>

using namespace std;

//Basic Segment tree for log(n) range queries that update in log(n) time after insertions. Should work with any summable datastructure.
template <class T>
class SegTree {
    private:
        vector<T> input_array;
        vector<T> seg_tree;

        T query_range_helper(T left_index, T right_index, T seg_index, T current_left_index, T current_right_index){
            if(right_index < current_left_index || left_index > current_right_index){
                //not covered
                return 0;
            }
            if(left_index <= current_left_index && right_index >= current_right_index){
                return seg_tree[seg_index];
            }else{
                //it must be contained inside
                return query_range_helper(left_index, right_index, get_left_child_index(seg_index), current_left_index, (current_left_index + current_right_index)/2)
                    + query_range_helper(left_index, right_index, get_right_child_index(seg_index), (current_left_index + current_right_index)/2 + 1, current_right_index);
            }
        }

        void insert_helper(T index, T value, T seg_index, T left_index, T right_index){
            if(index >= left_index && index <= right_index){
                seg_tree[seg_index] += value;
                if(left_index == right_index){
                    input_array[index] += value;
                }
                if(left_index != right_index){
                    insert_helper(index, value, get_left_child_index(seg_index), left_index, (left_index + right_index)/2);
                    insert_helper(index, value, get_right_child_index(seg_index), (left_index+right_index)/2+1, right_index);
                }
            }
        }

        T get_left_child_index(T index){
            return 2*index + 1;
        }

        T get_right_child_index(T index){
            return 2*index + 2;
        }

    public:
        SegTree(vector<T> input_array){
            this->input_array = input_array;
            seg_tree = vector<T>(4*input_array.size()+1, 0);
            build_seg_tree(0, 0, this->input_array.size()-1);
        }

        //O(n) time complexity. O(log(n)) space complexity for call stack. This could be reduced to O(1) by making it iterative. 
        T build_seg_tree(T seg_index, T left_index, T right_index){
            if(right_index == left_index){
                this->seg_tree[seg_index] = this->input_array[left_index];
            }else{
                T left_sum = build_seg_tree(get_left_child_index(seg_index), left_index, (left_index + right_index)/2);
                T right_sum = build_seg_tree(get_right_child_index(seg_index), (left_index + right_index)/2 + 1, right_index);
                this->seg_tree[seg_index] = left_sum + right_sum;
            }
            return this->seg_tree[seg_index];
        }

        //O(log(n)) time-complexity. O(log(n)) space-complexity from stack. Can be reduced to O(1) space.
        void insert(T index, T value){
            insert_helper(index, (value - this->input_array[index]), 0, 0, this->input_array.size()-1);
        }

        //assumes a valid query input (seg_index = 0)
        T query_range_sum(T left_index, T right_index){
            return query_range_helper(left_index, right_index, 0, 0, this->input_array.size()-1);
        }

        void print_seg_tree(T seg_index, T left_index, T right_index){
            if(seg_index >= this->seg_tree.size()){
                return;
            }
            cout << seg_index << "(" << left_index << ", " << right_index << "): " << this->seg_tree[seg_index] << "\n";
            if(left_index != right_index){
                print_seg_tree(get_left_child_index(seg_index), left_index, (left_index+right_index)/2);
                print_seg_tree(get_right_child_index(seg_index), (left_index+right_index)/2+1, right_index);
            }
        }

        T get_arr_size(){
            return this->input_array.size();
        }

        T get_tree_size(){
            return this->seg_tree.size();
        }

};

int main(){
    ios::sync_with_stdio(0); 
    cin.tie(0); 

    SegTree<int> t({1,2,3,4});

    t.print_seg_tree(0, 0, 3);
    t.insert(0, 4);
    t.insert(3, 2);
    t.print_seg_tree(0, 0, 3);
    
    return 0;
}
