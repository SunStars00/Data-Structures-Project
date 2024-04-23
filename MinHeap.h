#ifndef MINHEAP_H
#define MINHEAP_H

#include <vector>

using namespace std;

//binary min heap
template <typename T>
class MinHeap
{
public:
    vector<T> data;

    void AddItem(T item)
    {
        data.push_back(item);
        int index = data.size()-1;
        while (index > 0 && data[index] < data[(index-1)/2])
        {
            T temp = data[index];
            data[index] = data[(index-1)/2];
            data[(index-1)/2] = temp;
            index = (index-1)/2;
        }
    }
    T PopMin()
    {
        if (data.size() == 0)
            return T();

        T hold = data[0];
        data[0] = data[data.size()-1];
        data.pop_back();
        int index = 0;
        while (index < data.size())
        {
            //child nodes of current index are out of the heap's range
            if ((index*2)+1 >= data.size())
                break;
            int minInd = 0;
            T* min = &(data[index]);
            if (data[(index*2)+1] < *min)
            {
                min = &(data[(index*2)+1]);
                minInd = 1;
            }
            if ((index*2)+2 < data.size()) //right child node is inside heap's range
            {
                if (data[(index*2)+2] < *min)
                {
                    min = &(data[(index*2)+2]);
                    minInd = 2;
                }
            }
            if (minInd == 0) //current index is smaller than it's children, min heap is restored
                break;
            else if (minInd == 1) //left child is smallest, swap current index with left child
            {
                T temp = data[index];
                data[index] = data[(index*2)+1];
                data[(index*2)+1] = temp;
                index = (index*2)+1;
            }
            else if (minInd == 2) //right child is smallest, swap current index with left child
            {
                T temp = data[index];
                data[index] = data[(index*2)+2];
                data[(index*2)+2] = temp;
                index = (index*2)+2;
            }
        }
        return hold;
    }
};

#endif