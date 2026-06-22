#include <iostream>
#include <string>
#include <iomanip>
#include "../lib/algoracoon.hpp"
#include <locale>

#ifdef _WIN32
#include <windows.h>
#endif

struct PageTableEntry {
    int virtualAddr;
    int physicalAddr;

    bool operator<(const PageTableEntry& other) const {
        return virtualAddr < other.virtualAddr;
    }
};


struct MemoryCell {
    int address;
    std::string data;
    bool operator<(const MemoryCell& other) const {
        return address < other.address;
    }
};

struct MemoryRequest {
    char type; 
    int virtualAddr;
    std::string value; 
};

struct AddressStats {
    int physicalAddr;
    int hitCount;
    int missCount;

    int totalAccess() const { return hitCount + missCount; }
};

struct StatsDescending {
    bool operator()(const AddressStats& a, const AddressStats& b) const {
        return a.totalAccess() > b.totalAccess();
    }
};

struct CacheBlockFreq {
    int physicalAddr;
    int frequency;
};

struct MinFrequencyComp {
    bool operator()(const CacheBlockFreq& a, const CacheBlockFreq& b) const {
        return a.frequency > b.frequency; 
    }
};

class CacheSimulator {
private:

    algoracoon::BST<PageTableEntry> pageTable_;                      
    algoracoon::AVL<MemoryCell> mainMemory_;                       
    algoracoon::HashTable<int, std::string> cacheStore_;            
    algoracoon::LinkedList<int> lruList_;                          
    algoracoon::Queue<MemoryRequest> requestQueue_;                 
    algoracoon::Stack<std::string> logStack_;                       
    

    std::size_t cacheCapacity_;
    int totalHits_;
    int totalMisses_;
    AddressStats statsArray_[100];
    std::size_t statsCount_;
    algoracoon::HashTable<int, int> freqMap_;

    AddressStats* getOrCreateStats(int physAddr) {
        for (std::size_t i = 0; i < statsCount_; ++i) {
            if (statsArray_[i].physicalAddr == physAddr) {
                return &statsArray_[i];
            }
        }
        if (statsCount_ < 100) {
            statsArray_[statsCount_] = {physAddr, 0, 0};
            return &statsArray_[statsCount_++];
        }
        return nullptr;
    }

    void addLog(const std::string& log) {
        logStack_.push(log);
    }

    void incrementFreq(int physAddr) {
        if (freqMap_.contains(physAddr)) {
            freqMap_[physAddr] = freqMap_[physAddr] + 1;
        } else {
            freqMap_.insert(physAddr, 1);
        }
    }

    int getFreq(int physAddr) const {
        const int* val = freqMap_.find(physAddr);
        return val ? *val : 0;
    }

public:
    CacheSimulator(std::size_t cacheCapacity = 4)
        : cacheCapacity_(cacheCapacity), totalHits_(0), totalMisses_(0), statsCount_(0) {
        for (int i = 0; i < 8; ++i) {
            pageTable_.insert({100 + i, 10 + i});
            mainMemory_.insert({10 + i, "Data_Block_" + std::to_string(i)});
        }
    }

