#include <iostream>
const int INF = 1e9;
int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};
struct lift {
    int startX,startY;
    int destX,destY;
    int travelTime;
    int interval;
};
struct vertex {
    int x, y, height;
};
struct timeVertex {
    int x, y;
    int time;
    bool operator<(const timeVertex& other) const {
        return time > other.time;
    }
};
struct PriorityQueue {
    timeVertex* heap;
    int capacity;
    int size;

    PriorityQueue() {
        capacity = 10;
        heap = new timeVertex[capacity];
        size = 0;
    }

    ~PriorityQueue() {
        delete[] heap;
    }
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }
    void heapifyDown(int index) {
        while (true) {
            int smallest = index;
            int left = leftChild(index);
            int right = rightChild(index);
            if (left < size && heap[left].time < heap[smallest].time)
                smallest = left;
            if (right < size && heap[right].time < heap[smallest].time)
                smallest = right;
            if (smallest == index) break;
            std::swap(heap[index], heap[smallest]);
            index = smallest;
        }
    }
    void heapifyUp(int index) {
        timeVertex item = heap[index];
        while (index > 0) {
            int parentIdx = parent(index);
            if (heap[parentIdx].time <= item.time) break;

            heap[index] = heap[parentIdx];
            index = parentIdx;
        }
        heap[index] = item;
    }

    void resize() {
        int newCapacity = capacity * 2 + 16;
        timeVertex* newHeap = new timeVertex[newCapacity];
        std::copy(heap, heap + size, newHeap);

        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
    }

    void push(timeVertex s) {
        if (size == capacity) {
            resize();
        }

        heap[size] = s;
        heapifyUp(size);
        size++;
    }

    bool pop(timeVertex& result) {
        if (size == 0) return false;

        result = heap[0];
        heap[0] = heap[--size];

        if (size > 1) {
            heapifyDown(0);
        }

        return true;
    }

    bool empty() {
        return size == 0;
    }

    void clear() {
        size = 0;
    }
};
using namespace std;
/*A - B + 1 minut, jeĹĽeli A > B,
1 minutÄ™, jeĹĽeli A â‰¤ B.*/
int calculateMoveCost(int currHeight, int nextHeight, int x,int y) {
    int cost;
    if (nextHeight > currHeight) {
        cost = (nextHeight - currHeight) + 1;
    } else {
        cost = 1;
    }
    return cost;
}
int calculateLiftWaitTime(int currentTime, int interval) {
    if (interval == 0) return INF;
    int wait = (interval - (currentTime % interval)) % interval;
    return wait;
}
int getMinTime(vertex** map, lift** lift_data, int start_x, int start_y, int target_x, int target_y, int width, int height, int lift_num) {
    // INIT TIME ARRAY
    int** timeArr = new int*[height];
    for (int i = 0; i < height; i++) {
        timeArr[i] = new int[width];
        for (int j = 0; j < width; j++) {
            timeArr[i][j] = INF;
        }
    }

    //count how many lifts start at each position
    int** liftCount = new int*[height];
    for (int i = 0; i < height; i++) {
        liftCount[i] = new int[width]();
    }
    
    for (int i = 0; i < lift_num; i++) {
        int startX = lift_data[i]->startX;
        int startY = lift_data[i]->startY;
        liftCount[startY][startX]++;
    }
    
    //lookup table
    int*** liftMap = new int**[height];
    for (int i = 0; i < height; i++) {
        liftMap[i] = new int*[width];
        for (int j = 0; j < width; j++) {
            //allocating memory if there are lifts at this position
            if (liftCount[i][j] > 0) {
                liftMap[i][j] = new int[liftCount[i][j]];
            } else {
                liftMap[i][j] = nullptr;  // No lifts
            }
        }
    }
    
    // Reset counts
    int** currentCount = new int*[height];
    for (int i = 0; i < height; i++) {
        currentCount[i] = new int[width](); 
    }
    
    for (int i = 0; i < lift_num; i++) {
        int startX = lift_data[i]->startX;
        int startY = lift_data[i]->startY;
        if (liftCount[startY][startX] > 0) {
            liftMap[startY][startX][currentCount[startY][startX]++] = i;
        }
    }
    
    for (int i = 0; i < height; i++) {
        delete[] currentCount[i];
    }
    delete[] currentCount;

    PriorityQueue pq;
    timeVertex startTimeVertex = {start_x, start_y, 0};
    pq.push(startTimeVertex);
    timeArr[start_y][start_x] = 0;

    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {-1, 0, 1, 0};

    while (!pq.empty()) {
        timeVertex current;
        pq.pop(current);

        int currTime = current.time;
        int x = current.x;
        int y = current.y;

        // Skip if we've found a better path already
        if (currTime > timeArr[y][x]) continue;

        if (x == target_x && y == target_y) {
            int result = timeArr[target_y][target_x];
            // Clean up memory
            for (int i = 0; i < height; i++) {
                delete[] timeArr[i];
                for (int j = 0; j < width; j++) {
                    if (liftCount[i][j] > 0 && liftMap[i][j] != nullptr) {
                        delete[] liftMap[i][j];
                    }
                }
                delete[] liftMap[i];
                delete[] liftCount[i];
            }
            delete[] timeArr;
            delete[] liftMap;
            delete[] liftCount;

            return result;
        }

        // Try moving in each direction
        for (int dir = 0; dir < 4; dir++) {
            int newX = x + dx[dir];
            int newY = y + dy[dir];

            // Check map boundaries
            if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
                int moveCost = calculateMoveCost(map[y][x].height, map[newY][newX].height, newX, newY);
                int newTime = currTime + moveCost;

                // Update if we found a better path
                if (newTime < timeArr[newY][newX]) {
                    timeArr[newY][newX] = newTime;
                    pq.push({newX, newY, newTime});
                }
            }
        }

        // Check for lifts at current position 
        int count = liftCount[y][x];
        if (count > 0 && liftMap[y][x] != nullptr) {
            for (int i = 0; i < count; i++) {
                int liftIndex = liftMap[y][x][i];
                int waitTime = calculateLiftWaitTime(currTime, lift_data[liftIndex]->interval);
                int newTime = currTime + waitTime + lift_data[liftIndex]->travelTime;
                int destX = lift_data[liftIndex]->destX;
                int destY = lift_data[liftIndex]->destY;

                // Update if found a better path
                if (newTime < timeArr[destY][destX]) {
                    timeArr[destY][destX] = newTime;
                    pq.push({destX, destY, newTime});
                }
            }
        }
    }

    // if target not reached
    for (int i = 0; i < height; i++) {
        delete[] timeArr[i];
        for (int j = 0; j < width; j++) {
            if (liftCount[i][j] > 0 && liftMap[i][j] != nullptr) {
                delete[] liftMap[i][j];
            }
        }
        delete[] liftMap[i];
        delete[] liftCount[i];
    }
    delete[] timeArr;
    delete[] liftMap;
    delete[] liftCount;
    
    return INF;
}
void freeMemory(vertex**map,lift** lift_data,int height, int lift_num) {
    for (int i = 0; i < height; i++) {
        delete[] map[i];
    }
    delete[] map;
    for (int i = 0; i < lift_num; i++) {
        delete[] lift_data[i];
    }
    delete[] lift_data;
}
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int width, height;
    int start_x, start_y;
    int target_x, target_y;
    int lift_num;
    cin >> width >> height >> start_x >> start_y >> target_x >> target_y >> lift_num;

    vertex**map = new vertex*[height];
    lift** lift_data = new lift*[lift_num];

    for (int i = 0; i < lift_num; i++) {
        lift_data[i] = new lift;
        cin>>lift_data[i]->startX>>lift_data[i]->startY;
        cin>>lift_data[i]->destX>>lift_data[i]->destY;
        cin>>lift_data[i]->travelTime;
        cin>>lift_data[i]->interval;
    }
    for (int i = 0; i < height; i++) {
        map[i] = new vertex[width];
        for (int j = 0; j < width; j++) {
            map[i][j].x = j;
            map[i][j].y = i;
            cin >>map[i][j].height;
        }
    }
    int result = getMinTime(map,lift_data,start_x,start_y,target_x,target_y,width,height,lift_num);
    cout << result << endl;
    freeMemory(map,lift_data,height,lift_num);
}