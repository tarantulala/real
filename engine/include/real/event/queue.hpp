
#include <algorithm>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>

using namespace std;

class non_empty_queue : public std::exception {
  std::string what_;
 public:
  explicit non_empty_queue(std::string msg) { what_ = std::move(msg); }
  const char* what() const noexcept override  { return what_.c_str(); }
};

template<typename T>
class ThreadsafeQueue {
  std::queue<T> queue_;
  mutable std::mutex mutex_;

  // Moved out of public interface to prevent races between this
  // and pop().
  [[nodiscard]] bool empty() const {
    return queue_.empty();
  }

 public:
  ThreadsafeQueue() = default;
  ThreadsafeQueue(const ThreadsafeQueue<T> &) = delete ;
  ThreadsafeQueue& operator=(const ThreadsafeQueue<T> &) = delete ;

  ThreadsafeQueue(ThreadsafeQueue<T>&& other) noexcept(false) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!empty()) {
      throw non_empty_queue("Moving into a non-empty queue"s);
    }
    queue_ = std::move(other.queue_);
  }

  virtual ~ThreadsafeQueue() noexcept(false) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!empty()) {
      throw non_empty_queue("Destroying a non-empty queue"s);
    }
  }

  [[nodiscard]] unsigned long size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
  }

  std::optional<T> pop() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty()) {
      return {};
    }
    T tmp = queue_.front();
    queue_.pop();
    return tmp;
  }

  void push(const T &item) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(item);
  }
};


void FillQueue(int from, int to, ThreadsafeQueue<int> &q) {

  auto start = std::chrono::system_clock::now();
  for (int i = from; i < to; ++i) {
    q.push(i);
    std::this_thread::sleep_for(10us);
  }
  auto runtime = std::chrono::system_clock::now() - start;
  cout << "FillQueue thread took "
       << (std::chrono::duration_cast<std::chrono::microseconds>(runtime)).count()
       << " µsec\n";
}

// NOTE: `flags` is used only by ONE thread at a time; this is not
//   where the problem is.
std::vector<bool> flags(30, false);

void FlushQueue(ThreadsafeQueue<int> &q, int *count) {
  std::this_thread::sleep_for(100us);

  optional<int> jOpt = q.pop();
  while (jOpt) {
    int j = *jOpt;
    if (flags[j]) {
      cout << "We've already been here: " << j << endl;
      return;
    }
    flags[j] = true;
    (*count)++;
    jOpt = q.pop();
    if (!jOpt) {
      std::this_thread::sleep_for(100us);
      jOpt = q.pop();
    }
  }
}

int main() {

  ThreadsafeQueue<int> q;
  int num_elems = 0;

  std::vector<std::thread> threads;
  threads.push_back(
      std::thread(FillQueue, 0, 10, std::ref(q)));
  threads.push_back(
      std::thread(FillQueue, 10, 15, std::ref(q)));
  threads.push_back(
      std::thread(FillQueue, 15, 30, std::ref(q)));

  std::thread flush(FlushQueue, std::ref(q), &num_elems);

  cout << "Threads started, waiting for them to complete...\n";
  flush.join();
  std::for_each(threads.begin(), threads.end(),
                std::mem_fn(&std::thread::join));

  cout << "We processed " << num_elems << " elements" << endl;
  cout << "After running the threads the Q has " << q.size() << " elements left" << endl;

  for (auto f : flags) {
    if (!f) {
      cout << "ERROR: we missed one\n";
    }
  }
  return EXIT_SUCCESS;
}