    void enqueueRequest(char type, int vAddr, const std::string& val = "") {
        requestQueue_.enqueue({type, vAddr, val});
        addLog("CPU: Đã nhận yêu cầu " + std::string(1, type) + " tại Địa chỉ ảo " + std::to_string(vAddr));
    }
    void step() {
        if (requestQueue_.empty()) {
            addLog("System: Không có yêu cầu nào trong hàng đợi.");
            return;
        }

        MemoryRequest req = requestQueue_.dequeue();
        PageTableEntry dummyPt{req.virtualAddr, 0};
        PageTableEntry* ptEntry = pageTable_.find(dummyPt);

        if (!ptEntry) {
            addLog("ERROR: Lỗi trang (Page Fault) - Địa chỉ ảo " + std::to_string(req.virtualAddr) + " không tồn tại!");
            return;
        }

        int physAddr = ptEntry->physicalAddr;
        AddressStats* stats = getOrCreateStats(physAddr);
        std::string* cachedData = cacheStore_.find(physAddr);

        if (cachedData) {
            totalHits_++;
            if (stats) stats->hitCount++;
            incrementFreq(physAddr);
            int idx = lruList_.find(physAddr);
            if (idx != -1) {
                lruList_.removeAt(idx);
            }
            lruList_.insertBack(physAddr);

            if (req.type == 'W') {
                *cachedData = req.value;
                addLog("CACHE HIT (Write): Địa chỉ vật lý " + std::to_string(physAddr) + " ghi đè thành \"" + req.value + "\"");
            } else {
                addLog("CACHE HIT (Read): Địa chỉ vật lý " + std::to_string(physAddr) + " trả về \"" + *cachedData + "\"");
            }
        } else {
            totalMisses_++;
            if (stats) stats->missCount++;
            MemoryCell dummyMem{physAddr, ""};
            MemoryCell* memCell = mainMemory_.find(dummyMem);

            if (!memCell) {
                addLog("ERROR: Không tìm thấy Địa chỉ vật lý " + std::to_string(physAddr) + " trong Main Memory!");
                return;
            }

            std::string fetchedData = memCell->data;
            incrementFreq(physAddr);
            std::string evictionMsg = "";
            if (cacheStore_.size() >= cacheCapacity_) {
                int victimAddr = lruList_.front();
                lruList_.removeAt(0);
                cacheStore_.remove(victimAddr);
                evictionMsg = " (Đuổi Block " + std::to_string(victimAddr) + ")";
            }
            if (req.type == 'W') {
                fetchedData = req.value; 
                memCell->data = req.value; 
            }

            cacheStore_.insert(physAddr, fetchedData);
            lruList_.insertBack(physAddr);

            addLog("CACHE MISS: Nạp Địa chỉ " + std::to_string(physAddr) + " từ Main Memory" + evictionMsg);
        }
    }
    void processAll() {
        while (!requestQueue_.empty()) {
            step();
        }
    }

