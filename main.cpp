#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <time.h>
#include <cmath>
#include "heap.h"

struct {
bool MEAN = false;
bool STD = false;
bool MIN = false;
bool FIRSTQ = false;
bool MEDIAN = false;
bool THIRDQ = false;
bool MAX = false;
} options;

int track_index = 0;

struct element{
    float value;
    std::string date;
    std::string time;

};


class minmax_heap{
 std::vector<float> data;
    int heap_size = 0;
public:

    minmax_heap(){
        data.push_back(0);
    }
    int parent(int i){
        return std::floor(i/2);
    }
    int grand_parent(int i){
        return std::floor(i/4);
    }
    int left(int i){
        return 2*i;
    }
    int right(int i){
        return 2*i+1;
    }

    void minmax_heapify(int index){
        int left = minmax_heap::left(index);
        int right = minmax_heap::right(index);
        int largest;
        if(left <= minmax_heap::heap_size &&  minmax_heap::data[left] > minmax_heap::data[index]){
            largest = left;
        }
        else{
            largest = index;
        }

        if(right <= minmax_heap::heap_size && minmax_heap::data[right] > minmax_heap::data[largest]){
            largest = right;
        }
        std::cout << "largest: " << largest << "left: " << left<< "right: " << right<< std::endl;
        std::cout << "left child: " << minmax_heap::data[left] << "right: " << minmax_heap::data[right]<< std::endl;

        std::cout << "largest child: " << minmax_heap::data[largest] << std::endl;
        std::cout << "index: " << index << std::endl;

        if(largest != index){
            std::swap(minmax_heap::data[index], minmax_heap::data[largest]);
            minmax_heapify(largest);
        }
    }
    void pushdown_min(int index){
        int min_child;
        int child_foundinfo = 0;
        if(left(index) <= minmax_heap::heap_size){
            min_child = left(index);
            child_foundinfo = 1;
        }
        if(right(index) <= minmax_heap::heap_size &&
        minmax_heap::data[right(index)] < minmax_heap::data[min_child]){
                min_child = right(index);
                child_foundinfo = 1;
        }
        int left_gchild = left(left(index));
        for(int i = 0; i < 4 && left_gchild + i < minmax_heap::heap_size; i++){
            if(minmax_heap::data[left_gchild + i] < minmax_heap::data[min_child]){
                min_child = left_gchild + i;
                child_foundinfo = 2;
            }
        }
        if(child_foundinfo == 2){
            std::swap(minmax_heap::data[index], minmax_heap::data[min_child]);
            if(minmax_heap::data[min_child] > minmax_heap::data[parent(min_child)]){
                std::swap(minmax_heap::data[min_child], minmax_heap::data[parent(min_child)]);
            }
            pushdown_min(min_child);
        }
        else if(child_foundinfo == 1){
            std::swap(minmax_heap::data[index], minmax_heap::data[min_child]);
        }

    }
    void pushdown(int index){
        int level = std::floor(std::log2(index));
        if (level % 2 == 0){
            pushdown_min( index);
        }else{
            pushdown_max(index);
        }
    }
    void build_minmax_heap(int n){
        minmax_heap::heap_size = n;
        for(int i = std::floor(minmax_heap::heap_size/2); i >= 1; i--){
            pushdown( i);
        }
        return;
    }
    float extract_max(){
        if(minmax_heap::data.size() < 1){
            std::cout << "heap underflow" << std::endl;
        }
        float max = minmax_heap::data[1];
        minmax_heap::data[1] = minmax_heap::data[minmax_heap::heap_size ];
        heap_size--;
        minmax_heap::data.pop_back();
        print();
        minmax_heapify( 1);
        print();
        return max;
    }
    void insert(float value){
        heap_size++;
        data.push_back(value);
        //build_max_heap();
        print();
        
        int index = heap_size;
        while(index > 1){
            int parent = minmax_heap::parent(index);
            if(data[parent] < data[index]){
                float temp = data[parent];
                data[parent] = data[index];
                data[index] = temp;
                index = parent;
            }
            else{
                break;
            }
        }
        
        return;
    }
void print(){
    for(int i = 0; i <= heap_size; i++){
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}
    int size(){
        return data.size();
    }
bool check_heap(){
    for(int i = 2; i <= heap_size; i++){
        if(data[i] > data[minmax_heap::parent(i)]){
        std::cout << "Child at index "<< i << " " << data[i] <<  std::endl;
            std::cout << "Parent at index "<< parent(i)  << " " << data[parent(i)]<< std::endl;
            return false;
        }
    }
    return true;
    }




};

class stats{
    public:
float stats[7] = {0};
std::string first_date = "";
std::string first_time = "";
std::string last_date;
std::string last_time;
void calculate(std::vector<element> &data){
first_date = data[0].date;
first_time = data[0].time;
last_date = data[data.size()-1].date;
last_time = data[data.size()-1].time;

stats[0] = calculate_mean(data);
stats[1] = calculate_std(data);
stats[2] = calculate_min(data);
stats[3] = calculate_firstq(data);
stats[4] = calculate_median(data);
stats[5] = calculate_thirdq(data);
stats[6] = calculate_max(data);
};
float calculate_mean(std::vector<element> &data){
    float sum = 0;
    for(int i = 0; i < data.size(); i++){
        sum += data[i].value;
    }
    return sum/data.size();
}
float calculate_std(std::vector<element> &data){
    float mean = calculate_mean(data);
    float sum = 0;
    for(int i = 0; i < data.size(); i++){
        sum += (data[i].value - mean)*(data[i].value - mean);
    }
    return sqrt(sum/(data.size() - 1));
}
float calculate_min(std::vector<element> &data){
    float min = data[0].value;
    for(int i = 1; i < data.size(); i++){
        if(data[i].value < min){
            min = data[i].value;
        }
    }
    return min;
}
float calculate_firstq(std::vector<element> &data){
    //gumbel quartile
    float q1 = 0;
    int n = data.size();
    std::cout << n << std::endl;
    if(n%100 == 0){
        q1 = data[(n/4)-1].value;
    }
    else{
        float left = data[ceil(n*0.25)].value;
        float right = data[floor(n*0.25)].value;
        std::cout << left << " " << right << std::endl;

        q1 = data[left].value*(1 - left/(left+right)) + data[right].value*(right/(left+right));
    }
    return q1;
}
float calculate_median(std::vector<element> &data){
    if (data.size() % 2 == 0){
        int index = data.size()/2;
        return (data[index].value + data[index-1].value)/2;
    }
    else{
        int index = data.size()/2;
        return data[index].value;
    }
}

float calculate_thirdq(std::vector<element> &data){
 //gumbel quartile
    float q1 = 0;
    int n = data.size();
    if(n%100 == 0){
        q1 = data[(3*n/4)-1].value;
    }
    else{
        float left = ceil(n*0.75);
        float right = floor(n*0.75);
        q1 = data[left].value*(1 - left/(left+right)) + data[right].value*(right/(left+right));
    }
    return q1;

}
float calculate_max(std::vector<element> &data){
    float max = data[0].value;
    for(int i = 1; i < data.size(); i++){
        if(data[i].value > max){
            max = data[i].value;
        }
    }
    return max;
}

void print(){
    std::cout <<first_date<<","<<first_time<<","<<last_date<<","<<last_time<<",";
    if(options.MEAN){
        std::cout <<stats[0]<<",";
    }if(options.STD){
        std::cout <<stats[1]<<",";
    }if(options.MIN){
        std::cout <<stats[2]<<",";
    }if(options.FIRSTQ){
        std::cout <<stats[3]<<",";
    }if(options.MEDIAN){
        std::cout <<stats[4]<<",";
    }if(options.THIRDQ){
        std::cout <<stats[5]<<",";
    }if(options.MAX){
        std::cout <<stats[6]<< std::endl;
    }
}

};

//std::vector<struct element> data;
stats stats;

void reader(char* file) {
    std::ifstream reader;
    std::string line;
    //data.reserve(11128);

    reader.open(file);
    getline(reader, line);
    //std::cout << line << std::endl;
    int stat_count = stoi(line);
    for(int i = 0; i < stat_count; i++){
        getline(reader, line);
        //std::cout <<line << std::endl;
            if(line == "mean"){
                options.MEAN = true;
            }else if(line == "std"){
                options.STD = true;
            }else if(line == "min"){
                options.MIN = true;
            }else if(line == "firstq"){
                options.FIRSTQ = true;
            }else if(line == "median"){
                options.MEDIAN = true;
            }else if(line == "thirdq"){
                options.THIRDQ = true;
            }else if(line == "max"){
                options.MAX = true;
            }
        }

    
    
    getline(reader, line);
    getline(reader, line, ',');
    int data_count = stoi(line);
    //std::cout << data_count << std::endl;
    getline(reader, line);
    //std::cout << line << std::endl;
    if(line == "gap"){
        track_index = 2;
    }else if(line == "grp"){
        track_index = 3;
    }else if(line == "v"){
        track_index = 4;
    }else if(line == "gi"){
        track_index = 5;
    }

    for(int i = 0; i < data_count; i++){
        getline(reader, line);
        if(line == "add"){
            struct element temp;
            getline(reader, line, ',');
            //std::cout << line << std::endl;
            if(stats.first_date == ""){
                stats.first_date = line;
            }
            temp.date = line;
            stats.last_date = line;
            getline(reader, line, ',');
            if(stats.first_time == ""){
                stats.first_time = line;
            }
            temp.time = line;
            stats.last_time = line;
           for(int i = 0; i < track_index - 1; i++){
                getline(reader, line, ',');
            }
           std::cout <<"Added Value is: " <<line << std::endl;
            temp.value = std::stof(line);
            //data.push_back(temp);
        }
        if(line == "print"){
            //stats.calculate(data);
            stats.print();
        }
    }

    reader.close();
    return;
}


int main(int argc, char** argv) {
  // reader(argv[1]);
  minmax_heap heap;
  heap.insert(1);
    heap.insert(2);
    heap.insert(3);
    heap.insert(4);
    heap.insert(5);
    heap.insert(6);
    heap.insert(7);
    heap.insert(8);
    heap.insert(9);
    heap.insert(10);
    heap.insert(11);
    heap.insert(12);
    heap.insert(13);
    heap.insert(1.4);
    heap.print();
    if(heap.check_heap()){
        std::cout << "Heap is valid" << std::endl;
    }else{
        std::cout << "Heap is not valid" << std::endl;
    }
    std:: cout << heap.extract_max();
    heap.print();
    if(heap.check_heap()){
        std::cout << "Heap is valid" << std::endl;
    }else{
        std::cout << "Heap is not valid" << std::endl;
    }
	return 0;
}