    void renderUI() {
        std::cout << "\033[2J\033[H"; 
        std::cout << "========================================================================\n";
        std::cout << "               MÔ PHỎNG BỘ NHỚ CACHE (CACHE SIMULATOR)\n";
        std::cout << "========================================================================\n";
        double hitRate = 0.0;
        int totalRequests = totalHits_ + totalMisses_;
        if (totalRequests > 0) {
            hitRate = (double)totalHits_ / totalRequests * 100.0;
        }
        std::cout << " Kích thước Cache: " << cacheCapacity_ << " Blocks | "
                  << "Yêu cầu: " << totalRequests << " | "
                  << "Hits: " << totalHits_ << " | "
                  << "Misses: " << totalMisses_ << " | "
                  << "Hit Rate: " << std::fixed << std::setprecision(1) << hitRate << "%\n";
        std::cout << "------------------------------------------------------------------------\n";
        std::cout << "[Trạng thái Cache hiện tại (Thứ tự LRU từ trái qua phải)]\n";
        if (lruList_.empty()) {
            std::cout << " (Trống)\n";
        } else {
            std::cout << " LRU (Cũ nhất) -> ";
            lruList_.forEachForward([](const int& addr) {
                std::cout << "[Phys:" << addr << "] -> ";
            });
            std::cout << "MRU (Mới nhất)\n";
            
            std::cout << " Nội dung chi tiết:\n";
            lruList_.forEachForward([this](const int& addr) {
                std::string* val = cacheStore_.find(addr);
                if (val) {
                    std::cout << "  - Địa chỉ vật lý " << std::setw(2) << addr 
                              << " | Tần suất: " << std::setw(2) << getFreq(addr)
                              << " | Dữ liệu: \"" << *val << "\"\n";
                }
            });
        }
        std::cout << "------------------------------------------------------------------------\n";

        std::cout << "[Hàng đợi yêu cầu từ CPU (Queue)]\n";
        if (requestQueue_.empty()) {
            std::cout << " (Trống)\n";
        } else {
            std::cout << " [Đầu hàng đợi] <- ";
            algoracoon::Queue<MemoryRequest> tempQueue = requestQueue_;
            while (!tempQueue.empty()) {
                MemoryRequest r = tempQueue.dequeue();
                std::cout << "(" << r.type << " V:" << r.virtualAddr << ") <- ";
            }
            std::cout << "[Cuối hàng đợi]\n";
        }
        std::cout << "------------------------------------------------------------------------\n";
        std::cout << "[Nhật ký hoạt động (Stack - Gần đây nhất lên trên)]\n";
        if (logStack_.empty()) {
            std::cout << " (Trống)\n";
        } else {
            algoracoon::Stack<std::string> tempStack = logStack_;
            int logCount = 0;
            while (!tempStack.empty() && logCount < 5) {
                std::cout << "  * " << tempStack.pop() << "\n";
                logCount++;
            }
        }
        std::cout << "========================================================================\n";
    }
    void printStatisticsReport() {
        if (statsCount_ == 0) {
            std::cout << "Chưa có dữ liệu thống kê.\n";
            return;
        }
        AddressStats* sortedStats = new AddressStats[statsCount_];
        for (std::size_t i = 0; i < statsCount_; ++i) {
            sortedStats[i] = statsArray_[i];
        }
        algoracoon::bubbleSort(sortedStats, statsCount_, StatsDescending());

        std::cout << "\n================ BÁO CÁO PHÂN TÍCH TRUY CẬP ================\n";
        std::cout << " (Sắp xếp giảm dần theo tổng lượt truy cập bằng Bubble Sort)\n\n";
        std::cout << " Địa chỉ vật lý | Lượt Hits | Lượt Misses | Tổng truy cập\n";
        std::cout << "--------------------------------------------------------\n";
        for (std::size_t i = 0; i < statsCount_; ++i) {
            std::cout << "       " << std::setw(8) << sortedStats[i].physicalAddr
                      << " | " << std::setw(9) << sortedStats[i].hitCount
                      << " | " << std::setw(11) << sortedStats[i].missCount
                      << " | " << std::setw(12) << sortedStats[i].totalAccess() << "\n";
        }
        std::cout << "============================================================\n";
        delete[] sortedStats;
    }
    void printEvictionPriority() {
        if (lruList_.empty()) {
            std::cout << "Cache hiện đang trống.\n";
            return;
        }
        algoracoon::PriorityQueue<CacheBlockFreq, MinFrequencyComp> pq;
        lruList_.forEachForward([this, &pq](const int& addr) {
            pq.insert({addr, getFreq(addr)});
        });

        std::cout << "\n============ ĐỘ ƯU TIÊN GIẢI PHÓNG BỘ NHỚ CACHE (LFU View) ============\n";
        std::cout << " (Các block có tần suất truy cập thấp nhất sẽ ở đầu danh sách giải phóng)\n\n";
        std::cout << " Thứ tự giải phóng | Địa chỉ vật lý | Tần suất truy cập hiện tại\n";
        std::cout << "----------------------------------------------------------------------\n";
        
        int order = 1;
        while (!pq.empty()) {
            CacheBlockFreq block = pq.extract();
            std::cout << "        " << std::setw(10) << order++
                      << " | " << std::setw(14) << block.physicalAddr
                      << " | " << std::setw(26) << block.frequency << "\n";
        }
        std::cout << "======================================================================\n";
    }
};

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    CacheSimulator sim(5); 

    int choice = 0;
    while (true) {
        sim.renderUI();
        std::cout << " MENU ĐIỀU KHIỂN:\n";
        std::cout << " 1. Thêm yêu cầu Đọc (Read) địa chỉ ảo mới\n";
        std::cout << " 2. Thêm yêu cầu Ghi (Write) dữ liệu mới vào địa chỉ ảo\n";
        std::cout << " 3. Chạy từng bước yêu cầu kế tiếp (Step)\n";
        std::cout << " 4. Chạy toàn bộ yêu cầu trong hàng đợi (Process All)\n";
        std::cout << " 5. Xem báo cáo phân tích tần suất truy cập (Bubble Sort)\n";
        std::cout << " 6. Xem độ ưu tiên giải phóng bộ nhớ (Priority Queue - LFU)\n";
        std::cout << " 7. Thoát\n";
        std::cout << " Lựa chọn của bạn: ";
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 7) break;

        switch (choice) {
            case 1: {
                int vAddr;
                std::cout << " Nhập địa chỉ ảo để đọc (ví dụ: 100 - 107): ";
                std::cin >> vAddr;
                sim.enqueueRequest('R', vAddr);
                break;
            }
            case 2: {
                int vAddr;
                std::string val;
                std::cout << " Nhập địa chỉ ảo để ghi (ví dụ: 100 - 107): ";
                std::cin >> vAddr;
                std::cout << " Nhập nội dung dữ liệu cần ghi: ";
                std::cin.ignore();
                std::getline(std::cin, val);
                sim.enqueueRequest('W', vAddr, val);
                break;
            }
            case 3:
                sim.step();
                break;
            case 4:
                sim.processAll();
                break;
            case 5:
                sim.printStatisticsReport();
                std::cout << " Nhấn phím Enter để tiếp tục...";
                std::cin.ignore();
                std::cin.get();
                break;
            case 6:
                sim.printEvictionPriority();
                std::cout << " Nhấn phím Enter để tiếp tục...";
                std::cin.ignore();
                std::cin.get();
                break;
            default:
                break;
        }
    }

    return 0;
}